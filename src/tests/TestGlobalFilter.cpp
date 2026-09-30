#include "TestGlobalFilter.h"

#include "../CondFormat.h"
#include "../GlobalFilter.h"
#include "../sql/Query.h"

#include <QRegularExpression>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QtTest/QtTest>
#include <sqlite3.h>

QTEST_APPLESS_MAIN(TestGlobalFilter)

namespace
{

QStringList asStringList(const std::vector<QString>& values)
{
    QStringList result;
    for(const auto& value : values)
        result.push_back(value);
    return result;
}

void regexp(sqlite3_context* context, int argc, sqlite3_value** argv)
{
    if(argc != 2 || sqlite3_value_type(argv[0]) == SQLITE_NULL || sqlite3_value_type(argv[1]) == SQLITE_NULL)
    {
        sqlite3_result_int(context, 0);
        return;
    }

    const auto pattern = reinterpret_cast<const char*>(sqlite3_value_text(argv[0]));
    const auto value = reinterpret_cast<const char*>(sqlite3_value_text(argv[1]));
    sqlite3_result_int(context, QRegularExpression(QString::fromUtf8(pattern)).match(QString::fromUtf8(value)).hasMatch());
}

QStringList executeIssueQuery(const std::vector<QString>& filters)
{
    sqlb::Query query(sqlb::ObjectIdentifier("main", "SomeCities"));
    query.setColumnNames({"Name"});
    for(const auto& filter : filters)
        query.globalWhere().push_back(CondFormat::filterToSqlCondition(filter));

    sqlite3* db = nullptr;
    if(sqlite3_open(":memory:", &db) != SQLITE_OK)
        qFatal("Could not open the SQLite fixture");
    if(sqlite3_create_function(db, "regexp", 2, SQLITE_UTF8, nullptr, regexp, nullptr, nullptr) != SQLITE_OK)
        qFatal("Could not register REGEXP for the SQLite fixture");
    if(sqlite3_exec(db,
                    "CREATE TABLE SomeCities(Name TEXT);"
                    "INSERT INTO SomeCities VALUES ('New York'), ('Hawaii'), ('San Francisco');",
                    nullptr, nullptr, nullptr) != SQLITE_OK)
        qFatal("Could not create the SQLite fixture");

    QStringList rows;
    char* error = nullptr;
    const std::string sql = query.buildQuery(false) + " ORDER BY Name";
    const int result = sqlite3_exec(db, sql.c_str(), [](void* data, int count, char** values, char**) {
        if(count > 0 && values[0])
            static_cast<QStringList*>(data)->push_back(QString::fromUtf8(values[0]));
        return 0;
    }, &rows, &error);
    const QString errorMessage = error ? QString::fromUtf8(error) : QString();
    sqlite3_free(error);
    sqlite3_close(db);
    if(result != SQLITE_OK)
        qFatal("Issue fixture query failed: %s", qPrintable(errorMessage));
    return rows;
}

}

void TestGlobalFilter::tokenizes_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QStringList>("expected");

    QTest::newRow("regex-with-space") << "/New York/" << QStringList{"/New York/"};
    QTest::newRow("ordinary-and-terms") << "foo bar" << QStringList{"foo", "bar"};
    QTest::newRow("mixed") << "foo /New York/ bar" << QStringList{"foo", "/New York/", "bar"};
    QTest::newRow("multiple-regexes") << "/New York/ /San Francisco/" << QStringList{"/New York/", "/San Francisco/"};
    QTest::newRow("later-unmatched-slash-falls-back") << "/New York/ /San Francisco" << QStringList{"/New", "York/", "/San", "Francisco"};
    QTest::newRow("escaped-slash") << R"(/New \/ York/)" << QStringList{R"(/New \/ York/)"};
    QTest::newRow("unmatched-slash") << "/New York" << QStringList{"/New", "York"};
    QTest::newRow("repeated-spaces") << "  foo   /New York/   bar  " << QStringList{"foo", "/New York/", "bar"};
    QTest::newRow("closing-slash-needs-boundary") << "/foo/xyz bar" << QStringList{"/foo/xyz", "bar"};
    QTest::newRow("slash-literal-before-regex-falls-back") << "/usr/bin /New York/" << QStringList{"/usr/bin", "/New", "York/"};
    QTest::newRow("empty-regex") << "//" << QStringList{"//"};
    QTest::newRow("tabs-are-not-separators") << "alpha\tbeta gamma" << QStringList{"alpha\tbeta", "gamma"};
    QTest::newRow("even-backslash-parity") << R"(/a\\/ b)" << QStringList{R"(/a\\/)", "b"};
    QTest::newRow("odd-backslash-parity") << R"(/a\/ b/)" << QStringList{R"(/a\/ b/)"};
}

void TestGlobalFilter::tokenizes()
{
    QFETCH(QString, input);
    QFETCH(QStringList, expected);
    QCOMPARE(asStringList(GlobalFilter::tokenize(input)), expected);
}

void TestGlobalFilter::issueFixtureReturnsNewYork()
{
    QCOMPARE(executeIssueQuery(GlobalFilter::tokenize("/New York/")), QStringList{"New York"});
}

void TestGlobalFilter::ordinaryTermsRemainAndConnected()
{
    sqlb::Query query(sqlb::ObjectIdentifier("main", "SomeCities"));
    query.setColumnNames({"Name"});
    const auto filters = GlobalFilter::tokenize("New York");
    QCOMPARE(filters.size(), size_t{2});
    for(const auto& filter : filters)
        query.globalWhere().push_back(CondFormat::filterToSqlCondition(filter));
    QVERIFY(QString::fromStdString(query.buildQuery(false)).contains(") AND ("));
}

void TestGlobalFilter::projectRoundTripPreservesRegexTerm()
{
    QByteArray project;
    QXmlStreamWriter writer(&project);
    writer.writeStartDocument();
    writer.writeStartElement("table");
    GlobalFilter::save(GlobalFilter::tokenize("/New York/"), writer);
    writer.writeEndElement();
    writer.writeEndDocument();

    std::vector<QString> restored;
    QXmlStreamReader reader(project);
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement() && reader.name() == QT_UNICODE_LITERAL("global_filter"))
            GlobalFilter::load(restored, reader);
    }

    QVERIFY(!reader.hasError());
    QCOMPARE(asStringList(restored), QStringList{"/New York/"});
    QCOMPARE(executeIssueQuery(restored), QStringList{"New York"});
}
