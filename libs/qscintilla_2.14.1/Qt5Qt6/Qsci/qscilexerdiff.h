// This defines the interface to the QsciLexerDiff class.
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


#ifndef QSCILEXERDIFF_H
#define QSCILEXERDIFF_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The QsciLexerDiff class encapsulates the Scintilla Diff
//! lexer.
class QSCINTILLA_EXPORT QsciLexerDiff : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Diff lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A command.
        Command = 2,

        //! A header.
        Header = 3,

        //! A position.
        Position = 4,

        //! A line removed.
        LineRemoved = 5,

        //! A line added.
        LineAdded = 6,

        //! A line changed.
        LineChanged = 7,

        //! An adding patch added.
        AddingPatchAdded = 8,

        //! A removing patch added.
        RemovingPatchAdded = 9,

        //! An adding patch added.
        AddingPatchRemoved = 10,

        //! A removing patch added.
        RemovingPatchRemoved = 11,
    };

    //! Construct a QsciLexerDiff with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerDiff(QObject *parent = 0);

    //! Destroys the QsciLexerDiff instance.
    virtual ~QsciLexerDiff();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! Returns the string of characters that comprise a word.
    const char *wordCharacters() const;

    //! Returns the foreground colour of the text for style number \a style.
    QColor defaultColor(int style) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

private:
    QsciLexerDiff(const QsciLexerDiff &);
    QsciLexerDiff &operator=(const QsciLexerDiff &);
};

#endif
