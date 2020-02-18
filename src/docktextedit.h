#ifndef DOCKTEXTEDIT_H
#define DOCKTEXTEDIT_H

#include "ExtendedScintilla.h"

class QsciLexerJSON;
class QsciLexerXML;

/**
 * @brief The DockTextEdit class
 * This class is based on our Extended QScintilla widget
 */
class DockTextEdit : public ExtendedScintilla
{
    Q_OBJECT

public:
    explicit DockTextEdit(QWidget *parent = nullptr);

    // Enumeration of supported languages
    enum Language
    {
        PlainText,
        JSON,
        XML
    };

    void setLanguage(Language lang);
    Language language() const { return m_language; }

    // Disables the line-number margin and sets this text in the first line.
    void setTextInMargin(const QString& text);

    // Resets margin to their original line-number mode
    void clearTextInMargin();

public slots:
    void reloadSettings();

protected:
    static QsciLexerJSON* jsonLexer;
    static QsciLexerXML* xmlLexer;
private:
    Language m_language;
    QFont plainTextFont;
};

#endif
