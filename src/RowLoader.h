#ifndef ROW_LOADER_H
#define ROW_LOADER_H

#include <condition_variable>
#include <mutex>
#include <atomic>
#include <memory>
#include <future>
#include <functional>

#include <QThread>
#include <QString>
#include <QStringList>
#include <QMutex>

#include "sqlite.h"
#include "RowCache.h"

class RowLoader : public QThread
{
    Q_OBJECT

    void run() override;

public:
    typedef RowCache<QVector<QByteArray>> Cache;

    /// set up worker thread to handle row loading
    explicit RowLoader (
        std::function<std::shared_ptr<sqlite3>(void)> db_getter,
        std::function<void(QString)> statement_logger,
        QStringList & headers,
        QMutex & cache_mutex,
        Cache & cache_data
        );

    void setQuery (QString);

    void triggerRowCountDetermination (int token);

    /// trigger asynchronous reading of specified row range,
    /// cancelling previous tasks; 'row_end' is exclusive; \param
    /// token is eventually returned through the 'fetched'
    /// signal. depending on how and when tasks are cancelled, not
    /// every triggerFetch() will result in a 'fetched' signal, or the
    /// 'fetched' signal may be for a narrower row range.
    void triggerFetch (int token, size_t row_begin, size_t row_end);

    /// cancel everything
    void cancel ();

    /// cancel everything and terminate worker thread
    void stop ();

    /// currently reading any data, or anything in "queue"?
    bool readingData () const;

    /// wait until not reading any data
    void waitUntilIdle () const;

    /// get current database - note that the worker thread might be
    /// working on it, too... \returns current db, or nullptr.
    std::shared_ptr<sqlite3> getDb () const;

signals:
    void fetched(int token, size_t row_begin, size_t row_end);
    void rowCountComplete(int token, int num_rows);

private:
    const std::function<std::shared_ptr<sqlite3>()> db_getter;
    const std::function<void(QString)> statement_logger;
    QStringList & headers;
    QMutex & cache_mutex;
    Cache & cache_data;

    mutable std::mutex m;
    mutable std::condition_variable cv;

    QString query;

    mutable std::future<void> row_counter;

    size_t num_tasks;
    std::shared_ptr<sqlite3> pDb; //< exclusive access while held...

    bool stop_requested;

    struct Task
    {
        RowLoader & row_loader;
        int token;
        size_t row_begin;
        size_t row_end; //< exclusive
        std::atomic<bool> cancel;

        Task(RowLoader & row_loader_, int t, size_t a, size_t b)
            : row_loader(row_loader_), token(t), row_begin(a), row_end(b), cancel(false)
        {
            row_loader.num_tasks++;
        }

        ~Task()
        {
            // (... mutex being held ...)
            row_loader.nosync_taskDone();
        }
    };

    std::unique_ptr<Task> current_task;
    std::unique_ptr<Task> next_task;

    int countRows ();

    void process (Task &);

    void nosync_ensureDbAccess ();
    void nosync_taskDone ();

};

#endif // ROW_LOADER_H
