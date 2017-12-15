#ifndef EXTENDEDSCINTILLA_H
#define EXTENDEDSCINTILLA_H

#include "Qsci/qsciscintilla.h"

class FindReplaceDialog;

/**
 * @brief The ExtendedScintilla class
 * This class extends the QScintilla widget for the application
 */
class ExtendedScintilla : public QsciScintilla
{
    Q_OBJECT

public:
    explicit ExtendedScintilla(QWidget *parent = nullptr);
    virtual ~ExtendedScintilla();

    bool findText(QString text, bool regexp, bool caseSensitive, bool words, bool wrap, bool forward);
    void clearSelection();

public slots:
    void reloadKeywords();
    void reloadSettings();
    void clearErrorIndicators();
    void setErrorIndicator(int fromRow, int fromIndex, int toRow, int toIndex);
    // Set error indicator from position to end of line
    void setErrorIndicator(int position);
    void openFindReplaceDialog();

protected:
    void dropEvent(QDropEvent* e);

    void setupSyntaxHighlightingFormat(const QString& settings_name, int style);

    int errorIndicatorNumber;
    bool showErrorIndicators;
    FindReplaceDialog* findReplaceDialog;

private slots:
    void updateLineNumberAreaWidth();
    void showContextMenu(const QPoint &pos);
};

#endif
