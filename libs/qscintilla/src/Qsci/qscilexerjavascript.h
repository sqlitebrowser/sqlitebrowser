// This defines the interface to the QsciLexerJavaScript class.
//
// Copyright (c) 2023 Riverbank Computing Limited <info@riverbankcomputing.com>
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


#ifndef QSCILEXERJSCRIPT_H
#define QSCILEXERJSCRIPT_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexercpp.h>


//! \brief The QsciLexerJavaScript class encapsulates the Scintilla JavaScript
//! lexer.
class QSCINTILLA_EXPORT QsciLexerJavaScript : public QsciLexerCPP
{
    Q_OBJECT

public:
    //! Construct a QsciLexerJavaScript with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerJavaScript(QObject *parent = 0);

    //! Destroys the QsciLexerJavaScript instance.
    virtual ~QsciLexerJavaScript();

    //! Returns the name of the language.
    const char *language() const;

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

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    friend class QsciLexerHTML;

    static const char *keywordClass;

    QsciLexerJavaScript(const QsciLexerJavaScript &);
    QsciLexerJavaScript &operator=(const QsciLexerJavaScript &);
};

#endif
