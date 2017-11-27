#include "jsontextedit.h"
#include "Settings.h"

#include <QFile>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QDebug>
#include <cmath>

QsciLexerJSON* JsonTextEdit::jsonLexer = nullptr;

JsonTextEdit::JsonTextEdit(QWidget* parent) :
    QsciScintilla(parent)
{
    // Create lexer object if not done yet
    if(jsonLexer == nullptr)
        jsonLexer = new QsciLexerJSON(this);

    // Set the lexer
    setLexer(jsonLexer);

    // Enable UTF8
    setUtf8(true);

    // Enable brace matching
    setBraceMatching(QsciScintilla::SloppyBraceMatch);

    // Enable auto indentation
    setAutoIndent(true);

    // Enable folding
    setFolding(QsciScintilla::BoxedTreeFoldStyle);
    jsonLexer->setFoldCompact(false);

    // Create error indicator
    errorIndicatorNumber = indicatorDefine(QsciScintilla::SquiggleIndicator);
    setIndicatorForegroundColor(Qt::red, errorIndicatorNumber);

    // Set a sensible scroll width, so the scroll bar is avoided in
    // most cases.
    setScrollWidth(80);

    // Scroll width is adjusted to ensure that all of the lines
    // currently displayed can be completely scrolled. This mode never
    // adjusts the scroll width to be narrower.
    setScrollWidthTracking(true);

    // Do rest of initialisation
    reloadSettings();

    // Connect signals
    connect(this, SIGNAL(linesChanged()), this, SLOT(updateLineNumberAreaWidth()));
}

JsonTextEdit::~JsonTextEdit()
{
}

void JsonTextEdit::updateLineNumberAreaWidth()
{
    // Calculate number of digits of the current number of lines
    int digits = std::floor(std::log10(lines())) + 1;

    // Calculate the width of this number if it was all zeros (this is because a 1 might require less space than a 0 and this could
    // cause some flickering depending on the font) and set the new margin width.
    QFont font = lexer()->font(QsciLexerJSON::Default);
    setMarginWidth(0, QFontMetrics(font).width(QString("0").repeated(digits)) + 5);
}

void JsonTextEdit::dropEvent(QDropEvent* e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.isEmpty())
        return QsciScintilla::dropEvent(e);

    QString file = urls.first().toLocalFile();
    if(!QFile::exists(file))
        return;

    QFile f(file);
    f.open(QIODevice::ReadOnly);
    setText(f.readAll());
    f.close();
}

void JsonTextEdit::setupSyntaxHighlightingFormat(const QString& settings_name, int style)
{
    jsonLexer->setColor(QColor(Settings::getValue("syntaxhighlighter", settings_name + "_colour").toString()), style);

    QFont font(Settings::getValue("editor", "font").toString());
    font.setPointSize(Settings::getValue("editor", "fontsize").toInt());
    font.setBold(Settings::getValue("syntaxhighlighter", settings_name + "_bold").toBool());
    font.setItalic(Settings::getValue("syntaxhighlighter", settings_name + "_italic").toBool());
    font.setUnderline(Settings::getValue("syntaxhighlighter", settings_name + "_underline").toBool());
    jsonLexer->setFont(font, style);
}

void JsonTextEdit::reloadKeywords()
{
    // Set lexer again to reload the updated keywords list
    setLexer(lexer());
}

void JsonTextEdit::reloadSettings()
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

    // Set syntax highlighting settings
    QFont defaultfont(Settings::getValue("editor", "font").toString());
    defaultfont.setStyleHint(QFont::TypeWriter);
    defaultfont.setPointSize(Settings::getValue("editor", "fontsize").toInt());
    jsonLexer->setFont(defaultfont);
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

    // Set font
    QFont font(Settings::getValue("editor", "font").toString());
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(Settings::getValue("editor", "fontsize").toInt());
    setFont(font);

    // Show line numbers
    QFont marginsfont(QFont(Settings::getValue("editor", "font").toString()));
    marginsfont.setPointSize(font.pointSize());
    setMarginsFont(marginsfont);
    setMarginLineNumbers(0, true);
    setMarginsBackgroundColor(Qt::lightGray);
    updateLineNumberAreaWidth();

    // Highlight current line
    setCaretLineVisible(true);
    setCaretLineBackgroundColor(QColor(Settings::getValue("syntaxhighlighter", "currentline_colour").toString()));

    // Set tab width
    setTabWidth(Settings::getValue("editor", "tabsize").toInt());
    jsonLexer->refreshProperties();

    // Check if error indicators are enabled and clear them if they just got disabled
    showErrorIndicators = Settings::getValue("editor", "error_indicators").toBool();
    if(!showErrorIndicators)
        clearErrorIndicators();

}

void JsonTextEdit::clearErrorIndicators()
{
    // Clear any error indicators from position (0,0) to the last column of the last line
    clearIndicatorRange(0, 0, lines(), lineLength(lines()), errorIndicatorNumber);
}

void JsonTextEdit::setErrorIndicator(int fromRow, int fromIndex, int toRow, int toIndex)
{
    // Set error indicator for the specified range but only if they're enabled
    if(showErrorIndicators)
        fillIndicatorRange(fromRow, fromIndex, toRow, toIndex, errorIndicatorNumber);
}

void JsonTextEdit::setErrorIndicator(int position)
{
    // Set error indicator for the position until end of line, but only if they're enabled
    if(showErrorIndicators) {

        int column = SendScintilla(QsciScintillaBase::SCI_GETCOLUMN, position);
        int line = SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION, position);

        fillIndicatorRange(line, column, line+1, 0, errorIndicatorNumber);
    }
}
