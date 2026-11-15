#include "TestExplainTree.h"

#include "../ExplainTree.h"

#include <QStandardItemModel>
#include <QtTest/QtTest>
#include <sqlite3.h>

QTEST_MAIN(TestExplainTree)

using namespace ExplainTree;

namespace
{

std::vector<int> depthsOf(const std::vector<TreeLine>& lines)
{
    std::vector<int> result;
    for(const auto& l : lines)
        result.push_back(l.depth);
    return result;
}

std::vector<size_t> rowsOf(const std::vector<TreeLine>& lines)
{
    std::vector<size_t> result;
    for(const auto& l : lines)
        result.push_back(l.row);
    return result;
}

}

void TestExplainTree::detectsExplainQueryPlan()
{
    QVERIFY(isExplainQueryPlan("EXPLAIN QUERY PLAN SELECT 1"));
    QVERIFY(isExplainQueryPlan("  explain   query\n plan select 1;"));
    QVERIFY(!isExplainQueryPlan("EXPLAIN SELECT 1"));
    QVERIFY(!isExplainQueryPlan("SELECT 'EXPLAIN QUERY PLAN'"));
    QVERIFY(!isExplainQueryPlan("EXPLAIN QUERY PLANX SELECT 1"));
    QVERIFY(!isExplainQueryPlan(""));
}

void TestExplainTree::flatPlan()
{
    const std::vector<PlanRow> rows = {{2, 0, "SCAN a"}, {3, 0, "SCAN b"}};
    const auto lines = buildTree(rows);
    QCOMPARE(lines.size(), size_t(2));
    QCOMPARE(depthsOf(lines), (std::vector<int>{0, 0}));
    QCOMPARE(lines[1].detail, QString("SCAN b"));
    QCOMPARE(depths(rows), (std::vector<int>{0, 0}));
}

void TestExplainTree::nestedPlan()
{
    // SQLite emits parents before their children
    const std::vector<PlanRow> rows = {
        {2, 0, "MULTI-INDEX OR"},
        {3, 2, "INDEX 1"},
        {5, 3, "SEARCH t USING INDEX i"},
        {6, 2, "INDEX 2"},
        {9, 0, "USE TEMP B-TREE FOR ORDER BY"},
    };
    const auto lines = buildTree(rows);
    QCOMPARE(rowsOf(lines), (std::vector<size_t>{0, 1, 2, 3, 4}));
    QCOMPARE(depthsOf(lines), (std::vector<int>{0, 1, 2, 1, 0}));
    QCOMPARE(depths(rows), (std::vector<int>{0, 1, 2, 1, 0}));
}

void TestExplainTree::outOfOrderIds()
{
    // Children listed before their parents and ids not ascending: the output must be depth-first
    const std::vector<PlanRow> rows = {
        {40, 30, "grandchild"},
        {10, 0, "root B"},
        {30, 20, "child of A"},
        {20, 0, "root A"},
        {50, 10, "child of B"},
        {35, 20, "second child of A"},
    };
    const auto lines = buildTree(rows);
    // Roots in input order (B, A), children in input order below their parent
    QCOMPARE(rowsOf(lines), (std::vector<size_t>{1, 4, 3, 2, 0, 5}));
    QCOMPARE(depthsOf(lines), (std::vector<int>{0, 1, 0, 1, 2, 1}));
    // depths() stays in input order
    QCOMPARE(depths(rows), (std::vector<int>{2, 0, 1, 0, 1, 1}));
}

void TestExplainTree::unknownParentIsTopLevel()
{
    const std::vector<PlanRow> rows = {{2, 99, "orphan"}, {3, 2, "child of orphan"}};
    QCOMPARE(depths(rows), (std::vector<int>{0, 1}));
}

void TestExplainTree::cycleDoesNotLoop()
{
    // Self reference and a two-element loop must not hang and must not lose rows
    const std::vector<PlanRow> rows = {{1, 1, "self"}, {2, 3, "a"}, {3, 2, "b"}, {4, 0, "ok"}};
    const auto lines = buildTree(rows);
    QCOMPARE(lines.size(), size_t(4));
    QCOMPARE(depths(rows).size(), size_t(4));
    QCOMPARE(depths(rows)[3], 0);
}

void TestExplainTree::emptyInput()
{
    QVERIFY(buildTree({}).empty());
    QVERIFY(depths({}).empty());
}

void TestExplainTree::indentText()
{
    QCOMPARE(indent("SCAN t", 0), QString("SCAN t"));
    QVERIFY(indent("SCAN t", 1).endsWith("SCAN t"));
    QVERIFY(indent("SCAN t", 2).size() > indent("SCAN t", 1).size());
    QVERIFY(indent("SCAN t", 1).size() > QString("SCAN t").size());
    QCOMPARE(indent("SCAN t", -1), QString("SCAN t"));
}

void TestExplainTree::realPlanFromSqlite()
{
    sqlite3* db = nullptr;
    QCOMPARE(sqlite3_open(":memory:", &db), SQLITE_OK);
    QCOMPARE(sqlite3_exec(db, "CREATE TABLE t(a INTEGER, b INTEGER); CREATE INDEX ia ON t(a); CREATE INDEX ib ON t(b);", nullptr, nullptr, nullptr), SQLITE_OK);

    sqlite3_stmt* stmt = nullptr;
    QCOMPARE(sqlite3_prepare_v2(db, "EXPLAIN QUERY PLAN SELECT * FROM t WHERE a = 1 OR b = 2", -1, &stmt, nullptr), SQLITE_OK);
    std::vector<PlanRow> rows;
    while(sqlite3_step(stmt) == SQLITE_ROW)
        rows.push_back({sqlite3_column_int(stmt, 0), sqlite3_column_int(stmt, 1), QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)))});
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    QVERIFY(rows.size() >= 3);
    const auto d = depths(rows);
    QCOMPARE(d.front(), 0);
    // At least one step is nested below another one and no step is nested deeper than the number of rows
    QVERIFY(*std::max_element(d.begin(), d.end()) >= 1);
    QCOMPARE(buildTree(rows).size(), rows.size());
}

void TestExplainTree::delegateIndentsDetailColumn()
{
    QStandardItemModel model(3, 4);
    const int ids[] = {2, 3, 4};
    const int parents[] = {0, 2, 3};
    const char* details[] = {"A", "B", "C"};
    for(int r = 0; r < 3; ++r)
    {
        model.setItem(r, 0, new QStandardItem(QString::number(ids[r])));
        model.setItem(r, 1, new QStandardItem(QString::number(parents[r])));
        model.setItem(r, 2, new QStandardItem("0"));
        model.setItem(r, 3, new QStandardItem(details[r]));
    }

    Delegate delegate;
    QCOMPARE(delegate.indentedText(model.index(0, 3)), QString("A"));
    QCOMPARE(delegate.indentedText(model.index(1, 3)), indent("B", 1));
    QCOMPARE(delegate.indentedText(model.index(2, 3)), indent("C", 2));

    // Not applicable for other columns
    QCOMPARE(delegate.indentedText(model.index(2, 0)), QString("4"));

    // The text shown by the delegate is the indented one, the model is unchanged
    QStyleOptionViewItem option;
    delegate.initStyleOption(&option, model.index(2, 3));
    QCOMPARE(option.text, indent("C", 2));
    QCOMPARE(model.index(2, 3).data().toString(), QString("C"));

    // Models that don't look like an EXPLAIN QUERY PLAN result are left alone
    QStandardItemModel other(1, 2);
    other.setItem(0, 1, new QStandardItem("x"));
    QCOMPARE(delegate.indentedText(other.index(0, 1)), QString("x"));
}
