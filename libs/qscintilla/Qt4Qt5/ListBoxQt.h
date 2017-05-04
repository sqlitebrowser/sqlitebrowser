// This defines the specialisation of QListBox that handles the Scintilla
// double-click callback.
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


#include <qmap.h>
#include <qpixmap.h>
#include <qstring.h>

#include "SciNamespace.h"

#include "Platform.h"


class QsciSciListBox;


// This is an internal class but it is referenced by a public class so it has
// to have a Qsci prefix rather than being put in the Scintilla namespace
// which would mean exposing the SCI_NAMESPACE mechanism).
class QsciListBoxQt : public QSCI_SCI_NAMESPACE(ListBox)
{
public:
    QsciListBoxQt();

    QSCI_SCI_NAMESPACE(CallBackAction) cb_action;
    void *cb_data;

    virtual void SetFont(QSCI_SCI_NAMESPACE(Font) &font);
    virtual void Create(QSCI_SCI_NAMESPACE(Window) &parent, int,
            QSCI_SCI_NAMESPACE(Point), int, bool unicodeMode, int);
    virtual void SetAverageCharWidth(int);
    virtual void SetVisibleRows(int);
    virtual int GetVisibleRows() const;
    virtual QSCI_SCI_NAMESPACE(PRectangle) GetDesiredRect();
    virtual int CaretFromEdge();
    virtual void Clear();
    virtual void Append(char *s, int type = -1);
    virtual int Length();
    virtual void Select(int n);
    virtual int GetSelection();
    virtual int Find(const char *prefix);
    virtual void GetValue(int n, char *value, int len);
    virtual void Sort();
    virtual void RegisterImage(int type, const char *xpm_data);
    virtual void RegisterRGBAImage(int type, int width, int height,
            const unsigned char *pixelsImage);
    virtual void ClearRegisteredImages();
    virtual void SetDoubleClickAction(
            QSCI_SCI_NAMESPACE(CallBackAction) action, void *data);
    virtual void SetList(const char *list, char separator, char typesep);

private:
    QsciSciListBox *slb;
    int visible_rows;
    bool utf8;

    typedef QMap<int, QPixmap> xpmMap;
    xpmMap xset;
};
