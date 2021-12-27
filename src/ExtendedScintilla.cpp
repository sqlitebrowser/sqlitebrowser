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
    showErrorIndicators(true),
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
    connect(this, &ExtendedScintilla::linesChanged, this, &ExtendedScintilla::updateLineNumberAreaWidth);

    // The shortcuts are constrained to the Widget context so they do not conflict with other SqlTextEdit widgets in the Main Window.
    QShortcut* shortcutFindReplace = new QShortcut(QKeySequence(tr("Ctrl+H")), this, nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutFindReplace, &QShortcut::activated, this, &ExtendedScintilla::openFindReplaceDialog);
    shortcutFind = new QShortcut(QKeySequence(tr("Ctrl+F")), this, nullptr, nullptr, Qt::WidgetShortcut);
    connect(shortcutFind, &QShortcut::activated, this, &ExtendedScintilla::openFindDialog);

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
    connect(this, &ExtendedScintilla::customContextMenuRequested, this, &ExtendedScintilla::showContextMenu);
}

void ExtendedScintilla::updateLineNumberAreaWidth()
{
    // Calculate number of digits of the current number of lines
    int digits = static_cast<int>(std::log10(lines())) + 1;

    // Calculate the width of this number if it was all zeros (this is because a 1 might require less space than a 0 and this could
    // cause some flickering depending on the font) and set the new margin width.
#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
    setMarginWidth(0, QFontMetrics(font()).width(QString("0").repeated(digits)) + 5);
#else
    setMarginWidth(0, QFontMetrics(font()).horizontalAdvance(QString("0").repeated(digits)) + 5);
#endif
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

void ExtendedScintilla::setupSyntaxHighlightingFormat(QsciLexer* lexer, const std::string& settings_name, int style)
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
    reloadCommonSettings();
}

void ExtendedScintilla::reloadCommonSettings()
{
    // Set margins and default text colours according to settings. setLexer seems to reset these colours.

    // Use desktop default colors for margins when following desktop
    // style, or the colors matching the dark style-sheet, otherwise.
    switch (Settings::getValue("General", "appStyle").toInt()) {
    case Settings::FollowDesktopStyle :
        setMarginsBackgroundColor(QPalette().color(QPalette::Active, QPalette::Window));
        setMarginsForegroundColor(QPalette().color(QPalette::Active, QPalette::WindowText));
        break;
    case Settings::DarkStyle :
        setMarginsBackgroundColor(QColor(0x32, 0x41, 0x4B));
        setMarginsForegroundColor(QColor(0xEF, 0xF0, 0xF1));
        break;
    case Settings::LightStyle :
        setMarginsBackgroundColor(QColor(0xC9, 0xCD, 0xD0));
        setMarginsForegroundColor(QColor(0x00, 0x00, 0x00));
        break;
    }
    setPaper(Settings::getValue("syntaxhighlighter", "background_colour").toString());
    setColor(Settings::getValue("syntaxhighlighter", "foreground_colour").toString());
    setMatchedBraceBackgroundColor(Settings::getValue("syntaxhighlighter", "highlight_colour").toString());

    setSelectionBackgroundColor(Settings::getValue("syntaxhighlighter", "selected_bg_colour").toString());

    setSelectionForegroundColor(Settings::getValue("syntaxhighlighter", "selected_fg_colour").toString());
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
    QColor foreground (Settings::getValue("syntaxhighlighter", "foreground_colour").toString());
    QColor background (Settings::getValue("syntaxhighlighter", "background_colour").toString());

    QFont defaultfont(Settings::getValue("editor", "font").toString());
    defaultfont.setStyleHint(QFont::TypeWriter);
    defaultfont.setPointSize(Settings::getValue("editor", "fontsize").toInt());

    // Set syntax highlighting settings
    if(lexer)
    {
        lexer->setFont(defaultfont);

        lexer->setDefaultPaper(background);
        lexer->setDefaultColor(foreground);

        // This sets the base colors for all the styles
        lexer->setPaper(background);
        lexer->setColor(foreground);
    }

    // Set font
    setFont(defaultfont);

    // Show line numbers
    setMarginsFont(defaultfont);
    setMarginLineNumbers(0, true);
    updateLineNumberAreaWidth();

    // Highlight current line
    setCaretLineVisible(true);
    setCaretLineBackgroundColor(QColor(Settings::getValue("syntaxhighlighter", "currentline_colour").toString()));
    setCaretForegroundColor(foreground);

    // Set tab settings
    setTabWidth(Settings::getValue("editor", "tabsize").toInt());
    setIndentationsUseTabs(Settings::getValue("editor", "indentation_use_tabs").toBool());

    if(lexer)
        lexer->refreshProperties();

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

        int column = static_cast<int>(SendScintilla(QsciScintillaBase::SCI_GETCOLUMN, position));
        int line = static_cast<int>(SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION, position));

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
                     /* line */ -1, /* index */ -1,
                     /* show */ true, /* posix */ true, /* cxx11 */ true);
}

void ExtendedScintilla::setEnabledFindDialog(bool value)
{
    shortcutFind->setEnabled(value);
}

void ExtendedScintilla::clearSelection()
{
    setSelection(-1,-1,-1,-1);
}

void ExtendedScintilla::openFindReplaceDialog()
{
    findReplaceDialog->setExtendedScintilla(this);
    findReplaceDialog->showFindReplaceDialog(true);
}

void ExtendedScintilla::openFindDialog()
{
    findReplaceDialog->setExtendedScintilla(this);
    findReplaceDialog->showFindReplaceDialog(false);
}

void ExtendedScintilla::showContextMenu(const QPoint &pos)
{

    // This has to be created here, otherwise the set of enabled options would not update accordingly.
    QMenu* editContextMenu = createStandardContextMenu();
    editContextMenu->addSeparator();
    if (shortcutFind->isEnabled()) {
        QAction* findAction = new QAction(QIcon(":/icons/find"), tr("Find..."), this);
        findAction->setShortcut(shortcutFind->key());
        connect(findAction, &QAction::triggered, this, &ExtendedScintilla::openFindDialog);
        editContextMenu->addAction(findAction);
    }
    QAction* findReplaceAction = new QAction(QIcon(":/icons/text_replace"), tr("Find and Replace..."), this);
    findReplaceAction->setShortcut(QKeySequence(tr("Ctrl+H")));
    connect(findReplaceAction, &QAction::triggered, this, &ExtendedScintilla::openFindReplaceDialog);

    QAction* printAction = new QAction(QIcon(":/icons/print"), tr("Print..."), this);
    printAction->setShortcut(QKeySequence(tr("Ctrl+P")));
    connect(printAction, &QAction::triggered, this, &ExtendedScintilla::openPrintDialog);

    editContextMenu->addAction(findReplaceAction);
    editContextMenu->addSeparator();
    editContextMenu->addAction(printAction);

    editContextMenu->exec(mapToGlobal(pos));
}

void ExtendedScintilla::openPrintDialog()
{
    QsciPrinter printer;
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer);

    connect(dialog, &QPrintPreviewDialog::paintRequested, this, [&](QPrinter *previewPrinter) {
        QsciPrinter* sciPrinter = static_cast<QsciPrinter*>(previewPrinter);
        sciPrinter->printRange(this);
    });

    dialog->exec();

    delete dialog;
}

void ExtendedScintilla::setReadOnly(bool ro)
{
    QsciScintilla::setReadOnly(ro);
    // Disable or enable caret blinking so it is obvious whether the text can be modified or not. Otherwise there isn't any other hint.
    SendScintilla(QsciScintillaBase::SCI_SETCARETPERIOD, ro ? 0 : 500);
}

void ExtendedScintilla::setText(const QString& text)
{
    // Reset scroll width, so the scroll bar is readjusted to the new text.
    // Otherwise, it grows always bigger.
    setScrollWidth(80);
    QsciScintilla::setText(text);
}
