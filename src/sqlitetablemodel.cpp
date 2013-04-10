#include "sqlitetablemodel.h"
#include "sqlitedb.h"

#include <QDebug>

SqliteTableModel::SqliteTableModel(QObject* parent, DBBrowserDB* db)
    : QAbstractTableModel(parent)
    , m_db(db)
    , m_rowCount(0)
    , m_iSortColumn(0)
    , m_sSortOrder("ASC")
    , m_chunkSize(1000)
{
}

void SqliteTableModel::setChunkSize(size_t chunksize)
{
    m_chunkSize = chunksize;
}

void SqliteTableModel::setTable(const QString& table)
{
    m_sTable = table;

    m_headers.clear();
    m_headers.push_back("rowid");
    m_headers.append(m_db->getTableFields(table));

    m_mWhere.clear();

    buildQuery();
}

void SqliteTableModel::setQuery(const QString& sQuery)
{
    if(!m_db->isOpen())
        return;

    sqlite3_stmt *stmt;
    m_rowCount = 0;
    m_sQuery = sQuery;

    // do a count query to get the full row count in a fast manner
    QString sCountQuery = QString("SELECT COUNT(*) FROM (%1);").arg(sQuery);
    m_db->logSQL(sCountQuery, kLogMsg_App);
    QByteArray utf8Query = sCountQuery.toUtf8();
    int status = sqlite3_prepare_v2(m_db->_db, utf8Query, utf8Query.size(), &stmt, NULL);

    if(SQLITE_OK == status)
    {
        status = sqlite3_step(stmt);
        if(SQLITE_ROW == status)
        {
            QString sCount = QString::fromUtf8((const char *) sqlite3_column_text(stmt, 0));
            m_rowCount = sCount.toInt();
        }
    }
    sqlite3_finalize(stmt);

    // now fetch the first entries
    m_data.clear();
    fetchData(0, m_chunkSize);

    emit layoutChanged();
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
        return m_headers.at(section);
    else
        return QString("%1").arg(section + 1);
}

QVariant SqliteTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_rowCount)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        return m_data.at(index.row()).at(index.column());
    }
    else
        return QVariant();
}

bool SqliteTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        m_data[index.row()].replace(index.column(), value.toByteArray());

        if(m_db->updateRecord(m_sTable, m_headers.at(index.column()), m_data[index.row()].at(0).toInt(), value.toByteArray()))
        {
            emit(dataChanged(index, index));
            return true;
        } else {
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

    return QAbstractTableModel::flags(index)/* | Qt::ItemIsEditable*/;
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
    for(int i=0;i<m_headers.size();i++)
        blank_data.push_back("");

    for(int i=0;i<count;i++)
    {
        m_data.insert(row, blank_data);
        m_data[row].replace(0, QByteArray::number(m_db->addRecord(m_sTable)));
    }

    endInsertRows();
    return true;
}

bool SqliteTableModel::removeRows(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for(int i=count-1;i>=0;i--)
    {
        m_db->deleteRecord(m_sTable, m_data.at(row + i).at(0).toInt());
        m_data.removeAt(row + i);
    }

    endRemoveRows();
    return true;
}

void SqliteTableModel::fetchData(unsigned int from, unsigned to)
{
    int currentsize = m_data.size();

    QString sLimitQuery = QString("%1 LIMIT %2, %3;").arg(m_sQuery).arg(from).arg(to);
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
                rowdata.append(QByteArray(static_cast<const char*>(sqlite3_column_blob(stmt, i)), sqlite3_column_bytes(stmt, i)));
            m_data.push_back(rowdata);
        }
    }
    sqlite3_finalize(stmt);
    beginInsertRows(QModelIndex(), currentsize + 1, m_data.size());
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

    QString sql = QString("SELECT rowid,* FROM `%1` %2 ORDER BY `%3` %4").arg(m_sTable).arg(where).arg(m_headers.at(m_iSortColumn)).arg(m_sSortOrder);
    setQuery(sql);
}

void SqliteTableModel::updateFilter(int column, const QString& value)
{
    // Check for any special comparison operators at the beginning of the value string. If there are none default to LIKE.
    QString op = "LIKE";
    QString val;
    if(value.left(2) == ">=" || value.left(2) == "<=" || value.left(2) == "<>")
    {
        bool ok;
        value.mid(2).toFloat(&ok);
        if(ok)
        {
            op = value.left(2);
            val = value.mid(2);
        }
    } else if(value.left(1) == ">" || value.left(1) == "<") {
        bool ok;
        value.mid(1).toFloat(&ok);
        if(ok)
        {
            op = value.left(1);
            val = value.mid(1);
        }
    } else {
        if(value.left(1) == "=")
        {
            op = "=";
            val = value.mid(1);
        } else {
            val = value;
        }
        val = QString("'%1'").arg(val.replace("'", ""));
    }

    // If the value was set to an empty string remove any filter for this column. Otherwise insert a new filter rule or replace the old one if there is already one
    if(value.isEmpty())
        m_mWhere.remove(column);
    else
        m_mWhere.insert(column, QString("%1 %2").arg(op).arg(val));

    // Build the new query
    buildQuery();
}
