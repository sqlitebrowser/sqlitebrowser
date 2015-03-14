#include "sqlitetablemodel.h"
#include "sqlitedb.h"
#include "sqlite.h"

#include "PreferencesDialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QApplication>

SqliteTableModel::SqliteTableModel(QObject* parent, DBBrowserDB* db, size_t chunkSize)
    : QAbstractTableModel(parent)
    , m_db(db)
    , m_rowCount(0)
    , m_chunkSize(chunkSize)
    , m_valid(false)
{
    reset();
}

void SqliteTableModel::reset()
{
    m_iSortColumn = 0;
    m_sSortOrder = "ASC";
    m_headers.clear();
    m_mWhere.clear();
}

void SqliteTableModel::setChunkSize(size_t chunksize)
{
    m_chunkSize = chunksize;
}

void SqliteTableModel::setTable(const QString& table)
{
    reset();

    m_sTable = table;

    QString sColumnQuery = QString::fromUtf8("SELECT * FROM `%1`;").arg(table);
    if(m_db->getObjectByName(table).gettype() == "table")
    {
        sqlb::Table t = sqlb::Table::parseSQL(m_db->getObjectByName(table).getsql()).first;
        if(t.fields().size()) // parsing was OK
        {
            m_headers.push_back(t.rowidColumn());
            m_headers.append(m_db->getObjectByName(table).table.fieldNames());
        }
        else
        {
            m_headers.push_back("rowid");
            m_headers.append(getColumns(sColumnQuery));
        }
    }
    else
    {
        m_headers.push_back("rowid");
        m_headers.append(getColumns(sColumnQuery));
    }

    buildQuery();
}

namespace {
QString rtrimChar(const QString& s, QChar c) {
    QString r = s.trimmed();
    while(r.endsWith(c))
        r.chop(1);
    return r;
}

QString removeComments(QString s)
{
    // Feel free to fix all the bugs this probably contains or just replace this function entirely by
    // a 'simple' regular expression. I know there're better ways to do this...

    // This function removes any single line comments (starting with '--') from a given string. It does
    // so by going through the string character by character and trying to keep track of wether we currently
    // are in a string or identifier and only removing those parts starting with '--' which are in neither.

    QChar lastChar = 0;
    QList<QChar> stringChars;
    for(int i=0; i < s.length(); ++i)
    {
        if(lastChar != '\\' && (s.at(i) == '\'' || s.at(i) == '"' || s.at(i) == '`'))
        {
            if(!stringChars.empty() && stringChars.last() == s.at(i))
                stringChars.removeLast();
            else if(!(!stringChars.empty() && (stringChars.last() != '\'' || stringChars.last() != '"')) || stringChars.empty())
                stringChars.push_back(s.at(i));
        } else if(stringChars.empty() && s.at(i) == '-' && lastChar == '-') {
            int nextNL = s.indexOf('\n', i);
            if(nextNL >= 0)
                return removeComments(s.remove(i-1, nextNL - i + 2));
            else
                return s.left(i-1);
        }

        lastChar = s.at(i);
    }

    return s;
}
}

void SqliteTableModel::setQuery(const QString& sQuery, bool dontClearHeaders)
{
    // clear
    if(!dontClearHeaders)
        reset();

    if(!m_db->isOpen())
        return;

    m_sQuery = removeComments(sQuery).trimmed();

    // do a count query to get the full row count in a fast manner
    m_rowCount = getQueryRowCount();
    if(m_rowCount == -1)
    {
        m_valid = false;
        return;
    }

    // headers
    if(!dontClearHeaders)
    {
        m_headers.append(getColumns(sQuery));
    }

    // now fetch the first entries
    clearCache();
    fetchData(0, m_chunkSize);
    m_valid = true;

    emit layoutChanged();
}

int SqliteTableModel::getQueryRowCount()
{
    // Return -1 if there is an error
    int retval = -1;

    // Use a different approach of determining the row count when a EXPLAIN or a PRAGMA statement is used because a COUNT fails on these queries
    if(m_sQuery.startsWith("EXPLAIN", Qt::CaseInsensitive) || m_sQuery.startsWith("PRAGMA", Qt::CaseInsensitive))
    {
        // So just execute the statement as it is and fetch all results counting the rows
        sqlite3_stmt* stmt;
        QByteArray utf8Query = m_sQuery.toUtf8();
        if(sqlite3_prepare_v2(m_db->_db, utf8Query, utf8Query.size(), &stmt, NULL) == SQLITE_OK)
        {
            retval = 0;
            while(sqlite3_step(stmt) == SQLITE_ROW)
                retval++;
            sqlite3_finalize(stmt);

            // Return the results but also set the chunk size the number of rows to prevent the lazy population mechanism to kick in as using LIMIT
            // fails on this kind of queries as well
            m_chunkSize = retval;
            return retval;
        }
    } else {
        // If it is a normal query - hopefully starting with SELECT - just do a COUNT on it and return the results
        QString sCountQuery = QString("SELECT COUNT(*) FROM (%1);").arg(rtrimChar(m_sQuery, ';'));
        m_db->logSQL(sCountQuery, kLogMsg_App);
        QByteArray utf8Query = sCountQuery.toUtf8();

        sqlite3_stmt* stmt;
        int status = sqlite3_prepare_v2(m_db->_db, utf8Query, utf8Query.size(), &stmt, NULL);
        if(status == SQLITE_OK)
        {
            status = sqlite3_step(stmt);
            if(status == SQLITE_ROW)
            {
                QString sCount = QString::fromUtf8((const char*)sqlite3_column_text(stmt, 0));
                retval = sCount.toInt();
            }
            sqlite3_finalize(stmt);
        } else {
            qWarning() << "Count query failed: " << sCountQuery;
        }
    }

    return retval;
}

int SqliteTableModel::rowCount(const QModelIndex&) const
{
    return m_data.size(); // current fetched row count
}

int SqliteTableModel::totalRowCount() const
{
    return m_rowCount;
}

int SqliteTableModel::columnCount(const QModelIndex&) const
{
    return m_headers.size();
}

QVariant SqliteTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        // if we have a VIRTUAL table the model will not be valid, with no header data
        if(section < m_headers.size())
            return m_headers.at(section);

        return QString("%1").arg(section + 1);
    }
    else
        return QString("%1").arg(section + 1);
}

QVariant SqliteTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_rowCount)
        return QVariant();

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        // If this row is not in the cache yet get it first
        while(index.row() >= m_data.size() && canFetchMore())
            const_cast<SqliteTableModel*>(this)->fetchMore();   // Nothing evil to see here, move along

        if(role == Qt::DisplayRole && isBinary(index))
            return "BLOB";
        else if(role == Qt::DisplayRole && m_data.at(index.row()).at(index.column()).isNull())
            return PreferencesDialog::getSettingsValue("databrowser", "null_text").toString();
        else
            return m_data.at(index.row()).at(index.column());
    } else if(role == Qt::FontRole) {
        QFont font;
        if(m_data.at(index.row()).at(index.column()).isNull() || isBinary(index))
            font.setItalic(true);
        return font;
    } else if(role == Qt::TextColorRole) {
        if(m_data.at(index.row()).at(index.column()).isNull())
            return QColor(PreferencesDialog::getSettingsValue("databrowser", "null_fg_colour").toString());
        else if (isBinary(index))
            return QColor(Qt::gray);
        return QVariant();
    } else if (role == Qt::BackgroundRole) {
        if(m_data.at(index.row()).at(index.column()).isNull())
            return QColor(PreferencesDialog::getSettingsValue("databrowser", "null_bg_colour").toString());
        return QVariant();
    } else {
        return QVariant();
    }
}

bool SqliteTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        QByteArray newValue = value.toByteArray();
        QByteArray oldValue = m_data.at(index.row()).at(index.column());

        // Don't do anything if the data hasn't changed
        // To differentiate NULL and empty byte arrays, we also compare the NULL flag
        if(oldValue == newValue && oldValue.isNull() == newValue.isNull())
            return true;

        if(m_db->updateRecord(m_sTable, m_headers.at(index.column()), m_data[index.row()].at(0).toLongLong(), newValue))
        {
            // Only update the cache if this row has already been read, if not there's no need to do any changes to the cache
            if(index.row() < m_data.size())
                m_data[index.row()].replace(index.column(), newValue);

            emit(dataChanged(index, index));
            return true;
        } else {
            QMessageBox::warning(0, qApp->applicationName(), tr("Error changing data:\n%1").arg(m_db->lastErrorMessage));
            return false;
        }
    }

    return false;
}

bool SqliteTableModel::canFetchMore(const QModelIndex&) const
{
    return m_data.size() < m_rowCount;
}

void SqliteTableModel::fetchMore(const QModelIndex&)
{
    int row = m_data.size();
    fetchData(row, row + m_chunkSize);
}

Qt::ItemFlags SqliteTableModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::ItemIsEnabled;

    Qt::ItemFlags ret = QAbstractTableModel::flags(index);
    if(!isBinary(index))
        ret |= Qt::ItemIsEditable;
    return ret;
}

void SqliteTableModel::sort(int column, Qt::SortOrder order)
{
    // Don't do anything when the sort order hasn't changed
    if(m_iSortColumn == column && m_sSortOrder == (order == Qt::AscendingOrder ? "ASC" : "DESC"))
        return;

    // Save sort order
    m_iSortColumn = column;
    m_sSortOrder = (order == Qt::AscendingOrder ? "ASC" : "DESC");

    // Set the new query (but only if a table has already been set
    if(m_sTable != "")
        buildQuery();
}

bool SqliteTableModel::insertRows(int row, int count, const QModelIndex& parent)
{
    beginInsertRows(parent, row, row + count - 1);

    QByteArrayList blank_data;
    for(int i=0; i < m_headers.size(); ++i)
        blank_data.push_back("");

    for(int i=row; i < row + count; ++i)
    {
        qint64 rowid = m_db->addRecord(m_sTable);
        if(rowid < 0)
        {
            endInsertRows();
            return false;
        }
        m_rowCount++;
        m_data.insert(i, blank_data);
        m_data[i].replace(0, QByteArray::number(rowid));

        // update column with default values
        QByteArrayList rowdata;
        if( m_db->getRow(m_sTable, rowid, rowdata) )
        {
            for(int j=1; j < m_headers.size(); ++j)
            {
                m_data[i].replace(j, rowdata[j - 1]);
            }
        }
    }

    endInsertRows();
    return true;
}

bool SqliteTableModel::removeRows(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for(int i=count-1;i>=0;i--)
    {
        m_db->deleteRecord(m_sTable, m_data.at(row + i).at(0).toLongLong());
        m_data.removeAt(row + i);
    }

    m_rowCount -= count;

    endRemoveRows();
    return true;
}

void SqliteTableModel::fetchData(unsigned int from, unsigned to)
{
    int currentsize = m_data.size();

    QString sLimitQuery;
    if(m_sQuery.startsWith("PRAGMA", Qt::CaseInsensitive) || m_sQuery.startsWith("EXPLAIN", Qt::CaseInsensitive))
    {
        sLimitQuery = m_sQuery;
    } else {
        // Remove trailing trailing semicolon
        QString queryTemp = rtrimChar(m_sQuery, ';');

        // If the query ends with a LIMIT statement take it as it is, if not append our own LIMIT part for lazy population
        if(queryTemp.contains(QRegExp("LIMIT\\s+.+\\s*((,|\\b(OFFSET)\\b)\\s*.+\\s*)?$", Qt::CaseInsensitive)))
            sLimitQuery = queryTemp;
        else
            sLimitQuery = queryTemp + QString(" LIMIT %1, %2;").arg(from).arg(to-from);
    }
    m_db->logSQL(sLimitQuery, kLogMsg_App);
    QByteArray utf8Query = sLimitQuery.toUtf8();
    sqlite3_stmt *stmt;
    int status = sqlite3_prepare_v2(m_db->_db, utf8Query, utf8Query.size(), &stmt, NULL);

    if(SQLITE_OK == status)
    {
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            QByteArrayList rowdata;
            for (int i = 0; i < m_headers.size(); ++i)
            {
                if(sqlite3_column_type(stmt, i) == SQLITE_NULL)
                {
                    rowdata.append(QByteArray());
                } else {
                    int bytes = sqlite3_column_bytes(stmt, i);
                    if(bytes)
                        rowdata.append(QByteArray(static_cast<const char*>(sqlite3_column_blob(stmt, i)), bytes));
                    else
                        rowdata.append(QByteArray(""));
                }
            }
            m_data.push_back(rowdata);
        }
    }
    sqlite3_finalize(stmt);

    beginInsertRows(QModelIndex(), currentsize, m_data.size()-1);
    endInsertRows();
}

void SqliteTableModel::buildQuery()
{
    QString where;

    if(m_mWhere.size())
    {
        where = "WHERE 1=1";

        for(QMap<int, QString>::const_iterator i=m_mWhere.constBegin();i!=m_mWhere.constEnd();++i)
            where.append(QString(" AND `%1` %2").arg(m_headers.at(i.key())).arg(i.value()));
    }

    QString sql = QString("SELECT `%1`,* FROM `%2` %3 ORDER BY `%4` %5").arg(m_headers.at(0)).arg(m_sTable).arg(where).arg(m_headers.at(m_iSortColumn)).arg(m_sSortOrder);
    setQuery(sql, true);
}

QStringList SqliteTableModel::getColumns(const QString sQuery)
{
    sqlite3_stmt* stmt;
    QByteArray utf8Query = sQuery.toUtf8();
    int status = sqlite3_prepare_v2(m_db->_db, utf8Query, utf8Query.size(), &stmt, NULL);
    QStringList listColumns;
    if(SQLITE_OK == status)
    {
        status = sqlite3_step(stmt);
        int columns = sqlite3_data_count(stmt);
        for(int i = 0; i < columns; ++i)
            listColumns.append(QString::fromUtf8((const char*)sqlite3_column_name(stmt, i)));
    }
    sqlite3_finalize(stmt);

    return listColumns;
}

void SqliteTableModel::updateFilter(int column, const QString& value)
{
    // Check for any special comparison operators at the beginning of the value string. If there are none default to LIKE.
    QString op = "LIKE";
    QString val;
    bool numeric = false;
    if(value.left(2) == ">=" || value.left(2) == "<=" || value.left(2) == "<>")
    {
        bool ok;
        value.mid(2).toFloat(&ok);
        if(ok)
        {
            op = value.left(2);
            val = value.mid(2);
            numeric = true;
        }
    } else if(value.left(1) == ">" || value.left(1) == "<") {
        bool ok;
        value.mid(1).toFloat(&ok);
        if(ok)
        {
            op = value.left(1);
            val = value.mid(1);
            numeric = true;
        }
    } else {
        if(value.left(1) == "=")
        {
            op = "=";
            val = value.mid(1);
        }
    }
    if(val.isEmpty())
        val = value;
    if(!numeric)
        val = QString("'%1'").arg(val.replace("'", "''"));

    // If the value was set to an empty string remove any filter for this column. Otherwise insert a new filter rule or replace the old one if there is already one
    if(val == "''")
        m_mWhere.remove(column);
    else
        m_mWhere.insert(column, QString("%1 %2").arg(op).arg(val));

    // Build the new query
    buildQuery();
}

void SqliteTableModel::clearCache()
{
    beginRemoveRows(QModelIndex(), 0, m_data.size()-1);
    m_data.clear();
    endRemoveRows();
}

bool SqliteTableModel::isBinary(const QModelIndex& index) const
{
    QByteArray val = m_data.at(index.row()).at(index.column()).left(1024);
    return val.contains('\0');     // Cheap BLOB test here...
}
