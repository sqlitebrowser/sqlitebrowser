#include "TestRegex.h"
#include "../Data.h"

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

    QTest::newRow("single_quote_comment")
            << // dirtyQuery
               "SELECT 'something--something' -- comment"
            << // cleanQuery
               "SELECT 'something--something'";

    /* This still needs to be fixed in our code before activating the test
    QTest::newRow("double_quote_comment")
            << // dirtyQuery
               "SELECT \"something--something\" -- comment"
            << // cleanQuery
               "SELECT \"something--something\"";*/
}

void TestRegex::sqlQueryComments()
{
    QFETCH(QString, dirtyQuery);
    QFETCH(QString, clearQuery);

    removeCommentsFromQuery(dirtyQuery);
    QCOMPARE(dirtyQuery, clearQuery);
}
