#include "testsqlobjects.h"
#include "../sqlitetypes.h"

#include <QtTest/QtTest>

QTEST_APPLESS_MAIN(TestTable)

using namespace sqlb;

void TestTable::sqlOutput()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("id", "integer"));
    f->setPrimaryKey(true);
    FieldPtr fkm = FieldPtr(new Field("km", "integer", false, "", "km > 1000"));
    fkm->setPrimaryKey(true);
    tt.addField(f);
    tt.addField(FieldPtr(new Field("car", "text")));
    tt.addField(fkm);

    QCOMPARE(tt.sql(), QString("CREATE TABLE `testtable` (\n"
                               "\t`id`\tinteger,\n"
                               "\t`car`\ttext,\n"
                               "\t`km`\tinteger CHECK(km > 1000),\n"
                               "\tPRIMARY KEY(id,km)\n"
                               ");"));
}

void TestTable::autoincrement()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("id", "integer"));
    f->setPrimaryKey(true);
    f->setAutoIncrement(true);
    FieldPtr fkm = FieldPtr(new Field("km", "integer"));
    tt.addField(f);
    tt.addField(FieldPtr(new Field("car", "text")));
    tt.addField(fkm);

    QCOMPARE(tt.sql(), QString("CREATE TABLE `testtable` (\n"
                               "\t`id`\tinteger PRIMARY KEY AUTOINCREMENT,\n"
                               "\t`car`\ttext,\n"
                               "\t`km`\tinteger\n"
                               ");"));
}

void TestTable::notnull()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("id", "integer"));
    f->setPrimaryKey(true);
    f->setAutoIncrement(true);
    FieldPtr fkm = FieldPtr(new Field("km", "integer"));
    tt.addField(f);
    tt.addField(FieldPtr(new Field("car", "text", true)));
    tt.addField(fkm);

    QCOMPARE(tt.sql(), QString("CREATE TABLE `testtable` (\n"
                               "\t`id`\tinteger PRIMARY KEY AUTOINCREMENT,\n"
                               "\t`car`\ttext NOT NULL,\n"
                               "\t`km`\tinteger\n"
                               ");"));
}

void TestTable::withoutRowid()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("a", "integer"));
    f->setPrimaryKey(true);
    f->setAutoIncrement(true);
    tt.addField(f);
    tt.addField(FieldPtr(new Field("b", "integer")));
    tt.setRowidColumn("a");

    QCOMPARE(tt.sql(), QString("CREATE TABLE `testtable` (\n"
                               "\t`a`\tinteger PRIMARY KEY AUTOINCREMENT,\n"
                               "\t`b`\tinteger\n"
                               ") WITHOUT ROWID;"));
}

void TestTable::foreignKeys()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("a", "integer"));
    f->setForeignKey(sqlb::ForeignKeyClause("b", QStringList("c")));
    tt.addField(f);

    QCOMPARE(tt.sql(), QString("CREATE TABLE `testtable` (\n"
                               "\t`a`\tinteger,\n"
                               "\tFOREIGN KEY(`a`) REFERENCES `b`(`c`)\n"
                               ");"));
}

void TestTable::parseSQL()
{
    QString sSQL = "create TABLE hero (\n"
            "\tid integer PRIMARY KEY AUTOINCREMENT,\n"
            "\tname text NOT NULL DEFAULT 'xxxx',\n"
            "\tinfo VARCHAR(255) CHECK (info == 'x')\n"
            ");";

    Table tab = Table::parseSQL(sSQL).first;

    QVERIFY(tab.name() == "hero");
    QVERIFY(tab.rowidColumn() == "_rowid_");
    QVERIFY(tab.fields().at(0)->name() == "id");
    QVERIFY(tab.fields().at(1)->name() == "name");
    QVERIFY(tab.fields().at(2)->name() == "info");

    QVERIFY(tab.fields().at(0)->type() == "integer");
    QVERIFY(tab.fields().at(1)->type() == "text");
    QVERIFY(tab.fields().at(2)->type() == "VARCHAR(255)");

    QVERIFY(tab.fields().at(0)->autoIncrement());
    QVERIFY(tab.fields().at(0)->primaryKey());
    QVERIFY(tab.fields().at(1)->notnull());
    QCOMPARE(tab.fields().at(1)->defaultValue(), QString("'xxxx'"));
    QCOMPARE(tab.fields().at(1)->check(), QString(""));
    QCOMPARE(tab.fields().at(2)->check(), QString("info == 'x'"));
}

void TestTable::parseSQLdefaultexpr()
{
    QString sSQL = "CREATE TABLE chtest(\n"
            "id integer primary key,\n"
            "dumpytext text default('axa') CHECK(dumpytext == \"aa\"),\n"
            "date datetime default CURRENT_TIMESTAMP,"
            "zoi integer)";

    Table tab = Table::parseSQL(sSQL).first;

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
    QCOMPARE(tab.fields().at(1)->check(), QString("dumpytext == \"aa\""));
    QCOMPARE(tab.fields().at(2)->defaultValue(), QString("CURRENT_TIMESTAMP"));
    QCOMPARE(tab.fields().at(2)->check(), QString(""));
    QCOMPARE(tab.fields().at(3)->defaultValue(), QString(""));
    QCOMPARE(tab.fields().at(3)->check(), QString(""));

    QVERIFY(tab.fields().at(0)->primaryKey());
}

void TestTable::parseSQLMultiPk()
{
    QString sSQL = "CREATE TABLE hero (\n"
            "\tid1 integer,\n"
            "\tid2 integer,\n"
            "\tnonpkfield blob,\n"
            "PRIMARY KEY(id1,id2)\n"
            ");";

    Table tab = Table::parseSQL(sSQL).first;

    QVERIFY(tab.name() == "hero");
    QVERIFY(tab.fields().at(0)->name() == "id1");
    QVERIFY(tab.fields().at(1)->name() == "id2");

    QVERIFY(tab.fields().at(0)->type() == "integer");
    QVERIFY(tab.fields().at(1)->type() == "integer");

    QVERIFY(tab.fields().at(0)->primaryKey());
    QVERIFY(tab.fields().at(1)->primaryKey());
}

void TestTable::parseSQLForeignKey()
{
    QString sSQL = "CREATE TABLE grammar_test(id, test, FOREIGN KEY(test) REFERENCES other_table);";

    Table tab = Table::parseSQL(sSQL).first;

    QVERIFY(tab.name() == "grammar_test");
    QVERIFY(tab.fields().at(0)->name() == "id");
    QVERIFY(tab.fields().at(1)->name() == "test");
}

void TestTable::parseSQLSingleQuotes()
{
    QString sSQL = "CREATE TABLE 'test'('id','test');";

    Table tab = Table::parseSQL(sSQL).first;

    QVERIFY(tab.name() == "test");
    QVERIFY(tab.fields().at(0)->name() == "id");
    QVERIFY(tab.fields().at(1)->name() == "test");
}

void TestTable::parseSQLKeywordInIdentifier()
{
    QString sSQL = "CREATE TABLE deffered(key integer primary key, if text);";

    Table tab = Table::parseSQL(sSQL).first;

    QVERIFY(tab.name() == "deffered");
    QVERIFY(tab.fields().at(0)->name() == "key");
    QVERIFY(tab.fields().at(1)->name() == "if");
}

void TestTable::parseSQLWithoutRowid()
{
    QString sSQL = "CREATE TABLE test(a integer primary key, b integer) WITHOUT ROWID;";

    Table tab = Table::parseSQL(sSQL).first;

    QVERIFY(tab.fields().at(tab.findPk())->name() == "a");
    QVERIFY(tab.rowidColumn() == "a");
}

void TestTable::parseNonASCIIChars()
{
    QString sSQL = "CREATE TABLE `lösung` ("
            "`Fieldöäüß`	INTEGER,"
            "PRIMARY KEY(Fieldöäüß)"
            ");";

    Table tab = Table::parseSQL(sSQL).first;

    QVERIFY(tab.name() == "lösung");
    QVERIFY(tab.fields().at(0)->name() == "Fieldöäüß");
}

void TestTable::parseSQLEscapedQuotes()
{
    QString sSql = "CREATE TABLE double_quotes(a text default 'a''a');";

    Table tab = Table::parseSQL(sSql).first;

    QCOMPARE(tab.name(), QString("double_quotes"));
    QCOMPARE(tab.fields().at(0)->name(), QString("a"));
    QCOMPARE(tab.fields().at(0)->defaultValue(), QString("'a''a'"));
}

void TestTable::parseSQLForeignKeys()
{
    QString sql = "CREATE TABLE foreign_key_test(a int, b int, foreign key (a) references x, foreign key (b) references w(y,z) on delete set null);";

    Table tab = Table::parseSQL(sql).first;

    QCOMPARE(tab.name(), QString("foreign_key_test"));
    QCOMPARE(tab.fields().at(0)->name(), QString("a"));
    QCOMPARE(tab.fields().at(0)->type(), QString("int"));
    QCOMPARE(tab.fields().at(0)->foreignKey().table(), QString("x"));
    QCOMPARE(tab.fields().at(1)->name(), QString("b"));
    QCOMPARE(tab.fields().at(1)->type(), QString("int"));
    QCOMPARE(tab.fields().at(1)->foreignKey().toString(), QString("`w`(`y`,`z`) on delete set null"));
}

void TestTable::parseSQLCheckConstraint()
{
    QString sql = "CREATE TABLE a (`b` text CHECK(`b`='A' or `b`='B'));";

    Table tab = Table::parseSQL(sql).first;

    QCOMPARE(tab.name(), QString("a"));
    QCOMPARE(tab.fields().at(0)->name(), QString("b"));
    QCOMPARE(tab.fields().at(0)->type(), QString("text"));
    QCOMPARE(tab.fields().at(0)->check(), QString("`b` = 'A' or `b` = 'B'"));
}

void TestTable::createTableWithIn()
{
    QString sSQL = "CREATE TABLE not_working("
            "_id PRIMARY KEY NOT NULL,"
            "value NVARCHAR(5) CHECK (value IN ('a', 'b', 'c'))"
            ");";

    Table tab = Table::parseSQL(sSQL).first;
    QVERIFY(tab.name() == "not_working");

    QVERIFY(tab.fields().at(1)->check() == "value IN ( 'a' , 'b' , 'c' )");
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

    Table tab = Table::parseSQL(sSQL).first;
    QVERIFY(tab.name() == "hopefully_working");

    QVERIFY(tab.fields().at(0)->check() == "value NOT LIKE 'prefix%'");
    QVERIFY(tab.fields().at(1)->check() == "value2 NOT MATCH 'prefix%'");
    QVERIFY(tab.fields().at(2)->check() == "value3 NOT REGEXP 'prefix%'");
    QVERIFY(tab.fields().at(3)->check() == "value4 NOT GLOB 'prefix%'");
    QVERIFY(tab.fields().at(4)->check() == "value5 BETWEEN 1 + 4 AND 100 OR 200");
    QVERIFY(tab.fields().at(5)->check() == "value6 NOT BETWEEN 1 AND 100");
    QVERIFY(tab.fields().at(6)->check() == "NOT EXISTS ( 1 )");
}
