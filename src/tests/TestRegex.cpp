#include "TestRegex.h"
#include "../Data.h"

#include <QIODevice>
#include <QTextStream>
#include <QtTest/QTest>

using ClipboardRows = std::vector<std::vector<QByteArray>>;
Q_DECLARE_METATYPE(ClipboardRows)

QTEST_APPLESS_MAIN(TestRegex)

namespace
{

// A device which behaves like a full disk. It takes the given number of bytes, a write which doesn't fit completely is cut short and
// returns the number of bytes which were written. After that, writing fails.
class FullDisk : public QIODevice
{
public:
    explicit FullDisk(qint64 capacity) : m_capacity(capacity) { open(QIODevice::WriteOnly); }

    QByteArray written;

protected:
    qint64 readData(char*, qint64) override { return -1; }
    qint64 writeData(const char* data, qint64 size) override
    {
        const qint64 room = m_capacity - written.size();
        if(room <= 0)
        {
            setErrorString("No space left on device");
            return -1;
        }
        const qint64 count = qMin(room, size);
        written.append(data, static_cast<int>(count));
        return count;
    }

private:
    qint64 m_capacity;
};

}

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

void TestRegex::writeAllWritesEverything()
{
    FullDisk device(1000);
    const QByteArray data(100, 'x');
    QVERIFY(writeAll(device, data.constData(), data.size()));
    QCOMPARE(device.written, data);

    // Nothing to write is nothing which can fail
    FullDisk full(0);
    QVERIFY(writeAll(full, data.constData(), 0));
}

void TestRegex::writeAllReportsPartialWrites()
{
    const QByteArray data(100, 'x');

    // Only a part of the data fits. The device doesn't report an error for this, its write function returns a smaller number
    FullDisk partial(10);
    QVERIFY(!writeAll(partial, data.constData(), data.size()));
    QCOMPARE(partial.written, data.left(10));
    // Writing the rest of the data tells what went wrong
    QCOMPARE(partial.errorString(), QString("No space left on device"));

    FullDisk full(0);
    QVERIFY(!writeAll(full, data.constData(), data.size()));
}

void TestRegex::flushStreamWritesEverything()
{
    FullDisk device(1000);
    QTextStream stream(&device);
    stream << "some text";
    QVERIFY(flushStream(stream));
    QCOMPARE(device.written, QByteArray("some text"));
}

void TestRegex::flushStreamReportsFailureOfLastChunk()
{
    // Small amounts of data stay in the buffer of the stream until it is flushed, so that is where writing fails
    FullDisk device(5);
    QTextStream stream(&device);
    stream << "some text which does not fit";
    QVERIFY(!flushStream(stream));
}

void TestRegex::flushStreamReportsEarlierFailure()
{
    // This is more than the stream buffers, so writing already fails before the stream is flushed
    FullDisk device(5);
    QTextStream stream(&device);
    stream << QString(100000, 'x');
    QVERIFY(!flushStream(stream));
}
