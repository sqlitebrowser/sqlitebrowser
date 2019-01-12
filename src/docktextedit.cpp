#include "docktextedit.h"
#include "Settings.h"

QsciLexerJSON* DockTextEdit::jsonLexer = nullptr;
QsciLexerXML* DockTextEdit::xmlLexer = nullptr;

DockTextEdit::DockTextEdit(QWidget* parent) :
    ExtendedScintilla(parent)
{
    // Create lexer objects if not done yet
    if(jsonLexer == nullptr)
        jsonLexer = new QsciLexerJSON(this);
    if(xmlLexer == nullptr)
        xmlLexer = new QsciLexerXML(this);

    // Set the JSON lexer as default
    setLexer(jsonLexer);

    jsonLexer->setFoldCompact(false);
    jsonLexer->setHighlightComments(true);

    // Do rest of initialisation
    reloadSettings();
}

DockTextEdit::~DockTextEdit()
{
}

void DockTextEdit::reloadSettings()
{
    // Set the parent settings for both lexers
    reloadLexerSettings(jsonLexer);
    reloadLexerSettings(xmlLexer);

    setupSyntaxHighlightingFormat(jsonLexer, "comment", QsciLexerJSON::CommentLine);
    setupSyntaxHighlightingFormat(jsonLexer, "comment", QsciLexerJSON::CommentBlock);
    setupSyntaxHighlightingFormat(jsonLexer, "keyword", QsciLexerJSON::Keyword);
    setupSyntaxHighlightingFormat(jsonLexer, "keyword", QsciLexerJSON::KeywordLD);
    setupSyntaxHighlightingFormat(jsonLexer, "function", QsciLexerJSON::Operator);
    setupSyntaxHighlightingFormat(jsonLexer, "string", QsciLexerJSON::String);
    setupSyntaxHighlightingFormat(jsonLexer, "table", QsciLexerJSON::Number);
    setupSyntaxHighlightingFormat(jsonLexer, "identifier", QsciLexerJSON::Property);

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

    xmlLexer->setColor(QColor(Settings::getValue("syntaxhighlighter", "foreground_colour").toString()));
    setupSyntaxHighlightingFormat(xmlLexer, "comment", QsciLexerHTML::HTMLComment);
    setupSyntaxHighlightingFormat(xmlLexer, "keyword", QsciLexerHTML::Tag);
    setupSyntaxHighlightingFormat(xmlLexer, "keyword", QsciLexerHTML::XMLTagEnd);
    setupSyntaxHighlightingFormat(xmlLexer, "keyword", QsciLexerHTML::XMLStart);
    setupSyntaxHighlightingFormat(xmlLexer, "keyword", QsciLexerHTML::XMLEnd);
    setupSyntaxHighlightingFormat(xmlLexer, "string", QsciLexerHTML::HTMLDoubleQuotedString);
    setupSyntaxHighlightingFormat(xmlLexer, "string", QsciLexerHTML::HTMLSingleQuotedString);
    setupSyntaxHighlightingFormat(xmlLexer, "table", QsciLexerHTML::HTMLNumber);
    setupSyntaxHighlightingFormat(xmlLexer, "identifier", QsciLexerHTML::Attribute);
}

void DockTextEdit::setLanguage(Language lang)
{
    m_language = lang;
    switch (lang) {
    case JSON:
        setLexer(jsonLexer);
        break;
    case XML:
        setLexer(xmlLexer);
        break;
    }
}
