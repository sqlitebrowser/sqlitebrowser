// This defines the interface to the QsciLexerVerilog class.
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


#ifndef QSCILEXERVERILOG_H
#define QSCILEXERVERILOG_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The QsciLexerVerilog class encapsulates the Scintilla Verilog
//! lexer.
class QSCINTILLA_EXPORT QsciLexerVerilog : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Verilog lexer.
    enum {
        //! The default.
        Default = 0,
        InactiveDefault = Default + 64,

        //! A comment.
        Comment = 1,
        InactiveComment = Comment + 64,

        //! A line comment.
        CommentLine = 2,
        InactiveCommentLine = CommentLine + 64,

        //! A bang comment.
        CommentBang = 3,
        InactiveCommentBang = CommentBang + 64,

        //! A number
        Number = 4,
        InactiveNumber = Number + 64,

        //! A keyword.
        Keyword = 5,
        InactiveKeyword = Keyword + 64,

        //! A string.
        String = 6,
        InactiveString = String + 64,

        //! A keyword defined in keyword set number 2.  The class must
        //! be sub-classed and re-implement keywords() to make use of
        //! this style.
        KeywordSet2 = 7,
        InactiveKeywordSet2 = KeywordSet2 + 64,

        //! A system task.
        SystemTask = 8,
        InactiveSystemTask = SystemTask + 64,

        //! A pre-processor block.
        Preprocessor = 9,
        InactivePreprocessor = Preprocessor + 64,

        //! An operator.
        Operator = 10,
        InactiveOperator = Operator + 64,

        //! An identifier.
        Identifier = 11,
        InactiveIdentifier = Identifier + 64,

        //! The end of a line where a string is not closed.
        UnclosedString = 12,
        InactiveUnclosedString = UnclosedString + 64,

        //! A keyword defined in keyword set number 4.  The class must
        //! be sub-classed and re-implement keywords() to make use of
        //! this style.  This set is intended to be used for user defined
        //! identifiers and tasks.
        UserKeywordSet = 19,
        InactiveUserKeywordSet = UserKeywordSet + 64,

        //! A keyword comment.
        CommentKeyword = 20,
        InactiveCommentKeyword = CommentKeyword + 64,

        //! An input port declaration.
        DeclareInputPort = 21,
        InactiveDeclareInputPort = DeclareInputPort + 64,

        //! An output port declaration.
        DeclareOutputPort = 22,
        InactiveDeclareOutputPort = DeclareOutputPort + 64,

        //! An input/output port declaration.
        DeclareInputOutputPort = 23,
        InactiveDeclareInputOutputPort = DeclareInputOutputPort + 64,

        //! A port connection.
        PortConnection = 24,
        InactivePortConnection = PortConnection + 64,
    };

    //! Construct a QsciLexerVerilog with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerVerilog(QObject *parent = 0);

    //! Destroys the QsciLexerVerilog instance.
    virtual ~QsciLexerVerilog();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

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
    //! by the lexer as a space separated string.
    const char *keywords(int set) const;

    //! Returns the descriptive name for style number \a style.  If the
    //! style is invalid for this language then an empty QString is returned.
    //! This is intended to be used in user preference dialogs.
    QString description(int style) const;

    //! Causes all properties to be refreshed by emitting the
    //! propertyChanged() signal as required.
    void refreshProperties();

    //! If \a fold is true then "} else {" lines can be folded.  The
    //! default is false.
    //!
    //! \sa foldAtElse()
    void setFoldAtElse(bool fold);

    //! Returns true if "} else {" lines can be folded.
    //!
    //! \sa setFoldAtElse()
    bool foldAtElse() const {return fold_atelse;}

    //! If \a fold is true then multi-line comment blocks can be folded.
    //! The default is false.
    //!
    //! \sa foldComments()
    void setFoldComments(bool fold);

    //! Returns true if multi-line comment blocks can be folded.
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
    bool foldCompact() const {return fold_compact;};

    //! If \a fold is true then preprocessor blocks can be folded.  The
    //! default is true.
    //!
    //! \sa foldPreprocessor()
    void setFoldPreprocessor(bool fold);

    //! Returns true if preprocessor blocks can be folded.
    //!
    //! \sa setFoldPreprocessor()
    bool foldPreprocessor() const {return fold_preproc;};

    //! If \a fold is true then modules can be folded.  The default is false.
    //!
    //! \sa foldAtModule()
    void setFoldAtModule(bool fold);

    //! Returns true if modules can be folded.
    //!
    //! \sa setFoldAtModule()
    bool foldAtModule() const {return fold_atmodule;};

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
    void setAtElseProp();
    void setCommentProp();
    void setCompactProp();
    void setPreprocProp();
    void setAtModuleProp();

    bool fold_atelse;
    bool fold_comments;
    bool fold_compact;
    bool fold_preproc;
    bool fold_atmodule;

    QsciLexerVerilog(const QsciLexerVerilog &);
    QsciLexerVerilog &operator=(const QsciLexerVerilog &);
};

#endif
