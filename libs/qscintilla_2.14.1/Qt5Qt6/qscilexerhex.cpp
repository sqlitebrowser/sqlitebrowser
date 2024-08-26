// This module implements the abstract QsciLexerHex class.
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


#include "Qsci/qscilexerhex.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
QsciLexerHex::QsciLexerHex(QObject *parent)
    : QsciLexer(parent)
{
}


// The dtor.
QsciLexerHex::~QsciLexerHex()
{
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerHex::defaultColor(int style) const
{
    switch (style)
    {
    case RecordStart:
    case RecordType:
    case UnknownRecordType:
        return QColor(0x7f, 0x00, 0x00);

    case ByteCount:
        return QColor(0x7f, 0x7f, 0x00);

    case IncorrectByteCount:
    case IncorrectChecksum:
        return QColor(0xff, 0xff, 0x00);

    case NoAddress:
    case RecordCount:
        return QColor(0x7f, 0x00, 0xff);

    case DataAddress:
    case StartAddress:
    case ExtendedAddress:
        return QColor(0x00, 0x7f, 0xff);

    case Checksum:
        return QColor(0x00, 0xbf, 0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont QsciLexerHex::defaultFont(int style) const
{
    QFont f = QsciLexer::defaultFont(style);

    if (style == UnknownRecordType || style == UnknownData || style == TrailingGarbage)
        f.setItalic(true);
    else if (style == OddData)
        f.setBold(true);

    return f;
}


// Returns the background colour of the text for a style.
QColor QsciLexerHex::defaultPaper(int style) const
{
    if (style == IncorrectByteCount || style == IncorrectChecksum)
        return QColor(0xff, 0x00, 0x00);

    return QsciLexer::defaultPaper(style);
}


// Returns the user name of a style.
QString QsciLexerHex::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case RecordStart:
        return tr("Record start");

    case RecordType:
        return tr("Record type");

    case UnknownRecordType:
        return tr("Unknown record type");

    case ByteCount:
        return tr("Byte count");

    case IncorrectByteCount:
        return tr("Incorrect byte count");

    case NoAddress:
        return tr("No address");

    case DataAddress:
        return tr("Data address");

    case RecordCount:
        return tr("Record count");

    case StartAddress:
        return tr("Start address");

    case ExtendedAddress:
        return tr("Extended address");

    case OddData:
        return tr("Odd data");

    case EvenData:
        return tr("Even data");

    case UnknownData:
        return tr("Unknown data");

    case Checksum:
        return tr("Checksum");

    case IncorrectChecksum:
        return tr("Incorrect checksum");

    case TrailingGarbage:
        return tr("Trailing garbage after a record");
    }

    return QString();
}
