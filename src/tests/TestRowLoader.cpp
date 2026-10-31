#include "TestRowLoader.h"

#include "../Settings.h"
#include "../sqlitedb.h"
#include "../sqlitetablemodel.h"

#include <QApplication>
#include <QRegularExpression>
#include <QTemporaryDir>
#include <QtTest/QtTest>

QTEST_MAIN(TestRowLoader)

namespace
{

// Creates a database with a small table 't' which can be used by the queries
bool createTestDatabase(DBBrowserDB& db, const QTemporaryDir& dir)
{
    return db.create(dir.filePath("test.db")) &&
           db.executeSQL("CREATE TABLE t(a INTEGER, b TEXT)") &&
           db.executeSQL("INSERT INTO t VALUES(1,'one'),(2,'two'),(3,'three')");
}

// The visible cells of the model, one string per row with the cells separated by commas
QStringList modelRows(const SqliteTableModel& model)
{
    QStringList rows;
    for(int row = 0; row < model.rowCount(); ++row)
    {
        QStringList cells;
        for(int column = 0; column < model.columnCount(); ++column)
            cells << model.data(model.index(row, column), Qt::DisplayRole).toString();
        rows << cells.join(',');
    }
    return rows;
}

}

void TestRowLoader::initTestCase()
{
    // Load only a few rows at once, so that the chunks are small. This setting is neither saved nor does it depend on the settings of the user.
    Settings::setValue("db", "prefetchsize", 100, false);
}

void TestRowLoader::queryRows_data()
{
    QTest::addColumn<QString>("sql");
    QTest::addColumn<int>("columns");
    QTest::addColumn<QStringList>("rows");

    // Queries which are expected to work with and without the lazy population
    QTest::newRow("select")
        << "SELECT a, b FROM t ORDER BY a DESC" << 2
        << (QStringList() << "3,three" << "2,two" << "1,one");
    QTest::newRow("select from values")
        << "SELECT * FROM (VALUES (3),(1),(2))" << 1
        << (QStringList() << "3" << "1" << "2");
    QTest::newRow("with select from values")
        << "WITH x(a) AS (VALUES (3),(1),(2)) SELECT a FROM x" << 1
        << (QStringList() << "3" << "1" << "2");
    QTest::newRow("values with limit in subquery")
        << "VALUES ((SELECT a FROM t ORDER BY a LIMIT 1)), (2)" << 1
        << (QStringList() << "1" << "2");
    QTest::newRow("values compound")
        << "VALUES (1) UNION ALL VALUES (2)" << 1
        << (QStringList() << "1" << "2");
    QTest::newRow("select compound with values")
        << "SELECT 1 UNION ALL VALUES (2)" << 1
        << (QStringList() << "1" << "2");

    // A VALUES clause on its own does not accept a LIMIT clause
    QTest::newRow("values")
        << "VALUES (3),(1),(2)" << 1
        << (QStringList() << "3" << "1" << "2");
    QTest::newRow("values lower case")
        << "values (3),(1),(2)" << 1
        << (QStringList() << "3" << "1" << "2");
    QTest::newRow("values single row")
        << "VALUES (42)" << 1
        << (QStringList() << "42");
    QTest::newRow("values with place markers")
        << "VALUES ('%1'),('100%2 %3')" << 1
        << (QStringList() << "%1" << "100%2 %3");
    QTest::newRow("values multiple columns")
        << "VALUES (1,'one'),(2,'two')" << 2
        << (QStringList() << "1,one" << "2,two");
    QTest::newRow("values semicolon")
        << "VALUES (3),(1),(2);" << 1
        << (QStringList() << "3" << "1" << "2");
    QTest::newRow("values line breaks")
        << "VALUES\n  (3),\n  (1),\n  (2)\n" << 1
        << (QStringList() << "3" << "1" << "2");
    QTest::newRow("values leading whitespace")
        << " \n\t VALUES (3),(1),(2)" << 1
        << (QStringList() << "3" << "1" << "2");
    QTest::newRow("values leading line comment")
        << "-- some numbers\nVALUES (3),(1),(2)" << 1
        << (QStringList() << "3" << "1" << "2");
    QTest::newRow("values leading block comment")
        << "/* some numbers */ VALUES (3),(1),(2)" << 1
        << (QStringList() << "3" << "1" << "2");
    QTest::newRow("values trailing comment")
        << "VALUES (3),(1),(2) -- some numbers" << 1
        << (QStringList() << "3" << "1" << "2");
}

void TestRowLoader::queryRows()
{
    QFETCH(QString, sql);
    QFETCH(int, columns);
    QFETCH(QStringList, rows);

    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    DBBrowserDB db;
    QVERIFY(createTestDatabase(db, dir));

    SqliteTableModel model(db, nullptr, QString(), true);
    model.setQuery(sql);

    // The rows are loaded in the background. Wait for them and for the total row count.
    QTRY_COMPARE_WITH_TIMEOUT(model.rowCount(), rows.size(), 5000);
    QTRY_VERIFY_WITH_TIMEOUT(model.rowCountAvailable() == SqliteTableModel::RowCount::Complete, 5000);
    QVERIFY2(!model.hasError(), qPrintable(model.lastError()));
    QCOMPARE(model.realRowCount(), rows.size());
    QCOMPARE(model.columnCount(), columns);
    QCOMPARE(modelRows(model), rows);
}

void TestRowLoader::valuesBeyondFirstChunk()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    DBBrowserDB db;
    QVERIFY(createTestDatabase(db, dir));

    // This is more than twice the number of rows which are fetched at once, so the total row count has to be determined
    // separately and later rows are fetched with an offset.
    const int count = 250;
    QStringList values;
    for(int i = 0; i < count; ++i)
        values << QString("(%1)").arg(i);

    SqliteTableModel model(db, nullptr, QString(), true);
    model.setQuery("VALUES " + values.join(','));

    QTRY_COMPARE_WITH_TIMEOUT(model.rowCount(), count, 5000);
    QTRY_VERIFY_WITH_TIMEOUT(model.rowCountAvailable() == SqliteTableModel::RowCount::Complete, 5000);
    QVERIFY2(!model.hasError(), qPrintable(model.lastError()));
    QCOMPARE(model.realRowCount(), count);

    for(int row : {0, 120, count - 1})
    {
        model.triggerCacheLoad(row);
        QTRY_COMPARE_WITH_TIMEOUT(model.data(model.index(row, 0), Qt::DisplayRole).toString(), QString::number(row), 5000);
    }
}

void TestRowLoader::otherQueriesKeepTheirStatement_data()
{
    QTest::addColumn<QString>("sql");
    QTest::addColumn<QString>("statement");     // Regular expression for the statement which is used to load the rows

    QTest::newRow("select")
        << "SELECT a, b FROM t ORDER BY a"
        << "^SELECT a, b FROM t ORDER BY a LIMIT \\d+ OFFSET 0;$";
    QTest::newRow("select semicolon")
        << "SELECT a, b FROM t ORDER BY a;"
        << "^SELECT a, b FROM t ORDER BY a LIMIT \\d+ OFFSET 0;$";
    QTest::newRow("select with limit")
        << "SELECT a FROM t ORDER BY a LIMIT 2"
        << "^SELECT a FROM t ORDER BY a LIMIT 2$";
    QTest::newRow("select compound")
        << "SELECT a FROM t UNION SELECT 10"
        << "^SELECT a FROM t UNION SELECT 10$";
    QTest::newRow("select compound with values")
        << "SELECT 1 UNION ALL VALUES (2)"
        << "^SELECT 1 UNION ALL VALUES \\(2\\)$";
    QTest::newRow("values compound")
        << "VALUES (1) UNION ALL VALUES (2)"
        << "^VALUES \\(1\\) UNION ALL VALUES \\(2\\)$";
    QTest::newRow("with select")
        << "WITH x(a) AS (VALUES (3),(1),(2)) SELECT a FROM x"
        << "^WITH x\\(a\\) AS \\(VALUES \\(3\\),\\(1\\),\\(2\\)\\) SELECT a FROM x LIMIT \\d+ OFFSET 0;$";
}

void TestRowLoader::otherQueriesKeepTheirStatement()
{
    QFETCH(QString, sql);
    QFETCH(QString, statement);

    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    DBBrowserDB db;
    QVERIFY(createTestDatabase(db, dir));

    // Remember all statements which are run in the background. The signal is queued because it is emitted by the worker thread.
    QStringList statements;
    connect(&db, &DBBrowserDB::sqlExecuted, this, [&statements](const QString& sql, int) {
        statements << sql;
    });

    SqliteTableModel model(db, nullptr, QString(), true);
    model.setQuery(sql);

    QTRY_VERIFY_WITH_TIMEOUT(model.rowCount() > 0, 5000);
    QTRY_VERIFY_WITH_TIMEOUT(model.rowCountAvailable() == SqliteTableModel::RowCount::Complete, 5000);
    QVERIFY2(!model.hasError(), qPrintable(model.lastError()));
    QCOMPARE(statements.filter(QRegularExpression(statement)).size(), 1);
}
