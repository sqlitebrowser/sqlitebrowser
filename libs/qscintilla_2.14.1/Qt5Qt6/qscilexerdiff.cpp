// This module implements the QsciLexerDiff class.
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


#include "Qsci/qscilexerdiff.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerDiff::QsciLexerDiff(QObject *parent)
    : QsciLexer(parent)
{
}


// The dtor.
QsciLexerDiff::~QsciLexerDiff()
{
}


// Returns the language name.
const char *QsciLexerDiff::language() const
{
    return "Diff";
}


// Returns the lexer name.
const char *QsciLexerDiff::lexer() const
{
    return "diff";
}


// Return the string of characters that comprise a word.
const char *QsciLexerDiff::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerDiff::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x00,0x00,0x00);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case Command:
        return QColor(0x7f,0x7f,0x00);

    case Header:
        return QColor(0x7f,0x00,0x00);

    case Position:
        return QColor(0x7f,0x00,0x7f);

    case LineRemoved:
    case AddingPatchRemoved:
    case RemovingPatchRemoved:
        return QColor(0x00,0x7f,0x7f);

    case LineAdded:
    case AddingPatchAdded:
    case RemovingPatchAdded:
        return QColor(0x00,0x00,0x7f);

    case LineChanged:
        return QColor(0x7f,0x7f,0x7f);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the user name of a style.
QString QsciLexerDiff::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Command:
        return tr("Command");

    case Header:
        return tr("Header");

    case Position:
        return tr("Position");

    case LineRemoved:
        return tr("Removed line");

    case LineAdded:
        return tr("Added line");

    case LineChanged:
        return tr("Changed line");

    case AddingPatchAdded:
        return tr("Added adding patch");

    case RemovingPatchAdded:
        return tr("Removed adding patch");

    case AddingPatchRemoved:
        return tr("Added removing patch");

    case RemovingPatchRemoved:
        return tr("Removed removing patch");
    }

    return QString();
}
