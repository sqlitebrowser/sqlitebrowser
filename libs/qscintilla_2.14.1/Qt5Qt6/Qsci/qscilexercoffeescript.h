// This defines the interface to the QsciLexerCoffeeScript class.
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


#ifndef QSCILEXERCOFFEESCRIPT_H
#define QSCILEXERCOFFEESCRIPT_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The QsciLexerCoffeeScript class encapsulates the Scintilla
//! CoffeeScript lexer.
class QSCINTILLA_EXPORT QsciLexerCoffeeScript : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! C++ lexer.
    enum {
        //! The default.
        Default = 0,

        //! A C-style comment.
        Comment = 1,

        //! A C++-style comment line.
        CommentLine = 2,

        //! A JavaDoc/Doxygen C-style comment.
        CommentDoc = 3,

        //! A number.
        Number = 4,

        //! A keyword.
        Keyword = 5,

        //! A double-quoted string.
        DoubleQuotedString = 6,

        //! A single-quoted string.
        SingleQuotedString = 7,

        //! An IDL UUID.
        UUID = 8,

        //! A pre-processor block.
        PreProcessor = 9,

        //! An operator.
        Operator = 10,

        //! An identifier
        Identifier = 11,

        //! The end of a line where a string is not closed.
        UnclosedString = 12,

        //! A C# verbatim string.
        VerbatimString = 13,

        //! A regular expression.
        Regex = 14,

        //! A JavaDoc/Doxygen C++-style comment line.
        CommentLineDoc = 15,

        //! A keyword defined in keyword set number 2.  The class must be
        //! sub-classed and re-implement keywords() to make use of this style.
        KeywordSet2 = 16,

        //! A JavaDoc/Doxygen keyword.
        CommentDocKeyword = 17,

        //! A JavaDoc/Doxygen keyword error defined in keyword set number 3.
        //! The class must be sub-classed and re-implement keywords() to make
        //! use of this style.
        CommentDocKeywordError = 18,

        //! A global class defined in keyword set number 4.  The class must be
        //! sub-classed and re-implement keywords() to make use of this style.
        GlobalClass = 19,

        //! A block comment.
        CommentBlock = 22,

        //! A block regular expression.
        BlockRegex = 23,

        //! A block regular expression comment.
        BlockRegexComment = 24,

        //! An instance property.
        InstanceProperty = 25,
    };

    //! Construct a QsciLexerCoffeeScript with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerCoffeeScript(QObject *parent = 0);

    //! Destroys the QsciLexerCoffeeScript instance.
    virtual ~QsciLexerCoffeeScript();

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

    //! Returns the string of characters that comprise a word.
    const char *wordCharacters() const;

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
    //! by the lexer as a space separated string.  Set 1 is normally used for
    //! primary keywords and identifiers.  Set 2 is normally used for secondary
    //! keywords and identifiers.  Set 3 is normally used for documentation
    //! comment keywords.  Set 4 is normally used for global classes and
    //! typedefs.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

    //! Causes all properties to be refreshed by emitting the
    //! propertyChanged() signal as required.
    void refreshProperties();

    //! Returns true if '$' characters are allowed in identifier names.
    //!
    //! \sa setDollarsAllowed()
    bool dollarsAllowed() const {return dollars;}

    //! If \a allowed is true then '$' characters are allowed in identifier
    //! names.  The default is true.
    //!
    //! \sa dollarsAllowed()
    void setDollarsAllowed(bool allowed);

    //! Returns true if multi-line comment blocks can be folded.
    //!
    //! \sa setFoldComments()
    bool foldComments() const {return fold_comments;}

    //! If \a fold is true then multi-line comment blocks can be folded.
    //! The default is false.
    //!
    //! \sa foldComments()
    void setFoldComments(bool fold);

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const {return fold_compact;}

    //! If \a fold is true then trailing blank lines are included in a fold
    //! block. The default is true.
    //!
    //! \sa foldCompact()
    void setFoldCompact(bool fold);

    //! Returns true if preprocessor lines (after the preprocessor
    //! directive) are styled.
    //!
    //! \sa setStylePreprocessor()
    bool stylePreprocessor() const {return style_preproc;}

    //! If \a style is true then preprocessor lines (after the preprocessor
    //! directive) are styled.  The default is false.
    //!
    //! \sa stylePreprocessor()
    void setStylePreprocessor(bool style);

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
    void setStylePreprocProp();
    void setDollarsProp();

    bool fold_comments;
    bool fold_compact;
    bool style_preproc;
    bool dollars;

    QsciLexerCoffeeScript(const QsciLexerCoffeeScript &);
    QsciLexerCoffeeScript &operator=(const QsciLexerCoffeeScript &);
};

#endif
