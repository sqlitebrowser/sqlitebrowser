// This module implements the QsciAbstractAPIs class.
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


#include "Qsci/qsciabstractapis.h"

#include "Qsci/qscilexer.h"


// The ctor.
QsciAbstractAPIs::QsciAbstractAPIs(QsciLexer *lexer)
    : QObject(lexer), lex(lexer)
{
    lexer->setAPIs(this);
}


// The dtor.
QsciAbstractAPIs::~QsciAbstractAPIs()
{
}


// Return the lexer.
QsciLexer *QsciAbstractAPIs::lexer() const
{
    return lex;
}


// Called when the user has made a selection from the auto-completion list.
void QsciAbstractAPIs::autoCompletionSelected(const QString &selection)
{
    Q_UNUSED(selection);
}
