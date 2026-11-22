#include <QtTest/QTest>
#include <QApplication>
#include <QTemporaryDir>
#include <QFile>

#include "TestTableModel.h"
#include "../sqlitedb.h"
#include "../sqlitetablemodel.h"
#include "../Settings.h"
#include "../sql/ObjectIdentifier.h"

namespace
{
const int rowCount = 100;

// Column 0 of the model is the rowid column, so the 'val' column is column 2.
// Creates a table with rowCount rows and returns a model showing it. The prefetch size is set small enough that
// most rows are not loaded into the cache when the model is created.
struct Fixture
{
    QTemporaryDir dir;
    DBBrowserDB db;
    std::unique_ptr<SqliteTableModel> model;

    Fixture()
    {
        Settings::setValue("db", "prefetchsize", 10);
        // Open an empty file instead of using create(), which closes the database handle twice
        const QString filename = dir.filePath("test.db");
        QFile(filename).open(QIODevice::WriteOnly);
        db.open(filename);
        db.executeSQL("CREATE TABLE t (id INTEGER PRIMARY KEY, val TEXT);");
        db.executeSQL("WITH RECURSIVE c(x) AS (SELECT 1 UNION ALL SELECT x+1 FROM c WHERE x < " + std::to_string(rowCount) +
                      ") INSERT INTO t(id, val) SELECT x, 'v' || x FROM c;");
        model = std::make_unique<SqliteTableModel>(db, nullptr, QString(), true);
        model->setQuery(sqlb::Query(sqlb::ObjectIdentifier("main", "t")));
        model->waitUntilIdle();
        // The worker reports back through queued connections
        QTRY_VERIFY_WITH_TIMEOUT(model->rowCount() == rowCount, 5000);
        model->waitUntilIdle();
        QCoreApplication::processEvents();
    }
};
}

void TestTableModel::setDataOnCachedRow()
{
    Fixture f;
    QCOMPARE(f.model->rowCount(), rowCount);

    const QModelIndex idx = f.model->index(0, 2);
    QVERIFY(f.model->setData(idx, "changed"));
    QCOMPARE(f.model->data(idx, Qt::EditRole).toString(), QString("changed"));
}

void TestTableModel::setDataOnUncachedRowLoadsRowAndUpdates()
{
    Fixture f;
    QCOMPARE(f.model->rowCount(), rowCount);

    // Rows beyond the first chunk are valid model rows, but not in the cache yet. Editing them (e.g. by pasting a
    // large clipboard) used to throw std::out_of_range from the cache lookup and terminate the application.
    const QModelIndex idx = f.model->index(rowCount - 1, 2);
    try {
        QVERIFY(f.model->setData(idx, "changed"));
    } catch(const std::exception& e) {
        QFAIL(qPrintable(QString("setData threw: ") + e.what()));
    }
    QCOMPARE(f.model->data(idx, Qt::EditRole).toString(), QString("changed"));
}

namespace
{
QStringList capturedWarnings;

void captureWarnings(QtMsgType type, const QMessageLogContext&, const QString& msg)
{
    if(type == QtWarningMsg)
        capturedWarnings << msg;
}
}

void TestTableModel::createClosesTheHandleOnlyOnce()
{
    QTemporaryDir dir;
    DBBrowserDB db;

    // create() closes the handle it used to write the file header and opens the file again. Closing the stale handle a
    // second time is undefined behaviour: it is reported as a warning on some platforms and crashes on others.
    capturedWarnings.clear();
    const auto previous_handler = qInstallMessageHandler(captureWarnings);
    const bool created = db.create(dir.filePath("created.db"));
    qInstallMessageHandler(previous_handler);

    QVERIFY(created);
    QVERIFY(db.isOpen());
    QVERIFY2(capturedWarnings.isEmpty(), qPrintable(capturedWarnings.join("; ")));
    QVERIFY(db.executeSQL("CREATE TABLE t (id INTEGER);"));
}

int main(int argc, char* argv[])
{
    // The model needs a QApplication, but the tests must also run on machines without a display (e.g. CI)
    if(qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM"))
        qputenv("QT_QPA_PLATFORM", "offscreen");

    QApplication app(argc, argv);
    TestTableModel test;
    return QTest::qExec(&test, argc, argv);
}
