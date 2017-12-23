// This defines the interface to the QsciLexerHTML class.
//
// Copyright (c) 2017 Riverbank Computing Limited <info@riverbankcomputing.com>
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


#ifndef QSCILEXERHTML_H
#define QSCILEXERHTML_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The QsciLexerHTML class encapsulates the Scintilla HTML lexer.
class QSCINTILLA_EXPORT QsciLexerHTML : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! HTML lexer.
    enum {
        //! The default.
        Default = 0,

        //! A tag.
        Tag = 1,

        //! An unknown tag.
        UnknownTag = 2,

        //! An attribute.
        Attribute = 3,

        //! An unknown attribute.
        UnknownAttribute = 4,

        //! An HTML number.
        HTMLNumber = 5,

        //! An HTML double-quoted string.
        HTMLDoubleQuotedString = 6,

        //! An HTML single-quoted string.
        HTMLSingleQuotedString = 7,

        //! Other text within a tag.
        OtherInTag = 8,

        //! An HTML comment.
        HTMLComment = 9,

        //! An entity.
        Entity = 10,

        //! The end of an XML style tag.
        XMLTagEnd = 11,

        //! The start of an XML fragment.
        XMLStart = 12,

        //! The end of an XML fragment.
        XMLEnd = 13,

        //! A script tag.
        Script = 14,

        //! The start of an ASP fragment with @.
        ASPAtStart = 15,

        //! The start of an ASP fragment.
        ASPStart = 16,

        //! CDATA.
        CDATA = 17,

        //! The start of a PHP fragment.
        PHPStart = 18,

        //! An unquoted HTML value.
        HTMLValue = 19,

        //! An ASP X-Code comment.
        ASPXCComment = 20,

        //! The default for SGML.
        SGMLDefault = 21,

        //! An SGML command.
        SGMLCommand = 22,

        //! The first parameter of an SGML command.
        SGMLParameter = 23,

        //! An SGML double-quoted string.
        SGMLDoubleQuotedString = 24,

        //! An SGML single-quoted string.
        SGMLSingleQuotedString = 25,

        //! An SGML error.
        SGMLError = 26,

        //! An SGML special entity.
        SGMLSpecial = 27,

        //! An SGML entity.
        SGMLEntity = 28,

        //! An SGML comment.
        SGMLComment = 29,

        //! A comment with the first parameter of an SGML command.
        SGMLParameterComment = 30,

        //! The default for an SGML block.
        SGMLBlockDefault = 31,

        //! The start of a JavaScript fragment.
        JavaScriptStart = 40,

        //! The default for JavaScript.
        JavaScriptDefault = 41,

        //! A JavaScript comment.
        JavaScriptComment = 42,

        //! A JavaScript line comment.
        JavaScriptCommentLine = 43,

        //! A JavaDoc style JavaScript comment.
        JavaScriptCommentDoc = 44,

        //! A JavaScript number.
        JavaScriptNumber = 45,

        //! A JavaScript word.
        JavaScriptWord = 46,

        //! A JavaScript keyword.
        JavaScriptKeyword = 47,

        //! A JavaScript double-quoted string.
        JavaScriptDoubleQuotedString = 48,

        //! A JavaScript single-quoted string.
        JavaScriptSingleQuotedString = 49,

        //! A JavaScript symbol.
        JavaScriptSymbol = 50,

        //! The end of a JavaScript line where a string is not closed.
        JavaScriptUnclosedString = 51,

        //! A JavaScript regular expression.
        JavaScriptRegex = 52,

        //! The start of an ASP JavaScript fragment.
        ASPJavaScriptStart = 55,

        //! The default for ASP JavaScript.
        ASPJavaScriptDefault = 56,

        //! An ASP JavaScript comment.
        ASPJavaScriptComment = 57,

        //! An ASP JavaScript line comment.
        ASPJavaScriptCommentLine = 58,

        //! An ASP JavaDoc style JavaScript comment.
        ASPJavaScriptCommentDoc = 59,

        //! An ASP JavaScript number.
        ASPJavaScriptNumber = 60,

        //! An ASP JavaScript word.
        ASPJavaScriptWord = 61,

        //! An ASP JavaScript keyword.
        ASPJavaScriptKeyword = 62,

        //! An ASP JavaScript double-quoted string.
        ASPJavaScriptDoubleQuotedString = 63,

        //! An ASP JavaScript single-quoted string.
        ASPJavaScriptSingleQuotedString = 64,

        //! An ASP JavaScript symbol.
        ASPJavaScriptSymbol = 65,

        //! The end of an ASP JavaScript line where a string is not
        //! closed.
        ASPJavaScriptUnclosedString = 66,

        //! An ASP JavaScript regular expression.
        ASPJavaScriptRegex = 67,

        //! The start of a VBScript fragment.
        VBScriptStart = 70,

        //! The default for VBScript.
        VBScriptDefault = 71,

        //! A VBScript comment.
        VBScriptComment = 72,

        //! A VBScript number.
        VBScriptNumber = 73,

        //! A VBScript keyword.
        VBScriptKeyword = 74,

        //! A VBScript string.
        VBScriptString = 75,

        //! A VBScript identifier.
        VBScriptIdentifier = 76,

        //! The end of a VBScript line where a string is not closed.
        VBScriptUnclosedString = 77,

        //! The start of an ASP VBScript fragment.
        ASPVBScriptStart = 80,

        //! The default for ASP VBScript.
        ASPVBScriptDefault = 81,

        //! An ASP VBScript comment.
        ASPVBScriptComment = 82,

        //! An ASP VBScript number.
        ASPVBScriptNumber = 83,

        //! An ASP VBScript keyword.
        ASPVBScriptKeyword = 84,

        //! An ASP VBScript string.
        ASPVBScriptString = 85,

        //! An ASP VBScript identifier.
        ASPVBScriptIdentifier = 86,

        //! The end of an ASP VBScript line where a string is not
        //! closed.
        ASPVBScriptUnclosedString = 87,

        //! The start of a Python fragment.
        PythonStart = 90,

        //! The default for Python.
        PythonDefault = 91,

        //! A Python comment.
        PythonComment = 92,

        //! A Python number.
        PythonNumber = 93,

        //! A Python double-quoted string.
        PythonDoubleQuotedString = 94,

        //! A Python single-quoted string.
        PythonSingleQuotedString = 95,

        //! A Python keyword.
        PythonKeyword = 96,

        //! A Python triple single-quoted string.
        PythonTripleSingleQuotedString = 97,

        //! A Python triple double-quoted string.
        PythonTripleDoubleQuotedString = 98,

        //! The name of a Python class.
        PythonClassName = 99,

        //! The name of a Python function or method.
        PythonFunctionMethodName = 100,

        //! A Python operator.
        PythonOperator = 101,

        //! A Python identifier.
        PythonIdentifier = 102,

        //! The start of an ASP Python fragment.
        ASPPythonStart = 105,

        //! The default for ASP Python.
        ASPPythonDefault = 106,

        //! An ASP Python comment.
        ASPPythonComment = 107,

        //! An ASP Python number.
        ASPPythonNumber = 108,

        //! An ASP Python double-quoted string.
        ASPPythonDoubleQuotedString = 109,

        //! An ASP Python single-quoted string.
        ASPPythonSingleQuotedString = 110,

        //! An ASP Python keyword.
        ASPPythonKeyword = 111,

        //! An ASP Python triple single-quoted string.
        ASPPythonTripleSingleQuotedString = 112,

        //! An ASP Python triple double-quoted string.
        ASPPythonTripleDoubleQuotedString = 113,

        //! The name of an ASP Python class.
        ASPPythonClassName = 114,

        //! The name of an ASP Python function or method.
        ASPPythonFunctionMethodName = 115,

        //! An ASP Python operator.
        ASPPythonOperator = 116,

        //! An ASP Python identifier
        ASPPythonIdentifier = 117,

        //! The default for PHP.
        PHPDefault = 118,

        //! A PHP double-quoted string.
        PHPDoubleQuotedString = 119,

        //! A PHP single-quoted string.
        PHPSingleQuotedString = 120,

        //! A PHP keyword.
        PHPKeyword = 121,

        //! A PHP number.
        PHPNumber = 122,

        //! A PHP variable.
        PHPVariable = 123,

        //! A PHP comment.
        PHPComment = 124,

        //! A PHP line comment.
        PHPCommentLine = 125,

        //! A PHP double-quoted variable.
        PHPDoubleQuotedVariable = 126,

        //! A PHP operator.
        PHPOperator = 127
    };

    //! Construct a QsciLexerHTML with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerHTML(QObject *parent = 0);

    //! Destroys the QsciLexerHTML instance.
    virtual ~QsciLexerHTML();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! \internal Returns the auto-completion fillup characters.
    const char *autoCompletionFillups() const;

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

    //! Returns true if tags are case sensitive.
    //!
    //! \sa setCaseSensitiveTags()
    bool caseSensitiveTags() const {return case_sens_tags;}

    //! If \a enabled is true then Django templates are enabled.  The default
    //! is false.
    //!
    //! \sa djangoTemplates()
    void setDjangoTemplates(bool enabled);

    //! Returns true if support for Django templates is enabled.
    //!
    //! \sa setDjangoTemplates()
    bool djangoTemplates() const {return django_templates;}

    //! Returns true if trailing blank lines are included in a fold block.
    //!
    //! \sa setFoldCompact()
    bool foldCompact() const {return fold_compact;}

    //! Returns true if preprocessor blocks can be folded.
    //!
    //! \sa setFoldPreprocessor()
    bool foldPreprocessor() const {return fold_preproc;}

    //! If \a fold is true then script comments can be folded.  The default is
    //! false.
    //!
    //! \sa foldScriptComments()
    void setFoldScriptComments(bool fold);

    //! Returns true if script comments can be folded.
    //!
    //! \sa setFoldScriptComments()
    bool foldScriptComments() const {return fold_script_comments;}

    //! If \a fold is true then script heredocs can be folded.  The default is
    //! false.
    //!
    //! \sa foldScriptHeredocs()
    void setFoldScriptHeredocs(bool fold);

    //! Returns true if script heredocs can be folded.
    //!
    //! \sa setFoldScriptHeredocs()
    bool foldScriptHeredocs() const {return fold_script_heredocs;}

    //! If \a enabled is true then Mako templates are enabled.  The default is
    //! false.
    //!
    //! \sa makoTemplates()
    void setMakoTemplates(bool enabled);

    //! Returns true if support for Mako templates is enabled.
    //!
    //! \sa setMakoTemplates()
    bool makoTemplates() const {return mako_templates;}

public slots:
    //! If \a fold is true then trailing blank lines are included in a fold
    //! block. The default is true.
    //!
    //! \sa foldCompact()
    virtual void setFoldCompact(bool fold);

    //! If \a fold is true then preprocessor blocks can be folded.  The
    //! default is false.
    //!
    //! \sa foldPreprocessor()
    virtual void setFoldPreprocessor(bool fold);

    //! If \a sens is true then tags are case sensitive.  The default is false.
    //!
    //! \sa caseSensitiveTags()
    virtual void setCaseSensitiveTags(bool sens);

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
    void setCompactProp();
    void setPreprocProp();
    void setCaseSensTagsProp();
    void setScriptCommentsProp();
    void setScriptHeredocsProp();
    void setDjangoProp();
    void setMakoProp();

    bool fold_compact;
    bool fold_preproc;
    bool case_sens_tags;
    bool fold_script_comments;
    bool fold_script_heredocs;
    bool django_templates;
    bool mako_templates;

    QsciLexerHTML(const QsciLexerHTML &);
    QsciLexerHTML &operator=(const QsciLexerHTML &);
};

#endif
