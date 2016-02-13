#include "sqlitetablemodel.h"
#include "sqlitedb.h"
#include "sqlite.h"

#include "PreferencesDialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QTextCodec>
#include <QMimeData>
#include <QFile>
#include <QUrl>

SqliteTableModel::SqliteTableModel(QObject* parent, DBBrowserDB* db, size_t chunkSize, const QString& encoding)
    : QAbstractTableModel(parent)
    , m_db(db)
    , m_rowCount(0)
    , m_chunkSize(chunkSize)
    , m_valid(false)
    , m_encoding(encoding)
{
    reset();
}

void SqliteTableModel::reset()
{
    m_iSortColumn = 0;
    m_sSortOrder = "ASC";
    m_headers.clear();
    m_mWhere.clear();
    m_vDataTypes.clear();
    m_vDisplayFormat.clear();
}

void SqliteTableModel::setChunkSize(size_t chunksize)
{
    m_chunkSize = chunksize;
}

void SqliteTableModel::setTable(const QString& table, const QVector<QString>& display_format)
{
    reset();

    m_sTable = table;
    m_vDisplayFormat = display_format;

    m_vDataTypes.push_back(SQLITE_INTEGER);

    bool allOk = false;
    if(m_db->getObjectByName(table).gettype() == "table")
    {
        sqlb::Table t = sqlb::Table::parseSQL(m_db->getObjectByName(table).getsql()).first;
        if(t.fields().size()) // parsing was OK
        {
            m_headers.push_back(t.rowidColumn());
            m_headers.append(t.fieldNames());

            // parse columns types
            static QStringList dataTypes = QStringList()
                    << "INTEGER"
                    << "REAL"
                    << "TEXT"
                    << "BLOB";
            foreach(const sqlb::FieldPtr fld,  t.fields())
            {
                QString name(fld->type().toUpper());
                int colType = dataTypes.indexOf(name);
                colType = (colType == -1) ? SQLITE_TEXT : colType + 1;
                m_vDataTypes.push_back(colType);
            }
            allOk = true;
        }
    }

    if(!allOk)
    {
        QString sColumnQuery = QString::fromUtf8("SELECT * FROM %1;").arg(sqlb::escapeIdentifier(table));
        m_headers.push_back("rowid");
        m_headers.append(getColumns(sColumnQuery, m_vDataTypes));
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
    // so by going through the string character by character and trying to keep track of whether we currently
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
        m_headers.append(getColumns(sQuery, m_vDataTypes));
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

        if(role == Qt::DisplayRole && m_data.at(index.row()).at(index.column()).left(1024).contains('\0'))
            return "BLOB";
        else if(role == Qt::DisplayRole && m_data.at(index.row()).at(index.column()).isNull())
            return PreferencesDialog::getSettingsValue("databrowser", "null_text").toString();
        else
            return decode(m_data.at(index.row()).at(index.column()));
    } else if(role == Qt::FontRole) {
        QFont font;
        if(m_data.at(index.row()).at(index.column()).isNull() || isBinary(index))
            font.setItalic(true);
        return font;
    } else if(role == Qt::TextColorRole) {
        if(m_data.at(index.row()).at(index.column()).isNull())
            return QColor(PreferencesDialog::getSettingsValue("databrowser", "null_fg_colour").toString());
        else if (isBinary(index))
            return QColor(PreferencesDialog::getSettingsValue("databrowser", "bin_fg_colour").toString());
        return QColor(PreferencesDialog::getSettingsValue("databrowser", "reg_fg_colour").toString());
    } else if (role == Qt::BackgroundRole) {
        if(m_data.at(index.row()).at(index.column()).isNull())
            return QColor(PreferencesDialog::getSettingsValue("databrowser", "null_bg_colour").toString());
        else if (isBinary(index))
            return QColor(PreferencesDialog::getSettingsValue("databrowser", "bin_bg_colour").toString());
        return QColor(PreferencesDialog::getSettingsValue("databrowser", "reg_bg_colour").toString());
    } else if(role == Qt::ToolTipRole) {
        sqlb::ForeignKeyClause fk = getForeignKeyClause(index.column()-1);
        if(fk.isSet())
            return tr("References %1(%2)\nHold Ctrl+Shift and click to jump there").arg(fk.table()).arg(fk.columns().join(","));
        else
            return QString();
    } else {
        return QVariant();
    }
}

sqlb::ForeignKeyClause SqliteTableModel::getForeignKeyClause(int column) const
{
    DBBrowserObject obj = m_db->getObjectByName(m_sTable);
    if(obj.getname().size())
        return obj.table.fields().at(column)->foreignKey();
    else
        return sqlb::ForeignKeyClause();
}

bool SqliteTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    // This function is for in-place editing.
    // So, BLOB flag is false every times.
    return setTypedData(index, false, value, role);
}

bool SqliteTableModel::setTypedData(const QModelIndex& index, bool isBlob, const QVariant& value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        QByteArray newValue = encode(value.toByteArray());
        QByteArray oldValue = m_data.at(index.row()).at(index.column());

        // Don't do anything if the data hasn't changed
        // To differentiate NULL and empty byte arrays, we also compare the NULL flag
        if(oldValue == newValue && oldValue.isNull() == newValue.isNull())
            return true;

        if(m_db->updateRecord(m_sTable, m_headers.at(index.column()), m_data[index.row()].at(0), newValue, isBlob))
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

    Qt::ItemFlags ret = QAbstractTableModel::flags(index) | Qt::ItemIsDropEnabled;

    // Custom display format set?
    bool custom_display_format = false;
    if(m_vDisplayFormat.size())
    {
        // NOTE: This assumes that custom display formats never start and end with a backtick
        if(index.column() > 0)
            custom_display_format = !(m_vDisplayFormat.at(index.column()-1).startsWith("`") && m_vDisplayFormat.at(index.column()-1).endsWith("`"));
    }

    if(!isBinary(index) && !custom_display_format)
        ret |= Qt::ItemIsEditable;
    return ret;
}

void SqliteTableModel::sort(int column, Qt::SortOrder order)
{
    // Don't do anything when the sort order hasn't changed
    if(m_iSortColumn == column && m_sSortOrder == (order == Qt::AscendingOrder ? "ASC" : "DESC"))
        return;

    // Save sort order
	if (column >= 0 && column < m_headers.size())
		m_iSortColumn = column;
    m_sSortOrder = (order == Qt::AscendingOrder ? "ASC" : "DESC");

    // Set the new query (but only if a table has already been set
    if(m_sTable != "")
        buildQuery();
}

bool SqliteTableModel::insertRows(int row, int count, const QModelIndex& parent)
{
    QByteArrayList blank_data;
    for(int i=0; i < m_headers.size(); ++i)
        blank_data.push_back("");

    DataType tempList;
    for(int i=row; i < row + count; ++i)
    {
        QString rowid = m_db->addRecord(m_sTable);
        if(rowid.isNull())
        {
            return false;
        }
        m_rowCount++;
        tempList.append(blank_data);
        tempList[i - row].replace(0, rowid.toUtf8());

        // update column with default values
        QByteArrayList rowdata;
        if( m_db->getRow(m_sTable, rowid, rowdata) )
        {
            for(int j=1; j < m_headers.size(); ++j)
            {
                tempList[i - row].replace(j, rowdata[j - 1]);
            }
        }
    }

    beginInsertRows(parent, row, row + count - 1);
    for(int i = 0; i < tempList.size(); ++i)
    {
        m_data.insert(i + row, tempList.at(i));
    }
    endInsertRows();
    return true;
}

bool SqliteTableModel::removeRows(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for(int i=count-1;i>=0;i--)
    {
        m_db->deleteRecord(m_sTable, m_data.at(row + i).at(0));
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
        where = "WHERE ";

        for(QMap<int, QString>::const_iterator i=m_mWhere.constBegin();i!=m_mWhere.constEnd();++i)
        {
            QString column;
            if(m_vDisplayFormat.size())
                column = QString("col%1").arg(i.key());
            else
                column = m_headers.at(i.key());
            where.append(QString("%1 %2 AND ").arg(sqlb::escapeIdentifier(column)).arg(i.value()));
        }

        // Remove last 'AND '
        where.chop(4);
    }

    QString selector;
    if(m_vDisplayFormat.empty())
    {
        selector = "*";
    } else {
        for(int i=0;i<m_vDisplayFormat.size();i++)
            selector += m_vDisplayFormat.at(i) + " AS " + QString("col%1").arg(i+1) + ",";
        selector.chop(1);
    }

    // Note: Building the SQL string is intentionally split into several parts here instead of arg()'ing it all together as one.
    // The reason is that we're adding '%' characters automatically around search terms (and even if we didn't the user could add
    // them manually) which means that e.g. searching for '1' results in another '%1' in the string which then totally confuses
    // the QString::arg() function, resulting in an invalid SQL.
    QString sql = QString("SELECT %1,%2 FROM %3 ")
            .arg(sqlb::escapeIdentifier(m_headers.at(0)))
            .arg(selector)
            .arg(sqlb::escapeIdentifier(m_sTable))
            + where
            + QString("ORDER BY %1 %2")
            .arg(sqlb::escapeIdentifier(m_headers.at(m_iSortColumn)))
            .arg(m_sSortOrder);
    setQuery(sql, true);
}

QStringList SqliteTableModel::getColumns(const QString& sQuery, QVector<int>& fieldsTypes)
{
    sqlite3_stmt* stmt;
    QByteArray utf8Query = sQuery.toUtf8();
    int status = sqlite3_prepare_v2(m_db->_db, utf8Query, utf8Query.size(), &stmt, NULL);
    QStringList listColumns;
    if(SQLITE_OK == status)
    {
        sqlite3_step(stmt);
        int columns = sqlite3_data_count(stmt);
        for(int i = 0; i < columns; ++i)
        {
            listColumns.append(QString::fromUtf8((const char*)sqlite3_column_name(stmt, i)));
            fieldsTypes.push_back(sqlite3_column_type(stmt, i));
        }
    }
    sqlite3_finalize(stmt);

    return listColumns;
}

void SqliteTableModel::updateFilter(int column, const QString& value)
{
    // Check for any special comparison operators at the beginning of the value string. If there are none default to LIKE.
    QString op = "LIKE";
    QString val;
    QString escape;
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
    } else if(value.left(1) == "=") {
        op = "=";
        val = value.mid(1);
    } else {
        // Keep the default LIKE operator

        // Set the escape character if one has been specified in the settings dialog
        QString escape_character = PreferencesDialog::getSettingsValue("databrowser", "filter_escape").toString();
        if(escape_character == "'") escape_character = "''";
        if(escape_character.length())
            escape = QString("ESCAPE '%1'").arg(escape_character);

        // Add % wildcards at the start and at the beginning of the filter query, but only if there weren't set any
        // wildcards manually. The idea is to assume that a user who's just typing characters expects the wildcards to
        // be added but a user who adds them herself knows what she's doing and doesn't want us to mess up her query.
        if(!value.contains("%"))
        {
            val = value;
            val.prepend('%');
            val.append('%');
        }
    }
    if(val.isEmpty())
        val = value;
    if(!numeric)
        val = QString("'%1'").arg(val.replace("'", "''"));

    // If the value was set to an empty string remove any filter for this column. Otherwise insert a new filter rule or replace the old one if there is already one
    if(val == "''" || val == "'%'" || val == "'%%'")
        m_mWhere.remove(column);
    else
        m_mWhere.insert(column, op + " " + QString(encode(val.toUtf8())) + " " + escape);

    // Build the new query
    buildQuery();
}

void SqliteTableModel::clearCache()
{
	if(!m_data.empty())
	{
		beginRemoveRows(QModelIndex(), 0, m_data.size() - 1);
		m_data.clear();
		endRemoveRows();
	}
}

bool SqliteTableModel::isBinary(const QModelIndex& index) const
{
    return m_vDataTypes.at(index.column()) == SQLITE_BLOB;
}

QByteArray SqliteTableModel::encode(const QByteArray& str) const
{
    if(m_encoding.isEmpty())
        return str;
    else
        return QTextCodec::codecForName(m_encoding.toUtf8())->fromUnicode(str);
}

QByteArray SqliteTableModel::decode(const QByteArray& str) const
{
    if(m_encoding.isEmpty())
        return str;
    else
        return QTextCodec::codecForName(m_encoding.toUtf8())->toUnicode(str).toUtf8();
}

Qt::DropActions SqliteTableModel::supportedDropActions() const
{
    return Qt::CopyAction;
}

bool SqliteTableModel::dropMimeData(const QMimeData* data, Qt::DropAction, int row, int column, const QModelIndex& parent)
{
    // What has been dropped on the widget?
    if(data->hasUrls())
    {
        // If it's a URL, open the file and paste the content in the current cell
        QList<QUrl> urls = data->urls();
        QFile file(urls.first().toLocalFile());
        if(file.exists() && file.open(QFile::ReadOnly))
        {
            setData(index(row, column, parent), file.readAll());
            return true;
        }
    } else if(data->hasText()) {
        // If it's just text we can set the cell data directly
        setData(index(row, column, parent), data->text());
    }

    return false;
}
