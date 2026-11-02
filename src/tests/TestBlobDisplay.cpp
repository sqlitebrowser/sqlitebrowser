#include "TestBlobDisplay.h"

#include "../Settings.h"
#include "../sqlitedb.h"
#include "../sqlitetablemodel.h"

#include <QApplication>
#include <QTemporaryDir>
#include <QtTest/QtTest>

QTEST_MAIN(TestBlobDisplay)

namespace
{

// The cells of the model: column 0 is the id and column 1 the value
//   id  value
//   1   x'010203'       3 bytes
//   2   x'DEADBEEF'     4 bytes
//   3   x'DEADBEEF00'   5 bytes
//   4   NULL
//   5   'text'
//   6   x''             empty
//   7   x'00'           1 byte
class TestDb
{
public:
    TestDb()
    {
        if(!dir.isValid() || !db.create(dir.filePath("test.db")))
            return;
        ok = db.executeSQL("CREATE TABLE b(id INTEGER PRIMARY KEY, v BLOB)") &&
             db.executeSQL("INSERT INTO b VALUES(1,x'010203'),(2,x'DEADBEEF'),(3,x'DEADBEEF00'),(4,NULL),(5,'text'),(6,x''),(7,x'00')");
        db.updateSchema();
    }

    QTemporaryDir dir;
    DBBrowserDB db;
    bool ok = false;
};

// Run the query which shows all cells and wait for the rows
void loadAll(SqliteTableModel& model)
{
    model.setQuery(QString("SELECT id, v FROM b ORDER BY id"));
    QTRY_VERIFY_WITH_TIMEOUT(model.rowCountAvailable() == SqliteTableModel::RowCount::Complete, 5000);
    model.completeCache();
}

QString cell(const SqliteTableModel& model, int row, int role = Qt::DisplayRole)
{
    return model.data(model.index(row, 1), role).toString();
}

void setHexLimit(SqliteTableModel& model, int limit)
{
    Settings::setValue("databrowser", "blob_hex_limit", limit, /* save_to_disk */ false);
    model.reloadSettings();
}

}

void TestBlobDisplay::initTestCase()
{
    // Don't read (or write) the settings of whoever runs the tests
    QVERIFY(settingsDir.isValid());
    Settings::setUserSettingsFile(settingsDir.filePath("settings.ini"));
}

void TestBlobDisplay::init()
{
    // Settings are only changed in memory and are shared by all tests, so start each one with the same ones
    Settings::setValue("databrowser", "blob_text", "BLOB", false);
    Settings::setValue("databrowser", "symbol_limit", 5000, false);
}

void TestBlobDisplay::isOffByDefault()
{
    QCOMPARE(Settings::getValue("databrowser", "blob_hex_limit").toInt(), 0);

    // This is how BLOBs have always been shown
    TestDb t;
    QVERIFY2(t.ok, "could not set up the test database");
    SqliteTableModel model(t.db, nullptr, QString(), true);
    loadAll(model);
    for(int row : {0, 1, 2, 6})
        QCOMPARE(cell(model, row), QString("BLOB"));
}

void TestBlobDisplay::showsPlaceholderWhenOff()
{
    TestDb t;
    QVERIFY2(t.ok, "could not set up the test database");
    SqliteTableModel model(t.db, nullptr, QString(), true);
    loadAll(model);

    // Turning it off again after it was on
    setHexLimit(model, 16);
    QCOMPARE(cell(model, 0), QString("010203"));
    setHexLimit(model, 0);
    for(int row : {0, 1, 2, 6})
        QCOMPARE(cell(model, row), QString("BLOB"));
}

void TestBlobDisplay::showsSmallBlobsAsHex()
{
    TestDb t;
    QVERIFY2(t.ok, "could not set up the test database");
    SqliteTableModel model(t.db, nullptr, QString(), true);
    loadAll(model);

    setHexLimit(model, 16);
    QCOMPARE(cell(model, 0), QString("010203"));
    QCOMPARE(cell(model, 1), QString("DEADBEEF"));
    QCOMPARE(cell(model, 2), QString("DEADBEEF00"));
    QCOMPARE(cell(model, 6), QString("00"));
}

void TestBlobDisplay::usesTheConfiguredPlaceholderForLargerBlobs()
{
    TestDb t;
    QVERIFY2(t.ok, "could not set up the test database");
    SqliteTableModel model(t.db, nullptr, QString(), true);
    loadAll(model);

    Settings::setValue("databrowser", "blob_text", "<binary>", false);
    setHexLimit(model, 4);
    QCOMPARE(cell(model, 0), QString("010203"));
    QCOMPARE(cell(model, 1), QString("DEADBEEF"));
    QCOMPARE(cell(model, 2), QString("<binary>"));
    QCOMPARE(cell(model, 6), QString("00"));
}

void TestBlobDisplay::treatsTheLimitAsInclusive()
{
    TestDb t;
    QVERIFY2(t.ok, "could not set up the test database");
    SqliteTableModel model(t.db, nullptr, QString(), true);
    loadAll(model);

    setHexLimit(model, 3);
    QCOMPARE(cell(model, 0), QString("010203"));
    QCOMPARE(cell(model, 1), QString("BLOB"));

    setHexLimit(model, 2);
    QCOMPARE(cell(model, 0), QString("BLOB"));
    QCOMPARE(cell(model, 6), QString("00"));
}

void TestBlobDisplay::doesNotChangeOtherValues()
{
    TestDb t;
    QVERIFY2(t.ok, "could not set up the test database");
    SqliteTableModel model(t.db, nullptr, QString(), true);
    loadAll(model);

    setHexLimit(model, 100);
    QCOMPARE(cell(model, 3), QString("NULL"));
    QCOMPARE(cell(model, 4), QString("text"));
    QCOMPARE(cell(model, 5), QString(""));
    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole).toString(), QString("1"));
}

void TestBlobDisplay::doesNotChangeTheEditRole()
{
    TestDb t;
    QVERIFY2(t.ok, "could not set up the test database");
    SqliteTableModel model(t.db, nullptr, QString(), true);
    loadAll(model);

    // Copying and editing use the real contents, not what is displayed
    const QVariant before = model.data(model.index(1, 1), Qt::EditRole);
    setHexLimit(model, 100);
    QCOMPARE(model.data(model.index(1, 1), Qt::EditRole), before);
    QCOMPARE(model.data(model.index(1, 1), Qt::EditRole).toByteArray(), QByteArray("\xDE\xAD\xBE\xEF", 4));
}

void TestBlobDisplay::cutsLongHexAtTheSymbolLimit()
{
    TestDb t;
    QVERIFY2(t.ok, "could not set up the test database");
    SqliteTableModel model(t.db, nullptr, QString(), true);
    loadAll(model);

    // The hex string of the 5 bytes in row 2 has 10 characters
    Settings::setValue("databrowser", "symbol_limit", 6, false);
    setHexLimit(model, 100);
    QCOMPARE(cell(model, 2), QString("DEADBE ..."));
    QCOMPARE(cell(model, 0), QString("010203"));
}
