#include "TestRegex.h"
#include "../Data.h"

#include <QtTest/QTest>

using ClipboardRows = std::vector<std::vector<QByteArray>>;
Q_DECLARE_METATYPE(ClipboardRows)

QTEST_APPLESS_MAIN(TestRegex)

void TestRegex::clipboardRows_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<ClipboardRows>("expected");

    QTest::newRow("plain-rows") << QString("one\ntwo\n")
        << ClipboardRows{{"one"}, {"two"}};
    QTest::newRow("quoted-cell") << QString("\"one\"\ttwo")
        << ClipboardRows{{"one", "two"}};
    QTest::newRow("literal-quotes-between-rows") << QString("a\"b\nc\"d\ne")
        << ClipboardRows{{"a\"b"}, {"c\"d"}, {"e"}};
    QTest::newRow("literal-quotes-after-column") << QString("x\ta\"b\nc\"d\te")
        << ClipboardRows{{"x", "a\"b"}, {"c\"d", "e"}};
    QTest::newRow("quoted-multiline") << QString("\"one\ntwo\"\tthree")
        << ClipboardRows{{"one\ntwo", "three"}};
    QTest::newRow("literal-double-quotes") << QString("one \"\" two\nthree")
        << ClipboardRows{{"one \"\" two"}, {"three"}};
    QTest::newRow("escaped-quotes") << QString("\"one \"\"two\"\"\"\tthree")
        << ClipboardRows{{"one \"two\"", "three"}};
    QTest::newRow("empty-cells") << QString("one\t\tthree\r\n\tfive\t")
        << ClipboardRows{{"one", "", "three"}, {"", "five", ""}};
    QTest::newRow("unmatched-quote") << QString("one\n\"two\nthree")
        << ClipboardRows{{"one"}, {"\"two"}, {"three"}};
    QTest::newRow("libreoffice-tab") << QString("one\x01two\tthree")
        << ClipboardRows{{"one\ttwo", "three"}};
}

void TestRegex::clipboardRows()
{
    QFETCH(QString, text);
    QFETCH(ClipboardRows, expected);
    QVERIFY(parseClipboard(text) == expected);
}

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
