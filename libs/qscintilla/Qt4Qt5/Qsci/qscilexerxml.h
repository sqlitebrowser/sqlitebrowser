// This defines the interface to the QsciLexerXML class.
//
// Copyright (c) 2019 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the Free Software Foundation and appearing in
// the file LICENSE included in the packaging of this file.  Please review the
// following information to ensure the GNU General Public License version 3.0
// requirements will be met: http://www.gnu.org/copyleft/gpl.html.
// 
// If you do not wish to use this file under the terms of the GPL version 3.0
// then you may purchase a commercial license.  For more information contact
// info@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#ifndef QSCILEXERXML_H
#define QSCILEXERXML_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexerhtml.h>


//! \brief The QsciLexerXML class encapsulates the Scintilla XML lexer.
class QSCINTILLA_EXPORT QsciLexerXML : public QsciLexerHTML
{
    Q_OBJECT

public:
    //! Construct a QsciLexerXML with parent \a parent.  \a parent is typically
    //! the QsciScintilla instance.
    QsciLexerXML(QObject *parent = 0);

    //! Destroys the QsciLexerXML instance.
    virtual ~QsciLexerXML();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the end-of-line fill for style number \a style.
    bool defaultEolFill(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

    //! Returns the background colour of the text for style number \a style.
    //!
    //! \sa defaultColor()
    QColor defaultPaper(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Causes all properties to be refreshed by emitting the
    //! propertyChanged() signal as required.
    void refreshProperties();

    //! If \a allowed is true then scripts are styled.  The default is true.
    //!
    //! \sa scriptsStyled()
    void setScriptsStyled(bool styled);

    //! Returns true if scripts are styled.
    //!
    //! \sa setScriptsStyled()
    bool scriptsStyled() const;

protected:
    //! The lexer's properties are read from the settings \a qs.  \a prefix
    //! (which has a trailing '/') should be used as a prefix to the key of
    //! each setting.  true is returned if there is no error.
    //!
    bool readProperties(QSettings &qs, const QString &prefix);

    //! The lexer's properties are written to the settings \a qs.
    //! \a prefix (which has a trailing '/') should be used as a prefix to
    //! the key of each setting.  true is returned if there is no error.
    //!
    bool writeProperties(QSettings &qs, const QString &prefix) const;

private:
    void setScriptsProp();

    bool scripts;

    QsciLexerXML(const QsciLexerXML &);
    QsciLexerXML &operator=(const QsciLexerXML &);
};

#endif
