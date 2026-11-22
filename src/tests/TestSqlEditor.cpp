#include "TestSqlEditor.h"

#include "../Settings.h"
#include "../docktextedit.h"
#include "../sqltextedit.h"

#include <QApplication>
#include <QKeyEvent>
#include <QtTest/QtTest>

QTEST_MAIN(TestSqlEditor)

namespace
{

// Zoom level of the editor in points relative to its normal font size
int zoomLevel(SqlTextEdit& edit)
{
    return static_cast<int>(edit.SendScintilla(QsciScintillaBase::SCI_GETZOOM));
}

// Shortcuts constrained to the widget only fire when the editor has the focus in the active window
bool showWithFocus(SqlTextEdit& edit)
{
    edit.resize(400, 300);
    edit.show();
    if(!QTest::qWaitForWindowActive(&edit))
        return false;
    edit.setFocus();
    return edit.hasFocus();
}

// A point in the line number margin on the given line
QPoint marginPoint(SqlTextEdit& edit, int line)
{
    const long lineStart = edit.SendScintilla(QsciScintillaBase::SCI_POSITIONFROMLINE, static_cast<unsigned long>(line));
    const int top = static_cast<int>(edit.SendScintilla(QsciScintillaBase::SCI_POINTYFROMPOSITION, 0UL, lineStart));
    const int height = static_cast<int>(edit.SendScintilla(QsciScintillaBase::SCI_TEXTHEIGHT, static_cast<unsigned long>(line)));
    return QPoint(edit.marginWidth(0) / 2, top + height / 2);
}

void enableMarginClick(SqlTextEdit& edit, bool enable)
{
    Settings::setValue("editor", "toggle_comment_on_margin_click", enable, /* save_to_disk */ false);
    edit.reloadSettings();
}

}

void TestSqlEditor::initTestCase()
{
    // Never read or write the settings of the real user
    QVERIFY(settingsDir.isValid());
    Settings::setUserSettingsFile(settingsDir.filePath("settings.ini"));

    lexerOwner = new SqlTextEdit();
}

void TestSqlEditor::cleanup()
{
    Settings::setValue("editor", "toggle_comment_on_margin_click", false, /* save_to_disk */ false);
}

void TestSqlEditor::cleanupTestCase()
{
    delete lexerOwner;
    lexerOwner = nullptr;
}

void TestSqlEditor::zoomKeys_data()
{
    QTest::addColumn<int>("key");
    QTest::addColumn<int>("modifiers");
    QTest::addColumn<int>("startZoom");
    QTest::addColumn<int>("expectedZoom");

    const int ctrl = Qt::ControlModifier;
    const int shift = Qt::ShiftModifier;
    const int keypad = Qt::KeypadModifier;

    // The bindings QScintilla provides
    QTest::newRow("Ctrl++ zooms in")            << int(Qt::Key_Plus)  << ctrl                              << 0 << 1;
    QTest::newRow("Ctrl+- zooms out")           << int(Qt::Key_Minus) << ctrl                              << 0 << -1;
    QTest::newRow("Ctrl+keypad+ zooms in")      << int(Qt::Key_Plus)  << (ctrl | keypad)       << 0 << 1;
    QTest::newRow("Ctrl+keypad- zooms out")     << int(Qt::Key_Minus) << (ctrl | keypad)       << 0 << -1;

    // On keyboards where + needs Shift (US: Shift+=, Japanese: Shift+;) the plain Ctrl++ never arrives
    QTest::newRow("Ctrl+Shift++ zooms in")      << int(Qt::Key_Plus)  << (ctrl | shift)                    << 0 << 1;
    QTest::newRow("Ctrl+= zooms in")            << int(Qt::Key_Equal) << ctrl                              << 0 << 1;

    // Back to the normal size
    QTest::newRow("Ctrl+Shift+0 resets from bigger")  << int(Qt::Key_0) << (ctrl | shift) << 4  << 0;
    QTest::newRow("Ctrl+Shift+0 resets from smaller") << int(Qt::Key_0) << (ctrl | shift) << -3 << 0;
    QTest::newRow("Ctrl+Shift+0 keeps the normal size") << int(Qt::Key_0) << (ctrl | shift) << 0 << 0;
}

void TestSqlEditor::zoomKeys()
{
    QFETCH(int, key);
    QFETCH(int, modifiers);
    QFETCH(int, startZoom);
    QFETCH(int, expectedZoom);

    SqlTextEdit edit;
    QVERIFY(showWithFocus(edit));
    edit.zoomTo(startZoom);
    QCOMPARE(zoomLevel(edit), startZoom);

    QTest::keyClick(&edit, static_cast<Qt::Key>(key), static_cast<Qt::KeyboardModifiers>(modifiers));

    QCOMPARE(zoomLevel(edit), expectedZoom);
}

void TestSqlEditor::ctrlZeroStaysWithTheApplication()
{
    // Ctrl+0 is the shortcut of View > Window Layout > Reset Window Layout in the main window.
    // The editor must not claim it, otherwise that shortcut would stop working while typing SQL.
    SqlTextEdit edit;
    QVERIFY(showWithFocus(edit));

    QKeyEvent override(QEvent::ShortcutOverride, Qt::Key_0, Qt::ControlModifier);
    override.ignore();
    QApplication::sendEvent(&edit, &override);
    QVERIFY(!override.isAccepted());

    edit.zoomTo(3);
    QTest::keyClick(&edit, Qt::Key_0, Qt::ControlModifier);
    QCOMPARE(zoomLevel(edit), 3);
}

void TestSqlEditor::marginClickIsOffByDefault()
{
    // This must run before any other test changes the setting
    QVERIFY(!Settings::getValue("editor", "toggle_comment_on_margin_click").toBool());
}

void TestSqlEditor::toggleBlockCommentUsesSelection_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("lineFrom");
    QTest::addColumn<int>("indexFrom");
    QTest::addColumn<int>("lineTo");
    QTest::addColumn<int>("indexTo");
    QTest::addColumn<QString>("expected");

    // No selection: the line of the cursor (this is the position the cursor is moved to)
    QTest::newRow("cursor line") << "a\nb\nc" << 1 << 1 << 1 << 1 << "a\n-- b\nc";
    // A selection ending at the start of a line doesn't include that line
    QTest::newRow("selection up to the start of a line") << "a\nb\nc\nd" << 0 << 0 << 2 << 0 << "-- a\n-- b\nc\nd";
    QTest::newRow("selection inside lines") << "a\nb\nc\nd" << 1 << 1 << 2 << 1 << "a\n-- b\n-- c\nd";
}

void TestSqlEditor::toggleBlockCommentUsesSelection()
{
    QFETCH(QString, text);
    QFETCH(int, lineFrom);
    QFETCH(int, indexFrom);
    QFETCH(int, lineTo);
    QFETCH(int, indexTo);
    QFETCH(QString, expected);

    SqlTextEdit edit;
    edit.setText(text);
    if(lineFrom == lineTo && indexFrom == indexTo)
        edit.setCursorPosition(lineFrom, indexFrom);
    else
        edit.setSelection(lineFrom, indexFrom, lineTo, indexTo);

    edit.toggleBlockComment();

    QCOMPARE(edit.text(), expected);
}

void TestSqlEditor::toggleLineComments_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("lineFrom");
    QTest::addColumn<int>("lineTo");
    QTest::addColumn<QString>("expected");

    QTest::newRow("comment one line")           << "a\nb\nc"      << 1 << 1 << "a\n-- b\nc";
    QTest::newRow("uncomment one line")         << "a\n-- b\nc"   << 1 << 1 << "a\nb\nc";
    QTest::newRow("uncomment without space")    << "a\n--b\nc"    << 1 << 1 << "a\nb\nc";
    QTest::newRow("uncomment keeps indentation") << "a\n  -- b\nc" << 1 << 1 << "a\n  b\nc";
    QTest::newRow("comment a range")            << "a\nb\nc\nd"  << 1 << 2 << "a\n-- b\n-- c\nd";
    QTest::newRow("uncomment a range")          << "-- a\n-- b\n-- c" << 0 << 1 << "a\nb\n-- c";
    QTest::newRow("first line decides: comment") << "a\n-- b"    << 0 << 1 << "-- a\n-- -- b";
    QTest::newRow("first line decides: uncomment") << "-- a\nb\n-- c" << 0 << 2 << "a\nb\nc";
    QTest::newRow("last line without line break") << "a\nb"      << 1 << 1 << "a\n-- b";
    QTest::newRow("Windows line breaks")        << "a\r\nb\r\nc" << 0 << 1 << "-- a\r\n-- b\r\nc";
    QTest::newRow("whole text")                 << "a\nb"         << 0 << 1 << "-- a\n-- b";
    QTest::newRow("last line past the end")     << "a\nb"         << 1 << 7 << "a\n-- b";
    QTest::newRow("first line before the start") << "a\nb"        << -3 << 0 << "-- a\nb";
    QTest::newRow("empty range")                << "a\nb"         << 1 << 0 << "a\nb";
    QTest::newRow("range outside the text")     << "a\nb"         << 5 << 6 << "a\nb";
}

void TestSqlEditor::toggleLineComments()
{
    QFETCH(QString, text);
    QFETCH(int, lineFrom);
    QFETCH(int, lineTo);
    QFETCH(QString, expected);

    SqlTextEdit edit;
    edit.setText(text);

    edit.toggleLineComments(lineFrom, lineTo);

    QCOMPARE(edit.text(), expected);
}

void TestSqlEditor::toggleLineCommentsIsOneUndoStep()
{
    SqlTextEdit edit;
    edit.setText("a\nb\nc");

    edit.toggleLineComments(0, 2);
    QCOMPARE(edit.text(), QString("-- a\n-- b\n-- c"));

    edit.undo();
    QCOMPARE(edit.text(), QString("a\nb\nc"));
}

void TestSqlEditor::marginClickSelectsLineByDefault()
{
    // What a click on a line number does today: it selects that line and doesn't touch the text
    SqlTextEdit edit;
    edit.setText("a\nb\nc");
    QVERIFY(showWithFocus(edit));

    QTest::mouseClick(edit.viewport(), Qt::LeftButton, Qt::NoModifier, marginPoint(edit, 1));

    QCOMPARE(edit.text(), QString("a\nb\nc"));
    QVERIFY(edit.hasSelectedText());
    QVERIFY(edit.selectedText().startsWith("b"));
}

void TestSqlEditor::marginClickTogglesComment()
{
    SqlTextEdit edit;
    edit.setText("a\nb\nc");
    QVERIFY(showWithFocus(edit));
    enableMarginClick(edit, true);

    QTest::mouseClick(edit.viewport(), Qt::LeftButton, Qt::NoModifier, marginPoint(edit, 1));
    QCOMPARE(edit.text(), QString("a\n-- b\nc"));

    QTest::mouseClick(edit.viewport(), Qt::LeftButton, Qt::NoModifier, marginPoint(edit, 1));
    QCOMPARE(edit.text(), QString("a\nb\nc"));

    // Other lines are not affected
    QTest::mouseClick(edit.viewport(), Qt::LeftButton, Qt::NoModifier, marginPoint(edit, 2));
    QCOMPARE(edit.text(), QString("a\nb\n-- c"));

    // Turning it off again brings the old behaviour back
    enableMarginClick(edit, false);
    QTest::mouseClick(edit.viewport(), Qt::LeftButton, Qt::NoModifier, marginPoint(edit, 0));
    QCOMPARE(edit.text(), QString("a\nb\n-- c"));
    QVERIFY(edit.hasSelectedText());
}

void TestSqlEditor::marginClickOutsideMarginDoesNotToggle()
{
    SqlTextEdit edit;
    edit.setText("a\nb\nc");
    QVERIFY(showWithFocus(edit));
    enableMarginClick(edit, true);

    QPoint inText = marginPoint(edit, 1);
    inText.setX(edit.marginWidth(0) + edit.marginWidth(1) + edit.marginWidth(2) + 40);
    QTest::mouseClick(edit.viewport(), Qt::LeftButton, Qt::NoModifier, inText);

    QCOMPARE(edit.text(), QString("a\nb\nc"));
}

void TestSqlEditor::marginClickSelectsLineInReadOnlyEditor()
{
    // Nothing to comment in an editor that can't be changed, so clicking the line numbers must keep selecting the line there
    SqlTextEdit edit;
    edit.setText("a\nb\nc");
    edit.setReadOnly(true);
    QVERIFY(showWithFocus(edit));
    enableMarginClick(edit, true);

    QTest::mouseClick(edit.viewport(), Qt::LeftButton, Qt::NoModifier, marginPoint(edit, 1));

    QCOMPARE(edit.text(), QString("a\nb\nc"));
    QVERIFY(edit.hasSelectedText());
    QVERIFY(edit.selectedText().startsWith("b"));

    // ... but works as soon as the editor can be changed
    edit.setReadOnly(false);
    QTest::mouseClick(edit.viewport(), Qt::LeftButton, Qt::NoModifier, marginPoint(edit, 1));

    QCOMPARE(edit.text(), QString("a\n-- b\nc"));
}

void TestSqlEditor::marginClickSelectsLineInCellEditor()
{
    // The editor for the cell content is based on SqlTextEdit but doesn't hold SQL code
    DockTextEdit edit;
    edit.setText("a\nb\nc");
    QVERIFY(showWithFocus(edit));
    enableMarginClick(edit, true);

    QTest::mouseClick(edit.viewport(), Qt::LeftButton, Qt::NoModifier, marginPoint(edit, 1));

    QCOMPARE(edit.text(), QString("a\nb\nc"));
    QVERIFY(edit.hasSelectedText());
    QVERIFY(edit.selectedText().startsWith("b"));
}
