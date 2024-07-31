// This module implements the QsciMacro class.
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


#include "Qsci/qscimacro.h"

#include <QStringList>

#include "Qsci/qsciscintilla.h"


static int fromHex(unsigned char ch);


// The ctor.
QsciMacro::QsciMacro(QsciScintilla *parent)
    : QObject(parent), qsci(parent)
{
}


// The ctor that initialises the macro.
QsciMacro::QsciMacro(const QString &asc, QsciScintilla *parent)
    : QObject(parent), qsci(parent)
{
    load(asc);
}


// The dtor.
QsciMacro::~QsciMacro()
{
}


// Clear the contents of the macro.
void QsciMacro::clear()
{
    macro.clear();
}


// Read a macro from a string.
bool QsciMacro::load(const QString &asc)
{
    bool ok = true;

    macro.clear();

    QStringList fields = asc.split(' ');

    int f = 0;

    while (f < fields.size())
    {
        Macro cmd;
        unsigned len;

        // Extract the 3 fixed fields.
        if (f + 3 > fields.size())
        {
            ok = false;
            break;
        }

        cmd.msg = fields[f++].toUInt(&ok);

        if (!ok)
            break;

        cmd.wParam = fields[f++].toULong(&ok);

        if (!ok)
            break;

        len = fields[f++].toUInt(&ok);

        if (!ok)
            break;

        // Extract any text.
        if (len)
        {
            if (f + 1 > fields.size())
            {
                ok = false;
                break;
            }

            QByteArray ba = fields[f++].toLatin1();
            const char *sp = ba.data();

            if (!sp)
            {
                ok = false;
                break;
            }

            // Because of historical bugs the length field is unreliable.
            bool embedded_null = false;
            unsigned char ch;

            while ((ch = *sp++) != '\0')
            {
                if (ch == '"' || ch <= ' ' || ch >= 0x7f)
                {
                    ok = false;
                    break;
                }

                if (ch == '\\')
                {
                    int b1, b2;

                    if ((b1 = fromHex(*sp++)) < 0 ||
                        (b2 = fromHex(*sp++)) < 0)
                    {
                        ok = false;
                        break;
                    }

                    ch = (b1 << 4) + b2;
                }

                if (ch == '\0')
                {
                    // Don't add it now as it may be the terminating '\0'.
                    embedded_null = true;
                }
                else
                {
                    if (embedded_null)
                    {
                        // Add the pending embedded '\0'.
                        cmd.text += '\0';
                        embedded_null = false;
                    }

                    cmd.text += ch;
                }
            }

            if (!ok)
                break;
        }

        macro.append(cmd);
    }
        
    if (!ok)
        macro.clear();

    return ok;
}


// Write a macro to a string.
QString QsciMacro::save() const
{
    QString ms;

    QList<Macro>::const_iterator it;

    for (it = macro.begin(); it != macro.end(); ++it)
    {
        if (!ms.isEmpty())
            ms += ' ';

        unsigned len = (*it).text.size();

        ms += QString("%1 %2 %3").arg((*it).msg).arg((*it).wParam).arg(len);

        if (len)
        {
            // In Qt v3, if the length is greater than zero then it also
            // includes the '\0', so we need to make sure that Qt v4 writes the
            // '\0'.  That the '\0' is written at all is a bug because
            // QCString::size() is used instead of QCString::length().  We
            // don't fix this so as not to break old macros.  However this is
            // still broken because we have already written the unadjusted
            // length.  So, in summary, the length field should be interpreted
            // as a zero/non-zero value, and the end of the data is either at
            // the next space or the very end of the data.
            ++len;

            ms += ' ';

            const char *cp = (*it).text.data();

            while (len--)
            {
                unsigned char ch = *cp++;

                if (ch == '\\' || ch == '"' || ch <= ' ' || ch >= 0x7f)
                    ms += QString("\\%1").arg(static_cast<uint>(ch), 2, 16,
                            QLatin1Char('0'));
                else
                    ms += static_cast<char>(ch);
            }
        }
    }

    return ms;
}


// Play the macro.
void QsciMacro::play()
{
    if (!qsci)
        return;

    QList<Macro>::const_iterator it;

    for (it = macro.begin(); it != macro.end(); ++it)
        qsci->SendScintilla((*it).msg, static_cast<uintptr_t>((*it).wParam),
                (*it).text.constData());
}


// Start recording.
void QsciMacro::startRecording()
{
    if (!qsci)
        return;

    macro.clear();

    connect(qsci, SIGNAL(SCN_MACRORECORD(unsigned int, unsigned long, void *)),
            SLOT(record(unsigned int, unsigned long, void *)));

    qsci->SendScintilla(QsciScintillaBase::SCI_STARTRECORD);
}


// End recording.
void QsciMacro::endRecording()
{
    if (!qsci)
        return;

    qsci->SendScintilla(QsciScintillaBase::SCI_STOPRECORD);
    qsci->disconnect(this);
}


// Record a command.
void QsciMacro::record(unsigned int msg, unsigned long wParam, void *lParam)
{
    Macro m;

    m.msg = msg;
    m.wParam = wParam;

    // Determine commands which need special handling of the parameters.
    switch (msg)
    {
    case QsciScintillaBase::SCI_ADDTEXT:
        m.text = QByteArray(reinterpret_cast<const char *>(lParam), wParam);
        break;

    case QsciScintillaBase::SCI_REPLACESEL:
        if (!macro.isEmpty() && macro.last().msg == QsciScintillaBase::SCI_REPLACESEL)
        {
            // This is the command used for ordinary user input so it's a
            // significant space reduction to append it to the previous
            // command.

            macro.last().text.append(reinterpret_cast<const char *>(lParam));
            return;
        }

        /* Drop through. */

    case QsciScintillaBase::SCI_INSERTTEXT:
    case QsciScintillaBase::SCI_APPENDTEXT:
    case QsciScintillaBase::SCI_SEARCHNEXT:
    case QsciScintillaBase::SCI_SEARCHPREV:
        m.text.append(reinterpret_cast<const char *>(lParam));
        break;
    }

    macro.append(m);
}


// Return the given hex character as a binary.
static int fromHex(unsigned char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';

    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;

    return -1;
}
