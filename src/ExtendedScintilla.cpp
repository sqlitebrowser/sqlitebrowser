#include "ExtendedScintilla.h"
#include "FindReplaceDialog.h"
#include "Settings.h"
#include "Qsci/qscilexer.h"

#include <QFile>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QShortcut>
#include <QAction>
#include <QMenu>
#include <cmath>


ExtendedScintilla::ExtendedScintilla(QWidget* parent) :
    QsciScintilla(parent),
    findReplaceDialog(new FindReplaceDialog(this))
{
    // This class does not set any lexer, that must be done in the child classes.

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

    // Connect signals
    connect(this, SIGNAL(linesChanged()), this, SLOT(updateLineNumberAreaWidth()));

    // The shortcut is constrained to the Widget context so it does not conflict with other SqlTextEdit widgets in the Main Window.
    QShortcut* shortcutFindReplace = new QShortcut(QKeySequence(tr("Ctrl+H")), this, nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutFindReplace, SIGNAL(activated()), this, SLOT(openFindReplaceDialog()));

    // Prepare for adding the find/replace option to the QScintilla context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &)));
}

ExtendedScintilla::~ExtendedScintilla()
{
}

void ExtendedScintilla::updateLineNumberAreaWidth()
{
    // Calculate number of digits of the current number of lines
    int digits = std::floor(std::log10(lines())) + 1;

    // Calculate the width of this number if it was all zeros (this is because a 1 might require less space than a 0 and this could
    // cause some flickering depending on the font) and set the new margin width.
    QFont font = lexer()->defaultFont();
    setMarginWidth(0, QFontMetrics(font).width(QString("0").repeated(digits)) + 5);
}

void ExtendedScintilla::dropEvent(QDropEvent* e)
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

void ExtendedScintilla::setupSyntaxHighlightingFormat(const QString& settings_name, int style)
{
    lexer()->setColor(QColor(Settings::getValue("syntaxhighlighter", settings_name + "_colour").toString()), style);

    QFont font(Settings::getValue("editor", "font").toString());
    font.setPointSize(Settings::getValue("editor", "fontsize").toInt());
    font.setBold(Settings::getValue("syntaxhighlighter", settings_name + "_bold").toBool());
    font.setItalic(Settings::getValue("syntaxhighlighter", settings_name + "_italic").toBool());
    font.setUnderline(Settings::getValue("syntaxhighlighter", settings_name + "_underline").toBool());
    lexer()->setFont(font, style);
}

void ExtendedScintilla::reloadKeywords()
{
    // Set lexer again to reload the updated keywords list
    setLexer(lexer());
}

void ExtendedScintilla::reloadSettings()
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
    lexer()->setDefaultColor(Qt::black);
    lexer()->setFont(defaultfont);

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
    lexer()->refreshProperties();

    // Check if error indicators are enabled and clear them if they just got disabled
    showErrorIndicators = Settings::getValue("editor", "error_indicators").toBool();
    if(!showErrorIndicators)
        clearErrorIndicators();

}

void ExtendedScintilla::clearErrorIndicators()
{
    // Clear any error indicators from position (0,0) to the last column of the last line
    clearIndicatorRange(0, 0, lines(), lineLength(lines()), errorIndicatorNumber);
}

void ExtendedScintilla::setErrorIndicator(int fromRow, int fromIndex, int toRow, int toIndex)
{
    // Set error indicator for the specified range but only if they're enabled
    if(showErrorIndicators)
        fillIndicatorRange(fromRow, fromIndex, toRow, toIndex, errorIndicatorNumber);
}

void ExtendedScintilla::setErrorIndicator(int position)
{
    // Set error indicator for the position until end of line, but only if they're enabled
    if(showErrorIndicators) {

        int column = SendScintilla(QsciScintillaBase::SCI_GETCOLUMN, position);
        int line = SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION, position);

        fillIndicatorRange(line, column, line+1, 0, errorIndicatorNumber);
    }
}

bool ExtendedScintilla::findText(QString text, bool regexp, bool caseSensitive, bool words, bool wrap, bool forward) {

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

void ExtendedScintilla::clearSelection()
{
    setSelection(-1,-1,-1,-1);
}

void ExtendedScintilla::openFindReplaceDialog()
{
    findReplaceDialog->setExtendedScintilla(this);
    findReplaceDialog->show();
}

void ExtendedScintilla::showContextMenu(const QPoint &pos)
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
