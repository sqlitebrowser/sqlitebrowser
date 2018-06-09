#include <QDebug>

#include "RowLoader.h"
#include "sqlite.h"

namespace {

    QString rtrimChar(const QString& s, QChar c)
    {
        QString r = s.trimmed();
        while(r.endsWith(c))
            r.chop(1);
        return r;
    }

} // anon ns


RowLoader::RowLoader (
    std::function<std::shared_ptr<sqlite3>(void)> db_getter_,
    std::function<void(QString)> statement_logger_,
    QStringList & headers_,
    QMutex & cache_mutex_,
    Cache & cache_data_
    )
    : db_getter(db_getter_), statement_logger(statement_logger_), headers(headers_)
    , cache_mutex(cache_mutex_), cache_data(cache_data_)
    , query()
    , num_tasks(0)
    , pDb(nullptr)
    , stop_requested(false)
    , current_task(nullptr)
    , next_task(nullptr)
{
}

void RowLoader::setQuery (QString new_query)
{
    std::lock_guard<std::mutex> lk(m);
    query = new_query;
}

void RowLoader::triggerRowCountDetermination(int token)
{
    std::unique_lock<std::mutex> lk(m);

    num_tasks++;
    nosync_ensureDbAccess();

    // do a count query to get the full row count in a fast manner
    row_counter = std::async(std::launch::async, [this, token]() {
        auto nrows = countRows();
        if(nrows >= 0)
            emit rowCountComplete(token, nrows);

        std::lock_guard<std::mutex> lk(m);
        nosync_taskDone();
    });
}

void RowLoader::nosync_ensureDbAccess ()
{
    if(!pDb)
        pDb = db_getter();
}

std::shared_ptr<sqlite3> RowLoader::getDb () const
{
    std::lock_guard<std::mutex> lk(m);
    return pDb;
}

int RowLoader::countRows()
{
    int retval = -1;

    // Use a different approach of determining the row count when a EXPLAIN or a PRAGMA statement is used because a COUNT fails on these queries
    if(query.startsWith("EXPLAIN", Qt::CaseInsensitive) || query.startsWith("PRAGMA", Qt::CaseInsensitive))
    {
        // So just execute the statement as it is and fetch all results counting the rows
        sqlite3_stmt* stmt;
        QByteArray utf8Query = query.toUtf8();
        if(sqlite3_prepare_v2(pDb.get(), utf8Query, utf8Query.size(), &stmt, NULL) == SQLITE_OK)
        {
            retval = 0;
            while(sqlite3_step(stmt) == SQLITE_ROW)
                retval++;
            sqlite3_finalize(stmt);
            return retval;
        }
    } else {
        // If it is a normal query - hopefully starting with SELECT - just do a COUNT on it and return the results
        QString sCountQuery = QString("SELECT COUNT(*) FROM (%1);").arg(rtrimChar(query, ';'));
        statement_logger(sCountQuery);
        QByteArray utf8Query = sCountQuery.toUtf8();

        sqlite3_stmt* stmt;
        int status = sqlite3_prepare_v2(pDb.get(), utf8Query, utf8Query.size(), &stmt, NULL);
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

void RowLoader::triggerFetch (int token, size_t row_begin, size_t row_end)
{
    std::unique_lock<std::mutex> lk(m);

    if(pDb) {
        if(!row_counter.valid() || row_counter.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            // only if row count is complete, we can safely interrupt SQLite to speed up cancellation
            sqlite3_interrupt(pDb.get());
        }
    }

    if(current_task)
        current_task->cancel = true;

    nosync_ensureDbAccess();

    // (forget a possibly already existing "next task")
    next_task.reset(new Task{ *this, token, row_begin, row_end });

    lk.unlock();
    cv.notify_all();
}

void RowLoader::nosync_taskDone()
{
    if(--num_tasks == 0) {
        pDb = nullptr;
    }
}

void RowLoader::cancel ()
{
    std::unique_lock<std::mutex> lk(m);

    if(pDb)
        sqlite3_interrupt(pDb.get());

    if(current_task)
        current_task->cancel = true;

    next_task = nullptr;
    cv.notify_all();
}

void RowLoader::stop ()
{
    cancel();
    std::unique_lock<std::mutex> lk(m);
    stop_requested = true;
    cv.notify_all();
}

bool RowLoader::readingData () const
{
    std::unique_lock<std::mutex> lk(m);
    return pDb != nullptr;
}

void RowLoader::waitUntilIdle () const
{
    if(row_counter.valid())
        row_counter.wait();
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [this](){ return stop_requested || (!current_task && !next_task); });
}

void RowLoader::run ()
{
    for(;;)
    {
        std::unique_lock<std::mutex> lk(m);
        current_task = nullptr;
        cv.notify_all();

        cv.wait(lk, [this](){ return stop_requested || next_task; });

        if(stop_requested)
            return;

        current_task = std::move(next_task);
        lk.unlock();

        process(*current_task);
    }
}

void RowLoader::process (Task & t)
{
    QString sLimitQuery;
    if(query.startsWith("PRAGMA", Qt::CaseInsensitive) || query.startsWith("EXPLAIN", Qt::CaseInsensitive))
    {
        sLimitQuery = query;
    } else {
        // Remove trailing trailing semicolon
        QString queryTemp = rtrimChar(query, ';');

        // If the query ends with a LIMIT statement take it as it is, if not append our own LIMIT part for lazy population
        if(queryTemp.contains(QRegExp("LIMIT\\s+.+\\s*((,|\\b(OFFSET)\\b)\\s*.+\\s*)?$", Qt::CaseInsensitive)))
            sLimitQuery = queryTemp;
        else
            sLimitQuery = queryTemp + QString(" LIMIT %1, %2;").arg(t.row_begin).arg(t.row_end-t.row_begin);
    }
    statement_logger(sLimitQuery);

    QByteArray utf8Query = sLimitQuery.toUtf8();
    sqlite3_stmt *stmt;

    int status = sqlite3_prepare_v2(pDb.get(), utf8Query, utf8Query.size(), &stmt, NULL);

    auto row = t.row_begin;

    if(SQLITE_OK == status)
    {
        const int num_columns = headers.size();

        while(!t.cancel && sqlite3_step(stmt) == SQLITE_ROW)
        {
            Cache::value_type rowdata;
            for(int i=0;i<num_columns;++i)
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
            QMutexLocker lk(&cache_mutex);
            cache_data.set(row++, std::move(rowdata));
        }
    }
    sqlite3_finalize(stmt);

    if(row != t.row_begin)
        emit fetched(t.token, t.row_begin, row);
}
