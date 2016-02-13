#include "sqltextedit.h"
#include "PreferencesDialog.h"
#include "SqlUiLexer.h"

#include <QFile>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <cmath>

SqlUiLexer* SqlTextEdit::sqlLexer = 0;

SqlTextEdit::SqlTextEdit(QWidget* parent) :
    QsciScintilla(parent)
{
    // Create lexer object if not done yet
    if(sqlLexer == 0)
        sqlLexer = new SqlUiLexer(this);

    // Set the lexer
    setLexer(sqlLexer);

    // Enable auto completion
    setAutoCompletionThreshold(3);
    setAutoCompletionCaseSensitivity(false);
    setAutoCompletionShowSingle(true);
    setAutoCompletionSource(QsciScintilla::AcsAPIs);

    // Set icons for auto completion
    registerImage(SqlUiLexer::ApiCompleterIconIdKeyword, QImage(":/icons/keyword"));
    registerImage(SqlUiLexer::ApiCompleterIconIdFunction, QImage(":/icons/function"));
    registerImage(SqlUiLexer::ApiCompleterIconIdTable, QImage(":/icons/table"));
    registerImage(SqlUiLexer::ApiCompleterIconIdColumn, QImage(":/icons/field"));

    // Enable UTF8
    setUtf8(true);

    // Enable brace matching
    setBraceMatching(QsciScintilla::SloppyBraceMatch);

    // Enable auto indentation
    setAutoIndent(true);

    // Enable folding
    setFolding(QsciScintilla::BoxedTreeFoldStyle);

    // Create error indicator
    errorIndicatorNumber = indicatorDefine(QsciScintilla::SquiggleIndicator);
    setIndicatorForegroundColor(Qt::red, errorIndicatorNumber);

    // Do rest of initialisation
    reloadSettings();

    // Connect signals
    connect(this, SIGNAL(linesChanged()), this, SLOT(updateLineNumberAreaWidth()));
}

SqlTextEdit::~SqlTextEdit()
{
}

void SqlTextEdit::updateLineNumberAreaWidth()
{
    // Calculate number of digits of the current number of lines
    int digits = std::floor(std::log10(lines())) + 1;

    // Calculate the width of this number if it was all zeros (this is because a 1 might require less space than a 0 and this could
    // cause some flickering depending on the font) and set the new margin width.
    QFont font = lexer()->font(QsciLexerSQL::Default);
    setMarginWidth(0, QFontMetrics(font).width(QString("0").repeated(digits)) + 5);
}

void SqlTextEdit::dropEvent(QDropEvent* e)
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

void SqlTextEdit::setupSyntaxHighlightingFormat(const QString& settings_name, int style)
{
    sqlLexer->setColor(QColor(PreferencesDialog::getSettingsValue("syntaxhighlighter", settings_name + "_colour").toString()), style);

    QFont font(PreferencesDialog::getSettingsValue("editor", "font").toString());
    font.setPointSize(PreferencesDialog::getSettingsValue("editor", "fontsize").toInt());
    font.setBold(PreferencesDialog::getSettingsValue("syntaxhighlighter", settings_name + "_bold").toBool());
    font.setItalic(PreferencesDialog::getSettingsValue("syntaxhighlighter", settings_name + "_italic").toBool());
    font.setUnderline(PreferencesDialog::getSettingsValue("syntaxhighlighter", settings_name + "_underline").toBool());
    sqlLexer->setFont(font, style);
}

void SqlTextEdit::reloadKeywords()
{
    // Set lexer again to reload the updated keywords list
    setLexer(lexer());
}

void SqlTextEdit::reloadSettings()
{
    // Set syntax highlighting settings
    QFont defaultfont(PreferencesDialog::getSettingsValue("editor", "font").toString());
    defaultfont.setStyleHint(QFont::TypeWriter);
    defaultfont.setPointSize(PreferencesDialog::getSettingsValue("editor", "fontsize").toInt());
    sqlLexer->setColor(Qt::black, QsciLexerSQL::Default);
    sqlLexer->setFont(defaultfont);
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

    // Set font
    QFont font(PreferencesDialog::getSettingsValue("editor", "font").toString());
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(PreferencesDialog::getSettingsValue("editor", "fontsize").toInt());
    setFont(font);

    // Show line numbers
    QFont marginsfont(QFont(PreferencesDialog::getSettingsValue("editor", "font").toString()));
    marginsfont.setPointSize(font.pointSize());
    setMarginsFont(marginsfont);
    setMarginLineNumbers(0, true);
    setMarginsBackgroundColor(Qt::lightGray);
    updateLineNumberAreaWidth();

    // Highlight current line
    setCaretLineVisible(true);
    setCaretLineBackgroundColor(QColor(PreferencesDialog::getSettingsValue("syntaxhighlighter", "currentline_colour").toString()));

    // Set tab width
    setTabWidth(PreferencesDialog::getSettingsValue("editor", "tabsize").toInt());

    // Check if error indicators are enabled and clear them if they just got disabled
    showErrorIndicators = PreferencesDialog::getSettingsValue("editor", "error_indicators").toBool();
    if(!showErrorIndicators)
        clearErrorIndicators();
}

void SqlTextEdit::clearErrorIndicators()
{
    // Clear any error indicators from position (0,0) to the last column of the last line
    clearIndicatorRange(0, 0, lines(), lineLength(lines()), errorIndicatorNumber);
}

void SqlTextEdit::setErrorIndicator(int fromRow, int fromIndex, int toRow, int toIndex)
{
    // Set error indicator for the specified range but only if they're enabled
    if(showErrorIndicators)
        fillIndicatorRange(fromRow, fromIndex, toRow, toIndex, errorIndicatorNumber);
}
