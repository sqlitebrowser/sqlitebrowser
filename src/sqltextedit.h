#ifndef SQLTEXTEDIT_H
#define SQLTEXTEDIT_H

#include "Qsci/qsciscintilla.h"

class SqlUiLexer;

/**
 * @brief The SqlTextEdit class
 * This class is based on the QScintilla widget
 */
class SqlTextEdit : public QsciScintilla
{
    Q_OBJECT

public:
    explicit SqlTextEdit(QWidget *parent = nullptr);
    virtual ~SqlTextEdit();

    bool findText(QString text, bool regexp, bool caseSensitive, bool words, bool wrap, bool forward);
    void clearSelection();
    static SqlUiLexer* sqlLexer;

public slots:
    void reloadKeywords();
    void reloadSettings();
    void clearErrorIndicators();
    void setErrorIndicator(int fromRow, int fromIndex, int toRow, int toIndex);

protected:
    void dropEvent(QDropEvent* e);

private:
    void setupSyntaxHighlightingFormat(const QString& settings_name, int style);

    int errorIndicatorNumber;
    bool showErrorIndicators;

private slots:
    void updateLineNumberAreaWidth();
    void openFindReplaceDialog();
};

#endif
