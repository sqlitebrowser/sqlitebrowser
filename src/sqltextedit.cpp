#include "sql/sqlitetypes.h"
#include "sqltextedit.h"
#include "Settings.h"
#include "SqlUiLexer.h"

#include <Qsci/qscicommandset.h>
#include <Qsci/qscicommand.h>

#include <QShortcut>
#include <QRegExp>

SqlUiLexer* SqlTextEdit::sqlLexer = nullptr;

SqlTextEdit::SqlTextEdit(QWidget* parent) :
    ExtendedScintilla(parent)
{
    // Create lexer object if not done yet
    if(sqlLexer == nullptr)
        sqlLexer = new SqlUiLexer(this);

    // Set the SQL lexer
    setLexer(sqlLexer);

    // Set icons for auto completion
    registerImage(SqlUiLexer::ApiCompleterIconIdKeyword, QImage(":/icons/keyword"));
    registerImage(SqlUiLexer::ApiCompleterIconIdFunction, QImage(":/icons/function"));
    registerImage(SqlUiLexer::ApiCompleterIconIdTable, QImage(":/icons/table"));
    registerImage(SqlUiLexer::ApiCompleterIconIdColumn, QImage(":/icons/field"));
    registerImage(SqlUiLexer::ApiCompleterIconIdSchema, QImage(":/icons/database"));

    // Remove command bindings that would interfere with our shortcutToggleComment
    QsciCommand * command = standardCommands()->boundTo(Qt::ControlModifier+Qt::Key_Slash);
    command->setKey(0);
    command = standardCommands()->boundTo(Qt::ControlModifier+Qt::ShiftModifier+Qt::Key_Slash);
    command->setKey(0);

    // Change command binding for Ctrl+T so it doesn't interfere with "Open tab"
    command = standardCommands()->boundTo(Qt::ControlModifier+Qt::Key_T);
    command->setKey(Qt::ControlModifier+Qt::ShiftModifier+Qt::Key_Up);

    QShortcut* shortcutToggleComment = new QShortcut(QKeySequence(tr("Ctrl+/")), this, nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutToggleComment, &QShortcut::activated, this, &SqlTextEdit::toggleBlockComment);

    // Do rest of initialisation
    reloadSettings();
}

SqlTextEdit::~SqlTextEdit()
{
}

void SqlTextEdit::reloadSettings()
{
    // Enable auto completion if it hasn't been disabled
    if(Settings::getValue("editor", "auto_completion").toBool())
    {
        setAutoCompletionThreshold(3);
        setAutoCompletionCaseSensitivity(true);
        setAutoCompletionShowSingle(true);
        setAutoCompletionSource(QsciScintilla::AcsAPIs);
    } else {
        setAutoCompletionThreshold(0);
    }

    ExtendedScintilla::reloadSettings();

    setupSyntaxHighlightingFormat(sqlLexer, "comment", QsciLexerSQL::Comment);
    setupSyntaxHighlightingFormat(sqlLexer, "comment", QsciLexerSQL::CommentLine);
    setupSyntaxHighlightingFormat(sqlLexer, "comment", QsciLexerSQL::CommentDoc);
    setupSyntaxHighlightingFormat(sqlLexer, "keyword", QsciLexerSQL::Keyword);
    setupSyntaxHighlightingFormat(sqlLexer, "table", QsciLexerSQL::KeywordSet6);
    setupSyntaxHighlightingFormat(sqlLexer, "function", QsciLexerSQL::KeywordSet7);
    setupSyntaxHighlightingFormat(sqlLexer, "string", QsciLexerSQL::SingleQuotedString);

    // Highlight double quote strings as identifier or as literal string depending on user preference
    switch(static_cast<sqlb::escapeQuoting>(Settings::getValue("editor", "identifier_quotes").toInt())) {
    case sqlb::DoubleQuotes:
        setupSyntaxHighlightingFormat(sqlLexer, "identifier", QsciLexerSQL::DoubleQuotedString);
        sqlLexer->setQuotedIdentifiers(false);
        break;
    case sqlb::GraveAccents:
        sqlLexer->setQuotedIdentifiers(true);
        // Fall through, treat quoted string as literal string
    case sqlb::SquareBrackets:
        setupSyntaxHighlightingFormat(sqlLexer, "string", QsciLexerSQL::DoubleQuotedString);
        break;
    }
    setupSyntaxHighlightingFormat(sqlLexer, "identifier", QsciLexerSQL::Identifier);
    setupSyntaxHighlightingFormat(sqlLexer, "identifier", QsciLexerSQL::QuotedIdentifier);
}


void SqlTextEdit::toggleBlockComment()
{
    int lineFrom, indexFrom, lineTo, indexTo;

    // If there is no selection, select the current line
    if (!hasSelectedText()) {
        getCursorPosition(&lineFrom, &indexFrom);

        // Windows lines requires an adjustment, otherwise the selection would
        // end in the next line.
        indexTo = text(lineFrom).endsWith("\r\n") ? lineLength(lineFrom)-1 : lineLength(lineFrom);

        setSelection(lineFrom, 0, lineFrom, indexTo);
    }

    getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);

    bool uncomment = text(lineFrom).contains(QRegExp("^[ \t]*--"));

    // If the selection ends before the first character of a line, don't
    // take this line into account for un/commenting.
    if (indexTo==0)
        lineTo--;

    beginUndoAction();

    // Iterate over the selected lines, get line text, make
    // replacement depending on whether the first line was commented
    // or uncommented, and replace the line text. All in a single undo action.
    for (int line=lineFrom; line<=lineTo; line++) {
        QString lineText = text(line);

        if (uncomment)
            lineText.replace(QRegExp("^([ \t]*)-- ?"), "\\1");
        else
            lineText.replace(QRegExp("^"), "-- ");

        indexTo = lineText.endsWith("\r\n") ? lineLength(line)-1 : lineLength(line);

        setSelection(line, 0, line, indexTo);
        replaceSelectedText(lineText);
    }
    endUndoAction();
}
