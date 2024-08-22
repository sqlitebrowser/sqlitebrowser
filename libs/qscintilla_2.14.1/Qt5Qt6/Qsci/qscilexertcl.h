// This defines the interface to the QsciLexerTCL class.
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


#ifndef QSCILEXERTCL_H
#define QSCILEXERTCL_H

#include <QObject>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


//! \brief The QsciLexerTCL class encapsulates the Scintilla TCL lexer.
class QSCINTILLA_EXPORT QsciLexerTCL : public QsciLexer
{
	Q_OBJECT

public:
	//! This enum defines the meanings of the different styles used by the TCL
	//! lexer.
	enum {
		//! The default.
		Default = 0,

		//! A comment.
		Comment = 1,

		//! A comment line.
		CommentLine = 2,

		//! A number.
		Number = 3,

		//! A quoted keyword.
		QuotedKeyword = 4,

		//! A quoted string.
		QuotedString = 5,

		//! An operator.
		Operator = 6,

		//! An identifier
		Identifier = 7,

		//! A substitution.
		Substitution = 8,

		//! A substitution starting with a brace.
		SubstitutionBrace = 9,

		//! A modifier.
		Modifier = 10,

		//! Expand keyword (defined in keyword set number 5).
		ExpandKeyword = 11,

        //! A TCL keyword (defined in keyword set number 1).
        TCLKeyword = 12,

        //! A Tk keyword (defined in keyword set number 2).
        TkKeyword = 13,

        //! An iTCL keyword (defined in keyword set number 3).
        ITCLKeyword = 14,

        //! A Tk command (defined in keyword set number 4).
        TkCommand = 15,

        //! A keyword defined in keyword set number 6.  The class must be
        //! sub-classed and re-implement keywords() to make use of this style.
        KeywordSet6 = 16,

        //! A keyword defined in keyword set number 7.  The class must be
        //! sub-classed and re-implement keywords() to make use of this style.
        KeywordSet7 = 17,

        //! A keyword defined in keyword set number 8.  The class must be
        //! sub-classed and re-implement keywords() to make use of this style.
        KeywordSet8 = 18,

        //! A keyword defined in keyword set number 9.  The class must be
        //! sub-classed and re-implement keywords() to make use of this style.
        KeywordSet9 = 19,

        //! A comment box.
        CommentBox = 20,

        //! A comment block.
        CommentBlock = 21
	};

	//! Construct a QsciLexerTCL with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
	QsciLexerTCL(QObject *parent = 0);

	//! Destroys the QsciLexerTCL instance.
	virtual ~QsciLexerTCL();

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

	//! Returns the set of keywords for the keyword set \a set recognised
	//! by the lexer as a space separated string.
	const char *keywords(int set) const;

	//! Returns the descriptive name for style number \a style.  If the style
	//! is invalid for this language then an empty QString is returned.  This
	//! is intended to be used in user preference dialogs.
	QString description(int style) const;

	//! Causes all properties to be refreshed by emitting the
	//! propertyChanged() signal as required.
	void refreshProperties();

	//! If \a fold is true then multi-line comment blocks can be folded.  The
    //! default is false.
	//!
	//! \sa foldComments()
	void setFoldComments(bool fold);

    //! Returns true if multi-line comment blocks can be folded.
	//!
	//! \sa setFoldComments()
	bool foldComments() const {return fold_comments;}

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

	bool fold_comments;

	QsciLexerTCL(const QsciLexerTCL &);
	QsciLexerTCL &operator=(const QsciLexerTCL &);
};

#endif
