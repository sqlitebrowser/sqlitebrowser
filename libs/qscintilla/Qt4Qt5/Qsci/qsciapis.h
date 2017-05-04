// This module defines interface to the QsciAPIs class.
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


#ifndef QSCIAPIS_H
#define QSCIAPIS_H

#include <QList>
#include <QObject>
#include <QPair>
#include <QStringList>

#include <Qsci/qsciabstractapis.h>
#include <Qsci/qsciglobal.h>
#include <Qsci/qsciscintilla.h>


class QsciAPIsPrepared;
class QsciAPIsWorker;
class QsciLexer;


//! \brief The QsciAPIs class provies an implementation of the textual API
//! information used in call tips and for auto-completion.
//!
//! Raw API information is read from one or more files.  Each API function is
//! described by a single line of text comprising the function's name, followed
//! by the function's optional comma separated parameters enclosed in
//! parenthesis, and finally followed by optional explanatory text.
//!
//! A function name may be followed by a `?' and a number.  The number is used
//! by auto-completion to display a registered QPixmap with the function name.
//!
//! All function names are used by auto-completion, but only those that include
//! function parameters are used in call tips.
//!
//! QScintilla only deals with prepared API information and not the raw
//! information described above.  This is done so that large APIs can be
//! handled while still being responsive to user input.  The conversion of raw
//! information to prepared information is time consuming (think tens of
//! seconds) and implemented in a separate thread.  Prepared information can
//! be quickly saved to and loaded from files.  Such files are portable between
//! different architectures.
//!
//! QScintilla based applications that want to support large APIs would
//! normally provide the user with the ability to specify a set of, possibly
//! project specific, raw API files and convert them to prepared files that are
//! loaded quickly when the application is invoked.
class QSCINTILLA_EXPORT QsciAPIs : public QsciAbstractAPIs
{
    Q_OBJECT

public:
    //! Constructs a QsciAPIs instance attached to lexer \a lexer.  \a lexer
    //! becomes the instance's parent object although the instance can also be
    //! subsequently attached to other lexers.
    QsciAPIs(QsciLexer *lexer);

    //! Destroy the QsciAPIs instance.
    virtual ~QsciAPIs();

    //! Add the single raw API entry \a entry to the current set.
    //!
    //! \sa clear(), load(), remove()
    void add(const QString &entry);

    //! Deletes all raw API information.
    //!
    //! \sa add(), load(), remove()
    void clear();

    //! Load the API information from the file named \a filename, adding it to
    //! the current set.  Returns true if successful, otherwise false.
    bool load(const QString &filename);

    //! Remove the single raw API entry \a entry from the current set.
    //!
    //! \sa add(), clear(), load()
    void remove(const QString &entry);

    //! Convert the current raw API information to prepared API information.
    //! This is implemented by a separate thread.
    //!
    //! \sa cancelPreparation()
    void prepare();

    //! Cancel the conversion of the current raw API information to prepared
    //! API information.
    //!
    //! \sa prepare()
    void cancelPreparation();

    //! Return the default name of the prepared API information file.  It is
    //! based on the name of the associated lexer and in the directory defined
    //! by the QSCIDIR environment variable.  If the environment variable isn't
    //! set then $HOME/.qsci is used.
    QString defaultPreparedName() const;

    //! Check to see is a prepared API information file named \a filename
    //! exists.  If \a filename is empty then the value returned by
    //! defaultPreparedName() is used.  Returns true if successful, otherwise
    //! false.
    //!
    //! \sa defaultPreparedName()
    bool isPrepared(const QString &filename = QString()) const;

    //! Load the prepared API information from the file named \a filename.  If
    //! \a filename is empty then a name is constructed based on the name of
    //! the associated lexer and saved in the directory defined by the QSCIDIR
    //! environment variable.  If the environment variable isn't set then
    //! $HOME/.qsci is used.  Returns true if successful, otherwise false.
    bool loadPrepared(const QString &filename = QString());

    //! Save the prepared API information to the file named \a filename.  If
    //! \a filename is empty then a name is constructed based on the name of
    //! the associated lexer and saved in the directory defined by the QSCIDIR
    //! environment variable.  If the environment variable isn't set then
    //! $HOME/.qsci is used.  Returns true if successful, otherwise false.
    bool savePrepared(const QString &filename = QString()) const;

    //! \reimp
    virtual void updateAutoCompletionList(const QStringList &context,
            QStringList &list);

    //! \reimp
    virtual void autoCompletionSelected(const QString &sel);

    //! \reimp
    virtual QStringList callTips(const QStringList &context, int commas,
            QsciScintilla::CallTipsStyle style, QList<int> &shifts);

    //! \internal Reimplemented to receive termination events from the worker
    //! thread.
    virtual bool event(QEvent *e);

    //! Return a list of the installed raw API file names for the associated
    //! lexer.
    QStringList installedAPIFiles() const;

signals:
    //! This signal is emitted when the conversion of raw API information to
    //! prepared API information has been cancelled.
    //!
    //! \sa apiPreparationFinished(), apiPreparationStarted()
    void apiPreparationCancelled();

    //! This signal is emitted when the conversion of raw API information to
    //! prepared API information starts and can be used to give some visual
    //! feedback to the user.
    //!
    //! \sa apiPreparationCancelled(), apiPreparationFinished()
    void apiPreparationStarted();
    
    //! This signal is emitted when the conversion of raw API information to
    //! prepared API information has finished.
    //!
    //! \sa apiPreparationCancelled(), apiPreparationStarted()
    void apiPreparationFinished();

private:
    friend class QsciAPIsPrepared;
    friend class QsciAPIsWorker;

    // This indexes a word in a set of raw APIs.  The first part indexes the
    // entry in the set, the second part indexes the word within the entry.
    typedef QPair<quint32, quint32> WordIndex;

    // This is a list of word indexes.
    typedef QList<WordIndex> WordIndexList;

    QsciAPIsWorker *worker;
    QStringList old_context;
    QStringList::const_iterator origin;
    int origin_len;
    QString unambiguous_context;
    QStringList apis;
    QsciAPIsPrepared *prep;

    static bool enoughCommas(const QString &s, int commas);

    QStringList positionOrigin(const QStringList &context, QString &path);
    bool originStartsWith(const QString &path, const QString &wsep);
    const WordIndexList *wordIndexOf(const QString &word) const;
    void lastCompleteWord(const QString &word, QStringList &with_context,
            bool &unambig);
    void lastPartialWord(const QString &word, QStringList &with_context,
            bool &unambig);
    void addAPIEntries(const WordIndexList &wl, bool complete,
            QStringList &with_context, bool &unambig);
    QString prepName(const QString &filename, bool mkpath = false) const;
    void deleteWorker();

    QsciAPIs(const QsciAPIs &);
    QsciAPIs &operator=(const QsciAPIs &);
};

#endif
