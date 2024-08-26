// This module implements the QsciLexerCSharp class.
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


#include "Qsci/qscilexercsharp.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
QsciLexerCSharp::QsciLexerCSharp(QObject *parent)
    : QsciLexerCPP(parent)
{
}


// The dtor.
QsciLexerCSharp::~QsciLexerCSharp()
{
}


// Returns the language name.
const char *QsciLexerCSharp::language() const
{
    return "C#";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerCSharp::defaultColor(int style) const
{
    if (style == VerbatimString)
        return QColor(0x00,0x7f,0x00);

    return QsciLexerCPP::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerCSharp::defaultEolFill(int style) const
{
    if (style == VerbatimString)
        return true;

    return QsciLexerCPP::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerCSharp::defaultFont(int style) const
{
    if (style == VerbatimString)
#if defined(Q_OS_WIN)
        return QFont("Courier New",10);
#elif defined(Q_OS_MAC)
        return QFont("Courier", 12);
#else
        return QFont("Bitstream Vera Sans Mono",9);
#endif

    return QsciLexerCPP::defaultFont(style);
}


// Returns the set of keywords.
const char *QsciLexerCSharp::keywords(int set) const
{
    if (set != 1)
        return 0;

    return "abstract as base bool break byte case catch char checked "
           "class const continue decimal default delegate do double else "
           "enum event explicit extern false finally fixed float for "
           "foreach goto if implicit in int interface internal is lock "
           "long namespace new null object operator out override params "
           "private protected public readonly ref return sbyte sealed "
           "short sizeof stackalloc static string struct switch this "
           "throw true try typeof uint ulong unchecked unsafe ushort "
           "using virtual void while";
}


// Returns the user name of a style.
QString QsciLexerCSharp::description(int style) const
{
    if (style == VerbatimString)
        return tr("Verbatim string");

    return QsciLexerCPP::description(style);
}


// Returns the background colour of the text for a style.
QColor QsciLexerCSharp::defaultPaper(int style) const
{
    if (style == VerbatimString)
        return QColor(0xe0,0xff,0xe0);

    return QsciLexer::defaultPaper(style);
}
