// This defines the interface to the QsciCommandSet class.
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


#ifndef QSCICOMMANDSET_H
#define QSCICOMMANDSET_H

#include <qglobal.h>

#include <QList>

#include <Qsci/qsciglobal.h>
#include <Qsci/qscicommand.h>


QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

class QsciScintilla;


//! \brief The QsciCommandSet class represents the set of all internal editor
//! commands that may have keys bound.
//!
//! Methods are provided to access the individual commands and to read and
//! write the current bindings from and to settings files.
class QSCINTILLA_EXPORT QsciCommandSet
{
public:
    //! The key bindings for each command in the set are read from the
    //! settings \a qs.  \a prefix is prepended to the key of each entry.
    //! true is returned if there was no error.
    //!
    //! \sa writeSettings()
    bool readSettings(QSettings &qs, const char *prefix = "/Scintilla");

    //! The key bindings for each command in the set are written to the
    //! settings \a qs.  \a prefix is prepended to the key of each entry.
    //! true is returned if there was no error.
    //!
    //! \sa readSettings()
    bool writeSettings(QSettings &qs, const char *prefix = "/Scintilla");

    //! The commands in the set are returned as a list.
    QList<QsciCommand *> &commands() {return cmds;}

    //! The primary keys bindings for all commands are removed.
    void clearKeys();

    //! The alternate keys bindings for all commands are removed.
    void clearAlternateKeys();

    // Find the command that is bound to \a key.
    QsciCommand *boundTo(int key) const;

    // Find a specific command \a command.
    QsciCommand *find(QsciCommand::Command command) const;

private:
    friend class QsciScintilla;

    QsciCommandSet(QsciScintilla *qs);
    ~QsciCommandSet();

    QsciScintilla *qsci;
    QList<QsciCommand *> cmds;

    QsciCommandSet(const QsciCommandSet &);
    QsciCommandSet &operator=(const QsciCommandSet &);
};

#endif
