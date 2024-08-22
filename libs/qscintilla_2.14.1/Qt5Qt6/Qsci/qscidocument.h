// This defines the interface to the QsciDocument class.
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


#ifndef QSCIDOCUMENT_H
#define QSCIDOCUMENT_H

#include <Qsci/qsciglobal.h>


class QsciScintillaBase;
class QsciDocumentP;


//! \brief The QsciDocument class represents a document to be edited.
//!
//! It is an opaque class that can be attached to multiple instances of
//! QsciScintilla to create different simultaneous views of the same document.
//! QsciDocument uses implicit sharing so that copying class instances is a
//! cheap operation.
class QSCINTILLA_EXPORT QsciDocument
{
public:
    //! Create a new unattached document.
    QsciDocument();
    virtual ~QsciDocument();

    QsciDocument(const QsciDocument &);
    QsciDocument &operator=(const QsciDocument &);

private:
    friend class QsciScintilla;

    void attach(const QsciDocument &that);
    void detach();
    void display(QsciScintillaBase *qsb, const QsciDocument *from);
    void undisplay(QsciScintillaBase *qsb);

    bool isModified() const;
    void setModified(bool m);

    QsciDocumentP *pdoc;
};

#endif
