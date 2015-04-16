// This module defines interface to the QsciAbstractAPIs class.
//
// Copyright (c) 2015 Riverbank Computing Limited <info@riverbankcomputing.com>
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


#ifndef QSCIABSTRACTAPIS_H
#define QSCIABSTRACTAPIS_H

#ifdef __APPLE__
extern "C++" {
#endif

#include <QList>
#include <QObject>
#include <QStringList>

#include <Qsci/qsciglobal.h>
#include <Qsci/qsciscintilla.h>


class QsciLexer;


//! \brief The QsciAbstractAPIs class represents the interface to the textual
//! API information used in call tips and for auto-completion.  A sub-class
//! will provide the actual implementation of the interface.
//!
//! API information is specific to a particular language lexer but can be
//! shared by multiple instances of the lexer.
class QSCINTILLA_EXPORT QsciAbstractAPIs : public QObject
{
    Q_OBJECT

public:
    //! Constructs a QsciAbstractAPIs instance attached to lexer \a lexer.  \a
    //! lexer becomes the instance's parent object although the instance can
    //! also be subsequently attached to other lexers.
    QsciAbstractAPIs(QsciLexer *lexer);

    //! Destroy the QsciAbstractAPIs instance.
    virtual ~QsciAbstractAPIs();

    //! Return the lexer that the instance is attached to.
    QsciLexer *lexer() const;

    //! Update the list \a list with API entries derived from \a context.  \a
    //! context is the list of words in the text preceding the cursor position.
    //! The characters that make up a word and the characters that separate
    //! words are defined by the lexer.  The last word is a partial word and
    //! may be empty if the user has just entered a word separator.
    virtual void updateAutoCompletionList(const QStringList &context,
            QStringList &list) = 0;

    //! This is called when the user selects the entry \a selection from the
    //! auto-completion list.  A sub-class can use this as a hint to provide
    //! more specific API entries in future calls to
    //! updateAutoCompletionList().  The default implementation does nothing.
    virtual void autoCompletionSelected(const QString &selection);

    //! Return the call tips valid for the context \a context.  (Note that the
    //! last word of the context will always be empty.)  \a commas is the number
    //! of commas the user has typed after the context and before the cursor
    //! position.  The exact position of the list of call tips can be adjusted
    //! by specifying a corresponding left character shift in \a shifts.  This
    //! is normally done to correct for any displayed context according to \a
    //! style.
    //!
    //! \sa updateAutoCompletionList()
    virtual QStringList callTips(const QStringList &context, int commas,
            QsciScintilla::CallTipsStyle style, QList<int> &shifts) = 0;

private:
    QsciLexer *lex;

    QsciAbstractAPIs(const QsciAbstractAPIs &);
    QsciAbstractAPIs &operator=(const QsciAbstractAPIs &);
};

#ifdef __APPLE__
}
#endif

#endif
