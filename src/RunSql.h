#ifndef RUNSQL_H
#define RUNSQL_H

#include <memory>
#include <mutex>
#include <condition_variable>
#include <QThread>

class DBBrowserDB;
struct sqlite3;

class RunSql : public QThread
{
    Q_OBJECT

    void run() override;

public:
    RunSql(DBBrowserDB& db, QString query, int execute_from_position, int execute_to_position, bool interrupt_after_statements = false);
    ~RunSql() override = default;

    enum StatementType
    {
        SelectStatement,
        AlterStatement,
        DropStatement,
        RollbackStatement,
        PragmaStatement,
        VacuumStatement,
        InsertStatement,
        UpdateStatement,
        DeleteStatement,
        CreateStatement,
        AttachStatement,
        DetachStatement,
        OtherStatement,
    };

    static StatementType getQueryType(const QString& query);

    void startNextStatement();

    void stop();

signals:
    void statementErrored(QString message, int from_position, int to_position);
    void statementExecuted(QString message, int from_position, int to_position);
    void statementReturnsRows(QString query, int from_position, int to_position, qint64 time_in_ms);

    /**
     * This signal must be connected with a Qt::BlockingQueuedConnection in order to work as expected!
     */
    void confirmSaveBeforePragmaOrVacuum();

private:
    DBBrowserDB& db;
    std::shared_ptr<sqlite3> pDb;

    mutable std::mutex m;
    mutable std::condition_variable cv;
    bool may_continue_with_execution;

    bool interrupt_after_statements;

    QByteArray queries_left_to_execute;
    int execute_current_position;
    int execute_to_position;
    bool structure_updated;
    bool savepoint_created;
    bool was_dirty;
    bool modified;

    void stopExecution();
    bool executeNextStatement();

    void acquireDbAccess();
    void releaseDbAccess();
};

#endif
