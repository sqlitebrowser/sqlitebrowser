// This defines the interface to the abstract QsciLexerHex class.
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


#ifndef QSCILEXERHEX_H
#define QSCILEXERHEX_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The abstract QsciLexerHex class encapsulates the Scintilla Hex
//! lexer.
class QSCINTILLA_EXPORT QsciLexerHex : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Hex lexer.
    enum {
        //! The default.
        Default = 0,

        //! A record start.
        RecordStart = 1,

        //! A record type.
        RecordType = 2,

        //! An unknown record type.
        UnknownRecordType = 3,

        //! A correct byte count field.
        ByteCount = 4,

        //! An incorrect byte count field.
        IncorrectByteCount = 5,

        //! No address (S-Record and Intel Hex only).
        NoAddress = 6,

        //! A data address.
        DataAddress = 7,

        //! A record count (S-Record only).
        RecordCount = 8,

        //! A start address.
        StartAddress = 9,

        //! An extended address (Intel Hex only).
        ExtendedAddress = 11,

        //! Odd data.
        OddData = 12,

        //! Even data.
        EvenData = 13,

        //! Unknown data (S-Record and Intel Hex only).
        UnknownData = 14,

        //! A correct checksum.
        Checksum = 16,

        //! An incorrect checksum.
        IncorrectChecksum = 17,

        //! Garbage data after the record.
        TrailingGarbage = 18,
    };

    //! Construct a QsciLexerHex with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerHex(QObject *parent = 0);

    //! Destroys the QsciLexerHex instance.
    virtual ~QsciLexerHex();

    //! Returns the foreground colour of the text for style number \a style.
    QColor defaultColor(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

    //! Returns the background colour of the text for style number \a style.
    //!
    //! \sa defaultColor()
    QColor defaultPaper(int style) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    QsciLexerHex(const QsciLexerHex &);
    QsciLexerHex &operator=(const QsciLexerHex &);
};

#endif
