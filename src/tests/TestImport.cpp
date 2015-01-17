#include <QTemporaryFile>
#include <QtTest/QTest>
#include <QCoreApplication>
#include <QTextStream>

#include "csvparser.h"
#include "TestImport.h"
#include "../sqlitedb.h"

Q_DECLARE_METATYPE(CSVParser::TCSVResult)

TestImport::TestImport()
{
    // Init basic application
    // The app needs to be initialized for the utf8 test
    // to work
    argcount = 1;
    args[0] = "sqlb-unittests";
    app = new QCoreApplication(argcount, args);
}

TestImport::~TestImport()
{
    delete app;
}

void TestImport::csvImport()
{
    // Fetch data
    QFETCH(QString, csv);
    QFETCH(char, separator);
    QFETCH(char, quote);
    QFETCH(QString, encoding);
    QFETCH(int, numfields);
    QFETCH(QVector<QStringList>, result);

    // Create temporary CSV file
    QTemporaryFile file;
    QVERIFY(file.open());
    file.write(csv.toUtf8());
    file.flush();

    // Call decodeCSV function
    DBBrowserDB db;

    CSVParser csvparser(true, separator, quote);
    file.seek(0);
    QTextStream tstream(&file);
    csvparser.parse(tstream);

    // Check return values
    QCOMPARE(csvparser.csv(), result);
    QCOMPARE((int)csvparser.columns(), numfields);
}

void TestImport::csvImport_data()
{
    QTest::addColumn<QString>("csv");
    QTest::addColumn<char>("separator");
    QTest::addColumn<char>("quote");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<int>("numfields");
    QTest::addColumn<CSVParser::TCSVResult>("result");

    CSVParser::TCSVResult result;
    result.append(QStringList() << "a" << "b" << "c");
    result.append(QStringList() << "d" << "e" << "f");
    result.append(QStringList() << "g" << "h" << "i");
    QTest::newRow("commas_noquotes") << "a,b,c\nd,e,f\ng,h,i\n"
                                     << ','
                                     << (char)0
                                     << "UTF-8"
                                     << 3
                                     << result;
    QTest::newRow("semicolons_noquotes") << "a;b;c\nd;e;f\ng;h;i\n"
                                         << ';'
                                         << (char)0
                                         << "UTF-8"
                                         << 3
                                         << result;
    QTest::newRow("commas_doublequotes") << "\"a\",\"b\",\"c\"\n\"d\",\"e\",\"f\"\n\"g\",\"h\",\"i\"\n"
                                         << ','
                                         << '"'
                                         << "UTF-8"
                                         << 3
                                         << result;
    QTest::newRow("noquotes_butquotesset") << "a,b,c\nd,e,f\ng,h,i\n"
                                           << ','
                                           << '"'
                                           << "UTF-8"
                                           << 3
                                           << result;
    QTest::newRow("windowslinebreaks") << "a,b,c\r\nd,e,f\r\ng,h,i\r\n"
                                       << ','
                                       << (char)0
                                       << "UTF-8"
                                       << 3
                                       << result;

    result.clear();
    result.append(QStringList() << "a" << "b" << "c");
    QTest::newRow("oneline") << "a,b,c"
                             << ','
                             << (char)0
                             << "UTF-8"
                             << 3
                             << result;

    result.clear();
    result.append(QStringList() << "a,a\"" << "b" << "c");
    result.append(QStringList() << "d" << "e" << "\"\"f,f");
    QTest::newRow("manyquotes") << "\"a,a\"\"\",\"b\",\"c\"\n\"d\",\"e\",\"\"\"\"\"f,f\"\n"
                                << ','
                                << '"'
                                << "UTF-8"
                                << 3
                                << result;

    result.clear();
    result.append(QStringList() << QString::fromUtf8("\u4E18") << QString::fromUtf8("\u4E26") << QString::fromUtf8("\u4E4B"));
    QString csv = QString::fromUtf8("\u4E18") + "," + QString::fromUtf8("\u4E26") + "," + QString::fromUtf8("\u4E4B") + "\n";
    QTest::newRow("utf8chars") << csv
                               << ','
                               << (char)0
                               << "UTF-8"
                               << 3
                               << result;
}
