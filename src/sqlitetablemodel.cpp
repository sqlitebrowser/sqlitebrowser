#include "sqlitetablemodel.h"

#include "sqlitedb.h"

#include <QDebug>

SqliteTableModel::SqliteTableModel(QObject* parent, DBBrowserDB* db)
    : QAbstractTableModel(parent)
    , m_db(db)
    , m_rowCount(0)
    , m_columnCount(0)
    , m_chunkSize(1000)
{
}

void SqliteTableModel::setChunkSize(size_t chunksize)
{
    m_chunkSize = chunksize;
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

    // now fetch the first 100 entries and get headers
    m_headers.clear();
    m_data.clear();
    m_columnCount = 0;
    QString sLimitQuery = QString("%1 LIMIT 0, %2;").arg(sQuery).arg(m_chunkSize);
    utf8Query = sLimitQuery.toUtf8();
    status = sqlite3_prepare_v2(m_db->_db, utf8Query, utf8Query.size(), &stmt, NULL);

    if(SQLITE_OK == status)
    {
        status = sqlite3_step(stmt);

        if(SQLITE_ROW == status)
        {
            m_columnCount = sqlite3_data_count(stmt);
            for (int i = 0; i < m_columnCount; ++i)
                m_headers.append(QString::fromUtf8((const char *)sqlite3_column_name(stmt, i)));

            int row = 0;
            // row data starts here
            do
            {
                QStringList rowdata;
                for (int i = 0; i < m_columnCount; ++i)
                    rowdata.append(QString::fromUtf8((const char *)sqlite3_column_text(stmt, i)));
                m_data.insert(row++, rowdata);
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
        return QString("Row %1").arg(section);
}

QVariant SqliteTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_rowCount)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        return m_data[index.row()].at(index.column());
    }
    else
        return QVariant();
}

bool SqliteTableModel::canFetchMore(const QModelIndex &parent) const
{
    return m_data.size() < m_rowCount;
}

void SqliteTableModel::fetchMore(const QModelIndex& parent)
{
    int currentsize = m_data.size();
    int row = m_data.size();
    QString sLimitQuery = QString("%1 LIMIT %2, %3;").arg(m_sQuery).arg(row).arg(row + m_chunkSize);
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
                m_data.insert(row++, rowdata);
            } while(sqlite3_step(stmt) == SQLITE_ROW);
        }
    }
    sqlite3_finalize(stmt);
    beginInsertRows(parent, currentsize, m_data.size());
    endInsertRows();
}
