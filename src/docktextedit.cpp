#include "docktextedit.h"
#include "Settings.h"

#include <Qsci/qscistyle.h>

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

    // Set plain text as default
    setLanguage(PlainText);

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

    // Set the databrowser font for the plain text editor.
    plainTextFont = QFont(Settings::getValue("databrowser", "font").toString());
    plainTextFont.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());
    setFont(plainTextFont);

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
    case PlainText: {
        setLexer(nullptr);
        setFolding(QsciScintilla::NoFoldStyle);
        // This appears to be reset by setLexer
        setFont(plainTextFont);
        break;
    }
    case JSON:
        setLexer(jsonLexer);
        setFolding(QsciScintilla::BoxedTreeFoldStyle);
        break;
    case XML:
        setLexer(xmlLexer);
        setFolding(QsciScintilla::BoxedTreeFoldStyle);
        break;
    }
}

void DockTextEdit::setTextInMargin(const QString& text)
{
    clearMarginText();
    setMarginType(0, QsciScintilla::TextMargin);
    setMarginText(0, text, QsciStyle(QsciScintillaBase::STYLE_LINENUMBER));
    setMarginWidth(0, text);
    reloadCommonSettings();
}

void DockTextEdit::clearTextInMargin()
{
    clearMarginText();
    setMarginLineNumbers(0, true);
    reloadCommonSettings();
    linesChanged();
}
