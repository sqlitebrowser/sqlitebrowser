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
};

void TestTable::sqlOutput()
{
    Table tt("testtable");
    Field* f = new Field("id", "integer");
    Field* fkm = new Field("km", "integer", false, "km > 1000");
    tt.addField(f);
    tt.addField(new Field("car", "text"));
    tt.addField(fkm);
    QList<Field*> pk;
    pk.append(f);
    pk.append(fkm);
    tt.setPrimaryKey(pk);

    QCOMPARE(tt.sql(), QString("CREATE TABLE `testtable` (\n"
                               "\tid\tinteger,\n"
                               "\tcar\ttext,\n"
                               "\tkm\tinteger CHECK(km > 1000),\n"
                               "\tPRIMARY KEY(id,km)\n"
                               ")"));
}

void TestTable::autoincrement()
{
    Table tt("testtable");
    Field* f = new Field("id", "integer");
    Field* fkm = new Field("km", "integer");
    tt.addField(f);
    tt.addField(new Field("car", "text"));
    tt.addField(fkm);
    tt.setPrimaryKey(f, true);

    QCOMPARE(tt.sql(), QString("CREATE TABLE `testtable` (\n"
                               "\tid\tinteger PRIMARY KEY AUTOINCREMENT,\n"
                               "\tcar\ttext,\n"
                               "\tkm\tinteger\n"
                               ")"));
}

void TestTable::notnull()
{
    Table tt("testtable");
    Field* f = new Field("id", "integer");
    Field* fkm = new Field("km", "integer");
    tt.addField(f);
    tt.addField(new Field("car", "text", true));
    tt.addField(fkm);
    tt.setPrimaryKey(f, true);

    QCOMPARE(tt.sql(), QString("CREATE TABLE `testtable` (\n"
                               "\tid\tinteger PRIMARY KEY AUTOINCREMENT,\n"
                               "\tcar\ttext NOT NULL,\n"
                               "\tkm\tinteger\n"
                               ")"));
}

QTEST_MAIN(TestTable)
#include "testsqlobjects.moc"
