// This defines the interface to the QsciLexerPascal class.
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


#ifndef QSCILEXERPASCAL_H
#define QSCILEXERPASCAL_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The QsciLexerPascal class encapsulates the Scintilla Pascal lexer.
class QSCINTILLA_EXPORT QsciLexerPascal : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! C++ lexer.
    enum {
        //! The default.
        Default = 0,

        //! An identifier
        Identifier = 1,

        //! A '{ ... }' style comment.
        Comment = 2,

        //! A '(* ... *)' style comment.
        CommentParenthesis = 3,

        //! A comment line.
        CommentLine = 4,

        //! A '{$ ... }' style pre-processor block.
        PreProcessor = 5,

        //! A '(*$ ... *)' style pre-processor block.
        PreProcessorParenthesis = 6,

        //! A number.
        Number = 7,

        //! A hexadecimal number.
        HexNumber = 8,

        //! A keyword.
        Keyword = 9,

        //! A single-quoted string.
        SingleQuotedString = 10,

        //! The end of a line where a string is not closed.
        UnclosedString = 11,

        //! A character.
        Character = 12,

        //! An operator.
        Operator = 13,

        //! Inline Asm.
        Asm = 14
    };

    //! Construct a QsciLexerPascal with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerPascal(QObject *parent = 0);

    //! Destroys the QsciLexerPascal instance.
    virtual ~QsciLexerPascal();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! \internal Returns the character sequences that can separate
    //! auto-completion words.
    QStringList autoCompletionWordSeparators() const;

    //! \internal Returns a space separated list of words or characters in
    //! a particular style that define the end of a block for
    //! auto-indentation.  The styles is returned via \a style.
    const char *blockEnd(int *style = 0) const;

    //! \internal Returns a space separated list of words or characters in
    //! a particular style that define the start of a block for
    //! auto-indentation.  The styles is returned via \a style.
    const char *blockStart(int *style = 0) const;

    //! \internal Returns a space separated list of keywords in a
    //! particular style that define the start of a block for
    //! auto-indentation.  The styles is returned via \a style.
    const char *blockStartKeyword(int *style = 0) const;

    //! \internal Returns the style used for braces for brace matching.
    int braceStyle() const;

    //! Returns the foreground colour of the text for style number \a style.
    //!
    //! \sa defaultPaper()
    QColor defaultColor(int style) const;

    //! Returns the end-of-line fill for style number \a style.
    bool defaultEolFill(int style) const;

    //! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

    //! Returns the background colour of the text for style number \a style.
    //!
    //! \sa defaultColor()
    QColor defaultPaper(int style) const;

    //! Returns the set of keywords for the keyword set \a set recognised
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

    //! Causes all properties to be refreshed by emitting the
    //! propertyChanged() signal as required.
    void refreshProperties();

    //! Returns true if multi-line comment blocks can be folded.
    //!
    //! \sa setFoldComments()
    bool foldComments() const;

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const;

    //! Returns true if preprocessor blocks can be folded.
    //!
    //! \sa setFoldPreprocessor()
    bool foldPreprocessor() const;

    //! If \a enabled is true then some keywords will only be highlighted in an
    //! appropriate context (similar to how the Delphi IDE works).  The default
    //! is true.
    //!
    //! \sa smartHighlighting()
    void setSmartHighlighting(bool enabled);

    //! Returns true if some keywords will only be highlighted in an
    //! appropriate context (similar to how the Delphi IDE works).
    //!
    //! \sa setSmartHighlighting()
    bool smartHighlighting() const;

public slots:
    //! If \a fold is true then multi-line comment blocks can be folded.
    //! The default is false.
    //!
    //! \sa foldComments()
    virtual void setFoldComments(bool fold);

    //! If \a fold is true then trailing blank lines are included in a fold
    //! block. The default is true.
    //!
    //! \sa foldCompact()
    virtual void setFoldCompact(bool fold);

    //! If \a fold is true then preprocessor blocks can be folded.  The
    //! default is true.
    //!
    //! \sa foldPreprocessor()
    virtual void setFoldPreprocessor(bool fold);

protected:
    //! The lexer's properties are read from the settings \a qs.  \a prefix
    //! (which has a trailing '/') should be used as a prefix to the key of
    //! each setting.  true is returned if there is no error.
    //!
    //! \sa writeProperties()
    bool readProperties(QSettings &qs,const QString &prefix);

    //! The lexer's properties are written to the settings \a qs.
    //! \a prefix (which has a trailing '/') should be used as a prefix to
    //! the key of each setting.  true is returned if there is no error.
    //!
    //! \sa readProperties()
    bool writeProperties(QSettings &qs,const QString &prefix) const;

private:
    void setCommentProp();
    void setCompactProp();
    void setPreprocProp();
    void setSmartHighlightProp();

    bool fold_comments;
    bool fold_compact;
    bool fold_preproc;
    bool smart_highlight;

    QsciLexerPascal(const QsciLexerPascal &);
    QsciLexerPascal &operator=(const QsciLexerPascal &);
};

#endif
