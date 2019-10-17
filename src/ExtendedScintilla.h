#ifndef EXTENDEDSCINTILLA_H
#define EXTENDEDSCINTILLA_H

#include "Qsci/qsciscintilla.h"

class FindReplaceDialog;
class QShortcut;

/**
 * @brief The ExtendedScintilla class
 * This class extends the QScintilla widget for the application
 */
class ExtendedScintilla : public QsciScintilla
{
    Q_OBJECT

public:
    explicit ExtendedScintilla(QWidget *parent = nullptr);

    bool findText(QString text, bool regexp, bool caseSensitive, bool words, bool wrap, bool forward);
    void setEnabledFindDialog(bool value);
    void clearSelection();
    // Override parent setLexer
    void setLexer(QsciLexer *lexer) override;
    // Override parent setReadOnly
    void setReadOnly(bool ro) override;
    // Override parent setText
    void setText(const QString& text) override;

public slots:
    void reloadKeywords();
    void reloadSettings();
    void clearErrorIndicators();
    void setErrorIndicator(int fromRow, int fromIndex, int toRow, int toIndex);
    // Set error indicator from position to end of line
    void setErrorIndicator(int position);
    void openFindReplaceDialog();
    void openFindDialog();
    void openPrintDialog();

protected:
    void dropEvent(QDropEvent* e) override;

    void setupSyntaxHighlightingFormat(QsciLexer* lexer, const std::string& settings_name, int style);
    void reloadLexerSettings(QsciLexer *lexer);
    void reloadCommonSettings();

    int errorIndicatorNumber;
    bool showErrorIndicators;
    FindReplaceDialog* findReplaceDialog;
    QShortcut* shortcutFind;

private slots:
    void updateLineNumberAreaWidth();
    void showContextMenu(const QPoint &pos);
};

#endif
