// This defines the interface to the abstract QsciLexerAsm class.
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


#ifndef QSCILEXERASM_H
#define QSCILEXERASM_H

#include <QObject>
#include <QChar>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The abstract QsciLexerAsm class encapsulates the Scintilla Asm
//! lexer.
class QSCINTILLA_EXPORT QsciLexerAsm : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Asm lexer.
    enum {
        //! The default.
        Default = 0,

        //! A comment.
        Comment = 1,

        //! A number.
        Number = 2,

        //! A double-quoted string.
        DoubleQuotedString = 3,

        //! An operator.
        Operator = 4,

        //! An identifier.
        Identifier = 5,

        //! A CPU instruction.
        CPUInstruction = 6,

        //! An FPU instruction.
        FPUInstruction = 7,

        //! A register.
        Register = 8,

        //! A directive.
        Directive = 9,

        //! A directive operand.
        DirectiveOperand = 11,

        //! A block comment.
        BlockComment = 12,

        //! A single-quoted string.
        SingleQuotedString = 13,

        //! The end of a line where a string is not closed.
        UnclosedString = 14,

        //! An extended instruction.
        ExtendedInstruction = 16,

        //! A comment directive.
        CommentDirective = 17,
    };

    //! Construct a QsciLexerAsm with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerAsm(QObject *parent = 0);

    //! Destroys the QsciLexerAsm instance.
    virtual ~QsciLexerAsm();

    //! Returns the foreground colour of the text for style number \a style.
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
    //! CPU instructions.  Set 2 is normally used for FPU instructions.  Set 3
    //! is normally used for register names.  Set 4 is normally used for
    //! directives.  Set 5 is normally used for directive operands.  Set 6 is
    //! normally used for extended instructions.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

    //! Causes all properties to be refreshed by emitting the propertyChanged()
    //! signal as required.
    void refreshProperties();

    //! Returns true if multi-line comment blocks can be folded.
    //!
    //! \sa setFoldComments()
    bool foldComments() const;

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const;

    //! Returns the delimiter used by the COMMENT directive.
    //!
    //! \sa setCommentDelimiter()
    QChar commentDelimiter() const;

    //! Returns true if syntax-based folding is enabled.
    //!
    //! \sa setFoldSyntaxBased()
    bool foldSyntaxBased() const;

public slots:
    //! If \a fold is true then multi-line comment blocks can be folded.
    //! The default is true.
    //!
    //! \sa foldComments()
    virtual void setFoldComments(bool fold);

    //! If \a fold is true then trailing blank lines are included in a fold
    //! block. The default is true.
    //!
    //! \sa foldCompact()
    virtual void setFoldCompact(bool fold);

    //! \a delimiter is the character used for the COMMENT directive's
    //! delimiter.  The default is '~'.
    //!
    //! \sa commentDelimiter()
    virtual void setCommentDelimiter(QChar delimeter);

    //! If \a syntax_based is true then syntax-based folding is enabled.  The
    //! default is true.
    //!
    //! \sa foldSyntaxBased()
    virtual void setFoldSyntaxBased(bool syntax_based);

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
    void setCommentProp();
    void setCompactProp();
    void setCommentDelimiterProp();
    void setSyntaxBasedProp();

    bool fold_comments;
    bool fold_compact;
    QChar comment_delimiter;
    bool fold_syntax_based;

    QsciLexerAsm(const QsciLexerAsm &);
    QsciLexerAsm &operator=(const QsciLexerAsm &);
};

#endif
