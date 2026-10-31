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

// The byte position of the first character of the given line. Lines are separated by CR+LF, LF or CR, like in the editor.
int lineStart(const QByteArray& sql, int line)
{
    int position = 0;
    for(; line > 0; line--)
    {
        while(position < sql.length() && sql.at(position) != '\r' && sql.at(position) != '\n')
            position++;
        if(position < sql.length() && sql.at(position) == '\r' && position + 1 < sql.length() && sql.at(position + 1) == '\n')
            position++;
        position++;
    }
    return position;
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

void TestRunSql::executeCurrentLine_data()
{
    QTest::addColumn<QString>("sql");
    QTest::addColumn<int>("line");
    QTest::addColumn<QStringList>("executed");

    QTest::newRow("only statement") << "SELECT 1;" << 0 << (QStringList() << "SELECT 1;");
    QTest::newRow("first of two lines") << "SELECT 1;\nSELECT 2;\n" << 0 << (QStringList() << "SELECT 1;");
    QTest::newRow("last of two lines") << "SELECT 1;\nSELECT 2;\n" << 1 << (QStringList() << "SELECT 2;");
    QTest::newRow("last line without line break") << "SELECT 1;\nSELECT 2;" << 1 << (QStringList() << "SELECT 2;");
    QTest::newRow("last line without semicolon") << "SELECT 1;\nSELECT 2" << 1 << (QStringList() << "SELECT 2");
    QTest::newRow("CRLF line breaks") << "SELECT 1;\r\nSELECT 2;\r\nSELECT 3;\r\n" << 1 << (QStringList() << "SELECT 2;");
    QTest::newRow("CR line breaks") << "SELECT 1;\rSELECT 2;\rSELECT 3;\r" << 1 << (QStringList() << "SELECT 2;");
    QTest::newRow("several statements in the line") << "SELECT 1; SELECT 2;\nSELECT 3;\n" << 0 << (QStringList() << "SELECT 1;" << "SELECT 2;");
    QTest::newRow("statement over several lines, first line") << "SELECT 1,\n2;\nSELECT 3;\n" << 0 << (QStringList() << "SELECT 1,\n2;");
    QTest::newRow("statement over several lines, last line") << "SELECT 1,\n2;\nSELECT 3;\n" << 1 << (QStringList() << "SELECT 1,\n2;");
    QTest::newRow("leading comment") << "SELECT 1;\n/* c */ SELECT 2;\nSELECT 3;\n" << 1 << (QStringList() << "/* c */ SELECT 2;");

    // Whatever is left in the line after the last statement must not make the next line run
    QTest::newRow("empty statement after statement") << "SELECT 1; ;\n\nSELECT 2;\n" << 0 << (QStringList() << "SELECT 1;");
    QTest::newRow("two semicolons after statement") << "SELECT 1;;\nSELECT 2;\n" << 0 << (QStringList() << "SELECT 1;");
    QTest::newRow("trailing spaces") << "SELECT 1;  \nSELECT 2;\n" << 0 << (QStringList() << "SELECT 1;");
    QTest::newRow("trailing spaces and CRLF") << "SELECT 1; \t\r\nSELECT 2;\r\n" << 0 << (QStringList() << "SELECT 1;");
    QTest::newRow("trailing line comment") << "SELECT 1; -- note\nSELECT 2;\n" << 0 << (QStringList() << "SELECT 1;");
    QTest::newRow("trailing block comment") << "SELECT 1; /* note */\nSELECT 2;\n" << 0 << (QStringList() << "SELECT 1;");
    QTest::newRow("comment line") << "SELECT 1;\n-- SELECT 5;\nSELECT 2;\n" << 1 << QStringList();
    QTest::newRow("blank line") << "SELECT 1;\n\nSELECT 2;\n" << 1 << QStringList();
    QTest::newRow("line with only a semicolon") << "SELECT 1;\n;\nSELECT 2;\n" << 1 << QStringList();
}

void TestRunSql::executeCurrentLine()
{
    QFETCH(QString, sql);
    QFETCH(int, line);
    QFETCH(QStringList, executed);

    DBBrowserDB db;
    QVERIFY(db.open(":memory:"));

    const QByteArray bytes = sql.toUtf8();
    int from_position, to_position;
    RunSql::getCurrentLineRange(bytes, lineStart(bytes, line), from_position, to_position);

    QStringList statements;
    for(const StatementResult& result : runSql(db, sql, from_position, to_position))
    {
        QVERIFY2(result.type == StatementResult::ReturnsRows, qPrintable(result.message));
        statements.append(result.message);
    }

    QCOMPARE(statements, executed);
}

void TestRunSql::statementReturningNoRows_data()
{
    QTest::addColumn<QString>("sql");
    QTest::addColumn<bool>("returnsRows");

    QTest::newRow("select") << "SELECT 1 WHERE 0;" << true;
    QTest::newRow("with") << "WITH dup(id) AS (SELECT 1) SELECT dup.* FROM dup WHERE id = 10;" << true;
    QTest::newRow("with and comment") << "-- comment\nWITH dup(id) AS (SELECT 1) SELECT dup.* FROM dup WHERE id = 10;" << true;
    QTest::newRow("with recursive") << "WITH RECURSIVE c(x) AS (SELECT 1 UNION ALL SELECT x+1 FROM c WHERE x < 3) SELECT x FROM c WHERE x > 5;" << true;
    QTest::newRow("values") << "VALUES (1) EXCEPT VALUES (1);" << true;
    QTest::newRow("statement without data") << "CREATE TABLE t(a);" << false;
}

void TestRunSql::statementReturningNoRows()
{
    QFETCH(QString, sql);
    QFETCH(bool, returnsRows);

    DBBrowserDB db;
    QVERIFY(db.open(":memory:"));

    const QList<StatementResult> results = runSql(db, sql, 0, sql.toUtf8().length());

    QCOMPARE(results.size(), 1);
    QVERIFY2(results.at(0).type != StatementResult::Errored, qPrintable(results.at(0).message));
    QCOMPARE(results.at(0).type == StatementResult::ReturnsRows, returnsRows);
}

void TestRunSql::getCurrentLineRange_data()
{
    QTest::addColumn<QString>("sql");
    QTest::addColumn<int>("line");
    QTest::addColumn<int>("from");
    QTest::addColumn<int>("to");

    QTest::newRow("empty") << "" << 0 << 0 << 0;
    QTest::newRow("only line") << "SELECT 1;" << 0 << 0 << 9;
    QTest::newRow("only line with line break") << "SELECT 1;\n" << 0 << 0 << 9;
    QTest::newRow("empty last line") << "SELECT 1;\n" << 1 << 9 << 10;
    QTest::newRow("second line") << "SELECT 1;\nSELECT 2;\nSELECT 3;" << 1 << 9 << 19;
    QTest::newRow("last line without line break") << "SELECT 1;\nSELECT 2;\nSELECT 3;" << 2 << 19 << 29;
    QTest::newRow("last line without semicolon") << "SELECT 1;\nSELECT 2" << 1 << 9 << 18;
    QTest::newRow("CRLF") << "SELECT 1;\r\nSELECT 2;\r\nSELECT 3;\r\n" << 1 << 9 << 20;
    QTest::newRow("CR") << "SELECT 1;\rSELECT 2;\rSELECT 3;\r" << 1 << 9 << 19;
    QTest::newRow("first line of a statement") << "SELECT 1;\nSELECT 2,\n3;\nSELECT 4;" << 1 << 9 << 19;
    QTest::newRow("last line of a statement") << "SELECT 1;\nSELECT 2,\n3;\nSELECT 4;" << 2 << 9 << 22;
    QTest::newRow("no semicolon in front") << "SELECT 1,\n2,\n3" << 2 << 0 << 14;
    QTest::newRow("several statements in the line") << "SELECT 1;\nSELECT 2; SELECT 3;\n" << 1 << 9 << 29;
    QTest::newRow("blank line") << "SELECT 1;\n\nSELECT 2;" << 1 << 9 << 10;

    // Positions are in bytes, U+0416 has two of them
    QTest::newRow("multibyte") << "SELECT '" + QString(QChar(0x0416)) + "';\nSELECT '" + QString(2, QChar(0x0416)) + "';\n" << 1 << 12 << 27;
}

void TestRunSql::getCurrentLineRange()
{
    QFETCH(QString, sql);
    QFETCH(int, line);
    QFETCH(int, from);
    QFETCH(int, to);

    const QByteArray bytes = sql.toUtf8();
    int from_position = -1, to_position = -1;
    RunSql::getCurrentLineRange(bytes, lineStart(bytes, line), from_position, to_position);

    QCOMPARE(from_position, from);
    QCOMPARE(to_position, to);
}

void TestRunSql::findStatementStart_data()
{
    QTest::addColumn<QString>("sql");
    QTest::addColumn<int>("position");
    QTest::addColumn<int>("start");

    QTest::newRow("empty") << "" << 0 << 0;
    QTest::newRow("statement") << "SELECT 1;" << 0 << 0;
    QTest::newRow("white space") << " \t\r\n\fSELECT 1;" << 0 << 5;
    QTest::newRow("only white space") << " \n " << 0 << 3;
    QTest::newRow("empty statements") << " ; ;;SELECT 1;" << 0 << 5;
    QTest::newRow("line comment") << "-- comment\nSELECT 1;" << 0 << 11;
    QTest::newRow("line comment at the end") << "-- comment" << 0 << 10;
    QTest::newRow("line comment with CRLF") << "-- comment\r\nSELECT 1;" << 0 << 12;
    QTest::newRow("block comment") << "/* comment\n; */SELECT 1;" << 0 << 15;
    QTest::newRow("unterminated block comment") << "/* comment" << 0 << 10;
    QTest::newRow("block comment not closed by its opening") << "/*/ SELECT 1;" << 0 << 13;
    QTest::newRow("everything mixed") << " ; /* a */ -- b\n;\n/* c */SELECT 1;" << 0 << 25;
    QTest::newRow("single minus") << "- 1" << 0 << 0;
    QTest::newRow("single slash") << "/ 1" << 0 << 0;
    QTest::newRow("comment in a statement") << "SELECT /* a */ 1;" << 0 << 0;
    QTest::newRow("starting later") << "SELECT 1; -- a\nSELECT 2;" << 9 << 15;
    QTest::newRow("starting at the end") << "SELECT 1;" << 9 << 9;
}

void TestRunSql::findStatementStart()
{
    QFETCH(QString, sql);
    QFETCH(int, position);
    QFETCH(int, start);

    QCOMPARE(RunSql::findStatementStart(sql.toUtf8(), position), start);
}
