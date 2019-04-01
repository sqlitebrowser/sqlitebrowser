// This defines the interface to the QsciLexerPython class.
//
// Copyright (c) 2019 Riverbank Computing Limited <info@riverbankcomputing.com>
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


#ifndef QSCILEXERPYTHON_H
#define QSCILEXERPYTHON_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>
#include "Qsci/qsciscintillabase.h"


//! \brief The QsciLexerPython class encapsulates the Scintilla Python lexer.
class QSCINTILLA_EXPORT QsciLexerPython : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Python lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A number.
        Number = 2,

        //! A double-quoted string.
        DoubleQuotedString = 3,

        //! A single-quoted string.
        SingleQuotedString = 4,

        //! A keyword.
        Keyword = 5,

        //! A triple single-quoted string.
        TripleSingleQuotedString = 6,

        //! A triple double-quoted string.
        TripleDoubleQuotedString = 7,

        //! The name of a class.
        ClassName = 8,

        //! The name of a function or method.
        FunctionMethodName = 9,

        //! An operator.
        Operator = 10,

        //! An identifier
        Identifier = 11,

        //! A comment block.
        CommentBlock = 12,

        //! The end of a line where a string is not closed.
        UnclosedString = 13,

        //! A highlighted identifier.  These are defined by keyword set
        //! 2.  Reimplement keywords() to define keyword set 2.
        HighlightedIdentifier = 14,

        //! A decorator.
        Decorator = 15,

        //! A double-quoted f-string.
        DoubleQuotedFString = 16,

        //! A single-quoted f-string.
        SingleQuotedFString = 17,

        //! A triple single-quoted f-string.
        TripleSingleQuotedFString = 18,

        //! A triple double-quoted f-string.
        TripleDoubleQuotedFString = 19,
    };

    //! This enum defines the different conditions that can cause
    //! indentations to be displayed as being bad.
    enum IndentationWarning {
        //! Bad indentation is not displayed differently.
        NoWarning = 0,

        //! The indentation is inconsistent when compared to the
        //! previous line, ie. it is made up of a different combination
        //! of tabs and/or spaces.
        Inconsistent = 1,

        //! The indentation is made up of spaces followed by tabs.
        TabsAfterSpaces = 2,

        //! The indentation contains spaces.
        Spaces = 3,

        //! The indentation contains tabs.
        Tabs = 4
    };

    //! Construct a QsciLexerPython with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerPython(QObject *parent = 0);

    //! Destroys the QsciLexerPython instance.
    virtual ~QsciLexerPython();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! \internal Returns the character sequences that can separate
    //! auto-completion words.
    QStringList autoCompletionWordSeparators() const;

    //! \internal Returns the number of lines prior to the current one when
    //! determining the scope of a block when auto-indenting.
    int blockLookback() const;

    //! \internal Returns a space separated list of words or characters in
    //! a particular style that define the start of a block for
    //! auto-indentation.  The styles is returned via \a style.
    const char *blockStart(int *style = 0) const;

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

    //! \internal Returns the view used for indentation guides.
    virtual int indentationGuideView() const;

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

    //! Returns true if indented comment blocks can be folded.
    //!
    //! \sa setFoldComments()
    bool foldComments() const {return fold_comments;}

    //! If \a fold is true then trailing blank lines are included in a fold
    //! block. The default is true.
    //!
    //! \sa foldCompact()
    void setFoldCompact(bool fold);

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const {return fold_compact;}

    //! Returns true if triple quoted strings can be folded.
    //!
    //! \sa setFoldQuotes()
    bool foldQuotes() const {return fold_quotes;}

    //! Returns the condition that will cause bad indentations to be
    //! displayed.
    //!
    //! \sa setIndentationWarning()
    QsciLexerPython::IndentationWarning indentationWarning() const {return indent_warn;}

    //! If \a enabled is true then sub-identifiers defined in keyword set 2
    //! will be highlighted.  For example, if it is false and "open" is defined
    //! in keyword set 2 then "foo.open" will not be highlighted.  The default
    //! is true.
    //!
    //! \sa highlightSubidentifiers()
    void setHighlightSubidentifiers(bool enabled);

    //! Returns true if string literals are allowed to span newline characters.
    //!
    //! \sa setHighlightSubidentifiers()
    bool highlightSubidentifiers() const {return highlight_subids;}

    //! If \a allowed is true then string literals are allowed to span newline
    //! characters.  The default is false.
    //!
    //! \sa stringsOverNewlineAllowed()
    void setStringsOverNewlineAllowed(bool allowed);

    //! Returns true if string literals are allowed to span newline characters.
    //!
    //! \sa setStringsOverNewlineAllowed()
    bool stringsOverNewlineAllowed() const {return strings_over_newline;}

    //! If \a allowed is true then Python v2 unicode string literals (e.g.
    //! u"utf8") are allowed.  The default is true.
    //!
    //! \sa v2UnicodeAllowed()
    void setV2UnicodeAllowed(bool allowed);

    //! Returns true if Python v2 unicode string literals (e.g. u"utf8") are
    //! allowed.
    //!
    //! \sa setV2UnicodeAllowed()
    bool v2UnicodeAllowed() const {return v2_unicode;}

    //! If \a allowed is true then Python v3 binary and octal literals (e.g.
    //! 0b1011, 0o712) are allowed.  The default is true.
    //!
    //! \sa v3BinaryOctalAllowed()
    void setV3BinaryOctalAllowed(bool allowed);

    //! Returns true if Python v3 binary and octal literals (e.g. 0b1011,
    //! 0o712) are allowed.
    //!
    //! \sa setV3BinaryOctalAllowed()
    bool v3BinaryOctalAllowed() const {return v3_binary_octal;}

    //! If \a allowed is true then Python v3 bytes string literals (e.g.
    //! b"bytes") are allowed.  The default is true.
    //!
    //! \sa v3BytesAllowed()
    void setV3BytesAllowed(bool allowed);

    //! Returns true if Python v3 bytes string literals (e.g. b"bytes") are
    //! allowed.
    //!
    //! \sa setV3BytesAllowed()
    bool v3BytesAllowed() const {return v3_bytes;}

public slots:
    //! If \a fold is true then indented comment blocks can be folded.  The
    //! default is false.
    //!
    //! \sa foldComments()
    virtual void setFoldComments(bool fold);

    //! If \a fold is true then triple quoted strings can be folded.  The
    //! default is false.
    //!
    //! \sa foldQuotes()
    virtual void setFoldQuotes(bool fold);

    //! Sets the condition that will cause bad indentations to be
    //! displayed.
    //!
    //! \sa indentationWarning()
    virtual void setIndentationWarning(QsciLexerPython::IndentationWarning warn);

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
    void setCommentProp();
    void setCompactProp();
    void setQuotesProp();
    void setTabWhingeProp();
    void setStringsOverNewlineProp();
    void setV2UnicodeProp();
    void setV3BinaryOctalProp();
    void setV3BytesProp();
    void setHighlightSubidsProp();

    bool fold_comments;
    bool fold_compact;
    bool fold_quotes;
    IndentationWarning indent_warn;
    bool strings_over_newline;
    bool v2_unicode;
    bool v3_binary_octal;
    bool v3_bytes;
    bool highlight_subids;

    friend class QsciLexerHTML;

    static const char *keywordClass;

    QsciLexerPython(const QsciLexerPython &);
    QsciLexerPython &operator=(const QsciLexerPython &);
};

#endif
