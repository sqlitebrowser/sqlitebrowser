// This defines the interface to the QsciMacro class.
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


#ifndef QSCIMACRO_H
#define QSCIMACRO_H

#include <QList>
#include <QObject>
#include <QString>

#include <Qsci/qsciglobal.h>


class QsciScintilla;


//! \brief The QsciMacro class represents a sequence of recordable editor
//! commands.
//!
//! Methods are provided to convert convert a macro to and from a textual
//! representation so that they can be easily written to and read from
//! permanent storage.
class QSCINTILLA_EXPORT QsciMacro : public QObject
{
    Q_OBJECT

public:
    //! Construct a QsciMacro with parent \a parent.
    QsciMacro(QsciScintilla *parent);

    //! Construct a QsciMacro from the printable ASCII representation \a asc,
    //! with parent \a parent.
    QsciMacro(const QString &asc, QsciScintilla *parent);

    //! Destroy the QsciMacro instance.
    virtual ~QsciMacro();

    //! Clear the contents of the macro.
    void clear();

    //! Load the macro from the printable ASCII representation \a asc.  Returns
    //! true if there was no error.
    //!
    //! \sa save()
    bool load(const QString &asc);

    //! Return a printable ASCII representation of the macro.  It is guaranteed
    //! that only printable ASCII characters are used and that double quote
    //! characters will not be used.
    //!
    //! \sa load()
    QString save() const;

public slots:
    //! Play the macro.
    virtual void play();

    //! Start recording user commands and add them to the macro.
    virtual void startRecording();

    //! Stop recording user commands.
    virtual void endRecording();

private slots:
    void record(unsigned int msg, unsigned long wParam, void *lParam);

private:
    struct Macro {
        unsigned int msg;
        unsigned long wParam;
        QByteArray text;
    };

    QsciScintilla *qsci;
    QList<Macro> macro;

    QsciMacro(const QsciMacro &);
    QsciMacro &operator=(const QsciMacro &);
};

#endif
