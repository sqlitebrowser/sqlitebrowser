#include "testsqlobjects.h"
#include "../sql/ObjectIdentifier.h"
#include "../sql/sqlitetypes.h"

#include <QtTest/QtTest>

QTEST_APPLESS_MAIN(TestTable)
Q_DECLARE_METATYPE(std::string)

using namespace sqlb;

#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
namespace QTest
{
template <typename T1, typename T2>
inline bool qCompare(const T1 &t1, const T2 &t2, const char *actual, const char *expected,
                     const char *file, int line)
{
    return compare_helper(t1 == t2, "Compared values are not the same",
                          toString(t1), toString(t2), actual, expected, file, line);
}
}
#endif

void TestTable::sqlOutput()
{
    Table tt("testtable");
    Field f("id", "integer");
    Field fkm("km", "integer", false, "", "km > 1000");
    tt.fields.push_back(f);
    tt.fields.emplace_back("car", "text");
    tt.fields.push_back(fkm);
    tt.addConstraint(ConstraintPtr(new PrimaryKeyConstraint({f.name(), fkm.name()})));

    QCOMPARE(tt.sql(), "CREATE TABLE \"testtable\" (\n"
                       "\t\"id\"\tinteger,\n"
                       "\t\"car\"\ttext,\n"
                       "\t\"km\"\tinteger CHECK(km > 1000),\n"
                       "\tPRIMARY KEY(\"id\",\"km\")\n"
                       ");");
}

void TestTable::sqlGraveAccentOutput()
{
    Table tt("testtable");
    Field f("id", "integer");
    Field fkm("km", "integer", false, "", "km > 1000");
    tt.fields.push_back(f);
    tt.fields.emplace_back("car", "text");
    tt.fields.push_back(fkm);
    tt.addConstraint(ConstraintPtr(new PrimaryKeyConstraint({f.name(), fkm.name()})));
    sqlb::setIdentifierQuoting(sqlb::GraveAccents);

    QCOMPARE(tt.sql(), "CREATE TABLE `testtable` (\n"
                       "\t`id`\tinteger,\n"
                       "\t`car`\ttext,\n"
                       "\t`km`\tinteger CHECK(km > 1000),\n"
                       "\tPRIMARY KEY(`id`,`km`)\n"
                       ");");

    sqlb::setIdentifierQuoting(sqlb::DoubleQuotes);
}


void TestTable::sqlSquareBracketsOutput()
{
    Table tt("testtable");
    Field f("id", "integer");
    Field fkm("km", "integer", false, "", "km > 1000");
    tt.fields.push_back(f);
    tt.fields.emplace_back("car", "text");
    tt.fields.push_back(fkm);
    tt.addConstraint(ConstraintPtr(new PrimaryKeyConstraint({f.name(), fkm.name()})));
    sqlb::setIdentifierQuoting(sqlb::SquareBrackets);

    QCOMPARE(tt.sql(), "CREATE TABLE [testtable] (\n"
                       "\t[id]\tinteger,\n"
                       "\t[car]\ttext,\n"
                       "\t[km]\tinteger CHECK(km > 1000),\n"
                       "\tPRIMARY KEY([id],[km])\n"
                       ");");

    sqlb::setIdentifierQuoting(sqlb::DoubleQuotes);
}

void TestTable::autoincrement()
{
    Table tt("testtable");
    Field f("id", "integer");
    Field fkm("km", "integer");
    tt.fields.push_back(f);
    tt.fields.emplace_back("car", "text");
    tt.fields.push_back(fkm);
    PrimaryKeyConstraint pk({f.name()});
    pk.setAutoIncrement(true);
    tt.addConstraint(ConstraintPtr(new PrimaryKeyConstraint(pk)));

    QCOMPARE(tt.sql(), "CREATE TABLE \"testtable\" (\n"
                       "\t\"id\"\tinteger,\n"
                       "\t\"car\"\ttext,\n"
                       "\t\"km\"\tinteger,\n"
                       "\tPRIMARY KEY(\"id\" AUTOINCREMENT)\n"
                       ");");
}

void TestTable::notnull()
{
    Table tt("testtable");
    Field f("id", "integer");
    Field fkm("km", "integer");
    tt.fields.push_back(f);
    tt.fields.emplace_back("car", "text", true);
    tt.fields.push_back(fkm);
    PrimaryKeyConstraint pk({f.name()});
    pk.setAutoIncrement(true);
    tt.addConstraint(ConstraintPtr(new PrimaryKeyConstraint(pk)));

    QCOMPARE(tt.sql(), "CREATE TABLE \"testtable\" (\n"
                       "\t\"id\"\tinteger,\n"
                       "\t\"car\"\ttext NOT NULL,\n"
                       "\t\"km\"\tinteger,\n"
                       "\tPRIMARY KEY(\"id\" AUTOINCREMENT)\n"
                       ");");
}

void TestTable::withoutRowid()
{
    Table tt("testtable");
    Field f("a", "integer");
    tt.fields.push_back(f);
    tt.fields.emplace_back("b", "integer");
    tt.setWithoutRowidTable(true);
    tt.addConstraint(ConstraintPtr(new PrimaryKeyConstraint({f.name()})));

    QCOMPARE(tt.sql(), "CREATE TABLE \"testtable\" (\n"
                       "\t\"a\"\tinteger,\n"
                       "\t\"b\"\tinteger,\n"
                       "\tPRIMARY KEY(\"a\")\n"
                       ") WITHOUT ROWID;");
}

void TestTable::foreignKeys()
{
    Table tt("testtable");
    Field f("a", "integer");
    tt.fields.push_back(f);
    sqlb::ConstraintPtr fk = sqlb::ConstraintPtr(new sqlb::ForeignKeyClause("b", sqlb::StringVector{"c"}));
    fk->setColumnList({f.name()});
    tt.addConstraint(fk);

    QCOMPARE(tt.sql(), "CREATE TABLE \"testtable\" (\n"
                       "\t\"a\"\tinteger,\n"
                       "\tFOREIGN KEY(\"a\") REFERENCES \"b\"(\"c\")\n"
                       ");");
}

void TestTable::uniqueConstraint()
{
    Table tt("testtable");
    Field f1("a", "integer");
    Field f2("b", "integer");
    Field f3("c", "integer");
    f1.setUnique(true);
    tt.fields.push_back(f1);
    tt.fields.push_back(f2);
    tt.fields.push_back(f3);
    tt.addConstraint(sqlb::ConstraintPtr(new sqlb::UniqueConstraint({f2.name(), f3.name()})));

    QCOMPARE(tt.sql(), "CREATE TABLE \"testtable\" (\n"
                       "\t\"a\"\tinteger UNIQUE,\n"
                       "\t\"b\"\tinteger,\n"
                       "\t\"c\"\tinteger,\n"
                       "\tUNIQUE(\"b\",\"c\")\n"
                       ");");
}

void TestTable::parseSQL()
{
    std::string sSQL = "create TABLE hero (\n"
                       "\tid integer PRIMARY KEY AUTOINCREMENT,\n"
                       "\tname text NOT NULL DEFAULT 'xxxx',\n"
                       "\tinfo VARCHAR(255) CHECK (info == 'x')\n"
                       ");";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL))));

    QCOMPARE(tab.name(), "hero");
    QCOMPARE(tab.rowidColumns(), {"_rowid_"});
    QCOMPARE(tab.fields.at(0).name(), "id");
    QCOMPARE(tab.fields.at(1).name(), "name");
    QCOMPARE(tab.fields.at(2).name(), "info");

    QCOMPARE(tab.fields.at(0).type(), "integer");
    QCOMPARE(tab.fields.at(1).type(), "text");
    QCOMPARE(tab.fields.at(2).type(), "VARCHAR(255)");

    auto pk = tab.primaryKey();
    QVERIFY(pk->autoIncrement());
    QCOMPARE(pk->columnList().size(), 1);
    QCOMPARE(pk->columnList().at(0), tab.fields.at(0).name());
    QVERIFY(tab.fields.at(1).notnull());
    QCOMPARE(tab.fields.at(1).defaultValue(), "'xxxx'");
    QCOMPARE(tab.fields.at(1).check(), "");
    QCOMPARE(tab.fields.at(2).check(), "\"info\" == 'x'");
}

void TestTable::parseSQLdefaultexpr()
{
    std::string sSQL = "CREATE TABLE chtest(\n"
                       "id integer primary key,\n"
                       "dumpytext text default('axa') CHECK(dumpytext == \"aa\"),\n"
                       "date datetime default CURRENT_TIMESTAMP,"
                       "zoi integer)";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL))));

    QCOMPARE(tab.name(), "chtest");
    QCOMPARE(tab.fields.at(0).name(), "id");
    QCOMPARE(tab.fields.at(1).name(), "dumpytext");
    QCOMPARE(tab.fields.at(2).name(), "date");
    QCOMPARE(tab.fields.at(3).name(), "zoi");

    QCOMPARE(tab.fields.at(0).type(), "integer");
    QCOMPARE(tab.fields.at(1).type(), "text");
    QCOMPARE(tab.fields.at(2).type(), "datetime");
    QCOMPARE(tab.fields.at(3).type(), "integer");

    QCOMPARE(tab.fields.at(1).defaultValue(), "('axa')");
    QCOMPARE(tab.fields.at(1).check(), "\"dumpytext\" == \"aa\"");
    QCOMPARE(tab.fields.at(2).defaultValue(), "CURRENT_TIMESTAMP");
    QCOMPARE(tab.fields.at(2).check(), "");
    QCOMPARE(tab.fields.at(3).defaultValue(), "");
    QCOMPARE(tab.fields.at(3).check(), "");

    auto pk = tab.primaryKey();
    QCOMPARE(pk->columnList().size(), 1);
    QCOMPARE(pk->columnList().at(0), tab.fields.at(0).name());
}

void TestTable::parseSQLMultiPk()
{
    std::string sSQL = "CREATE TABLE hero (\n"
                       "\tid1 integer,\n"
                       "\tid2 integer,\n"
                       "\tnonpkfield blob,\n"
                       "PRIMARY KEY(\"id1\",\"id2\")\n"
                       ");";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL))));

    QCOMPARE(tab.name(), "hero");
    QCOMPARE(tab.fields.at(0).name(), "id1");
    QCOMPARE(tab.fields.at(1).name(), "id2");

    QCOMPARE(tab.fields.at(0).type(), "integer");
    QCOMPARE(tab.fields.at(1).type(), "integer");

    auto pk = tab.primaryKey();
    QCOMPARE(pk->columnList().size(), 2);
    QCOMPARE(pk->columnList().at(0), tab.fields.at(0).name());
    QCOMPARE(pk->columnList().at(1), tab.fields.at(1).name());
}

void TestTable::parseSQLForeignKey()
{
    std::string sSQL = "CREATE TABLE grammar_test(id, test, FOREIGN KEY(test) REFERENCES other_table);";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL))));

    QCOMPARE(tab.name(), "grammar_test");
    QCOMPARE(tab.fields.at(0).name(), "id");
    QCOMPARE(tab.fields.at(1).name(), "test");
}

void TestTable::parseSQLSingleQuotes()
{
    std::string sSQL = "CREATE TABLE 'test'('id','test');";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL))));

    QCOMPARE(tab.name(), "test");
    QCOMPARE(tab.fields.at(0).name(), "id");
    QCOMPARE(tab.fields.at(1).name(), "test");
}

void TestTable::parseSQLSquareBrackets()
{
    std::string sSQL = "CREATE TABLE [test]([id],[test]);";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL))));

    QCOMPARE(tab.name(), "test");
    QCOMPARE(tab.fields.at(0).name(), "id");
    QCOMPARE(tab.fields.at(1).name(), "test");
}


void TestTable::parseSQLKeywordInIdentifier()
{
    std::string sSQL = "CREATE TABLE deffered(key integer primary key, if text);";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL))));

    QCOMPARE(tab.name(), "deffered");
    QCOMPARE(tab.fields.at(0).name(), "key");
    QCOMPARE(tab.fields.at(1).name(), "if");
}


void TestTable::parseSQLSomeKeywordsInIdentifier()
{
    std::string sSQL = "CREATE TABLE \"Average Number of Volunteers by Area of Work\" ("
                       "`Area of Work`	TEXT,"
                       "`Average Number of Volunteers`	INTEGER);";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL))));

    QCOMPARE(tab.name(), "Average Number of Volunteers by Area of Work");
    QCOMPARE(tab.fields.at(0).name(), "Area of Work");
    QCOMPARE(tab.fields.at(1).name(), "Average Number of Volunteers");
}

void TestTable::parseSQLWithoutRowid()
{
    std::string sSQL = "CREATE TABLE test(a integer primary key, b integer) WITHOUT ROWID;";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL))));

    QCOMPARE(tab.primaryKey()->columnList(), {"a"});
    QCOMPARE(tab.rowidColumns(), {"a"});
}

void TestTable::parseNonASCIIChars()
{
    std::string sSQL = "CREATE TABLE `lösung` ("
                       "`Fieldöäüß`	INTEGER,"
                       "PRIMARY KEY(`Fieldöäüß`)"
                       ");";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL))));

    QCOMPARE(tab.name(), "lösung");
    QCOMPARE(tab.fields.at(0).name(), "Fieldöäüß");
}

void TestTable::parseNonASCIICharsEs()
{
    std::string sSQL = "CREATE TABLE \"Cigüeñas de Alcalá\" ("
                       "\"Field áéíóúÁÉÍÓÚñÑçÇ\"	INTEGER,"
                       "PRIMARY KEY(\"Field áéíóúÁÉÍÓÚñÑçÇ\")"
                       ");";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL))));

    QCOMPARE(tab.name(), "Cigüeñas de Alcalá");
    QCOMPARE(tab.fields.at(0).name(), "Field áéíóúÁÉÍÓÚñÑçÇ");
}

void TestTable::parseSQLEscapedQuotes()
{
    std::string sSql = "CREATE TABLE double_quotes(a text default 'a''a');";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSql))));

    QCOMPARE(tab.name(), "double_quotes");
    QCOMPARE(tab.fields.at(0).name(), "a");
    QCOMPARE(tab.fields.at(0).defaultValue(), "'a''a'");
}

void TestTable::parseSQLForeignKeys()
{
    std::string sql = "CREATE TABLE foreign_key_test(a int, b int, foreign key (a) references x, foreign key (b) references w(y,z) on delete set null);";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql))));

    QCOMPARE(tab.name(), "foreign_key_test");
    QCOMPARE(tab.fields.at(0).name(), "a");
    QCOMPARE(tab.fields.at(0).type(), "int");
    QCOMPARE(std::dynamic_pointer_cast<sqlb::ForeignKeyClause>(tab.constraint({tab.fields.at(0).name()}, sqlb::Constraint::ForeignKeyConstraintType))->table(), "x");
    QCOMPARE(tab.fields.at(1).name(), "b");
    QCOMPARE(tab.fields.at(1).type(), "int");
    QCOMPARE(std::dynamic_pointer_cast<sqlb::ForeignKeyClause>(tab.constraint({tab.fields.at(1).name()}, sqlb::Constraint::ForeignKeyConstraintType))->toString(), "\"w\"(\"y\",\"z\") on delete set null");
}

void TestTable::parseSQLCheckConstraint()
{
    std::string sql = "CREATE TABLE a (\"b\" text CHECK(\"b\"='A' or \"b\"='B'));";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql))));

    QCOMPARE(tab.name(), "a");
    QCOMPARE(tab.fields.at(0).name(), "b");
    QCOMPARE(tab.fields.at(0).type(), "text");
    QCOMPARE(tab.fields.at(0).check(), "\"b\" = 'A' OR \"b\" = 'B'");
}

void TestTable::parseDefaultValues()
{
    std::string sql = "CREATE TABLE test(a int DEFAULT 0, b int DEFAULT -1, c text DEFAULT 'hello', d text DEFAULT '0');";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql))));

    QCOMPARE(tab.name(), "test");
    QCOMPARE(tab.fields.at(0).name(), "a");
    QCOMPARE(tab.fields.at(0).type(), "int");
    QCOMPARE(tab.fields.at(0).defaultValue(), "0");
    QCOMPARE(tab.fields.at(1).name(), "b");
    QCOMPARE(tab.fields.at(1).type(), "int");
    QCOMPARE(tab.fields.at(1).defaultValue(), "-1");
    QCOMPARE(tab.fields.at(2).name(), "c");
    QCOMPARE(tab.fields.at(2).type(), "text");
    QCOMPARE(tab.fields.at(2).defaultValue(), "'hello'");
    QCOMPARE(tab.fields.at(3).name(), "d");
    QCOMPARE(tab.fields.at(3).type(), "text");
    QCOMPARE(tab.fields.at(3).defaultValue(), "'0'");
}

void TestTable::createTableWithIn()
{
    std::string sSQL = "CREATE TABLE not_working("
                       "_id PRIMARY KEY NOT NULL,"
                       "value NVARCHAR(5) CHECK (value IN ('a', 'b', 'c'))"
                       ");";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL))));
    QCOMPARE(tab.name(), "not_working");

    QCOMPARE(tab.fields.at(1).check(), "\"value\" IN ('a', 'b', 'c')");
}

void TestTable::createTableWithNotLikeConstraint()
{
    std::string sSQL = "CREATE TABLE hopefully_working(\n"
                       "value TEXT CHECK(value NOT LIKE 'prefix%'),\n"
                       "value2 TEXT CHECK(value2 NOT MATCH 'prefix%'),\n"
                       "value3 TEXT CHECK(value3 NOT REGEXP 'prefix%'),\n"
                       "value4 TEXT CHECK(value4 NOT GLOB 'prefix%'),\n"
                       "value5 INTEGER CHECK(value5 BETWEEN 1+4 AND 100 OR 200),\n"
                       "value6 INTEGER CHECK(value6 NOT BETWEEN 1 AND 100)\n"
                       ");";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL))));
    QCOMPARE(tab.name(), "hopefully_working");

    QCOMPARE(tab.fields.at(0).check(), "\"value\" NOT LIKE 'prefix%'");
    QCOMPARE(tab.fields.at(1).check(), "\"value2\" NOT MATCH 'prefix%'");
    QCOMPARE(tab.fields.at(2).check(), "\"value3\" NOT REGEXP 'prefix%'");
    QCOMPARE(tab.fields.at(3).check(), "\"value4\" NOT GLOB 'prefix%'");
    QCOMPARE(tab.fields.at(4).check(), "\"value5\" BETWEEN 1 + 4 AND 100 OR 200");
    QCOMPARE(tab.fields.at(5).check(), "\"value6\" NOT BETWEEN 1 AND 100");
}

void TestTable::rowValues()
{
    std::string sql = "CREATE TABLE test(\n"
                      "a INTEGER,\n"
                      "b INTEGER,\n"
                      "CHECK((a, b) = (1, 2))\n"
                      ");";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql))));
    QCOMPARE(tab.name(), "test");

    QCOMPARE(std::dynamic_pointer_cast<sqlb::CheckConstraint>(tab.constraint({}, sqlb::Constraint::CheckConstraintType))->expression(), "(\"a\", \"b\") = (1, 2)");
}

void TestTable::complexExpressions()
{
    std::string sql = "CREATE TABLE test(\n"
                      "a INTEGER CHECK((a > 0)),\n"
                      "b INTEGER CHECK((b > 0 and b > 1)),\n"
                      "c INTEGER CHECK((c = -1) or (c > 0 and c > 1) or (c = 0)),\n"
                      "d INTEGER CHECK((((d > 0))))\n"
                      ");";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql))));
    QCOMPARE(tab.name(), "test");

    QCOMPARE(tab.fields.at(0).check(), "(\"a\" > 0)");
    QCOMPARE(tab.fields.at(1).check(), "(\"b\" > 0 AND \"b\" > 1)");
    QCOMPARE(tab.fields.at(2).check(), "(\"c\" = -1) OR (\"c\" > 0 AND \"c\" > 1) OR (\"c\" = 0)");
    QCOMPARE(tab.fields.at(3).check(), "(((\"d\" > 0)))");
}

void TestTable::datetimeExpression()
{
    std::string sql = "CREATE TABLE test(\n"
                      "entry INTEGER DEFAULT (DATETIME(CURRENT_TIMESTAMP, 'LOCALTIME'))\n"
                      ");";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql))));
    QCOMPARE(tab.name(), "test");

    QCOMPARE(tab.fields.at(0).name(), "entry");
    QCOMPARE(tab.fields.at(0).type(), "INTEGER");
    QCOMPARE(tab.fields.at(0).defaultValue(), "(DATETIME(CURRENT_TIMESTAMP, 'LOCALTIME'))");
}

void TestTable::extraParentheses()
{
    std::string sql = "CREATE TABLE test(\n"
                      "xy INTEGER DEFAULT (1 + (5) - 4)\n"
                      ");";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql))));
    QCOMPARE(tab.name(), "test");

    QCOMPARE(tab.fields.at(0).name(), "xy");
    QCOMPARE(tab.fields.at(0).type(), "INTEGER");
    QCOMPARE(tab.fields.at(0).defaultValue(), "(1 + (5) - 4)");
}

void TestTable::moduloOperator()
{
    std::string sql = "CREATE TABLE test(\n"
                      "xy INTEGER DEFAULT (7 % 2)\n"
                      ");";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql))));
    QCOMPARE(tab.name(), "test");

    QCOMPARE(tab.fields.at(0).name(), "xy");
    QCOMPARE(tab.fields.at(0).type(), "INTEGER");
    QCOMPARE(tab.fields.at(0).defaultValue(), "(7 % 2)");
}

void TestTable::complexExpression()
{
    std::string sql = "CREATE TABLE test(\n"
                      "uuid INTEGER DEFAULT (hex(randomblob(4))||'-'||hex(randomblob(2))||'-'||'4'||substr(hex(randomblob(2)),2)||'-'||substr('AB89',1+(abs(random())%4),1)||substr(hex(randomblob(2)),2)||'-'||hex(randomblob(6))),\n"
                      "a INTEGER,\n"
                      "b INTEGER,\n"
                      "CHECK((a = 'S' AND b IS NOT NULL) OR (a IN ('A', 'P')))"
                      ");";

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql))));
    QCOMPARE(tab.name(), "test");

    QCOMPARE(tab.fields.at(0).name(), "uuid");
    QCOMPARE(tab.fields.at(0).type(), "INTEGER");
    QCOMPARE(tab.fields.at(0).defaultValue(), "(hex(randomblob(4)) || '-' || hex(randomblob(2)) || '-' || '4' || substr(hex(randomblob(2)), 2) || '-' || substr('AB89', 1 + (abs(random()) % 4), 1) || substr(hex(randomblob(2)), 2) || '-' || hex(randomblob(6)))");

    auto c = tab.constraints({}, sqlb::Constraint::CheckConstraintType);
    QCOMPARE(c.size(), 1);
    QCOMPARE(std::dynamic_pointer_cast<sqlb::CheckConstraint>(c.at(0))->expression(), "(\"a\" = 'S' AND \"b\" IS NOT NULL) OR (\"a\" IN ('A', 'P'))");
}

void TestTable::parseTest()
{
    QFETCH(std::string, sql);

    Table tab(*(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql))));
    QVERIFY(tab.fullyParsed());
}

void TestTable::parseTest_data()
{
    // These are some rather unspecific queries but we include them here to test some basic parser features. They are
    // extracted from this file: https://www.sqlite.org/cgi/src/artifact/1c602347e73ab80b

    QTest::addColumn<std::string>("sql");

    QTest::newRow("1") << std::string("CREATE TABLE t1(c1 one)");
    QTest::newRow("2") << std::string("CREATE TABLE t1(c1 one two)");
    QTest::newRow("3") << std::string("CREATE TABLE t1(c1 one two three)");
    QTest::newRow("4") << std::string("CREATE TABLE t1(c1 one two three four)");
    QTest::newRow("5") << std::string("CREATE TABLE t1(c1 one two three four(14))");
    QTest::newRow("6") << std::string("CREATE TABLE t1(c1 one two three four(14, 22))");
    QTest::newRow("7") << std::string("CREATE TABLE t1(c1 var(+14, -22.3))");
    QTest::newRow("8") << std::string("CREATE TABLE t1(c1 var(1.0e10))");
    QTest::newRow("9") << std::string("CREATE TABLE t1(c1 text PRIMARY KEY)");
    QTest::newRow("10") << std::string("CREATE TABLE t1(c1 text PRIMARY KEY ASC)");
    QTest::newRow("11") << std::string("CREATE TABLE t1(c1 text PRIMARY KEY DESC)");
    QTest::newRow("12") << std::string("CREATE TABLE t1(c1 text CONSTRAINT cons PRIMARY KEY DESC)");
    QTest::newRow("13") << std::string("CREATE TABLE t1(c1 text NOT NULL)");
    // TODO Requires named column constraint: QTest::newRow("14") << std::string("CREATE TABLE t1(c1 text CONSTRAINT nm NOT NULL)");
    QTest::newRow("15") << std::string("CREATE TABLE t1(c1 text NULL)");
    QTest::newRow("16") << std::string("CREATE TABLE t1(c1 text CONSTRAINT nm NULL)");
    QTest::newRow("17") << std::string("CREATE TABLE t1(c1 text UNIQUE)");
    // TODO Requires named column constraint: QTest::newRow("18") << std::string("CREATE TABLE t1(c1 text CONSTRAINT un UNIQUE)");
    QTest::newRow("19") << std::string("CREATE TABLE t1(c1 text CHECK(c1!=0))");
    // TODO Requires named column constraint: QTest::newRow("20") << std::string("CREATE TABLE t1(c1 text CONSTRAINT chk CHECK(c1!=0))");
    QTest::newRow("21") << std::string("CREATE TABLE t1(c1 text DEFAULT 1)");
    QTest::newRow("22") << std::string("CREATE TABLE t1(c1 text DEFAULT -1)");
    QTest::newRow("23") << std::string("CREATE TABLE t1(c1 text DEFAULT +1)");
    QTest::newRow("24") << std::string("CREATE TABLE t1(c1 text DEFAULT -45.8e22)");
    QTest::newRow("25") << std::string("CREATE TABLE t1(c1 text DEFAULT (1+1))");
    // TODO Requires named column constraint: QTest::newRow("26") << std::string("CREATE TABLE t1(c1 text CONSTRAINT \"1 2\" DEFAULT (1+1))");
    QTest::newRow("27") << std::string("CREATE TABLE t1(c1 text COLLATE nocase)");
    // TODO Requires named column constraint: QTest::newRow("28") << std::string("CREATE TABLE t1(c1 text CONSTRAINT 'a x' COLLATE nocase)");
    QTest::newRow("29") << std::string("CREATE TABLE t1(c1 REFERENCES t2)");
    QTest::newRow("30") << std::string("CREATE TABLE t1(c1 CONSTRAINT abc REFERENCES t2)");
    QTest::newRow("31") << std::string("CREATE TABLE t1(c1 PRIMARY KEY NOT NULL UNIQUE CHECK(c1 IS 'ten') DEFAULT 123 REFERENCES t1);");
    QTest::newRow("32") << std::string("CREATE TABLE t1(c1 REFERENCES t1 DEFAULT 123 CHECK(c1 IS 'ten') UNIQUE NOT NULL PRIMARY KEY);");
    QTest::newRow("33") << std::string("CREATE TABLE t1(c1, c2, PRIMARY KEY(c1))");
    QTest::newRow("34") << std::string("CREATE TABLE t1(c1, c2, PRIMARY KEY(c1, c2))");
    QTest::newRow("35") << std::string("CREATE TABLE t1(c1, c2, PRIMARY KEY(c1, c2) ON CONFLICT IGNORE)");
    QTest::newRow("36") << std::string("CREATE TABLE t1(c1, c2, UNIQUE(c1))");
    QTest::newRow("37") << std::string("CREATE TABLE t1(c1, c2, UNIQUE(c1, c2))");
    QTest::newRow("38") << std::string("CREATE TABLE t1(c1, c2, UNIQUE(c1, c2) ON CONFLICT IGNORE)");
    QTest::newRow("39") << std::string("CREATE TABLE t1(c1, c2, CHECK(c1 IS NOT c2))");
    QTest::newRow("40") << std::string("CREATE TABLE t1(c1, c2, FOREIGN KEY(c1) REFERENCES t2)");
    QTest::newRow("41") << std::string("CREATE TABLE t1(col1, col2 TEXT, col3 INTEGER UNIQUE, col4 VARCHAR(10, 10) PRIMARY KEY, \"name with spaces\" REFERENCES t1);");
    QTest::newRow("42") << std::string("CREATE TABLE t1(a, b, c)");
    QTest::newRow("43") << std::string("CREATE TEMP TABLE t1(a, b, c)");
    QTest::newRow("44") << std::string("CREATE TEMPORARY TABLE t1(a, b, c)");
    QTest::newRow("45") << std::string("CREATE TABLE IF NOT EXISTS t1(a, b, c)");
    QTest::newRow("46") << std::string("CREATE TEMP TABLE IF NOT EXISTS t1(a, b, c)");
    QTest::newRow("47") << std::string("CREATE TEMPORARY TABLE IF NOT EXISTS t1(a, b, c)");
    QTest::newRow("48") << std::string("CREATE TABLE main.t1(a, b, c)");
    QTest::newRow("49") << std::string("CREATE TEMP TABLE temp.t1(a, b, c)");
    QTest::newRow("50") << std::string("CREATE TEMPORARY TABLE temp.t1(a, b, c)");
    QTest::newRow("51") << std::string("CREATE TABLE IF NOT EXISTS main.t1(a, b, c)");
    QTest::newRow("52") << std::string("CREATE TEMP TABLE IF NOT EXISTS temp.t1(a, b, c)");
    QTest::newRow("53") << std::string("CREATE TEMPORARY TABLE IF NOT EXISTS temp.t1(a, b, c)");
    //QTest::newRow("54") << std::string("CREATE TABLE t1 AS SELECT * FROM t2");
    //QTest::newRow("55") << std::string("CREATE TEMP TABLE t1 AS SELECT c, b, a FROM t2");
    //QTest::newRow("56") << std::string("CREATE TABLE t1 AS SELECT count(*), max(b), min(a) FROM t2");
    QTest::newRow("57") << std::string("CREATE TABLE t1(a REFERENCES t2(x) MATCH FULL ON DELETE SET NULL ON UPDATE RESTRICT DEFERRABLE)");
    QTest::newRow("58") << std::string("CREATE TABLE t1(a REFERENCES t2(x) ON DELETE RESTRICT ON UPDATE SET NULL MATCH FULL NOT DEFERRABLE INITIALLY IMMEDIATE)");
    QTest::newRow("59") << std::string("CREATE TABLE t1(a REFERENCES t2(x) MATCH PARTIAL ON DELETE SET NULL ON UPDATE CASCADE DEFERRABLE INITIALLY IMMEDIATE)");
    QTest::newRow("60") << std::string("CREATE TABLE t1(a REFERENCES t2(x) MATCH PARTIAL ON DELETE RESTRICT ON UPDATE SET DEFAULT )");
    QTest::newRow("61") << std::string("CREATE TABLE t1(a REFERENCES t2(x) MATCH PARTIAL ON DELETE RESTRICT ON UPDATE RESTRICT DEFERRABLE)");
    QTest::newRow("62") << std::string("CREATE TABLE t1(a REFERENCES t2(x) MATCH PARTIAL ON DELETE NO ACTION ON UPDATE SET DEFAULT NOT DEFERRABLE INITIALLY IMMEDIATE)");
    QTest::newRow("63") << std::string("CREATE TABLE t1(a REFERENCES t2(x) MATCH SIMPLE ON DELETE SET NULL ON UPDATE CASCADE NOT DEFERRABLE)");
    QTest::newRow("64") << std::string("CREATE TABLE t1(a REFERENCES t2(x) MATCH SIMPLE ON DELETE SET DEFAULT ON UPDATE SET NULL DEFERRABLE)");
    QTest::newRow("65") << std::string("CREATE TABLE t1(a REFERENCES t2(x) MATCH SIMPLE ON DELETE SET DEFAULT  NOT DEFERRABLE)");
    QTest::newRow("66") << std::string("CREATE TABLE t1(a REFERENCES t2(x) MATCH SIMPLE ON DELETE RESTRICT ON UPDATE SET DEFAULT NOT DEFERRABLE INITIALLY DEFERRED)");
    QTest::newRow("67") << std::string("CREATE TABLE t1(a REFERENCES t2(x) MATCH SIMPLE ON DELETE RESTRICT ON UPDATE CASCADE NOT DEFERRABLE INITIALLY IMMEDIATE)");
    QTest::newRow("68") << std::string("CREATE TABLE t1(a REFERENCES t2(x) MATCH SIMPLE ON DELETE NO ACTION ON UPDATE SET DEFAULT NOT DEFERRABLE)");
    QTest::newRow("69") << std::string("CREATE TABLE t1(a REFERENCES t2(x) MATCH STICK ON DELETE CASCADE ON UPDATE CASCADE DEFERRABLE)");
    QTest::newRow("70") << std::string("CREATE TABLE t1(a REFERENCES t2(x) MATCH STICK ON UPDATE SET NULL NOT DEFERRABLE INITIALLY DEFERRED)");
    QTest::newRow("71") << std::string("CREATE TABLE t1(a REFERENCES t2(x) ON DELETE SET NULL ON UPDATE NO ACTION DEFERRABLE INITIALLY IMMEDIATE)");
    QTest::newRow("72") << std::string("CREATE TABLE t1(a REFERENCES t2(x) ON DELETE RESTRICT ON UPDATE NO ACTION NOT DEFERRABLE)");
    QTest::newRow("73") << std::string("CREATE TABLE t1(a REFERENCES t2(x) NOT DEFERRABLE INITIALLY DEFERRED)");
    QTest::newRow("74") << std::string("CREATE TABLE t1(a REFERENCES t2 MATCH FULL ON DELETE SET NULL ON UPDATE SET NULL DEFERRABLE INITIALLY IMMEDIATE)");
    QTest::newRow("75") << std::string("CREATE TABLE t1(a REFERENCES t2 MATCH FULL ON DELETE SET NULL ON UPDATE SET DEFAULT NOT DEFERRABLE)");
    QTest::newRow("76") << std::string("CREATE TABLE t1(a REFERENCES t2 MATCH FULL ON DELETE SET DEFAULT ON UPDATE SET NULL )");
    QTest::newRow("77") << std::string("CREATE TABLE t1(a REFERENCES t2 MATCH FULL ON DELETE CASCADE NOT DEFERRABLE INITIALLY IMMEDIATE)");
    QTest::newRow("78") << std::string("CREATE TABLE t1(a REFERENCES t2 MATCH PARTIAL ON DELETE SET NULL ON UPDATE RESTRICT NOT DEFERRABLE)");
    QTest::newRow("79") << std::string("CREATE TABLE t1(a REFERENCES t2 MATCH PARTIAL ON DELETE SET NULL ON UPDATE NO ACTION DEFERRABLE)");
    QTest::newRow("80") << std::string("CREATE TABLE t1(a REFERENCES t2 MATCH PARTIAL ON DELETE CASCADE ON UPDATE SET DEFAULT )");
    QTest::newRow("81") << std::string("CREATE TABLE t1(a REFERENCES t2 MATCH PARTIAL NOT DEFERRABLE)");
    QTest::newRow("82") << std::string("CREATE TABLE t1(a REFERENCES t2 MATCH SIMPLE ON DELETE SET DEFAULT ON UPDATE CASCADE DEFERRABLE)");
    QTest::newRow("83") << std::string("CREATE TABLE t1(a REFERENCES t2 MATCH STICK ON DELETE SET NULL ON UPDATE NO ACTION DEFERRABLE INITIALLY IMMEDIATE)");
    QTest::newRow("84") << std::string("CREATE TABLE t1(a REFERENCES t2 MATCH STICK ON DELETE NO ACTION ON UPDATE SET DEFAULT NOT DEFERRABLE INITIALLY IMMEDIATE)");
    QTest::newRow("85") << std::string("CREATE TABLE t1(a REFERENCES t2 MATCH STICK ON UPDATE SET DEFAULT DEFERRABLE INITIALLY IMMEDIATE)");
    QTest::newRow("86") << std::string("CREATE TABLE t1(a REFERENCES t2 ON DELETE RESTRICT ON UPDATE NO ACTION DEFERRABLE INITIALLY DEFERRED)");
    QTest::newRow("87") << std::string("CREATE TABLE sqlit_abc(a, b, c)");
    QTest::newRow("88") << std::string("CREATE TABLE temp.sqlitehelloworld(x)");
    QTest::newRow("89") << std::string("CREATE TABLE auxa.\"sqlite\"(x, y)");
    QTest::newRow("90") << std::string("CREATE TABLE auxb.\"sqlite-\"(z)");
    QTest::newRow("91") << std::string("CREATE TABLE \"SQLITE-TBL\"(z)");
    QTest::newRow("92") << std::string("CREATE TABLE main.abc(a, b, c)");
    QTest::newRow("93") << std::string("CREATE TABLE temp.helloworld(x)");
    QTest::newRow("94") << std::string("CREATE TABLE auxa.\"t 1\"(x, y)");
    QTest::newRow("95") << std::string("CREATE TABLE auxb.xyz(z)");
    QTest::newRow("96") << std::string("CREATE TABLE main.abc(a, b, c)");
    QTest::newRow("97") << std::string("CREATE TABLE main.t1(a, b, c)");
    QTest::newRow("98") << std::string("CREATE TABLE temp.tmp(a, b, c)");
    QTest::newRow("99") << std::string("CREATE TABLE auxb.tbl(x, y)");
    QTest::newRow("100") << std::string("CREATE TABLE auxb.t1(k, v)");
    QTest::newRow("101") << std::string("CREATE TABLE auxa.next(c, d)");
    QTest::newRow("102") << std::string("CREATE TEMP TABLE t1(a, b)");
    QTest::newRow("103") << std::string("CREATE TEMPORARY TABLE t2(a, b)");
    QTest::newRow("104") << std::string("CREATE TEMP TABLE temp.t1(a, b)");
    QTest::newRow("105") << std::string("CREATE TEMPORARY TABLE temp.t2(a, b)");
    QTest::newRow("106") << std::string("CREATE TEMP TABLE TEMP.t3(a, b)");
    QTest::newRow("107") << std::string("CREATE TEMPORARY TABLE TEMP.xxx(x)");
    QTest::newRow("108") << std::string("CREATE TABLE t1(a, b)");
    QTest::newRow("109") << std::string("CREATE TABLE t2(a, b)");
    QTest::newRow("110") << std::string("CREATE TABLE t3(a, b)");
    QTest::newRow("111") << std::string("CREATE TABLE xxx(x)");
    QTest::newRow("112") << std::string("CREATE TABLE auxa.t1(a, b)");
    QTest::newRow("113") << std::string("CREATE TABLE auxa.i1(a, b)");
    QTest::newRow("114") << std::string("CREATE TABLE auxa.v1(a, b)");
    QTest::newRow("115") << std::string("CREATE TABLE tbl1(a, b)");
    QTest::newRow("116") << std::string("CREATE TABLE idx1(a, b)");
    QTest::newRow("117") << std::string("CREATE TABLE view1(a, b)");
    QTest::newRow("118") << std::string("CREATE TABLE IF NOT EXISTS t1(a, b)");
    QTest::newRow("119") << std::string("CREATE TABLE IF NOT EXISTS auxa.tbl1(a, b)");
    QTest::newRow("120") << std::string("CREATE TABLE IF NOT EXISTS v1(a, b)");
    QTest::newRow("121") << std::string("CREATE TABLE IF NOT EXISTS auxa.view1(a, b)");
    QTest::newRow("122") << std::string("CREATE TABLE t1(a, b, c);");
    QTest::newRow("123") << std::string("CREATE TABLE t2(d, e, f);");
    QTest::newRow("124") << std::string("CREATE TABLE t3(g BIGINT, h VARCHAR(10));");
    QTest::newRow("125") << std::string("CREATE TABLE t4(i BLOB, j ANYOLDATA);");
    QTest::newRow("126") << std::string("CREATE TABLE t5(k FLOAT, l INTEGER);");
    QTest::newRow("127") << std::string("CREATE TABLE t6(m DEFAULT 10, n DEFAULT 5, PRIMARY KEY(m, n));");
    QTest::newRow("128") << std::string("CREATE TABLE t7(x INTEGER PRIMARY KEY);");
    QTest::newRow("129") << std::string("CREATE TABLE t8(o COLLATE nocase DEFAULT 'abc');");
    QTest::newRow("130") << std::string("CREATE TABLE t9(p NOT NULL, q DOUBLE CHECK (q!=0), r STRING UNIQUE);");
    QTest::newRow("131") << std::string("CREATE TABLE t1(x VARCHAR(10), y INTEGER, z DOUBLE);");
    QTest::newRow("132") << std::string("CREATE TABLE t2(a DATETIME, b STRING, c REAL);");
    QTest::newRow("133") << std::string("CREATE TABLE t3(o, t);");
    QTest::newRow("134") << std::string("CREATE TABLE t4(a DEFAULT NULL,b DEFAULT 'string constant',c DEFAULT X'424C4F42',d DEFAULT 1,e DEFAULT -1,f DEFAULT 3.14,g DEFAULT -3.14,h DEFAULT ( substr('abcd', 0, 2) || 'cd' ),i DEFAULT CURRENT_TIME,j DEFAULT CURRENT_DATE,k DEFAULT CURRENT_TIMESTAMP);");
    QTest::newRow("135") << std::string("CREATE TABLE t5(x DEFAULT ( 'abc' ))");
    QTest::newRow("136") << std::string("CREATE TABLE t5(x DEFAULT ( 1 IN (1, 2, 3) ))");
    QTest::newRow("137") << std::string("CREATE TABLE t5(a DEFAULT NULL,  b DEFAULT 'text value',  c DEFAULT X'424C4F42',d DEFAULT -45678.6,e DEFAULT 394507);");
    QTest::newRow("138") << std::string("CREATE TABLE t6(a DEFAULT ( nextint() ), b DEFAULT ( nextint() ));");
    QTest::newRow("139") << std::string("CREATE TABLE t7(a DEFAULT CURRENT_TIME, b DEFAULT CURRENT_DATE, c DEFAULT CURRENT_TIMESTAMP);");
    QTest::newRow("140") << std::string("CREATE TABLE t8(a COLLATE nocase, b COLLATE rtrim, c COLLATE binary, d);");
    QTest::newRow("141") << std::string("CREATE TABLE t1(a, b, c)");
    QTest::newRow("142") << std::string("CREATE TABLE t2(a PRIMARY KEY, b, c)");
    QTest::newRow("143") << std::string("CREATE TABLE t3(a, b, c, PRIMARY KEY(a))");
    QTest::newRow("144") << std::string("CREATE TABLE t4(a, b, c, PRIMARY KEY(c,b,a))");
    QTest::newRow("145") << std::string("CREATE TABLE t5(a, b INTEGER PRIMARY KEY, c)");
    QTest::newRow("146") << std::string("CREATE TABLE t5(a PRIMARY KEY, b, c)");
    QTest::newRow("147") << std::string("CREATE TABLE t5(a, b, c, PRIMARY KEY(a))");
    QTest::newRow("148") << std::string("CREATE TABLE t5(a, b, c, PRIMARY KEY(c,b,a))");
    QTest::newRow("149") << std::string("CREATE TABLE t5(a, b INTEGER PRIMARY KEY, c)");
    QTest::newRow("150") << std::string("CREATE TABLE t1(a UNIQUE, b UNIQUE)");
    QTest::newRow("151") << std::string("CREATE TABLE t2(a UNIQUE, b, c, UNIQUE(c, b))");
    QTest::newRow("152") << std::string("CREATE TABLE t3(a, b, c, UNIQUE(a), UNIQUE(b), UNIQUE(c))");
    QTest::newRow("153") << std::string("CREATE TABLE t4(a, b, c, UNIQUE(a, b, c))");
    QTest::newRow("154") << std::string("CREATE TABLE t1(a TEXT PRIMARY KEY, b)");
    QTest::newRow("155") << std::string("CREATE TABLE t1(a INTEGER PRIMARY KEY, b)");
    QTest::newRow("156") << std::string("CREATE TABLE t1(a TEXT UNIQUE, b)");
    QTest::newRow("157") << std::string("CREATE TABLE t1(a PRIMARY KEY, b TEXT UNIQUE)");
    QTest::newRow("158") << std::string("CREATE TABLE t1(a PRIMARY KEY, b, c, UNIQUE(c, b))");
    QTest::newRow("159") << std::string("CREATE TABLE t1(a, b PRIMARY KEY);");
    QTest::newRow("160") << std::string("CREATE TABLE t2(a, b, c, UNIQUE(b, c));");
    QTest::newRow("161") << std::string("CREATE TABLE x1(a TEXT, b INTEGER CHECK( b>0 ));");
    QTest::newRow("162") << std::string("CREATE TABLE t1(a TEXT, b INTEGER, CHECK( b>0 ));");
    QTest::newRow("163") << std::string("CREATE TABLE x2(a CHECK( a||b ), b);");
    QTest::newRow("164") << std::string("CREATE TABLE t2(a, b, CHECK( a||b ));");
    QTest::newRow("165") << std::string("CREATE TABLE t1(a NOT NULL, b)");
    QTest::newRow("166") << std::string("CREATE TABLE t2(a PRIMARY KEY NOT NULL, b)");
    QTest::newRow("167") << std::string("CREATE TABLE t3(a NOT NULL, b NOT NULL, c NOT NULL UNIQUE)");
    // TODO Requires NOT NULL table constraints: QTest::newRow("168") << std::string("CREATE TABLE t4(a, b, NOT NULL(a))");
    // TODO Requires NOT NULL table constraints: QTest::newRow("169") << std::string("CREATE TABLE t4(a PRIMARY KEY, b, NOT NULL(a))");
    // TODO Requires NOT NULL table constraints: QTest::newRow("170") << std::string("CREATE TABLE t4(a, b, c UNIQUE, NOT NULL(a, b, c))");
    QTest::newRow("171") << std::string("CREATE TABLE t1_ab(a PRIMARY KEY ON CONFLICT ABORT, b);");
    QTest::newRow("172") << std::string("CREATE TABLE t1_ro(a PRIMARY KEY ON CONFLICT ROLLBACK, b);");
    QTest::newRow("173") << std::string("CREATE TABLE t1_ig(a PRIMARY KEY ON CONFLICT IGNORE, b);");
    QTest::newRow("174") << std::string("CREATE TABLE t1_fa(a PRIMARY KEY ON CONFLICT FAIL, b);");
    QTest::newRow("175") << std::string("CREATE TABLE t1_re(a PRIMARY KEY ON CONFLICT REPLACE, b);");
    QTest::newRow("176") << std::string("CREATE TABLE t1_xx(a PRIMARY KEY, b);");
    // TODO Requires NOT NULL conflict actions: QTest::newRow("177") << std::string("CREATE TABLE t2_ab(a, b NOT NULL ON CONFLICT ABORT);");
    // TODO Requires NOT NULL conflict actions: QTest::newRow("178") << std::string("CREATE TABLE t2_ro(a, b NOT NULL ON CONFLICT ROLLBACK);");
    // TODO Requires NOT NULL conflict actions: QTest::newRow("179") << std::string("CREATE TABLE t2_ig(a, b NOT NULL ON CONFLICT IGNORE);");
    // TODO Requires NOT NULL conflict actions: QTest::newRow("180") << std::string("CREATE TABLE t2_fa(a, b NOT NULL ON CONFLICT FAIL);");
    // TODO Requires NOT NULL conflict actions: QTest::newRow("181") << std::string("CREATE TABLE t2_re(a, b NOT NULL ON CONFLICT REPLACE);");
    QTest::newRow("182") << std::string("CREATE TABLE t2_xx(a, b NOT NULL);");
    QTest::newRow("183") << std::string("CREATE TABLE t3_ab(a, b, UNIQUE(a, b) ON CONFLICT ABORT);");
    QTest::newRow("184") << std::string("CREATE TABLE t3_ro(a, b, UNIQUE(a, b) ON CONFLICT ROLLBACK);");
    QTest::newRow("185") << std::string("CREATE TABLE t3_ig(a, b, UNIQUE(a, b) ON CONFLICT IGNORE);");
    QTest::newRow("186") << std::string("CREATE TABLE t3_fa(a, b, UNIQUE(a, b) ON CONFLICT FAIL);");
    QTest::newRow("187") << std::string("CREATE TABLE t3_re(a, b, UNIQUE(a, b) ON CONFLICT REPLACE);");
    QTest::newRow("188") << std::string("CREATE TABLE t3_xx(a, b, UNIQUE(a, b));");
    QTest::newRow("189") << std::string("CREATE TABLE t4(a, b CHECK (b!=10));");
    QTest::newRow("190") << std::string("CREATE TABLE t2(oid, b);");
    QTest::newRow("191") << std::string("CREATE TABLE t3(a, _rowid_);");
    QTest::newRow("192") << std::string("CREATE TABLE t4(a, b, rowid);");
    QTest::newRow("193") << std::string("CREATE TABLE t5(pk integer primary key)");
    QTest::newRow("194") << std::string("CREATE TABLE t5(pk integer, primary key(pk))");
    QTest::newRow("195") << std::string("CREATE TABLE t5(pk integer, v integer, primary key(pk))");
    QTest::newRow("196") << std::string("CREATE TABLE t5(pk integer, v integer, primary key(pk, v))");
    QTest::newRow("197") << std::string("CREATE TABLE t5(pk int, v integer, primary key(pk, v))");
    QTest::newRow("198") << std::string("CREATE TABLE t5(pk int, v integer, primary key(pk))");
    QTest::newRow("199") << std::string("CREATE TABLE t5(pk int primary key, v integer)");
    QTest::newRow("200") << std::string("CREATE TABLE t5(pk inTEger primary key)");
    QTest::newRow("201") << std::string("CREATE TABLE t5(pk inteGEr, primary key(pk))");
    QTest::newRow("202") << std::string("CREATE TABLE t5(pk INTEGER, v integer, primary key(pk))");
    QTest::newRow("203") << std::string("CREATE TABLE t6(pk INT primary key);");
    QTest::newRow("204") << std::string("CREATE TABLE t7(pk BIGINT primary key);");
    QTest::newRow("205") << std::string("CREATE TABLE t8(pk SHORT INTEGER primary key);");
    QTest::newRow("206") << std::string("CREATE TABLE t9(pk UNSIGNED INTEGER primary key);");
    QTest::newRow("207") << std::string("CREATE TABLE t(x INTEGER PRIMARY KEY ASC, y, z)");
    QTest::newRow("208") << std::string("CREATE TABLE t(x INTEGER, y, z, PRIMARY KEY(x ASC))");
    QTest::newRow("209") << std::string("CREATE TABLE t(x INTEGER, y, z, PRIMARY KEY(x DESC))");
    QTest::newRow("210") << std::string("CREATE TABLE t(x INTEGER PRIMARY KEY DESC, y, z)");
}
