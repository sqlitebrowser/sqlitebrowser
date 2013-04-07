#include "sqlitetablemodel.h"
#include "sqlitedb.h"

#include <QDebug>

SqliteTableModel::SqliteTableModel(QObject* parent, DBBrowserDB* db)
    : QAbstractTableModel(parent)
    , m_db(db)
    , m_rowCount(0)
    , m_columnCount(0)
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

    setQuery(QString("SELECT rowid,* FROM `%1`").arg(table));
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

    // now fetch the first 100 entries
    m_data.clear();
    m_columnCount = 0;
    QString sLimitQuery = QString("%1 LIMIT 0, %2;").arg(sQuery).arg(m_chunkSize);
    m_db->logSQL(sLimitQuery, kLogMsg_App);
    utf8Query = sLimitQuery.toUtf8();
    status = sqlite3_prepare_v2(m_db->_db, utf8Query, utf8Query.size(), &stmt, NULL);

    if(SQLITE_OK == status)
    {
        status = sqlite3_step(stmt);

        if(SQLITE_ROW == status)
        {
            m_columnCount = sqlite3_data_count(stmt);
            // row data starts here
            do
            {
                QStringList rowdata;
                for (int i = 0; i < m_columnCount; ++i)
                    rowdata.append(QString::fromUtf8((const char *)sqlite3_column_text(stmt, i)));
                m_data.push_back(rowdata);
            } while(sqlite3_step(stmt) == SQLITE_ROW);
        }
    }
    sqlite3_finalize(stmt);

    emit layoutChanged();
}

int SqliteTableModel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_data.size(); // current fetched row count
}

int SqliteTableModel::totalRowCount() const
{
    return m_rowCount;
}

int SqliteTableModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_columnCount;
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
        m_data[index.row()].replace(index.column(), value.toString());

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

void SqliteTableModel::fetchMore(const QModelIndex& parent)
{
    int currentsize = m_data.size();
    int row = m_data.size();
    QString sLimitQuery = QString("%1 LIMIT %2, %3;").arg(m_sQuery).arg(row).arg(row + m_chunkSize);
    m_db->logSQL(sLimitQuery, kLogMsg_App);
    QByteArray utf8Query = sLimitQuery.toUtf8();
    sqlite3_stmt *stmt;
    int status = sqlite3_prepare_v2(m_db->_db, utf8Query, utf8Query.size(), &stmt, NULL);

    if(SQLITE_OK == status)
    {
        status = sqlite3_step(stmt);

        if(SQLITE_ROW == status)
        {
            do
            {
                QStringList rowdata;
                for (int i = 0; i < m_columnCount; ++i)
                    rowdata.append(QString::fromUtf8((const char *)sqlite3_column_text(stmt, i)));
                Q_ASSERT(m_headers.size() == rowdata.size());
                m_data.push_back(rowdata);
            } while(sqlite3_step(stmt) == SQLITE_ROW);
        }
    }
    sqlite3_finalize(stmt);
    beginInsertRows(parent, currentsize, m_data.size());
    endInsertRows();
}

Qt::ItemFlags SqliteTableModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index)/* | Qt::ItemIsEditable*/;
}

void SqliteTableModel::sort(int column, Qt::SortOrder order)
{
    // Save sort order
    m_iSortColumn = column;
    m_sSortOrder = (order == Qt::AscendingOrder ? "ASC" : "DESC");

    // Set the new query (but only if a table has already been set
    if(m_sTable != "")
    {
        setQuery(QString("SELECT rowid,* FROM `%1` ORDER BY `%2` %3")
                 .arg(m_sTable)
                 .arg(m_headers.at(m_iSortColumn))
                 .arg(m_sSortOrder)
        );
    }
}

bool SqliteTableModel::insertRows(int row, int count, const QModelIndex& parent)
{
    beginInsertRows(parent, row, row + count - 1);

    QStringList blank_data;
    for(int i=0;i<m_headers.size();i++)
        blank_data.push_back("");

    for(int i=0;i<count;i++)
    {
        m_data.insert(row, blank_data);
        m_data[row].replace(0, QString::number(m_db->addRecord(m_sTable)));
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
