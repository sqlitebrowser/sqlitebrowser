#include "TestActionShortcuts.h"

#include <QFile>
#include <QApplication>
#include <QKeySequence>
#include <QMap>
#include <QXmlStreamReader>
#include <QtTest/QtTest>

QTEST_MAIN(TestActionShortcuts)

namespace
{

struct ActionShortcut
{
    QString file;
    QString action;
    QKeySequence shortcut;
    QString context;
};

// Read the shortcuts of all actions defined in one of the .ui files
QList<ActionShortcut> readShortcuts(const QString& file_name)
{
    QFile file(QString(SOURCE_DIR) + "/" + file_name);
    if(!file.open(QFile::ReadOnly))
        qFatal("Could not open %s", qPrintable(file.fileName()));

    QList<ActionShortcut> result;
    QXmlStreamReader xml(&file);
    QString action;
    QString property;
    while(!xml.atEnd())
    {
        xml.readNext();
        if(xml.isStartElement())
        {
            if(xml.name() == QLatin1String("action"))
                action = xml.attributes().value("name").toString();
            else if(xml.name() == QLatin1String("property"))
                property = xml.attributes().value("name").toString();
            else if(!action.isEmpty() && property == "shortcut" && xml.name() == QLatin1String("string"))
                result.push_back({file_name, action, QKeySequence(xml.readElementText()), "Qt::WindowShortcut"});
            else if(!action.isEmpty() && property == "shortcutContext" && xml.name() == QLatin1String("enum") && !result.isEmpty() && result.last().action == action)
                result.last().context = xml.readElementText();
        } else if(xml.isEndElement() && xml.name() == QLatin1String("action")) {
            action.clear();
            property.clear();
        }
    }
    if(xml.hasError())
        qFatal("Could not parse %s: %s", qPrintable(file.fileName()), qPrintable(xml.errorString()));

    return result;
}

QList<ActionShortcut> allShortcuts()
{
    return readShortcuts("MainWindow.ui") + readShortcuts("TableBrowser.ui");
}

// Shortcuts which are not defined in the .ui files but in the code. Only the ones which are available in the same places as the actions
// above are listed here. Keep this list in sync when adding shortcuts to MainWindow.cpp and TableBrowser.cpp.
const QStringList codeShortcuts = {
    "Ctrl+R", "Ctrl+Enter", "F5", "Ctrl+W", "Ctrl+Tab", "Ctrl+Shift+Tab",
    "Ctrl+L", "Ctrl+D", "Ctrl+I", "Ctrl+E", "Ctrl+0", "Alt+Shift+0", "Alt+Shift+W", "Ctrl+Shift+F4",
    "Ctrl+\"", "Ctrl+F", "Ctrl+H", "Ctrl+Shift+F", "F3", "Shift+F3", "Ctrl+Shift+C", "Ctrl+Alt+C", "Alt+Del", "Shift+Space",
};

}

void TestActionShortcuts::recordActionsHaveShortcuts_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("action");

    QTest::newRow("New Record") << "TableBrowser.ui" << "actionNewRecord";
    QTest::newRow("Delete Record") << "TableBrowser.ui" << "actionDeleteRecord";
    QTest::newRow("Revert Changes") << "MainWindow.ui" << "fileRevertAction";
}

void TestActionShortcuts::recordActionsHaveShortcuts()
{
    QFETCH(QString, file);
    QFETCH(QString, action);

    bool found = false;
    for(const ActionShortcut& entry : allShortcuts())
    {
        if(entry.file == file && entry.action == action)
        {
            found = true;
            QVERIFY(!entry.shortcut.isEmpty());
        }
    }
    QVERIFY2(found, "The action has no shortcut");
}

void TestActionShortcuts::shortcutsAreUnique()
{
    // Actions limited to a single widget can reuse the keys of other actions, e.g. for the different text editors. All other shortcuts
    // are active in overlapping places and have to be different.
    QMap<QString, QStringList> users;
    for(const ActionShortcut& entry : allShortcuts())
    {
        if(entry.context != "Qt::WidgetShortcut")
            users[entry.shortcut.toString(QKeySequence::PortableText)].push_back(entry.action);
    }
    for(const QString& key : codeShortcuts)
        users[QKeySequence(key).toString(QKeySequence::PortableText)].push_back("shortcut defined in the code");

    for(auto it = users.constBegin(); it != users.constEnd(); ++it)
        QVERIFY2(it.value().count() == 1, qPrintable(it.key() + " is used more than once: " + it.value().join(", ")));
}

void TestActionShortcuts::shortcutsDoNotShadowStandardKeys()
{
    // Standard keys like Cut, Copy, Paste or the text navigation keys must keep working in the table cells and in the editors
    const QKeySequence::StandardKey standardKeys[] = {
        QKeySequence::Open, QKeySequence::Close, QKeySequence::Save, QKeySequence::New, QKeySequence::Delete, QKeySequence::Cut,
        QKeySequence::Copy, QKeySequence::Paste, QKeySequence::Undo, QKeySequence::Redo, QKeySequence::Find, QKeySequence::FindNext,
        QKeySequence::FindPrevious, QKeySequence::Replace, QKeySequence::SelectAll, QKeySequence::Refresh, QKeySequence::Print,
        QKeySequence::DeleteStartOfWord, QKeySequence::DeleteEndOfWord, QKeySequence::DeleteEndOfLine, QKeySequence::DeleteCompleteLine,
        QKeySequence::Backspace, QKeySequence::ZoomIn, QKeySequence::ZoomOut,
    };

    for(const char* action : {"actionNewRecord", "actionDeleteRecord", "fileRevertAction"})
    {
        for(const ActionShortcut& entry : allShortcuts())
        {
            if(entry.action != action)
                continue;
            for(QKeySequence::StandardKey standardKey : standardKeys)
                QVERIFY2(!QKeySequence::keyBindings(standardKey).contains(entry.shortcut),
                         qPrintable(QString("%1 uses the shortcut %2 which already is a standard key").arg(entry.action, entry.shortcut.toString())));
        }
    }
}
