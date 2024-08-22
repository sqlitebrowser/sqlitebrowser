// This defines the interface to the QsciLexerOctave class.
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


#ifndef QSCILEXEROCTAVE_H
#define QSCILEXEROCTAVE_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexermatlab.h>


//! \brief The QsciLexerOctave class encapsulates the Scintilla Octave file
//! lexer.
class QSCINTILLA_EXPORT QsciLexerOctave : public QsciLexerMatlab
{
    Q_OBJECT

public:
    //! Construct a QsciLexerOctave with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerOctave(QObject *parent = 0);

    //! Destroys the QsciLexerOctave instance.
    virtual ~QsciLexerOctave();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

private:
    QsciLexerOctave(const QsciLexerOctave &);
    QsciLexerOctave &operator=(const QsciLexerOctave &);
};

#endif
