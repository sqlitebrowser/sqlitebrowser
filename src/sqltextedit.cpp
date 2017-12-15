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
    ExtendedScintilla::reloadSettings();

    setupSyntaxHighlightingFormat("comment", QsciLexerSQL::Comment);
    setupSyntaxHighlightingFormat("comment", QsciLexerSQL::CommentLine);
    setupSyntaxHighlightingFormat("comment", QsciLexerSQL::CommentDoc);
    setupSyntaxHighlightingFormat("keyword", QsciLexerSQL::Keyword);
    setupSyntaxHighlightingFormat("table", QsciLexerSQL::KeywordSet6);
    setupSyntaxHighlightingFormat("function", QsciLexerSQL::KeywordSet7);
    setupSyntaxHighlightingFormat("string", QsciLexerSQL::DoubleQuotedString);
    setupSyntaxHighlightingFormat("string", QsciLexerSQL::SingleQuotedString);
    setupSyntaxHighlightingFormat("identifier", QsciLexerSQL::Identifier);
    setupSyntaxHighlightingFormat("identifier", QsciLexerSQL::QuotedIdentifier);
}
