#include "sqltextedit.h"
#include "Settings.h"
#include "SqlUiLexer.h"

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
    setupSyntaxHighlightingFormat(sqlLexer, "string", QsciLexerSQL::DoubleQuotedString);
    setupSyntaxHighlightingFormat(sqlLexer, "string", QsciLexerSQL::SingleQuotedString);
    setupSyntaxHighlightingFormat(sqlLexer, "identifier", QsciLexerSQL::Identifier);
    setupSyntaxHighlightingFormat(sqlLexer, "identifier", QsciLexerSQL::QuotedIdentifier);
}
