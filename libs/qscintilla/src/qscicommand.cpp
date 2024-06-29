// This module implements the QsciCommand class.
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


#include "Qsci/qscicommand.h"

#include <qnamespace.h>
#include <qapplication.h>

#include "Qsci/qsciscintilla.h"
#include "Qsci/qsciscintillabase.h"


static int convert(int key);


// The ctor.
QsciCommand::QsciCommand(QsciScintilla *qs, QsciCommand::Command cmd, int key,
        int altkey, const char *desc)
    : qsCmd(qs), scicmd(cmd), qkey(key), qaltkey(altkey), descCmd(desc)
{
    scikey = convert(qkey);

    if (scikey)
        qsCmd->SendScintilla(QsciScintillaBase::SCI_ASSIGNCMDKEY, scikey,
                scicmd);

    scialtkey = convert(qaltkey);

    if (scialtkey)
        qsCmd->SendScintilla(QsciScintillaBase::SCI_ASSIGNCMDKEY, scialtkey,
                scicmd);
}


// Execute the command.
void QsciCommand::execute()
{
    qsCmd->SendScintilla(scicmd);
}


// Bind a key to a command.
void QsciCommand::setKey(int key)
{
    bindKey(key,qkey,scikey);
}


// Bind an alternate key to a command.
void QsciCommand::setAlternateKey(int altkey)
{
    bindKey(altkey,qaltkey,scialtkey);
}


// Do the hard work of binding a key.
void QsciCommand::bindKey(int key,int &qk,int &scik)
{
    int new_scikey;

    // Ignore if it is invalid, allowing for the fact that we might be
    // unbinding it.
    if (key)
    {
        new_scikey = convert(key);

        if (!new_scikey)
            return;
    }
    else
        new_scikey = 0;

    if (scik)
        qsCmd->SendScintilla(QsciScintillaBase::SCI_CLEARCMDKEY, scik);

    qk = key;
    scik = new_scikey;

    if (scik)
        qsCmd->SendScintilla(QsciScintillaBase::SCI_ASSIGNCMDKEY, scik, scicmd);
}


// See if a key is valid.
bool QsciCommand::validKey(int key)
{
    return convert(key);
}


// Convert a Qt character to the Scintilla equivalent.  Return zero if it is
// invalid.
static int convert(int key)
{
    // Convert the modifiers.
    int sci_mod = 0;

    if (key & Qt::SHIFT)
        sci_mod |= QsciScintillaBase::SCMOD_SHIFT;

    if (key & Qt::CTRL)
        sci_mod |= QsciScintillaBase::SCMOD_CTRL;

    if (key & Qt::ALT)
        sci_mod |= QsciScintillaBase::SCMOD_ALT;

    if (key & Qt::META)
        sci_mod |= QsciScintillaBase::SCMOD_META;

    key &= ~Qt::MODIFIER_MASK;

    // Convert the key.
    int sci_key = QsciScintillaBase::commandKey(key, sci_mod);

    if (sci_key)
        sci_key |= (sci_mod << 16);

    return sci_key;
}


// Return the translated user friendly description.
QString QsciCommand::description() const
{
    return qApp->translate("QsciCommand", descCmd);
}
