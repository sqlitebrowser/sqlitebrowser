#ifndef SQLTEXTEDIT_H
#define SQLTEXTEDIT_H

#include "ExtendedScintilla.h"

class FindReplaceDialog;
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
    ~SqlTextEdit() override;

    static SqlUiLexer* sqlLexer;

public slots:
    void reloadSettings();
    void toggleBlockComment();

};

#endif
