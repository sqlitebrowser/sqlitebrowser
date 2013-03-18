#include "../sqlitetypes.h"

#include <QtTest/QtTest>

using namespace sqlb;

class TestTable: public QObject
{
    Q_OBJECT
private slots:
    void sqlOutput();
    void autoincrement();
    void notnull();

    void parseSQL();
    void parseSQLMultiPk();
};

void TestTable::sqlOutput()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("id", "integer"));
    FieldPtr fkm = FieldPtr(new Field("km", "integer", false, "", "km > 1000"));
    tt.addField(f);
    tt.addField(FieldPtr(new Field("car", "text")));
    tt.addField(fkm);
    FieldVector pk;
    pk.append(f);
    pk.append(fkm);
    tt.setPrimaryKey(pk);

    QCOMPARE(tt.sql(), QString("CREATE TABLE `testtable` (\n"
                               "\tid\tinteger,\n"
                               "\tcar\ttext,\n"
                               "\tkm\tinteger CHECK(km > 1000),\n"
                               "\tPRIMARY KEY(id,km)\n"
                               ");"));
}

void TestTable::autoincrement()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("id", "integer"));
    FieldPtr fkm = FieldPtr(new Field("km", "integer"));
    tt.addField(f);
    tt.addField(FieldPtr(new Field("car", "text")));
    tt.addField(fkm);
    tt.setPrimaryKey(f, true);

    QCOMPARE(tt.sql(), QString("CREATE TABLE `testtable` (\n"
                               "\tid\tinteger PRIMARY KEY AUTOINCREMENT,\n"
                               "\tcar\ttext,\n"
                               "\tkm\tinteger\n"
                               ");"));
}

void TestTable::notnull()
{
    Table tt("testtable");
    FieldPtr f = FieldPtr(new Field("id", "integer"));
    FieldPtr fkm = FieldPtr(new Field("km", "integer"));
    tt.addField(f);
    tt.addField(FieldPtr(new Field("car", "text", true)));
    tt.addField(fkm);
    tt.setPrimaryKey(f, true);

    QCOMPARE(tt.sql(), QString("CREATE TABLE `testtable` (\n"
                               "\tid\tinteger PRIMARY KEY AUTOINCREMENT,\n"
                               "\tcar\ttext NOT NULL,\n"
                               "\tkm\tinteger\n"
                               ");"));
}

void TestTable::parseSQL()
{
    QString sSQL = "CREATE TABLE hero (\n"
            "\tid integer PRIMARY KEY AUTOINCREMENT,\n"
            "\tname text NOT NULL DEFAULT 'xxxx',\n"
            "\tinfo VARCHAR(255) CHECK (info == x)\n"
            ");";

    Table tab = Table::parseSQL(sSQL);

    QVERIFY(tab.name() == "hero");
    QVERIFY(tab.fields().at(0)->name() == "id");
    QVERIFY(tab.fields().at(1)->name() == "name");
    QVERIFY(tab.fields().at(2)->name() == "info");

    QVERIFY(tab.fields().at(0)->type() == "integer");
    QVERIFY(tab.fields().at(1)->type() == "text");
    QVERIFY(tab.fields().at(2)->type() == "VARCHAR(255)");

    QVERIFY(tab.fields().at(0)->autoIncrement());
    QVERIFY(tab.fields().at(1)->notnull());
    QCOMPARE(tab.fields().at(1)->check(), QString(""));
    QCOMPARE(tab.fields().at(2)->check(), QString("(info==x)"));

    QVERIFY(tab.primarykey().contains(tab.fields().at(0)));
}

void TestTable::parseSQLMultiPk()
{
    QString sSQL = "CREATE TABLE hero (\n"
            "\tid1 integer,\n"
            "\tid2 integer,\n"
            "\tnonpkfield blob,\n"
            "PRIMARY KEY(id1,id2)\n"
            ");";

    Table tab = Table::parseSQL(sSQL);

    QVERIFY(tab.name() == "hero");
    QVERIFY(tab.fields().at(0)->name() == "id1");
    QVERIFY(tab.fields().at(1)->name() == "id2");

    QVERIFY(tab.fields().at(0)->type() == "integer");
    QVERIFY(tab.fields().at(1)->type() == "integer");

    QVERIFY(tab.primarykey().contains(tab.fields().at(0)));
    QVERIFY(tab.primarykey().contains(tab.fields().at(1)));
}

QTEST_MAIN(TestTable)
#include "testsqlobjects.moc"
