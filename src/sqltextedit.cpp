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
    // Initialise lexer if not done yet
    if(sqlLexer == 0)
    {
        // Create lexer object
        sqlLexer = new SqlUiLexer(this);

        // Set syntax highlighting settings
        sqlLexer->setDefaultColor(Qt::black);
        QFont defaultfont("Monospace");
        defaultfont.setStyleHint(QFont::TypeWriter);
        defaultfont.setPointSize(PreferencesDialog::getSettingsValue("editor", "fontsize").toInt());
        sqlLexer->setDefaultFont(defaultfont);
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

    // Set the lexer
    setLexer(sqlLexer);

    // Set font
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(PreferencesDialog::getSettingsValue("editor", "fontsize").toInt());
    setFont(font);

    // Show line numbers
    QFont marginsfont(QFont("Monospace"));
    marginsfont.setPointSize(font.pointSize());
    setMarginsFont(marginsfont);
    setMarginLineNumbers(0, true);
    setMarginsBackgroundColor(Qt::lightGray);
    connect(this, SIGNAL(linesChanged()), this, SLOT(updateLineNumberAreaWidth()));
    updateLineNumberAreaWidth();

    // Highlight current line
    setCaretLineVisible(true);
    setCaretLineBackgroundColor(QColor(PreferencesDialog::getSettingsValue("syntaxhighlighter", "currentline_colour").toString()));

    // Enable auto completion
    setAutoCompletionThreshold(1);
    setAutoCompletionCaseSensitivity(false);
    setAutoCompletionShowSingle(true);
    setAutoCompletionSource(QsciScintilla::AcsAll);

    // Set icons for auto completion
    registerImage(SqlUiLexer::ApiCompleterIconIdKeyword, QImage(":/icons/keyword"));
    registerImage(SqlUiLexer::ApiCompleterIconIdFunction, QImage(":/icons/function"));
    registerImage(SqlUiLexer::ApiCompleterIconIdTable, QImage(":/icons/table"));
    registerImage(SqlUiLexer::ApiCompleterIconIdColumn, QImage(":/icons/field"));

    // Enable brace matching
    setBraceMatching(QsciScintilla::SloppyBraceMatch);

    // Enable auto indentation
    setAutoIndent(true);

    // Set tab width
    setTabWidth(PreferencesDialog::getSettingsValue("editor", "tabsize").toInt());

    // Enable folding
    setFolding(QsciScintilla::BoxedTreeFoldStyle);

    // Create error indicator
    errorIndicatorNumber = indicatorDefine(QsciScintilla::SquiggleIndicator);
    setIndicatorForegroundColor(Qt::red, errorIndicatorNumber);
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
    QFont font = lexer()->defaultFont(QsciLexerSQL::Default);
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

    QFont font("Monospace");
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
