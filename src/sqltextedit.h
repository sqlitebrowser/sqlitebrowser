#ifndef SQLTEXTEDIT_H
#define SQLTEXTEDIT_H

#include "ExtendedScintilla.h"

class SqlUiLexer;

/**
 * @brief The SqlTextEdit class
 * This class is based on the QScintilla widget
 */
class SqlTextEdit : public ExtendedScintilla
{
    Q_OBJECT

public:
    explicit SqlTextEdit(QWidget *parent = nullptr);

    static SqlUiLexer* sqlLexer;

    // Override parent setReadOnly
    void setReadOnly(bool ro) override;

public slots:
    void reloadSettings();
    void toggleBlockComment();
    // Comment or uncomment the lines from lineFrom to lineTo (both included) as a single undo action.
    // The first of them decides whether all of them get commented or uncommented.
    void toggleLineComments(int lineFrom, int lineTo);

protected:
    // Whether the setting to comment a line by clicking its number applies to this editor. Subclasses which don't hold SQL code turn it off.
    bool commentOnMarginClickApplies;

private:
    void updateMarginSensitivity();
};

#endif
