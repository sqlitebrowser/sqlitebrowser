// This defines the interface to the QsciLexerCSS class.
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


#ifndef QSCILEXERCSS_H
#define QSCILEXERCSS_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The QsciLexerCSS class encapsulates the Scintilla CSS lexer.
class QSCINTILLA_EXPORT QsciLexerCSS : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! CSS lexer.
    enum {
        //! The default.
        Default = 0,

        //! A tag.
        Tag = 1,

        //! A class selector.
        ClassSelector = 2,

        //! A pseudo class.  The list of pseudo classes is defined by keyword
        //! set 2.
        PseudoClass = 3,

        //! An unknown pseudo class.
        UnknownPseudoClass = 4,

        //! An operator.
        Operator = 5,

        //! A CSS1 property.  The list of CSS1 properties is defined by keyword
        //! set 1.
        CSS1Property = 6,

        //! An unknown property.
        UnknownProperty = 7,

        //! A value.
        Value = 8,

        //! A comment.
        Comment = 9,

        //! An ID selector.
        IDSelector = 10,

        //! An important value.
        Important = 11,

        //! An @-rule.
        AtRule = 12,

        //! A double-quoted string.
        DoubleQuotedString = 13,

        //! A single-quoted string.
        SingleQuotedString = 14,

        //! A CSS2 property.  The list of CSS2 properties is defined by keyword
        //! set 3.
        CSS2Property = 15,

        //! An attribute.
        Attribute = 16,

        //! A CSS3 property.  The list of CSS3 properties is defined by keyword
        //! set 4.
        CSS3Property = 17,

        //! A pseudo element.  The list of pseudo elements is defined by
        //! keyword set 5.
        PseudoElement = 18,

        //! An extended (browser specific) CSS property.  The list of extended
        //! CSS properties is defined by keyword set 6.
        ExtendedCSSProperty = 19,

        //! An extended (browser specific) pseudo class.  The list of extended
        //! pseudo classes is defined by keyword set 7.
        ExtendedPseudoClass = 20,

        //! An extended (browser specific) pseudo element.  The list of
        //! extended pseudo elements is defined by keyword set 8.
        ExtendedPseudoElement = 21,

        //! A media rule.
        MediaRule = 22,

        //! A variable.
        Variable = 23,
    };

    //! Construct a QsciLexerCSS with parent \a parent.  \a parent is typically
    //! the QsciScintilla instance.
    QsciLexerCSS(QObject *parent = 0);

    //! Destroys the QsciLexerCSS instance.
    virtual ~QsciLexerCSS();

    //! Returns the name of the language.
    const char *language() const;

    //! Returns the name of the lexer.  Some lexers support a number of
    //! languages.
    const char *lexer() const;

    //! \internal Returns a space separated list of words or characters in
    //! a particular style that define the end of a block for
    //! auto-indentation.  The styles is returned via \a style.
    const char *blockEnd(int *style = 0) const;

    //! \internal Returns a space separated list of words or characters in
    //! a particular style that define the start of a block for
    //! auto-indentation.  The styles is returned via \a style.
    const char *blockStart(int *style = 0) const;

    //! Returns the string of characters that comprise a word.
    const char *wordCharacters() const;

    //! Returns the foreground colour of the text for style number \a style.
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

    //! If \a enabled is true then support for HSS is enabled.  The default is
    //! false.
    //!
    //! \sa HSSLanguage()
    void setHSSLanguage(bool enabled);

    //! Returns true if support for HSS is enabled.
    //!
    //! \sa setHSSLanguage()
    bool HSSLanguage() const {return hss_language;}

    //! If \a enabled is true then support for Less CSS is enabled.  The
    //! default is false.
    //!
    //! \sa LessLanguage()
    void setLessLanguage(bool enabled);

    //! Returns true if support for Less CSS is enabled.
    //!
    //! \sa setLessLanguage()
    bool LessLanguage() const {return less_language;}

    //! If \a enabled is true then support for Sassy CSS is enabled.  The
    //! default is false.
    //!
    //! \sa SCSSLanguage()
    void setSCSSLanguage(bool enabled);

    //! Returns true if support for Sassy CSS is enabled.
    //!
    //! \sa setSCSSLanguage()
    bool SCSSLanguage() const {return scss_language;}

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
    void setCommentProp();
    void setCompactProp();
    void setHSSProp();
    void setLessProp();
    void setSCSSProp();

    bool fold_comments;
    bool fold_compact;
    bool hss_language;
    bool less_language;
    bool scss_language;

    QsciLexerCSS(const QsciLexerCSS &);
    QsciLexerCSS &operator=(const QsciLexerCSS &);
};

#endif
