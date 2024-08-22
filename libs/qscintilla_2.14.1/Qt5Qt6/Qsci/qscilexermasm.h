// This defines the interface to the QsciLexerMASM class.
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


#ifndef QSCILEXERMASM_H
#define QSCILEXERMASM_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexerasm.h>


//! \brief The QsciLexerMASM class encapsulates the Scintilla MASM lexer.
class QSCINTILLA_EXPORT QsciLexerMASM : public QsciLexerAsm
{
    Q_OBJECT

public:
    //! Construct a QsciLexerMASM with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerMASM(QObject *parent = 0);

    //! Destroys the QsciLexerMASM instance.
    virtual ~QsciLexerMASM();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.
    const char *lexer() const;

private:
    QsciLexerMASM(const QsciLexerMASM &);
    QsciLexerMASM &operator=(const QsciLexerMASM &);
};

#endif
