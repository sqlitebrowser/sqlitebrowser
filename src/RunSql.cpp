#include "RunSql.h"
#include "sqlite.h"
#include "sqlitedb.h"
#include "sqlitetablemodel.h"

#include <chrono>
#include <QApplication>
#include <QMessageBox>

RunSql::RunSql(DBBrowserDB& _db, QString query, int execute_from_position, int _execute_to_position, bool _interrupt_after_statements) :
    db(_db),
    may_continue_with_execution(true),
    interrupt_after_statements(_interrupt_after_statements),
    execute_current_position(execute_from_position),
    execute_to_position(_execute_to_position),
    structure_updated(false),
    savepoint_created(false),
    was_dirty(db.getDirty()),
    modified(false)
{
    // Get lock to set up everything
    std::unique_lock<std::mutex> lk(m);

    // Cancel if there is nothing to execute
    if(query.trimmed().isEmpty() || query.trimmed() == ";" || execute_from_position == execute_to_position ||
            query.mid(execute_from_position, execute_to_position-execute_from_position).trimmed().isEmpty() ||
            query.mid(execute_from_position, execute_to_position-execute_from_position).trimmed() == ";")
        return;

    // All replacements in the query should be made by the same amount of characters, so the positions in the file
    // for error indicators and line and column logs are not displaced.
    // Whitespace and comments are discarded by SQLite, so it is better to just let it ignore them.
    query = query.replace(QRegExp("^(\\s*)BEGIN TRANSACTION;", Qt::CaseInsensitive), "\\1                  ");
    query = query.replace(QRegExp("COMMIT;(\\s*)$", Qt::CaseInsensitive), "       \\1");

    // Convert query to byte array which we will use from now on, starting from the determined start position and
    // until the end of the SQL code. By doing so we go further than the determined end position because in Line
    // mode the last statement might go beyond that point.
    queries_left_to_execute = query.toUtf8().mid(execute_from_position);
}

void RunSql::run()
{
    // Execute statement by statement
    for(;;)
    {
        if(!executeNextStatement())
            break;
    }

    // Execution finished

    // If the DB structure was changed by some command in this SQL script, send a signal
    if(structure_updated)
        emit structureUpdated();
}

void RunSql::startNextStatement()
{
    std::unique_lock<std::mutex> lk(m);
    may_continue_with_execution = true;
    cv.notify_one();
}

void RunSql::stop()
{
    std::unique_lock<std::mutex> lk(m);

    stopExecution();
    if(pDb)
        sqlite3_interrupt(pDb.get());
    may_continue_with_execution = true;
    cv.notify_all();
}

bool RunSql::executeNextStatement()
{
    std::unique_lock<std::mutex> lk(m);

    // Is there anything left to do?
    if(queries_left_to_execute.isEmpty())
        return false;

    // What type of query is this?
    QString qtail = QString(queries_left_to_execute).trimmed();
    // Remove trailing comments so we don't get fooled by some trailing text at the end of the stream.
    // Otherwise we'll pass them to SQLite and its execution will trigger a savepoint that wouldn't be
    // reverted.
    SqliteTableModel::removeCommentsFromQuery(qtail);
    if (qtail.isEmpty())
        return false;

    StatementType query_type = getQueryType(qtail);

    // Check whether the DB structure is changed by this statement
    if(!structure_updated && (query_type == AlterStatement ||
                              query_type == CreateStatement ||
                              query_type == DropStatement ||
                              query_type == RollbackStatement ||
                              query_type == AttachStatement ||
                              query_type == DetachStatement))
        structure_updated = true;

    // Check whether this is trying to set a pragma or to vacuum the database
    // TODO This is wrong. The '=' or the 'defer_foreign_keys' might be in a completely different statement of the queries string.
    if((query_type == PragmaStatement && qtail.contains('=') && !qtail.contains("defer_foreign_keys", Qt::CaseInsensitive)) || query_type == VacuumStatement)
    {
        // We're trying to set a pragma. If the database has been modified it needs to be committed first. We'll need to ask the
        // user about that
        if(db.getDirty())
        {
            lk.unlock();
            // Ask user, then check if we should abort execution or continue with it. We depend on a BlockingQueueConnection here which makes sure to
            // block this worker thread until the slot function in the main thread is completed and could tell us about its decision.
            emit confirmSaveBeforePragmaOrVacuum();
            if(!queries_left_to_execute.isEmpty())
            {
                // Commit all changes
                db.releaseAllSavepoints();
            } else {
                // Abort
                emit statementErrored(tr("Execution aborted by user"), execute_current_position, execute_current_position + (query_type == PragmaStatement ? 5 : 6));
                return false;
            }
            lk.lock();
        }
    } else {
        // We're not trying to set a pragma or to vacuum the database. In this case make sure a savepoint has been created in order to avoid committing
        // all changes to the database immediately. Don't set more than one savepoint.

        if(!savepoint_created && !db.readOnly())
        {
            // We have to start a transaction before we create the prepared statement otherwise every executed
            // statement will get committed after the prepared statement gets finalized
            db.setSavepoint();
            savepoint_created = true;
        }
    }

    // Start execution timer. We do that after opening any message boxes and after creating savepoints because both are not part of the actual
    // query execution.
    auto time_start = std::chrono::high_resolution_clock::now();

    // Execute next statement
    const char* tail = queries_left_to_execute.data();
    int tail_length = queries_left_to_execute.length();
    lk.unlock();
    const char* qbegin = tail;
    acquireDbAccess();
    sqlite3_stmt* vm;
    int sql3status = sqlite3_prepare_v2(pDb.get(), tail, tail_length, &vm, &tail);
    QString queryPart = QString::fromUtf8(qbegin, static_cast<int>(tail - qbegin));
    int tail_length_before = tail_length;
    tail_length -= static_cast<int>(tail - qbegin);
    int end_of_current_statement_position = execute_current_position + tail_length_before - tail_length;

    // Save remaining statements
    lk.lock();
    queries_left_to_execute = QByteArray(tail);
    lk.unlock();

    QString error;
    if (sql3status == SQLITE_OK)
    {
        // Get type
        StatementType query_part_type = getQueryType(queryPart.trimmed());

        // Check if this statement returned any data. We skip this check if this is an ALTER TABLE statement which, for some reason, are reported to return one column.
        if(query_part_type != AlterStatement && sqlite3_column_count(vm))
        {
            // It did. So it is definitely some SELECT statement or similar and we don't need to actually execute it here
            sql3status = SQLITE_ROW;
        } else {
            // It did not. So it's probably some modifying SQL statement and we want to execute it here. If for some reason
            // it turns out to return data after all, we just change the status
            sql3status = sqlite3_step(vm);

            // SQLite returns SQLITE_DONE when a valid SELECT statement was executed but returned no results. To run into the branch that updates
            // the status message and the table view anyway manipulate the status value here. This is also done for PRAGMA statements as they (sometimes)
            // return rows just like SELECT statements, too.
            if((query_part_type == SelectStatement || query_part_type == PragmaStatement) && sql3status == SQLITE_DONE)
                sql3status = SQLITE_ROW;
        }

        // Destroy statement
        sqlite3_finalize(vm);

        switch(sql3status)
        {
        case SQLITE_ROW:
        {
            // If we get here, the SQL statement returns some sort of data. So hand it over to the model for display. Don't set the modified flag
            // because statements that display data don't change data as well.

            releaseDbAccess();

            lk.lock();
            may_continue_with_execution = false;

            auto time_end = std::chrono::high_resolution_clock::now();
            auto time_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
            emit statementReturnsRows(queryPart, execute_current_position, end_of_current_statement_position, time_in_ms.count());

            // Make sure the next statement isn't executed until we're told to do so
            if(interrupt_after_statements)
                cv.wait(lk, [this](){ return may_continue_with_execution; });
            lk.unlock();
            break;
        }
        case SQLITE_DONE:
        case SQLITE_OK:
        {
            // If we get here, the SQL statement doesn't return data and just executes. Don't run it again because it has already been executed.
            // But do set the modified flag because statements that don't return data, often modify the database.

            QString stmtHasChangedDatabase;
            if(query_part_type == InsertStatement || query_part_type == UpdateStatement || query_part_type == DeleteStatement)
                stmtHasChangedDatabase = tr(", %1 rows affected").arg(sqlite3_changes(pDb.get()));

            releaseDbAccess();

            lk.lock();

            // Attach/Detach statements don't modify the original database
            if(query_part_type != StatementType::AttachStatement && query_part_type != StatementType::DetachStatement)
                modified = true;

            may_continue_with_execution = false;

            auto time_end = std::chrono::high_resolution_clock::now();
            auto time_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
            emit statementExecuted(tr("query executed successfully. Took %1ms%2").arg(time_in_ms.count()).arg(stmtHasChangedDatabase),
                                   execute_current_position, end_of_current_statement_position);

            // Make sure the next statement isn't executed until we're told to do so
            if(interrupt_after_statements)
                cv.wait(lk, [this](){ return may_continue_with_execution; });
            lk.unlock();
            break;
        }
        case SQLITE_MISUSE:
            break;
        default:
            error = QString::fromUtf8(sqlite3_errmsg(pDb.get()));
        }
    } else {
        error = QString::fromUtf8(sqlite3_errmsg(pDb.get()));
    }

    // Release the database
    lk.lock();
    releaseDbAccess();

    // Revert to save point now if it wasn't needed. We need to do this here because there are some rare cases where the next statement might
    // be affected by what is only a temporary and unnecessary savepoint. For example in this case:
    // ATTACH 'xxx' AS 'db2'
    // SELECT * FROM db2.xy;    -- Savepoint created here
    // DETACH db2;              -- Savepoint makes this statement fail
    if(!modified && !was_dirty && savepoint_created)
    {
        db.revertToSavepoint();
        savepoint_created = false;
    }

    if(!error.isEmpty())
    {
        emit statementErrored(error, execute_current_position, end_of_current_statement_position);
        stopExecution();
        return false;
    }
    // Update the start position for the next statement and check if we are at
    // the end of the part we want to execute. If so, stop the execution now.
    execute_current_position = end_of_current_statement_position;
    if(execute_current_position >= execute_to_position)
    {
        stopExecution();
        return false;
    }

    return true;
}

void RunSql::stopExecution()
{
    queries_left_to_execute.clear();
}

RunSql::StatementType RunSql::getQueryType(const QString& query)
{
    // Helper function for getting the type of a given query

    if(query.startsWith("SELECT", Qt::CaseInsensitive)) return SelectStatement;
    if(query.startsWith("ALTER", Qt::CaseInsensitive)) return AlterStatement;
    if(query.startsWith("DROP", Qt::CaseInsensitive)) return DropStatement;
    if(query.startsWith("ROLLBACK", Qt::CaseInsensitive)) return RollbackStatement;
    if(query.startsWith("PRAGMA", Qt::CaseInsensitive)) return PragmaStatement;
    if(query.startsWith("VACUUM", Qt::CaseInsensitive)) return VacuumStatement;
    if(query.startsWith("INSERT", Qt::CaseInsensitive)) return InsertStatement;
    if(query.startsWith("UPDATE", Qt::CaseInsensitive)) return UpdateStatement;
    if(query.startsWith("DELETE", Qt::CaseInsensitive)) return DeleteStatement;
    if(query.startsWith("CREATE", Qt::CaseInsensitive)) return CreateStatement;
    if(query.startsWith("ATTACH", Qt::CaseInsensitive)) return AttachStatement;
    if(query.startsWith("DETACH", Qt::CaseInsensitive)) return DetachStatement;

    return OtherStatement;
}

void RunSql::acquireDbAccess()
{
    pDb = db.get(tr("executing query"), true);
}

void RunSql::releaseDbAccess()
{
    pDb = nullptr;
}
