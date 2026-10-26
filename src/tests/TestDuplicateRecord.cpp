#include "TestDuplicateRecord.h"

#include "../sqlitedb.h"
#include "../sqlitetablemodel.h"
#include "../sql/Query.h"

#include <QApplication>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTimer>
#include <QtTest/QtTest>

QTEST_MAIN(TestDuplicateRecord)

namespace
{

// All rows of the test table 't', each one rendered as a single string, ordered by the given expression
QStringList tableRows(DBBrowserDB& db, const QString& columns, const QString& order)
{
    const std::string sql = "SELECT group_concat(r, char(10)) FROM (SELECT " + columns.toStdString() + " AS r FROM t ORDER BY " + order.toStdString() + ")";
    const QString rows = QString::fromUtf8(db.querySingleValueFromDb(sql, false, DBBrowserDB::Wait));
    return rows.isEmpty() ? QStringList() : rows.split('\n');
}

}

void TestDuplicateRecord::duplicateKeepsExistingRecords_data()
{
    QTest::addColumn<QString>("create");
    QTest::addColumn<QString>("seed");
    QTest::addColumn<QString>("columns");
    QTest::addColumn<QString>("order");
    QTest::addColumn<int>("rowToDuplicate");
    QTest::addColumn<int>("sortColumn");
    QTest::addColumn<bool>("loadAll");

    const QString columns = "quote(a)||','||quote(b)||','||quote(c)";

    QTest::newRow("no primary key")
        << "CREATE TABLE t(a INTEGER, b TEXT, c TEXT)"
        << "INSERT INTO t VALUES(1,'one','x'),(2,'two','y'),(3,'three','z')"
        << columns << "rowid" << 1 << -1 << true;
    QTest::newRow("integer primary key")
        << "CREATE TABLE t(a INTEGER PRIMARY KEY, b TEXT, c TEXT)"
        << "INSERT INTO t VALUES(1,'one','x'),(2,'two','y'),(3,'three','z')"
        << columns << "a" << 1 << -1 << true;
    QTest::newRow("integer primary key autoincrement")
        << "CREATE TABLE t(a INTEGER PRIMARY KEY AUTOINCREMENT, b TEXT, c TEXT)"
        << "INSERT INTO t VALUES(1,'one','x'),(2,'two','y'),(3,'three','z')"
        << columns << "a" << 1 << -1 << true;
    QTest::newRow("integer primary key not null")
        << "CREATE TABLE t(a INTEGER NOT NULL PRIMARY KEY, b TEXT, c TEXT)"
        << "INSERT INTO t VALUES(1,'one','x'),(2,'two','y'),(3,'three','z')"
        << columns << "a" << 1 << -1 << true;
    QTest::newRow("text primary key")
        << "CREATE TABLE t(a TEXT PRIMARY KEY, b TEXT, c TEXT)"
        << "INSERT INTO t VALUES('1','one','x'),('2','two','y'),('3','three','z')"
        << columns << "a" << 1 << -1 << true;
    QTest::newRow("composite primary key")
        << "CREATE TABLE t(a INTEGER, b INTEGER, c TEXT, PRIMARY KEY(a,b))"
        << "INSERT INTO t VALUES(1,1,'x'),(1,2,'y'),(2,1,'z')"
        << columns << "a,b" << 1 << -1 << true;
    QTest::newRow("without rowid")
        << "CREATE TABLE t(a INTEGER PRIMARY KEY, b TEXT, c TEXT) WITHOUT ROWID"
        << "INSERT INTO t VALUES(1,'one','x'),(2,'two','y'),(3,'three','z')"
        << columns << "a" << 1 << -1 << true;
    QTest::newRow("without rowid composite primary key")
        << "CREATE TABLE t(a INTEGER, b INTEGER, c TEXT, PRIMARY KEY(a,b)) WITHOUT ROWID"
        << "INSERT INTO t VALUES(1,1,'x'),(1,2,'y'),(2,1,'z')"
        << columns << "a,b" << 1 << -1 << true;
    QTest::newRow("unique column")
        << "CREATE TABLE t(a INTEGER PRIMARY KEY, b TEXT UNIQUE, c TEXT)"
        << "INSERT INTO t VALUES(1,'one','x'),(2,'two','y'),(3,'three','z')"
        << columns << "a" << 1 << -1 << true;
    QTest::newRow("sorted descending")
        << "CREATE TABLE t(a INTEGER PRIMARY KEY, b TEXT, c TEXT)"
        << "INSERT INTO t VALUES(1,'one','x'),(2,'two','y'),(3,'three','z')"
        << columns << "a" << 0 << 1 << true;
    QTest::newRow("sorted descending text")
        << "CREATE TABLE t(a INTEGER PRIMARY KEY, b TEXT, c TEXT)"
        << "INSERT INTO t VALUES(1,'one','x'),(2,'two','y'),(3,'three','z')"
        << columns << "a" << 0 << 2 << true;
    QTest::newRow("large partially loaded")
        << "CREATE TABLE t(a INTEGER PRIMARY KEY, b TEXT, c TEXT)"
        << "WITH RECURSIVE s(n) AS (SELECT 1 UNION ALL SELECT n+1 FROM s WHERE n<120000) INSERT INTO t SELECT n, 'v'||n, 'x' FROM s"
        << columns << "a" << 5 << -1 << false;
    QTest::newRow("large partially loaded no pk")
        << "CREATE TABLE t(a INTEGER, b TEXT, c TEXT)"
        << "WITH RECURSIVE s(n) AS (SELECT 1 UNION ALL SELECT n+1 FROM s WHERE n<120000) INSERT INTO t SELECT n, 'v'||n, 'x' FROM s"
        << columns << "rowid" << 5 << -1 << false;
}

void TestDuplicateRecord::duplicateKeepsExistingRecords()
{
    QFETCH(QString, create);
    QFETCH(QString, seed);
    QFETCH(QString, columns);
    QFETCH(QString, order);
    QFETCH(int, rowToDuplicate);
    QFETCH(int, sortColumn);
    QFETCH(bool, loadAll);

    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    DBBrowserDB db;
    QVERIFY(db.create(dir.filePath("test.db")));
    QVERIFY(db.executeSQL(create.toStdString()));
    QVERIFY(db.executeSQL(seed.toStdString()));
    db.updateSchema();

    const QStringList before = tableRows(db, columns, order);

    SqliteTableModel model(db, nullptr, QString(), true);
    QSignalSpy rowCountSpy(&model, &SqliteTableModel::finishedRowCount);
    model.setQuery(sqlb::Query(sqlb::ObjectIdentifier("main", "t")));
    QVERIFY(rowCountSpy.wait(5000) || rowCountSpy.count() > 0);
    QCOMPARE(model.rowCount(), before.size());
    if(sortColumn >= 0)
        model.sort(sortColumn, Qt::DescendingOrder);
    if(loadAll)
        QVERIFY(model.completeCache());
    else
        model.waitUntilIdle();

    // Any message box popping up (e.g. for a failed update) would block the test, so close it and remember it
    bool dialogShown = false;
    QTimer closer;
    connect(&closer, &QTimer::timeout, this, [&]() {
        if(QWidget* w = QApplication::activeModalWidget())
        {
            dialogShown = true;
            w->close();
        }
    });
    closer.start(50);

    model.dittoRecord(rowToDuplicate);
    closer.stop();

    // The original records must all still be there, unmodified, and the table must have received one more record.
    const QStringList after = tableRows(db, columns, order);
    QVERIFY2(!dialogShown, "an error dialog was shown while duplicating");
    QCOMPARE(after.size(), before.size() + 1);
    const QSet<QString> afterSet = QSet<QString>::fromList(after);
    for(const QString& row : before)
        QVERIFY2(afterSet.contains(row), qPrintable("record was lost or overwritten: " + row));
}

void TestDuplicateRecord::duplicateTwiceWithoutRowidCompositeKey()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    DBBrowserDB db;
    QVERIFY(db.create(dir.filePath("test.db")));
    QVERIFY(db.executeSQL("CREATE TABLE t(a INTEGER, b INTEGER, c TEXT, PRIMARY KEY(a,b)) WITHOUT ROWID"));
    QVERIFY(db.executeSQL("INSERT INTO t VALUES(1,1,'x'),(1,2,'y'),(2,1,'z')"));
    db.updateSchema();

    SqliteTableModel model(db, nullptr, QString(), true);
    QSignalSpy rowCountSpy(&model, &SqliteTableModel::finishedRowCount);
    model.setQuery(sqlb::Query(sqlb::ObjectIdentifier("main", "t")));
    QVERIFY(rowCountSpy.wait(5000) || rowCountSpy.count() > 0);
    QVERIFY(model.completeCache());

    // Any message box popping up would block the test, so close it and remember it
    bool dialogShown = false;
    QTimer closer;
    connect(&closer, &QTimer::timeout, this, [&]() {
        if(QWidget* w = QApplication::activeModalWidget())
        {
            dialogShown = true;
            w->close();
        }
    });
    closer.start(50);

    // The identifier of the new row must match the inserted row, otherwise the second duplicate runs into the first one.
    // Row 1 has the key (1,2)
    QVERIFY(model.dittoRecord(1).isValid());
    QVERIFY(model.dittoRecord(1).isValid());
    closer.stop();
    QVERIFY2(!dialogShown, "an error dialog was shown while duplicating");

    QCOMPARE(tableRows(db, "quote(a)||','||quote(b)||','||quote(c)", "a,b"),
             QStringList() << "0,3,'y'" << "0,4,'y'" << "1,1,'x'" << "1,2,'y'" << "2,1,'z'");

    // The rows in the model have to be the ones in the database
    QCOMPARE(model.rowCount(), 5);
    QCOMPARE(model.data(model.index(3, 1), Qt::EditRole).toString(), QString("0"));
    QCOMPARE(model.data(model.index(3, 2), Qt::EditRole).toString(), QString("3"));
    QCOMPARE(model.data(model.index(3, 3), Qt::EditRole).toString(), QString("y"));
    QCOMPARE(model.data(model.index(4, 2), Qt::EditRole).toString(), QString("4"));
    QCOMPARE(model.data(model.index(4, 3), Qt::EditRole).toString(), QString("y"));
}

void TestDuplicateRecord::duplicateSkipsUniqueColumns()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    DBBrowserDB db;
    QVERIFY(db.create(dir.filePath("test.db")));
    QVERIFY(db.executeSQL("CREATE TABLE t(a INTEGER PRIMARY KEY, b TEXT UNIQUE, c TEXT, d TEXT, e TEXT, UNIQUE(c,d))"));
    QVERIFY(db.executeSQL("INSERT INTO t VALUES(1,'one','x','p','q'),(2,'two','y','r','s')"));
    db.updateSchema();

    SqliteTableModel model(db, nullptr, QString(), true);
    QSignalSpy rowCountSpy(&model, &SqliteTableModel::finishedRowCount);
    model.setQuery(sqlb::Query(sqlb::ObjectIdentifier("main", "t")));
    QVERIFY(rowCountSpy.wait(5000) || rowCountSpy.count() > 0);
    QVERIFY(model.completeCache());

    bool dialogShown = false;
    QTimer closer;
    connect(&closer, &QTimer::timeout, this, [&]() {
        if(QWidget* w = QApplication::activeModalWidget())
        {
            dialogShown = true;
            w->close();
        }
    });
    closer.start(50);

    // Columns covered by a UNIQUE constraint (b and c,d) are not copied, the others are
    const QModelIndex edit = model.dittoRecord(0);
    closer.stop();
    QVERIFY2(!dialogShown, "an error dialog was shown while duplicating");
    QVERIFY(edit.isValid());
    QCOMPARE(edit.row(), 2);

    QCOMPARE(tableRows(db, "quote(a)||','||quote(b)||','||quote(c)||','||quote(d)||','||quote(e)", "a"),
             QStringList() << "1,'one','x','p','q'" << "2,'two','y','r','s'" << "3,NULL,NULL,NULL,'q'");
}
