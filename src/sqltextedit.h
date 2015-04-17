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

    int getErrorIndicatorNumber() const { return errorIndicatorNumber; }

    static SqlUiLexer* sqlLexer;

public slots:
    void reloadKeywords();

protected:
    void dropEvent(QDropEvent* e);

private:
    void setupSyntaxHighlightingFormat(const QString& settings_name, int style);

    int errorIndicatorNumber;

private slots:
    void updateLineNumberAreaWidth();
};

#endif
