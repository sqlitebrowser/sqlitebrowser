// This defines the interface to the QsciLexerMatlab class.
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


#ifndef QSCILEXERMATLAB_H
#define QSCILEXERMATLAB_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The QsciLexerMatlab class encapsulates the Scintilla Matlab file
//! lexer.
class QSCINTILLA_EXPORT QsciLexerMatlab : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Matlab file lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A command.
        Command = 2,

        //! A number.
        Number = 3,

        //! A keyword.
        Keyword = 4,

        //! A single quoted string.
        SingleQuotedString = 5,

        //! An operator
        Operator = 6,

        //! An identifier.
        Identifier = 7,

        //! A double quoted string.
        DoubleQuotedString = 8
    };

    //! Construct a QsciLexerMatlab with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerMatlab(QObject *parent = 0);

    //! Destroys the QsciLexerMatlab instance.
    virtual ~QsciLexerMatlab();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    QsciLexerMatlab(const QsciLexerMatlab &);
    QsciLexerMatlab &operator=(const QsciLexerMatlab &);
};

#endif
