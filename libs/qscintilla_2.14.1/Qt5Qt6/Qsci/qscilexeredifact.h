// This defines the interface to the QsciLexerEDIFACT class.
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


#ifndef QSCILEXEREDIFACT_H
#define QSCILEXEREDIFACT_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The QsciLexerEDIFACT class encapsulates the Scintilla EDIFACT lexer.
class QSCINTILLA_EXPORT QsciLexerEDIFACT : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! EDIFACT lexer.
    enum {
        //! The default.
        Default = 0,

        //! A segment start.
        SegmentStart = 1,

        //! A segment end.
        SegmentEnd = 2,

        //! An element separator.
        ElementSeparator = 3,

        //! A composite separator.
        CompositeSeparator = 4,

        //! A release separator.
        ReleaseSeparator = 5,

        //! A UNA segment header.
        UNASegmentHeader = 6,

        //! A UNH segment header.
        UNHSegmentHeader = 7,

        //! A bad segment.
        BadSegment = 8,
    };

    //! Construct a QsciLexerEDIFACT with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerEDIFACT(QObject *parent = 0);

    //! Destroys the QsciLexerEDIFACT instance.
    virtual ~QsciLexerEDIFACT();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    QsciLexerEDIFACT(const QsciLexerEDIFACT &);
    QsciLexerEDIFACT &operator=(const QsciLexerEDIFACT &);
};

#endif
