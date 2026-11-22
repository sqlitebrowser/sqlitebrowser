#include "TestLocaleNumbers.h"

#include "../Settings.h"
#include "../sqlitedb.h"
#include "../sqlitetablemodel.h"
#include "../sql/Query.h"

#include <QApplication>
#include <QFile>
#include <QLocale>
#include <QSignalSpy>
#include <QtTest/QtTest>

QTEST_MAIN(TestLocaleNumbers)

namespace
{

// A database which is closed when it goes out of scope, without asking whether to save the changes, even when a test fails early
class TestDb : public DBBrowserDB
{
public:
    ~TestDb() override
    {
        releaseAllSavepoints();
        close();
    }
};

// Creates the table t in a new database. The columns are i (INTEGER), r (REAL), n (NUMERIC), s (TEXT) and u (no type). The row id is column 0.
bool createTable(DBBrowserDB& db, const QString& fileName, const QStringList& values)
{
    // An empty file is a valid empty database
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
        return false;
    file.close();
    if(!db.open(fileName))
        return false;
    if(!db.executeSQL("CREATE TABLE t(i INTEGER, r REAL, n NUMERIC, s TEXT, u)"))
        return false;
    for(const QString& row : values)
    {
        if(!db.executeSQL("INSERT INTO t VALUES(" + row.toStdString() + ")"))
            return false;
    }
    db.updateSchema();
    return true;
}

// Loads the table t into the model
bool loadTable(SqliteTableModel& model, const sqlb::Query& query)
{
    QSignalSpy rowCountSpy(&model, &SqliteTableModel::finishedRowCount);
    model.setQuery(query);
    return (rowCountSpy.wait(5000) || rowCountSpy.count() > 0) && model.completeCache();
}

sqlb::Query tableQuery()
{
    return sqlb::Query(sqlb::ObjectIdentifier("main", "t"));
}

// What the data browser shows in the cells of a row, without the row id column
QStringList displayedRow(const SqliteTableModel& model, int row)
{
    QStringList result;
    for(int column=1;column<model.columnCount();column++)
        result.push_back(model.data(model.index(row, column), Qt::DisplayRole).toString());
    return result;
}

void setLocaleNumbers(SqliteTableModel& model, bool enabled)
{
    Settings::setValue("databrowser", "locale_numbers", enabled);
    model.reloadSettings();
}

}

void TestLocaleNumbers::initTestCase()
{
    QVERIFY(m_dir.isValid());

    // Do not read or modify the settings of the user
    Settings::setUserSettingsFile(m_dir.filePath("settings.ini"));
}

void TestLocaleNumbers::init()
{
    Settings::clearValue("databrowser", "locale_numbers");

    // Numbers are shown in the format of the default locale, which is the system locale unless the application changes it. A comma as the
    // decimal separator and a dot for grouping the digits make it easy to see what has been converted.
    QLocale::setDefault(QLocale("tr_TR"));
}

void TestLocaleNumbers::cleanup()
{
    QLocale::setDefault(QLocale::c());
}

void TestLocaleNumbers::offByDefault()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    TestDb db;
    QVERIFY(createTable(db, dir.filePath("test.db"), QStringList() << "1234567, 1234.5, 1234.5, '1234567', 1234567"));

    // Nothing has been set, so numbers are shown as they are stored
    SqliteTableModel model(db, nullptr, QString(), true);
    QVERIFY(loadTable(model, tableQuery()));
    QCOMPARE(displayedRow(model, 0), QStringList() << "1234567" << "1234.5" << "1234.5" << "1234567" << "1234567");

    // and turning the option off again has the same result
    setLocaleNumbers(model, true);
    setLocaleNumbers(model, false);
    QCOMPARE(displayedRow(model, 0), QStringList() << "1234567" << "1234.5" << "1234.5" << "1234567" << "1234567");
}

void TestLocaleNumbers::formatsNumbers_data()
{
    QTest::addColumn<QString>("values");
    QTest::addColumn<QStringList>("displayed");

    // Numeric columns are formatted, text columns and columns without a type are not because they can hold anything
    QTest::newRow("typed_columns") << "1234567, 1234.5, 1234.5, '1234567', 1234567"
                                   << (QStringList() << "1.234.567" << "1.234,5" << "1.234,5" << "1234567" << "1234567");
    QTest::newRow("small_numbers") << "7, 0.25, 12, '7', 7"
                                   << (QStringList() << "7" << "0,25" << "12" << "7" << "7");
    QTest::newRow("zero") << "0, 0.0, 0, '0', 0"
                          << (QStringList() << "0" << "0,0" << "0" << "0" << "0");
    QTest::newRow("negative") << "-1234567, -1234.5, -1234567, '-1', -1"
                              << (QStringList() << "-1.234.567" << "-1.234,5" << "-1.234.567" << "-1" << "-1");
    QTest::newRow("negative_fraction_only") << "-1, -0.5, -0.5, '-', 0"
                                            << (QStringList() << "-1" << "-0,5" << "-0,5" << "-" << "0");

    // A real number which has no fraction keeps its decimal digit
    QTest::newRow("whole_real") << "5, 5.0, 5, '5', 5"
                                << (QStringList() << "5" << "5,0" << "5" << "5" << "5");

    // The fraction is not rounded or changed in any way
    QTest::newRow("precision") << "1, 3.14159265358979, 0.1, '1', 1"
                               << (QStringList() << "1" << "3,14159265358979" << "0,1" << "1" << "1");

    // All digits of the largest and smallest integers are kept
    QTest::newRow("integer_limits") << "9223372036854775807, 1.5, -9223372036854775808, '1', 1"
                                    << (QStringList() << "9.223.372.036.854.775.807" << "1,5" << "-9.223.372.036.854.775.808" << "1" << "1");

    // What cannot be recognised as a plain number is left alone
    QTest::newRow("exponent") << "1, 1.0e+20, 1e20, '1', 1"
                              << (QStringList() << "1" << "1.0e+20" << "1.0e+20" << "1" << "1");
    QTest::newRow("infinity") << "1, 9e999, -9e999, '1', 1"
                              << (QStringList() << "1" << "Inf" << "-Inf" << "1" << "1");
    QTest::newRow("text_in_numeric_columns") << "'abc', 'abc', 'abc', 'abc', 'abc'"
                                             << (QStringList() << "abc" << "abc" << "abc" << "abc" << "abc");
    QTest::newRow("text_looking_like_numbers") << "'007', '1 234', '1,5', '007', '007'"
                                               << (QStringList() << "7" << "1 234" << "1,5" << "007" << "007");
    QTest::newRow("null") << "NULL, NULL, NULL, NULL, NULL"
                          << (QStringList() << "NULL" << "NULL" << "NULL" << "NULL" << "NULL");
    QTest::newRow("blob") << "X'0102', X'0102', X'0102', X'0102', X'0102'"
                          << (QStringList() << "BLOB" << "BLOB" << "BLOB" << "BLOB" << "BLOB");
}

void TestLocaleNumbers::formatsNumbers()
{
    QFETCH(QString, values);
    QFETCH(QStringList, displayed);

    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    TestDb db;
    QVERIFY(createTable(db, dir.filePath("test.db"), QStringList() << values));

    SqliteTableModel model(db, nullptr, QString(), true);
    QVERIFY(loadTable(model, tableQuery()));
    setLocaleNumbers(model, true);

    QCOMPARE(displayedRow(model, 0), displayed);
}

void TestLocaleNumbers::keepsEditedValue()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    TestDb db;
    QVERIFY(createTable(db, dir.filePath("test.db"), QStringList() << "1234567, 1234.5, 1234.5, '1234567', 1234567"));

    SqliteTableModel model(db, nullptr, QString(), true);
    QVERIFY(loadTable(model, tableQuery()));
    setLocaleNumbers(model, true);

    // Only what is shown changes. The value which is edited, copied or exported is still the number as it is stored.
    QCOMPARE(model.data(model.index(0, 1), Qt::DisplayRole).toString(), QString("1.234.567"));
    QCOMPARE(model.data(model.index(0, 1), Qt::EditRole).toLongLong(), 1234567LL);
    QCOMPARE(model.data(model.index(0, 2), Qt::EditRole).toDouble(), 1234.5);
    QCOMPARE(model.data(model.index(0, 4), Qt::EditRole).toString(), QString("1234567"));
}

void TestLocaleNumbers::followsLocale_data()
{
    QTest::addColumn<QString>("locale");
    QTest::addColumn<QStringList>("displayed");

    QTest::newRow("turkish") << "tr_TR" << (QStringList() << "1.234.567" << "1.234,5");
    QTest::newRow("german") << "de_DE" << (QStringList() << "1.234.567" << "1.234,5");
    QTest::newRow("english") << "en_US" << (QStringList() << "1,234,567" << "1,234.5");

    // The C locale has no digit grouping and the same decimal point as the data, so nothing changes with it
    QTest::newRow("c") << "C" << (QStringList() << "1234567" << "1234.5");
}

void TestLocaleNumbers::followsLocale()
{
    QFETCH(QString, locale);
    QFETCH(QStringList, displayed);

    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    TestDb db;
    QVERIFY(createTable(db, dir.filePath("test.db"), QStringList() << "1234567, 1234.5, 1, '1', 1"));

    SqliteTableModel model(db, nullptr, QString(), true);
    QVERIFY(loadTable(model, tableQuery()));
    setLocaleNumbers(model, true);

    QLocale::setDefault(QLocale(locale));
    QCOMPARE(displayedRow(model, 0).mid(0, 2), displayed);
}

void TestLocaleNumbers::keepsDisplayFormat()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    TestDb db;
    QVERIFY(createTable(db, dir.filePath("test.db"), QStringList() << "1234567, 1234.5, 1, '1', 1"));

    // A column with a display format shows what the format makes of the value, which must not be changed again
    sqlb::Query query = tableQuery();
    query.selectedColumns().emplace_back("i", "i * 2");
    query.selectedColumns().emplace_back("r", "r");
    query.selectedColumns().emplace_back("n", "n");
    query.selectedColumns().emplace_back("s", "s");
    query.selectedColumns().emplace_back("u", "u");

    SqliteTableModel model(db, nullptr, QString(), true);
    QVERIFY(loadTable(model, query));
    setLocaleNumbers(model, true);

    QCOMPARE(displayedRow(model, 0), QStringList() << "2469134" << "1.234,5" << "1" << "1" << "1");
}

void TestLocaleNumbers::leavesSqlResults()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    TestDb db;
    QVERIFY(createTable(db, dir.filePath("test.db"), QStringList() << "1234567, 1234.5, 1, '1', 1"));

    // The values in the result of a query have no column type which tells whether they are numbers, so they are shown as they are
    SqliteTableModel model(db, nullptr, QString(), true);
    QSignalSpy rowCountSpy(&model, &SqliteTableModel::finishedRowCount);
    model.setQuery(QString("SELECT i, r FROM t"));
    QVERIFY(rowCountSpy.wait(5000) || rowCountSpy.count() > 0);
    QVERIFY(model.completeCache());
    setLocaleNumbers(model, true);

    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole).toString(), QString("1234567"));
    QCOMPARE(model.data(model.index(0, 1), Qt::DisplayRole).toString(), QString("1234.5"));
}
