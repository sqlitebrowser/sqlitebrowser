#include "TestRegex.h"
#include "../sqlitedb.h"
#include "../sqlitetablemodel.h"

#include <QtTest/QTest>

QTEST_APPLESS_MAIN(TestRegex)

void TestRegex::sqlQueryComments_data()
{
    QTest::addColumn<QString>("dirtyQuery");
    QTest::addColumn<QString>("clearQuery");

    QTest::newRow("test1")
        << // dirtyQuery
        "SELECT * -- asd ffdsf\n"
        "-- saf ewf sf\n"
        "-- dsaf fd\n"
        "FROM \t-- sfsdf\n"
        "qwfwqf -- asdasd"
        << // clearQuery
        "SELECT *\nFROM\nqwfwqf";

    QTest::newRow("test2")
        << // dirtyQuery
        "SELECT *-- comment\n"
        "FROM\n\n"
        "-- something\n"
        "qwfqwf"
        << // cleanQuery
        "SELECT *\nFROM\nqwfqwf";

    QTest::newRow("test3")
            << // dirtyQuery
               "-- Comment before the query\n"
               "SELECT * FROM test"
            << // cleanQuery
               "SELECT * FROM test";

    QTest::newRow("test4")
            << // dirtyQuery
               "SELECT * FROM test\n"
               "-- Comment after the query"
            << // cleanQuery
               "SELECT * FROM test";

    QTest::newRow("test5")
            << // dirtyQuery
               "SELECT 40+2 -- get the answer\n"
               "AS answer"
            << // cleanQuery
               "SELECT 40+2\n"
               "AS answer";

    QTest::newRow("test6")
            << // dirtyQuery
               "SELECT '-- comment inside quotes'"
            << // cleanQuery
               "SELECT '-- comment inside quotes'";
}

void TestRegex::sqlQueryComments()
{
    DBBrowserDB db;
    SqliteTableModel model(db);

    QFETCH(QString, dirtyQuery);
    QFETCH(QString, clearQuery);

    model.removeCommentsFromQuery(dirtyQuery);
    QCOMPARE(dirtyQuery, clearQuery);
}
