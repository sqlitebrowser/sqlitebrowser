#include "TestDropOrder.h"
#include "../sql/DropOrder.h"

#include <QtTest/QTest>

#include <sqlite3.h>

#include <string>
#include <vector>

QTEST_GUILESS_MAIN(TestDropOrder)

using sqlb::DropItem;
using sqlb::ObjectIdentifier;

static DropItem item(const std::string& type, const std::string& name, const std::string& schema = "main")
{
    return DropItem{type, ObjectIdentifier(schema, name)};
}

// Create a database with a table, an index, a trigger and a view on it, plus a second independent table
static sqlite3* openFixture()
{
    sqlite3* db = nullptr;
    sqlite3_open(":memory:", &db);
    const char* sql =
        "CREATE TABLE t (id INTEGER PRIMARY KEY, a TEXT);"
        "CREATE INDEX idx_t_a ON t (a);"
        "CREATE TRIGGER trg_t AFTER INSERT ON t BEGIN UPDATE t SET a = a WHERE id = NEW.id; END;"
        "CREATE VIEW v_t AS SELECT id, a FROM t;"
        "CREATE TABLE u (x INTEGER);";
    char* err = nullptr;
    if(sqlite3_exec(db, sql, nullptr, nullptr, &err) != SQLITE_OK)
        sqlite3_free(err);
    return db;
}

static int objectCount(sqlite3* db)
{
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(db, "SELECT count(*) FROM sqlite_master WHERE name NOT LIKE 'sqlite_%';", -1, &stmt, nullptr);
    int count = -1;
    if(sqlite3_step(stmt) == SQLITE_ROW)
        count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return count;
}

static int execAll(sqlite3* db, const std::vector<std::string>& statements)
{
    int failures = 0;
    for(const auto& s : statements)
    {
        char* err = nullptr;
        if(sqlite3_exec(db, s.c_str(), nullptr, nullptr, &err) != SQLITE_OK)
        {
            failures++;
            sqlite3_free(err);
        }
    }
    return failures;
}

void TestDropOrder::dropStatementMatchesSingleDelete()
{
    // This must stay identical to what the single object delete has always executed
    QCOMPARE(QString::fromStdString(sqlb::dropStatement(item("table", "t"))), QString("DROP TABLE \"main\".\"t\";"));
    QCOMPARE(QString::fromStdString(sqlb::dropStatement(item("view", "v", "temp"))), QString("DROP VIEW \"temp\".\"v\";"));
    QCOMPARE(QString::fromStdString(sqlb::dropStatement(item("index", "i"))), QString("DROP INDEX \"main\".\"i\";"));
    QCOMPARE(QString::fromStdString(sqlb::dropStatement(item("trigger", "tr"))), QString("DROP TRIGGER \"main\".\"tr\";"));
}

void TestDropOrder::dependantsComeFirst()
{
    const auto ordered = sqlb::orderForDrop({
        item("table", "t"),
        item("view", "v_t"),
        item("index", "idx_t_a"),
        item("trigger", "trg_t"),
        item("table", "u"),
    });

    const std::vector<DropItem> expected = {
        item("trigger", "trg_t"),
        item("index", "idx_t_a"),
        item("view", "v_t"),
        item("table", "t"),
        item("table", "u"),
    };
    QVERIFY(ordered == expected);
}

void TestDropOrder::duplicatesAndUnknownTypesAreRemoved()
{
    const auto ordered = sqlb::orderForDrop({
        item("table", "t"),
        item("table", "t"),
        item("database", "main"),
        item("field", "id"),
        item("table", "t", "temp"),
    });

    const std::vector<DropItem> expected = {
        item("table", "t"),
        item("table", "t", "temp"),
    };
    QVERIFY(ordered == expected);
}

void TestDropOrder::sameTypeKeepsSelectionOrder()
{
    const auto ordered = sqlb::orderForDrop({item("table", "c"), item("table", "a"), item("table", "b")});
    const std::vector<DropItem> expected = {item("table", "c"), item("table", "a"), item("table", "b")};
    QVERIFY(ordered == expected);
}

void TestDropOrder::orderedStatementsExecuteWithoutErrors()
{
    // Select everything in the worst possible order for a naive implementation
    sqlite3* db = openFixture();
    QCOMPARE(objectCount(db), 5);

    std::vector<std::string> statements;
    for(const auto& i : sqlb::orderForDrop({item("table", "t"), item("view", "v_t"), item("index", "idx_t_a"),
                                            item("trigger", "trg_t"), item("table", "u")}))
        statements.push_back(sqlb::dropStatement(i));

    QCOMPARE(statements.size(), static_cast<size_t>(5));
    QCOMPARE(execAll(db, statements), 0);
    QCOMPARE(objectCount(db), 0);
    sqlite3_close(db);
}

void TestDropOrder::naiveSelectionOrderFails()
{
    // Sanity check of the fixture: dropping the table first removes its index and trigger, so the later
    // explicit drops of those fail. This is what the ordering protects against.
    sqlite3* db = openFixture();
    const std::vector<std::string> naive = {
        sqlb::dropStatement(item("table", "t")),
        sqlb::dropStatement(item("index", "idx_t_a")),
        sqlb::dropStatement(item("trigger", "trg_t")),
    };
    QCOMPARE(execAll(db, naive), 2);
    sqlite3_close(db);
}
