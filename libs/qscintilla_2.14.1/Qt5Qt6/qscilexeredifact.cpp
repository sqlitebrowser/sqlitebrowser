// This module implements the QsciLexerEDIFACT class.
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


#include "Qsci/qscilexeredifact.h"


// The ctor.
QsciLexerEDIFACT::QsciLexerEDIFACT(QObject *parent)
    : QsciLexer(parent)
{
}


// The dtor.
QsciLexerEDIFACT::~QsciLexerEDIFACT()
{
}


// Returns the language name.
const char *QsciLexerEDIFACT::language() const
{
    return "EDIFACT";
}


// Returns the lexer name.
const char *QsciLexerEDIFACT::lexer() const
{
    return "edifact";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerEDIFACT::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80, 0x80, 0x80);

    case SegmentStart:
        return QColor(0x00, 0x00, 0xcb);

    case SegmentEnd:
        return QColor(0xff, 0x8d, 0xb1);

    case ElementSeparator:
        return QColor(0xff, 0x8d, 0xb1);

    case CompositeSeparator:
        return QColor(0x80, 0x80, 0x00);

    case ReleaseSeparator:
        return QColor(0x5e, 0x5e, 0x5e);

    case UNASegmentHeader:
        return QColor(0x00, 0x80, 0x00);

    case UNHSegmentHeader:
        return QColor(0x2f, 0x8b, 0xbd);

    case BadSegment:
        return QColor(0x80, 0x00, 0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the user name of a style.
QString QsciLexerEDIFACT::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case SegmentStart:
        return tr("Segment start");

    case SegmentEnd:
        return tr("Segment end");

    case ElementSeparator:
        return tr("Element separator");

    case CompositeSeparator:
        return tr("Composite separator");

    case ReleaseSeparator:
        return tr("Release separator");

    case UNASegmentHeader:
        return tr("UNA segment header");

    case UNHSegmentHeader:
        return tr("UNH segment header");

    case BadSegment:
        return tr("Badly formed segment");
    }

    return QString();
}
