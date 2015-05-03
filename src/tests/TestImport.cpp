// force QtCore-only main application by QTEST_MAIN
#undef QT_GUI_LIB
#include <QTemporaryFile>
#include <QtTest/QTest>
#include <QCoreApplication>
#include <QTextStream>

#include "csvparser.h"
#include "TestImport.h"

QTEST_MAIN(TestImport)

Q_DECLARE_METATYPE(CSVParser::TCSVResult)

TestImport::TestImport()
{
}

TestImport::~TestImport()
{
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
    {
    QTextStream out(&file);
    out.setCodec(encoding.toUtf8());
    out << csv;
    }
    file.flush();

    CSVParser csvparser(true, separator, quote);
    file.seek(0);
    QTextStream tstream(&file);
    tstream.setCodec(encoding.toUtf8());
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
    QTest::newRow("oldmaclinebreaks")  << "a,b,c\rd,e,f\rg,h,i\r"
                                       << ','
                                       << (char)0
                                       << "UTF-8"
                                       << 3
                                       << result;

    result.clear();
    result.append(QStringList() << "a" << "b" << "");
    result.append(QStringList() << "c" << "");
    result.append(QStringList() << "d" << "" << "e");
    result.append(QStringList() << "");
    result.append(QStringList() << "" << "" << "f");
    QTest::newRow("emptyvalues") << "a,b,\nc,\nd,,e\n\n,,f"
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
    result.append(QStringList() << QString::fromUtf8("\xC2\xAE") << QString::fromUtf8("\xC9\x85") << QString::fromUtf8("\xC6\x89"));
    QString csv = QString::fromUtf8("\xC2\xAE") + "," + QString::fromUtf8("\xC9\x85") + "," + QString::fromUtf8("\xC6\x89") + "\n";
    QTest::newRow("utf8chars") << csv
                               << ','
                               << (char)0
                               << "UTF-8"
                               << 3
                               << result;

    result.clear();
    result.append(QStringList() << QString::fromUtf8("\u4E18") << QString::fromUtf8("\u4E26") << QString::fromUtf8("\u4E4B"));
    QString csv2 = QString::fromUtf8("\u4E18") + "," + QString::fromUtf8("\u4E26") + "," + QString::fromUtf8("\u4E4B") + "\n";
    QTest::newRow("utf16chars") << csv2
                               << ','
                               << (char)0
                               << "UTF-16"
                               << 3
                               << result;
}
