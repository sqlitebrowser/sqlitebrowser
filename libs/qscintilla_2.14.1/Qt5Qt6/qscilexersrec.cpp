// This module implements the QsciLexerSRec class.
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


#include "Qsci/qscilexersrec.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
QsciLexerSRec::QsciLexerSRec(QObject *parent)
    : QsciLexerHex(parent)
{
}


// The dtor.
QsciLexerSRec::~QsciLexerSRec()
{
}


// Returns the language name.
const char *QsciLexerSRec::language() const
{
    return "S-Record";
}


// Returns the lexer name.
const char *QsciLexerSRec::lexer() const
{
    return "srec";
}


// Returns the user name of a style.
QString QsciLexerSRec::description(int style) const
{
    // Handle unsupported styles.
    if (style == ExtendedAddress)
        return QString();

    return QsciLexerHex::description(style);
}
