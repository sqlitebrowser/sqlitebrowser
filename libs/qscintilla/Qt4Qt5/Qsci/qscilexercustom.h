// This defines the interface to the QsciLexerCustom class.
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


#ifndef QSCILEXERCUSTOM_H
#define QSCILEXERCUSTOM_H

#include <Qsci/qsciglobal.h>
#include <Qsci/qscilexer.h>


class QsciScintilla;
class QsciStyle;


//! \brief The QsciLexerCustom class is an abstract class used as a base for
//! new language lexers.
//!
//! The advantage of implementing a new lexer this way (as opposed to adding
//! the lexer to the underlying Scintilla code) is that it does not require the
//! QScintilla library to be re-compiled.  It also makes it possible to
//! integrate external lexers.
//!
//! All that is necessary to implement a new lexer is to define appropriate
//! styles and to re-implement the styleText() method.
class QSCINTILLA_EXPORT QsciLexerCustom : public QsciLexer
{
    Q_OBJECT

public:
    //! Construct a QsciLexerCustom with parent \a parent.  \a parent is
    //! typically the QsciScintilla instance.
    QsciLexerCustom(QObject *parent = 0);

    //! Destroy the QSciLexerCustom.
    virtual ~QsciLexerCustom();

    //! The next \a length characters starting from the current styling
    //! position have their style set to style number \a style.  The current
    //! styling position is moved.  The styling position is initially set by
    //! calling startStyling().
    //!
    //! \sa startStyling(), styleText()
    void setStyling(int length, int style);

    //! The next \a length characters starting from the current styling
    //! position have their style set to style \a style.  The current styling
    //! position is moved.  The styling position is initially set by calling
    //! startStyling().
    //!
    //! \sa startStyling(), styleText()
    void setStyling(int length, const QsciStyle &style);

    //! The styling position is set to \a start.  \a styleBits is unused.
    //!
    //! \sa setStyling(), styleBitsNeeded(), styleText()
    void startStyling(int pos, int styleBits = 0);

    //! This is called when the section of text beginning at position \a start
    //! and up to position \a end needs to be styled.  \a start will always be
    //! at the start of a line.  The text is styled by calling startStyling()
    //! followed by one or more calls to setStyling().  It must be
    //! re-implemented by a sub-class.
    //!
    //! \sa setStyling(), startStyling(), QsciScintilla::bytes(),
    //! QsciScintilla::text()
    virtual void styleText(int start, int end) = 0;

    //! \reimp
    virtual void setEditor(QsciScintilla *editor);

    //! \reimp This re-implementation returns 5 as the number of style bits
    //! needed.
    virtual int styleBitsNeeded() const;

private slots:
    void handleStyleNeeded(int pos);

private:
    QsciLexerCustom(const QsciLexerCustom &);
    QsciLexerCustom &operator=(const QsciLexerCustom &);
};

#endif
