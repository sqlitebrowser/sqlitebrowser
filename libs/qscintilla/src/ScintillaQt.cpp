// The implementation of the Qt specific subclass of ScintillaBase.
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


#include <string.h>

#include <qapplication.h>
#include <qbytearray.h>
#include <qdrag.h>
#include <qevent.h>
#include <qmimedata.h>
#include <qpainter.h>
#include <qscrollbar.h>
#include <qstring.h>

#include "Qsci/qsciscintillabase.h"
#include "ScintillaQt.h"
#if !defined(QT_NO_ACCESSIBILITY)
#include "SciAccessibility.h"
#endif
#include "SciClasses.h"


// We want to use the Scintilla notification names as Qt signal names.
#undef  SCEN_CHANGE
#undef  SCN_AUTOCCANCELLED
#undef  SCN_AUTOCCHARDELETED
#undef  SCN_AUTOCCOMPLETED
#undef  SCN_AUTOCSELECTION
#undef  SCN_AUTOCSELECTIONCHANGE
#undef  SCN_CALLTIPCLICK
#undef  SCN_CHARADDED
#undef  SCN_DOUBLECLICK
#undef  SCN_DWELLEND
#undef  SCN_DWELLSTART
#undef  SCN_FOCUSIN
#undef  SCN_FOCUSOUT
#undef  SCN_HOTSPOTCLICK
#undef  SCN_HOTSPOTDOUBLECLICK
#undef  SCN_HOTSPOTRELEASECLICK
#undef  SCN_INDICATORCLICK
#undef  SCN_INDICATORRELEASE
#undef  SCN_MACRORECORD
#undef  SCN_MARGINCLICK
#undef  SCN_MARGINRIGHTCLICK
#undef  SCN_MODIFIED
#undef  SCN_MODIFYATTEMPTRO
#undef  SCN_NEEDSHOWN
#undef  SCN_PAINTED
#undef  SCN_SAVEPOINTLEFT
#undef  SCN_SAVEPOINTREACHED
#undef  SCN_STYLENEEDED
#undef  SCN_UPDATEUI
#undef  SCN_USERLISTSELECTION
#undef  SCN_ZOOM

enum
{
    SCEN_CHANGE = 768,
    SCN_AUTOCCANCELLED = 2025,
    SCN_AUTOCCHARDELETED = 2026,
    SCN_AUTOCCOMPLETED = 2030,
    SCN_AUTOCSELECTION = 2022,
    SCN_AUTOCSELECTIONCHANGE = 2032,
    SCN_CALLTIPCLICK = 2021,
    SCN_CHARADDED = 2001,
    SCN_DOUBLECLICK = 2006,
    SCN_DWELLEND = 2017,
    SCN_DWELLSTART = 2016,
    SCN_FOCUSIN = 2028,
    SCN_FOCUSOUT = 2029,
    SCN_HOTSPOTCLICK = 2019,
    SCN_HOTSPOTDOUBLECLICK = 2020,
    SCN_HOTSPOTRELEASECLICK = 2027,
    SCN_INDICATORCLICK = 2023,
    SCN_INDICATORRELEASE = 2024,
    SCN_MACRORECORD = 2009,
    SCN_MARGINCLICK = 2010,
    SCN_MARGINRIGHTCLICK = 2031,
    SCN_MODIFIED = 2008,
    SCN_MODIFYATTEMPTRO = 2004,
    SCN_NEEDSHOWN = 2011,
    SCN_PAINTED = 2013,
    SCN_SAVEPOINTLEFT = 2003,
    SCN_SAVEPOINTREACHED = 2002,
    SCN_STYLENEEDED = 2000,
    SCN_UPDATEUI = 2007,
    SCN_USERLISTSELECTION = 2014,
    SCN_ZOOM = 2018
};


// The ctor.
QsciScintillaQt::QsciScintillaQt(QsciScintillaBase *qsb_)
    : vMax(0), hMax(0), vPage(0), hPage(0), capturedMouse(false), qsb(qsb_)
{
    wMain = qsb->viewport();

    // This is ignored.
    imeInteraction = imeInline;

    // Using pixmaps screws things up when moving to a different display
    // (although this could be because we haven't got the pixmap code right).
    // However Qt shouldn't need buffered drawing anyway.
    WndProc(SCI_SETBUFFEREDDRAW, 0, 0);

    for (int i = 0; i <= static_cast<int>(tickPlatform); ++i)
        timers[i] = 0;

    Initialise();
}


// The dtor.
QsciScintillaQt::~QsciScintillaQt()
{ 
    Finalise();
}


// Initialise the instance.
void QsciScintillaQt::Initialise()
{
    // This signal is only ever emitted for systems that have a separate
    // selection (ie. X11).
    connect(QApplication::clipboard(), SIGNAL(selectionChanged()), this,
            SLOT(onSelectionChanged()));
}


// Tidy up the instance.
void QsciScintillaQt::Finalise()
{
    for (int i = 0; i <= static_cast<int>(tickPlatform); ++i)
        FineTickerCancel(static_cast<TickReason>(i));

    ScintillaBase::Finalise();
}


// Start a drag.
void QsciScintillaQt::StartDrag()
{
    inDragDrop = ddDragging;

    QDrag *qdrag = new QDrag(qsb);
    qdrag->setMimeData(mimeSelection(drag));

    Qt::DropAction action = qdrag->exec(Qt::MoveAction | Qt::CopyAction, Qt::MoveAction);

    // Remove the dragged text if it was a move to another widget or
    // application.
    if (action == Qt::MoveAction && qdrag->target() != qsb->viewport())
        ClearSelection();

    SetDragPosition(Scintilla::SelectionPosition());
    inDragDrop = ddNone;
}


// Re-implement to trap certain messages.
sptr_t QsciScintillaQt::WndProc(unsigned int iMessage, uptr_t wParam,
        sptr_t lParam)
{
    switch (iMessage)
    {
    case SCI_GETDIRECTFUNCTION:
        return reinterpret_cast<sptr_t>(DirectFunction);
    
    case SCI_GETDIRECTPOINTER:
        return reinterpret_cast<sptr_t>(this);
    }

    return ScintillaBase::WndProc(iMessage, wParam, lParam);
}


// Windows nonsense.
sptr_t QsciScintillaQt::DefWndProc(unsigned int, uptr_t, sptr_t)
{
    return 0;
}


// Grab or release the mouse (and keyboard).
void QsciScintillaQt::SetMouseCapture(bool on)
{
    if (mouseDownCaptures)
    {
        if (on)
            qsb->viewport()->grabMouse();
        else
            qsb->viewport()->releaseMouse();
    }

    capturedMouse = on;
}


// Return true if the mouse/keyboard are currently grabbed.
bool QsciScintillaQt::HaveMouseCapture()
{
    return capturedMouse;
}


// Set the position of the vertical scrollbar.
void QsciScintillaQt::SetVerticalScrollPos()
{
    QScrollBar *sb = qsb->verticalScrollBar();
    bool was_blocked = sb->blockSignals(true);

    sb->setValue(topLine);

    sb->blockSignals(was_blocked);
}


// Set the position of the horizontal scrollbar.
void QsciScintillaQt::SetHorizontalScrollPos()
{
    QScrollBar *sb = qsb->horizontalScrollBar();
    bool was_blocked = sb->blockSignals(true);

    sb->setValue(xOffset);

    sb->blockSignals(was_blocked);
}


// Set the extent of the vertical and horizontal scrollbars and return true if
// the view needs re-drawing.
bool QsciScintillaQt::ModifyScrollBars(Sci::Line nMax, Sci::Line nPage)
{
    bool modified = false;
    QScrollBar *sb;

    int vNewPage = nPage;
    int vNewMax = nMax - vNewPage + 1;

    if (vMax != vNewMax || vPage != vNewPage)
    {
        vMax = vNewMax;
        vPage = vNewPage;
        modified = true;

        sb = qsb->verticalScrollBar();
        sb->setMaximum(vMax);
        sb->setPageStep(vPage);
    }

    int hNewPage = GetTextRectangle().Width();
    int hNewMax = (scrollWidth > hNewPage) ? scrollWidth - hNewPage : 0;
    int charWidth = vs.styles[STYLE_DEFAULT].aveCharWidth;

    sb = qsb->horizontalScrollBar();

    if (hMax != hNewMax || hPage != hNewPage || sb->singleStep() != charWidth)
    {
        hMax = hNewMax;
        hPage = hNewPage;
        modified = true;

        sb->setMaximum(hMax);
        sb->setPageStep(hPage);
        sb->setSingleStep(charWidth);
    }

    return modified;
}


// Called after SCI_SETWRAPMODE and SCI_SETHSCROLLBAR.
void QsciScintillaQt::ReconfigureScrollBars()
{
    // Hide or show the scrollbars if needed.
    bool hsb = (horizontalScrollBarVisible && !Wrapping());

    qsb->setHorizontalScrollBarPolicy(hsb ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
    qsb->setVerticalScrollBarPolicy(verticalScrollBarVisible ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
}


// Notify interested parties of any change in the document.
void QsciScintillaQt::NotifyChange()
{
    emit qsb->SCEN_CHANGE();
}


// Notify interested parties of various events.  This is the main mapping
// between Scintilla notifications and Qt signals.
void QsciScintillaQt::NotifyParent(SCNotification scn)
{
    switch (scn.nmhdr.code)
    {
    case SCN_CALLTIPCLICK:
        emit qsb->SCN_CALLTIPCLICK(scn.position);
        break;

    case SCN_AUTOCCANCELLED:
        emit qsb->SCN_AUTOCCANCELLED();
        break;

    case SCN_AUTOCCHARDELETED:
        emit qsb->SCN_AUTOCCHARDELETED();
        break;

    case SCN_AUTOCCOMPLETED:
        emit qsb->SCN_AUTOCCOMPLETED(scn.text, scn.position, scn.ch,
                scn.listCompletionMethod);
        break;

    case SCN_AUTOCSELECTION:
        emit qsb->SCN_AUTOCSELECTION(scn.text, scn.position, scn.ch,
                scn.listCompletionMethod);
        emit qsb->SCN_AUTOCSELECTION(scn.text, scn.position);
        break;

    case SCN_AUTOCSELECTIONCHANGE:
        emit qsb->SCN_AUTOCSELECTIONCHANGE(scn.text, scn.listType,
                scn.position);
        break;

    case SCN_CHARADDED:
        emit qsb->SCN_CHARADDED(scn.ch);
        break;

    case SCN_DOUBLECLICK:
        emit qsb->SCN_DOUBLECLICK(scn.position, scn.line, scn.modifiers);
        break;

    case SCN_DWELLEND:
        emit qsb->SCN_DWELLEND(scn.position, scn.x, scn.y);
        break;

    case SCN_DWELLSTART:
        emit qsb->SCN_DWELLSTART(scn.position, scn.x, scn.y);
        break;

    case SCN_FOCUSIN:
        emit qsb->SCN_FOCUSIN();
        break;

    case SCN_FOCUSOUT:
        emit qsb->SCN_FOCUSOUT();
        break;

    case SCN_HOTSPOTCLICK:
        emit qsb->SCN_HOTSPOTCLICK(scn.position, scn.modifiers);
        break;

    case SCN_HOTSPOTDOUBLECLICK:
        emit qsb->SCN_HOTSPOTDOUBLECLICK(scn.position, scn.modifiers);
        break;

    case SCN_HOTSPOTRELEASECLICK:
        emit qsb->SCN_HOTSPOTRELEASECLICK(scn.position, scn.modifiers);
        break;

    case SCN_INDICATORCLICK:
        emit qsb->SCN_INDICATORCLICK(scn.position, scn.modifiers);
        break;

    case SCN_INDICATORRELEASE:
        emit qsb->SCN_INDICATORRELEASE(scn.position, scn.modifiers);
        break;

    case SCN_MACRORECORD:
        emit qsb->SCN_MACRORECORD(scn.message, scn.wParam,
                reinterpret_cast<void *>(scn.lParam));
        break;

    case SCN_MARGINCLICK:
        emit qsb->SCN_MARGINCLICK(scn.position, scn.modifiers, scn.margin);
        break;

    case SCN_MARGINRIGHTCLICK:
        emit qsb->SCN_MARGINRIGHTCLICK(scn.position, scn.modifiers,
                scn.margin);
        break;

    case SCN_MODIFIED:
        {
            char *text;

#if !defined(QT_NO_ACCESSIBILITY)
            if ((scn.modificationType & SC_MOD_INSERTTEXT) != 0)
                QsciAccessibleScintillaBase::textInserted(qsb, scn.position,
                        scn.text, scn.length);
            else if ((scn.modificationType & SC_MOD_DELETETEXT) != 0)
                QsciAccessibleScintillaBase::textDeleted(qsb, scn.position,
                        scn.text, scn.length);
#endif

            // Give some protection to the Python bindings.
            if (scn.text && (scn.modificationType & (SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT)) != 0)
            {
                text = new char[scn.length + 1];
                memcpy(text, scn.text, scn.length);
                text[scn.length] = '\0';
            }
            else
            {
                text = 0;
            }

            emit qsb->SCN_MODIFIED(scn.position, scn.modificationType, text,
                    scn.length, scn.linesAdded, scn.line, scn.foldLevelNow,
                    scn.foldLevelPrev, scn.token, scn.annotationLinesAdded);

            if (text)
                delete[] text;

            break;
        }

    case SCN_MODIFYATTEMPTRO:
        emit qsb->SCN_MODIFYATTEMPTRO();
        break;

    case SCN_NEEDSHOWN:
        emit qsb->SCN_NEEDSHOWN(scn.position, scn.length);
        break;

    case SCN_PAINTED:
        emit qsb->SCN_PAINTED();
        break;

    case SCN_SAVEPOINTLEFT:
        emit qsb->SCN_SAVEPOINTLEFT();
        break;

    case SCN_SAVEPOINTREACHED:
        emit qsb->SCN_SAVEPOINTREACHED();
        break;

    case SCN_STYLENEEDED:
        emit qsb->SCN_STYLENEEDED(scn.position);
        break;

    case SCN_UPDATEUI:
#if !defined(QT_NO_ACCESSIBILITY)
        QsciAccessibleScintillaBase::updated(qsb);
#endif
        emit qsb->SCN_UPDATEUI(scn.updated);
        break;

    case SCN_USERLISTSELECTION:
        emit qsb->SCN_USERLISTSELECTION(scn.text, scn.listType, scn.ch,
                scn.listCompletionMethod, scn.position);
        emit qsb->SCN_USERLISTSELECTION(scn.text, scn.listType, scn.ch,
                scn.listCompletionMethod);
        emit qsb->SCN_USERLISTSELECTION(scn.text, scn.listType);
        break;

    case SCN_ZOOM:
        emit qsb->SCN_ZOOM();
        break;

    default:
        qWarning("Unknown notification: %u", scn.nmhdr.code);
    }
}


// Convert a selection to mime data.
QMimeData *QsciScintillaQt::mimeSelection(
        const Scintilla::SelectionText &text) const
{
    return qsb->toMimeData(QByteArray(text.Data()), text.rectangular);
}


// Copy the selected text to the clipboard.
void QsciScintillaQt::CopyToClipboard(
        const Scintilla::SelectionText &selectedText)
{
    QApplication::clipboard()->setMimeData(mimeSelection(selectedText));
}


// Implement copy.
void QsciScintillaQt::Copy()
{
    if (!sel.Empty())
    {
        Scintilla::SelectionText text;

        CopySelectionRange(&text);
        CopyToClipboard(text);
    }
}


// Implement pasting text.
void QsciScintillaQt::Paste()
{
    pasteFromClipboard(QClipboard::Clipboard);
}


// Paste text from either the clipboard or selection.
void QsciScintillaQt::pasteFromClipboard(QClipboard::Mode mode)
{
    int len;
    const char *s;
    bool rectangular;

    const QMimeData *source = QApplication::clipboard()->mimeData(mode);

    if (!source || !qsb->canInsertFromMimeData(source))
        return;

    QByteArray text = qsb->fromMimeData(source, rectangular);
    len = text.length();
    s = text.data();

    std::string dest = Scintilla::Document::TransformLineEnds(s, len,
            pdoc->eolMode);

    Scintilla::SelectionText selText;
    selText.Copy(dest, (IsUnicodeMode() ? SC_CP_UTF8 : 0),
            vs.styles[STYLE_DEFAULT].characterSet, rectangular, false);

    Scintilla::UndoGroup ug(pdoc);

    ClearSelection();
    InsertPasteShape(selText.Data(), selText.Length(),
            selText.rectangular ? pasteRectangular : pasteStream);
    EnsureCaretVisible();
}


// Create a call tip window.
void QsciScintillaQt::CreateCallTipWindow(Scintilla::PRectangle rc)
{
    if (!ct.wCallTip.Created())
        ct.wCallTip = new QsciSciCallTip(qsb, this);

    QsciSciCallTip *w = reinterpret_cast<QsciSciCallTip *>(ct.wCallTip.GetID());

    w->resize(rc.right - rc.left, rc.bottom - rc.top);
    ct.wCallTip.Show();
}


// Add an item to the right button menu.
void QsciScintillaQt::AddToPopUp(const char *label, int cmd, bool enabled)
{
    QsciSciPopup *pm = static_cast<QsciSciPopup *>(popup.GetID());

    if (*label)
        pm->addItem(qApp->translate("ContextMenu", label), cmd, enabled, this);
    else
        pm->addSeparator();
}


// Claim the (primary) selection.
void QsciScintillaQt::ClaimSelection()
{
    QClipboard *cb = QApplication::clipboard();
    bool isSel = !sel.Empty();

    if (cb->supportsSelection())
    {
        if (isSel)
        {
            Scintilla::SelectionText text;

            CopySelectionRange(&text);

            if (text.Data())
                cb->setMimeData(mimeSelection(text), QClipboard::Selection);

            primarySelection = true;
        }
        else
        {
            primarySelection = false;
        }
    }

#if !defined(QT_NO_ACCESSIBILITY)
    QsciAccessibleScintillaBase::selectionChanged(qsb, isSel);
#endif

    emit qsb->QSCN_SELCHANGED(isSel);
}


// Unclaim the (primary) selection.
void QsciScintillaQt::onSelectionChanged()
{
    bool new_primary = QApplication::clipboard()->ownsSelection();

    if (primarySelection != new_primary)
    {
        primarySelection = new_primary;
        qsb->viewport()->update();
    }
}


// Implemented to provide compatibility with the Windows version.
sptr_t QsciScintillaQt::DirectFunction(QsciScintillaQt *sciThis, unsigned int iMessage,
        uptr_t wParam, sptr_t lParam)
{
    return sciThis->WndProc(iMessage,wParam,lParam);
}


// Draw the contents of the widget.
void QsciScintillaQt::paintEvent(QPaintEvent *e)
{
    Scintilla::Surface *sw;

    const QRect &qr = e->rect();

    rcPaint.left = qr.left();
    rcPaint.top = qr.top();
    rcPaint.right = qr.right() + 1;
    rcPaint.bottom = qr.bottom() + 1;

    Scintilla::PRectangle rcClient = GetClientRectangle();
    paintingAllText = rcPaint.Contains(rcClient);

    sw = Scintilla::Surface::Allocate(SC_TECHNOLOGY_DEFAULT);
    if (!sw)
        return;

    QPainter painter(qsb->viewport());

    paintState = painting;
    sw->Init(&painter);
    sw->SetUnicodeMode(CodePage() == SC_CP_UTF8);
    Paint(sw, rcPaint);

    delete sw;

    // If the painting area was insufficient to cover the new style or brace
    // highlight positions then repaint the whole thing.
    if (paintState == paintAbandoned)
    {
        // Do a full re-paint immediately.  This may only be needed on OS X (to
        // avoid flicker).
        paintingAllText = true;

        sw = Scintilla::Surface::Allocate(SC_TECHNOLOGY_DEFAULT);
        if (!sw)
            return;

        QPainter painter(qsb->viewport());

        paintState = painting;
        sw->Init(&painter);
        sw->SetUnicodeMode(CodePage() == SC_CP_UTF8);
        Paint(sw, rcPaint);

        delete sw;

        qsb->viewport()->update();
    }

    paintState = notPainting;
}


// Re-implemented to drive the tickers.
void QsciScintillaQt::timerEvent(QTimerEvent *e)
{
    for (int i = 0; i <= static_cast<int>(tickPlatform); ++i)
        if (timers[i] == e->timerId())
            TickFor(static_cast<TickReason>(i));
}


// Re-implemented to say we support fine tickers.
bool QsciScintillaQt::FineTickerAvailable()
{
    return true;
}


// Re-implemented to stop a ticker.
void QsciScintillaQt::FineTickerCancel(TickReason reason)
{
    int &ticker = timers[static_cast<int>(reason)];

    if (ticker != 0)
    {
        killTimer(ticker);
        ticker = 0;
    }
}


// Re-implemented to check if a particular ticker is running.
bool QsciScintillaQt::FineTickerRunning(TickReason reason)
{
    return (timers[static_cast<int>(reason)] != 0);
}


// Re-implemented to start a ticker.
void QsciScintillaQt::FineTickerStart(TickReason reason, int ms, int)
{
    int &ticker = timers[static_cast<int>(reason)];

    if (ticker != 0)
        killTimer(ticker);

    ticker = startTimer(ms);
}


// Re-implemented to support idle processing.
bool QsciScintillaQt::SetIdle(bool on)
{
    if (on)
    {
        if (!idler.state)
        {
            QTimer *timer = reinterpret_cast<QTimer *>(idler.idlerID);

            if (!timer)
            {
                idler.idlerID = timer = new QTimer(this);
                connect(timer, SIGNAL(timeout()), this, SLOT(onIdle()));
            }

            timer->start(0);
            idler.state = true;
        }
    }
    else if (idler.state)
    {
        reinterpret_cast<QTimer *>(idler.idlerID)->stop();
        idler.state = false;
    }

    return true;
}


// Invoked to trigger any idle processing.
void QsciScintillaQt::onIdle()
{
    if (!Idle())
        SetIdle(false);
}
