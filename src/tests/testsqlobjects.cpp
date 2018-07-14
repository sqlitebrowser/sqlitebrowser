#include "testsqlobjects.h"
#include "../sqlitetypes.h"

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
    FieldPtr f = FieldPtr(new Field("id", "integer"));
    FieldPtr fkm = FieldPtr(new Field("km", "integer", false, "", "km > 1000"));
    tt.addField(f);
    tt.addField(FieldPtr(new Field("car", "text")));
    tt.addField(fkm);
    tt.addConstraint({f, fkm}, ConstraintPtr(new PrimaryKeyConstraint()));

    QCOMPARE(tt.sql(), QString("CREATE TABLE \"testtable\" (\n"
                               "\t\"id\"\tinteger,\n"
                               "\t\"car\"\ttext,\n"
                               "\t\"km\"\tinteger CHECK(km > 1000),\n"
                               "\tPRIMARY KEY(\"id\",\"km\")\n"
                               ");"));
}

void TestTable::sqlGraveAccentOutput()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("id", "integer"));
    FieldPtr fkm = FieldPtr(new Field("km", "integer", false, "", "km > 1000"));
    tt.addField(f);
    tt.addField(FieldPtr(new Field("car", "text")));
    tt.addField(fkm);
    tt.addConstraint({f, fkm}, ConstraintPtr(new PrimaryKeyConstraint()));
    sqlb::setIdentifierQuoting(sqlb::GraveAccents);

    QCOMPARE(tt.sql(), QString("CREATE TABLE `testtable` (\n"
                               "\t`id`\tinteger,\n"
                               "\t`car`\ttext,\n"
                               "\t`km`\tinteger CHECK(km > 1000),\n"
                               "\tPRIMARY KEY(`id`,`km`)\n"
                               ");"));

    sqlb::setIdentifierQuoting(sqlb::DoubleQuotes);
}


void TestTable::sqlSquareBracketsOutput()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("id", "integer"));
    FieldPtr fkm = FieldPtr(new Field("km", "integer", false, "", "km > 1000"));
    tt.addField(f);
    tt.addField(FieldPtr(new Field("car", "text")));
    tt.addField(fkm);
    tt.addConstraint({f, fkm}, ConstraintPtr(new PrimaryKeyConstraint()));
    sqlb::setIdentifierQuoting(sqlb::SquareBrackets);

    QCOMPARE(tt.sql(), QString("CREATE TABLE [testtable] (\n"
                               "\t[id]\tinteger,\n"
                               "\t[car]\ttext,\n"
                               "\t[km]\tinteger CHECK(km > 1000),\n"
                               "\tPRIMARY KEY([id],[km])\n"
                               ");"));

    sqlb::setIdentifierQuoting(sqlb::DoubleQuotes);
}

void TestTable::autoincrement()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("id", "integer"));
    f->setAutoIncrement(true);
    FieldPtr fkm = FieldPtr(new Field("km", "integer"));
    tt.addField(f);
    tt.addField(FieldPtr(new Field("car", "text")));
    tt.addField(fkm);
    tt.addConstraint({f}, ConstraintPtr(new PrimaryKeyConstraint()));

    QCOMPARE(tt.sql(), QString("CREATE TABLE \"testtable\" (\n"
                               "\t\"id\"\tinteger PRIMARY KEY AUTOINCREMENT,\n"
                               "\t\"car\"\ttext,\n"
                               "\t\"km\"\tinteger\n"
                               ");"));
}

void TestTable::notnull()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("id", "integer"));
    f->setAutoIncrement(true);
    FieldPtr fkm = FieldPtr(new Field("km", "integer"));
    tt.addField(f);
    tt.addField(FieldPtr(new Field("car", "text", true)));
    tt.addField(fkm);
    tt.addConstraint({f}, ConstraintPtr(new PrimaryKeyConstraint()));

    QCOMPARE(tt.sql(), QString("CREATE TABLE \"testtable\" (\n"
                               "\t\"id\"\tinteger PRIMARY KEY AUTOINCREMENT,\n"
                               "\t\"car\"\ttext NOT NULL,\n"
                               "\t\"km\"\tinteger\n"
                               ");"));
}

void TestTable::withoutRowid()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("a", "integer"));
    f->setAutoIncrement(true);
    tt.addField(f);
    tt.addField(FieldPtr(new Field("b", "integer")));
    tt.setRowidColumn("a");
    tt.addConstraint({f}, ConstraintPtr(new PrimaryKeyConstraint()));

    QCOMPARE(tt.sql(), QString("CREATE TABLE \"testtable\" (\n"
                               "\t\"a\"\tinteger PRIMARY KEY AUTOINCREMENT,\n"
                               "\t\"b\"\tinteger\n"
                               ") WITHOUT ROWID;"));
}

void TestTable::foreignKeys()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("a", "integer"));
    tt.addField(f);
    tt.addConstraint({f}, sqlb::ConstraintPtr(new sqlb::ForeignKeyClause("b", QStringList("c"))));

    QCOMPARE(tt.sql(), QString("CREATE TABLE \"testtable\" (\n"
                               "\t\"a\"\tinteger,\n"
                               "\tFOREIGN KEY(\"a\") REFERENCES \"b\"(\"c\")\n"
                               ");"));
}

void TestTable::uniqueConstraint()
{
    Table tt("testtable");
    FieldPtr f1 = FieldPtr(new Field("a", "integer"));
    FieldPtr f2 = FieldPtr(new Field("b", "integer"));
    FieldPtr f3 = FieldPtr(new Field("c", "integer"));
    f1->setUnique(true);
    tt.addField(f1);
    tt.addField(f2);
    tt.addField(f3);
    tt.addConstraint({f2, f3}, sqlb::ConstraintPtr(new sqlb::UniqueConstraint()));

    QCOMPARE(tt.sql(), QString("CREATE TABLE \"testtable\" (\n"
                               "\t\"a\"\tinteger UNIQUE,\n"
                               "\t\"b\"\tinteger,\n"
                               "\t\"c\"\tinteger,\n"
                               "\tUNIQUE(\"b\",\"c\")\n"
                               ");"));
}

void TestTable::parseSQL()
{
    QString sSQL = "create TABLE hero (\n"
            "\tid integer PRIMARY KEY AUTOINCREMENT,\n"
            "\tname text NOT NULL DEFAULT 'xxxx',\n"
            "\tinfo VARCHAR(255) CHECK (info == 'x')\n"
            ");";

    Table tab = *(Table::parseSQL(sSQL).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), "hero");
    QCOMPARE(tab.rowidColumn(), "_rowid_");
    QCOMPARE(tab.fields().at(0)->name(), "id");
    QCOMPARE(tab.fields().at(1)->name(), "name");
    QCOMPARE(tab.fields().at(2)->name(), "info");

    QCOMPARE(tab.fields().at(0)->type(), "integer");
    QCOMPARE(tab.fields().at(1)->type(), "text");
    QCOMPARE(tab.fields().at(2)->type(), QString("VARCHAR(255)"));

    FieldVector pk = tab.primaryKey();
    QVERIFY(tab.fields().at(0)->autoIncrement());
    QCOMPARE(pk.size(), 1);
    QCOMPARE(pk.at(0), tab.fields().at(0));
    QVERIFY(tab.fields().at(1)->notnull());
    QCOMPARE(tab.fields().at(1)->defaultValue(), QString("'xxxx'"));
    QCOMPARE(tab.fields().at(1)->check(), QString(""));
    QCOMPARE(tab.fields().at(2)->check(), QString("info=='x'"));
}

void TestTable::parseSQLdefaultexpr()
{
    QString sSQL = "CREATE TABLE chtest(\n"
            "id integer primary key,\n"
            "dumpytext text default('axa') CHECK(dumpytext == \"aa\"),\n"
            "date datetime default CURRENT_TIMESTAMP,"
            "zoi integer)";

    Table tab = *(Table::parseSQL(sSQL).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), QString("chtest"));
    QCOMPARE(tab.fields().at(0)->name(), QString("id"));
    QCOMPARE(tab.fields().at(1)->name(), QString("dumpytext"));
    QCOMPARE(tab.fields().at(2)->name(), QString("date"));
    QCOMPARE(tab.fields().at(3)->name(), QString("zoi"));

    QCOMPARE(tab.fields().at(0)->type(), QString("integer"));
    QCOMPARE(tab.fields().at(1)->type(), QString("text"));
    QCOMPARE(tab.fields().at(2)->type(), QString("datetime"));
    QCOMPARE(tab.fields().at(3)->type(), QString("integer"));

    QCOMPARE(tab.fields().at(1)->defaultValue(), QString("('axa')"));
    QCOMPARE(tab.fields().at(1)->check(), QString("dumpytext==\"aa\""));
    QCOMPARE(tab.fields().at(2)->defaultValue(), QString("CURRENT_TIMESTAMP"));
    QCOMPARE(tab.fields().at(2)->check(), QString(""));
    QCOMPARE(tab.fields().at(3)->defaultValue(), QString(""));
    QCOMPARE(tab.fields().at(3)->check(), QString(""));

    sqlb::FieldVector pk = tab.primaryKey();
    QCOMPARE(pk.size(), 1);
    QCOMPARE(pk.at(0), tab.fields().at(0));
}

void TestTable::parseSQLMultiPk()
{
    QString sSQL = "CREATE TABLE hero (\n"
            "\tid1 integer,\n"
            "\tid2 integer,\n"
            "\tnonpkfield blob,\n"
            "PRIMARY KEY(\"id1\",\"id2\")\n"
            ");";

    Table tab = *(Table::parseSQL(sSQL).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), "hero");
    QCOMPARE(tab.fields().at(0)->name(), "id1");
    QCOMPARE(tab.fields().at(1)->name(), "id2");

    QCOMPARE(tab.fields().at(0)->type(), "integer");
    QCOMPARE(tab.fields().at(1)->type(), "integer");

    sqlb::FieldVector pk = tab.primaryKey();
    QCOMPARE(pk.size(), 2);
    QCOMPARE(pk.at(0), tab.fields().at(0));
    QCOMPARE(pk.at(1), tab.fields().at(1));
}

void TestTable::parseSQLForeignKey()
{
    QString sSQL = "CREATE TABLE grammar_test(id, test, FOREIGN KEY(test) REFERENCES other_table);";

    Table tab = *(Table::parseSQL(sSQL).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), "grammar_test");
    QCOMPARE(tab.fields().at(0)->name(), "id");
    QCOMPARE(tab.fields().at(1)->name(), "test");
}

void TestTable::parseSQLSingleQuotes()
{
    QString sSQL = "CREATE TABLE 'test'('id','test');";

    Table tab = *(Table::parseSQL(sSQL).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), "test");
    QCOMPARE(tab.fields().at(0)->name(), "id");
    QCOMPARE(tab.fields().at(1)->name(), "test");
}

void TestTable::parseSQLSquareBrackets()
{
    QString sSQL = "CREATE TABLE [test]([id],[test]);";

    Table tab = *(Table::parseSQL(sSQL).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), "test");
    QCOMPARE(tab.fields().at(0)->name(), "id");
    QCOMPARE(tab.fields().at(1)->name(), "test");
}


void TestTable::parseSQLKeywordInIdentifier()
{
    QString sSQL = "CREATE TABLE deffered(key integer primary key, if text);";

    Table tab = *(Table::parseSQL(sSQL).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), "deffered");
    QCOMPARE(tab.fields().at(0)->name(), "key");
    QCOMPARE(tab.fields().at(1)->name(), "if");
}


void TestTable::parseSQLSomeKeywordsInIdentifier()
{
    QString sSQL = "CREATE TABLE \"Average Number of Volunteers by Area of Work\" ("
      "`Area of Work`	TEXT,"
      "`Average Number of Volunteers`	INTEGER);";

    Table tab = *(Table::parseSQL(sSQL).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), "Average Number of Volunteers by Area of Work");
    QCOMPARE(tab.fields().at(0)->name(), "Area of Work");
    QCOMPARE(tab.fields().at(1)->name(), "Average Number of Volunteers");
}

void TestTable::parseSQLWithoutRowid()
{
    QString sSQL = "CREATE TABLE test(a integer primary key, b integer) WITHOUT ROWID;";

    Table tab = *(Table::parseSQL(sSQL).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.fields().at(tab.findPk())->name(), "a");
    QCOMPARE(tab.rowidColumn(), "a");
}

void TestTable::parseNonASCIIChars()
{
    QString sSQL = "CREATE TABLE `lösung` ("
            "`Fieldöäüß`	INTEGER,"
            "PRIMARY KEY(`Fieldöäüß`)"
            ");";

    Table tab = *(Table::parseSQL(sSQL).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), "lösung");
    QCOMPARE(tab.fields().at(0)->name(), "Fieldöäüß");
}

void TestTable::parseNonASCIICharsEs()
{
    QString sSQL = "CREATE TABLE \"Cigüeñas de Alcalá\" ("
            "\"Field áéíóúÁÉÍÓÚñÑçÇ\"	INTEGER,"
            "PRIMARY KEY(\"Field áéíóúÁÉÍÓÚñÑçÇ\")"
            ");";

    Table tab = *(Table::parseSQL(sSQL).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), "Cigüeñas de Alcalá");
    QCOMPARE(tab.fields().at(0)->name(), "Field áéíóúÁÉÍÓÚñÑçÇ");
}

void TestTable::parseSQLEscapedQuotes()
{
    QString sSql = "CREATE TABLE double_quotes(a text default 'a''a');";

    Table tab = *(Table::parseSQL(sSql).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), QString("double_quotes"));
    QCOMPARE(tab.fields().at(0)->name(), QString("a"));
    QCOMPARE(tab.fields().at(0)->defaultValue(), QString("'a''a'"));
}

void TestTable::parseSQLForeignKeys()
{
    QString sql = "CREATE TABLE foreign_key_test(a int, b int, foreign key (a) references x, foreign key (b) references w(y,z) on delete set null);";

    Table tab = *(Table::parseSQL(sql).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), QString("foreign_key_test"));
    QCOMPARE(tab.fields().at(0)->name(), QString("a"));
    QCOMPARE(tab.fields().at(0)->type(), QString("int"));
    QCOMPARE(tab.constraint({tab.fields().at(0)}, sqlb::Constraint::ForeignKeyConstraintType).dynamicCast<sqlb::ForeignKeyClause>()->table(), QString("x"));
    QCOMPARE(tab.fields().at(1)->name(), QString("b"));
    QCOMPARE(tab.fields().at(1)->type(), QString("int"));
    QCOMPARE(tab.constraint({tab.fields().at(1)}, sqlb::Constraint::ForeignKeyConstraintType).dynamicCast<sqlb::ForeignKeyClause>()->toString(), QString("\"w\"(\"y\",\"z\") on delete set null"));
}

void TestTable::parseSQLCheckConstraint()
{
    QString sql = "CREATE TABLE a (\"b\" text CHECK(\"b\"='A' or \"b\"='B'));";

    Table tab = *(Table::parseSQL(sql).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), QString("a"));
    QCOMPARE(tab.fields().at(0)->name(), QString("b"));
    QCOMPARE(tab.fields().at(0)->type(), QString("text"));
    QCOMPARE(tab.fields().at(0)->check(), QString("\"b\"='A' or \"b\"='B'"));
}

void TestTable::parseDefaultValues()
{
    QString sql = "CREATE TABLE test(a int DEFAULT 0, b int DEFAULT -1, c text DEFAULT 'hello', d text DEFAULT '0');";

    Table tab = *(Table::parseSQL(sql).dynamicCast<sqlb::Table>());

    QCOMPARE(tab.name(), QString("test"));
    QCOMPARE(tab.fields().at(0)->name(), QString("a"));
    QCOMPARE(tab.fields().at(0)->type(), QString("int"));
    QCOMPARE(tab.fields().at(0)->defaultValue(), QString("0"));
    QCOMPARE(tab.fields().at(1)->name(), QString("b"));
    QCOMPARE(tab.fields().at(1)->type(), QString("int"));
    QCOMPARE(tab.fields().at(1)->defaultValue(), QString("-1"));
    QCOMPARE(tab.fields().at(2)->name(), QString("c"));
    QCOMPARE(tab.fields().at(2)->type(), QString("text"));
    QCOMPARE(tab.fields().at(2)->defaultValue(), QString("'hello'"));
    QCOMPARE(tab.fields().at(3)->name(), QString("d"));
    QCOMPARE(tab.fields().at(3)->type(), QString("text"));
    QCOMPARE(tab.fields().at(3)->defaultValue(), QString("'0'"));
}

void TestTable::createTableWithIn()
{
    QString sSQL = "CREATE TABLE not_working("
            "_id PRIMARY KEY NOT NULL,"
            "value NVARCHAR(5) CHECK (value IN ('a', 'b', 'c'))"
            ");";

    Table tab = *(Table::parseSQL(sSQL).dynamicCast<sqlb::Table>());
    QCOMPARE(tab.name(), "not_working");

    QCOMPARE(tab.fields().at(1)->check(), "value IN ('a','b','c')");
}

void TestTable::createTableWithNotLikeConstraint()
{
    QString sSQL = "CREATE TABLE hopefully_working(\n"
            "value TEXT CONSTRAINT 'value' CHECK(value NOT LIKE 'prefix%'),\n"
            "value2 TEXT CONSTRAINT 'value' CHECK(value2 NOT MATCH 'prefix%'),\n"
            "value3 TEXT CONSTRAINT 'value' CHECK(value3 NOT REGEXP 'prefix%'),\n"
            "value4 TEXT CONSTRAINT 'value' CHECK(value4 NOT GLOB 'prefix%'),\n"
            "value5 INTEGER CONSTRAINT 'value' CHECK(value5 BETWEEN 1+4 AND 100 OR 200),\n"
            "value6 INTEGER CONSTRAINT 'value' CHECK(value6 NOT BETWEEN 1 AND 100),\n"
            "value7 INTEGER CONSTRAINT 'value' CHECK(NOT EXISTS (1))\n"
            ");";

    Table tab = *(Table::parseSQL(sSQL).dynamicCast<sqlb::Table>());
    QCOMPARE(tab.name(), "hopefully_working");

    QCOMPARE(tab.fields().at(0)->check(), "value NOT LIKE 'prefix%'");
    QCOMPARE(tab.fields().at(1)->check(), "value2 NOT MATCH 'prefix%'");
    QCOMPARE(tab.fields().at(2)->check(), "value3 NOT REGEXP 'prefix%'");
    QCOMPARE(tab.fields().at(3)->check(), "value4 NOT GLOB 'prefix%'");
    QCOMPARE(tab.fields().at(4)->check(), "value5 BETWEEN 1+4 AND 100 OR 200");
    QCOMPARE(tab.fields().at(5)->check(), "value6 NOT BETWEEN 1 AND 100");
    QCOMPARE(tab.fields().at(6)->check(), "NOT EXISTS (1)");
}

void TestTable::rowValues()
{
    QString sql = "CREATE TABLE test(\n"
            "a INTEGER,\n"
            "b INTEGER,\n"
            "CHECK((a, b) = (1, 2))\n"
            ");";

    Table tab = *(Table::parseSQL(sql).dynamicCast<sqlb::Table>());
    QCOMPARE(tab.name(), "test");

    QCOMPARE(tab.constraint({}, sqlb::Constraint::CheckConstraintType).dynamicCast<sqlb::CheckConstraint>()->expression(), QString("(a,b)=(1,2)"));
}

void TestTable::complexExpressions()
{
    QString sql = "CREATE TABLE test(\n"
            "a INTEGER CHECK((a > 0)),\n"
            "b INTEGER CHECK((b > 0 and b > 1)),\n"
            "c INTEGER CHECK((c = -1) or (c > 0 and c > 1) or (c = 0)),\n"
            "d INTEGER CHECK((((d > 0))))\n"
            ");";

    Table tab = *(Table::parseSQL(sql).dynamicCast<sqlb::Table>());
    QCOMPARE(tab.name(), "test");

    QCOMPARE(tab.fields().at(0)->check(), "(a>0)");
    QCOMPARE(tab.fields().at(1)->check(), "(b>0 and b>1)");
    QCOMPARE(tab.fields().at(2)->check(), "(c=-1) or (c>0 and c>1) or (c=0)");
    QCOMPARE(tab.fields().at(3)->check(), "(((d>0)))");
}
