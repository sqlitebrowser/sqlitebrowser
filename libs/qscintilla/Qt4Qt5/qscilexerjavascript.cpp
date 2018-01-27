// This module implements the QsciLexerJavaScript class.
//
// Copyright (c) 2017 Riverbank Computing Limited <info@riverbankcomputing.com>
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


#include "Qsci/qscilexerjavascript.h"

#include <qcolor.h>
#include <qfont.h>


// The list of JavaScript keywords that can be used by other friendly lexers.
const char *QsciLexerJavaScript::keywordClass =
    "abstract boolean break byte case catch char class const continue "
    "debugger default delete do double else enum export extends final "
    "finally float for function goto if implements import in instanceof "
    "int interface long native new package private protected public "
    "return short static super switch synchronized this throw throws "
    "transient try typeof var void volatile while with";


// The ctor.
QsciLexerJavaScript::QsciLexerJavaScript(QObject *parent)
    : QsciLexerCPP(parent)
{
}


// The dtor.
QsciLexerJavaScript::~QsciLexerJavaScript()
{
}


// Returns the language name.
const char *QsciLexerJavaScript::language() const
{
    return "JavaScript";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerJavaScript::defaultColor(int style) const
{
    if (style == Regex)
        return QColor(0x3f,0x7f,0x3f);

    return QsciLexerCPP::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerJavaScript::defaultEolFill(int style) const
{
    if (style == Regex)
        return true;

    return QsciLexerCPP::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerJavaScript::defaultFont(int style) const
{
    if (style == Regex)
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
const char *QsciLexerJavaScript::keywords(int set) const
{
    if (set != 1)
        return 0;

    return keywordClass;
}


// Returns the user name of a style.
QString QsciLexerJavaScript::description(int style) const
{
    if (style == Regex)
        return tr("Regular expression");

    return QsciLexerCPP::description(style);
}


// Returns the background colour of the text for a style.
QColor QsciLexerJavaScript::defaultPaper(int style) const
{
    if (style == Regex)
        return QColor(0xe0,0xf0,0xff);

    return QsciLexer::defaultPaper(style);
}
