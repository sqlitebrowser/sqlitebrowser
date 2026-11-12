#include "TestImportCsvOptions.h"

#include "../ImportCsvDialog.h"
#include "../Settings.h"
#include "../sqlitedb.h"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QLineEdit>
#include <QListWidget>
#include <QtTest/QtTest>

QTEST_MAIN(TestImportCsvOptions)

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

bool writeFile(const QString& fileName, const QByteArray& content)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
        return false;
    const bool ok = file.write(content) == content.size();
    file.close();
    return ok;
}

// An empty file is a valid empty database
bool openEmptyDb(DBBrowserDB& db, const QString& fileName)
{
    return writeFile(fileName, QByteArray()) && db.open(fileName);
}

QString queryValue(DBBrowserDB& db, const std::string& sql)
{
    return QString::fromUtf8(db.querySingleValueFromDb(sql, false, DBBrowserDB::Wait));
}

// Number of columns of a table in a schema, which is 0 for tables which do not exist
QString columnCount(DBBrowserDB& db, const QString& schema, const QString& table)
{
    return queryValue(db, "SELECT count(*) FROM pragma_table_info('" + table.toStdString() + "', '" + schema.toStdString() + "')");
}

// Commit the import so closing the database does not ask whether to save the changes
bool finish(DBBrowserDB& db)
{
    return db.releaseAllSavepoints() && db.close();
}

// Imports two files with column names in the first line into one table named t. The first file creates the table. Importing the
// second file would ask whether to import into the existing table if the table was not created in the same session of the dialog,
// which is why both files are imported by the same dialog. In between, the table can be changed, e.g. to add a column.
bool importTwoFiles(DBBrowserDB& db, const QString& dir, const QByteArray& first, const QByteArray& second, bool matchColumnsByName, const std::string& changeTable = std::string())
{
    const QString firstFile = dir + "/first.csv";
    const QString secondFile = dir + "/second.csv";
    if(!writeFile(firstFile, first) || !writeFile(secondFile, second))
        return false;

    ImportCsvDialog dialog({firstFile, secondFile}, &db);
    // The list of files is only in use when the dialog is visible
    dialog.show();

    QCheckBox* checkHeader = dialog.findChild<QCheckBox*>("checkboxHeader");
    QLineEdit* editName = dialog.findChild<QLineEdit*>("editName");
    QListWidget* filePicker = dialog.findChild<QListWidget*>("filePicker");
    if(!checkHeader || !editName || !filePicker || filePicker->count() != 2)
        return false;
    checkHeader->setChecked(true);
    editName->setText("t");

    // Import the first file only. The dialog stays open because there is a file left.
    filePicker->item(1)->setCheckState(Qt::Unchecked);
    if(!QMetaObject::invokeMethod(&dialog, "accept", Qt::DirectConnection) || !dialog.isVisible())
        return false;

    if(!changeTable.empty() && !db.executeSQL(changeTable))
        return false;

    // Then import the second file
    if(matchColumnsByName)
    {
        QCheckBox* checkMatch = dialog.findChild<QCheckBox*>("checkMatchColumnsByName");
        if(!checkMatch)
            return false;
        checkMatch->setChecked(true);
    }
    filePicker->item(1)->setCheckState(Qt::Checked);
    return QMetaObject::invokeMethod(&dialog, "accept", Qt::DirectConnection) && !dialog.isVisible();
}

}

void TestImportCsvOptions::initTestCase()
{
    QVERIFY(m_dir.isValid());

    // Do not read or modify the settings of the user
    Settings::setUserSettingsFile(m_dir.filePath("settings.ini"));
}

void TestImportCsvOptions::init()
{
    // The dialog saves its settings when it is accepted. Start every test with the defaults.
    Settings::clearValue("importcsv", "firstrowheader");
}

void TestImportCsvOptions::schemaChoices()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString csvFile = dir.filePath("data.csv");
    QVERIFY(writeFile(csvFile, "1,x\n"));

    TestDb db;
    QVERIFY(openEmptyDb(db, dir.filePath("test.db")));
    QVERIFY(writeFile(dir.filePath("other.db"), QByteArray()));
    QVERIFY(db.attach(dir.filePath("other.db"), "other"));

    ImportCsvDialog dialog({csvFile}, &db);
    QComboBox* comboSchema = dialog.findChild<QComboBox*>("comboSchema");
    QVERIFY(comboSchema);

    // The main schema is the default, as it has always been, and it comes first. The temp schema is offered even though the
    // database has no temporary objects yet and SQLite does not report it. The attached schemata follow.
    QCOMPARE(comboSchema->currentText(), QString("main"));
    QStringList items;
    for(int i=0;i<comboSchema->count();i++)
        items.push_back(comboSchema->itemText(i));
    QCOMPARE(items, QStringList() << "main" << "temp" << "other");

    QVERIFY(finish(db));
}

void TestImportCsvOptions::importIntoSchema_data()
{
    QTest::addColumn<QString>("schema");

    QTest::newRow("main") << "main";
    QTest::newRow("temp") << "temp";
    QTest::newRow("attached") << "other";
}

void TestImportCsvOptions::importIntoSchema()
{
    QFETCH(QString, schema);

    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString csvFile = dir.filePath("data.csv");
    QVERIFY(writeFile(csvFile, "a,b\n1,x\n2,y\n"));

    TestDb db;
    QVERIFY(openEmptyDb(db, dir.filePath("test.db")));
    QVERIFY(writeFile(dir.filePath("other.db"), QByteArray()));
    QVERIFY(db.attach(dir.filePath("other.db"), "other"));

    ImportCsvDialog dialog({csvFile}, &db);
    QCheckBox* checkHeader = dialog.findChild<QCheckBox*>("checkboxHeader");
    QVERIFY(checkHeader);
    checkHeader->setChecked(true);
    QComboBox* comboSchema = dialog.findChild<QComboBox*>("comboSchema");
    QVERIFY(comboSchema);
    const int index = comboSchema->findText(schema);
    QVERIFY(index != -1);
    comboSchema->setCurrentIndex(index);

    // This imports the file into a table named after the file
    QVERIFY(QMetaObject::invokeMethod(&dialog, "accept", Qt::DirectConnection));

    // The table has been created in the selected schema, with the rows in it, and in no other one
    const QStringList schemata = QStringList() << "main" << "temp" << "other";
    for(const QString& s : schemata)
        QCOMPARE(columnCount(db, s, "data"), QString(s == schema ? "2" : "0"));
    QCOMPARE(queryValue(db, "SELECT group_concat(a || b, ',') FROM " + sqlb::escapeIdentifier(schema.toStdString()) + ".data"),
             QString("1x,2y"));

    QVERIFY(finish(db));
}

void TestImportCsvOptions::importIgnoresSameNameInOtherSchema()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString csvFile = dir.filePath("data.csv");
    QVERIFY(writeFile(csvFile, "1,x\n2,y\n"));

    TestDb db;
    QVERIFY(openEmptyDb(db, dir.filePath("test.db")));

    // The table in main has a different number of columns than the file. Importing into it would be refused, but the
    // import goes to the temp schema where there is no such table yet.
    QVERIFY(db.executeSQL("CREATE TABLE main.data(a, b, c)"));
    QVERIFY(db.executeSQL("INSERT INTO main.data VALUES(0, 0, 0)"));

    ImportCsvDialog dialog({csvFile}, &db);
    QComboBox* comboSchema = dialog.findChild<QComboBox*>("comboSchema");
    QVERIFY(comboSchema);
    comboSchema->setCurrentIndex(comboSchema->findText("temp"));
    QCOMPARE(comboSchema->currentText(), QString("temp"));

    QVERIFY(QMetaObject::invokeMethod(&dialog, "accept", Qt::DirectConnection));

    QCOMPARE(queryValue(db, "SELECT count(*) FROM temp.data"), QString("2"));
    QCOMPARE(queryValue(db, "SELECT count(*) FROM main.data"), QString("1"));

    QVERIFY(finish(db));
}

void TestImportCsvOptions::importIntoMainWhenTempHasSameName()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString csvFile = dir.filePath("data.csv");
    QVERIFY(writeFile(csvFile, "1,x\n2,y\n"));

    TestDb db;
    QVERIFY(openEmptyDb(db, dir.filePath("test.db")));
    QVERIFY(db.executeSQL("CREATE TEMP TABLE data(a, b)"));

    // The default is the main schema. The temp table of the same name must not receive the rows, even though an
    // unqualified table name would find it first.
    ImportCsvDialog dialog({csvFile}, &db);
    QVERIFY(QMetaObject::invokeMethod(&dialog, "accept", Qt::DirectConnection));

    QCOMPARE(queryValue(db, "SELECT count(*) FROM main.data"), QString("2"));
    QCOMPARE(queryValue(db, "SELECT count(*) FROM temp.data"), QString("0"));

    QVERIFY(finish(db));
}

void TestImportCsvOptions::matchColumnsByName_data()
{
    QTest::addColumn<QByteArray>("second");
    QTest::addColumn<bool>("matchColumnsByName");
    QTest::addColumn<QString>("changeTable");
    QTest::addColumn<QString>("rows");

    // The table is created by the first file as t(a INTEGER, b TEXT) and contains 1,'x' and 2,'y'

    // Without the option the values are inserted in the order of the columns of the file, as they have always been
    QTest::newRow("default_positional") << QByteArray("b,a\nz,3\n") << false << QString()
                                        << "1,'x'\n2,'y'\n'z','3'";

    QTest::newRow("reordered") << QByteArray("b,a\nz,3\n") << true << QString()
                               << "1,'x'\n2,'y'\n3,'z'";
    QTest::newRow("same_order") << QByteArray("a,b\n3,z\n") << true << QString()
                                << "1,'x'\n2,'y'\n3,'z'";
    QTest::newRow("case_insensitive") << QByteArray("B,A\nz,3\n") << true << QString()
                                      << "1,'x'\n2,'y'\n3,'z'";

    // Columns of the file which are not in the table are skipped and columns of the table which are not in the file get no value
    QTest::newRow("unknown_column_in_file") << QByteArray("x,b\nignored,z\n") << true << QString()
                                            << "1,'x'\n2,'y'\nNULL,'z'";
    QTest::newRow("unknown_column_last_in_file") << QByteArray("b,x\nz,ignored\n") << true << QString()
                                                 << "1,'x'\n2,'y'\nNULL,'z'";
    QTest::newRow("fewer_columns_in_file") << QByteArray("b\nz\n") << true << QString()
                                           << "1,'x'\n2,'y'\nNULL,'z'";
    QTest::newRow("more_columns_in_file") << QByteArray("c,b,x,a\nfoo,z,bar,3\n") << true
                                          << "ALTER TABLE t ADD COLUMN c TEXT"
                                          << "1,'x'\n2,'y'\n3,'z'";

    // The first of two columns with the same name wins
    QTest::newRow("duplicate_column_in_file") << QByteArray("a,a\n7,8\n") << true << QString()
                                              << "1,'x'\n2,'y'\n7,NULL";

    // Columns which are not in the file get their default value
    QTest::newRow("default_value") << QByteArray("b,a\nz,3\n") << true
                                   << "ALTER TABLE t ADD COLUMN c TEXT DEFAULT 'dflt'"
                                   << "1,'x','dflt'\n2,'y','dflt'\n3,'z','dflt'";

    // Empty values are replaced depending on the column they go to, not on the column they are at in the file
    QTest::newRow("empty_values") << QByteArray("c,a,b\n,5,\n") << true
                                  << "ALTER TABLE t ADD COLUMN c INTEGER NOT NULL DEFAULT 7"
                                  << "1,'x',7\n2,'y',7\n5,NULL,7";
}

void TestImportCsvOptions::matchColumnsByName()
{
    QFETCH(QByteArray, second);
    QFETCH(bool, matchColumnsByName);
    QFETCH(QString, changeTable);
    QFETCH(QString, rows);

    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    TestDb db;
    QVERIFY(openEmptyDb(db, dir.filePath("test.db")));

    QVERIFY(importTwoFiles(db, dir.path(), "a,b\n1,x\n2,y\n", second, matchColumnsByName, changeTable.toStdString()));

    // Show each row as a string, e.g. 7,'abc'
    QStringList columns;
    for(int i=0;i<rows.section('\n', 0, 0).count(',')+1;i++)
        columns.push_back(QString("quote(%1)").arg(QChar('a' + i)));
    QCOMPARE(queryValue(db, "SELECT group_concat(r, char(10)) FROM (SELECT " + columns.join(" || ',' || ").toStdString() + " AS r FROM t ORDER BY rowid)"),
             rows);

    QVERIFY(finish(db));
}

void TestImportCsvOptions::matchColumnsByNameNeedsHeader()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString csvFile = dir.filePath("data.csv");
    QVERIFY(writeFile(csvFile, "a,b\n1,x\n"));

    TestDb db;
    QVERIFY(openEmptyDb(db, dir.filePath("test.db")));

    ImportCsvDialog dialog({csvFile}, &db);
    QCheckBox* checkHeader = dialog.findChild<QCheckBox*>("checkboxHeader");
    QVERIFY(checkHeader);
    QCheckBox* checkMatch = dialog.findChild<QCheckBox*>("checkMatchColumnsByName");
    QVERIFY(checkMatch);

    // The option is off unless the user turns it on. Without names in the file there is nothing to match.
    QCOMPARE(checkMatch->isChecked(), false);
    checkHeader->setChecked(false);
    QCOMPARE(checkMatch->isEnabled(), false);
    checkHeader->setChecked(true);
    QCOMPARE(checkMatch->isEnabled(), true);
    checkHeader->setChecked(false);
    QCOMPARE(checkMatch->isEnabled(), false);
}
