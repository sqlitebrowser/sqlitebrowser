// This module implements the portability layer for the Qt port of Scintilla.
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


#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <qapplication.h>
#include <qcursor.h>
#include <qdatetime.h>
#include <qfont.h>
#include <qimage.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpolygon.h>
#include <qscreen.h>
#include <qstring.h>
#include <qtextlayout.h>
#include <qwidget.h>

#if !defined(Q_OS_WASM)
#include <qlibrary.h>
#endif

#include "Platform.h"
#include "XPM.h"

#include "Qsci/qsciscintillabase.h"
#include "SciClasses.h"

#include "FontQuality.h"


namespace Scintilla {

// Type convertors.
static QFont *PFont(FontID fid)
{
    return reinterpret_cast<QFont *>(fid);
}

static QWidget *PWindow(WindowID wid)
{
    return reinterpret_cast<QWidget *>(wid);
}

static QsciSciPopup *PMenu(MenuID mid)
{
    return reinterpret_cast<QsciSciPopup *>(mid);
}


// Font management.
Font::Font() noexcept : fid(0)
{
}

Font::~Font()
{
}

void Font::Create(const FontParameters &fp)
{
    Release();

    QFont *f = new QFont();

    QFont::StyleStrategy strategy;

    switch (fp.extraFontFlag & SC_EFF_QUALITY_MASK)
    {
    case SC_EFF_QUALITY_NON_ANTIALIASED:
        strategy = QFont::NoAntialias;
        break;

    case SC_EFF_QUALITY_ANTIALIASED:
        strategy = QFont::PreferAntialias;
        break;

    default:
        strategy = QFont::PreferDefault;
    }

    f->setStyleStrategy(strategy);
    f->setFamily(fp.faceName);
    f->setPointSizeF(fp.size);
    f->setItalic(fp.italic);

    // Scintilla weights are between 1 and 100, Qt5 weights are between 0 and
    // 99, and Qt6 weights match Scintilla.  A negative weight is interpreted
    // as an explicit Qt weight (ie. the back door).
#if QT_VERSION >= 0x060000
    QFont::Weight qt_weight = static_cast<QFont::Weight>(abs(fp.weight));
#else
    int qt_weight;

    if (fp.weight < 0)
        qt_weight = -fp.weight;
    else if (fp.weight <= 200)
        qt_weight = QFont::Light;
    else if (fp.weight <= QsciScintillaBase::SC_WEIGHT_NORMAL)
        qt_weight = QFont::Normal;
    else if (fp.weight <= 600)
        qt_weight = QFont::DemiBold;
    else if (fp.weight <= 850)
        qt_weight = QFont::Bold;
    else
        qt_weight = QFont::Black;
#endif

    f->setWeight(qt_weight);

    fid = f;
}

void Font::Release()
{
    if (fid)
    {
        delete PFont(fid);
        fid = 0;
    }
}


// A surface abstracts a place to draw.
class SurfaceImpl : public Surface
{
public:
    SurfaceImpl();
    virtual ~SurfaceImpl();

    void Init(WindowID wid);
    void Init(SurfaceID sid, WindowID);
    void Init(QPainter *p);
    void InitPixMap(int width, int height, Surface *sid, WindowID wid);

    void Release();
    bool Initialised() {return painter;}
    void PenColour(ColourDesired fore);
    int LogPixelsY() {return pd->logicalDpiY();}
    int DeviceHeightFont(int points) {return points;}
    void MoveTo(int x_,int y_);
    void LineTo(int x_,int y_);
    void Polygon(Point *pts, size_t npts, ColourDesired fore,
            ColourDesired back);
    void RectangleDraw(PRectangle rc, ColourDesired fore,
            ColourDesired back);
    void FillRectangle(PRectangle rc, ColourDesired back);
    void FillRectangle(PRectangle rc, Surface &surfacePattern);
    void RoundedRectangle(PRectangle rc, ColourDesired fore,
            ColourDesired back);
    void AlphaRectangle(PRectangle rc, int cornerSize, ColourDesired fill,
            int alphaFill, ColourDesired outline, int alphaOutline,
            int flags);
    void GradientRectangle(PRectangle rc, const std::vector<ColourStop> &stops,
            GradientOptions options);
    void DrawRGBAImage(PRectangle rc, int width, int height, const unsigned char *pixelsImage);
    void Ellipse(PRectangle rc, ColourDesired fore, ColourDesired back);
    void Copy(PRectangle rc, Point from, Surface &surfaceSource);

    void DrawTextNoClip(PRectangle rc, Font &font_, XYPOSITION ybase,
            const char *s, int len, ColourDesired fore, ColourDesired back);
    void DrawTextClipped(PRectangle rc, Font &font_, XYPOSITION ybase,
            const char *s, int len, ColourDesired fore, ColourDesired back);
    void DrawTextTransparent(PRectangle rc, Font &font_, XYPOSITION ybase,
            const char *s, int len, ColourDesired fore);
    void MeasureWidths(Font &font_, const char *s, int len,
            XYPOSITION *positions);
    XYPOSITION WidthText(Font &font_, const char *s, int len);
    XYPOSITION Ascent(Font &font_);
    XYPOSITION Descent(Font &font_);
    XYPOSITION InternalLeading(Font &font_) {Q_UNUSED(font_); return 0;}
    XYPOSITION Height(Font &font_);
    XYPOSITION AverageCharWidth(Font &font_);

    void SetClip(PRectangle rc);
    void FlushCachedState();

    void SetUnicodeMode(bool unicodeMode_) {unicodeMode = unicodeMode_;}
    void SetDBCSMode(int codePage) {Q_UNUSED(codePage);}

    void DrawXPM(PRectangle rc, const XPM *xpm);

private:
    void drawRect(const PRectangle &rc);
    void drawText(const PRectangle &rc, Font &font_, XYPOSITION ybase,
            const char *s, int len, ColourDesired fore);
    static QFont convertQFont(Font &font);
    QFontMetricsF metrics(Font &font_);
    QString convertText(const char *s, int len);
    static QColor convertQColor(const ColourDesired &col,
            unsigned alpha = 255);

    bool unicodeMode;
    QPaintDevice *pd;
    QPainter *painter;
    bool my_resources;
    int pen_x, pen_y;
};

Surface *Surface::Allocate(int)
{
    return new SurfaceImpl;
}

SurfaceImpl::SurfaceImpl()
    : unicodeMode(false), pd(0), painter(0), my_resources(false), pen_x(0),
      pen_y(0)
{
}

SurfaceImpl::~SurfaceImpl()
{
    Release();
}

void SurfaceImpl::Init(WindowID wid)
{
    Release();

    pd = reinterpret_cast<QWidget *>(wid);
}

void SurfaceImpl::Init(SurfaceID sid, WindowID)
{
    Release();

    // This method, and the SurfaceID type, is only used when printing.  As it
    // is actually a void * we pass (when using SCI_FORMATRANGE) a pointer to a
    // QPainter rather than a pointer to a SurfaceImpl as might be expected.
    QPainter *p = reinterpret_cast<QPainter *>(sid);

    pd = p->device();
    painter = p;
}

void SurfaceImpl::Init(QPainter *p)
{
    Release();

    pd = p->device();
    painter = p;
}

void SurfaceImpl::InitPixMap(int width, int height, Surface *sid, WindowID wid)
{
    Release();

    int dpr = PWindow(wid)->devicePixelRatio();
    QPixmap *pixmap = new QPixmap(width * dpr, height * dpr);
    pixmap->setDevicePixelRatio(dpr);

    pd = pixmap;

    painter = new QPainter(pd);
    my_resources = true;

    SetUnicodeMode(static_cast<SurfaceImpl *>(sid)->unicodeMode);
}

void SurfaceImpl::Release()
{
    if (my_resources)
    {
        if (painter)
            delete painter;

        if (pd)
            delete pd;

        my_resources = false;
    }

    painter = 0;
    pd = 0;
}

void SurfaceImpl::MoveTo(int x_, int y_)
{
    Q_ASSERT(painter);

    pen_x = x_;
    pen_y = y_;
}

void SurfaceImpl::LineTo(int x_, int y_)
{
    Q_ASSERT(painter);

    painter->drawLine(pen_x, pen_y, x_, y_);

    pen_x = x_;
    pen_y = y_;
}

void SurfaceImpl::PenColour(ColourDesired fore)
{
    Q_ASSERT(painter);

    painter->setPen(convertQColor(fore));
}

void SurfaceImpl::Polygon(Point *pts, size_t npts, ColourDesired fore,
        ColourDesired back)
{
    Q_ASSERT(painter);

    QPolygonF qpts(npts);

    for (size_t i = 0; i < npts; ++i)
        qpts[i] = QPointF(pts[i].x, pts[i].y);

    painter->setPen(convertQColor(fore));
    painter->setBrush(convertQColor(back));
    painter->drawPolygon(qpts);
}

void SurfaceImpl::RectangleDraw(PRectangle rc, ColourDesired fore,
        ColourDesired back)
{
    Q_ASSERT(painter);

    painter->setPen(convertQColor(fore));
    painter->setBrush(convertQColor(back));
    drawRect(rc);
}

void SurfaceImpl::FillRectangle(PRectangle rc, ColourDesired back)
{
    Q_ASSERT(painter);

    painter->setPen(Qt::NoPen);
    painter->setBrush(convertQColor(back));
    drawRect(rc);
}

void SurfaceImpl::FillRectangle(PRectangle rc, Surface &surfacePattern)
{
    Q_ASSERT(painter);

    SurfaceImpl &si = static_cast<SurfaceImpl &>(surfacePattern);
    QPixmap *pm = static_cast<QPixmap *>(si.pd);

    if (pm)
    {
        QBrush brsh(Qt::black, *pm);

        painter->setPen(Qt::NoPen);
        painter->setBrush(brsh);
        drawRect(rc);
    }
    else
    {
        FillRectangle(rc, ColourDesired(0));
    }
}

void SurfaceImpl::RoundedRectangle(PRectangle rc, ColourDesired fore,
        ColourDesired back)
{
    Q_ASSERT(painter);

    painter->setPen(convertQColor(fore));
    painter->setBrush(convertQColor(back));
    painter->drawRoundedRect(
            QRectF(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top),
            25, 25, Qt::RelativeSize);
}

void SurfaceImpl::AlphaRectangle(PRectangle rc, int cornerSize,
        ColourDesired fill, int alphaFill, ColourDesired outline,
        int alphaOutline, int)
{
    Q_ASSERT(painter);

    QColor outline_colour = convertQColor(outline, alphaOutline);
    QColor fill_colour = convertQColor(fill, alphaFill);

    // There was a report of Qt seeming to ignore the alpha value of the pen so
    // so we disable the pen if the outline and fill colours are the same.
    if (outline_colour == fill_colour)
        painter->setPen(Qt::NoPen);
    else
        painter->setPen(outline_colour);

    painter->setBrush(fill_colour);

    const int radius = (cornerSize ? 25 : 0);

    painter->drawRoundedRect(
            QRectF(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top),
            radius, radius, Qt::RelativeSize);
}

void SurfaceImpl::GradientRectangle(PRectangle rc,
        const std::vector<ColourStop> &stops, GradientOptions options)
{
    Q_ASSERT(painter);

    QLinearGradient gradient;

    switch (options)
    {
    case GradientOptions::leftToRight:
        gradient = QLinearGradient(rc.left, rc.top, rc.right, rc.top);
        break;

    case GradientOptions::topToBottom:
    default:
        gradient = QLinearGradient(rc.left, rc.top, rc.left, rc.bottom);
    }

    gradient.setSpread(QGradient::RepeatSpread);

    for (const ColourStop &stop : stops)
        gradient.setColorAt(stop.position,
                convertQColor(stop.colour, stop.colour.GetAlpha()));

    painter->fillRect(
            QRectF(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top),
            QBrush(gradient));
}

void SurfaceImpl::drawRect(const PRectangle &rc)
{
    painter->drawRect(
            QRectF(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top));
}

void SurfaceImpl::Ellipse(PRectangle rc, ColourDesired fore,
        ColourDesired back)
{
    Q_ASSERT(painter);

    painter->setPen(convertQColor(fore));
    painter->setBrush(convertQColor(back));
    painter->drawEllipse(
            QRectF(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top));
}

void SurfaceImpl::Copy(PRectangle rc, Point from, Surface &surfaceSource)
{
    Q_ASSERT(painter);

    SurfaceImpl &si = static_cast<SurfaceImpl &>(surfaceSource);

    if (si.pd)
    {
        QPixmap *pm = static_cast<QPixmap *>(si.pd);
        qreal x = from.x;
        qreal y = from.y;
        qreal width = rc.right - rc.left;
        qreal height = rc.bottom - rc.top;

        qreal dpr = pm->devicePixelRatio();

        x *= dpr;
        y *= dpr;
        width *= dpr;
        height *= dpr;

        painter->drawPixmap(QPointF(rc.left, rc.top), *pm,
                QRectF(x, y, width, height));
    }
}

void SurfaceImpl::DrawTextNoClip(PRectangle rc, Font &font_, XYPOSITION ybase,
        const char *s, int len, ColourDesired fore, ColourDesired back)
{
    Q_ASSERT(painter);

    FillRectangle(rc, back);
    drawText(rc, font_, ybase, s, len, fore);
}

void SurfaceImpl::DrawTextClipped(PRectangle rc, Font &font_, XYPOSITION ybase,
        const char *s, int len, ColourDesired fore, ColourDesired back)
{
    Q_ASSERT(painter);

    SetClip(rc);
    DrawTextNoClip(rc, font_, ybase, s, len, fore, back);
    painter->setClipping(false);
}

void SurfaceImpl::DrawTextTransparent(PRectangle rc, Font &font_,
        XYPOSITION ybase, const char *s, int len, ColourDesired fore)
{
    // Only draw if there is a non-space.
    for (int i = 0; i < len; ++i)
        if (s[i] != ' ')
        {
            drawText(rc, font_, ybase, s, len, fore);
            return;
        }
}

void SurfaceImpl::drawText(const PRectangle &rc, Font &font_, XYPOSITION ybase,
        const char *s, int len, ColourDesired fore)
{
    QString qs = convertText(s, len);

    QFont *f = PFont(font_.GetID());

    if (f)
        painter->setFont(*f);

    painter->setPen(convertQColor(fore));
    painter->drawText(QPointF(rc.left, ybase), qs);
}

void SurfaceImpl::DrawXPM(PRectangle rc, const XPM *xpm)
{
    Q_ASSERT(painter);

    XYPOSITION x, y;
    const QPixmap &qpm = xpm->Pixmap();

    x = rc.left + (rc.Width() - qpm.width()) / 2.0;
    y = rc.top + (rc.Height() - qpm.height()) / 2.0;

    painter->drawPixmap(QPointF(x, y), qpm);
}

void SurfaceImpl::DrawRGBAImage(PRectangle rc, int width, int height,
        const unsigned char *pixelsImage)
{
    Q_UNUSED(width);
    Q_UNUSED(height);
    Q_ASSERT(painter);

    const QImage *qim = reinterpret_cast<const QImage *>(pixelsImage);

    painter->drawImage(QPointF(rc.left, rc.top), *qim);
}

void SurfaceImpl::MeasureWidths(Font &font_, const char *s, int len,
        XYPOSITION *positions)
{
    QString qs = convertText(s, len);
    QTextLayout text_layout(qs, convertQFont(font_), pd);

    text_layout.beginLayout();
    QTextLine text_line = text_layout.createLine();
    text_layout.endLayout();

    if (unicodeMode)
    {
        int i_char = 0, i_byte = 0;;

        while (i_char < qs.size())
        {
            unsigned char byte = s[i_byte];
            int nbytes, code_units;

            // Work out character sizes by looking at the byte stream.
            if (byte >= 0xf0)
            {
                nbytes = 4;
                code_units = 2;
            }
            else
            {
                if (byte >= 0xe0)
                    nbytes = 3;
                else if (byte >= 0x80)
                    nbytes = 2;
                else
                    nbytes = 1;

                code_units = 1;
            }

            XYPOSITION position = text_line.cursorToX(i_char + code_units);

            // Set the same position for each byte of the character.
            for (int i = 0; i < nbytes && i_byte < len; ++i)
                positions[i_byte++] = position;

            i_char += code_units;
        }

        // This shouldn't be necessary...
        XYPOSITION last_position = ((i_byte > 0) ? positions[i_byte - 1] : 0);

        while (i_byte < len)
            positions[i_byte++] = last_position;
    }
    else
    {
        for (int i = 0; i < len; ++i)
            positions[i] = text_line.cursorToX(i + 1);
    }
}

XYPOSITION SurfaceImpl::WidthText(Font &font_, const char *s, int len)
{
    return metrics(font_).horizontalAdvance(convertText(s, len));

}

XYPOSITION SurfaceImpl::Ascent(Font &font_)
{
    return metrics(font_).ascent();
}

XYPOSITION SurfaceImpl::Descent(Font &font_)
{
    // Qt doesn't include the baseline in the descent, so add it.  Note that
    // a descent from Qt4 always seems to be 2 pixels larger (irrespective of
    // font or size) than the same descent from Qt3.  This means that text is a
    // little more spaced out with Qt4 - and will be more noticeable with
    // smaller fonts.
    return metrics(font_).descent() + 1;
}

XYPOSITION SurfaceImpl::Height(Font &font_)
{
    return metrics(font_).height();
}

XYPOSITION SurfaceImpl::AverageCharWidth(Font &font_)
{
    return metrics(font_).averageCharWidth();
}

void SurfaceImpl::SetClip(PRectangle rc)
{
    Q_ASSERT(painter);

    painter->setClipRect(
            QRectF(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top));
}

void SurfaceImpl::FlushCachedState()
{
}

// Return the QFont for a Font.
QFont SurfaceImpl::convertQFont(Font &font)
{
    QFont *f = PFont(font.GetID());

    if (f)
        return *f;

    return QApplication::font();
}

// Get the metrics for a font.
QFontMetricsF SurfaceImpl::metrics(Font &font_)
{
    QFont fnt = convertQFont(font_);

    return QFontMetricsF(fnt, pd);
}

// Convert a Scintilla string to a Qt Unicode string.
QString SurfaceImpl::convertText(const char *s, int len)
{
    if (unicodeMode)
        return QString::fromUtf8(s, len);

    return QString::fromLatin1(s, len);
}


// Convert a Scintilla colour, and alpha component, to a Qt QColor.
QColor SurfaceImpl::convertQColor(const ColourDesired &col, unsigned alpha)
{
    int c = col.AsInteger();

    unsigned r = c & 0xff;
    unsigned g = (c >> 8) & 0xff;
    unsigned b = (c >> 16) & 0xff;

    return QColor(r, g, b, alpha);
}


// Window (widget) management.
Window::~Window()
{
}

void Window::Destroy()
{
    QWidget *w = PWindow(wid);

    if (w)
    {
        // Delete the widget next time round the event loop rather than
        // straight away.  This gets round a problem when auto-completion lists
        // are cancelled after an entry has been double-clicked, ie. the list's
        // dtor is called from one of the list's slots.  There are other ways
        // around the problem but this is the simplest and doesn't seem to
        // cause problems of its own.
        w->deleteLater();
        wid = 0;
    }
}

PRectangle Window::GetPosition() const
{
    QWidget *w = PWindow(wid);

    // Before any size allocated pretend its big enough not to be scrolled.
    PRectangle rc(0,0,5000,5000);

    if (w)
    {
        const QRect &r = w->geometry();

        rc.right = r.right() - r.left() + 1;
        rc.bottom = r.bottom() - r.top() + 1;
    }

    return rc;
}

void Window::SetPosition(PRectangle rc)
{
    PWindow(wid)->setGeometry(rc.left, rc.top, rc.right - rc.left,
            rc.bottom - rc.top);
}

void Window::SetPositionRelative(PRectangle rc, const Window *relativeTo)
{
    QWidget *rel = PWindow(relativeTo->wid);
    QPoint pos = rel->mapToGlobal(rel->pos());

    int x = pos.x() + rc.left;
    int y = pos.y() + rc.top;

    PWindow(wid)->setGeometry(x, y, rc.right - rc.left, rc.bottom - rc.top);
}

PRectangle Window::GetClientPosition() const
{
    return GetPosition();
}

void Window::Show(bool show)
{
    QWidget *w = PWindow(wid);

    if (show)
        w->show();
    else
        w->hide();
}

void Window::InvalidateAll()
{
    QWidget *w = PWindow(wid);

    if (w)
        w->update();
}

void Window::InvalidateRectangle(PRectangle rc)
{
    QWidget *w = PWindow(wid);

    if (w)
        w->update(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

void Window::SetFont(Font &font)
{
    PWindow(wid)->setFont(*PFont(font.GetID()));
}

void Window::SetCursor(Cursor curs)
{
    Qt::CursorShape qc;

    switch (curs)
    {
    case cursorText:
        qc = Qt::IBeamCursor;
        break;

    case cursorUp:
        qc = Qt::UpArrowCursor;
        break;

    case cursorWait:
        qc = Qt::WaitCursor;
        break;

    case cursorHoriz:
        qc = Qt::SizeHorCursor;
        break;

    case cursorVert:
        qc = Qt::SizeVerCursor;
        break;

    case cursorHand:
        qc = Qt::PointingHandCursor;
        break;

    default:
        // Note that Qt doesn't have a standard cursor that could be used to
        // implement cursorReverseArrow.
        qc = Qt::ArrowCursor;
    }

    PWindow(wid)->setCursor(qc);
}

PRectangle Window::GetMonitorRect(Point pt)
{
    QPoint qpt = PWindow(wid)->mapToGlobal(QPoint(pt.x, pt.y));
    QRect qr = QApplication::screenAt(qpt)->availableGeometry();
    qpt = PWindow(wid)->mapFromGlobal(qr.topLeft());

    return PRectangle(qpt.x(), qpt.y(), qpt.x() + qr.width(), qpt.y() + qr.height());
}


// Menu management.
Menu::Menu() noexcept : mid(0)
{
}

void Menu::CreatePopUp()
{
    Destroy();
    mid = new QsciSciPopup();
}

void Menu::Destroy()
{
    QsciSciPopup *m = PMenu(mid);

    if (m)
    {
        delete m;
        mid = 0;
    }
}

void Menu::Show(Point pt, Window &)
{
    PMenu(mid)->popup(QPoint(pt.x, pt.y));
}


class DynamicLibraryImpl : public DynamicLibrary
{
public:
    DynamicLibraryImpl(const char *modulePath)
    {
#if !defined(Q_OS_WASM)
        m = new QLibrary(modulePath);
        m->load();
#endif
    }

    virtual ~DynamicLibraryImpl()
    {
#if !defined(Q_OS_WASM)
        if (m)
            delete m;
#endif
    }

    virtual Function FindFunction(const char *name)
    {
#if !defined(Q_OS_WASM)
        if (m)
            return (Function)m->resolve(name);
#endif

        return 0;
    }

    virtual bool IsValid()
    {
#if !defined(Q_OS_WASM)
        return m && m->isLoaded();
#else
        return false;
#endif
    }

private:
#if !defined(Q_OS_WASM)
    QLibrary* m;
#endif
};

DynamicLibrary *DynamicLibrary::Load(const char *modulePath)
{
    return new DynamicLibraryImpl(modulePath);
}


// Manage system wide parameters.
ColourDesired Platform::Chrome()
{
    return ColourDesired(0xe0,0xe0,0xe0);
}

ColourDesired Platform::ChromeHighlight()
{
    return ColourDesired(0xff,0xff,0xff);
}

const char *Platform::DefaultFont()
{
    static QByteArray def_font;

    def_font = QApplication::font().family().toLatin1();

    return def_font.constData();
}

int Platform::DefaultFontSize()
{
    return QApplication::font().pointSize();
}

unsigned int Platform::DoubleClickTime()
{
    return QApplication::doubleClickInterval();
}

void Platform::DebugDisplay(const char *s)
{
    qDebug("%s", s);
}

//#define TRACE

#ifdef TRACE
void Platform::DebugPrintf(const char *format, ...)
{
    char buffer[2000];
    va_list pArguments;

    va_start(pArguments, format);
    vsprintf(buffer, format, pArguments);
    va_end(pArguments);

    DebugDisplay(buffer);
}
#else
void Platform::DebugPrintf(const char *, ...)
{
}
#endif

static bool assertionPopUps = true;

bool Platform::ShowAssertionPopUps(bool assertionPopUps_)
{
    bool ret = assertionPopUps;

    assertionPopUps = assertionPopUps_;

    return ret;
}

void Platform::Assert(const char *c, const char *file, int line)
{
    qFatal("Assertion [%s] failed at %s %d\n", c, file, line);
}

}
