// force QtCore-only main application by QTEST_MAIN
#undef QT_GUI_LIB
#include <QTemporaryFile>
#include <QtTest/QTest>
#include <QCoreApplication>
#include <QTextStream>
#include <vector>

#include "csvparser.h"
#include "TestImport.h"

QTEST_MAIN(TestImport)

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
    QFETCH(std::vector<std::vector<QByteArray>>, result);

    // Create temporary CSV file
    QTemporaryFile file;
    QVERIFY(file.open());
    {
        QTextStream out(&file);
// Qt6 QTextStream defaults to utf-8.
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        out.setCodec(encoding.toUtf8());
#endif
        out << csv;
    }
    file.flush();

    CSVParser csvparser(true, separator, quote);
    file.seek(0);
    QTextStream tstream(&file);
// Qt6 QTextStream defaults to utf-8.
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    tstream.setCodec(encoding.toUtf8());
#endif

    std::vector<std::vector<QByteArray>> parsedCsv;
    int parsedCsvColumns = 0;
    csvparser.parse([&parsedCsv, &parsedCsvColumns](size_t /*rowNum*/, const CSVRow& data) -> bool {
        std::vector<QByteArray> row;
        for(size_t i=0;i<data.num_fields;i++)
            row.push_back(QByteArray(data.fields[i].data, data.fields[i].data_length));
        parsedCsv.push_back(row);
        if(row.size() > parsedCsvColumns)
            parsedCsvColumns = row.size();
        return true;
    }, tstream);

    // Check return values
    QCOMPARE(parsedCsvColumns, numfields);
    QCOMPARE(parsedCsv.size(), result.size());
    for(int i=0;i<parsedCsv.size();i++)
    {
        QCOMPARE(parsedCsv.at(i).size(), result.at(i).size());
        for(int j=0;j<parsedCsv.at(i).size();j++)
            QCOMPARE(parsedCsv.at(i).at(j), result.at(i).at(j));
    }
    QCOMPARE(parsedCsv, result);
}

void TestImport::csvImport_data()
{
    QTest::addColumn<QString>("csv");
    QTest::addColumn<char>("separator");
    QTest::addColumn<char>("quote");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<int>("numfields");
    QTest::addColumn<std::vector<std::vector<QByteArray>>>("result");

    std::vector<std::vector<QByteArray>> result{
        {"a", "b", "c"},
        {"d", "e", "f"},
        {"g", "h", "i"}
    };
    QTest::newRow("commas_noquotes") << "a,b,c\nd,e,f\ng,h,i\n"
                                     << ','
                                     << static_cast<char>(0)
                                     << "UTF-8"
                                     << 3
                                     << result;
    QTest::newRow("semicolons_noquotes") << "a;b;c\nd;e;f\ng;h;i\n"
                                         << ';'
                                         << static_cast<char>(0)
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
                                       << static_cast<char>(0)
                                       << "UTF-8"
                                       << 3
                                       << result;
    QTest::newRow("oldmaclinebreaks")  << "a,b,c\rd,e,f\rg,h,i\r"
                                       << ','
                                       << static_cast<char>(0)
                                       << "UTF-8"
                                       << 3
                                       << result;

    result.clear();
    result = {
        {"a", "b", ""},
        {"c", ""},
        {"d", "", "e"},
        {""},
        {"", "", "f"}
    };
    QTest::newRow("emptyvalues") << "a,b,\nc,\nd,,e\n\n,,f"
                                     << ','
                                     << static_cast<char>(0)
                                     << "UTF-8"
                                     << 3
                                     << result;

    result.clear();
    result = {{"a", "b", "c"}};
    QTest::newRow("oneline") << "a,b,c"
                             << ','
                             << static_cast<char>(0)
                             << "UTF-8"
                             << 3
                             << result;

    result.clear();
    result = {
        {"a,a\"", "b", "c"},
        {"d", "e", "\"\"f,f"}
    };

    QTest::newRow("manyquotes") << "\"a,a\"\"\",\"b\",\"c\"\n\"d\",\"e\",\"\"\"\"\"f,f\"\n"
                                << ','
                                << '"'
                                << "UTF-8"
                                << 3
                                << result;

    result.clear();
    result = {{QByteArray("\xC2\xAE"), QByteArray("\xC9\x85"), QByteArray("\xC6\x89")}};
    QString csv = QString::fromUtf8("\xC2\xAE") + "," + QString::fromUtf8("\xC9\x85") + "," + QString::fromUtf8("\xC6\x89") + "\n";
    QTest::newRow("utf8chars") << csv
                               << ','
                               << static_cast<char>(0)
                               << "UTF-8"
                               << 3
                               << result;

    result.clear();
    result = {{QByteArray("\u4E18"), QByteArray("\u4E26"), QByteArray("\u4E4B")}};
    QString csv2 = QString::fromUtf8("\u4E18") + "," + QString::fromUtf8("\u4E26") + "," + QString::fromUtf8("\u4E4B") + "\n";
    QTest::newRow("utf16chars") << csv2
                               << ','
                               << static_cast<char>(0)
                               << "UTF-16"
                               << 3
                               << result;
}
