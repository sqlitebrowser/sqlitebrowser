// This module implements the QsciLexerCustom class.
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


#include "Qsci/qscilexercustom.h"

#include "Qsci/qsciscintilla.h"
#include "Qsci/qsciscintillabase.h"
#include "Qsci/qscistyle.h"


// The ctor.
QsciLexerCustom::QsciLexerCustom(QObject *parent)
    : QsciLexer(parent)
{
}


// The dtor.
QsciLexerCustom::~QsciLexerCustom()
{
}


// Start styling.
void QsciLexerCustom::startStyling(int start, int)
{
    if (!editor())
        return;

    editor()->SendScintilla(QsciScintillaBase::SCI_STARTSTYLING, start);
}


// Set the style for a number of characters.
void QsciLexerCustom::setStyling(int length, int style)
{
    if (!editor())
        return;

    editor()->SendScintilla(QsciScintillaBase::SCI_SETSTYLING, length, style);
}


// Set the style for a number of characters.
void QsciLexerCustom::setStyling(int length, const QsciStyle &style)
{
    setStyling(length, style.style());
}


// Set the attached editor.
void QsciLexerCustom::setEditor(QsciScintilla *new_editor)
{
    if (editor())
        disconnect(editor(), SIGNAL(SCN_STYLENEEDED(int)), this,
                SLOT(handleStyleNeeded(int)));

    QsciLexer::setEditor(new_editor);

    if (editor())
        connect(editor(), SIGNAL(SCN_STYLENEEDED(int)), this,
                SLOT(handleStyleNeeded(int)));
}


// Return the number of style bits needed by the lexer.
int QsciLexerCustom::styleBitsNeeded() const
{
    return 5;
}


// Handle a request to style some text.
void QsciLexerCustom::handleStyleNeeded(int pos)
{
    int start = editor()->SendScintilla(QsciScintillaBase::SCI_GETENDSTYLED);
    int line = editor()->SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION,
            start);
    start = editor()->SendScintilla(QsciScintillaBase::SCI_POSITIONFROMLINE,
            line);

    if (start != pos)
        styleText(start, pos);
}
