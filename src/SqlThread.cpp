#include "SqlThread.h"
#include "sqlitedb.h"
#include "sqlite.h"

SqlThread::SqlThread(DBBrowserDB& db, const QString& query, int column_count, QList<QByteArrayList>& result)
    : m_db(db),
      m_query(query),
      m_columnCount(column_count),
      m_result(result)
{
}

void SqlThread::run()
{
    // Prepare statement
    QByteArray utf8Query = m_query.toUtf8();
    sqlite3_stmt *stmt;
    if(sqlite3_prepare_v2(m_db._db, utf8Query, utf8Query.size(), &stmt, NULL) == SQLITE_OK)
    {
        // Fetch rows
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            // Fetch columns
            QByteArrayList rowdata;
            for(int i=0;i<m_columnCount;++i)
            {
                // Null?
                if(sqlite3_column_type(stmt, i) == SQLITE_NULL)
                {
                    rowdata.append(QByteArray());
                } else {
                    // Some data or empty string?
                    int bytes = sqlite3_column_bytes(stmt, i);
                    if(bytes)
                        rowdata.append(QByteArray(static_cast<const char*>(sqlite3_column_blob(stmt, i)), bytes));
                    else
                        rowdata.append(QByteArray(""));
                }
            }

            // Save row
            m_result.push_back(rowdata);
        }
    }

    // Clean up
    sqlite3_finalize(stmt);
    emit done();
}
