// This defines the interface to the QsciLexerPerl class.
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


#ifndef QSCILEXERPERL_H
#define QSCILEXERPERL_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The QsciLexerPerl class encapsulates the Scintilla Perl
//! lexer.
class QSCINTILLA_EXPORT QsciLexerPerl : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Perl lexer.
    enum {
        //! The default.
        Default = 0,

        //! An error.
        Error = 1,

        //! A comment.
        Comment = 2,

        //! A POD.
        POD = 3,

        //! A number.
        Number = 4,

        //! A keyword.
        Keyword = 5,

        //! A double-quoted string.
        DoubleQuotedString = 6,

        //! A single-quoted string.
        SingleQuotedString = 7,

        //! An operator.
        Operator = 10,

        //! An identifier
        Identifier = 11,

        //! A scalar.
        Scalar = 12,

        //! An array.
        Array = 13,

        //! A hash.
        Hash = 14,

        //! A symbol table.
        SymbolTable = 15,

        //! A regular expression.
        Regex = 17,

        //! A substitution.
        Substitution = 18,

        //! Backticks.
        Backticks = 20,

        //! A data section.
        DataSection = 21,

        //! A here document delimiter.
        HereDocumentDelimiter = 22,

        //! A single quoted here document.
        SingleQuotedHereDocument = 23,

        //! A double quoted here document.
        DoubleQuotedHereDocument = 24,

        //! A backtick here document.
        BacktickHereDocument = 25,

        //! A quoted string (q).
        QuotedStringQ = 26,

        //! A quoted string (qq).
        QuotedStringQQ = 27,

        //! A quoted string (qx).
        QuotedStringQX = 28,

        //! A quoted string (qr).
        QuotedStringQR = 29,

        //! A quoted string (qw).
        QuotedStringQW = 30,

        //! A verbatim POD.
        PODVerbatim = 31,

        //! A Subroutine prototype.
        SubroutinePrototype = 40,

        //! A format identifier.
        FormatIdentifier = 41,

        //! A format body.
        FormatBody = 42,

        //! A double-quoted string (interpolated variable).
        DoubleQuotedStringVar = 43,

        //! A translation.
        Translation = 44,

        //! A regular expression (interpolated variable).
        RegexVar = 54,

        //! A substitution (interpolated variable).
        SubstitutionVar = 55,

        //! Backticks (interpolated variable).
        BackticksVar = 57,

        //! A double quoted here document (interpolated variable).
        DoubleQuotedHereDocumentVar = 61,

        //! A backtick here document (interpolated variable).
        BacktickHereDocumentVar = 62,

        //! A quoted string (qq, interpolated variable).
        QuotedStringQQVar = 64,

        //! A quoted string (qx, interpolated variable).
        QuotedStringQXVar = 65,

        //! A quoted string (qr, interpolated variable).
        QuotedStringQRVar = 66
    };

    //! Construct a QsciLexerPerl with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerPerl(QObject *parent = 0);

    //! Destroys the QsciLexerPerl instance.
    virtual ~QsciLexerPerl();

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

    //! \internal Returns the style used for braces for brace matching.
    int braceStyle() const;

    //! Returns the string of characters that comprise a word.
    const char *wordCharacters() const;

    //! Returns the foreground colour of the text for style number
    //! \a style.
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

    //! If \a fold is true then "} else {" lines can be folded.  The default is
    //! false.
    //!
    //! \sa foldAtElse()
    void setFoldAtElse(bool fold);

    //! Returns true if "} else {" lines can be folded.
    //!
    //! \sa setFoldAtElse()
    bool foldAtElse() const {return fold_atelse;}

    //! Returns true if multi-line comment blocks can be folded.
    //!
    //! \sa setFoldComments()
    bool foldComments() const;

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const;

    //! If \a fold is true then packages can be folded.  The default is true.
    //!
    //! \sa foldPackages()
    void setFoldPackages(bool fold);

    //! Returns true if packages can be folded.
    //!
    //! \sa setFoldPackages()
    bool foldPackages() const;

    //! If \a fold is true then POD blocks can be folded.  The default is true.
    //!
    //! \sa foldPODBlocks()
    void setFoldPODBlocks(bool fold);

    //! Returns true if POD blocks can be folded.
    //!
    //! \sa setFoldPODBlocks()
    bool foldPODBlocks() const;

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

protected:
    //! The lexer's properties are read from the settings \a qs.  \a prefix
    //! (which has a trailing '/') should be used as a prefix to the key of
    //! each setting.  true is returned if there is no error.
    //!
    bool readProperties(QSettings &qs,const QString &prefix);

    //! The lexer's properties are written to the settings \a qs.
    //! \a prefix (which has a trailing '/') should be used as a prefix to
    //! the key of each setting.  true is returned if there is no error.
    //!
    bool writeProperties(QSettings &qs,const QString &prefix) const;

private:
    void setAtElseProp();
    void setCommentProp();
    void setCompactProp();
    void setPackagesProp();
    void setPODBlocksProp();

    bool fold_atelse;
    bool fold_comments;
    bool fold_compact;
    bool fold_packages;
    bool fold_pod_blocks;

    QsciLexerPerl(const QsciLexerPerl &);
    QsciLexerPerl &operator=(const QsciLexerPerl &);
};

#endif
