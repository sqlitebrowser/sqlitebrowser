// This defines the interface to the QsciLexerTekHex class.
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


#ifndef QSCILEXERTEKHEX_H
#define QSCILEXERTEKHEX_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexerhex.h>


//! \brief The QsciLexerTekHex class encapsulates the Scintilla Tektronix Hex
//! lexer.
class QSCINTILLA_EXPORT QsciLexerTekHex : public QsciLexerHex
{
    Q_OBJECT

public:
    //! Construct a QsciLexerTekHex with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerTekHex(QObject *parent = 0);

    //! Destroys the QsciLexerTekHex instance.
    virtual ~QsciLexerTekHex();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.
    const char *lexer() const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    QsciLexerTekHex(const QsciLexerTekHex &);
    QsciLexerTekHex &operator=(const QsciLexerTekHex &);
};

#endif
