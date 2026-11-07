#include <QtTest/QTest>
#include <QApplication>
#include <QComboBox>
#include <QLabel>
#include <QToolButton>
#include <Qsci/qsciscintilla.h>
#include <sqlite3.h>

#include "TestEditDialog.h"
#include "../EditDialog.h"
#include "../docktextedit.h"

namespace
{
const int TextEditorMode = 0;
const int JsonEditorMode = 4;

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

// Shows data in the given editor mode like selecting a cell does
void showData(EditDialog& dialog, int mode, const QByteArray& data)
{
    dialog.findChild<QComboBox*>("comboMode")->setCurrentIndex(mode);
    QMetaObject::invokeMethod(&dialog, "loadData", Qt::DirectConnection, Q_ARG(QByteArray, data));
    QMetaObject::invokeMethod(&dialog, "updateCellInfoAndMode", Qt::DirectConnection, Q_ARG(QByteArray, data));
}
}

void TestEditDialog::initTestCase()
{
    // One dialog for all tests: the SQL editors share a lexer which is owned by the first editor that is created
    dialog = new EditDialog();
    dialog->setReadOnly(false);

    // Stay in the mode the test selects, the automatic mode would switch to the hex editor for binary data
    dialog->findChild<QToolButton*>("buttonAutoSwitchMode")->setChecked(false);
}

void TestEditDialog::cleanupTestCase()
{
    delete dialog;
}

void TestEditDialog::jsonbIsShownAsReadOnlyJson()
{
    if(sqlite3_libversion_number() < 3045000)
        QSKIP("JSONB needs SQLite 3.45.0 or newer");

    auto* editor = dialog->findChild<DockTextEdit*>();
    QVERIFY(editor);

    showData(*dialog, TextEditorMode, makeJsonb("{\"a\":[1,2]}"));
    QCOMPARE(editor->text(), QString("{\"a\":[1,2]}"));
    QVERIFY(editor->isReadOnly());
    QVERIFY(editor->isEnabled());
    QVERIFY(dialog->findChild<QLabel*>("labelInfo")->text().contains("JSONB"));

    // The JSON mode shows it formatted, if that is enabled in the settings (the default)
    showData(*dialog, JsonEditorMode, makeJsonb("{\"a\":[1,2]}"));
    QVERIFY(editor->text().contains("\"a\""));
    QVERIFY(editor->text().contains("1"));
    QVERIFY(editor->isReadOnly());
}

void TestEditDialog::editorBecomesWritableAgainAfterJsonb()
{
    if(sqlite3_libversion_number() < 3045000)
        QSKIP("JSONB needs SQLite 3.45.0 or newer");

    auto* editor = dialog->findChild<DockTextEdit*>();
    QVERIFY(editor);

    showData(*dialog, TextEditorMode, makeJsonb("[1,2]"));
    QVERIFY(editor->isReadOnly());

    showData(*dialog, TextEditorMode, "some text");
    QCOMPARE(editor->text(), QString("some text"));
    QVERIFY(!editor->isReadOnly());
}

void TestEditDialog::otherBinaryDataIsNotShownAsText()
{
    auto* editor = dialog->findChild<DockTextEdit*>();
    QVERIFY(editor);

    showData(*dialog, TextEditorMode, QByteArray("\x00\x01\x02\xff", 4));
    QVERIFY(editor->text().contains("Binary data can't be viewed"));
    QVERIFY(!editor->isEnabled());
    QVERIFY(!dialog->findChild<QLabel*>("labelInfo")->text().contains("JSONB"));
}

int main(int argc, char* argv[])
{
    // The dialog needs a QApplication, but the tests must also run on machines without a display (e.g. CI)
    if(qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM"))
        qputenv("QT_QPA_PLATFORM", "offscreen");

    QApplication app(argc, argv);
    TestEditDialog test;
    return QTest::qExec(&test, argc, argv);
}
