#include "TestResultSort.h"

#include "../sqlitedb.h"
#include "../Settings.h"
#include "../sqlitetablemodel.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QtTest/QtTest>

// TEMPORARY CI DIAGNOSTICS: ctest on Windows shows nothing of the test output, so write it to stderr
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    TestResultSort tc;
    const QString logFile = QDir::temp().filePath("test-result-sort-log.txt");
    QStringList args = QCoreApplication::arguments();
    args << "-o" << logFile + ",txt";
    const int result = QTest::qExec(&tc, args);
    QFile f(logFile);
    if(f.open(QIODevice::ReadOnly))
    {
        const QByteArray b = f.readAll();
        fwrite(b.constData(), 1, static_cast<size_t>(b.size()), stderr);
        fflush(stderr);
    }
    return result;
}

namespace
{

// A database with the table t(a, b) in an order which is neither sorted by a nor by b, plus a table u to run statements with side effects on
class TestDb
{
public:
    TestDb()
    {
        if(!dir.isValid() || !db.create(dir.filePath("test.db")))
            return;
        ok = db.executeSQL("CREATE TABLE t(a INTEGER, b TEXT)") &&
             db.executeSQL("INSERT INTO t VALUES(3,'apple'),(1,'cherry'),(2,'banana')") &&
             db.executeSQL("CREATE TABLE u(x INTEGER)");
        db.updateSchema();
    }

    QTemporaryDir dir;
    DBBrowserDB db;
    bool ok = false;
};

// Wait until the model has loaded the first rows and knows how many there are, then load everything
void waitForRows(SqliteTableModel& model)
{
    QTRY_VERIFY_WITH_TIMEOUT(model.rowCountAvailable() == SqliteTableModel::RowCount::Complete, 5000);
    model.completeCache();
}

void runQuery(SqliteTableModel& model, const QString& sql)
{
    model.setQuery(sql);
    waitForRows(model);
}

void sortBy(SqliteTableModel& model, int column, Qt::SortOrder order)
{
    model.sort(column, order);
    waitForRows(model);
}

QStringList columnValues(const SqliteTableModel& model, int column)
{
    QStringList values;
    for(int row = 0; row < model.rowCount(); ++row)
        values << model.data(model.index(row, column), Qt::DisplayRole).toString();
    return values;
}

QString header(const SqliteTableModel& model, int column, int role = Qt::DisplayRole)
{
    return model.headerData(column, Qt::Horizontal, role).toString();
}

}

#define QVERIFY_DB(testdb) QVERIFY2((testdb).ok, "could not set up the test database")

void TestResultSort::initTestCase()
{
    // Don't read (or write) the settings of whoever runs the tests
    QVERIFY(settingsDir.isValid());
    Settings::setUserSettingsFile(settingsDir.filePath("settings.ini"));
}

void TestResultSort::sortsByColumn()
{
    TestDb t;
    QVERIFY_DB(t);
    SqliteTableModel model(t.db, nullptr, QString(), true);
    runQuery(model, "SELECT a, b FROM t");
    QVERIFY(model.isResultSortable());
    QCOMPARE(columnValues(model, 0), QStringList() << "3" << "1" << "2");

    sortBy(model, 0, Qt::AscendingOrder);
    QCOMPARE(columnValues(model, 0), QStringList() << "1" << "2" << "3");
    QCOMPARE(columnValues(model, 1), QStringList() << "cherry" << "banana" << "apple");

    sortBy(model, 0, Qt::DescendingOrder);
    QCOMPARE(columnValues(model, 0), QStringList() << "3" << "2" << "1");

    sortBy(model, 1, Qt::AscendingOrder);
    QCOMPARE(columnValues(model, 1), QStringList() << "apple" << "banana" << "cherry");
    QCOMPARE(columnValues(model, 0), QStringList() << "3" << "2" << "1");

    sortBy(model, 1, Qt::DescendingOrder);
    QCOMPARE(columnValues(model, 1), QStringList() << "cherry" << "banana" << "apple");
    QCOMPARE(columnValues(model, 0), QStringList() << "1" << "2" << "3");
}

void TestResultSort::keepsOrderByAndLimitOfTheQuery()
{
    TestDb t;
    QVERIFY_DB(t);
    SqliteTableModel model(t.db, nullptr, QString(), true);
    // The query selects the two records with the highest a, the sorting must only reorder these two and not pick others
    runQuery(model, "SELECT a, b FROM t ORDER BY a DESC LIMIT 2");
    QCOMPARE(columnValues(model, 0), QStringList() << "3" << "2");

    sortBy(model, 1, Qt::DescendingOrder);
    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(columnValues(model, 1), QStringList() << "banana" << "apple");
    QCOMPARE(columnValues(model, 0), QStringList() << "2" << "3");
}

void TestResultSort::sortsByTheRightOfTwoColumnsWithTheSameName()
{
    TestDb t;
    QVERIFY_DB(t);
    SqliteTableModel model(t.db, nullptr, QString(), true);
    runQuery(model, "SELECT a AS x, b AS x FROM t");
    QCOMPARE(model.columnCount(), 2);

    sortBy(model, 1, Qt::AscendingOrder);
    QCOMPARE(columnValues(model, 1), QStringList() << "apple" << "banana" << "cherry");
    QCOMPARE(columnValues(model, 0), QStringList() << "3" << "2" << "1");

    sortBy(model, 0, Qt::AscendingOrder);
    QCOMPARE(columnValues(model, 0), QStringList() << "1" << "2" << "3");
}

void TestResultSort::ignoresTrailingSemicolonAndComments()
{
    TestDb t;
    QVERIFY_DB(t);
    SqliteTableModel model(t.db, nullptr, QString(), true);

    runQuery(model, "SELECT a, b FROM t ;  ");
    sortBy(model, 0, Qt::AscendingOrder);
    QCOMPARE(columnValues(model, 0), QStringList() << "1" << "2" << "3");

    // A comment in the last line must not swallow the closing parenthesis of the wrapping query
    runQuery(model, "SELECT a, b FROM t -- sorted by hand\n");
    QVERIFY(model.isResultSortable());
    sortBy(model, 1, Qt::AscendingOrder);
    QCOMPARE(columnValues(model, 1), QStringList() << "apple" << "banana" << "cherry");

    runQuery(model, "/* all of them */ SELECT a, b FROM t /* end */");
    QVERIFY(model.isResultSortable());
    sortBy(model, 0, Qt::DescendingOrder);
    QCOMPARE(columnValues(model, 0), QStringList() << "3" << "2" << "1");
}

void TestResultSort::sortsWithStatements()
{
    TestDb t;
    QVERIFY_DB(t);
    SqliteTableModel model(t.db, nullptr, QString(), true);

    runQuery(model, "WITH s(n) AS (SELECT a FROM t) SELECT n FROM s");
    QVERIFY(model.isResultSortable());
    sortBy(model, 0, Qt::AscendingOrder);
    QCOMPARE(columnValues(model, 0), QStringList() << "1" << "2" << "3");

    // The statement may be written in lower case and with newlines
    runQuery(model, "with s(n) as (select a from t)\nselect n from s");
    QVERIFY(model.isResultSortable());
    sortBy(model, 0, Qt::DescendingOrder);
    QCOMPARE(columnValues(model, 0), QStringList() << "3" << "2" << "1");
}

void TestResultSort::clearingTheSortRestoresTheOriginalQuery()
{
    TestDb t;
    QVERIFY_DB(t);
    SqliteTableModel model(t.db, nullptr, QString(), true);
    runQuery(model, "SELECT a, b FROM t");
    const std::string original = model.query();

    sortBy(model, 0, Qt::AscendingOrder);
    QCOMPARE(columnValues(model, 0), QStringList() << "1" << "2" << "3");
    QVERIFY(model.query() != original);

    // Sorting by no column at all is how Qt asks a model to go back to its natural order
    sortBy(model, -1, Qt::AscendingOrder);
    QCOMPARE(columnValues(model, 0), QStringList() << "3" << "1" << "2");
    QCOMPARE(model.query(), original);
}

void TestResultSort::cyclesThroughAscendingDescendingAndNone()
{
    TestDb t;
    QVERIFY_DB(t);
    SqliteTableModel model(t.db, nullptr, QString(), true);
    runQuery(model, "SELECT a, b FROM t");

    model.cycleResultSort(0);
    waitForRows(model);
    QCOMPARE(columnValues(model, 0), QStringList() << "1" << "2" << "3");
    QCOMPARE(model.resultSortColumn(), 0);

    model.cycleResultSort(0);
    waitForRows(model);
    QCOMPARE(columnValues(model, 0), QStringList() << "3" << "2" << "1");
    QCOMPARE(model.resultSortColumn(), 0);

    model.cycleResultSort(0);
    waitForRows(model);
    QCOMPARE(columnValues(model, 0), QStringList() << "3" << "1" << "2");
    QCOMPARE(model.resultSortColumn(), -1);

    // Another column starts with ascending again, no matter what the previous column was doing
    model.cycleResultSort(0);
    waitForRows(model);
    model.cycleResultSort(1);
    waitForRows(model);
    QCOMPARE(columnValues(model, 1), QStringList() << "apple" << "banana" << "cherry");
    QCOMPARE(model.resultSortColumn(), 1);
}

void TestResultSort::showsSortIndicatorInTheHeader()
{
    TestDb t;
    QVERIFY_DB(t);
    SqliteTableModel model(t.db, nullptr, QString(), true);
    runQuery(model, "SELECT a AS x, b AS x FROM t");
    QCOMPARE(header(model, 0), QString("x"));
    QCOMPARE(header(model, 1), QString("x"));

    sortBy(model, 1, Qt::AscendingOrder);
    QCOMPARE(header(model, 0), QString("x"));
    QVERIFY2(header(model, 1).startsWith("x ") && header(model, 1).contains(QString::fromUtf8("\xE2\x96\xB4")), qPrintable(QString("utf8 hex of the header: ") + QString::fromLatin1(header(model, 1).toUtf8().toHex())));
    // The edit role is the plain column name, it must never contain the indicator
    QCOMPARE(header(model, 1, Qt::EditRole), QString("x"));

    sortBy(model, 1, Qt::DescendingOrder);
    QVERIFY2(header(model, 1).contains(QString::fromUtf8("\xE2\x96\xBE")), qPrintable(header(model, 1)));

    sortBy(model, -1, Qt::AscendingOrder);
    QCOMPARE(header(model, 1), QString("x"));
}

void TestResultSort::exportsTheSortedQuery()
{
    TestDb t;
    QVERIFY_DB(t);
    SqliteTableModel model(t.db, nullptr, QString(), true);
    runQuery(model, "SELECT a, b FROM t;");
    sortBy(model, 1, Qt::DescendingOrder);

    // Exporting the results or saving them as a view has to give what is on the screen
    QCOMPARE(model.query(), std::string("SELECT * FROM (SELECT a, b FROM t) ORDER BY 2 DESC"));
}

void TestResultSort::doesNotSortStatementsThatCannotBeWrapped_data()
{
    QTest::addColumn<QString>("sql");

    QTest::newRow("pragma") << "PRAGMA table_info(t)";
    QTest::newRow("explain") << "EXPLAIN SELECT a FROM t";
    QTest::newRow("explain query plan") << "EXPLAIN QUERY PLAN SELECT a FROM t";
    QTest::newRow("insert returning") << "INSERT INTO u VALUES(1) RETURNING x";
    QTest::newRow("update returning") << "UPDATE u SET x = x + 1 RETURNING x";
    QTest::newRow("delete returning") << "DELETE FROM u RETURNING x";
    QTest::newRow("with delete returning") << "WITH s(n) AS (SELECT 1) DELETE FROM u WHERE x IN s RETURNING x";
}

void TestResultSort::doesNotSortStatementsThatCannotBeWrapped()
{
    QFETCH(QString, sql);

    TestDb t;
    QVERIFY_DB(t);
    SqliteTableModel model(t.db, nullptr, QString(), true);
    model.setQuery(sql);
    const std::string query = model.query();
    QVERIFY(!model.isResultSortable());

    // Asking for a sort anyway must leave everything as it is, in particular it must not run the statement again in a broken wrapper
    model.sort(0, Qt::AscendingOrder);
    model.cycleResultSort(0);
    QCOMPARE(model.query(), query);
    QCOMPARE(model.resultSortColumn(), -1);
}

void TestResultSort::newQueryResetsTheSort()
{
    TestDb t;
    QVERIFY_DB(t);
    SqliteTableModel model(t.db, nullptr, QString(), true);
    runQuery(model, "SELECT a, b FROM t");
    sortBy(model, 0, Qt::AscendingOrder);
    QCOMPARE(model.resultSortColumn(), 0);

    runQuery(model, "SELECT a, b FROM t");
    QCOMPARE(model.resultSortColumn(), -1);
    QCOMPARE(columnValues(model, 0), QStringList() << "3" << "1" << "2");
    QCOMPARE(header(model, 0), QString("a"));
}
