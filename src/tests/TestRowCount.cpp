#include "TestRowCount.h"

#include "../RowLoader.h"
#include "../sqlite.h"

#include <QtTest/QtTest>

#include <climits>
#include <memory>
#include <mutex>
#include <vector>

QTEST_GUILESS_MAIN(TestRowCount)

namespace
{

std::shared_ptr<sqlite3> openMemoryDb()
{
    sqlite3* raw = nullptr;
    if(sqlite3_open(":memory:", &raw) != SQLITE_OK)
        qFatal("Could not open the SQLite fixture");
    return std::shared_ptr<sqlite3>(raw, sqlite3_close);
}

// Runs the row count determination of a RowLoader for the given queries (an empty count query means that the loader derives
// one from the query) and returns the number which was reported through its rowCountComplete signal. -1 is returned when no
// row count was reported at all.
int determineRowCount(const std::shared_ptr<sqlite3>& db, const QString& query, const QString& countQuery)
{
    std::vector<std::string> headers;
    std::mutex cacheMutex;
    RowLoader::Cache cache;
    RowLoader loader([db]() { return db; }, [](QString) {}, headers, cacheMutex, cache);

    // The signal is emitted from a worker thread
    QObject receiver;
    bool reported = false;
    int rowCount = -1;
    QObject::connect(&loader, &RowLoader::rowCountComplete, &receiver, [&](int, int numRows) {
        reported = true;
        rowCount = numRows;
    }, Qt::QueuedConnection);

    loader.setQuery(query, countQuery);
    loader.triggerRowCountDetermination(0);

    // Wait for the signal but also give up when none arrives: there is nothing to wait for then
    for(int i = 0; i < 100 && !reported; ++i)
        QTest::qWait(30);
    loader.waitUntilIdle();
    QCoreApplication::processEvents();

    return rowCount;
}

}

void TestRowCount::rowCountOfOrdinaryQuery()
{
    auto db = openMemoryDb();
    QCOMPARE(sqlite3_exec(db.get(), "CREATE TABLE t(a); WITH RECURSIVE c(x) AS (SELECT 1 UNION ALL SELECT x+1 FROM c WHERE x<25) INSERT INTO t SELECT x FROM c;",
                          nullptr, nullptr, nullptr), SQLITE_OK);

    QCOMPARE(determineRowCount(db, "SELECT * FROM t;", QString()), 25);
    QCOMPARE(determineRowCount(db, "SELECT * FROM t WHERE a > 100;", QString()), 0);
}

void TestRowCount::rowCountBeyondIntMaxIsClamped_data()
{
    QTest::addColumn<QString>("countQuery");
    QTest::addColumn<int>("expected");

    // The row count is reported as an int. Tables with more rows than that must not make the count wrap around: a wrapped
    // negative number was dropped (the row count then stayed unknown for good) and a wrapped small number was taken for
    // the real number of rows (hiding all the others).
    QTest::newRow("INT_MAX - 1") << "SELECT 2147483646;" << INT_MAX - 1;
    QTest::newRow("INT_MAX") << "SELECT 2147483647;" << INT_MAX;
    QTest::newRow("INT_MAX + 1 (wraps to a negative number)") << "SELECT 2147483648;" << INT_MAX;
    QTest::newRow("3 billion (wraps to a negative number)") << "SELECT 3000000000;" << INT_MAX;
    QTest::newRow("2^32 + 5 (wraps to a small number)") << "SELECT 4294967301;" << INT_MAX;
    QTest::newRow("100 billion") << "SELECT 100000000000;" << INT_MAX;
}

void TestRowCount::rowCountBeyondIntMaxIsClamped()
{
    QFETCH(QString, countQuery);
    QFETCH(int, expected);

    auto db = openMemoryDb();
    // The query itself is irrelevant here, it is the (explicitly given) count query which produces the huge number
    QCOMPARE(determineRowCount(db, "SELECT 1;", countQuery), expected);
}
