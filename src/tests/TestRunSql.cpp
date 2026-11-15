#include "TestRunSql.h"

#include "../RunSql.h"
#include "../sqlitedb.h"

#include <QApplication>
#include <QFileInfo>
#include <QTemporaryDir>
#include <QtTest/QtTest>

QTEST_MAIN(TestRunSql)

namespace
{

struct RunResult
{
    QStringList errors;
    int executed;
};

// Execute the complete script in the worker thread, just like the Execute SQL tab does, but without
// asking the user anything: the confirmation before committing is always answered with "Yes".
RunResult runScript(DBBrowserDB& db, const QString& sql)
{
    RunResult result{{}, 0};

    RunSql runner(db, sql, 0, sql.toUtf8().length(), false);
    QObject::connect(&runner, &RunSql::statementErrored, &runner, [&result](const QString& message, int, int) {
        result.errors << message;
    }, Qt::DirectConnection);
    QObject::connect(&runner, &RunSql::statementExecuted, &runner, [&result](const QString&, int, int) {
        result.executed++;
    }, Qt::DirectConnection);
    QObject::connect(&runner, &RunSql::confirmSaveBeforePragmaOrVacuum, &runner, []() {
    }, Qt::DirectConnection);

    runner.start();
    runner.wait();

    return result;
}

struct Fixture
{
    QTemporaryDir dir;
    DBBrowserDB db;

    Fixture()
    {
        if(!dir.isValid() || !db.create(dir.filePath("test.db")))
            qFatal("Could not set up the test database");
        if(!db.executeSQL("CREATE TABLE t(a INTEGER); INSERT INTO t VALUES(1);", false))
            qFatal("Could not fill the test database");
    }
    ~Fixture()
    {
        db.releaseAllSavepoints();
        db.close();
    }
};

}

void TestRunSql::queryTypeIgnoresLeadingComments_data()
{
    QTest::addColumn<QString>("query");
    QTest::addColumn<int>("type");

    QTest::newRow("plain vacuum") << "VACUUM;" << int(RunSql::VacuumStatement);
    QTest::newRow("line comment") << "-- compact\nVACUUM;" << int(RunSql::VacuumStatement);
    QTest::newRow("block comment") << "/* compact */ vacuum" << int(RunSql::VacuumStatement);
    QTest::newRow("several comments") << "-- a\n/* b */\n-- c\nVACUUM INTO 'x.db'" << int(RunSql::VacuumStatement);
    QTest::newRow("pragma") << "-- set\nPRAGMA user_version = 2" << int(RunSql::PragmaStatement);
    QTest::newRow("select") << "/* c */ SELECT 1" << int(RunSql::SelectStatement);
    QTest::newRow("comment only mentioning vacuum") << "-- VACUUM\nSELECT 1" << int(RunSql::SelectStatement);
    QTest::newRow("block comment mentioning vacuum") << "/* VACUUM */ UPDATE t SET a = 1" << int(RunSql::UpdateStatement);
}

void TestRunSql::queryTypeIgnoresLeadingComments()
{
    QFETCH(QString, query);
    QFETCH(int, type);

    QCOMPARE(int(RunSql::getQueryType(query)), type);
}

void TestRunSql::vacuumPrecededByComment()
{
    Fixture f;

    // The database is clean here, so no savepoint may be created for the VACUUM. A leading comment must
    // not hide the VACUUM from the statement type detection, otherwise SQLite refuses to run it inside
    // the savepoint ("cannot VACUUM from within a transaction").
    const RunResult result = runScript(f.db, "-- compact the database\nVACUUM;");

    QVERIFY2(result.errors.isEmpty(), qPrintable(result.errors.join('\n')));
    QCOMPARE(result.executed, 1);
    QVERIFY(!f.db.getDirty());
}

void TestRunSql::vacuumFollowingModifyingStatementWithComment()
{
    Fixture f;

    const RunResult result = runScript(f.db,
        "INSERT INTO t VALUES(2);\n"
        "/* shrink the file */\n"
        "VACUUM;\n"
        "INSERT INTO t VALUES(3);");

    QVERIFY2(result.errors.isEmpty(), qPrintable(result.errors.join('\n')));
    QCOMPARE(result.executed, 3);
    // The last INSERT is pending in a new savepoint, everything before it was committed by the VACUUM
    QVERIFY(f.db.getDirty());
}

void TestRunSql::vacuumInto()
{
    Fixture f;

    const QString target = f.dir.filePath("copy.db");
    const RunResult result = runScript(f.db, "-- backup\nVACUUM INTO '" + target + "';");

    QVERIFY2(result.errors.isEmpty(), qPrintable(result.errors.join('\n')));
    QVERIFY(QFileInfo::exists(target));
    QVERIFY(!f.db.getDirty());
}

void TestRunSql::commentedVacuumIsNotVacuum()
{
    Fixture f;

    // A statement which merely mentions VACUUM in a comment must be executed like any other statement
    // and be undoable, i.e. it must run inside a savepoint.
    const RunResult result = runScript(f.db, "-- VACUUM\nINSERT INTO t VALUES(2);");

    QVERIFY2(result.errors.isEmpty(), qPrintable(result.errors.join('\n')));
    QVERIFY(f.db.getDirty());
}
