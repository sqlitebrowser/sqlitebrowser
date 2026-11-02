#include "TestImportCsvDialog.h"

#include "../ImportCsvDialog.h"
#include "../Settings.h"
#include "../sqlitedb.h"

#include <QApplication>
#include <QCheckBox>
#include <QFile>
#include <QtTest/QtTest>

QTEST_MAIN(TestImportCsvDialog)

void TestImportCsvDialog::initTestCase()
{
    QVERIFY(m_dir.isValid());

    // Do not read or modify the settings of the user
    Settings::setUserSettingsFile(m_dir.filePath("settings.ini"));
}

void TestImportCsvDialog::init()
{
    // The dialog saves its settings when it is accepted. Start every test with the defaults.
    Settings::clearValue("importcsv", "quotedastext");
    Settings::clearValue("importcsv", "localconventions");
}

void TestImportCsvDialog::quotedValues_data()
{
    QTest::addColumn<QByteArray>("csv");
    QTest::addColumn<bool>("quotedAsText");
    QTest::addColumn<bool>("localConventions");
    QTest::addColumn<QString>("types");
    QTest::addColumn<QString>("rows");

    // By default, type detection looks at the contents of the fields, not at how they are written in the file
    QTest::newRow("default") << QByteArray("\"007\",x\n\"123\",y\n")
                             << false << false
                             << "INTEGER,TEXT"
                             << "7,'x'\n123,'y'";
    QTest::newRow("default_local_conventions") << QByteArray("\"007\",x\n\"123\",y\n")
                                               << false << true
                                               << "INTEGER,TEXT"
                                               << "7,'x'\n123,'y'";

    // With the option set, quoted values are never converted
    QTest::newRow("quoted_as_text") << QByteArray("\"007\",x\n\"123\",y\n")
                                    << true << false
                                    << "TEXT,TEXT"
                                    << "'007','x'\n'123','y'";
    QTest::newRow("quoted_as_text_local_conventions") << QByteArray("\"007\",x\n\"123\",y\n")
                                                      << true << true
                                                      << "TEXT,TEXT"
                                                      << "'007','x'\n'123','y'";
    QTest::newRow("quoted_as_text_real") << QByteArray("\"1.50\",\"2\"\n\"3.25\",\"4\"\n")
                                         << true << false
                                         << "TEXT,TEXT"
                                         << "'1.50','2'\n'3.25','4'";

    // Only the columns containing quoted values are affected. Decimal numbers are only used without local number conventions
    // because the result would depend on the system locale otherwise.
    QTest::newRow("quoted_as_text_unquoted_numbers") << QByteArray("007,\"007\",1.5\n123,\"123\",2.5\n")
                                                     << true << false
                                                     << "INTEGER,TEXT,REAL"
                                                     << "7,'007',1.5\n123,'123',2.5";
    QTest::newRow("quoted_as_text_unquoted_numbers_local_conventions") << QByteArray("007,\"007\",15\n123,\"123\",25\n")
                                                                       << true << true
                                                                       << "INTEGER,TEXT,INTEGER"
                                                                       << "7,'007',15\n123,'123',25";
    QTest::newRow("quoted_as_text_text_unaffected") << QByteArray("\"abc\",def\n")
                                                    << true << false
                                                    << "TEXT,TEXT"
                                                    << "'abc','def'";

    // A single quoted value is enough to make the column TEXT. The other values are then converted by the column affinity.
    QTest::newRow("quoted_as_text_one_quoted") << QByteArray("1,x\n\"2\",y\n3,z\n")
                                               << true << false
                                               << "TEXT,TEXT"
                                               << "'1','x'\n'2','y'\n'3','z'";
}

void TestImportCsvDialog::quotedValues()
{
    QFETCH(QByteArray, csv);
    QFETCH(bool, quotedAsText);
    QFETCH(bool, localConventions);
    QFETCH(QString, types);
    QFETCH(QString, rows);

    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    const QString csvFile = dir.filePath("quoted.csv");
    QFile file(csvFile);
    QVERIFY(file.open(QIODevice::WriteOnly));
    QCOMPARE(file.write(csv), static_cast<qint64>(csv.size()));
    file.close();

    // An empty file is a valid empty database
    const QString dbFile = dir.filePath("test.db");
    QFile dbFileCreator(dbFile);
    QVERIFY(dbFileCreator.open(QIODevice::WriteOnly));
    dbFileCreator.close();

    DBBrowserDB db;
    QVERIFY(db.open(dbFile));

    ImportCsvDialog dialog({csvFile}, &db);
    QCheckBox* checkQuotedAsText = dialog.findChild<QCheckBox*>("checkQuotedAsText");
    QVERIFY(checkQuotedAsText);
    QCheckBox* checkLocalConventions = dialog.findChild<QCheckBox*>("checkLocalConventions");
    QVERIFY(checkLocalConventions);

    // The option must be off unless the user turns it on
    QCOMPARE(checkQuotedAsText->isChecked(), false);

    checkQuotedAsText->setChecked(quotedAsText);
    checkLocalConventions->setChecked(localConventions);

    // This imports the file into a table named after the file
    QVERIFY(QMetaObject::invokeMethod(&dialog, "accept", Qt::DirectConnection));

    // Show each row as a string, e.g. 7,'abc'
    QStringList columns;
    for(int i=1;i<=types.count(',')+1;i++)
        columns.push_back(QString("quote(field%1)").arg(i));
    const std::string select_rows = "SELECT group_concat(r, char(10)) FROM (SELECT " + columns.join(" || ',' || ").toStdString() +
            " AS r FROM quoted ORDER BY rowid)";

    QCOMPARE(QString::fromUtf8(db.querySingleValueFromDb(
                 "SELECT group_concat(type, ',') FROM (SELECT type FROM pragma_table_info('quoted') ORDER BY cid)", false, DBBrowserDB::Wait)),
             types);
    QCOMPARE(QString::fromUtf8(db.querySingleValueFromDb(select_rows, false, DBBrowserDB::Wait)), rows);

    // Commit the import so closing the database does not ask whether to save the changes
    QVERIFY(db.releaseAllSavepoints());
    QVERIFY(db.close());
}
