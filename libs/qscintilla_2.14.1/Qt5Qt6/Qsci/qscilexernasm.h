// This defines the interface to the QsciLexerNASM class.
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


#ifndef QSCILEXERNASM_H
#define QSCILEXERNASM_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexerasm.h>


//! \brief The QsciLexerNASM class encapsulates the Scintilla NASM lexer.
class QSCINTILLA_EXPORT QsciLexerNASM : public QsciLexerAsm
{
    Q_OBJECT

public:
    //! Construct a QsciLexerNASM with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerNASM(QObject *parent = 0);

    //! Destroys the QsciLexerNASM instance.
    virtual ~QsciLexerNASM();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.
    const char *lexer() const;

private:
    QsciLexerNASM(const QsciLexerNASM &);
    QsciLexerNASM &operator=(const QsciLexerNASM &);
};

#endif
