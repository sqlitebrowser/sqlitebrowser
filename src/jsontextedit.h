#ifndef JSONTEXTEDIT_H
#define JSONTEXTEDIT_H

#include "Qsci/qsciscintilla.h"
#include <Qsci/qscilexerjson.h>

/**
 * @brief The JsonTextEdit class
 * This class is based on the QScintilla widget
 */
class JsonTextEdit : public QsciScintilla
{
    Q_OBJECT

public:
    explicit JsonTextEdit(QWidget *parent = nullptr);
    virtual ~JsonTextEdit();

    static QsciLexerJSON* jsonLexer;

public slots:
    void reloadKeywords();
    void reloadSettings();

protected:
    void dropEvent(QDropEvent* e);

private:
    void setupSyntaxHighlightingFormat(const QString& settings_name, int style);

private slots:
    void updateLineNumberAreaWidth();
};

#endif
