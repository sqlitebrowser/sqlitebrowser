#include "TestEditTableDialog.h"

#include "../EditTableDialog.h"
#include "../Settings.h"
#include "../sqlitedb.h"
#include "../sqltextedit.h"

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QLineEdit>
#include <QToolButton>
#include <QTemporaryDir>
#include <QTimer>
#include <QTreeWidget>
#include <QtTest/QtTest>

#include <memory>

// Showing a QMessageBox with the offscreen platform crashes in QMessageBox::showEvent() on Windows CI, so the
// tests which need the dialog to show a message box are only run on the other platforms
#ifdef Q_OS_WIN
#define SKIP_IF_MESSAGE_BOXES_CRASH() QSKIP("Message boxes can't be shown with the offscreen platform on Windows")
#else
#define SKIP_IF_MESSAGE_BOXES_CRASH() do {} while(false)
#endif

namespace
{

// Closes any message box which pops up while a dialog is waiting for the user's answer and counts them
class MessageBoxCloser : public QObject
{
public:
    explicit MessageBoxCloser(QMessageBox::StandardButton answer = QMessageBox::Ok)
        : m_answer(answer)
    {
        connect(&m_timer, &QTimer::timeout, this, [this]() {
            if(auto* box = qobject_cast<QMessageBox*>(QApplication::activeModalWidget()))
            {
                m_count++;
                if(QAbstractButton* button = box->button(m_answer))
                    button->click();
                else
                    box->done(QMessageBox::Cancel);
            }
        });
        m_timer.start(10);
    }

    int count() const { return m_count; }

private:
    QTimer m_timer;
    QMessageBox::StandardButton m_answer;
    int m_count = 0;
};

// Finds a column of the field list by the text of its header
int column(QTreeWidget* tree, const QString& header)
{
    for(int i = 0; i < tree->columnCount(); ++i)
    {
        if(tree->headerItem()->text(i) == header)
            return i;
    }
    return -1;
}

QTreeWidget* fieldTree(EditTableDialog& dialog)
{
    return dialog.findChild<QTreeWidget*>("treeWidget");
}

QTreeWidgetItem* fieldItem(EditTableDialog& dialog, const QString& name)
{
    auto items = fieldTree(dialog)->findItems(name, Qt::MatchExactly, column(fieldTree(dialog), "Name"));
    return items.size() == 1 ? items.front() : nullptr;
}

// Adds a new field using the dialog's add button and returns its list item
QTreeWidgetItem* addField(EditTableDialog& dialog, const QString& name)
{
    dialog.findChild<QToolButton*>("addFieldButton")->click();
    QTreeWidget* tree = fieldTree(dialog);
    QTreeWidgetItem* item = tree->topLevelItem(tree->topLevelItemCount() - 1);
    item->setText(column(tree, "Name"), name);
    return item;
}

void acceptDialog(EditTableDialog& dialog)
{
    // accept() is a private slot of the dialog, so go through the base class
    static_cast<QDialog&>(dialog).accept();
}

}


namespace
{
std::unique_ptr<QTemporaryDir> g_dir;
std::unique_ptr<DBBrowserDB> g_db;

// The SQL editor in the dialog shares a lexer which belongs to the first editor ever created. Keep that one alive for
// the whole test run because the editors in the dialogs are destroyed with the dialogs.
std::unique_ptr<SqlTextEdit> g_lexer_owner;

const sqlb::ObjectIdentifier tableId("main", "t");

void exec(const std::string& sql)
{
    QVERIFY2(g_db->executeSQL(sql), qPrintable(g_db->lastError()));
    g_db->updateSchema();
}

// Runs a query and returns each row as the values of its columns joined by '|'
QStringList rows(const std::string& sql)
{
    QStringList result;
    g_db->executeSQL(sql, false, true, [&result](int, std::vector<QByteArray> values, std::vector<QByteArray>) -> bool {
        QStringList row;
        for(const auto& v : values)
            row << QString::fromUtf8(v);
        result << row.join('|');
        return false;
    });
    return result;
}

const sqlb::Field& tableField(const std::string& field)
{
    static sqlb::Field none;
    auto table = g_db->getTableByName(tableId);
    auto it = sqlb::findField(*table, field);
    return it == table->fields.end() ? none : *it;
}
}

void TestEditTableDialog::initTestCase()
{
    // Keep the user's real settings out of the test
    static QTemporaryDir settings_dir;
    Settings::setUserSettingsFile(settings_dir.filePath("settings.ini"));

    g_lexer_owner = std::make_unique<SqlTextEdit>();
}

void TestEditTableDialog::cleanupTestCase()
{
    g_lexer_owner.reset();
}

void TestEditTableDialog::init()
{
    g_dir = std::make_unique<QTemporaryDir>();
    QVERIFY(g_dir->isValid());
    g_db = std::make_unique<DBBrowserDB>();

    // An empty file is a valid empty database
    QFile file(g_dir->filePath("test.db"));
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();
    QVERIFY(g_db->open(file.fileName()));
}

void TestEditTableDialog::cleanup()
{
    g_db.reset();
    g_dir.reset();
}

void TestEditTableDialog::showsGeneratedColumns()
{
    exec("CREATE TABLE t(a INTEGER, b INTEGER, v INTEGER GENERATED ALWAYS AS (a + b) VIRTUAL, s INTEGER AS (a * b) STORED);");

    EditTableDialog dialog(*g_db, tableId, false);
    QTreeWidget* tree = fieldTree(dialog);
    const int gen = column(tree, "Generated");
    const int stored = column(tree, "Stored");
    QVERIFY2(gen != -1, "The field list has no 'Generated' column");
    QVERIFY2(stored != -1, "The field list has no 'Stored' column");

    // Ordinary column: no expression
    QCOMPARE(fieldItem(dialog, "a")->text(gen), QString());
    QCOMPARE(fieldItem(dialog, "a")->checkState(stored), Qt::Unchecked);

    // Virtual column
    QCOMPARE(fieldItem(dialog, "v")->text(gen), QString::fromStdString(tableField("v").generated()->expression()));
    QVERIFY(!fieldItem(dialog, "v")->text(gen).isEmpty());
    QCOMPARE(fieldItem(dialog, "v")->checkState(stored), Qt::Unchecked);

    // Stored column
    QCOMPARE(fieldItem(dialog, "s")->text(gen), QString::fromStdString(tableField("s").generated()->expression()));
    QCOMPARE(fieldItem(dialog, "s")->checkState(stored), Qt::Checked);
}

void TestEditTableDialog::addVirtualGeneratedColumn()
{
    exec("CREATE TABLE t(a INTEGER, b INTEGER);");
    exec("INSERT INTO t VALUES (1, 2), (3, 4);");

    MessageBoxCloser boxes;
    {
        EditTableDialog dialog(*g_db, tableId, false);
        const int gen = column(fieldTree(dialog), "Generated");
        QVERIFY(gen != -1);

        QTreeWidgetItem* item = addField(dialog, "total");
        item->setText(gen, "a + b");
        acceptDialog(dialog);
        QCOMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
    }

    QCOMPARE(boxes.count(), 0);
    QVERIFY(tableField("total").generated());
    QCOMPARE(tableField("total").generated()->storage(), "VIRTUAL");
    QCOMPARE(rows("SELECT a, b, total FROM t ORDER BY a;"), (QStringList{"1|2|3", "3|4|7"}));
}

void TestEditTableDialog::addStoredGeneratedColumn()
{
    // SQLite can't add a STORED column to an existing table with ALTER TABLE ADD COLUMN, so the table has to be rebuilt
    exec("CREATE TABLE t(a INTEGER, b INTEGER);");
    exec("INSERT INTO t VALUES (1, 2), (3, 4);");

    MessageBoxCloser boxes;
    {
        EditTableDialog dialog(*g_db, tableId, false);
        const int gen = column(fieldTree(dialog), "Generated");
        const int stored = column(fieldTree(dialog), "Stored");
        QVERIFY(gen != -1 && stored != -1);

        QTreeWidgetItem* item = addField(dialog, "prod");
        item->setCheckState(stored, Qt::Checked);   // Choosing the storage first must work, too
        item->setText(gen, "a * b");
        acceptDialog(dialog);
        QCOMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
    }

    QCOMPARE(boxes.count(), 0);
    QVERIFY(tableField("prod").generated());
    QCOMPARE(tableField("prod").generated()->storage(), "STORED");
    QCOMPARE(rows("SELECT a, b, prod FROM t ORDER BY a;"), (QStringList{"1|2|2", "3|4|12"}));
}

void TestEditTableDialog::changeGeneratedExpression()
{
    exec("CREATE TABLE t(a INTEGER, b INTEGER, c INTEGER AS (a + b));");
    exec("INSERT INTO t(a, b) VALUES (5, 2), (7, 3);");

    MessageBoxCloser boxes;
    {
        EditTableDialog dialog(*g_db, tableId, false);
        const int gen = column(fieldTree(dialog), "Generated");
        QVERIFY(gen != -1);

        fieldItem(dialog, "c")->setText(gen, "a - b");
        acceptDialog(dialog);
        QCOMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
    }

    QCOMPARE(boxes.count(), 0);
    QCOMPARE(rows("SELECT c FROM t ORDER BY a;"), (QStringList{"3", "4"}));
}

void TestEditTableDialog::switchBetweenVirtualAndStored()
{
    exec("CREATE TABLE t(a INTEGER, b INTEGER, c INTEGER AS (a + b));");
    exec("INSERT INTO t(a, b) VALUES (5, 2);");

    MessageBoxCloser boxes;
    {
        EditTableDialog dialog(*g_db, tableId, false);
        const int stored = column(fieldTree(dialog), "Stored");
        QVERIFY(stored != -1);

        fieldItem(dialog, "c")->setCheckState(stored, Qt::Checked);
        acceptDialog(dialog);
        QCOMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
    }
    QCOMPARE(tableField("c").generated()->storage(), "STORED");
    QCOMPARE(rows("SELECT c FROM t;"), (QStringList{"7"}));

    {
        EditTableDialog dialog(*g_db, tableId, false);
        const int stored = column(fieldTree(dialog), "Stored");
        QVERIFY(stored != -1);

        fieldItem(dialog, "c")->setCheckState(stored, Qt::Unchecked);
        acceptDialog(dialog);
        QCOMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
    }
    QCOMPARE(tableField("c").generated()->storage(), "VIRTUAL");
    QCOMPARE(rows("SELECT c FROM t;"), (QStringList{"7"}));
    QCOMPARE(boxes.count(), 0);
}

void TestEditTableDialog::convertOrdinaryColumnToGenerated()
{
    SKIP_IF_MESSAGE_BOXES_CRASH();
    exec("CREATE TABLE t(a INTEGER, b INTEGER, c INTEGER);");
    exec("INSERT INTO t VALUES (1, 2, 99);");

    // Making an existing column a generated one replaces its contents, so the user is asked first. Saying no keeps it as it is.
    {
        MessageBoxCloser boxes(QMessageBox::No);
        EditTableDialog dialog(*g_db, tableId, false);
        const int gen = column(fieldTree(dialog), "Generated");
        QVERIFY(gen != -1);

        fieldItem(dialog, "c")->setText(gen, "a + b");
        QCOMPARE(boxes.count(), 1);
        QCOMPARE(fieldItem(dialog, "c")->text(gen), QString());
        acceptDialog(dialog);
    }
    QVERIFY(!tableField("c").generated());
    QCOMPARE(rows("SELECT c FROM t;"), (QStringList{"99"}));

    {
        MessageBoxCloser boxes(QMessageBox::Yes);
        {
            EditTableDialog dialog(*g_db, tableId, false);
            const int gen = column(fieldTree(dialog), "Generated");
            QVERIFY(gen != -1);

            fieldItem(dialog, "c")->setText(gen, "a + b");
            acceptDialog(dialog);
            QCOMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
        }
        QCOMPARE(boxes.count(), 1);
    }
    QVERIFY(tableField("c").generated());
    QCOMPARE(rows("SELECT c FROM t;"), (QStringList{"3"}));
}

void TestEditTableDialog::convertGeneratedColumnToOrdinary()
{
    exec("CREATE TABLE t(a INTEGER, b INTEGER, c INTEGER AS (a + b) STORED);");
    exec("INSERT INTO t(a, b) VALUES (1, 2);");

    MessageBoxCloser boxes;
    {
        EditTableDialog dialog(*g_db, tableId, false);
        const int gen = column(fieldTree(dialog), "Generated");
        const int stored = column(fieldTree(dialog), "Stored");
        QVERIFY(gen != -1 && stored != -1);

        fieldItem(dialog, "c")->setText(gen, "");
        QCOMPARE(fieldItem(dialog, "c")->checkState(stored), Qt::Unchecked);
        acceptDialog(dialog);
        QCOMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
    }

    QCOMPARE(boxes.count(), 0);
    QVERIFY(!tableField("c").generated());
    QCOMPARE(rows("SELECT c FROM t;"), (QStringList{"3"}));
}

void TestEditTableDialog::createTableWithGeneratedColumn()
{
    MessageBoxCloser boxes;
    {
        EditTableDialog dialog(*g_db, sqlb::ObjectIdentifier(), true);
        const int gen = column(fieldTree(dialog), "Generated");
        QVERIFY(gen != -1);

        dialog.findChild<QLineEdit*>("editTableName")->setText("t");
        addField(dialog, "a");
        addField(dialog, "double_a")->setText(gen, "a * 2");
        acceptDialog(dialog);
        QCOMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
    }

    QCOMPARE(boxes.count(), 0);
    QVERIFY(tableField("double_a").generated());
    exec("INSERT INTO t(a) VALUES (4);");
    QCOMPARE(rows("SELECT double_a FROM t;"), (QStringList{"8"}));
}

void TestEditTableDialog::generatedColumnCannotBePrimaryKey()
{
    SKIP_IF_MESSAGE_BOXES_CRASH();
    exec("CREATE TABLE t(a INTEGER PRIMARY KEY, b INTEGER, c INTEGER AS (b + 1));");

    MessageBoxCloser boxes;
    EditTableDialog dialog(*g_db, tableId, false);
    QTreeWidget* tree = fieldTree(dialog);
    const int gen = column(tree, "Generated");
    const int pk = column(tree, "PK");
    const int ai = column(tree, "AI");
    QVERIFY(gen != -1);

    // A primary key column can't be made a generated column
    fieldItem(dialog, "a")->setText(gen, "b + 1");
    QCOMPARE(boxes.count(), 1);
    QCOMPARE(fieldItem(dialog, "a")->text(gen), QString());

    // A generated column can't be made part of the primary key
    fieldItem(dialog, "c")->setCheckState(pk, Qt::Checked);
    QCOMPARE(boxes.count(), 2);
    QCOMPARE(fieldItem(dialog, "c")->checkState(pk), Qt::Unchecked);

    fieldItem(dialog, "c")->setCheckState(ai, Qt::Checked);
    QCOMPARE(boxes.count(), 3);
    QCOMPARE(fieldItem(dialog, "c")->checkState(ai), Qt::Unchecked);
    QCOMPARE(fieldItem(dialog, "c")->checkState(pk), Qt::Unchecked);
    QCOMPARE(fieldItem(dialog, "a")->checkState(pk), Qt::Checked);

    // None of this changed the table
    acceptDialog(dialog);
    QCOMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
    QCOMPARE(boxes.count(), 3);
    QVERIFY(!tableField("a").generated());
    QVERIFY(tableField("c").generated());
    QVERIFY(g_db->getTableByName(tableId)->primaryKey());
}

void TestEditTableDialog::generatedColumnCannotHaveDefault()
{
    SKIP_IF_MESSAGE_BOXES_CRASH();
    exec("CREATE TABLE t(a INTEGER, b INTEGER DEFAULT 5, c INTEGER AS (a + 1));");

    MessageBoxCloser boxes;
    EditTableDialog dialog(*g_db, tableId, false);
    QTreeWidget* tree = fieldTree(dialog);
    const int gen = column(tree, "Generated");
    const int def = column(tree, "Default");
    QVERIFY(gen != -1);

    // A column with a default value can't be made a generated column
    fieldItem(dialog, "b")->setText(gen, "a + 1");
    QCOMPARE(boxes.count(), 1);
    QCOMPARE(fieldItem(dialog, "b")->text(gen), QString());
    QCOMPARE(fieldItem(dialog, "b")->text(def), QString("5"));

    // A generated column can't get a default value
    fieldItem(dialog, "c")->setText(def, "7");
    QCOMPARE(boxes.count(), 2);
    QCOMPARE(fieldItem(dialog, "c")->text(def), QString());

    // None of this changed the table
    acceptDialog(dialog);
    QCOMPARE(dialog.result(), static_cast<int>(QDialog::Accepted));
    QCOMPARE(boxes.count(), 2);
    QVERIFY(!tableField("b").generated());
    QCOMPARE(tableField("b").defaultValue(), "5");
    QVERIFY(tableField("c").generated());
    QCOMPARE(tableField("c").defaultValue(), "");
}

int main(int argc, char* argv[])
{
    // The dialog needs a QApplication but the tests must also run without a display
    qputenv("QT_QPA_PLATFORM", "offscreen");

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("sqlitebrowser-test");
    QCoreApplication::setApplicationName("test-edit-table");

    TestEditTableDialog test;
    return QTest::qExec(&test, argc, argv);
}
