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
    explicit SqlTextEdit(QWidget *parent = 0);
    virtual ~SqlTextEdit();

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
};

#endif
