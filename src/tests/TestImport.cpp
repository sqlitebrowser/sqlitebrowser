// force QtCore-only main application by QTEST_MAIN
#undef QT_GUI_LIB
#include <QBuffer>
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

    // RFC 4180: fields containing line breaks, quotes and separators are enclosed in quotes. A quote inside a quoted field is escaped by
    // a second quote.
    result.clear();
    result = {
        {"a\r\nb", "c"},
        {"d", "e\r\nf"},
        {"g\nh", "i\rj"}
    };
    QTest::newRow("linebreaks_in_quotes") << "\"a\r\nb\",c\r\nd,\"e\r\nf\"\r\n\"g\nh\",\"i\rj\"\r\n"
                                          << ','
                                          << '"'
                                          << "UTF-8"
                                          << 2
                                          << result;

    result.clear();
    result = {
        {"\"a\"", "\"", "b\"c", ""},
        {"", "", "x,y", "\"\""}
    };
    QTest::newRow("escaped_quotes_and_empty_quoted") << "\"\"\"a\"\"\",\"\"\"\",\"b\"\"c\",\"\"\r\n\"\",\"\",\"x,y\",\"\"\"\"\"\"\r\n"
                                                     << ','
                                                     << '"'
                                                     << "UTF-8"
                                                     << 4
                                                     << result;

    // RFC 4180: the last record in the file may or may not have an ending line break
    result.clear();
    result = {
        {"a", "b"},
        {"c", "d"}
    };
    QTest::newRow("last_record_without_linebreak") << "a,b\r\nc,d"
                                                   << ','
                                                   << static_cast<char>(0)
                                                   << "UTF-8"
                                                   << 2
                                                   << result;
    QTest::newRow("last_quoted_record_without_linebreak") << "\"a\",\"b\"\r\n\"c\",\"d\""
                                                          << ','
                                                          << '"'
                                                          << "UTF-8"
                                                          << 2
                                                          << result;

    result.clear();
    result = {
        {"a"},
        {""}
    };
    QTest::newRow("last_empty_quoted_record_without_linebreak") << "a\n\"\""
                                                                << ','
                                                                << '"'
                                                                << "UTF-8"
                                                                << 1
                                                                << result;

    // The parser accepts lone CR as a line break (see oldmaclinebreaks). This must also work when the last field of a line is quoted
    // and the first field of the following line is quoted, too.
    result.clear();
    result = {
        {"a", "b"},
        {"c", "d"}
    };
    QTest::newRow("quoted_oldmaclinebreaks") << "\"a\",\"b\"\r\"c\",\"d\"\r"
                                             << ','
                                             << '"'
                                             << "UTF-8"
                                             << 2
                                             << result;

    // The parser reads the file in chunks of 4096 characters. Make sure the line break and the escaped quote are parsed correctly
    // no matter where the chunk boundary falls.
    for(int length = 4085; length <= 4097; length++)
    {
        const QString first_field = QString(length, 'a');
        result.clear();
        result = {
            {first_field.toUtf8(), "b\"c"},
            {"d\r\ne", "f"}
        };
        QTest::newRow(QString("chunk_boundary_%1").arg(length).toUtf8().constData())
                << first_field + ",\"b\"\"c\"\r\n\"d\r\ne\",f\r\n"
                << ','
                << '"'
                << "UTF-8"
                << 2
                << result;
    }

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

void TestImport::csvQuotedFields()
{
    QFETCH(QByteArray, csv);
    QFETCH(QChar, quote);
    QFETCH(QStringList, expected);

    QBuffer buffer(&csv);
    QVERIFY(buffer.open(QIODevice::ReadOnly));
    QTextStream tstream(&buffer);
// Qt6 QTextStream defaults to utf-8, with Qt5 the codec of the locale would be used which is not UTF-8 on Windows.
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    tstream.setCodec("UTF-8");
#endif

    // Each row is rendered as a string with a "|" between the fields and a "*" after every field which was quoted
    QStringList parsed;
    CSVParser csvparser(true, ',', quote.unicode());
    QCOMPARE(csvparser.parse([&parsed](size_t /*rowNum*/, const CSVRow& data) -> bool {
        QStringList row;
        for(size_t i=0;i<data.num_fields;i++)
            row.push_back(QString::fromUtf8(data.fields[i].data, static_cast<int>(data.fields[i].data_length)) + (data.fields[i].quoted ? "*" : ""));
        parsed.push_back(row.join("|"));
        return true;
    }, tstream), CSVParser::ParserResultSuccess);

    QCOMPARE(parsed, expected);
}

void TestImport::csvQuotedFields_data()
{
    QTest::addColumn<QByteArray>("csv");
    QTest::addColumn<QChar>("quote");
    QTest::addColumn<QStringList>("expected");

    QTest::newRow("unquoted") << QByteArray("007,123,abc\n")
                              << QChar('"')
                              << QStringList{"007|123|abc"};
    QTest::newRow("mixed") << QByteArray("\"007\",123,\"abc\"\n")
                           << QChar('"')
                           << QStringList{"007*|123|abc*"};
    // The state must not be carried over to the following fields, rows or to the fields reused for them
    QTest::newRow("reset_between_rows") << QByteArray("\"1\",\"2\",\"3\"\r\n4,5,6\r\n\"7\",8,\"9\"\n10,\"11\",12")
                                        << QChar('"')
                                        << QStringList{"1*|2*|3*", "4|5|6", "7*|8|9*", "10|11*|12"};
    QTest::newRow("more_than_initial_fields") << QByteArray("a,b,c,d,e,f,g,\"h\",i\n1,2,3,4,5,6,7,8,9\n")
                                              << QChar('"')
                                              << QStringList{"a|b|c|d|e|f|g|h*|i", "1|2|3|4|5|6|7|8|9"};
    QTest::newRow("empty_quoted") << QByteArray("\"\",,\"\"\n")
                                  << QChar('"')
                                  << QStringList{"*||*"};
    QTest::newRow("escaped_quote_and_separator") << QByteArray("\"a\"\"b\",\"c,d\"\n")
                                                 << QChar('"')
                                                 << QStringList{"a\"b*|c,d*"};
    QTest::newRow("last_without_linebreak") << QByteArray("1,\"2\"")
                                            << QChar('"')
                                            << QStringList{"1|2*"};
    QTest::newRow("last_empty_without_linebreak") << QByteArray("1\n\"\"")
                                                  << QChar('"')
                                                  << QStringList{"1", "*"};
    QTest::newRow("oldmaclinebreaks") << QByteArray("\"1\",2\r3,\"4\"\r")
                                      << QChar('"')
                                      << QStringList{"1*|2", "3|4*"};
    QTest::newRow("other_quote_char") << QByteArray("'1',\"2\"\n")
                                      << QChar('\'')
                                      << QStringList{"1*|\"2\""};
    QTest::newRow("no_quote_char") << QByteArray("\"1\",2\n")
                                   << QChar()
                                   << QStringList{"\"1\"|2"};
}

void TestImport::csvQuoteField()
{
    QFETCH(QString, content);
    QFETCH(QChar, separator);
    QFETCH(QChar, quote);
    QFETCH(QString, newline);
    QFETCH(QString, expected);

    QCOMPARE(quoteCsvField(content, separator, quote, newline), expected);
}

void TestImport::csvQuoteField_data()
{
    QTest::addColumn<QString>("content");
    QTest::addColumn<QChar>("separator");
    QTest::addColumn<QChar>("quote");
    QTest::addColumn<QString>("newline");
    QTest::addColumn<QString>("expected");

    QTest::newRow("plain") << "abc" << QChar(',') << QChar('"') << "\n" << "abc";
    QTest::newRow("empty") << "" << QChar(',') << QChar('"') << "\n" << "";
    QTest::newRow("separator") << "a,b" << QChar(',') << QChar('"') << "\n" << "\"a,b\"";
    QTest::newRow("other_separator") << "a,b;c" << QChar(';') << QChar('"') << "\n" << "\"a,b;c\"";
    QTest::newRow("quote") << "a\"b" << QChar(',') << QChar('"') << "\n" << "\"a\"\"b\"";
    QTest::newRow("quote_only") << "\"" << QChar(',') << QChar('"') << "\n" << "\"\"\"\"";
    QTest::newRow("other_quote") << "a'b\"c" << QChar(',') << QChar('\'') << "\n" << "'a''b\"c'";
    QTest::newRow("lf") << "a\nb" << QChar(',') << QChar('"') << "\n" << "\"a\nb\"";
    QTest::newRow("crlf") << "a\r\nb" << QChar(',') << QChar('"') << "\r\n" << "\"a\r\nb\"";
    QTest::newRow("lf_with_crlf_newline") << "a\nb" << QChar(',') << QChar('"') << "\r\n" << "\"a\nb\"";
    QTest::newRow("cr_with_crlf_newline") << "a\rb" << QChar(',') << QChar('"') << "\r\n" << "\"a\rb\"";
    // A lone CR is a line break for many CSV readers, including the one of this application
    QTest::newRow("cr_with_lf_newline") << "a\rb" << QChar(',') << QChar('"') << "\n" << "\"a\rb\"";
    QTest::newRow("crlf_with_lf_newline") << "a\r\nb" << QChar(',') << QChar('"') << "\n" << "\"a\r\nb\"";
    QTest::newRow("non_ascii_quote") << QString::fromUtf8("a\xC2\xAB" "b") << QChar(',') << QChar(0xAB) << "\n" << QString::fromUtf8("\xC2\xAB" "a\xC2\xAB\xC2\xAB" "b\xC2\xAB");
    // The UTF-8 encoding of the e acute contains a byte which is also the quote character in Latin-1
    QTest::newRow("non_ascii_no_false_positive") << QString::fromUtf8("\xC3\xA9") << QChar(',') << QChar(0xA9) << "\n" << QString::fromUtf8("\xC3\xA9");
    QTest::newRow("leading_trailing_spaces") << " a " << QChar(',') << QChar('"') << "\n" << " a ";
    QTest::newRow("no_separator") << "a,b" << QChar() << QChar('"') << "\n" << "a,b";
    QTest::newRow("no_separator_lf") << "a\nb" << QChar() << QChar('"') << "\n" << "\"a\nb\"";

    // Without quote character no quoting is done except when it is unavoidable because of line breaks
    QTest::newRow("noquote_plain") << "abc" << QChar(',') << QChar() << "\n" << "abc";
    QTest::newRow("noquote_separator") << "a,b" << QChar(',') << QChar() << "\n" << "a,b";
    QTest::newRow("noquote_quote") << "a\"b" << QChar(',') << QChar() << "\n" << "a\"b";
    QTest::newRow("noquote_lf") << "a\nb" << QChar(',') << QChar() << "\n" << "\"a\nb\"";
    QTest::newRow("noquote_lf_quote") << "a\"\nb" << QChar(',') << QChar() << "\n" << "\"a\"\"\nb\"";
    QTest::newRow("noquote_crlf") << "a\r\nb" << QChar(',') << QChar() << "\r\n" << "\"a\r\nb\"";
    QTest::newRow("noquote_cr_with_lf_newline") << "a\rb" << QChar(',') << QChar() << "\n" << "\"a\rb\"";
}

void TestImport::csvExportImportRoundTrip()
{
    QFETCH(QString, newline);
    QFETCH(QChar, separator);
    QFETCH(QChar, quote);

    // Values which are difficult for CSV files. There is no leading or trailing white space because the parser trims by default
    // and RFC 4180 does not say how to preserve it.
    const std::vector<std::vector<QByteArray>> table{
        {"plain", "with,comma", "with;semicolon", "with\ttab"},
        {"say \"hi\"", "\"", "\"\"", "'single'"},
        {"line\nfeed", "carriage\rreturn", "both\r\nin a row", "\n"},
        {"", "", "last", ""},
        {"x", "\r", "\r\n\r\n", "\"\r\n\""},
        {QByteArray("\xC3\xA9\xE2\x82\xAC"), "a,\"b\"\n", "", "end"}
    };

    // Write the table in the same way the export dialog does it
    QString csv;
    for(const auto& row : table)
    {
        for(size_t i=0;i<row.size();i++)
        {
            csv += quoteCsvField(QString::fromUtf8(row.at(i)), separator, quote, newline);
            if(i != row.size() - 1)
                csv += separator;
        }
        csv += newline;
    }

    QByteArray csv_data = csv.toUtf8();
    QBuffer buffer(&csv_data);
    QVERIFY(buffer.open(QIODevice::ReadOnly));
    QTextStream tstream(&buffer);
// Qt6 QTextStream defaults to utf-8, with Qt5 the codec of the locale would be used which is not UTF-8 on Windows.
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    tstream.setCodec("UTF-8");
#endif

    std::vector<std::vector<QByteArray>> parsedCsv;
    CSVParser csvparser(false, separator.unicode(), quote.unicode());
    QCOMPARE(csvparser.parse([&parsedCsv](size_t /*rowNum*/, const CSVRow& data) -> bool {
        std::vector<QByteArray> row;
        for(size_t i=0;i<data.num_fields;i++)
            row.push_back(QByteArray(data.fields[i].data, static_cast<int>(data.fields[i].data_length)));
        parsedCsv.push_back(row);
        return true;
    }, tstream), CSVParser::ParserResultSuccess);

    QCOMPARE(parsedCsv, table);
}

void TestImport::csvExportImportRoundTrip_data()
{
    QTest::addColumn<QString>("newline");
    QTest::addColumn<QChar>("separator");
    QTest::addColumn<QChar>("quote");

    QTest::newRow("crlf") << "\r\n" << QChar(',') << QChar('"');
    QTest::newRow("lf") << "\n" << QChar(',') << QChar('"');
    QTest::newRow("lf_semicolon") << "\n" << QChar(';') << QChar('"');
    QTest::newRow("lf_tab_singlequote") << "\n" << QChar('\t') << QChar('\'');
    QTest::newRow("crlf_tab_singlequote") << "\r\n" << QChar('\t') << QChar('\'');
    QTest::newRow("cr") << "\r" << QChar(',') << QChar('"');
}
