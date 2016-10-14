#include "TestRegex.h"
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
}

void TestRegex::sqlQueryComments()
{
    SqliteTableModel model;

    QFETCH(QString, dirtyQuery);
    QFETCH(QString, clearQuery);

    model.removeCommentsFromQuery(dirtyQuery);
    QCOMPARE(dirtyQuery, clearQuery);
}
