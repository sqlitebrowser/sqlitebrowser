// This module implements the "official" low-level API.
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


#include "Qsci/qsciscintillabase.h"

#include <QApplication>
#include <QClipboard>
#include <QColor>
#include <QContextMenuEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QDragLeaveEvent>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QList>
#include <QMimeData>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QScrollBar>
#include <QStyle>
#include <QTextCodec>

#include "ScintillaQt.h"


// The #defines in Scintilla.h and the enums in qsciscintillabase.h conflict
// (because we want to use the same names) so we have to undefine those we use
// in this file.
#undef  SCI_SETCARETPERIOD
#undef  SCK_DOWN
#undef  SCK_UP
#undef  SCK_LEFT
#undef  SCK_RIGHT
#undef  SCK_HOME
#undef  SCK_END
#undef  SCK_PRIOR
#undef  SCK_NEXT
#undef  SCK_DELETE
#undef  SCK_INSERT
#undef  SCK_ESCAPE
#undef  SCK_BACK
#undef  SCK_TAB
#undef  SCK_RETURN
#undef  SCK_ADD
#undef  SCK_SUBTRACT
#undef  SCK_DIVIDE
#undef  SCK_WIN
#undef  SCK_RWIN
#undef  SCK_MENU


// Remember if we have linked the lexers.
static bool lexersLinked = false;

// The list of instances.
static QList<QsciScintillaBase *> poolList;

// Mime support.
static const QLatin1String mimeTextPlain("text/plain");
static const QLatin1String mimeRectangularWin("MSDEVColumnSelect");
static const QLatin1String mimeRectangular("text/x-qscintilla-rectangular");

#if (QT_VERSION >= 0x040200 && QT_VERSION < 0x050000 && defined(Q_OS_MAC)) || (QT_VERSION >= 0x050200 && defined(Q_OS_OSX))
extern void initialiseRectangularPasteboardMime();
#endif

// The ctor.
QsciScintillaBase::QsciScintillaBase(QWidget *parent)
    : QAbstractScrollArea(parent), preeditPos(-1), preeditNrBytes(0)
#if QT_VERSION >= 0x050000
        , clickCausedFocus(false)
#endif
{
    connectVerticalScrollBar();
    connectHorizontalScrollBar();

    setAcceptDrops(true);
    setFocusPolicy(Qt::WheelFocus);
    setAttribute(Qt::WA_KeyCompression);
    setAttribute(Qt::WA_InputMethodEnabled);
#if QT_VERSION >= 0x050100
    setInputMethodHints(
            Qt::ImhNoAutoUppercase|Qt::ImhNoPredictiveText|Qt::ImhMultiLine);
#elif QT_VERSION >= 0x040600
    setInputMethodHints(Qt::ImhNoAutoUppercase|Qt::ImhNoPredictiveText);
#endif

    viewport()->setBackgroundRole(QPalette::Base);
    viewport()->setMouseTracking(true);
    viewport()->setAttribute(Qt::WA_NoSystemBackground);

    triple_click.setSingleShot(true);

#if (QT_VERSION >= 0x040200 && QT_VERSION < 0x050000 && defined(Q_OS_MAC)) || (QT_VERSION >= 0x050200 && defined(Q_OS_OSX))
    initialiseRectangularPasteboardMime();
#endif

    sci = new QsciScintillaQt(this);

    SendScintilla(SCI_SETCARETPERIOD, QApplication::cursorFlashTime() / 2);

    // Make sure the lexers are linked in.
    if (!lexersLinked)
    {
        Scintilla_LinkLexers();
        lexersLinked = true;
    }

    QClipboard *cb = QApplication::clipboard();

    if (cb->supportsSelection())
        connect(cb, SIGNAL(selectionChanged()), SLOT(handleSelection()));

    // Add it to the pool.
    poolList.append(this);
}


// The dtor.
QsciScintillaBase::~QsciScintillaBase()
{
    // The QsciScintillaQt object isn't a child so delete it explicitly.
    delete sci;

    // Remove it from the pool.
    poolList.removeAt(poolList.indexOf(this));
}


// Return an instance from the pool.
QsciScintillaBase *QsciScintillaBase::pool()
{
    return poolList.first();
}


// Tell Scintilla to update the scroll bars.  Scintilla should be doing this
// itself.
void QsciScintillaBase::setScrollBars()
{
    sci->SetScrollBars();
}


// Send a message to the real Scintilla widget using the low level Scintilla
// API.
long QsciScintillaBase::SendScintilla(unsigned int msg, unsigned long wParam,
        long lParam) const
{
    return sci->WndProc(msg, wParam, lParam);
}


// Overloaded message send.
long QsciScintillaBase::SendScintilla(unsigned int msg, unsigned long wParam,
        void *lParam) const
{
    return sci->WndProc(msg, wParam, reinterpret_cast<sptr_t>(lParam));
}


// Overloaded message send.
long QsciScintillaBase::SendScintilla(unsigned int msg, unsigned long wParam,
        const char *lParam) const
{
    return sci->WndProc(msg, wParam, reinterpret_cast<sptr_t>(lParam));
}


// Overloaded message send.
long QsciScintillaBase::SendScintilla(unsigned int msg,
        const char *lParam) const
{
    return sci->WndProc(msg, static_cast<uptr_t>(0),
            reinterpret_cast<sptr_t>(lParam));
}


// Overloaded message send.
long QsciScintillaBase::SendScintilla(unsigned int msg, const char *wParam,
        const char *lParam) const
{
    return sci->WndProc(msg, reinterpret_cast<uptr_t>(wParam),
            reinterpret_cast<sptr_t>(lParam));
}


// Overloaded message send.
long QsciScintillaBase::SendScintilla(unsigned int msg, long wParam) const
{
    return sci->WndProc(msg, static_cast<uptr_t>(wParam),
            static_cast<sptr_t>(0));
}


// Overloaded message send.
long QsciScintillaBase::SendScintilla(unsigned int msg, int wParam) const
{
    return sci->WndProc(msg, static_cast<uptr_t>(wParam),
            static_cast<sptr_t>(0));
}


// Overloaded message send.
long QsciScintillaBase::SendScintilla(unsigned int msg, long cpMin, long cpMax,
        char *lpstrText) const
{
    Sci_TextRange tr;

    tr.chrg.cpMin = cpMin;
    tr.chrg.cpMax = cpMax;
    tr.lpstrText = lpstrText;

    return sci->WndProc(msg, static_cast<uptr_t>(0),
            reinterpret_cast<sptr_t>(&tr));
}


// Overloaded message send.
long QsciScintillaBase::SendScintilla(unsigned int msg, unsigned long wParam,
        const QColor &col) const
{
    sptr_t lParam = (col.blue() << 16) | (col.green() << 8) | col.red();

    return sci->WndProc(msg, wParam, lParam);
}


// Overloaded message send.
long QsciScintillaBase::SendScintilla(unsigned int msg, const QColor &col) const
{
    uptr_t wParam = (col.blue() << 16) | (col.green() << 8) | col.red();

    return sci->WndProc(msg, wParam, static_cast<sptr_t>(0));
}


// Overloaded message send.
long QsciScintillaBase::SendScintilla(unsigned int msg, unsigned long wParam,
        QPainter *hdc, const QRect &rc, long cpMin, long cpMax) const
{
    Sci_RangeToFormat rf;

    rf.hdc = rf.hdcTarget = reinterpret_cast<QSCI_SCI_NAMESPACE(SurfaceID)>(hdc);

    rf.rc.left = rc.left();
    rf.rc.top = rc.top();
    rf.rc.right = rc.right() + 1;
    rf.rc.bottom = rc.bottom() + 1;

    rf.chrg.cpMin = cpMin;
    rf.chrg.cpMax = cpMax;

    return sci->WndProc(msg, wParam, reinterpret_cast<sptr_t>(&rf));
}


// Overloaded message send.
long QsciScintillaBase::SendScintilla(unsigned int msg, unsigned long wParam,
        const QPixmap &lParam) const
{
    return sci->WndProc(msg, wParam, reinterpret_cast<sptr_t>(&lParam));
}


// Overloaded message send.
long QsciScintillaBase::SendScintilla(unsigned int msg, unsigned long wParam,
        const QImage &lParam) const
{
    return sci->WndProc(msg, wParam, reinterpret_cast<sptr_t>(&lParam));
}


// Send a message to the real Scintilla widget using the low level Scintilla
// API that returns a pointer result.
void *QsciScintillaBase::SendScintillaPtrResult(unsigned int msg) const
{
    return reinterpret_cast<void *>(sci->WndProc(msg, static_cast<uptr_t>(0),
            static_cast<sptr_t>(0)));
}


// Re-implemented to handle the context menu.
void QsciScintillaBase::contextMenuEvent(QContextMenuEvent *e)
{
    sci->ContextMenu(QSCI_SCI_NAMESPACE(Point)(e->globalX(), e->globalY()));
}


// Re-implemented to tell the widget it has the focus.
void QsciScintillaBase::focusInEvent(QFocusEvent *e)
{
    sci->SetFocusState(true);

#if QT_VERSION >= 0x050000
    clickCausedFocus = (e->reason() == Qt::MouseFocusReason);
#endif

    QAbstractScrollArea::focusInEvent(e);
}


// Re-implemented to tell the widget it has lost the focus.
void QsciScintillaBase::focusOutEvent(QFocusEvent *e)
{
    if (e->reason() == Qt::ActiveWindowFocusReason)
    {
        // Only tell Scintilla we have lost focus if the new active window
        // isn't our auto-completion list.  This is probably only an issue on
        // Linux and there are still problems because subsequent focus out
        // events don't always seem to get generated (at least with Qt5).

        QWidget *aw = QApplication::activeWindow();

        if (!aw || aw->parent() != this || !aw->inherits("QsciSciListBox"))
            sci->SetFocusState(false);
    }
    else
    {
        sci->SetFocusState(false);
    }

    QAbstractScrollArea::focusOutEvent(e);
}


// Re-implemented to make sure tabs are passed to the editor.
bool QsciScintillaBase::focusNextPrevChild(bool next)
{
    if (!sci->pdoc->IsReadOnly())
        return false;

    return QAbstractScrollArea::focusNextPrevChild(next);
}


// Handle the selection changing.
void QsciScintillaBase::handleSelection()
{
    if (!QApplication::clipboard()->ownsSelection())
        sci->UnclaimSelection();
}


// Handle key presses.
void QsciScintillaBase::keyPressEvent(QKeyEvent *e)
{
    int modifiers = 0;

    if (e->modifiers() & Qt::ShiftModifier)
        modifiers |= SCMOD_SHIFT;

    if (e->modifiers() & Qt::ControlModifier)
        modifiers |= SCMOD_CTRL;

    if (e->modifiers() & Qt::AltModifier)
        modifiers |= SCMOD_ALT;

    if (e->modifiers() & Qt::MetaModifier)
        modifiers |= SCMOD_META;

    int key = commandKey(e->key(), modifiers);

    if (key)
    {
        bool consumed = false;

        sci->KeyDownWithModifiers(key, modifiers, &consumed);

        if (consumed)
        {
            e->accept();
            return;
        }
    }

    QString text = e->text();

    if (!text.isEmpty() && text[0].isPrint())
    {
        ScintillaBytes bytes = textAsBytes(text);
        sci->AddCharUTF(bytes.data(), bytes.length());
        e->accept();
    }
    else
    {
        QAbstractScrollArea::keyPressEvent(e);
    }
}


// Map a Qt key to a valid Scintilla command key, or 0 if none.
int QsciScintillaBase::commandKey(int qt_key, int &modifiers)
{
    int key;

    switch (qt_key)
    {
    case Qt::Key_Down:
        key = SCK_DOWN;
        break;

    case Qt::Key_Up:
        key = SCK_UP;
        break;

    case Qt::Key_Left:
        key = SCK_LEFT;
        break;

    case Qt::Key_Right:
        key = SCK_RIGHT;
        break;

    case Qt::Key_Home:
        key = SCK_HOME;
        break;

    case Qt::Key_End:
        key = SCK_END;
        break;

    case Qt::Key_PageUp:
        key = SCK_PRIOR;
        break;

    case Qt::Key_PageDown:
        key = SCK_NEXT;
        break;

    case Qt::Key_Delete:
        key = SCK_DELETE;
        break;

    case Qt::Key_Insert:
        key = SCK_INSERT;
        break;

    case Qt::Key_Escape:
        key = SCK_ESCAPE;
        break;

    case Qt::Key_Backspace:
        key = SCK_BACK;
        break;

    case Qt::Key_Tab:
        key = SCK_TAB;
        break;

    case Qt::Key_Backtab:
        // Scintilla assumes a backtab is shift-tab.
        key = SCK_TAB;
        modifiers |= SCMOD_SHIFT;
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        key = SCK_RETURN;
        break;

    case Qt::Key_Super_L:
        key = SCK_WIN;
        break;

    case Qt::Key_Super_R:
        key = SCK_RWIN;
        break;

    case Qt::Key_Menu:
        key = SCK_MENU;
        break;

    default:
        if ((key = qt_key) > 0x7f)
            key = 0;
    }

    return key;
}


// Encode a QString as bytes.
QsciScintillaBase::ScintillaBytes QsciScintillaBase::textAsBytes(const QString &text) const
{
    if (sci->IsUnicodeMode())
        return text.toUtf8();

    return text.toLatin1();
}


// Decode bytes as a QString.
QString QsciScintillaBase::bytesAsText(const char *bytes) const
{
    if (sci->IsUnicodeMode())
        return QString::fromUtf8(bytes);

    return QString::fromLatin1(bytes);
}


// Handle a mouse button double click.
void QsciScintillaBase::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
    {
        e->ignore();
        return;
    }

    setFocus();

    // Make sure Scintilla will interpret this as a double-click.
    unsigned clickTime = sci->lastClickTime + QSCI_SCI_NAMESPACE(Platform)::DoubleClickTime() - 1;

    bool shift = e->modifiers() & Qt::ShiftModifier;
    bool ctrl = e->modifiers() & Qt::ControlModifier;
    bool alt = e->modifiers() & Qt::AltModifier;

    sci->ButtonDown(QSCI_SCI_NAMESPACE(Point)(e->x(), e->y()), clickTime,
            shift, ctrl, alt);

    // Remember the current position and time in case it turns into a triple
    // click.
    triple_click_at = e->globalPos();
    triple_click.start(QApplication::doubleClickInterval());
}


// Handle a mouse move.
void QsciScintillaBase::mouseMoveEvent(QMouseEvent *e)
{
    sci->ButtonMove(QSCI_SCI_NAMESPACE(Point)(e->x(), e->y()));
}


// Handle a mouse button press.
void QsciScintillaBase::mousePressEvent(QMouseEvent *e)
{
    setFocus();

    QSCI_SCI_NAMESPACE(Point) pt(e->x(), e->y());

    if (e->button() == Qt::LeftButton)
    {
        unsigned clickTime;

        // It is a triple click if the timer is running and the mouse hasn't
        // moved too much.
        if (triple_click.isActive() && (e->globalPos() - triple_click_at).manhattanLength() < QApplication::startDragDistance())
            clickTime = sci->lastClickTime + QSCI_SCI_NAMESPACE(Platform)::DoubleClickTime() - 1;
        else
            clickTime = sci->lastClickTime + QSCI_SCI_NAMESPACE(Platform)::DoubleClickTime() + 1;

        triple_click.stop();

        // Scintilla uses the Alt modifier to initiate rectangular selection.
        // However the GTK port (under X11, not Windows) uses the Control
        // modifier (by default, although it is configurable).  It does this
        // because most X11 window managers hijack Alt-drag to move the window.
        // We do the same, except that (for the moment at least) we don't allow
        // the modifier to be configured.
        bool shift = e->modifiers() & Qt::ShiftModifier;
        bool ctrl = e->modifiers() & Qt::ControlModifier;
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
        bool alt = e->modifiers() & Qt::AltModifier;
#else
        bool alt = ctrl;
#endif

        sci->ButtonDown(pt, clickTime, shift, ctrl, alt);
    }
    else if (e->button() == Qt::MidButton)
    {
        QClipboard *cb = QApplication::clipboard();

        if (cb->supportsSelection())
        {
            int pos = sci->PositionFromLocation(pt);

            sci->sel.Clear();
            sci->SetSelection(pos, pos);

            sci->pasteFromClipboard(QClipboard::Selection);
        }
    }
}


// Handle a mouse button releases.
void QsciScintillaBase::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
        return;

    QSCI_SCI_NAMESPACE(Point) pt(e->x(), e->y());

    if (sci->HaveMouseCapture())
    {
        bool ctrl = e->modifiers() & Qt::ControlModifier;

        sci->ButtonUp(pt, 0, ctrl);
    }

#if QT_VERSION >= 0x050000
    if (!sci->pdoc->IsReadOnly() && !sci->PointInSelMargin(pt) && qApp->autoSipEnabled())
    {
        QStyle::RequestSoftwareInputPanel rsip = QStyle::RequestSoftwareInputPanel(style()->styleHint(QStyle::SH_RequestSoftwareInputPanel));

        if (!clickCausedFocus || rsip == QStyle::RSIP_OnMouseClick)
            qApp->inputMethod()->show();
    }

    clickCausedFocus = false;
#endif
}


// Handle paint events.
void QsciScintillaBase::paintEvent(QPaintEvent *e)
{
    sci->paintEvent(e);
}


// Handle resize events.
void QsciScintillaBase::resizeEvent(QResizeEvent *)
{
    sci->ChangeSize();
}


// Re-implemented to suppress the default behaviour as Scintilla works at a
// more fundamental level.  Note that this means that replacing the scrollbars
// with custom versions does not work.
void QsciScintillaBase::scrollContentsBy(int, int)
{
}


// Handle the vertical scrollbar.
void QsciScintillaBase::handleVSb(int value)
{
    sci->ScrollTo(value);
}


// Handle the horizontal scrollbar.
void QsciScintillaBase::handleHSb(int value)
{
    sci->HorizontalScrollTo(value);
}


// Handle drag enters.
void QsciScintillaBase::dragEnterEvent(QDragEnterEvent *e)
{
    QsciScintillaBase::dragMoveEvent(e);
}


// Handle drag leaves.
void QsciScintillaBase::dragLeaveEvent(QDragLeaveEvent *)
{
    sci->SetDragPosition(QSCI_SCI_NAMESPACE(SelectionPosition)());
}


// Handle drag moves.
void QsciScintillaBase::dragMoveEvent(QDragMoveEvent *e)
{
    sci->SetDragPosition(
            sci->SPositionFromLocation(
                    QSCI_SCI_NAMESPACE(Point)(e->pos().x(), e->pos().y()),
                    false, false, sci->UserVirtualSpace()));

    acceptAction(e);
}


// Handle drops.
void QsciScintillaBase::dropEvent(QDropEvent *e)
{
    bool moving;
    int len;
    const char *s;
    bool rectangular;

    acceptAction(e);

    if (!e->isAccepted())
        return;

    moving = (e->dropAction() == Qt::MoveAction);

    QByteArray text = fromMimeData(e->mimeData(), rectangular);
    len = text.length();
    s = text.data();

    std::string dest = QSCI_SCI_NAMESPACE(Document)::TransformLineEnds(s, len,
                sci->pdoc->eolMode);

    sci->DropAt(sci->posDrop, dest.c_str(), dest.length(), moving,
            rectangular);

    sci->Redraw();
}


void QsciScintillaBase::acceptAction(QDropEvent *e)
{
    if (sci->pdoc->IsReadOnly() || !canInsertFromMimeData(e->mimeData()))
        e->ignore();
    else
        e->acceptProposedAction();
}


// See if a MIME data object can be decoded.
bool QsciScintillaBase::canInsertFromMimeData(const QMimeData *source) const
{
    return source->hasFormat(mimeTextPlain);
}


// Create text from a MIME data object.
QByteArray QsciScintillaBase::fromMimeData(const QMimeData *source, bool &rectangular) const
{
    // See if it is rectangular.  We try all of the different formats that
    // Scintilla supports in case we are working across different platforms.
    if (source->hasFormat(mimeRectangularWin))
        rectangular = true;
    else if (source->hasFormat(mimeRectangular))
        rectangular = true;
    else
        rectangular = false;

    // Note that we don't support Scintilla's hack of adding a '\0' as Qt
    // strips it off under the covers when pasting from another process.
    QString utf8 = source->text();
    QByteArray text;

    if (sci->IsUnicodeMode())
        text = utf8.toUtf8();
    else
        text = utf8.toLatin1();

    return text;
}


// Create a MIME data object for some text.
QMimeData *QsciScintillaBase::toMimeData(const QByteArray &text, bool rectangular) const
{
    QMimeData *mime = new QMimeData;

    QString utf8;

    if (sci->IsUnicodeMode())
        utf8 = QString::fromUtf8(text.constData(), text.size());
    else
        utf8 = QString::fromLatin1(text.constData(), text.size());

    mime->setText(utf8);

    if (rectangular)
    {
        // Use the platform specific "standard" for specifying a rectangular
        // selection.
#if defined(Q_OS_WIN)
        mime->setData(mimeRectangularWin, QByteArray());
#else
        mime->setData(mimeRectangular, QByteArray());
#endif
    }

    return mime;
}


// Connect up the vertical scroll bar.
void QsciScintillaBase::connectVerticalScrollBar()
{
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)),
            SLOT(handleVSb(int)));
}


// Connect up the horizontal scroll bar.
void QsciScintillaBase::connectHorizontalScrollBar()
{
    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)),
            SLOT(handleHSb(int)));
}


//! Replace the vertical scroll bar.
void QsciScintillaBase::replaceVerticalScrollBar(QScrollBar *scrollBar)
{
    setVerticalScrollBar(scrollBar);
    connectVerticalScrollBar();
}


// Replace the horizontal scroll bar.
void QsciScintillaBase::replaceHorizontalScrollBar(QScrollBar *scrollBar)
{
    setHorizontalScrollBar(scrollBar);
    connectHorizontalScrollBar();
}
