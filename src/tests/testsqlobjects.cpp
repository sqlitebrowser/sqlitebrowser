#include "testsqlobjects.h"
#include "../sql/ObjectIdentifier.h"
#include "../sql/sqlitetypes.h"

#include <QtTest/QtTest>

QTEST_APPLESS_MAIN(TestTable)

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
    f.setAutoIncrement(true);
    Field fkm("km", "integer");
    tt.fields.push_back(f);
    tt.fields.emplace_back("car", "text");
    tt.fields.push_back(fkm);
    tt.addConstraint(ConstraintPtr(new PrimaryKeyConstraint({f.name()})));

    QCOMPARE(tt.sql(), "CREATE TABLE \"testtable\" (\n"
                       "\t\"id\"\tinteger PRIMARY KEY AUTOINCREMENT,\n"
                       "\t\"car\"\ttext,\n"
                       "\t\"km\"\tinteger\n"
                       ");");
}

void TestTable::notnull()
{
    Table tt("testtable");
    Field f("id", "integer");
    f.setAutoIncrement(true);
    Field fkm("km", "integer");
    tt.fields.push_back(f);
    tt.fields.emplace_back("car", "text", true);
    tt.fields.push_back(fkm);
    tt.addConstraint(ConstraintPtr(new PrimaryKeyConstraint({f.name()})));

    QCOMPARE(tt.sql(), "CREATE TABLE \"testtable\" (\n"
                       "\t\"id\"\tinteger PRIMARY KEY AUTOINCREMENT,\n"
                       "\t\"car\"\ttext NOT NULL,\n"
                       "\t\"km\"\tinteger\n"
                       ");");
}

void TestTable::withoutRowid()
{
    Table tt("testtable");
    Field f("a", "integer");
    f.setAutoIncrement(true);
    tt.fields.push_back(f);
    tt.fields.emplace_back("b", "integer");
    tt.setWithoutRowidTable(true);
    tt.addConstraint(ConstraintPtr(new PrimaryKeyConstraint({f.name()})));

    QCOMPARE(tt.sql(), "CREATE TABLE \"testtable\" (\n"
                       "\t\"a\"\tinteger PRIMARY KEY AUTOINCREMENT,\n"
                       "\t\"b\"\tinteger\n"
                       ") WITHOUT ROWID;");
}

void TestTable::foreignKeys()
{
    Table tt("testtable");
    Field f("a", "integer");
    tt.fields.push_back(f);
    sqlb::ConstraintPtr fk = sqlb::ConstraintPtr(new sqlb::ForeignKeyClause("b", sqlb::StringVector{"c"}));
    fk->column_list = {f.name()};
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

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL)));

    QCOMPARE(tab.name(), "hero");
    QCOMPARE(tab.rowidColumns(), {"_rowid_"});
    QCOMPARE(tab.fields.at(0).name(), "id");
    QCOMPARE(tab.fields.at(1).name(), "name");
    QCOMPARE(tab.fields.at(2).name(), "info");

    QCOMPARE(tab.fields.at(0).type(), "integer");
    QCOMPARE(tab.fields.at(1).type(), "text");
    QCOMPARE(tab.fields.at(2).type(), "VARCHAR(255)");

    sqlb::StringVector pk = tab.primaryKey();
    QVERIFY(tab.fields.at(0).autoIncrement());
    QCOMPARE(pk.size(), 1);
    QCOMPARE(pk.at(0), tab.fields.at(0).name());
    QVERIFY(tab.fields.at(1).notnull());
    QCOMPARE(tab.fields.at(1).defaultValue(), "'xxxx'");
    QCOMPARE(tab.fields.at(1).check(), "");
    QCOMPARE(tab.fields.at(2).check(), "info=='x'");
}

void TestTable::parseSQLdefaultexpr()
{
    std::string sSQL = "CREATE TABLE chtest(\n"
                       "id integer primary key,\n"
                       "dumpytext text default('axa') CHECK(dumpytext == \"aa\"),\n"
                       "date datetime default CURRENT_TIMESTAMP,"
                       "zoi integer)";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL)));

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
    QCOMPARE(tab.fields.at(1).check(), "dumpytext==\"aa\"");
    QCOMPARE(tab.fields.at(2).defaultValue(), "CURRENT_TIMESTAMP");
    QCOMPARE(tab.fields.at(2).check(), "");
    QCOMPARE(tab.fields.at(3).defaultValue(), "");
    QCOMPARE(tab.fields.at(3).check(), "");

    sqlb::StringVector pk = tab.primaryKey();
    QCOMPARE(pk.size(), 1);
    QCOMPARE(pk.at(0), tab.fields.at(0).name());
}

void TestTable::parseSQLMultiPk()
{
    std::string sSQL = "CREATE TABLE hero (\n"
                       "\tid1 integer,\n"
                       "\tid2 integer,\n"
                       "\tnonpkfield blob,\n"
                       "PRIMARY KEY(\"id1\",\"id2\")\n"
                       ");";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL)));

    QCOMPARE(tab.name(), "hero");
    QCOMPARE(tab.fields.at(0).name(), "id1");
    QCOMPARE(tab.fields.at(1).name(), "id2");

    QCOMPARE(tab.fields.at(0).type(), "integer");
    QCOMPARE(tab.fields.at(1).type(), "integer");

    sqlb::StringVector pk = tab.primaryKey();
    QCOMPARE(pk.size(), 2);
    QCOMPARE(pk.at(0), tab.fields.at(0).name());
    QCOMPARE(pk.at(1), tab.fields.at(1).name());
}

void TestTable::parseSQLForeignKey()
{
    std::string sSQL = "CREATE TABLE grammar_test(id, test, FOREIGN KEY(test) REFERENCES other_table);";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL)));

    QCOMPARE(tab.name(), "grammar_test");
    QCOMPARE(tab.fields.at(0).name(), "id");
    QCOMPARE(tab.fields.at(1).name(), "test");
}

void TestTable::parseSQLSingleQuotes()
{
    std::string sSQL = "CREATE TABLE 'test'('id','test');";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL)));

    QCOMPARE(tab.name(), "test");
    QCOMPARE(tab.fields.at(0).name(), "id");
    QCOMPARE(tab.fields.at(1).name(), "test");
}

void TestTable::parseSQLSquareBrackets()
{
    std::string sSQL = "CREATE TABLE [test]([id],[test]);";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL)));

    QCOMPARE(tab.name(), "test");
    QCOMPARE(tab.fields.at(0).name(), "id");
    QCOMPARE(tab.fields.at(1).name(), "test");
}


void TestTable::parseSQLKeywordInIdentifier()
{
    std::string sSQL = "CREATE TABLE deffered(key integer primary key, if text);";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL)));

    QCOMPARE(tab.name(), "deffered");
    QCOMPARE(tab.fields.at(0).name(), "key");
    QCOMPARE(tab.fields.at(1).name(), "if");
}


void TestTable::parseSQLSomeKeywordsInIdentifier()
{
    std::string sSQL = "CREATE TABLE \"Average Number of Volunteers by Area of Work\" ("
                       "`Area of Work`	TEXT,"
                       "`Average Number of Volunteers`	INTEGER);";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL)));

    QCOMPARE(tab.name(), "Average Number of Volunteers by Area of Work");
    QCOMPARE(tab.fields.at(0).name(), "Area of Work");
    QCOMPARE(tab.fields.at(1).name(), "Average Number of Volunteers");
}

void TestTable::parseSQLWithoutRowid()
{
    std::string sSQL = "CREATE TABLE test(a integer primary key, b integer) WITHOUT ROWID;";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL)));

    QCOMPARE(tab.primaryKey(), {"a"});
    QCOMPARE(tab.rowidColumns(), {"a"});
}

void TestTable::parseNonASCIIChars()
{
    std::string sSQL = "CREATE TABLE `lösung` ("
                       "`Fieldöäüß`	INTEGER,"
                       "PRIMARY KEY(`Fieldöäüß`)"
                       ");";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL)));

    QCOMPARE(tab.name(), "lösung");
    QCOMPARE(tab.fields.at(0).name(), "Fieldöäüß");
}

void TestTable::parseNonASCIICharsEs()
{
    std::string sSQL = "CREATE TABLE \"Cigüeñas de Alcalá\" ("
                       "\"Field áéíóúÁÉÍÓÚñÑçÇ\"	INTEGER,"
                       "PRIMARY KEY(\"Field áéíóúÁÉÍÓÚñÑçÇ\")"
                       ");";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL)));

    QCOMPARE(tab.name(), "Cigüeñas de Alcalá");
    QCOMPARE(tab.fields.at(0).name(), "Field áéíóúÁÉÍÓÚñÑçÇ");
}

void TestTable::parseSQLEscapedQuotes()
{
    std::string sSql = "CREATE TABLE double_quotes(a text default 'a''a');";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSql)));

    QCOMPARE(tab.name(), "double_quotes");
    QCOMPARE(tab.fields.at(0).name(), "a");
    QCOMPARE(tab.fields.at(0).defaultValue(), "'a''a'");
}

void TestTable::parseSQLForeignKeys()
{
    std::string sql = "CREATE TABLE foreign_key_test(a int, b int, foreign key (a) references x, foreign key (b) references w(y,z) on delete set null);";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql)));

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

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql)));

    QCOMPARE(tab.name(), "a");
    QCOMPARE(tab.fields.at(0).name(), "b");
    QCOMPARE(tab.fields.at(0).type(), "text");
    QCOMPARE(tab.fields.at(0).check(), "\"b\"='A' or \"b\"='B'");
}

void TestTable::parseDefaultValues()
{
    std::string sql = "CREATE TABLE test(a int DEFAULT 0, b int DEFAULT -1, c text DEFAULT 'hello', d text DEFAULT '0');";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql)));

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

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL)));
    QCOMPARE(tab.name(), "not_working");

    QCOMPARE(tab.fields.at(1).check(), "value IN ('a','b','c')");
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

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sSQL)));
    QCOMPARE(tab.name(), "hopefully_working");

    QCOMPARE(tab.fields.at(0).check(), "value NOT LIKE 'prefix%'");
    QCOMPARE(tab.fields.at(1).check(), "value2 NOT MATCH 'prefix%'");
    QCOMPARE(tab.fields.at(2).check(), "value3 NOT REGEXP 'prefix%'");
    QCOMPARE(tab.fields.at(3).check(), "value4 NOT GLOB 'prefix%'");
    QCOMPARE(tab.fields.at(4).check(), "value5 BETWEEN 1+4 AND 100 OR 200");
    QCOMPARE(tab.fields.at(5).check(), "value6 NOT BETWEEN 1 AND 100");
}

void TestTable::rowValues()
{
    std::string sql = "CREATE TABLE test(\n"
                      "a INTEGER,\n"
                      "b INTEGER,\n"
                      "CHECK((a, b) = (1, 2))\n"
                      ");";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql)));
    QCOMPARE(tab.name(), "test");

    QCOMPARE(std::dynamic_pointer_cast<sqlb::CheckConstraint>(tab.constraint({}, sqlb::Constraint::CheckConstraintType))->expression(), "(a,b)=(1,2)");
}

void TestTable::complexExpressions()
{
    std::string sql = "CREATE TABLE test(\n"
                      "a INTEGER CHECK((a > 0)),\n"
                      "b INTEGER CHECK((b > 0 and b > 1)),\n"
                      "c INTEGER CHECK((c = -1) or (c > 0 and c > 1) or (c = 0)),\n"
                      "d INTEGER CHECK((((d > 0))))\n"
                      ");";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql)));
    QCOMPARE(tab.name(), "test");

    QCOMPARE(tab.fields.at(0).check(), "(a>0)");
    QCOMPARE(tab.fields.at(1).check(), "(b>0 and b>1)");
    QCOMPARE(tab.fields.at(2).check(), "(c=-1) or (c>0 and c>1) or (c=0)");
    QCOMPARE(tab.fields.at(3).check(), "(((d>0)))");
}

void TestTable::datetimeExpression()
{
    std::string sql = "CREATE TABLE test(\n"
                      "entry INTEGER DEFAULT (DATETIME(CURRENT_TIMESTAMP, 'LOCALTIME'))\n"
                      ");";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql)));
    QCOMPARE(tab.name(), "test");

    QCOMPARE(tab.fields.at(0).name(), "entry");
    QCOMPARE(tab.fields.at(0).type(), "INTEGER");
    QCOMPARE(tab.fields.at(0).defaultValue(), "(DATETIME(CURRENT_TIMESTAMP,'LOCALTIME'))");
}

void TestTable::extraParentheses()
{
    std::string sql = "CREATE TABLE test(\n"
                      "xy INTEGER DEFAULT (1 + (5) - 4)\n"
                      ");";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql)));
    QCOMPARE(tab.name(), "test");

    QCOMPARE(tab.fields.at(0).name(), "xy");
    QCOMPARE(tab.fields.at(0).type(), "INTEGER");
    QCOMPARE(tab.fields.at(0).defaultValue(), "(1+(5)-4)");
}

void TestTable::moduloOperator()
{
    std::string sql = "CREATE TABLE test(\n"
                      "xy INTEGER DEFAULT (7 % 2)\n"
                      ");";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql)));
    QCOMPARE(tab.name(), "test");

    QCOMPARE(tab.fields.at(0).name(), "xy");
    QCOMPARE(tab.fields.at(0).type(), "INTEGER");
    QCOMPARE(tab.fields.at(0).defaultValue(), "(7%2)");
}

void TestTable::complexExpression()
{
    std::string sql = "CREATE TABLE test(\n"
                      "uuid INTEGER DEFAULT (hex(randomblob(4))||'-'||hex(randomblob(2))||'-'||'4'||substr(hex(randomblob(2)),2)||'-'||substr('AB89',1+(abs(random())%4),1)||substr(hex(randomblob(2)),2)||'-'||hex(randomblob(6))),\n"
                      "a INTEGER,\n"
                      "b INTEGER,\n"
                      "CHECK((a = 'S' AND b IS NOT NULL) OR (a IN ('A', 'P')))"
                      ");";

    Table tab = *(std::dynamic_pointer_cast<sqlb::Table>(Table::parseSQL(sql)));
    QCOMPARE(tab.name(), "test");

    QCOMPARE(tab.fields.at(0).name(), "uuid");
    QCOMPARE(tab.fields.at(0).type(), "INTEGER");
    QCOMPARE(tab.fields.at(0).defaultValue(), "(hex(randomblob(4))||'-'||hex(randomblob(2))||'-'||'4'||substr(hex(randomblob(2)),2)||'-'||substr('AB89',1+(abs(random())%4),1)||substr(hex(randomblob(2)),2)||'-'||hex(randomblob(6)))");

    auto c = tab.constraints({}, sqlb::Constraint::CheckConstraintType);
    QCOMPARE(c.size(), 1);
    QCOMPARE(std::dynamic_pointer_cast<sqlb::CheckConstraint>(c.at(0))->expression(), "(a='S' AND b IS NOT NULL) OR (a IN ('A','P'))");
}
