// This defines the interface to the QsciLexerFortran class.
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


#ifndef QSCILEXERFORTRAN_H
#define QSCILEXERFORTRAN_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexerfortran77.h>


//! \brief The QsciLexerFortran class encapsulates the Scintilla Fortran lexer.
class QSCINTILLA_EXPORT QsciLexerFortran : public QsciLexerFortran77
{
    Q_OBJECT

public:
    //! Construct a QsciLexerFortran with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerFortran(QObject *parent = 0);

    //! Destroys the QsciLexerFortran instance.
    virtual ~QsciLexerFortran();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

private:
    QsciLexerFortran(const QsciLexerFortran &);
    QsciLexerFortran &operator=(const QsciLexerFortran &);
};

#endif
