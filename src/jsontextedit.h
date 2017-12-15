#ifndef JSONTEXTEDIT_H
#define JSONTEXTEDIT_H

#include "ExtendedScintilla.h"
#include <Qsci/qscilexerjson.h>

/**
 * @brief The JsonTextEdit class
 * This class is based on our Extended QScintilla widget
 */
class JsonTextEdit : public ExtendedScintilla
{
    Q_OBJECT

public:
    explicit JsonTextEdit(QWidget *parent = nullptr);
    virtual ~JsonTextEdit();

    static QsciLexerJSON* jsonLexer;

public slots:
    void reloadSettings();
};

#endif
