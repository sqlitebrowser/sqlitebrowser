#include "TestPaste.h"

#include "../ExtendedTableWidget.h"
#include "../sqlitedb.h"
#include "../sqlitetablemodel.h"
#include "../sql/ObjectIdentifier.h"
#include "../sql/Query.h"

#include <QApplication>
#include <QFile>
#include <QItemSelectionModel>
#include <QTemporaryDir>
#include <QtTest/QtTest>

QTEST_MAIN(TestPaste)

namespace
{

// The table has the columns id, a, b and n. Rows 1 and 2 hold the data to copy, the other rows are empty and used as the destination.
// Column 0 of the model is the rowid, so the model columns of a, b and n are 2, 3 and 4. The rows of the model start at 0.
const int columnA = 2;
const int columnB = 3;
const int columnN = 4;

struct Fixture
{
    QTemporaryDir dir;
    DBBrowserDB db;
    SqliteTableModel model;
    ExtendedTableWidget view;

    Fixture()
        : model(db, nullptr, QString(), true)
    {
        // Open an empty file instead of using create(), which closes the database handle twice
        const QString filename = dir.filePath("test.db");
        QVERIFY(QFile(filename).open(QIODevice::WriteOnly));
        QVERIFY(db.open(filename));
        // The first row holds binary data which is not valid UTF-8, the second one text and a number
        QVERIFY(db.executeSQL("CREATE TABLE t(id INTEGER PRIMARY KEY, a BLOB, b BLOB, n INTEGER);"
                              "INSERT INTO t VALUES (1, X'DEADBEEF', X'CAFEBABE', 1), (2, 'one', 'two', 7),"
                              "(3, '', '', NULL), (4, '', '', NULL);", false));

        model.setQuery(sqlb::Query(sqlb::ObjectIdentifier("main", "t")));
        model.waitUntilIdle();
        // The worker reports back through queued connections
        QTRY_VERIFY_WITH_TIMEOUT(model.rowCount() == 4, 5000);
        model.waitUntilIdle();
        QCoreApplication::processEvents();

        view.setModel(&model);
    }

    void select(int firstRow, int firstColumn, int lastRow, int lastColumn)
    {
        view.selectionModel()->select(QItemSelection(model.index(firstRow, firstColumn), model.index(lastRow, lastColumn)),
                                      QItemSelectionModel::ClearAndSelect);
    }

    // Use the keyboard shortcuts like the user does
    void copy(int firstRow, int firstColumn, int lastRow, int lastColumn)
    {
        select(firstRow, firstColumn, lastRow, lastColumn);
        QTest::keyClick(&view, Qt::Key_C, Qt::ControlModifier);
    }
    void paste(int firstRow, int firstColumn, int lastRow, int lastColumn)
    {
        select(firstRow, firstColumn, lastRow, lastColumn);
        QTest::keyClick(&view, Qt::Key_V, Qt::ControlModifier);
    }

    // The value of the column in the row with the given id, together with its SQLite type, e.g. "blob:DEADBEEF"
    QString cell(int id, const QString& column)
    {
        const QString sql = QString("SELECT typeof(%1) || ':' || hex(%1) FROM t WHERE id = %2").arg(column).arg(id);
        return QString::fromUtf8(db.querySingleValueFromDb(sql.toStdString(), false, DBBrowserDB::Wait));
    }
};

QString blob(const QByteArray& data) { return "blob:" + QString::fromLatin1(data.toHex().toUpper()); }
QString text(const QByteArray& data) { return "text:" + QString::fromLatin1(data.toHex().toUpper()); }

}

void TestPaste::pasteBlobIntoSingleCell()
{
    Fixture f;

    // Binary data must stay binary data. It has to be a BLOB in the destination, even if the destination holds text.
    f.copy(0, columnA, 0, columnA);
    f.paste(1, columnA, 1, columnA);

    QCOMPARE(f.cell(2, "a"), blob("\xDE\xAD\xBE\xEF"));
    // The cells which were not touched keep their values
    QCOMPARE(f.cell(2, "b"), text("two"));
    QCOMPARE(f.cell(1, "a"), blob("\xDE\xAD\xBE\xEF"));
}

void TestPaste::pasteBlobsIntoSeveralCells()
{
    Fixture f;

    f.copy(0, columnA, 0, columnB);
    f.paste(2, columnA, 2, columnB);

    QCOMPARE(f.cell(3, "a"), blob("\xDE\xAD\xBE\xEF"));
    QCOMPARE(f.cell(3, "b"), blob("\xCA\xFE\xBA\xBE"));
}

void TestPaste::pasteBlobsAndTextIntoSeveralRows()
{
    Fixture f;

    f.copy(0, columnA, 1, columnB);
    f.paste(2, columnA, 3, columnB);

    QCOMPARE(f.cell(3, "a"), blob("\xDE\xAD\xBE\xEF"));
    QCOMPARE(f.cell(3, "b"), blob("\xCA\xFE\xBA\xBE"));
    QCOMPARE(f.cell(4, "a"), text("one"));
    QCOMPARE(f.cell(4, "b"), text("two"));
}

void TestPaste::pasteTextIntoSeveralCells()
{
    Fixture f;

    f.copy(1, columnA, 1, columnB);
    f.paste(2, columnA, 2, columnB);

    QCOMPARE(f.cell(3, "a"), text("one"));
    QCOMPARE(f.cell(3, "b"), text("two"));
}

void TestPaste::pasteNumberIntoIntegerColumn()
{
    Fixture f;

    // Pasting text still lets the column affinity decide about the type
    f.copy(1, columnA, 1, columnN);
    f.paste(2, columnA, 2, columnN);

    QCOMPARE(f.cell(3, "a"), text("one"));
    QCOMPARE(f.cell(3, "n"), QString("integer:37"));   // hex() of the number 7 is the hex of the text "7"
}
