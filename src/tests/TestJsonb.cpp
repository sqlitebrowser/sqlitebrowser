#include <QtTest/QTest>
#include <sqlite3.h>

#include "TestJsonb.h"
#include "../JsonbUtil.h"

namespace
{
// Let SQLite create the JSONB blob for us
QByteArray makeJsonb(const char* json)
{
    sqlite3* db = nullptr;
    sqlite3_open(":memory:", &db);
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(db, "SELECT jsonb(?1)", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, json, -1, SQLITE_STATIC);
    QByteArray result;
    if(sqlite3_step(stmt) == SQLITE_ROW)
        result = QByteArray(static_cast<const char*>(sqlite3_column_blob(stmt, 0)), sqlite3_column_bytes(stmt, 0));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result;
}
}

void TestJsonb::convertsObjectAndArray()
{
    if(sqlite3_libversion_number() < 3045000)
        QSKIP("JSONB needs SQLite 3.45.0 or newer");

    QString text;
    QVERIFY(jsonbToText(makeJsonb("{\"a\":[1,2],\"b\":\"x\"}"), text));
    QCOMPARE(text, QString("{\"a\":[1,2],\"b\":\"x\"}"));

    QVERIFY(jsonbToText(makeJsonb("[1,{\"c\":null}]"), text));
    QCOMPARE(text, QString("[1,{\"c\":null}]"));

    QVERIFY(jsonbToText(makeJsonb("[]"), text));
    QCOMPARE(text, QString("[]"));

    // Payloads of 256 bytes or more need a two byte size in the header
    const QString long_json = "[\"" + QString(300, 'x') + "\"]";
    QVERIFY(jsonbToText(makeJsonb(long_json.toUtf8().constData()), text));
    QCOMPARE(text, long_json);
}

void TestJsonb::rejectsOtherData()
{
    QString text;

    // Scalars are valid JSONB too, but cannot be told apart from other binary data
    QVERIFY(!jsonbToText(QByteArray(), text));
    QVERIFY(!jsonbToText(QByteArray("\x00", 1), text));
    QVERIFY(!jsonbToText(QByteArray("\x01", 1), text));
    if(sqlite3_libversion_number() >= 3045000)
        QVERIFY(!jsonbToText(makeJsonb("123"), text));

    // Text and arbitrary binary data
    QVERIFY(!jsonbToText(QByteArray("[1,2,3]"), text));
    QVERIFY(!jsonbToText(QByteArray("\x89PNG\r\n\x1a\n", 8), text));

    // Container header with a length that does not match the data
    QVERIFY(!jsonbToText(QByteArray("\xfb\x01\x02", 3), text));
}

QTEST_APPLESS_MAIN(TestJsonb)
