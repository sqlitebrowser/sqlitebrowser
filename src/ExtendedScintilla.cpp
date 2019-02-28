#include "ExtendedScintilla.h"
#include "FindReplaceDialog.h"
#include "Settings.h"

#include "Qsci/qscilexer.h"
#include "Qsci/qsciprinter.h"
#ifdef Q_OS_MACX
#include <Qsci/qscicommandset.h>
#include <Qsci/qscicommand.h>
#endif

#include <QFile>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QShortcut>
#include <QAction>
#include <QMenu>
#include <QPalette>
#include <QPrintPreviewDialog>
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

    // Visual flags for when wrap lines is enabled
    setWrapVisualFlags(QsciScintilla::WrapFlagByBorder);

    // Connect signals
    connect(this, SIGNAL(linesChanged()), this, SLOT(updateLineNumberAreaWidth()));

    // The shortcuts are constrained to the Widget context so they do not conflict with other SqlTextEdit widgets in the Main Window.
    QShortcut* shortcutFindReplace = new QShortcut(QKeySequence(tr("Ctrl+H")), this, nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutFindReplace, SIGNAL(activated()), this, SLOT(openFindReplaceDialog()));

#ifdef Q_OS_MACX
    // Alt+Backspace on Mac is expected to delete one word to the left,
    // instead of undoing (default Scintilla binding).
    QsciCommand * command = standardCommands()->find(QsciCommand::DeleteWordLeft);
    command->setKey(Qt::AltModifier+Qt::Key_Backspace);
    // And Cmd+Backspace should delete from cursor to the beginning of line
    command = standardCommands()->find(QsciCommand::DeleteLineLeft);
    command->setKey(Qt::ControlModifier+Qt::Key_Backspace);
#endif

    QShortcut* shortcutPrint = new QShortcut(QKeySequence(tr("Ctrl+P")), this, nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutPrint, &QShortcut::activated, this, &ExtendedScintilla::openPrintDialog);

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
    setMarginWidth(0, QFontMetrics(font()).width(QString("0").repeated(digits)) + 5);
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

void ExtendedScintilla::setupSyntaxHighlightingFormat(QsciLexer *lexer, const QString& settings_name, int style)
{
    lexer->setColor(QColor(Settings::getValue("syntaxhighlighter", settings_name + "_colour").toString()), style);

    QFont font(Settings::getValue("editor", "font").toString());
    font.setPointSize(Settings::getValue("editor", "fontsize").toInt());
    font.setBold(Settings::getValue("syntaxhighlighter", settings_name + "_bold").toBool());
    font.setItalic(Settings::getValue("syntaxhighlighter", settings_name + "_italic").toBool());
    font.setUnderline(Settings::getValue("syntaxhighlighter", settings_name + "_underline").toBool());
    lexer->setFont(font, style);
}

void ExtendedScintilla::setLexer(QsciLexer *lexer)
{
    QsciScintilla::setLexer(lexer);

    // Set margins to system window theme. setLexer seems to reset these colours.
    setMarginsBackgroundColor(QPalette().color(QPalette::Active, QPalette::Window));
    setMarginsForegroundColor(QPalette().color(QPalette::Active, QPalette::WindowText));
    setIndentationGuidesBackgroundColor(QPalette().color(QPalette::Active, QPalette::Window));
    setIndentationGuidesForegroundColor(QPalette().color(QPalette::Active, QPalette::WindowText));
}

void ExtendedScintilla::reloadKeywords()
{
    // Set lexer again to reload the updated keywords list
    setLexer(lexer());
}

void ExtendedScintilla::reloadSettings()
{
    reloadLexerSettings(lexer());
    reloadKeywords();
}
void ExtendedScintilla::reloadLexerSettings(QsciLexer *lexer)
{
    // Set syntax highlighting settings
    if(lexer)
    {
        QFont defaultfont(Settings::getValue("editor", "font").toString());
        defaultfont.setStyleHint(QFont::TypeWriter);
        defaultfont.setPointSize(Settings::getValue("editor", "fontsize").toInt());
        lexer->setFont(defaultfont);

        lexer->setDefaultColor(QColor(Settings::getValue("syntaxhighlighter", "foreground_colour").toString()));
        lexer->setPaper(QColor(Settings::getValue("syntaxhighlighter", "background_colour").toString()));
    }

    // Set font
    QFont font(Settings::getValue("editor", "font").toString());
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(Settings::getValue("editor", "fontsize").toInt());
    setFont(font);

    // Show line numbers
    setMarginsFont(font);
    setMarginLineNumbers(0, true);
    updateLineNumberAreaWidth();

    // Highlight current line
    setCaretLineVisible(true);
    setCaretLineBackgroundColor(QColor(Settings::getValue("syntaxhighlighter", "currentline_colour").toString()));
    setCaretForegroundColor(QColor(Settings::getValue("syntaxhighlighter", "foreground_colour").toString()));

    // Set tab width
    setTabWidth(Settings::getValue("editor", "tabsize").toInt());
    if(lexer)
        lexer->refreshProperties();

    // Set wrap lines
    setWrapMode(static_cast<QsciScintilla::WrapMode>(Settings::getValue("editor", "wrap_lines").toInt()));

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

    return findFirst(text, regexp, caseSensitive, words, wrap, forward,
                     /* line */ -1, /* index */ -1, /* show */ true, /* posix */ true);
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
    connect(findReplaceAction, &QAction::triggered, this, &ExtendedScintilla::openFindReplaceDialog);

    QAction* printAction = new QAction(QIcon(":/icons/print"), tr("Print..."), this);
    printAction->setShortcut(QKeySequence(tr("Ctrl+P")));
    connect(printAction, &QAction::triggered, this, &ExtendedScintilla::openPrintDialog);

    // This has to be created here, otherwise the set of enabled options would not update accordingly.
    QMenu* editContextMenu = createStandardContextMenu();
    editContextMenu->addSeparator();
    editContextMenu->addAction(findReplaceAction);
    editContextMenu->addAction(printAction);

    editContextMenu->exec(mapToGlobal(pos));
}

void ExtendedScintilla::openPrintDialog()
{
    QsciPrinter printer;
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer);

    connect(dialog, &QPrintPreviewDialog::paintRequested, [&](QPrinter *previewPrinter) {
        QsciPrinter* sciPrinter = static_cast<QsciPrinter*>(previewPrinter);
        sciPrinter->printRange(this);
    });

    dialog->exec();

    delete dialog;
}
