// This defines the interface to the QsciLexerIDL class.
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


#ifndef QSCILEXERIDL_H
#define QSCILEXERIDL_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexercpp.h>


//! \brief The QsciLexerIDL class encapsulates the Scintilla IDL
//! lexer.
class QSCINTILLA_EXPORT QsciLexerIDL : public QsciLexerCPP
{
    Q_OBJECT

public:
    //! Construct a QsciLexerIDL with parent \a parent.  \a parent is typically
    //! the QsciScintilla instance.
    QsciLexerIDL(QObject *parent = 0);

    //! Destroys the QsciLexerIDL instance.
    virtual ~QsciLexerIDL();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the foreground colour of the text for style number \a style.
    QColor defaultColor(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    QsciLexerIDL(const QsciLexerIDL &);
    QsciLexerIDL &operator=(const QsciLexerIDL &);
};

#endif
