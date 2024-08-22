// This module implements the QsciLexerTekHex class.
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


#include "Qsci/qscilexertekhex.h"


// The ctor.
QsciLexerTekHex::QsciLexerTekHex(QObject *parent)
    : QsciLexerHex(parent)
{
}


// The dtor.
QsciLexerTekHex::~QsciLexerTekHex()
{
}


// Returns the language name.
const char *QsciLexerTekHex::language() const
{
    return "Tektronix-Hex";
}


// Returns the lexer name.
const char *QsciLexerTekHex::lexer() const
{
    return "tehex";
}


// Returns the user name of a style.
QString QsciLexerTekHex::description(int style) const
{
    // Handle unsupported styles.
    if (style == NoAddress || style == RecordCount || style == ExtendedAddress || style == UnknownData)
        return QString();

    return QsciLexerHex::description(style);
}
