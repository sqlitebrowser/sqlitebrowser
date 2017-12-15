#include "jsontextedit.h"
#include "Settings.h"

QsciLexerJSON* JsonTextEdit::jsonLexer = nullptr;

JsonTextEdit::JsonTextEdit(QWidget* parent) :
    ExtendedScintilla(parent)
{
    // Create lexer object if not done yet
    if(jsonLexer == nullptr)
        jsonLexer = new QsciLexerJSON(this);

    // Set the JSON lexer
    setLexer(jsonLexer);

    jsonLexer->setFoldCompact(false);

    // Do rest of initialisation
    reloadSettings();
}

JsonTextEdit::~JsonTextEdit()
{
}

void JsonTextEdit::reloadSettings()
{
    ExtendedScintilla::reloadSettings();

    setupSyntaxHighlightingFormat("comment", QsciLexerJSON::CommentLine);
    setupSyntaxHighlightingFormat("comment", QsciLexerJSON::CommentBlock);
    setupSyntaxHighlightingFormat("keyword", QsciLexerJSON::Keyword);
    setupSyntaxHighlightingFormat("keyword", QsciLexerJSON::KeywordLD);
    setupSyntaxHighlightingFormat("string", QsciLexerJSON::String);
    setupSyntaxHighlightingFormat("table", QsciLexerJSON::Number);
    setupSyntaxHighlightingFormat("identifier", QsciLexerJSON::Property);
    jsonLexer->setHighlightComments(true);

    // The default style for invalid JSON or unclosed strings uses red
    // background and white foreground, but the current line has
    // precedence, so it is by default white over gray. We change the
    // default to something more readable for the current line at
    // invalid JSON.
    QColor stringColor = QColor(Settings::getValue("syntaxhighlighter", "string_colour").toString());
    jsonLexer->setColor(stringColor, QsciLexerJSON::Error);
    jsonLexer->setColor(stringColor, QsciLexerJSON::UnclosedString);
    QFont errorFont(Settings::getValue("editor", "font").toString());
    errorFont.setPointSize(Settings::getValue("editor", "fontsize").toInt());
    errorFont.setItalic(true);
    jsonLexer->setFont(errorFont, QsciLexerJSON::Error);
    jsonLexer->setFont(errorFont, QsciLexerJSON::UnclosedString);
    jsonLexer->setPaper(jsonLexer->defaultPaper(QsciLexerJSON::String), QsciLexerJSON::Error);
    jsonLexer->setPaper(jsonLexer->defaultPaper(QsciLexerJSON::String), QsciLexerJSON::UnclosedString);
}
