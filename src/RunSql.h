#ifndef RUNSQL_H
#define RUNSQL_H

#include <QObject>

class DBBrowserDB;

class RunSql : public QObject
{
    Q_OBJECT

public:
    RunSql(DBBrowserDB& _db);
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

    void runStatements(QString _query, int execute_from_position, int _execute_to_position);

signals:
    void statementErrored(QString message, int from_position, int to_position);
    void statementExecuted(QString message, int from_position, int to_position);
    void statementReturnsRows(QString query, int from_position, int to_position, qint64 time_in_ms);

private:
    DBBrowserDB& db;

    QByteArray queries_left_to_execute;
    int execute_current_position;
    int execute_to_position;
    bool structure_updated;
    bool savepoint_created;
    bool was_dirty;

    void stopExecution();
    bool executeNextStatement();
};

#endif
