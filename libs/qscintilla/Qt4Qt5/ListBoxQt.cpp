// This module implements the specialisation of QListBox that handles the
// Scintilla double-click callback.
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


#include "ListBoxQt.h"

#include <stdlib.h>

#include "SciClasses.h"
#include "Qsci/qsciscintilla.h"


QsciListBoxQt::QsciListBoxQt()
    : cb_action(0), cb_data(0), slb(0), visible_rows(5), utf8(false)
{
}


void QsciListBoxQt::SetFont(QSCI_SCI_NAMESPACE(Font) &font)
{
    QFont *f = reinterpret_cast<QFont *>(font.GetID());

    if (f)
        slb->setFont(*f);
}


void QsciListBoxQt::Create(QSCI_SCI_NAMESPACE(Window) &parent, int,
        QSCI_SCI_NAMESPACE(Point), int, bool unicodeMode, int)
{
    utf8 = unicodeMode;

    // The parent we want is the QsciScintillaBase, not the text area.
    wid = slb = new QsciSciListBox(reinterpret_cast<QWidget *>(parent.GetID())->parentWidget(), this);
}


void QsciListBoxQt::SetAverageCharWidth(int)
{
    // We rely on sizeHint() for the size of the list box rather than make
    // calculations based on the average character width and the number of
    // visible rows.
}


void QsciListBoxQt::SetVisibleRows(int vrows)
{
    // We only pretend to implement this.
    visible_rows = vrows;
}


int QsciListBoxQt::GetVisibleRows() const
{
    return visible_rows;
}


QSCI_SCI_NAMESPACE(PRectangle) QsciListBoxQt::GetDesiredRect()
{
    QSCI_SCI_NAMESPACE(PRectangle) rc(0, 0, 100, 100);

    if (slb)
    {
        QSize sh = slb->sizeHint();

        rc.right = sh.width();
        rc.bottom = sh.height();
    }

    return rc;
}


int QsciListBoxQt::CaretFromEdge()
{
    int dist = 0;

    // Find the width of the biggest image.
    for (xpmMap::const_iterator it = xset.begin(); it != xset.end(); ++it)
    {
        int w = it.value().width();

        if (dist < w)
            dist = w;
    }

    if (slb)
        dist += slb->frameWidth();

    // Fudge factor - adjust if required.
    dist += 3;

    return dist;
}


void QsciListBoxQt::Clear()
{
    Q_ASSERT(slb);

    slb->clear();
}


void QsciListBoxQt::Append(char *s, int type)
{
    Q_ASSERT(slb);

    QString qs;

    if (utf8)
        qs = QString::fromUtf8(s);
    else
        qs = QString::fromLatin1(s);

    xpmMap::const_iterator it;

    if (type < 0 || (it = xset.find(type)) == xset.end())
        slb->addItem(qs);
    else
        slb->addItemPixmap(it.value(), qs);
}


int QsciListBoxQt::Length()
{
    Q_ASSERT(slb);

    return slb->count();
}


void QsciListBoxQt::Select(int n)
{
    Q_ASSERT(slb);

    slb->setCurrentRow(n);
}


int QsciListBoxQt::GetSelection()
{
    Q_ASSERT(slb);

    return slb->currentRow();
}


int QsciListBoxQt::Find(const char *prefix)
{
    Q_ASSERT(slb);

    return slb->find(prefix);
}


void QsciListBoxQt::GetValue(int n, char *value, int len)
{
    Q_ASSERT(slb);

    QString selection = slb->text(n);

    bool trim_selection = false;
    QObject *sci_obj = slb->parent();

    if (sci_obj->inherits("QsciScintilla"))
    {
        QsciScintilla *sci = static_cast<QsciScintilla *>(sci_obj);

        if (sci->isAutoCompletionList())
        {
            // Save the full selection and trim the value we return.
            sci->acSelection = selection;
            trim_selection = true;
        }
    }

    if (selection.isEmpty() || len <= 0)
        value[0] = '\0';
    else
    {
        const char *s;
        int slen;

        QByteArray bytes;

        if (utf8)
            bytes = selection.toUtf8();
        else
            bytes = selection.toLatin1();

        s = bytes.data();
        slen = bytes.length();

        while (slen-- && len--)
        {
            if (trim_selection && *s == ' ')
                break;

            *value++ = *s++;
        }

        *value = '\0';
    }
}


void QsciListBoxQt::Sort()
{
    Q_ASSERT(slb);

    slb->sortItems();
}


void QsciListBoxQt::RegisterImage(int type, const char *xpm_data)
{
    xset.insert(type, *reinterpret_cast<const QPixmap *>(xpm_data));
}


void QsciListBoxQt::RegisterRGBAImage(int type, int, int,
        const unsigned char *pixelsImage)
{
    QPixmap pm;

#if QT_VERSION >= 0x040700
    pm.convertFromImage(*reinterpret_cast<const QImage *>(pixelsImage));
#else
    pm = QPixmap::fromImage(*reinterpret_cast<const QImage *>(pixelsImage));
#endif

    xset.insert(type, pm);
}


void QsciListBoxQt::ClearRegisteredImages()
{
    xset.clear();
}


void QsciListBoxQt::SetDoubleClickAction(
        QSCI_SCI_NAMESPACE(CallBackAction) action, void *data)
{
    cb_action = action;
    cb_data = data;
}


void QsciListBoxQt::SetList(const char *list, char separator, char typesep)
{
    char *words;

    Clear();

    if ((words = qstrdup(list)) != NULL)
    {
        char *startword = words;
        char *numword = NULL;

        for (int i = 0; words[i] != '\0'; i++)
        {
            if (words[i] == separator)
            {
                words[i] = '\0';

                if (numword)
                    *numword = '\0';

                Append(startword, numword ? atoi(numword + 1) : -1);

                startword = words + i + 1;
                numword = NULL;
            }
            else if (words[i] == typesep)
            {
                numword = words + i;
            }
        }

        if (startword)
        {
            if (numword)
                *numword = '\0';

            Append(startword, numword ? atoi(numword + 1) : -1);
        }

        delete[] words;
    }
}


// The ListBox methods that need to be implemented explicitly.

QSCI_SCI_NAMESPACE(ListBox)::ListBox()
{
}


QSCI_SCI_NAMESPACE(ListBox)::~ListBox()
{
}


QSCI_SCI_NAMESPACE(ListBox) *QSCI_SCI_NAMESPACE(ListBox)::Allocate()
{
    return new QsciListBoxQt();
}
