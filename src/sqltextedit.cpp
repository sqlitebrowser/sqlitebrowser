#include "sqltextedit.h"
#include "Settings.h"
#include "SqlUiLexer.h"
#include "FindReplaceDialog.h"

#include <QFile>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QShortcut>
#include <QAction>
#include <QMenu>
#include <cmath>

SqlUiLexer* SqlTextEdit::sqlLexer = nullptr;

SqlTextEdit::SqlTextEdit(QWidget* parent) :
    QsciScintilla(parent),
    findReplaceDialog(new FindReplaceDialog(this))
{
    // Create lexer object if not done yet
    if(sqlLexer == nullptr)
        sqlLexer = new SqlUiLexer(this);

    // Set the lexer
    setLexer(sqlLexer);

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

    // The shortcut is constrained to the Widget context so it does not conflict with other SqlTextEdit widgets in the Main Window.
    QShortcut* shortcutFindReplace = new QShortcut(QKeySequence(tr("Ctrl+H")), this, nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutFindReplace, SIGNAL(activated()), this, SLOT(openFindReplaceDialog()));

    // Prepare for adding the find/replace option to the QScintilla context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &)));
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
    sqlLexer->setColor(QColor(Settings::getValue("syntaxhighlighter", settings_name + "_colour").toString()), style);

    QFont font(Settings::getValue("editor", "font").toString());
    font.setPointSize(Settings::getValue("editor", "fontsize").toInt());
    font.setBold(Settings::getValue("syntaxhighlighter", settings_name + "_bold").toBool());
    font.setItalic(Settings::getValue("syntaxhighlighter", settings_name + "_italic").toBool());
    font.setUnderline(Settings::getValue("syntaxhighlighter", settings_name + "_underline").toBool());
    sqlLexer->setFont(font, style);
}

void SqlTextEdit::reloadKeywords()
{
    // Set lexer again to reload the updated keywords list
    setLexer(lexer());
}

void SqlTextEdit::reloadSettings()
{
    // Enable auto completion if it hasn't been disabled
    if(Settings::getValue("editor", "auto_completion").toBool())
    {
        setAutoCompletionThreshold(3);
        setAutoCompletionCaseSensitivity(false);
        setAutoCompletionShowSingle(true);
        setAutoCompletionSource(QsciScintilla::AcsAPIs);
    } else {
        setAutoCompletionThreshold(0);
    }

    // Set syntax highlighting settings
    QFont defaultfont(Settings::getValue("editor", "font").toString());
    defaultfont.setStyleHint(QFont::TypeWriter);
    defaultfont.setPointSize(Settings::getValue("editor", "fontsize").toInt());
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

    // Check if error indicators are enabled and clear them if they just got disabled
    showErrorIndicators = Settings::getValue("editor", "error_indicators").toBool();
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

bool SqlTextEdit::findText(QString text, bool regexp, bool caseSensitive, bool words, bool wrap, bool forward) {

    // For finding the previous occurrence, we need to skip the current
    // selection, otherwise we'd always found the same occurrence.
    if (!forward && hasSelectedText()) {
        int lineFrom, indexFrom;
		int lineTo, indexTo;
        getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);
        setCursorPosition(lineFrom, indexFrom);
    }

    return findFirst(text, regexp, caseSensitive, words, wrap, forward);
}

void SqlTextEdit::clearSelection()
{
    setSelection(-1,-1,-1,-1);
}

void SqlTextEdit::openFindReplaceDialog()
{
    findReplaceDialog->setSqlTextEdit(this);
    findReplaceDialog->show();
}

void SqlTextEdit::showContextMenu(const QPoint &pos)
{

    QAction* findReplaceAction = new QAction(QIcon(":/icons/text_replace"), tr("Find and Replace..."), this);
    findReplaceAction->setShortcut(QKeySequence(tr("Ctrl+H")));
    connect(findReplaceAction, &QAction::triggered, [&]() {
            openFindReplaceDialog();
        });

    // This has to be created here, otherwise the set of enabled options would not update accordingly.
    QMenu* editContextMenu = createStandardContextMenu();
    editContextMenu->addSeparator();
    editContextMenu->addAction(findReplaceAction);

    editContextMenu->exec(mapToGlobal(pos));
}
