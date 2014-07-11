#include <QTemporaryFile>
#include <QtTest/QTest>

#include "Application.h"
#include "TestImport.h"
#include "../sqlitedb.h"

void TestImport::csvImport()
{
    // Fetch data
    QFETCH(QString, csv);
    QFETCH(char, separator);
    QFETCH(char, quote);
    QFETCH(int, numfields);
    QFETCH(QStringList, result);

    // Init basic application
    int argcount = 1;
    const char* appname = "sqlb-unittests";
    Application app(argcount, const_cast<char**>(&appname));

    // Create temporary CSV file
    QTemporaryFile file;
    QVERIFY(file.open());
    file.write(csv.toUtf8());
    file.flush();

    // Call decodeCSV function
    DBBrowserDB db;
    int numfields_read;
    QStringList retval = db.decodeCSV(file.fileName(), separator, quote, -1, &numfields_read);

    // Check return values
    QCOMPARE(retval, result);
    QCOMPARE(numfields_read, numfields);
}

void TestImport::csvImport_data()
{
    QTest::addColumn<QString>("csv");
    QTest::addColumn<char>("separator");
    QTest::addColumn<char>("quote");
    QTest::addColumn<int>("numfields");
    QTest::addColumn<QStringList>("result");

    QStringList result;
    result << "a" << "b" << "c" << "d" << "e" << "f" << "g" << "h" << "i";
    QTest::newRow("commas_noquotes") << "a,b,c\nd,e,f\ng,h,i\n"
                                     << ','
                                     << (char)0
                                     << 3
                                     << result;
    QTest::newRow("semicolons_noquotes") << "a;b;c\nd;e;f\ng;h;i\n"
                                         << ';'
                                         << (char)0
                                         << 3
                                         << result;
    QTest::newRow("commas_doublequotes") << "\"a\",\"b\",\"c\"\n\"d\",\"e\",\"f\"\n\"g\",\"h\",\"i\"\n"
                                         << ','
                                         << '"'
                                         << 3
                                         << result;
    QTest::newRow("noquotes_butquotesset") << "a,b,c\nd,e,f\ng,h,i\n"
                                           << ','
                                           << '"'
                                           << 3
                                           << result;
    QTest::newRow("windowslinebreaks") << "a,b,c\r\nd,e,f\r\ng,h,i\r\n"
                                       << ','
                                       << (char)0
                                       << 3
                                       << result;

    result.clear();
    result << "a" << "b" << "c";
    QTest::newRow("oneline") << "a,b,c"
                             << ','
                             << (char)0
                             << 3
                             << result;

    result.clear();
    result << "a,a\"" << "b" << "c" << "d" << "e" << "\"\"f,f";
    QTest::newRow("manyquotes") << "\"a,a\"\"\",\"b\",\"c\"\n\"d\",\"e\",\"\"\"\"\"f,f\"\n"
                                << ','
                                << '"'
                                << 3
                                << result;

    result.clear();
    result << QString::fromUtf8("\u4E18") << QString::fromUtf8("\u4E26") << QString::fromUtf8("\u4E4B");
    QString csv = QString::fromUtf8("\u4E18") + "," + QString::fromUtf8("\u4E26") + "," + QString::fromUtf8("\u4E4B") + "\n";
    QTest::newRow("utf8chars") << csv
                               << ','
                               << (char)0
                               << 3
                               << result;
}
