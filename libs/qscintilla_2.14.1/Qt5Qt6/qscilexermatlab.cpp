// This module implements the QsciLexerMatlab class.
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


#include "Qsci/qscilexermatlab.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
QsciLexerMatlab::QsciLexerMatlab(QObject *parent)
    : QsciLexer(parent)
{
}


// The dtor.
QsciLexerMatlab::~QsciLexerMatlab()
{
}


// Returns the language name.
const char *QsciLexerMatlab::language() const
{
    return "Matlab";
}


// Returns the lexer name.
const char *QsciLexerMatlab::lexer() const
{
    return "matlab";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerMatlab::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
    case Operator:
        return QColor(0x00,0x00,0x00);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case Command:
        return QColor(0x7f,0x7f,0x00);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case Keyword:
        return QColor(0x00,0x00,0x7f);

    case SingleQuotedString:
    case DoubleQuotedString:
        return QColor(0x7f,0x00,0x7f);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont QsciLexerMatlab::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case Keyword:
    case Operator:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerMatlab::keywords(int set) const
{
    if (set == 1)
        return
            "break case catch continue else elseif end for function "
            "global if otherwise persistent return switch try while";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerMatlab::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Command:
        return tr("Command");

    case Number:
        return tr("Number");

    case Keyword:
        return tr("Keyword");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case DoubleQuotedString:
        return tr("Double-quoted string");
    }

    return QString();
}
