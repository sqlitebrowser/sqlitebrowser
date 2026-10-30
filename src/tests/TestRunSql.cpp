#include "TestRunSql.h"

#include "../RunSql.h"
#include "../sqlitedb.h"

#include <QApplication>
#include <QtTest/QtTest>

QTEST_MAIN(TestRunSql)

namespace
{

struct StatementResult
{
    enum Type
    {
        Executed,
        ReturnsRows,
        Errored,
    };

    Type type;
    QString message;    // The status message, or the statement for those returning rows
    int from_position;
    int to_position;
};

// Runs the part of the SQL code between the two byte positions with a RunSql worker, just like the Execute SQL tab does, and returns what was reported
// for each of the statements that have been executed.
QList<StatementResult> runSql(DBBrowserDB& db, const QString& sql, int from_position, int to_position)
{
    QList<StatementResult> results;

    RunSql worker(db, sql, from_position, to_position, false);
    // These are emitted by the worker thread. We only look at the results after it has finished.
    QObject::connect(&worker, &RunSql::statementExecuted, &worker, [&](const QString& message, int from, int to) {
        results.append(StatementResult{StatementResult::Executed, message, from, to});
    }, Qt::DirectConnection);
    QObject::connect(&worker, &RunSql::statementReturnsRows, &worker, [&](const QString& query, int from, int to, qint64) {
        results.append(StatementResult{StatementResult::ReturnsRows, query, from, to});
    }, Qt::DirectConnection);
    QObject::connect(&worker, &RunSql::statementErrored, &worker, [&](const QString& message, int from, int to) {
        results.append(StatementResult{StatementResult::Errored, message, from, to});
    }, Qt::DirectConnection);

    worker.start();
    worker.wait();

    return results;
}

}

void TestRunSql::executeRangeAfterMultibyteText_data()
{
    QTest::addColumn<QString>("text");

    // The positions passed to RunSql are byte positions of the UTF-8 encoded SQL code, as provided by the editor. Every character in front of the
    // statement we want to execute which takes up more than one byte moves its byte position away from its character position.
    QTest::newRow("ASCII") << QString(12, QChar('x'));
    QTest::newRow("two bytes per character") << QString(12, QChar(0x0416));
    QTest::newRow("three bytes per character") << QString(12, QChar(0x4E2D));
    QTest::newRow("four bytes per character") << QString::fromUtf8("\xF0\x9F\x98\x80").repeated(12);
}

void TestRunSql::executeRangeAfterMultibyteText()
{
    QFETCH(QString, text);

    DBBrowserDB db;
    QVERIFY(db.open(":memory:"));

    // Execute the last line, i.e. everything after the semicolon of the previous one
    const QString sql = "SELECT '" + text + "';\nSELECT 2;\n";
    const QByteArray bytes = sql.toUtf8();
    const int from_position = bytes.indexOf(';') + 1;
    const int to_position = bytes.length();

    const QList<StatementResult> results = runSql(db, sql, from_position, to_position);

    QCOMPARE(results.size(), 1);
    QCOMPARE(results.at(0).type, StatementResult::ReturnsRows);
    QCOMPARE(results.at(0).message, QString("SELECT 2;"));
    QCOMPARE(results.at(0).from_position, from_position);
    QCOMPARE(results.at(0).to_position, to_position - 1);
}

void TestRunSql::executeEverythingWithMultibyteText()
{
    DBBrowserDB db;
    QVERIFY(db.open(":memory:"));

    const QString sql = "SELECT '" + QString(12, QChar(0x0416)) + "';\nSELECT 2;\n";

    const QList<StatementResult> results = runSql(db, sql, 0, sql.toUtf8().length());

    QCOMPARE(results.size(), 2);
    QCOMPARE(results.at(1).message, QString("SELECT 2;"));
}

void TestRunSql::executeBlankRangeAfterMultibyteText()
{
    DBBrowserDB db;
    QVERIFY(db.open(":memory:"));

    // There is nothing but white space and a semicolon after the first statement, so nothing is to be executed for the second line
    const QString sql = "SELECT '" + QString(12, QChar(0x0416)) + "';\n;\n";
    const QByteArray bytes = sql.toUtf8();

    const QList<StatementResult> results = runSql(db, sql, bytes.indexOf(';') + 1, bytes.length());

    QVERIFY(results.isEmpty());
}
