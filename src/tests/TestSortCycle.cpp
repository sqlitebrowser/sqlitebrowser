#include "TestSortCycle.h"

#include "../sql/Query.h"

#include <QtTest/QtTest>

#include <string>
#include <vector>

QTEST_APPLESS_MAIN(TestSortCycle)

namespace
{

using Columns = std::vector<sqlb::OrderBy>;

const auto Asc = sqlb::OrderBy::Ascending;
const auto Desc = sqlb::OrderBy::Descending;

// Click the same header several times in a row and return the resulting list of sort columns after each click
std::vector<Columns> clicks(Columns& columns, const std::string& column, bool multi_column, bool cycle, int count)
{
    std::vector<Columns> result;
    for(int i = 0; i < count; i++)
    {
        sqlb::updateSortColumns(columns, column, multi_column, cycle);
        result.push_back(columns);
    }
    return result;
}

}

void TestSortCycle::firstClickSortsAscending()
{
    for(bool cycle : {false, true})
    {
        Columns columns;
        sqlb::updateSortColumns(columns, "a", false, cycle);
        QCOMPARE(columns, (Columns{{"a", Asc}}));
    }
}

void TestSortCycle::singleClickToggleKeepsTwoStates()
{
    // This is the default behaviour: ascending and descending alternate, sorting is never removed by clicking
    Columns columns;
    const auto states = clicks(columns, "a", false, false, 4);
    QCOMPARE(states[0], (Columns{{"a", Asc}}));
    QCOMPARE(states[1], (Columns{{"a", Desc}}));
    QCOMPARE(states[2], (Columns{{"a", Asc}}));
    QCOMPARE(states[3], (Columns{{"a", Desc}}));
}

void TestSortCycle::singleClickCycleEndsWithoutSorting()
{
    Columns columns;
    const auto states = clicks(columns, "a", false, true, 4);
    QCOMPARE(states[0], (Columns{{"a", Asc}}));
    QCOMPARE(states[1], (Columns{{"a", Desc}}));
    QCOMPARE(states[2], Columns{});
    QCOMPARE(states[3], (Columns{{"a", Asc}}));
}

void TestSortCycle::singleClickOnOtherColumnReplacesSorting()
{
    for(bool cycle : {false, true})
    {
        Columns columns{{"a", Desc}, {"b", Asc}};
        sqlb::updateSortColumns(columns, "c", false, cycle);
        QCOMPARE(columns, (Columns{{"c", Asc}}));

        // A single click on one of several sorted columns also starts over with only that column
        columns = Columns{{"a", Desc}, {"b", Asc}};
        sqlb::updateSortColumns(columns, "b", false, cycle);
        QCOMPARE(columns, (Columns{{"b", Asc}}));
    }
}

void TestSortCycle::multiColumnToggleKeepsTwoStates()
{
    Columns columns{{"a", Asc}, {"b", Asc}};
    const auto states = clicks(columns, "b", true, false, 3);
    QCOMPARE(states[0], (Columns{{"a", Asc}, {"b", Desc}}));
    QCOMPARE(states[1], (Columns{{"a", Asc}, {"b", Asc}}));
    QCOMPARE(states[2], (Columns{{"a", Asc}, {"b", Desc}}));
}

void TestSortCycle::multiColumnCycleRemovesOnlyThatColumn()
{
    Columns columns{{"a", Asc}, {"b", Asc}, {"c", Desc}};
    const auto states = clicks(columns, "b", true, true, 3);
    QCOMPARE(states[0], (Columns{{"a", Asc}, {"b", Desc}, {"c", Desc}}));
    QCOMPARE(states[1], (Columns{{"a", Asc}, {"c", Desc}}));
    QCOMPARE(states[2], (Columns{{"a", Asc}, {"c", Desc}, {"b", Asc}}));
}

void TestSortCycle::multiColumnAppendsNewColumn()
{
    for(bool cycle : {false, true})
    {
        Columns columns{{"a", Desc}};
        sqlb::updateSortColumns(columns, "b", true, cycle);
        QCOMPARE(columns, (Columns{{"a", Desc}, {"b", Asc}}));
    }
}
