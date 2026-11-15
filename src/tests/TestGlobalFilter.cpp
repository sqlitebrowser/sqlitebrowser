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

// Runs the statements in a fresh in-memory database and returns the first column of all rows of the last query,
// joined by commas
QString runQuery(const char* setup, const std::string& query)
{
    sqlite3* db = nullptr;
    if(sqlite3_open(":memory:", &db) != SQLITE_OK)
        qFatal("Could not open the SQLite fixture");
    if(sqlite3_exec(db, setup, nullptr, nullptr, nullptr) != SQLITE_OK)
        qFatal("Could not create the SQLite fixture: %s", sqlite3_errmsg(db));

    QStringList rows;
    const int result = sqlite3_exec(db, query.c_str(), [](void* data, int count, char** values, char**) {
        if(count > 0 && values[0])
            static_cast<QStringList*>(data)->push_back(QString::fromUtf8(values[0]));
        return 0;
    }, &rows, nullptr);
    if(result != SQLITE_OK)
        qFatal("Fixture query failed: %s", sqlite3_errmsg(db));
    sqlite3_close(db);
    return rows.join(',');
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

void TestGlobalFilter::exactFilterMatches_data()
{
    QTest::addColumn<QString>("setup");
    QTest::addColumn<QString>("filter");
    QTest::addColumn<QString>("expected");

    // The result of COUNT(*) has no type affinity, so SQLite never considers it equal to a quoted string (#3608)
    QTest::newRow("count column")
        << "CREATE TABLE d(g); INSERT INTO d VALUES (1),(2),(2),(3),(3),(3);"
           "CREATE VIEW t AS SELECT COUNT(*) AS c FROM d GROUP BY g;"
        << "=2" << "2";
    QTest::newRow("count column zero")
        << "CREATE TABLE d(g); CREATE TABLE e(g); INSERT INTO d VALUES (1),(2),(2);"
           "INSERT INTO e VALUES (1),(2),(3);"
           "CREATE VIEW t AS SELECT COUNT(d.g) AS c FROM e LEFT JOIN d ON d.g = e.g GROUP BY e.g;"
        << "=0" << "0";
    QTest::newRow("integer column")
        << "CREATE TABLE t(c INTEGER); INSERT INTO t VALUES (4),(5),(50);"
        << "=5" << "5";
    QTest::newRow("negative integer column")
        << "CREATE TABLE t(c INTEGER); INSERT INTO t VALUES (-3),(3),(-30);"
        << "=-3" << "-3";
    QTest::newRow("real column")
        << "CREATE TABLE t(c REAL); INSERT INTO t VALUES (2.5),(25),(2);"
        << "=2.5" << "2.5";
    QTest::newRow("computed real")
        << "CREATE TABLE d(x); INSERT INTO d VALUES (1),(2);"
           "CREATE VIEW t AS SELECT x / 2.0 AS c FROM d;"
        << "=0.5" << "0.5";

    // Text columns must keep matching the text which is stored in them, even if it looks like a number
    QTest::newRow("text column")
        << "CREATE TABLE t(c TEXT); INSERT INTO t VALUES ('5'),('50'),('05');"
        << "=5" << "5";
    QTest::newRow("text column leading zeros")
        << "CREATE TABLE t(c TEXT); INSERT INTO t VALUES ('007'),('7'),('70');"
        << "=007" << "007";
    QTest::newRow("text column trailing zero")
        << "CREATE TABLE t(c TEXT); INSERT INTO t VALUES ('9.90'),('9.9'),('9');"
        << "=9.90" << "9.90";
    QTest::newRow("text column too long for a number")
        << "CREATE TABLE t(c TEXT); INSERT INTO t VALUES ('12345678901234567890123'),('1');"
        << "=12345678901234567890123" << "12345678901234567890123";
    QTest::newRow("text column word")
        << "CREATE TABLE t(c TEXT); INSERT INTO t VALUES ('abc'),('abcd');"
        << "=abc" << "abc";
}

void TestGlobalFilter::exactFilterMatches()
{
    QFETCH(QString, setup);
    QFETCH(QString, filter);
    QFETCH(QString, expected);

    const std::string query = "SELECT group_concat(c) FROM (SELECT c FROM t WHERE c " + CondFormat::filterToSqlCondition(filter) + " ORDER BY c)";
    QCOMPARE(runQuery(setup.toUtf8().constData(), query), expected);
}

void TestGlobalFilter::exactFilterQuoting_data()
{
    QTest::addColumn<QString>("filter");
    QTest::addColumn<QString>("expected");

    QTest::newRow("integer") << "=2" << "= 2 ";
    QTest::newRow("zero") << "=0" << "= 0 ";
    QTest::newRow("negative") << "=-3" << "= -3 ";
    QTest::newRow("decimal") << "=2.5" << "= 2.5 ";
    QTest::newRow("small decimal") << "=0.5" << "= 0.5 ";
    QTest::newRow("largest integer") << "=9223372036854775807" << "= 9223372036854775807 ";

    // These would not be the same value after being converted to a number and back, or are no numbers at all.
    // They must stay quoted because SQLite would read them differently or not at all, e.g. inf as a column name.
    QTest::newRow("leading zeros") << "=007" << "= '007' ";
    QTest::newRow("trailing zero") << "=1.50" << "= '1.50' ";
    QTest::newRow("exponent") << "=1e5" << "= '1e5' ";
    QTest::newRow("explicit plus") << "=+5" << "= '+5' ";
    QTest::newRow("negative zero") << "=-0" << "= '-0' ";
    QTest::newRow("no integer part") << "=.5" << "= '.5' ";
    QTest::newRow("no fraction part") << "=5." << "= '5.' ";
    QTest::newRow("hexadecimal") << "=0x10" << "= '0x10' ";
    QTest::newRow("infinity") << "=inf" << "= 'inf' ";
    QTest::newRow("not a number") << "=nan" << "= 'nan' ";
    QTest::newRow("beyond 64 bit integers") << "=9223372036854775808" << "= '9223372036854775808' ";
    QTest::newRow("text") << "=abc" << "= 'abc' ";
    QTest::newRow("null") << "=NULL" << "IS NULL ";
}

void TestGlobalFilter::exactFilterQuoting()
{
    QFETCH(QString, filter);
    QFETCH(QString, expected);
    QCOMPARE(QString::fromStdString(CondFormat::filterToSqlCondition(filter)), expected);
}
