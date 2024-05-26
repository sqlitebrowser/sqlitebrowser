// This defines the interface to the QsciLexerSQL class.
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


#ifndef QSCILEXERSQL_H
#define QSCILEXERSQL_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The QsciLexerSQL class encapsulates the Scintilla SQL lexer.
class QSCINTILLA_EXPORT QsciLexerSQL : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! SQL lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A line comment.
        CommentLine = 2,

        //! A JavaDoc/Doxygen style comment.
        CommentDoc = 3,

        //! A number.
        Number = 4,

        //! A keyword.
        Keyword = 5,

        //! A double-quoted string.
        DoubleQuotedString = 6,

        //! A single-quoted string.
        SingleQuotedString = 7,

        //! An SQL*Plus keyword.
        PlusKeyword = 8,

        //! An SQL*Plus prompt.
        PlusPrompt = 9,

        //! An operator.
        Operator = 10,

        //! An identifier
        Identifier = 11,

        //! An SQL*Plus comment.
        PlusComment = 13,

        //! A '#' line comment.
        CommentLineHash = 15,

        //! A JavaDoc/Doxygen keyword.
        CommentDocKeyword = 17,

        //! A JavaDoc/Doxygen keyword error.
        CommentDocKeywordError = 18,

        //! A keyword defined in keyword set number 5.  The class must be
        //! sub-classed and re-implement keywords() to make use of this style.
        //! Note that keywords must be defined using lower case.
        KeywordSet5 = 19,

        //! A keyword defined in keyword set number 6.  The class must be
        //! sub-classed and re-implement keywords() to make use of this style.
        //! Note that keywords must be defined using lower case.
        KeywordSet6 = 20,

        //! A keyword defined in keyword set number 7.  The class must be
        //! sub-classed and re-implement keywords() to make use of this style.
        //! Note that keywords must be defined using lower case.
        KeywordSet7 = 21,

        //! A keyword defined in keyword set number 8.  The class must be
        //! sub-classed and re-implement keywords() to make use of this style.
        //! Note that keywords must be defined using lower case.
        KeywordSet8 = 22,

        //! A quoted identifier.
        QuotedIdentifier = 23,

        //! A quoted operator.
        QuotedOperator = 24,
    };

    //! Construct a QsciLexerSQL with parent \a parent.  \a parent is typically
    //! the QsciScintilla instance.
    QsciLexerSQL(QObject *parent = 0);

    //! Destroys the QsciLexerSQL instance.
    virtual ~QsciLexerSQL();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

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

    //! Returns the set of keywords for the keyword set \a set recognised by
    //! the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the style
    //! is invalid for this language then an empty QString is returned.  This
    //! is intended to be used in user preference dialogs.
    QString description(int style) const;

    //! Causes all properties to be refreshed by emitting the
    //! propertyChanged() signal as required.
    void refreshProperties();

    //! Returns true if backslash escapes are enabled.
    //!
    //! \sa setBackslashEscapes()
    bool backslashEscapes() const {return backslash_escapes;}

    //! If \a enable is true then words may contain dots (i.e. periods or full
    //! stops).  The default is false.
    //!
    //! \sa dottedWords()
    void setDottedWords(bool enable);

    //! Returns true if words may contain dots (i.e. periods or full stops).
    //!
    //! \sa setDottedWords()
    bool dottedWords() const {return allow_dotted_word;}

    //! If \a fold is true then ELSE blocks can be folded.  The default is
    //! false.
    //!
    //! \sa foldAtElse()
    void setFoldAtElse(bool fold);

    //! Returns true if ELSE blocks can be folded.
    //!
    //! \sa setFoldAtElse()
    bool foldAtElse() const {return at_else;}

    //! Returns true if multi-line comment blocks can be folded.
    //!
    //! \sa setFoldComments()
    bool foldComments() const {return fold_comments;}

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const {return fold_compact;}

    //! If \a fold is true then only BEGIN blocks can be folded.  The default
    //! is false.
    //!
    //! \sa foldOnlyBegin()
    void setFoldOnlyBegin(bool fold);

    //! Returns true if BEGIN blocks only can be folded.
    //!
    //! \sa setFoldOnlyBegin()
    bool foldOnlyBegin() const {return only_begin;}

    //! If \a enable is true then '#' is used as a comment character.  It is
    //! typically enabled for MySQL and disabled for Oracle.  The default is
    //! false.
    //!
    //! \sa hashComments()
    void setHashComments(bool enable);

    //! Returns true if '#' is used as a comment character.
    //!
    //! \sa setHashComments()
    bool hashComments() const {return numbersign_comment;}

    //! If \a enable is true then quoted identifiers are enabled.  The default
    //! is false.
    //!
    //! \sa quotedIdentifiers()
    void setQuotedIdentifiers(bool enable);

    //! Returns true if quoted identifiers are enabled.
    //!
    //! \sa setQuotedIdentifiers()
    bool quotedIdentifiers() const {return backticks_identifier;}

public slots:
    //! If \a enable is true then backslash escapes are enabled.  The
    //! default is false.
    //!
    //! \sa backslashEscapes()
    virtual void setBackslashEscapes(bool enable);

    //! If \a fold is true then multi-line comment blocks can be folded.  The
    //! default is false.
    //!
    //! \sa foldComments()
    virtual void setFoldComments(bool fold);

    //! If \a fold is true then trailing blank lines are included in a fold
    //! block. The default is true.
    //!
    //! \sa foldCompact()
    virtual void setFoldCompact(bool fold);

protected:
    //! The lexer's properties are read from the settings \a qs.  \a prefix
    //! (which has a trailing '/') should be used as a prefix to the key of
    //! each setting.  true is returned if there is no error.
    //!
    bool readProperties(QSettings &qs, const QString &prefix);

    //! The lexer's properties are written to the settings \a qs.
    //! \a prefix (which has a trailing '/') should be used as a prefix to
    //! the key of each setting.  true is returned if there is no error.
    //!
    bool writeProperties(QSettings &qs, const QString &prefix) const;

private:
    void setAtElseProp();
    void setCommentProp();
    void setCompactProp();
    void setOnlyBeginProp();
    void setBackticksIdentifierProp();
    void setNumbersignCommentProp();
    void setBackslashEscapesProp();
    void setAllowDottedWordProp();

    bool at_else;
    bool fold_comments;
    bool fold_compact;
    bool only_begin;
    bool backticks_identifier;
    bool numbersign_comment;
    bool backslash_escapes;
    bool allow_dotted_word;

    QsciLexerSQL(const QsciLexerSQL &);
    QsciLexerSQL &operator=(const QsciLexerSQL &);
};

#endif
