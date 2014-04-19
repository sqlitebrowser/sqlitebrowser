/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011, 2012, 2013, 2014 Emanuel Eichhammer               **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 07.04.14                                             **
**          Version: 1.2.1                                                **
****************************************************************************/

#include "qcustomplot.h"




////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPainter
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPainter
  \brief QPainter subclass used internally
  
  This QPainter subclass is used to provide some extended functionality e.g. for tweaking position
  consistency between antialiased and non-antialiased painting. Further it provides workarounds
  for QPainter quirks.
  
  \warning This class intentionally hides non-virtual functions of QPainter, e.g. setPen, save and
  restore. So while it is possible to pass a QCPPainter instance to a function that expects a
  QPainter pointer, some of the workarounds and tweaks will be unavailable to the function (because
  it will call the base class implementations of the functions actually hidden by QCPPainter).
*/

/*!
  Creates a new QCPPainter instance and sets default values
*/
QCPPainter::QCPPainter() :
  QPainter(),
  mModes(pmDefault),
  mIsAntialiasing(false)
{
  // don't setRenderHint(QPainter::NonCosmeticDefautPen) here, because painter isn't active yet and
  // a call to begin() will follow
}

/*!
  Creates a new QCPPainter instance on the specified paint \a device and sets default values. Just
  like the analogous QPainter constructor, begins painting on \a device immediately.
  
  Like \ref begin, this method sets QPainter::NonCosmeticDefaultPen in Qt versions before Qt5.
*/
QCPPainter::QCPPainter(QPaintDevice *device) :
  QPainter(device),
  mModes(pmDefault),
  mIsAntialiasing(false)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0) // before Qt5, default pens used to be cosmetic if NonCosmeticDefaultPen flag isn't set. So we set it to get consistency across Qt versions.
  if (isActive())
    setRenderHint(QPainter::NonCosmeticDefaultPen);
#endif
}

QCPPainter::~QCPPainter()
{
}

/*!
  Sets the pen of the painter and applies certain fixes to it, depending on the mode of this
  QCPPainter.
  
  \note this function hides the non-virtual base class implementation.
*/
void QCPPainter::setPen(const QPen &pen)
{
  QPainter::setPen(pen);
  if (mModes.testFlag(pmNonCosmetic))
    makeNonCosmetic();
}

/*! \overload
  
  Sets the pen (by color) of the painter and applies certain fixes to it, depending on the mode of
  this QCPPainter.
  
  \note this function hides the non-virtual base class implementation.
*/
void QCPPainter::setPen(const QColor &color)
{
  QPainter::setPen(color);
  if (mModes.testFlag(pmNonCosmetic))
    makeNonCosmetic();
}

/*! \overload
  
  Sets the pen (by style) of the painter and applies certain fixes to it, depending on the mode of
  this QCPPainter.
  
  \note this function hides the non-virtual base class implementation.
*/
void QCPPainter::setPen(Qt::PenStyle penStyle)
{
  QPainter::setPen(penStyle);
  if (mModes.testFlag(pmNonCosmetic))
    makeNonCosmetic();
}

/*! \overload
  
  Works around a Qt bug introduced with Qt 4.8 which makes drawing QLineF unpredictable when
  antialiasing is disabled. Thus when antialiasing is disabled, it rounds the \a line to
  integer coordinates and then passes it to the original drawLine.
  
  \note this function hides the non-virtual base class implementation.
*/
void QCPPainter::drawLine(const QLineF &line)
{
  if (mIsAntialiasing || mModes.testFlag(pmVectorized))
    QPainter::drawLine(line);
  else
    QPainter::drawLine(line.toLine());
}

/*!
  Sets whether painting uses antialiasing or not. Use this method instead of using setRenderHint
  with QPainter::Antialiasing directly, as it allows QCPPainter to regain pixel exactness between
  antialiased and non-antialiased painting (Since Qt < 5.0 uses slightly different coordinate systems for
  AA/Non-AA painting).
*/
void QCPPainter::setAntialiasing(bool enabled)
{
  setRenderHint(QPainter::Antialiasing, enabled);
  if (mIsAntialiasing != enabled)
  {
    mIsAntialiasing = enabled;
    if (!mModes.testFlag(pmVectorized)) // antialiasing half-pixel shift only needed for rasterized outputs
    {
      if (mIsAntialiasing)
        translate(0.5, 0.5);
      else
        translate(-0.5, -0.5);
    }
  }
}

/*!
  Sets the mode of the painter. This controls whether the painter shall adjust its
  fixes/workarounds optimized for certain output devices.
*/
void QCPPainter::setModes(QCPPainter::PainterModes modes)
{
  mModes = modes;
}

/*!
  Sets the QPainter::NonCosmeticDefaultPen in Qt versions before Qt5 after beginning painting on \a
  device. This is necessary to get cosmetic pen consistency across Qt versions, because since Qt5,
  all pens are non-cosmetic by default, and in Qt4 this render hint must be set to get that
  behaviour.
  
  The Constructor \ref QCPPainter(QPaintDevice *device) which directly starts painting also sets
  the render hint as appropriate.
  
  \note this function hides the non-virtual base class implementation.
*/
bool QCPPainter::begin(QPaintDevice *device)
{
  bool result = QPainter::begin(device);
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0) // before Qt5, default pens used to be cosmetic if NonCosmeticDefaultPen flag isn't set. So we set it to get consistency across Qt versions.
  if (result)
    setRenderHint(QPainter::NonCosmeticDefaultPen);
#endif
  return result;
}

/*! \overload
  
  Sets the mode of the painter. This controls whether the painter shall adjust its
  fixes/workarounds optimized for certain output devices.
*/
void QCPPainter::setMode(QCPPainter::PainterMode mode, bool enabled)
{
  if (!enabled && mModes.testFlag(mode))
    mModes &= ~mode;
  else if (enabled && !mModes.testFlag(mode))
    mModes |= mode;
}

/*!
  Saves the painter (see QPainter::save). Since QCPPainter adds some new internal state to
  QPainter, the save/restore functions are reimplemented to also save/restore those members.
  
  \note this function hides the non-virtual base class implementation.
  
  \see restore
*/
void QCPPainter::save()
{
  mAntialiasingStack.push(mIsAntialiasing);
  QPainter::save();
}

/*!
  Restores the painter (see QPainter::restore). Since QCPPainter adds some new internal state to
  QPainter, the save/restore functions are reimplemented to also save/restore those members.
  
  \note this function hides the non-virtual base class implementation.
  
  \see save
*/
void QCPPainter::restore()
{
  if (!mAntialiasingStack.isEmpty())
    mIsAntialiasing = mAntialiasingStack.pop();
  else
    qDebug() << Q_FUNC_INFO << "Unbalanced save/restore";
  QPainter::restore();
}

/*!
  Changes the pen width to 1 if it currently is 0. This function is called in the \ref setPen
  overrides when the \ref pmNonCosmetic mode is set.
*/
void QCPPainter::makeNonCosmetic()
{
  if (qFuzzyIsNull(pen().widthF()))
  {
    QPen p = pen();
    p.setWidth(1);
    QPainter::setPen(p);
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPScatterStyle
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPScatterStyle
  \brief Represents the visual appearance of scatter points
  
  This class holds information about shape, color and size of scatter points. In plottables like
  QCPGraph it is used to store how scatter points shall be drawn. For example, \ref
  QCPGraph::setScatterStyle takes a QCPScatterStyle instance.
  
  A scatter style consists of a shape (\ref setShape), a line color (\ref setPen) and possibly a
  fill (\ref setBrush), if the shape provides a fillable area. Further, the size of the shape can
  be controlled with \ref setSize.

  \section QCPScatterStyle-defining Specifying a scatter style
  
  You can set all these configurations either by calling the respective functions on an instance:
  \code
  QCPScatterStyle myScatter;
  myScatter.setShape(QCPScatterStyle::ssCircle);
  myScatter.setPen(Qt::blue);
  myScatter.setBrush(Qt::white);
  myScatter.setSize(5);
  customPlot->graph(0)->setScatterStyle(myScatter);
  \endcode
  
  Or you can use one of the various constructors that take different parameter combinations, making
  it easy to specify a scatter style in a single call, like so:
  \code
  customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::white, 5));
  \endcode
  
  \section QCPScatterStyle-undefinedpen Leaving the color/pen up to the plottable
  
  There are two constructors which leave the pen undefined: \ref QCPScatterStyle() and \ref
  QCPScatterStyle(ScatterShape shape, double size). If those constructors are used, a call to \ref
  isPenDefined will return false. It leads to scatter points that inherit the pen from the
  plottable that uses the scatter style. Thus, if such a scatter style is passed to QCPGraph, the line
  color of the graph (\ref QCPGraph::setPen) will be used by the scatter points. This makes
  it very convenient to set up typical scatter settings:
  
  \code
  customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssPlus);
  \endcode

  Notice that it wasn't even necessary to explicitly call a QCPScatterStyle constructor. This works
  because QCPScatterStyle provides a constructor that can transform a \ref ScatterShape directly
  into a QCPScatterStyle instance (that's the \ref QCPScatterStyle(ScatterShape shape, double size)
  constructor with a default for \a size). In those cases, C++ allows directly supplying a \ref
  ScatterShape, where actually a QCPScatterStyle is expected.
  
  \section QCPScatterStyle-custompath-and-pixmap Custom shapes and pixmaps
  
  QCPScatterStyle supports drawing custom shapes and arbitrary pixmaps as scatter points.

  For custom shapes, you can provide a QPainterPath with the desired shape to the \ref
  setCustomPath function or call the constructor that takes a painter path. The scatter shape will
  automatically be set to \ref ssCustom.
  
  For pixmaps, you call \ref setPixmap with the desired QPixmap. Alternatively you can use the
  constructor that takes a QPixmap. The scatter shape will automatically be set to \ref ssPixmap.
  Note that \ref setSize does not influence the appearance of the pixmap.
*/

/* start documentation of inline functions */

/*! \fn bool QCPScatterStyle::isNone() const
  
  Returns whether the scatter shape is \ref ssNone.
  
  \see setShape
*/

/*! \fn bool QCPScatterStyle::isPenDefined() const
  
  Returns whether a pen has been defined for this scatter style.
  
  The pen is undefined if a constructor is called that does not carry \a pen as parameter. Those are
  \ref QCPScatterStyle() and \ref QCPScatterStyle(ScatterShape shape, double size). If the pen is
  left undefined, the scatter color will be inherited from the plottable that uses this scatter
  style.
  
  \see setPen
*/

/* end documentation of inline functions */

/*!
  Creates a new QCPScatterStyle instance with size set to 6. No shape, pen or brush is defined.
  
  Since the pen is undefined (\ref isPenDefined returns false), the scatter color will be inherited
  from the plottable that uses this scatter style.
*/
QCPScatterStyle::QCPScatterStyle() :
  mSize(6),
  mShape(ssNone),
  mPen(Qt::NoPen),
  mBrush(Qt::NoBrush),
  mPenDefined(false)
{
}

/*!
  Creates a new QCPScatterStyle instance with shape set to \a shape and size to \a size. No pen or
  brush is defined.
  
  Since the pen is undefined (\ref isPenDefined returns false), the scatter color will be inherited
  from the plottable that uses this scatter style.
*/
QCPScatterStyle::QCPScatterStyle(ScatterShape shape, double size) :
  mSize(size),
  mShape(shape),
  mPen(Qt::NoPen),
  mBrush(Qt::NoBrush),
  mPenDefined(false)
{
}

/*!
  Creates a new QCPScatterStyle instance with shape set to \a shape, the pen color set to \a color,
  and size to \a size. No brush is defined, i.e. the scatter point will not be filled.
*/
QCPScatterStyle::QCPScatterStyle(ScatterShape shape, const QColor &color, double size) :
  mSize(size),
  mShape(shape),
  mPen(QPen(color)),
  mBrush(Qt::NoBrush),
  mPenDefined(true)
{
}

/*!
  Creates a new QCPScatterStyle instance with shape set to \a shape, the pen color set to \a color,
  the brush color to \a fill (with a solid pattern), and size to \a size.
*/
QCPScatterStyle::QCPScatterStyle(ScatterShape shape, const QColor &color, const QColor &fill, double size) :
  mSize(size),
  mShape(shape),
  mPen(QPen(color)),
  mBrush(QBrush(fill)),
  mPenDefined(true)
{
}

/*!
  Creates a new QCPScatterStyle instance with shape set to \a shape, the pen set to \a pen, the
  brush to \a brush, and size to \a size.
  
  \warning In some cases it might be tempting to directly use a pen style like <tt>Qt::NoPen</tt> as \a pen
  and a color like <tt>Qt::blue</tt> as \a brush. Notice however, that the corresponding call\n
  <tt>QCPScatterStyle(QCPScatterShape::ssCircle, Qt::NoPen, Qt::blue, 5)</tt>\n
  doesn't necessarily lead C++ to use this constructor in some cases, but might mistake
  <tt>Qt::NoPen</tt> for a QColor and use the
  \ref QCPScatterStyle(ScatterShape shape, const QColor &color, const QColor &fill, double size)
  constructor instead (which will lead to an unexpected look of the scatter points). To prevent
  this, be more explicit with the parameter types. For example, use <tt>QBrush(Qt::blue)</tt>
  instead of just <tt>Qt::blue</tt>, to clearly point out to the compiler that this constructor is
  wanted.
*/
QCPScatterStyle::QCPScatterStyle(ScatterShape shape, const QPen &pen, const QBrush &brush, double size) :
  mSize(size),
  mShape(shape),
  mPen(pen),
  mBrush(brush),
  mPenDefined(pen.style() != Qt::NoPen)
{
}

/*!
  Creates a new QCPScatterStyle instance which will show the specified \a pixmap. The scatter shape
  is set to \ref ssPixmap.
*/
QCPScatterStyle::QCPScatterStyle(const QPixmap &pixmap) :
  mSize(5),
  mShape(ssPixmap),
  mPen(Qt::NoPen),
  mBrush(Qt::NoBrush),
  mPixmap(pixmap),
  mPenDefined(false)
{
}

/*!
  Creates a new QCPScatterStyle instance with a custom shape that is defined via \a customPath. The
  scatter shape is set to \ref ssCustom.
  
  The custom shape line will be drawn with \a pen and filled with \a brush. The size has a slightly
  different meaning than for built-in scatter points: The custom path will be drawn scaled by a
  factor of \a size/6.0. Since the default \a size is 6, the custom path will appear at a its
  natural size by default. To double the size of the path for example, set \a size to 12.
*/
QCPScatterStyle::QCPScatterStyle(const QPainterPath &customPath, const QPen &pen, const QBrush &brush, double size) :
  mSize(size),
  mShape(ssCustom),
  mPen(pen),
  mBrush(brush),
  mCustomPath(customPath),
  mPenDefined(false)
{
}

/*!
  Sets the size (pixel diameter) of the drawn scatter points to \a size.
  
  \see setShape
*/
void QCPScatterStyle::setSize(double size)
{
  mSize = size;
}

/*!
  Sets the shape to \a shape.
  
  Note that the calls \ref setPixmap and \ref setCustomPath automatically set the shape to \ref
  ssPixmap and \ref ssCustom, respectively.
  
  \see setSize
*/
void QCPScatterStyle::setShape(QCPScatterStyle::ScatterShape shape)
{
  mShape = shape;
}

/*!
  Sets the pen that will be used to draw scatter points to \a pen.
  
  If the pen was previously undefined (see \ref isPenDefined), the pen is considered defined after
  a call to this function, even if \a pen is <tt>Qt::NoPen</tt>.
  
  \see setBrush
*/
void QCPScatterStyle::setPen(const QPen &pen)
{
  mPenDefined = true;
  mPen = pen;
}

/*!
  Sets the brush that will be used to fill scatter points to \a brush. Note that not all scatter
  shapes have fillable areas. For example, \ref ssPlus does not while \ref ssCircle does.
  
  \see setPen
*/
void QCPScatterStyle::setBrush(const QBrush &brush)
{
  mBrush = brush;
}

/*!
  Sets the pixmap that will be drawn as scatter point to \a pixmap.
  
  Note that \ref setSize does not influence the appearance of the pixmap.
  
  The scatter shape is automatically set to \ref ssPixmap.
*/
void QCPScatterStyle::setPixmap(const QPixmap &pixmap)
{
  setShape(ssPixmap);
  mPixmap = pixmap;
}

/*!
  Sets the custom shape that will be drawn as scatter point to \a customPath.
  
  The scatter shape is automatically set to \ref ssCustom.
*/
void QCPScatterStyle::setCustomPath(const QPainterPath &customPath)
{
  setShape(ssCustom);
  mCustomPath = customPath;
}

/*!
  Applies the pen and the brush of this scatter style to \a painter. If this scatter style has an
  undefined pen (\ref isPenDefined), sets the pen of \a painter to \a defaultPen instead.
  
  This function is used by plottables (or any class that wants to draw scatters) just before a
  number of scatters with this style shall be drawn with the \a painter.
  
  \see drawShape
*/
void QCPScatterStyle::applyTo(QCPPainter *painter, const QPen &defaultPen) const
{
  painter->setPen(mPenDefined ? mPen : defaultPen);
  painter->setBrush(mBrush);
}

/*!
  Draws the scatter shape with \a painter at position \a pos.
  
  This function does not modify the pen or the brush on the painter, as \ref applyTo is meant to be
  called before scatter points are drawn with \ref drawShape.
  
  \see applyTo
*/
void QCPScatterStyle::drawShape(QCPPainter *painter, QPointF pos) const
{
  drawShape(painter, pos.x(), pos.y());
}

/*! \overload
  Draws the scatter shape with \a painter at position \a x and \a y.
*/
void QCPScatterStyle::drawShape(QCPPainter *painter, double x, double y) const
{
  double w = mSize/2.0;
  switch (mShape)
  {
    case ssNone: break;
    case ssDot:
    {
      painter->drawLine(QPointF(x, y), QPointF(x+0.0001, y));
      break;
    }
    case ssCross:
    {
      painter->drawLine(QLineF(x-w, y-w, x+w, y+w));
      painter->drawLine(QLineF(x-w, y+w, x+w, y-w));
      break;
    }
    case ssPlus:
    {
      painter->drawLine(QLineF(x-w,   y, x+w,   y));
      painter->drawLine(QLineF(  x, y+w,   x, y-w));
      break;
    }
    case ssCircle:
    {
      painter->drawEllipse(QPointF(x , y), w, w);
      break;
    }
    case ssDisc:
    {
      QBrush b = painter->brush();
      painter->setBrush(painter->pen().color());
      painter->drawEllipse(QPointF(x , y), w, w);
      painter->setBrush(b);
      break;
    }
    case ssSquare:
    {
      painter->drawRect(QRectF(x-w, y-w, mSize, mSize));
      break;
    }
    case ssDiamond:
    {
      painter->drawLine(QLineF(x-w,   y,   x, y-w));
      painter->drawLine(QLineF(  x, y-w, x+w,   y));
      painter->drawLine(QLineF(x+w,   y,   x, y+w));
      painter->drawLine(QLineF(  x, y+w, x-w,   y));
      break;
    }
    case ssStar:
    {
      painter->drawLine(QLineF(x-w,   y, x+w,   y));
      painter->drawLine(QLineF(  x, y+w,   x, y-w));
      painter->drawLine(QLineF(x-w*0.707, y-w*0.707, x+w*0.707, y+w*0.707));
      painter->drawLine(QLineF(x-w*0.707, y+w*0.707, x+w*0.707, y-w*0.707));
      break;
    }
    case ssTriangle:
    {
       painter->drawLine(QLineF(x-w, y+0.755*w, x+w, y+0.755*w));
       painter->drawLine(QLineF(x+w, y+0.755*w,   x, y-0.977*w));
       painter->drawLine(QLineF(  x, y-0.977*w, x-w, y+0.755*w));
      break;
    }
    case ssTriangleInverted:
    {
       painter->drawLine(QLineF(x-w, y-0.755*w, x+w, y-0.755*w));
       painter->drawLine(QLineF(x+w, y-0.755*w,   x, y+0.977*w));
       painter->drawLine(QLineF(  x, y+0.977*w, x-w, y-0.755*w));
      break;
    }
    case ssCrossSquare:
    {
       painter->drawLine(QLineF(x-w, y-w, x+w*0.95, y+w*0.95));
       painter->drawLine(QLineF(x-w, y+w*0.95, x+w*0.95, y-w));
       painter->drawRect(QRectF(x-w, y-w, mSize, mSize));
      break;
    }
    case ssPlusSquare:
    {
       painter->drawLine(QLineF(x-w,   y, x+w*0.95,   y));
       painter->drawLine(QLineF(  x, y+w,        x, y-w));
       painter->drawRect(QRectF(x-w, y-w, mSize, mSize));
      break;
    }
    case ssCrossCircle:
    {
       painter->drawLine(QLineF(x-w*0.707, y-w*0.707, x+w*0.670, y+w*0.670));
       painter->drawLine(QLineF(x-w*0.707, y+w*0.670, x+w*0.670, y-w*0.707));
       painter->drawEllipse(QPointF(x, y), w, w);
      break;
    }
    case ssPlusCircle:
    {
       painter->drawLine(QLineF(x-w,   y, x+w,   y));
       painter->drawLine(QLineF(  x, y+w,   x, y-w));
       painter->drawEllipse(QPointF(x, y), w, w);
      break;
    }
    case ssPeace:
    {
       painter->drawLine(QLineF(x, y-w,         x,       y+w));
       painter->drawLine(QLineF(x,   y, x-w*0.707, y+w*0.707));
       painter->drawLine(QLineF(x,   y, x+w*0.707, y+w*0.707));
       painter->drawEllipse(QPointF(x, y), w, w);
      break;
    }
    case ssPixmap:
    {
      painter->drawPixmap(x-mPixmap.width()*0.5, y-mPixmap.height()*0.5, mPixmap);
      break;
    }
    case ssCustom:
    {
      QTransform oldTransform = painter->transform();
      painter->translate(x, y);
      painter->scale(mSize/6.0, mSize/6.0);
      painter->drawPath(mCustomPath);
      painter->setTransform(oldTransform);
      break;
    }
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayer
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLayer
  \brief A layer that may contain objects, to control the rendering order
  
  The Layering system of QCustomPlot is the mechanism to control the rendering order of the
  elements inside the plot.
  
  It is based on the two classes QCPLayer and QCPLayerable. QCustomPlot holds an ordered list of
  one or more instances of QCPLayer (see QCustomPlot::addLayer, QCustomPlot::layer,
  QCustomPlot::moveLayer, etc.). When replotting, QCustomPlot goes through the list of layers
  bottom to top and successively draws the layerables of the layers.
  
  A QCPLayer contains an ordered list of QCPLayerable instances. QCPLayerable is an abstract base
  class from which almost all visible objects derive, like axes, grids, graphs, items, etc.
  
  Initially, QCustomPlot has five layers: "background", "grid", "main", "axes" and "legend" (in
  that order). The top two layers "axes" and "legend" contain the default axes and legend, so they
  will be drawn on top. In the middle, there is the "main" layer. It is initially empty and set as
  the current layer (see QCustomPlot::setCurrentLayer). This means, all new plottables, items etc.
  are created on this layer by default. Then comes the "grid" layer which contains the QCPGrid
  instances (which belong tightly to QCPAxis, see \ref QCPAxis::grid). The Axis rect background
  shall be drawn behind everything else, thus the default QCPAxisRect instance is placed on the
  "background" layer. Of course, the layer affiliation of the individual objects can be changed as
  required (\ref QCPLayerable::setLayer).
  
  Controlling the ordering of objects is easy: Create a new layer in the position you want it to
  be, e.g. above "main", with QCustomPlot::addLayer. Then set the current layer with
  QCustomPlot::setCurrentLayer to that new layer and finally create the objects normally. They will
  be placed on the new layer automatically, due to the current layer setting. Alternatively you
  could have also ignored the current layer setting and just moved the objects with
  QCPLayerable::setLayer to the desired layer after creating them.
  
  It is also possible to move whole layers. For example, If you want the grid to be shown in front
  of all plottables/items on the "main" layer, just move it above "main" with
  QCustomPlot::moveLayer.
  
  The rendering order within one layer is simply by order of creation or insertion. The item
  created last (or added last to the layer), is drawn on top of all other objects on that layer.
  
  When a layer is deleted, the objects on it are not deleted with it, but fall on the layer below
  the deleted layer, see QCustomPlot::removeLayer.
*/

/* start documentation of inline functions */

/*! \fn QList<QCPLayerable*> QCPLayer::children() const
  
  Returns a list of all layerables on this layer. The order corresponds to the rendering order:
  layerables with higher indices are drawn above layerables with lower indices.
*/

/*! \fn int QCPLayer::index() const
  
  Returns the index this layer has in the QCustomPlot. The index is the integer number by which this layer can be
  accessed via \ref QCustomPlot::layer.
  
  Layers with higher indices will be drawn above layers with lower indices.
*/

/* end documentation of inline functions */

/*!
  Creates a new QCPLayer instance.
  
  Normally you shouldn't directly instantiate layers, use \ref QCustomPlot::addLayer instead.
  
  \warning It is not checked that \a layerName is actually a unique layer name in \a parentPlot.
  This check is only performed by \ref QCustomPlot::addLayer.
*/
QCPLayer::QCPLayer(QCustomPlot *parentPlot, const QString &layerName) :
  QObject(parentPlot),
  mParentPlot(parentPlot),
  mName(layerName),
  mIndex(-1), // will be set to a proper value by the QCustomPlot layer creation function
  mVisible(true)
{
  // Note: no need to make sure layerName is unique, because layer
  // management is done with QCustomPlot functions.
}

QCPLayer::~QCPLayer()
{
  // If child layerables are still on this layer, detach them, so they don't try to reach back to this
  // then invalid layer once they get deleted/moved themselves. This only happens when layers are deleted
  // directly, like in the QCustomPlot destructor. (The regular layer removal procedure for the user is to
  // call QCustomPlot::removeLayer, which moves all layerables off this layer before deleting it.)
  
  while (!mChildren.isEmpty())
    mChildren.last()->setLayer(0); // removes itself from mChildren via removeChild()
  
  if (mParentPlot->currentLayer() == this)
    qDebug() << Q_FUNC_INFO << "The parent plot's mCurrentLayer will be a dangling pointer. Should have been set to a valid layer or 0 beforehand.";
}

/*!
  Sets whether this layer is visible or not. If \a visible is set to false, all layerables on this
  layer will be invisible.

  This function doesn't change the visibility property of the layerables (\ref
  QCPLayerable::setVisible), but the \ref QCPLayerable::realVisibility of each layerable takes the
  visibility of the parent layer into account.
*/
void QCPLayer::setVisible(bool visible)
{
  mVisible = visible;
}

/*! \internal
  
  Adds the \a layerable to the list of this layer. If \a prepend is set to true, the layerable will
  be prepended to the list, i.e. be drawn beneath the other layerables already in the list.
  
  This function does not change the \a mLayer member of \a layerable to this layer. (Use
  QCPLayerable::setLayer to change the layer of an object, not this function.)
  
  \see removeChild
*/
void QCPLayer::addChild(QCPLayerable *layerable, bool prepend)
{
  if (!mChildren.contains(layerable))
  {
    if (prepend)
      mChildren.prepend(layerable);
    else
      mChildren.append(layerable);
  } else
    qDebug() << Q_FUNC_INFO << "layerable is already child of this layer" << reinterpret_cast<quintptr>(layerable);
}

/*! \internal
  
  Removes the \a layerable from the list of this layer.
  
  This function does not change the \a mLayer member of \a layerable. (Use QCPLayerable::setLayer
  to change the layer of an object, not this function.)
  
  \see addChild
*/
void QCPLayer::removeChild(QCPLayerable *layerable)
{
  if (!mChildren.removeOne(layerable))
    qDebug() << Q_FUNC_INFO << "layerable is not child of this layer" << reinterpret_cast<quintptr>(layerable);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayerable
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLayerable
  \brief Base class for all drawable objects
  
  This is the abstract base class most visible objects derive from, e.g. plottables, axes, grid
  etc.

  Every layerable is on a layer (QCPLayer) which allows controlling the rendering order by stacking
  the layers accordingly.
  
  For details about the layering mechanism, see the QCPLayer documentation.
*/

/* start documentation of inline functions */

/*! \fn QCPLayerable *QCPLayerable::parentLayerable() const
 
  Returns the parent layerable of this layerable. The parent layerable is used to provide
  visibility hierarchies in conjunction with the method \ref realVisibility. This way, layerables
  only get drawn if their parent layerables are visible, too.
  
  Note that a parent layerable is not necessarily also the QObject parent for memory management.
  Further, a layerable doesn't always have a parent layerable, so this function may return 0.
  
  A parent layerable is set implicitly with when placed inside layout elements and doesn't need to be
  set manually by the user.
*/

/* end documentation of inline functions */
/* start documentation of pure virtual functions */

/*! \fn virtual void QCPLayerable::applyDefaultAntialiasingHint(QCPPainter *painter) const = 0
  \internal
  
  This function applies the default antialiasing setting to the specified \a painter, using the
  function \ref applyAntialiasingHint. It is the antialiasing state the painter is put in, when
  \ref draw is called on the layerable. If the layerable has multiple entities whose antialiasing
  setting may be specified individually, this function should set the antialiasing state of the
  most prominent entity. In this case however, the \ref draw function usually calls the specialized
  versions of this function before drawing each entity, effectively overriding the setting of the
  default antialiasing hint.
  
  <b>First example:</b> QCPGraph has multiple entities that have an antialiasing setting: The graph
  line, fills, scatters and error bars. Those can be configured via QCPGraph::setAntialiased,
  QCPGraph::setAntialiasedFill, QCPGraph::setAntialiasedScatters etc. Consequently, there isn't
  only the QCPGraph::applyDefaultAntialiasingHint function (which corresponds to the graph line's
  antialiasing), but specialized ones like QCPGraph::applyFillAntialiasingHint and
  QCPGraph::applyScattersAntialiasingHint. So before drawing one of those entities, QCPGraph::draw
  calls the respective specialized applyAntialiasingHint function.
  
  <b>Second example:</b> QCPItemLine consists only of a line so there is only one antialiasing
  setting which can be controlled with QCPItemLine::setAntialiased. (This function is inherited by
  all layerables. The specialized functions, as seen on QCPGraph, must be added explicitly to the
  respective layerable subclass.) Consequently it only has the normal
  QCPItemLine::applyDefaultAntialiasingHint. The \ref QCPItemLine::draw function doesn't need to
  care about setting any antialiasing states, because the default antialiasing hint is already set
  on the painter when the \ref draw function is called, and that's the state it wants to draw the
  line with.
*/

/*! \fn virtual void QCPLayerable::draw(QCPPainter *painter) const = 0
  \internal
  
  This function draws the layerable with the specified \a painter. It is only called by
  QCustomPlot, if the layerable is visible (\ref setVisible).
  
  Before this function is called, the painter's antialiasing state is set via \ref
  applyDefaultAntialiasingHint, see the documentation there. Further, the clipping rectangle was
  set to \ref clipRect.
*/

/* end documentation of pure virtual functions */
/* start documentation of signals */

/*! \fn void QCPLayerable::layerChanged(QCPLayer *newLayer);
  
  This signal is emitted when the layer of this layerable changes, i.e. this layerable is moved to
  a different layer.
  
  \see setLayer
*/

/* end documentation of signals */

/*!
  Creates a new QCPLayerable instance.
  
  Since QCPLayerable is an abstract base class, it can't be instantiated directly. Use one of the
  derived classes.
  
  If \a plot is provided, it automatically places itself on the layer named \a targetLayer. If \a
  targetLayer is an empty string, it places itself on the current layer of the plot (see \ref
  QCustomPlot::setCurrentLayer).
  
  It is possible to provide 0 as \a plot. In that case, you should assign a parent plot at a later
  time with \ref initializeParentPlot.
  
  The layerable's parent layerable is set to \a parentLayerable, if provided. Direct layerable parents
  are mainly used to control visibility in a hierarchy of layerables. This means a layerable is
  only drawn, if all its ancestor layerables are also visible. Note that \a parentLayerable does
  not become the QObject-parent (for memory management) of this layerable, \a plot does.
*/
QCPLayerable::QCPLayerable(QCustomPlot *plot, QString targetLayer, QCPLayerable *parentLayerable) :
  QObject(plot),
  mVisible(true),
  mParentPlot(plot),
  mParentLayerable(parentLayerable),
  mLayer(0),
  mAntialiased(true)
{
  if (mParentPlot)
  {
    if (targetLayer.isEmpty())
      setLayer(mParentPlot->currentLayer());
    else if (!setLayer(targetLayer))
      qDebug() << Q_FUNC_INFO << "setting QCPlayerable initial layer to" << targetLayer << "failed.";
  }
}

QCPLayerable::~QCPLayerable()
{
  if (mLayer)
  {
    mLayer->removeChild(this);
    mLayer = 0;
  }
}

/*!
  Sets the visibility of this layerable object. If an object is not visible, it will not be drawn
  on the QCustomPlot surface, and user interaction with it (e.g. click and selection) is not
  possible.
*/
void QCPLayerable::setVisible(bool on)
{
  mVisible = on;
}

/*!
  Sets the \a layer of this layerable object. The object will be placed on top of the other objects
  already on \a layer.
  
  Returns true on success, i.e. if \a layer is a valid layer.
*/
bool QCPLayerable::setLayer(QCPLayer *layer)
{
  return moveToLayer(layer, false);
}

/*! \overload
  Sets the layer of this layerable object by name
  
  Returns true on success, i.e. if \a layerName is a valid layer name.
*/
bool QCPLayerable::setLayer(const QString &layerName)
{
  if (!mParentPlot)
  {
    qDebug() << Q_FUNC_INFO << "no parent QCustomPlot set";
    return false;
  }
  if (QCPLayer *layer = mParentPlot->layer(layerName))
  {
    return setLayer(layer);
  } else
  {
    qDebug() << Q_FUNC_INFO << "there is no layer with name" << layerName;
    return false;
  }
}

/*!
  Sets whether this object will be drawn antialiased or not.
  
  Note that antialiasing settings may be overridden by QCustomPlot::setAntialiasedElements and
  QCustomPlot::setNotAntialiasedElements.
*/
void QCPLayerable::setAntialiased(bool enabled)
{
  mAntialiased = enabled;
}

/*!
  Returns whether this layerable is visible, taking the visibility of the layerable parent and the
  visibility of the layer this layerable is on into account. This is the method that is consulted
  to decide whether a layerable shall be drawn or not.
  
  If this layerable has a direct layerable parent (usually set via hierarchies implemented in
  subclasses, like in the case of QCPLayoutElement), this function returns true only if this
  layerable has its visibility set to true and the parent layerable's \ref realVisibility returns
  true.
  
  If this layerable doesn't have a direct layerable parent, returns the state of this layerable's
  visibility.
*/
bool QCPLayerable::realVisibility() const
{
  return mVisible && (!mLayer || mLayer->visible()) && (!mParentLayerable || mParentLayerable.data()->realVisibility());
}

/*!
  This function is used to decide whether a click hits a layerable object or not.

  \a pos is a point in pixel coordinates on the QCustomPlot surface. This function returns the
  shortest pixel distance of this point to the object. If the object is either invisible or the
  distance couldn't be determined, -1.0 is returned. Further, if \a onlySelectable is true and the
  object is not selectable, -1.0 is returned, too.

  If the item is represented not by single lines but by an area like QCPItemRect or QCPItemText, a
  click inside the area returns a constant value greater zero (typically the selectionTolerance of
  the parent QCustomPlot multiplied by 0.99). If the click lies outside the area, this function
  returns -1.0.
  
  Providing a constant value for area objects allows selecting line objects even when they are
  obscured by such area objects, by clicking close to the lines (i.e. closer than
  0.99*selectionTolerance).
  
  The actual setting of the selection state is not done by this function. This is handled by the
  parent QCustomPlot when the mouseReleaseEvent occurs, and the finally selected object is notified
  via the selectEvent/deselectEvent methods.
  
  \a details is an optional output parameter. Every layerable subclass may place any information
  in \a details. This information will be passed to \ref selectEvent when the parent QCustomPlot
  decides on the basis of this selectTest call, that the object was successfully selected. The
  subsequent call to \ref selectEvent will carry the \a details. This is useful for multi-part
  objects (like QCPAxis). This way, a possibly complex calculation to decide which part was clicked
  is only done once in \ref selectTest. The result (i.e. the actually clicked part) can then be
  placed in \a details. So in the subsequent \ref selectEvent, the decision which part was
  selected doesn't have to be done a second time for a single selection operation.
  
  You may pass 0 as \a details to indicate that you are not interested in those selection details.
  
  \see selectEvent, deselectEvent, QCustomPlot::setInteractions
*/
double QCPLayerable::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(pos)
  Q_UNUSED(onlySelectable)
  Q_UNUSED(details)
  return -1.0;
}

/*! \internal
  
  Sets the parent plot of this layerable. Use this function once to set the parent plot if you have
  passed 0 in the constructor. It can not be used to move a layerable from one QCustomPlot to
  another one.
  
  Note that, unlike when passing a non-null parent plot in the constructor, this function does not
  make \a parentPlot the QObject-parent of this layerable. If you want this, call
  QObject::setParent(\a parentPlot) in addition to this function.
  
  Further, you will probably want to set a layer (\ref setLayer) after calling this function, to
  make the layerable appear on the QCustomPlot.
  
  The parent plot change will be propagated to subclasses via a call to \ref parentPlotInitialized
  so they can react accordingly (e.g. also initialize the parent plot of child layerables, like
  QCPLayout does).
*/
void QCPLayerable::initializeParentPlot(QCustomPlot *parentPlot)
{
  if (mParentPlot)
  {
    qDebug() << Q_FUNC_INFO << "called with mParentPlot already initialized";
    return;
  }
  
  if (!parentPlot)
    qDebug() << Q_FUNC_INFO << "called with parentPlot zero";
  
  mParentPlot = parentPlot;
  parentPlotInitialized(mParentPlot);
}

/*! \internal
  
  Sets the parent layerable of this layerable to \a parentLayerable. Note that \a parentLayerable does not
  become the QObject-parent (for memory management) of this layerable.
  
  The parent layerable has influence on the return value of the \ref realVisibility method. Only
  layerables with a fully visible parent tree will return true for \ref realVisibility, and thus be
  drawn.
  
  \see realVisibility
*/
void QCPLayerable::setParentLayerable(QCPLayerable *parentLayerable)
{
  mParentLayerable = parentLayerable;
}

/*! \internal
  
  Moves this layerable object to \a layer. If \a prepend is true, this object will be prepended to
  the new layer's list, i.e. it will be drawn below the objects already on the layer. If it is
  false, the object will be appended.
  
  Returns true on success, i.e. if \a layer is a valid layer.
*/
bool QCPLayerable::moveToLayer(QCPLayer *layer, bool prepend)
{
  if (layer && !mParentPlot)
  {
    qDebug() << Q_FUNC_INFO << "no parent QCustomPlot set";
    return false;
  }
  if (layer && layer->parentPlot() != mParentPlot)
  {
    qDebug() << Q_FUNC_INFO << "layer" << layer->name() << "is not in same QCustomPlot as this layerable";
    return false;
  }
  
  QCPLayer *oldLayer = mLayer;
  if (mLayer)
    mLayer->removeChild(this);
  mLayer = layer;
  if (mLayer)
    mLayer->addChild(this, prepend);
  if (mLayer != oldLayer)
    emit layerChanged(mLayer);
  return true;
}

/*! \internal

  Sets the QCPainter::setAntialiasing state on the provided \a painter, depending on the \a
  localAntialiased value as well as the overrides \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements. Which override enum this function takes into account is
  controlled via \a overrideElement.
*/
void QCPLayerable::applyAntialiasingHint(QCPPainter *painter, bool localAntialiased, QCP::AntialiasedElement overrideElement) const
{
  if (mParentPlot && mParentPlot->notAntialiasedElements().testFlag(overrideElement))
    painter->setAntialiasing(false);
  else if (mParentPlot && mParentPlot->antialiasedElements().testFlag(overrideElement))
    painter->setAntialiasing(true);
  else
    painter->setAntialiasing(localAntialiased);
}

/*! \internal

  This function is called by \ref initializeParentPlot, to allow subclasses to react on the setting
  of a parent plot. This is the case when 0 was passed as parent plot in the constructor, and the
  parent plot is set at a later time.
  
  For example, QCPLayoutElement/QCPLayout hierarchies may be created independently of any
  QCustomPlot at first. When they are then added to a layout inside the QCustomPlot, the top level
  element of the hierarchy gets its parent plot initialized with \ref initializeParentPlot. To
  propagate the parent plot to all the children of the hierarchy, the top level element then uses
  this function to pass the parent plot on to its child elements.
  
  The default implementation does nothing.
  
  \see initializeParentPlot
*/
void QCPLayerable::parentPlotInitialized(QCustomPlot *parentPlot)
{
   Q_UNUSED(parentPlot)
}

/*! \internal

  Returns the selection category this layerable shall belong to. The selection category is used in
  conjunction with \ref QCustomPlot::setInteractions to control which objects are selectable and
  which aren't.
  
  Subclasses that don't fit any of the normal \ref QCP::Interaction values can use \ref
  QCP::iSelectOther. This is what the default implementation returns.
  
  \see QCustomPlot::setInteractions
*/
QCP::Interaction QCPLayerable::selectionCategory() const
{
  return QCP::iSelectOther;
}

/*! \internal
  
  Returns the clipping rectangle of this layerable object. By default, this is the viewport of the
  parent QCustomPlot. Specific subclasses may reimplement this function to provide different
  clipping rects.
  
  The returned clipping rect is set on the painter before the draw function of the respective
  object is called.
*/
QRect QCPLayerable::clipRect() const
{
  if (mParentPlot)
    return mParentPlot->viewport();
  else
    return QRect();
}

/*! \internal
  
  This event is called when the layerable shall be selected, as a consequence of a click by the
  user. Subclasses should react to it by setting their selection state appropriately. The default
  implementation does nothing.
  
  \a event is the mouse event that caused the selection. \a additive indicates, whether the user
  was holding the multi-select-modifier while performing the selection (see \ref
  QCustomPlot::setMultiSelectModifier). if \a additive is true, the selection state must be toggled
  (i.e. become selected when unselected and unselected when selected).
  
  Every selectEvent is preceded by a call to \ref selectTest, which has returned positively (i.e.
  returned a value greater than 0 and less than the selection tolerance of the parent QCustomPlot).
  The \a details data you output from \ref selectTest is fed back via \a details here. You may
  use it to transport any kind of information from the selectTest to the possibly subsequent
  selectEvent. Usually \a details is used to transfer which part was clicked, if it is a layerable
  that has multiple individually selectable parts (like QCPAxis). This way selectEvent doesn't need
  to do the calculation again to find out which part was actually clicked.
  
  \a selectionStateChanged is an output parameter. If the pointer is non-null, this function must
  set the value either to true or false, depending on whether the selection state of this layerable
  was actually changed. For layerables that only are selectable as a whole and not in parts, this
  is simple: if \a additive is true, \a selectionStateChanged must also be set to true, because the
  selection toggles. If \a additive is false, \a selectionStateChanged is only set to true, if the
  layerable was previously unselected and now is switched to the selected state.
  
  \see selectTest, deselectEvent
*/
void QCPLayerable::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
  Q_UNUSED(event)
  Q_UNUSED(additive)
  Q_UNUSED(details)
  Q_UNUSED(selectionStateChanged)
}

/*! \internal
  
  This event is called when the layerable shall be deselected, either as consequence of a user
  interaction or a call to \ref QCustomPlot::deselectAll. Subclasses should react to it by
  unsetting their selection appropriately.
  
  just as in \ref selectEvent, the output parameter \a selectionStateChanged (if non-null), must
  return true or false when the selection state of this layerable has changed or not changed,
  respectively.
  
  \see selectTest, selectEvent
*/
void QCPLayerable::deselectEvent(bool *selectionStateChanged)
{
  Q_UNUSED(selectionStateChanged)
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPRange
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \class QCPRange
  \brief Represents the range an axis is encompassing.
  
  contains a \a lower and \a upper double value and provides convenience input, output and
  modification functions.
  
  \see QCPAxis::setRange
*/

/*!
  Minimum range size (\a upper - \a lower) the range changing functions will accept. Smaller
  intervals would cause errors due to the 11-bit exponent of double precision numbers,
  corresponding to a minimum magnitude of roughly 1e-308.
  \see validRange, maxRange
*/
const double QCPRange::minRange = 1e-280;

/*!
  Maximum values (negative and positive) the range will accept in range-changing functions.
  Larger absolute values would cause errors due to the 11-bit exponent of double precision numbers,
  corresponding to a maximum magnitude of roughly 1e308.
  Since the number of planck-volumes in the entire visible universe is only ~1e183, this should
  be enough.
  \see validRange, minRange
*/
const double QCPRange::maxRange = 1e250;

/*!
  Constructs a range with \a lower and \a upper set to zero.
*/
QCPRange::QCPRange() :
  lower(0),
  upper(0)
{
}

/*! \overload
  Constructs a range with the specified \a lower and \a upper values.
*/
QCPRange::QCPRange(double lower, double upper) :
  lower(lower),
  upper(upper)
{
  normalize();
}

/*!
  Returns the size of the range, i.e. \a upper-\a lower
*/
double QCPRange::size() const
{
  return upper-lower;
}

/*!
  Returns the center of the range, i.e. (\a upper+\a lower)*0.5
*/
double QCPRange::center() const
{
  return (upper+lower)*0.5;
}

/*!
  Makes sure \a lower is numerically smaller than \a upper. If this is not the case, the values
  are swapped.
*/
void QCPRange::normalize()
{
  if (lower > upper)
    qSwap(lower, upper);
}

/*!
  Expands this range such that \a otherRange is contained in the new range. It is assumed that both
  this range and \a otherRange are normalized (see \ref normalize).
  
  If \a otherRange is already inside the current range, this function does nothing.
  
  \see expanded
*/
void QCPRange::expand(const QCPRange &otherRange)
{
  if (lower > otherRange.lower)
    lower = otherRange.lower;
  if (upper < otherRange.upper)
    upper = otherRange.upper;
}


/*!
  Returns an expanded range that contains this and \a otherRange. It is assumed that both this
  range and \a otherRange are normalized (see \ref normalize).
  
  \see expand
*/
QCPRange QCPRange::expanded(const QCPRange &otherRange) const
{
  QCPRange result = *this;
  result.expand(otherRange);
  return result;
}

/*!
  Returns a sanitized version of the range. Sanitized means for logarithmic scales, that
  the range won't span the positive and negative sign domain, i.e. contain zero. Further
  \a lower will always be numerically smaller (or equal) to \a upper.
  
  If the original range does span positive and negative sign domains or contains zero,
  the returned range will try to approximate the original range as good as possible.
  If the positive interval of the original range is wider than the negative interval, the
  returned range will only contain the positive interval, with lower bound set to \a rangeFac or
  \a rangeFac *\a upper, whichever is closer to zero. Same procedure is used if the negative interval
  is wider than the positive interval, this time by changing the \a upper bound.
*/
QCPRange QCPRange::sanitizedForLogScale() const
{
  double rangeFac = 1e-3;
  QCPRange sanitizedRange(lower, upper);
  sanitizedRange.normalize();
  // can't have range spanning negative and positive values in log plot, so change range to fix it
  //if (qFuzzyCompare(sanitizedRange.lower+1, 1) && !qFuzzyCompare(sanitizedRange.upper+1, 1))
  if (sanitizedRange.lower == 0.0 && sanitizedRange.upper != 0.0)
  {
    // case lower is 0
    if (rangeFac < sanitizedRange.upper*rangeFac)
      sanitizedRange.lower = rangeFac;
    else
      sanitizedRange.lower = sanitizedRange.upper*rangeFac;
  } //else if (!qFuzzyCompare(lower+1, 1) && qFuzzyCompare(upper+1, 1))
  else if (sanitizedRange.lower != 0.0 && sanitizedRange.upper == 0.0)
  {
    // case upper is 0
    if (-rangeFac > sanitizedRange.lower*rangeFac)
      sanitizedRange.upper = -rangeFac;
    else
      sanitizedRange.upper = sanitizedRange.lower*rangeFac;
  } else if (sanitizedRange.lower < 0 && sanitizedRange.upper > 0)
  {
    // find out whether negative or positive interval is wider to decide which sign domain will be chosen
    if (-sanitizedRange.lower > sanitizedRange.upper)
    {
      // negative is wider, do same as in case upper is 0
      if (-rangeFac > sanitizedRange.lower*rangeFac)
        sanitizedRange.upper = -rangeFac;
      else
        sanitizedRange.upper = sanitizedRange.lower*rangeFac;
    } else
    {
      // positive is wider, do same as in case lower is 0
      if (rangeFac < sanitizedRange.upper*rangeFac)
        sanitizedRange.lower = rangeFac;
      else
        sanitizedRange.lower = sanitizedRange.upper*rangeFac;
    }
  }
  // due to normalization, case lower>0 && upper<0 should never occur, because that implies upper<lower
  return sanitizedRange;
}

/*!
  Returns a sanitized version of the range. Sanitized means for linear scales, that
  \a lower will always be numerically smaller (or equal) to \a upper.
*/
QCPRange QCPRange::sanitizedForLinScale() const
{
  QCPRange sanitizedRange(lower, upper);
  sanitizedRange.normalize();
  return sanitizedRange;
}

/*!
  Returns true when \a value lies within or exactly on the borders of the range.
*/
bool QCPRange::contains(double value) const
{
  return value >= lower && value <= upper;
}

/*!
  Checks, whether the specified range is within valid bounds, which are defined
  as QCPRange::maxRange and QCPRange::minRange.
  A valid range means:
  \li range bounds within -maxRange and maxRange
  \li range size above minRange
  \li range size below maxRange
*/
bool QCPRange::validRange(double lower, double upper)
{
  /*
  return (lower > -maxRange &&
          upper < maxRange &&
          qAbs(lower-upper) > minRange &&
          (lower < -minRange || lower > minRange) &&
          (upper < -minRange || upper > minRange));
          */
  return (lower > -maxRange &&
          upper < maxRange &&
          qAbs(lower-upper) > minRange &&
          qAbs(lower-upper) < maxRange);
}

/*!
  \overload
  Checks, whether the specified range is within valid bounds, which are defined
  as QCPRange::maxRange and QCPRange::minRange.
  A valid range means:
  \li range bounds within -maxRange and maxRange
  \li range size above minRange
  \li range size below maxRange
*/
bool QCPRange::validRange(const QCPRange &range)
{
  /*
  return (range.lower > -maxRange &&
          range.upper < maxRange &&
          qAbs(range.lower-range.upper) > minRange &&
          qAbs(range.lower-range.upper) < maxRange &&
          (range.lower < -minRange || range.lower > minRange) &&
          (range.upper < -minRange || range.upper > minRange));
          */
  return (range.lower > -maxRange &&
          range.upper < maxRange &&
          qAbs(range.lower-range.upper) > minRange &&
          qAbs(range.lower-range.upper) < maxRange);
}


/*! \page thelayoutsystem The Layout System
 
  The layout system is responsible for positioning and scaling layout elements such as axis rects,
  legends and plot titles in a QCustomPlot.

  \section layoutsystem-classesandmechanisms Classes and mechanisms
  
  The layout system is based on the abstract base class \ref QCPLayoutElement. All objects that
  take part in the layout system derive from this class, either directly or indirectly.
  
  Since QCPLayoutElement itself derives from \ref QCPLayerable, a layout element may draw its own
  content. However, it is perfectly possible for a layout element to only serve as a structuring
  and/or positioning element, not drawing anything on its own.
  
  \subsection layoutsystem-rects Rects of a layout element
  
  A layout element is a rectangular object described by two rects: the inner rect (\ref
  QCPLayoutElement::rect) and the outer rect (\ref QCPLayoutElement::setOuterRect). The inner rect
  is calculated automatically by applying the margin (\ref QCPLayoutElement::setMargins) inward
  from the outer rect. The inner rect is meant for main content while the margin area may either be
  left blank or serve for displaying peripheral graphics. For example, \ref QCPAxisRect positions
  the four main axes at the sides of the inner rect, so graphs end up inside it and the axis labels
  and tick labels are in the margin area.
  
  \subsection layoutsystem-margins Margins
  
  Each layout element may provide a mechanism to automatically determine its margins. Internally,
  this is realized with the \ref QCPLayoutElement::calculateAutoMargin function which takes a \ref
  QCP::MarginSide and returns an integer value which represents the ideal margin for the specified
  side. The automatic margin will be used on the sides specified in \ref
  QCPLayoutElement::setAutoMargins. By default, it is set to \ref QCP::msAll meaning automatic
  margin calculation is enabled for all four sides. In this case, a minimum margin may be set with
  \ref QCPLayoutElement::setMinimumMargins, to prevent the automatic margin mechanism from setting
  margins smaller than desired for a specific situation. If automatic margin calculation is unset
  for a specific side, the margin of that side can be controlled directy via \ref
  QCPLayoutElement::setMargins.
  
  If multiple layout ements are arranged next to or beneath each other, it may be desirable to
  align their inner rects on certain sides. Since they all might have different automatic margins,
  this usually isn't the case. The class \ref QCPMarginGroup and \ref
  QCPLayoutElement::setMarginGroup fix this by allowing to synchronize multiple margins. See the
  documentation there for details.
  
  \subsection layoutsystem-layout Layouts
  
  As mentioned, a QCPLayoutElement may have an arbitrary number of child layout elements and in
  princple can have the only purpose to manage/arrange those child elements. This is what the
  subclass \ref QCPLayout specializes on. It is a QCPLayoutElement itself but has no visual
  representation. It defines an interface to add, remove and manage child layout elements.
  QCPLayout isn't a usable layout though, it's an abstract base class that concrete layouts derive
  from, like \ref QCPLayoutGrid which arranges its child elements in a grid and \ref QCPLayoutInset
  which allows placing child elements freely inside its rect.
  
  Since a QCPLayout is a layout element itself, it may be placed inside other layouts. This way,
  complex hierarchies may be created, offering very flexible arrangements.
  
  \image html LayoutsystemSketch.png 
  
  Above is a sketch of the default \ref QCPLayoutGrid accessible via \ref QCustomPlot::plotLayout.
  It shows how two child layout elements are placed inside the grid layout next to each other in
  cells (0, 0) and (0, 1).
  
  \subsection layoutsystem-plotlayout The top level plot layout
  
  Every QCustomPlot has one top level layout of type \ref QCPLayoutGrid. It is accessible via \ref
  QCustomPlot::plotLayout and contains (directly or indirectly via other sub-layouts) all layout
  elements in the QCustomPlot. By default, this top level grid layout contains a single cell which
  holds the main axis rect.
 
  \subsection layoutsystem-examples Examples
  
  <b>Adding a plot title</b> is a typical and simple case to demonstrate basic workings of the layout system.
  \code
  // first we create and prepare a plot title layout element:
  QCPPlotTitle *title = new QCPPlotTitle(customPlot);
  title->setText("Plot Title Example");
  title->setFont(QFont("sans", 12, QFont::Bold));
  // then we add it to the main plot layout:
  customPlot->plotLayout()->insertRow(0); // insert an empty row above the axis rect
  customPlot->plotLayout()->addElement(0, 0, title); // place the title in the empty cell we've just created
  \endcode
  \image html layoutsystem-addingplottitle.png

  <b>Arranging multiple axis rects</b> actually is the central purpose of the layout system.
  \code
  customPlot->plotLayout()->clear(); // let's start from scratch and remove the default axis rect
  // add the first axis rect in second row (row index 1):
  QCPAxisRect *topAxisRect = new QCPAxisRect(customPlot);
  customPlot->plotLayout()->addElement(1, 0, topAxisRect);
  // create a sub layout that we'll place in first row:
  QCPLayoutGrid *subLayout = new QCPLayoutGrid;
  customPlot->plotLayout()->addElement(0, 0, subLayout);
  // add two axis rects in the sub layout next to each other:
  QCPAxisRect *leftAxisRect = new QCPAxisRect(customPlot);
  QCPAxisRect *rightAxisRect = new QCPAxisRect(customPlot);
  subLayout->addElement(0, 0, leftAxisRect);
  subLayout->addElement(0, 1, rightAxisRect);
  subLayout->setColumnStretchFactor(0, 3); // left axis rect shall have 60% of width
  subLayout->setColumnStretchFactor(1, 2); // right one only 40% (3:2 = 60:40)
  // since we've created the axis rects and axes from scratch, we need to place them on
  // according layers, if we don't want the grid to be drawn above the axes etc.
  // place the axis on "axes" layer and grids on the "grid" layer, which is below "axes":
  QList<QCPAxis*> allAxes;
  allAxes << topAxisRect->axes() << leftAxisRect->axes() << rightAxisRect->axes();
  foreach (QCPAxis *axis, allAxes)
  {
    axis->setLayer("axes");
    axis->grid()->setLayer("grid");
  }
  \endcode
  \image html layoutsystem-multipleaxisrects.png
  
*/


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPMarginGroup
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPMarginGroup
  \brief A margin group allows synchronization of margin sides if working with multiple layout elements.
  
  QCPMarginGroup allows you to tie a margin side of two or more layout elements together, such that
  they will all have the same size, based on the largest required margin in the group.
  
  \n
  \image html QCPMarginGroup.png "Demonstration of QCPMarginGroup"
  \n
  
  In certain situations it is desirable that margins at specific sides are synchronized across
  layout elements. For example, if one QCPAxisRect is below another one in a grid layout, it will
  provide a cleaner look to the user if the left and right margins of the two axis rects are of the
  same size. The left axis of the top axis rect will then be at the same horizontal position as the
  left axis of the lower axis rect, making them appear aligned. The same applies for the right
  axes. This is what QCPMarginGroup makes possible.
  
  To add/remove a specific side of a layout element to/from a margin group, use the \ref
  QCPLayoutElement::setMarginGroup method. To completely break apart the margin group, either call
  \ref clear, or just delete the margin group.
  
  \section QCPMarginGroup-example Example
  
  First create a margin group:
  \code
  QCPMarginGroup *group = new QCPMarginGroup(customPlot);
  \endcode
  Then set this group on the layout element sides:
  \code
  customPlot->axisRect(0)->setMarginGroup(QCP::msLeft|QCP::msRight, group);
  customPlot->axisRect(1)->setMarginGroup(QCP::msLeft|QCP::msRight, group);
  \endcode
  Here, we've used the first two axis rects of the plot and synchronized their left margins with
  each other and their right margins with each other.
*/

/* start documentation of inline functions */

/*! \fn QList<QCPLayoutElement*> QCPMarginGroup::elements(QCP::MarginSide side) const
  
  Returns a list of all layout elements that have their margin \a side associated with this margin
  group.
*/

/* end documentation of inline functions */

/*!
  Creates a new QCPMarginGroup instance in \a parentPlot.
*/
QCPMarginGroup::QCPMarginGroup(QCustomPlot *parentPlot) :
  QObject(parentPlot),
  mParentPlot(parentPlot)
{
  mChildren.insert(QCP::msLeft, QList<QCPLayoutElement*>());
  mChildren.insert(QCP::msRight, QList<QCPLayoutElement*>());
  mChildren.insert(QCP::msTop, QList<QCPLayoutElement*>());
  mChildren.insert(QCP::msBottom, QList<QCPLayoutElement*>());
}

QCPMarginGroup::~QCPMarginGroup()
{
  clear();
}

/*!
  Returns whether this margin group is empty. If this function returns true, no layout elements use
  this margin group to synchronize margin sides.
*/
bool QCPMarginGroup::isEmpty() const
{
  QHashIterator<QCP::MarginSide, QList<QCPLayoutElement*> > it(mChildren);
  while (it.hasNext())
  {
    it.next();
    if (!it.value().isEmpty())
      return false;
  }
  return true;
}

/*!
  Clears this margin group. The synchronization of the margin sides that use this margin group is
  lifted and they will use their individual margin sizes again.
*/
void QCPMarginGroup::clear()
{
  // make all children remove themselves from this margin group:
  QHashIterator<QCP::MarginSide, QList<QCPLayoutElement*> > it(mChildren);
  while (it.hasNext())
  {
    it.next();
    const QList<QCPLayoutElement*> elements = it.value();
    for (int i=elements.size()-1; i>=0; --i)
      elements.at(i)->setMarginGroup(it.key(), 0); // removes itself from mChildren via removeChild
  }
}

/*! \internal
  
  Returns the synchronized common margin for \a side. This is the margin value that will be used by
  the layout element on the respective side, if it is part of this margin group.
  
  The common margin is calculated by requesting the automatic margin (\ref
  QCPLayoutElement::calculateAutoMargin) of each element associated with \a side in this margin
  group, and choosing the largest returned value. (QCPLayoutElement::minimumMargins is taken into
  account, too.)
*/
int QCPMarginGroup::commonMargin(QCP::MarginSide side) const
{
  // query all automatic margins of the layout elements in this margin group side and find maximum:
  int result = 0;
  const QList<QCPLayoutElement*> elements = mChildren.value(side);
  for (int i=0; i<elements.size(); ++i)
  {
    if (!elements.at(i)->autoMargins().testFlag(side))
      continue;
    int m = qMax(elements.at(i)->calculateAutoMargin(side), QCP::getMarginValue(elements.at(i)->minimumMargins(), side));
    if (m > result)
      result = m;
  }
  return result;
}

/*! \internal
  
  Adds \a element to the internal list of child elements, for the margin \a side.
  
  This function does not modify the margin group property of \a element.
*/
void QCPMarginGroup::addChild(QCP::MarginSide side, QCPLayoutElement *element)
{
  if (!mChildren[side].contains(element))
    mChildren[side].append(element);
  else
    qDebug() << Q_FUNC_INFO << "element is already child of this margin group side" << reinterpret_cast<quintptr>(element);
}

/*! \internal
  
  Removes \a element from the internal list of child elements, for the margin \a side.
  
  This function does not modify the margin group property of \a element.
*/
void QCPMarginGroup::removeChild(QCP::MarginSide side, QCPLayoutElement *element)
{
  if (!mChildren[side].removeOne(element))
    qDebug() << Q_FUNC_INFO << "element is not child of this margin group side" << reinterpret_cast<quintptr>(element);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayoutElement
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLayoutElement
  \brief The abstract base class for all objects that form \ref thelayoutsystem "the layout system".
  
  This is an abstract base class. As such, it can't be instantiated directly, rather use one of its subclasses.
  
  A Layout element is a rectangular object which can be placed in layouts. It has an outer rect
  (QCPLayoutElement::outerRect) and an inner rect (\ref QCPLayoutElement::rect). The difference
  between outer and inner rect is called its margin. The margin can either be set to automatic or
  manual (\ref setAutoMargins) on a per-side basis. If a side is set to manual, that margin can be
  set explicitly with \ref setMargins and will stay fixed at that value. If it's set to automatic,
  the layout element subclass will control the value itself (via \ref calculateAutoMargin).
  
  Layout elements can be placed in layouts (base class QCPLayout) like QCPLayoutGrid. The top level
  layout is reachable via \ref QCustomPlot::plotLayout, and is a \ref QCPLayoutGrid. Since \ref
  QCPLayout itself derives from \ref QCPLayoutElement, layouts can be nested.
  
  Thus in QCustomPlot one can divide layout elements into two categories: The ones that are
  invisible by themselves, because they don't draw anything. Their only purpose is to manage the
  position and size of other layout elements. This category of layout elements usually use
  QCPLayout as base class. Then there is the category of layout elements which actually draw
  something. For example, QCPAxisRect, QCPLegend and QCPPlotTitle are of this category. This does
  not necessarily mean that the latter category can't have child layout elements. QCPLegend for
  instance, actually derives from QCPLayoutGrid and the individual legend items are child layout
  elements in the grid layout.
*/

/* start documentation of inline functions */

/*! \fn QCPLayout *QCPLayoutElement::layout() const
  
  Returns the parent layout of this layout element.
*/

/*! \fn QRect QCPLayoutElement::rect() const
  
  Returns the inner rect of this layout element. The inner rect is the outer rect (\ref
  setOuterRect) shrinked by the margins (\ref setMargins, \ref setAutoMargins).
  
  In some cases, the area between outer and inner rect is left blank. In other cases the margin
  area is used to display peripheral graphics while the main content is in the inner rect. This is
  where automatic margin calculation becomes interesting because it allows the layout element to
  adapt the margins to the peripheral graphics it wants to draw. For example, \ref QCPAxisRect
  draws the axis labels and tick labels in the margin area, thus needs to adjust the margins (if
  \ref setAutoMargins is enabled) according to the space required by the labels of the axes.
*/

/*! \fn virtual void QCPLayoutElement::mousePressEvent(QMouseEvent *event)
  
  This event is called, if the mouse was pressed while being inside the outer rect of this layout
  element.
*/

/*! \fn virtual void QCPLayoutElement::mouseMoveEvent(QMouseEvent *event)
  
  This event is called, if the mouse is moved inside the outer rect of this layout element.
*/

/*! \fn virtual void QCPLayoutElement::mouseReleaseEvent(QMouseEvent *event)
  
  This event is called, if the mouse was previously pressed inside the outer rect of this layout
  element and is now released.
*/

/*! \fn virtual void QCPLayoutElement::mouseDoubleClickEvent(QMouseEvent *event)
  
  This event is called, if the mouse is double-clicked inside the outer rect of this layout
  element.
*/

/*! \fn virtual void QCPLayoutElement::wheelEvent(QWheelEvent *event)
  
  This event is called, if the mouse wheel is scrolled while the cursor is inside the rect of this
  layout element.
*/

/* end documentation of inline functions */

/*!
  Creates an instance of QCPLayoutElement and sets default values.
*/
QCPLayoutElement::QCPLayoutElement(QCustomPlot *parentPlot) :
  QCPLayerable(parentPlot), // parenthood is changed as soon as layout element gets inserted into a layout (except for top level layout)
  mParentLayout(0),
  mMinimumSize(),
  mMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX),
  mRect(0, 0, 0, 0),
  mOuterRect(0, 0, 0, 0),
  mMargins(0, 0, 0, 0),
  mMinimumMargins(0, 0, 0, 0),
  mAutoMargins(QCP::msAll)
{
}

QCPLayoutElement::~QCPLayoutElement()
{
  setMarginGroup(QCP::msAll, 0); // unregister at margin groups, if there are any
  // unregister at layout:
  if (qobject_cast<QCPLayout*>(mParentLayout)) // the qobject_cast is just a safeguard in case the layout forgets to call clear() in its dtor and this dtor is called by QObject dtor
    mParentLayout->take(this);
}

/*!
  Sets the outer rect of this layout element. If the layout element is inside a layout, the layout
  sets the position and size of this layout element using this function.
  
  Calling this function externally has no effect, since the layout will overwrite any changes to
  the outer rect upon the next replot.
  
  The layout element will adapt its inner \ref rect by applying the margins inward to the outer rect.
  
  \see rect
*/
void QCPLayoutElement::setOuterRect(const QRect &rect)
{
  if (mOuterRect != rect)
  {
    mOuterRect = rect;
    mRect = mOuterRect.adjusted(mMargins.left(), mMargins.top(), -mMargins.right(), -mMargins.bottom());
  }
}

/*!
  Sets the margins of this layout element. If \ref setAutoMargins is disabled for some or all
  sides, this function is used to manually set the margin on those sides. Sides that are still set
  to be handled automatically are ignored and may have any value in \a margins.
  
  The margin is the distance between the outer rect (controlled by the parent layout via \ref
  setOuterRect) and the inner \ref rect (which usually contains the main content of this layout
  element).
  
  \see setAutoMargins
*/
void QCPLayoutElement::setMargins(const QMargins &margins)
{
  if (mMargins != margins)
  {
    mMargins = margins;
    mRect = mOuterRect.adjusted(mMargins.left(), mMargins.top(), -mMargins.right(), -mMargins.bottom());
  }
}

/*!
  If \ref setAutoMargins is enabled on some or all margins, this function is used to provide
  minimum values for those margins.
  
  The minimum values are not enforced on margin sides that were set to be under manual control via
  \ref setAutoMargins.
  
  \see setAutoMargins
*/
void QCPLayoutElement::setMinimumMargins(const QMargins &margins)
{
  if (mMinimumMargins != margins)
  {
    mMinimumMargins = margins;
  }
}

/*!
  Sets on which sides the margin shall be calculated automatically. If a side is calculated
  automatically, a minimum margin value may be provided with \ref setMinimumMargins. If a side is
  set to be controlled manually, the value may be specified with \ref setMargins.
  
  Margin sides that are under automatic control may participate in a \ref QCPMarginGroup (see \ref
  setMarginGroup), to synchronize (align) it with other layout elements in the plot.
  
  \see setMinimumMargins, setMargins
*/
void QCPLayoutElement::setAutoMargins(QCP::MarginSides sides)
{
  mAutoMargins = sides;
}

/*!
  Sets the minimum size for the inner \ref rect of this layout element. A parent layout tries to
  respect the \a size here by changing row/column sizes in the layout accordingly.
  
  If the parent layout size is not sufficient to satisfy all minimum size constraints of its child
  layout elements, the layout may set a size that is actually smaller than \a size. QCustomPlot
  propagates the layout's size constraints to the outside by setting its own minimum QWidget size
  accordingly, so violations of \a size should be exceptions.
*/
void QCPLayoutElement::setMinimumSize(const QSize &size)
{
  if (mMinimumSize != size)
  {
    mMinimumSize = size;
    if (mParentLayout)
      mParentLayout->sizeConstraintsChanged();
  }
}

/*! \overload
  
  Sets the minimum size for the inner \ref rect of this layout element.
*/
void QCPLayoutElement::setMinimumSize(int width, int height)
{
  setMinimumSize(QSize(width, height));
}

/*!
  Sets the maximum size for the inner \ref rect of this layout element. A parent layout tries to
  respect the \a size here by changing row/column sizes in the layout accordingly.
*/
void QCPLayoutElement::setMaximumSize(const QSize &size)
{
  if (mMaximumSize != size)
  {
    mMaximumSize = size;
    if (mParentLayout)
      mParentLayout->sizeConstraintsChanged();
  }
}

/*! \overload
  
  Sets the maximum size for the inner \ref rect of this layout element.
*/
void QCPLayoutElement::setMaximumSize(int width, int height)
{
  setMaximumSize(QSize(width, height));
}

/*!
  Sets the margin \a group of the specified margin \a sides.
  
  Margin groups allow synchronizing specified margins across layout elements, see the documentation
  of \ref QCPMarginGroup.
  
  To unset the margin group of \a sides, set \a group to 0.
  
  Note that margin groups only work for margin sides that are set to automatic (\ref
  setAutoMargins).
*/
void QCPLayoutElement::setMarginGroup(QCP::MarginSides sides, QCPMarginGroup *group)
{
  QVector<QCP::MarginSide> sideVector;
  if (sides.testFlag(QCP::msLeft)) sideVector.append(QCP::msLeft);
  if (sides.testFlag(QCP::msRight)) sideVector.append(QCP::msRight);
  if (sides.testFlag(QCP::msTop)) sideVector.append(QCP::msTop);
  if (sides.testFlag(QCP::msBottom)) sideVector.append(QCP::msBottom);
  
  for (int i=0; i<sideVector.size(); ++i)
  {
    QCP::MarginSide side = sideVector.at(i);
    if (marginGroup(side) != group)
    {
      QCPMarginGroup *oldGroup = marginGroup(side);
      if (oldGroup) // unregister at old group
        oldGroup->removeChild(side, this);
      
      if (!group) // if setting to 0, remove hash entry. Else set hash entry to new group and register there
      {
        mMarginGroups.remove(side);
      } else // setting to a new group
      {
        mMarginGroups[side] = group;
        group->addChild(side, this);
      }
    }
  }
}

/*!
  Updates the layout element and sub-elements. This function is automatically called before every
  replot by the parent layout element. It is called multiple times, once for every \ref
  UpdatePhase. The phases are run through in the order of the enum values. For details about what
  happens at the different phases, see the documentation of \ref UpdatePhase.
  
  Layout elements that have child elements should call the \ref update method of their child
  elements, and pass the current \a phase unchanged.
  
  The default implementation executes the automatic margin mechanism in the \ref upMargins phase.
  Subclasses should make sure to call the base class implementation.
*/
void QCPLayoutElement::update(UpdatePhase phase)
{
  if (phase == upMargins)
  {
    if (mAutoMargins != QCP::msNone)
    {
      // set the margins of this layout element according to automatic margin calculation, either directly or via a margin group:
      QMargins newMargins = mMargins;
      foreach (QCP::MarginSide side, QList<QCP::MarginSide>() << QCP::msLeft << QCP::msRight << QCP::msTop << QCP::msBottom)
      {
        if (mAutoMargins.testFlag(side)) // this side's margin shall be calculated automatically
        {
          if (mMarginGroups.contains(side))
            QCP::setMarginValue(newMargins, side, mMarginGroups[side]->commonMargin(side)); // this side is part of a margin group, so get the margin value from that group
          else
            QCP::setMarginValue(newMargins, side, calculateAutoMargin(side)); // this side is not part of a group, so calculate the value directly
          // apply minimum margin restrictions:
          if (QCP::getMarginValue(newMargins, side) < QCP::getMarginValue(mMinimumMargins, side))
            QCP::setMarginValue(newMargins, side, QCP::getMarginValue(mMinimumMargins, side));
        }
      }
      setMargins(newMargins);
    }
  }
}

/*!
  Returns the minimum size this layout element (the inner \ref rect) may be compressed to.
  
  if a minimum size (\ref setMinimumSize) was not set manually, parent layouts consult this
  function to determine the minimum allowed size of this layout element. (A manual minimum size is
  considered set if it is non-zero.)
*/
QSize QCPLayoutElement::minimumSizeHint() const
{
  return mMinimumSize;
}

/*!
  Returns the maximum size this layout element (the inner \ref rect) may be expanded to.
  
  if a maximum size (\ref setMaximumSize) was not set manually, parent layouts consult this
  function to determine the maximum allowed size of this layout element. (A manual maximum size is
  considered set if it is smaller than Qt's QWIDGETSIZE_MAX.)
*/
QSize QCPLayoutElement::maximumSizeHint() const
{
  return mMaximumSize;
}

/*!
  Returns a list of all child elements in this layout element. If \a recursive is true, all
  sub-child elements are included in the list, too.
  
  \warning There may be entries with value 0 in the returned list. (For example, QCPLayoutGrid may have
  empty cells which yield 0 at the respective index.)
*/
QList<QCPLayoutElement*> QCPLayoutElement::elements(bool recursive) const
{
  Q_UNUSED(recursive)
  return QList<QCPLayoutElement*>();
}

/*!
  Layout elements are sensitive to events inside their outer rect. If \a pos is within the outer
  rect, this method returns a value corresponding to 0.99 times the parent plot's selection
  tolerance. However, layout elements are not selectable by default. So if \a onlySelectable is
  true, -1.0 is returned.
  
  See \ref QCPLayerable::selectTest for a general explanation of this virtual method.
  
  QCPLayoutElement subclasses may reimplement this method to provide more specific selection test
  behaviour.
*/
double QCPLayoutElement::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  
  if (onlySelectable)
    return -1;
  
  if (QRectF(mOuterRect).contains(pos))
  {
    if (mParentPlot)
      return mParentPlot->selectionTolerance()*0.99;
    else
    {
      qDebug() << Q_FUNC_INFO << "parent plot not defined";
      return -1;
    }
  } else
    return -1;
}

/*! \internal
  
  propagates the parent plot initialization to all child elements, by calling \ref
  QCPLayerable::initializeParentPlot on them.
*/
void QCPLayoutElement::parentPlotInitialized(QCustomPlot *parentPlot)
{
  foreach (QCPLayoutElement* el, elements(false))
  {
    if (!el->parentPlot())
      el->initializeParentPlot(parentPlot);
  }
}

/*! \internal
  
  Returns the margin size for this \a side. It is used if automatic margins is enabled for this \a
  side (see \ref setAutoMargins). If a minimum margin was set with \ref setMinimumMargins, the
  returned value will not be smaller than the specified minimum margin.
  
  The default implementation just returns the respective manual margin (\ref setMargins) or the
  minimum margin, whichever is larger.
*/
int QCPLayoutElement::calculateAutoMargin(QCP::MarginSide side)
{
  return qMax(QCP::getMarginValue(mMargins, side), QCP::getMarginValue(mMinimumMargins, side));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayout
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLayout
  \brief The abstract base class for layouts
  
  This is an abstract base class for layout elements whose main purpose is to define the position
  and size of other child layout elements. In most cases, layouts don't draw anything themselves
  (but there are exceptions to this, e.g. QCPLegend).
  
  QCPLayout derives from QCPLayoutElement, and thus can itself be nested in other layouts.
  
  QCPLayout introduces a common interface for accessing and manipulating the child elements. Those
  functions are most notably \ref elementCount, \ref elementAt, \ref takeAt, \ref take, \ref
  simplify, \ref removeAt, \ref remove and \ref clear. Individual subclasses may add more functions
  to this interface which are more specialized to the form of the layout. For example, \ref
  QCPLayoutGrid adds functions that take row and column indices to access cells of the layout grid
  more conveniently.
  
  Since this is an abstract base class, you can't instantiate it directly. Rather use one of its
  subclasses like QCPLayoutGrid or QCPLayoutInset.
  
  For a general introduction to the layout system, see the dedicated documentation page \ref
  thelayoutsystem "The Layout System".
*/

/* start documentation of pure virtual functions */

/*! \fn virtual int QCPLayout::elementCount() const = 0
  
  Returns the number of elements/cells in the layout.
  
  \see elements, elementAt
*/

/*! \fn virtual QCPLayoutElement* QCPLayout::elementAt(int index) const = 0
  
  Returns the element in the cell with the given \a index. If \a index is invalid, returns 0.
  
  Note that even if \a index is valid, the respective cell may be empty in some layouts (e.g.
  QCPLayoutGrid), so this function may return 0 in those cases. You may use this function to check
  whether a cell is empty or not.
  
  \see elements, elementCount, takeAt
*/

/*! \fn virtual QCPLayoutElement* QCPLayout::takeAt(int index) = 0
  
  Removes the element with the given \a index from the layout and returns it.
  
  If the \a index is invalid or the cell with that index is empty, returns 0.
  
  Note that some layouts don't remove the respective cell right away but leave an empty cell after
  successful removal of the layout element. To collapse empty cells, use \ref simplify.
  
  \see elementAt, take
*/

/*! \fn virtual bool QCPLayout::take(QCPLayoutElement* element) = 0
  
  Removes the specified \a element from the layout and returns true on success.
  
  If the \a element isn't in this layout, returns false.
  
  Note that some layouts don't remove the respective cell right away but leave an empty cell after
  successful removal of the layout element. To collapse empty cells, use \ref simplify.
  
  \see takeAt
*/

/* end documentation of pure virtual functions */

/*!
  Creates an instance of QCPLayout and sets default values. Note that since QCPLayout
  is an abstract base class, it can't be instantiated directly.
*/
QCPLayout::QCPLayout()
{
}

/*!
  First calls the QCPLayoutElement::update base class implementation to update the margins on this
  layout.
  
  Then calls \ref updateLayout which subclasses reimplement to reposition and resize their cells.
  
  Finally, \ref update is called on all child elements.
*/
void QCPLayout::update(UpdatePhase phase)
{
  QCPLayoutElement::update(phase);
  
  // set child element rects according to layout:
  if (phase == upLayout)
    updateLayout();
  
  // propagate update call to child elements:
  const int elCount = elementCount();
  for (int i=0; i<elCount; ++i)
  {
    if (QCPLayoutElement *el = elementAt(i))
      el->update(phase);
  }
}

/* inherits documentation from base class */
QList<QCPLayoutElement*> QCPLayout::elements(bool recursive) const
{
  const int c = elementCount();
  QList<QCPLayoutElement*> result;
#if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
  result.reserve(c);
#endif
  for (int i=0; i<c; ++i)
    result.append(elementAt(i));
  if (recursive)
  {
    for (int i=0; i<c; ++i)
    {
      if (result.at(i))
        result << result.at(i)->elements(recursive);
    }
  }
  return result;
}

/*!
  Simplifies the layout by collapsing empty cells. The exact behavior depends on subclasses, the
  default implementation does nothing.
  
  Not all layouts need simplification. For example, QCPLayoutInset doesn't use explicit
  simplification while QCPLayoutGrid does.
*/
void QCPLayout::simplify()
{
}

/*!
  Removes and deletes the element at the provided \a index. Returns true on success. If \a index is
  invalid or points to an empty cell, returns false.
  
  This function internally uses \ref takeAt to remove the element from the layout and then deletes
  the returned element.
  
  \see remove, takeAt
*/
bool QCPLayout::removeAt(int index)
{
  if (QCPLayoutElement *el = takeAt(index))
  {
    delete el;
    return true;
  } else
    return false;
}

/*!
  Removes and deletes the provided \a element. Returns true on success. If \a element is not in the
  layout, returns false.
  
  This function internally uses \ref takeAt to remove the element from the layout and then deletes
  the element.
  
  \see removeAt, take
*/
bool QCPLayout::remove(QCPLayoutElement *element)
{
  if (take(element))
  {
    delete element;
    return true;
  } else
    return false;
}

/*!
  Removes and deletes all layout elements in this layout.
  
  \see remove, removeAt
*/
void QCPLayout::clear()
{
  for (int i=elementCount()-1; i>=0; --i)
  {
    if (elementAt(i))
      removeAt(i);
  }
  simplify();
}

/*!
  Subclasses call this method to report changed (minimum/maximum) size constraints.
  
  If the parent of this layout is again a QCPLayout, forwards the call to the parent's \ref
  sizeConstraintsChanged. If the parent is a QWidget (i.e. is the \ref QCustomPlot::plotLayout of
  QCustomPlot), calls QWidget::updateGeometry, so if the QCustomPlot widget is inside a Qt QLayout,
  it may update itself and resize cells accordingly.
*/
void QCPLayout::sizeConstraintsChanged() const
{
  if (QWidget *w = qobject_cast<QWidget*>(parent()))
    w->updateGeometry();
  else if (QCPLayout *l = qobject_cast<QCPLayout*>(parent()))
    l->sizeConstraintsChanged();
}

/*! \internal
  
  Subclasses reimplement this method to update the position and sizes of the child elements/cells
  via calling their \ref QCPLayoutElement::setOuterRect. The default implementation does nothing.
  
  The geometry used as a reference is the inner \ref rect of this layout. Child elements should stay
  within that rect.
  
  \ref getSectionSizes may help with the reimplementation of this function.
  
  \see update
*/
void QCPLayout::updateLayout()
{
}


/*! \internal
  
  Associates \a el with this layout. This is done by setting the \ref QCPLayoutElement::layout, the
  \ref QCPLayerable::parentLayerable and the QObject parent to this layout.
  
  Further, if \a el didn't previously have a parent plot, calls \ref
  QCPLayerable::initializeParentPlot on \a el to set the paret plot.
  
  This method is used by subclass specific methods that add elements to the layout. Note that this
  method only changes properties in \a el. The removal from the old layout and the insertion into
  the new layout must be done additionally.
*/
void QCPLayout::adoptElement(QCPLayoutElement *el)
{
  if (el)
  {
    el->mParentLayout = this;
    el->setParentLayerable(this);
    el->setParent(this);
    if (!el->parentPlot())
      el->initializeParentPlot(mParentPlot);
  } else
    qDebug() << Q_FUNC_INFO << "Null element passed";
}

/*! \internal
  
  Disassociates \a el from this layout. This is done by setting the \ref QCPLayoutElement::layout
  and the \ref QCPLayerable::parentLayerable to zero. The QObject parent is set to the parent
  QCustomPlot.
  
  This method is used by subclass specific methods that remove elements from the layout (e.g. \ref
  take or \ref takeAt). Note that this method only changes properties in \a el. The removal from
  the old layout must be done additionally.
*/
void QCPLayout::releaseElement(QCPLayoutElement *el)
{
  if (el)
  {
    el->mParentLayout = 0;
    el->setParentLayerable(0);
    el->setParent(mParentPlot);
    // Note: Don't initializeParentPlot(0) here, because layout element will stay in same parent plot
  } else
    qDebug() << Q_FUNC_INFO << "Null element passed";
}

/*! \internal
  
  This is a helper function for the implementation of \ref updateLayout in subclasses.
  
  It calculates the sizes of one-dimensional sections with provided constraints on maximum section
  sizes, minimum section sizes, relative stretch factors and the final total size of all sections.
  
  The QVector entries refer to the sections. Thus all QVectors must have the same size.
  
  \a maxSizes gives the maximum allowed size of each section. If there shall be no maximum size
  imposed, set all vector values to Qt's QWIDGETSIZE_MAX.
  
  \a minSizes gives the minimum allowed size of each section. If there shall be no minimum size
  imposed, set all vector values to zero. If the \a minSizes entries add up to a value greater than
  \a totalSize, sections will be scaled smaller than the proposed minimum sizes. (In other words,
  not exceeding the allowed total size is taken to be more important than not going below minimum
  section sizes.)
  
  \a stretchFactors give the relative proportions of the sections to each other. If all sections
  shall be scaled equally, set all values equal. If the first section shall be double the size of
  each individual other section, set the first number of \a stretchFactors to double the value of
  the other individual values (e.g. {2, 1, 1, 1}).
  
  \a totalSize is the value that the final section sizes will add up to. Due to rounding, the
  actual sum may differ slightly. If you want the section sizes to sum up to exactly that value,
  you could distribute the remaining difference on the sections.
  
  The return value is a QVector containing the section sizes.
*/
QVector<int> QCPLayout::getSectionSizes(QVector<int> maxSizes, QVector<int> minSizes, QVector<double> stretchFactors, int totalSize) const
{
  if (maxSizes.size() != minSizes.size() || minSizes.size() != stretchFactors.size())
  {
    qDebug() << Q_FUNC_INFO << "Passed vector sizes aren't equal:" << maxSizes << minSizes << stretchFactors;
    return QVector<int>();
  }
  if (stretchFactors.isEmpty())
    return QVector<int>();
  int sectionCount = stretchFactors.size();
  QVector<double> sectionSizes(sectionCount);
  // if provided total size is forced smaller than total minimum size, ignore minimum sizes (squeeze sections):
  int minSizeSum = 0;
  for (int i=0; i<sectionCount; ++i)
    minSizeSum += minSizes.at(i);
  if (totalSize < minSizeSum)
  {
    // new stretch factors are minimum sizes and minimum sizes are set to zero:
    for (int i=0; i<sectionCount; ++i)
    {
      stretchFactors[i] = minSizes.at(i);
      minSizes[i] = 0;
    }
  }
  
  QList<int> minimumLockedSections;
  QList<int> unfinishedSections;
  for (int i=0; i<sectionCount; ++i)
    unfinishedSections.append(i);
  double freeSize = totalSize;
  
  int outerIterations = 0;
  while (!unfinishedSections.isEmpty() && outerIterations < sectionCount*2) // the iteration check ist just a failsafe in case something really strange happens
  {
    ++outerIterations;
    int innerIterations = 0;
    while (!unfinishedSections.isEmpty() && innerIterations < sectionCount*2) // the iteration check ist just a failsafe in case something really strange happens
    {
      ++innerIterations;
      // find section that hits its maximum next:
      int nextId = -1;
      double nextMax = 1e12;
      for (int i=0; i<unfinishedSections.size(); ++i)
      {
        int secId = unfinishedSections.at(i);
        double hitsMaxAt = (maxSizes.at(secId)-sectionSizes.at(secId))/stretchFactors.at(secId);
        if (hitsMaxAt < nextMax)
        {
          nextMax = hitsMaxAt;
          nextId = secId;
        }
      }
      // check if that maximum is actually within the bounds of the total size (i.e. can we stretch all remaining sections so far that the found section
      // actually hits its maximum, without exceeding the total size when we add up all sections)
      double stretchFactorSum = 0;
      for (int i=0; i<unfinishedSections.size(); ++i)
        stretchFactorSum += stretchFactors.at(unfinishedSections.at(i));
      double nextMaxLimit = freeSize/stretchFactorSum;
      if (nextMax < nextMaxLimit) // next maximum is actually hit, move forward to that point and fix the size of that section
      {
        for (int i=0; i<unfinishedSections.size(); ++i)
        {
          sectionSizes[unfinishedSections.at(i)] += nextMax*stretchFactors.at(unfinishedSections.at(i)); // increment all sections
          freeSize -= nextMax*stretchFactors.at(unfinishedSections.at(i));
        }
        unfinishedSections.removeOne(nextId); // exclude the section that is now at maximum from further changes
      } else // next maximum isn't hit, just distribute rest of free space on remaining sections
      {
        for (int i=0; i<unfinishedSections.size(); ++i)
          sectionSizes[unfinishedSections.at(i)] += nextMaxLimit*stretchFactors.at(unfinishedSections.at(i)); // increment all sections
        unfinishedSections.clear();
      }
    }
    if (innerIterations == sectionCount*2)
      qDebug() << Q_FUNC_INFO << "Exceeded maximum expected inner iteration count, layouting aborted. Input was:" << maxSizes << minSizes << stretchFactors << totalSize;
    
    // now check whether the resulting section sizes violate minimum restrictions:
    bool foundMinimumViolation = false;
    for (int i=0; i<sectionSizes.size(); ++i)
    {
      if (minimumLockedSections.contains(i))
        continue;
      if (sectionSizes.at(i) < minSizes.at(i)) // section violates minimum
      {
        sectionSizes[i] = minSizes.at(i); // set it to minimum
        foundMinimumViolation = true; // make sure we repeat the whole optimization process
        minimumLockedSections.append(i);
      }
    }
    if (foundMinimumViolation)
    {
      freeSize = totalSize;
      for (int i=0; i<sectionCount; ++i)
      {
        if (!minimumLockedSections.contains(i)) // only put sections that haven't hit their minimum back into the pool
          unfinishedSections.append(i);
        else
          freeSize -= sectionSizes.at(i); // remove size of minimum locked sections from available space in next round
      }
      // reset all section sizes to zero that are in unfinished sections (all others have been set to their minimum):
      for (int i=0; i<unfinishedSections.size(); ++i)
        sectionSizes[unfinishedSections.at(i)] = 0;
    }
  }
  if (outerIterations == sectionCount*2)
    qDebug() << Q_FUNC_INFO << "Exceeded maximum expected outer iteration count, layouting aborted. Input was:" << maxSizes << minSizes << stretchFactors << totalSize;
  
  QVector<int> result(sectionCount);
  for (int i=0; i<sectionCount; ++i)
    result[i] = qRound(sectionSizes.at(i));
  return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayoutGrid
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLayoutGrid
  \brief A layout that arranges child elements in a grid
  
  Elements are laid out in a grid with configurable stretch factors (\ref setColumnStretchFactor,
  \ref setRowStretchFactor) and spacing (\ref setColumnSpacing, \ref setRowSpacing).
  
  Elements can be added to cells via \ref addElement. The grid is expanded if the specified row or
  column doesn't exist yet. Whether a cell contains a valid layout element can be checked with \ref
  hasElement, that element can be retrieved with \ref element. If rows and columns that only have
  empty cells shall be removed, call \ref simplify. Removal of elements is either done by just
  adding the element to a different layout or by using the QCPLayout interface \ref take or \ref
  remove.
  
  Row and column insertion can be performed with \ref insertRow and \ref insertColumn.
*/

/*!
  Creates an instance of QCPLayoutGrid and sets default values.
*/
QCPLayoutGrid::QCPLayoutGrid() :
  mColumnSpacing(5),
  mRowSpacing(5)
{
}

QCPLayoutGrid::~QCPLayoutGrid()
{
  // clear all child layout elements. This is important because only the specific layouts know how
  // to handle removing elements (clear calls virtual removeAt method to do that).
  clear();
}

/*!
  Returns the element in the cell in \a row and \a column.
  
  Returns 0 if either the row/column is invalid or if the cell is empty. In those cases, a qDebug
  message is printed. To check whether a cell exists and isn't empty, use \ref hasElement.
  
  \see addElement, hasElement
*/
QCPLayoutElement *QCPLayoutGrid::element(int row, int column) const
{
  if (row >= 0 && row < mElements.size())
  {
    if (column >= 0 && column < mElements.first().size())
    {
      if (QCPLayoutElement *result = mElements.at(row).at(column))
        return result;
      else
        qDebug() << Q_FUNC_INFO << "Requested cell is empty. Row:" << row << "Column:" << column;
    } else
      qDebug() << Q_FUNC_INFO << "Invalid column. Row:" << row << "Column:" << column;
  } else
    qDebug() << Q_FUNC_INFO << "Invalid row. Row:" << row << "Column:" << column;
  return 0;
}

/*!
  Returns the number of rows in the layout.
  
  \see columnCount
*/
int QCPLayoutGrid::rowCount() const
{
  return mElements.size();
}

/*!
  Returns the number of columns in the layout.
  
  \see rowCount
*/
int QCPLayoutGrid::columnCount() const
{
  if (mElements.size() > 0)
    return mElements.first().size();
  else
    return 0;
}

/*!
  Adds the \a element to cell with \a row and \a column. If \a element is already in a layout, it
  is first removed from there. If \a row or \a column don't exist yet, the layout is expanded
  accordingly.
  
  Returns true if the element was added successfully, i.e. if the cell at \a row and \a column
  didn't already have an element.
  
  \see element, hasElement, take, remove
*/
bool QCPLayoutGrid::addElement(int row, int column, QCPLayoutElement *element)
{
  if (element)
  {
    if (!hasElement(row, column))
    {
      if (element->layout()) // remove from old layout first
        element->layout()->take(element);
      expandTo(row+1, column+1);
      mElements[row][column] = element;
      adoptElement(element);
      return true;
    } else
      qDebug() << Q_FUNC_INFO << "There is already an element in the specified row/column:" << row << column;
  } else
    qDebug() << Q_FUNC_INFO << "Can't add null element to row/column:" << row << column;
  return false;
}

/*!
  Returns whether the cell at \a row and \a column exists and contains a valid element, i.e. isn't
  empty.
  
  \see element
*/
bool QCPLayoutGrid::hasElement(int row, int column)
{
  if (row >= 0 && row < rowCount() && column >= 0 && column < columnCount())
    return mElements.at(row).at(column);
  else
    return false;
}

/*!
  Sets the stretch \a factor of \a column.
  
  Stretch factors control the relative sizes of rows and columns. Cells will not be resized beyond
  their minimum and maximum widths/heights (\ref QCPLayoutElement::setMinimumSize, \ref
  QCPLayoutElement::setMaximumSize), regardless of the stretch factor.
  
  The default stretch factor of newly created rows/columns is 1.
  
  \see setColumnStretchFactors, setRowStretchFactor
*/
void QCPLayoutGrid::setColumnStretchFactor(int column, double factor)
{
  if (column >= 0 && column < columnCount())
  {
    if (factor > 0)
      mColumnStretchFactors[column] = factor;
    else
      qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << factor;
  } else
    qDebug() << Q_FUNC_INFO << "Invalid column:" << column;
}

/*!
  Sets the stretch \a factors of all columns. \a factors must have the size \ref columnCount.
  
  Stretch factors control the relative sizes of rows and columns. Cells will not be resized beyond
  their minimum and maximum widths/heights (\ref QCPLayoutElement::setMinimumSize, \ref
  QCPLayoutElement::setMaximumSize), regardless of the stretch factor.
  
  The default stretch factor of newly created rows/columns is 1.
  
  \see setColumnStretchFactor, setRowStretchFactors
*/
void QCPLayoutGrid::setColumnStretchFactors(const QList<double> &factors)
{
  if (factors.size() == mColumnStretchFactors.size())
  {
    mColumnStretchFactors = factors;
    for (int i=0; i<mColumnStretchFactors.size(); ++i)
    {
      if (mColumnStretchFactors.at(i) <= 0)
      {
        qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << mColumnStretchFactors.at(i);
        mColumnStretchFactors[i] = 1;
      }
    }
  } else
    qDebug() << Q_FUNC_INFO << "Column count not equal to passed stretch factor count:" << factors;
}

/*!
  Sets the stretch \a factor of \a row.
  
  Stretch factors control the relative sizes of rows and columns. Cells will not be resized beyond
  their minimum and maximum widths/heights (\ref QCPLayoutElement::setMinimumSize, \ref
  QCPLayoutElement::setMaximumSize), regardless of the stretch factor.
  
  The default stretch factor of newly created rows/columns is 1.
  
  \see setColumnStretchFactors, setRowStretchFactor
*/
void QCPLayoutGrid::setRowStretchFactor(int row, double factor)
{
  if (row >= 0 && row < rowCount())
  {
    if (factor > 0)
      mRowStretchFactors[row] = factor;
    else
      qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << factor;
  } else
    qDebug() << Q_FUNC_INFO << "Invalid row:" << row;
}

/*!
  Sets the stretch \a factors of all rows. \a factors must have the size \ref rowCount.
  
  Stretch factors control the relative sizes of rows and columns. Cells will not be resized beyond
  their minimum and maximum widths/heights (\ref QCPLayoutElement::setMinimumSize, \ref
  QCPLayoutElement::setMaximumSize), regardless of the stretch factor.
  
  The default stretch factor of newly created rows/columns is 1.
  
  \see setRowStretchFactor, setColumnStretchFactors
*/
void QCPLayoutGrid::setRowStretchFactors(const QList<double> &factors)
{
  if (factors.size() == mRowStretchFactors.size())
  {
    mRowStretchFactors = factors;
    for (int i=0; i<mRowStretchFactors.size(); ++i)
    {
      if (mRowStretchFactors.at(i) <= 0)
      {
        qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << mRowStretchFactors.at(i);
        mRowStretchFactors[i] = 1;
      }
    }
  } else
    qDebug() << Q_FUNC_INFO << "Row count not equal to passed stretch factor count:" << factors;
}

/*!
  Sets the gap that is left blank between columns to \a pixels.
  
  \see setRowSpacing
*/
void QCPLayoutGrid::setColumnSpacing(int pixels)
{
  mColumnSpacing = pixels;
}

/*!
  Sets the gap that is left blank between rows to \a pixels.
  
  \see setColumnSpacing
*/
void QCPLayoutGrid::setRowSpacing(int pixels)
{
  mRowSpacing = pixels;
}

/*!
  Expands the layout to have \a newRowCount rows and \a newColumnCount columns. So the last valid
  row index will be \a newRowCount-1, the last valid column index will be \a newColumnCount-1.
  
  If the current column/row count is already larger or equal to \a newColumnCount/\a newRowCount,
  this function does nothing in that dimension.
  
  Newly created cells are empty, new rows and columns have the stretch factor 1.
  
  Note that upon a call to \ref addElement, the layout is expanded automatically to contain the
  specified row and column, using this function.
  
  \see simplify
*/
void QCPLayoutGrid::expandTo(int newRowCount, int newColumnCount)
{
  // add rows as necessary:
  while (rowCount() < newRowCount)
  {
    mElements.append(QList<QCPLayoutElement*>());
    mRowStretchFactors.append(1);
  }
  // go through rows and expand columns as necessary:
  int newColCount = qMax(columnCount(), newColumnCount);
  for (int i=0; i<rowCount(); ++i)
  {
    while (mElements.at(i).size() < newColCount)
      mElements[i].append(0);
  }
  while (mColumnStretchFactors.size() < newColCount)
    mColumnStretchFactors.append(1);
}

/*!
  Inserts a new row with empty cells at the row index \a newIndex. Valid values for \a newIndex
  range from 0 (inserts a row at the top) to \a rowCount (appends a row at the bottom).
  
  \see insertColumn
*/
void QCPLayoutGrid::insertRow(int newIndex)
{
  if (mElements.isEmpty() || mElements.first().isEmpty()) // if grid is completely empty, add first cell
  {
    expandTo(1, 1);
    return;
  }
  
  if (newIndex < 0)
    newIndex = 0;
  if (newIndex > rowCount())
    newIndex = rowCount();
  
  mRowStretchFactors.insert(newIndex, 1);
  QList<QCPLayoutElement*> newRow;
  for (int col=0; col<columnCount(); ++col)
    newRow.append((QCPLayoutElement*)0);
  mElements.insert(newIndex, newRow);
}

/*!
  Inserts a new column with empty cells at the column index \a newIndex. Valid values for \a
  newIndex range from 0 (inserts a row at the left) to \a rowCount (appends a row at the right).
  
  \see insertRow
*/
void QCPLayoutGrid::insertColumn(int newIndex)
{
  if (mElements.isEmpty() || mElements.first().isEmpty()) // if grid is completely empty, add first cell
  {
    expandTo(1, 1);
    return;
  }
  
  if (newIndex < 0)
    newIndex = 0;
  if (newIndex > columnCount())
    newIndex = columnCount();
  
  mColumnStretchFactors.insert(newIndex, 1);
  for (int row=0; row<rowCount(); ++row)
    mElements[row].insert(newIndex, (QCPLayoutElement*)0);
}

/* inherits documentation from base class */
void QCPLayoutGrid::updateLayout()
{
  QVector<int> minColWidths, minRowHeights, maxColWidths, maxRowHeights;
  getMinimumRowColSizes(&minColWidths, &minRowHeights);
  getMaximumRowColSizes(&maxColWidths, &maxRowHeights);
  
  int totalRowSpacing = (rowCount()-1) * mRowSpacing;
  int totalColSpacing = (columnCount()-1) * mColumnSpacing;
  QVector<int> colWidths = getSectionSizes(maxColWidths, minColWidths, mColumnStretchFactors.toVector(), mRect.width()-totalColSpacing);
  QVector<int> rowHeights = getSectionSizes(maxRowHeights, minRowHeights, mRowStretchFactors.toVector(), mRect.height()-totalRowSpacing);
  
  // go through cells and set rects accordingly:
  int yOffset = mRect.top();
  for (int row=0; row<rowCount(); ++row)
  {
    if (row > 0)
      yOffset += rowHeights.at(row-1)+mRowSpacing;
    int xOffset = mRect.left();
    for (int col=0; col<columnCount(); ++col)
    {
      if (col > 0)
        xOffset += colWidths.at(col-1)+mColumnSpacing;
      if (mElements.at(row).at(col))
        mElements.at(row).at(col)->setOuterRect(QRect(xOffset, yOffset, colWidths.at(col), rowHeights.at(row)));
    }
  }
}

/* inherits documentation from base class */
int QCPLayoutGrid::elementCount() const
{
  return rowCount()*columnCount();
}

/* inherits documentation from base class */
QCPLayoutElement *QCPLayoutGrid::elementAt(int index) const
{
  if (index >= 0 && index < elementCount())
    return mElements.at(index / columnCount()).at(index % columnCount());
  else
    return 0;
}

/* inherits documentation from base class */
QCPLayoutElement *QCPLayoutGrid::takeAt(int index)
{
  if (QCPLayoutElement *el = elementAt(index))
  {
    releaseElement(el);
    mElements[index / columnCount()][index % columnCount()] = 0;
    return el;
  } else
  {
    qDebug() << Q_FUNC_INFO << "Attempt to take invalid index:" << index;
    return 0;
  }
}

/* inherits documentation from base class */
bool QCPLayoutGrid::take(QCPLayoutElement *element)
{
  if (element)
  {
    for (int i=0; i<elementCount(); ++i)
    {
      if (elementAt(i) == element)
      {
        takeAt(i);
        return true;
      }
    }
    qDebug() << Q_FUNC_INFO << "Element not in this layout, couldn't take";
  } else
    qDebug() << Q_FUNC_INFO << "Can't take null element";
  return false;
}

/* inherits documentation from base class */
QList<QCPLayoutElement*> QCPLayoutGrid::elements(bool recursive) const
{
  QList<QCPLayoutElement*> result;
  int colC = columnCount();
  int rowC = rowCount();
#if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
  result.reserve(colC*rowC);
#endif
  for (int row=0; row<rowC; ++row)
  {
    for (int col=0; col<colC; ++col)
    {
      result.append(mElements.at(row).at(col));
    }
  }
  if (recursive)
  {
    int c = result.size();
    for (int i=0; i<c; ++i)
    {
      if (result.at(i))
        result << result.at(i)->elements(recursive);
    }
  }
  return result;
}

/*!
  Simplifies the layout by collapsing rows and columns which only contain empty cells.
*/
void QCPLayoutGrid::simplify()
{
  // remove rows with only empty cells:
  for (int row=rowCount()-1; row>=0; --row)
  {
    bool hasElements = false;
    for (int col=0; col<columnCount(); ++col)
    {
      if (mElements.at(row).at(col))
      {
        hasElements = true;
        break;
      }
    }
    if (!hasElements)
    {
      mRowStretchFactors.removeAt(row);
      mElements.removeAt(row);
      if (mElements.isEmpty()) // removed last element, also remove stretch factor (wouldn't happen below because also columnCount changed to 0 now)
        mColumnStretchFactors.clear();
    }
  }
  
  // remove columns with only empty cells:
  for (int col=columnCount()-1; col>=0; --col)
  {
    bool hasElements = false;
    for (int row=0; row<rowCount(); ++row)
    {
      if (mElements.at(row).at(col))
      {
        hasElements = true;
        break;
      }
    }
    if (!hasElements)
    {
      mColumnStretchFactors.removeAt(col);
      for (int row=0; row<rowCount(); ++row)
        mElements[row].removeAt(col);
    }
  }
}

/* inherits documentation from base class */
QSize QCPLayoutGrid::minimumSizeHint() const
{
  QVector<int> minColWidths, minRowHeights;
  getMinimumRowColSizes(&minColWidths, &minRowHeights);
  QSize result(0, 0);
  for (int i=0; i<minColWidths.size(); ++i)
    result.rwidth() += minColWidths.at(i);
  for (int i=0; i<minRowHeights.size(); ++i)
    result.rheight() += minRowHeights.at(i);
  result.rwidth() += qMax(0, columnCount()-1) * mColumnSpacing + mMargins.left() + mMargins.right();
  result.rheight() += qMax(0, rowCount()-1) * mRowSpacing + mMargins.top() + mMargins.bottom();
  return result;
}

/* inherits documentation from base class */
QSize QCPLayoutGrid::maximumSizeHint() const
{
  QVector<int> maxColWidths, maxRowHeights;
  getMaximumRowColSizes(&maxColWidths, &maxRowHeights);
  
  QSize result(0, 0);
  for (int i=0; i<maxColWidths.size(); ++i)
    result.setWidth(qMin(result.width()+maxColWidths.at(i), QWIDGETSIZE_MAX));
  for (int i=0; i<maxRowHeights.size(); ++i)
    result.setHeight(qMin(result.height()+maxRowHeights.at(i), QWIDGETSIZE_MAX));
  result.rwidth() += qMax(0, columnCount()-1) * mColumnSpacing + mMargins.left() + mMargins.right();
  result.rheight() += qMax(0, rowCount()-1) * mRowSpacing + mMargins.top() + mMargins.bottom();
  return result;
}

/*! \internal
  
  Places the minimum column widths and row heights into \a minColWidths and \a minRowHeights
  respectively.
  
  The minimum height of a row is the largest minimum height of any element in that row. The minimum
  width of a column is the largest minimum width of any element in that column.
  
  This is a helper function for \ref updateLayout.
  
  \see getMaximumRowColSizes
*/
void QCPLayoutGrid::getMinimumRowColSizes(QVector<int> *minColWidths, QVector<int> *minRowHeights) const
{
  *minColWidths = QVector<int>(columnCount(), 0);
  *minRowHeights = QVector<int>(rowCount(), 0);
  for (int row=0; row<rowCount(); ++row)
  {
    for (int col=0; col<columnCount(); ++col)
    {
      if (mElements.at(row).at(col))
      {
        QSize minHint = mElements.at(row).at(col)->minimumSizeHint();
        QSize min = mElements.at(row).at(col)->minimumSize();
        QSize final(min.width() > 0 ? min.width() : minHint.width(), min.height() > 0 ? min.height() : minHint.height());
        if (minColWidths->at(col) < final.width())
          (*minColWidths)[col] = final.width();
        if (minRowHeights->at(row) < final.height())
          (*minRowHeights)[row] = final.height();
      }
    }
  }
}

/*! \internal
  
  Places the maximum column widths and row heights into \a maxColWidths and \a maxRowHeights
  respectively.
  
  The maximum height of a row is the smallest maximum height of any element in that row. The
  maximum width of a column is the smallest maximum width of any element in that column.
  
  This is a helper function for \ref updateLayout.
  
  \see getMinimumRowColSizes
*/
void QCPLayoutGrid::getMaximumRowColSizes(QVector<int> *maxColWidths, QVector<int> *maxRowHeights) const
{
  *maxColWidths = QVector<int>(columnCount(), QWIDGETSIZE_MAX);
  *maxRowHeights = QVector<int>(rowCount(), QWIDGETSIZE_MAX);
  for (int row=0; row<rowCount(); ++row)
  {
    for (int col=0; col<columnCount(); ++col)
    {
      if (mElements.at(row).at(col))
      {
        QSize maxHint = mElements.at(row).at(col)->maximumSizeHint();
        QSize max = mElements.at(row).at(col)->maximumSize();
        QSize final(max.width() < QWIDGETSIZE_MAX ? max.width() : maxHint.width(), max.height() < QWIDGETSIZE_MAX ? max.height() : maxHint.height());
        if (maxColWidths->at(col) > final.width())
          (*maxColWidths)[col] = final.width();
        if (maxRowHeights->at(row) > final.height())
          (*maxRowHeights)[row] = final.height();
      }
    }
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayoutInset
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \class QCPLayoutInset
  \brief A layout that places child elements aligned to the border or arbitrarily positioned
  
  Elements are placed either aligned to the border or at arbitrary position in the area of the
  layout. Which placement applies is controlled with the \ref InsetPlacement (\ref
  setInsetPlacement).

  Elements are added via \ref addElement(QCPLayoutElement *element, Qt::Alignment alignment) or
  addElement(QCPLayoutElement *element, const QRectF &rect). If the first method is used, the inset
  placement will default to \ref ipBorderAligned and the element will be aligned according to the
  \a alignment parameter. The second method defaults to \ref ipFree and allows placing elements at
  arbitrary position and size, defined by \a rect.
  
  The alignment or rect can be set via \ref setInsetAlignment or \ref setInsetRect, respectively.
  
  This is the layout that every QCPAxisRect has as \ref QCPAxisRect::insetLayout.
*/

/* start documentation of inline functions */

/*! \fn virtual void QCPLayoutInset::simplify()
  
  The QCPInsetLayout does not need simplification since it can never have empty cells due to its
  linear index structure. This method does nothing.
*/

/* end documentation of inline functions */

/*!
  Creates an instance of QCPLayoutInset and sets default values.
*/
QCPLayoutInset::QCPLayoutInset()
{
}

QCPLayoutInset::~QCPLayoutInset()
{
  // clear all child layout elements. This is important because only the specific layouts know how
  // to handle removing elements (clear calls virtual removeAt method to do that).
  clear();
}

/*!
  Returns the placement type of the element with the specified \a index.
*/
QCPLayoutInset::InsetPlacement QCPLayoutInset::insetPlacement(int index) const
{
  if (elementAt(index))
    return mInsetPlacement.at(index);
  else
  {
    qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
    return ipFree;
  }
}

/*!
  Returns the alignment of the element with the specified \a index. The alignment only has a
  meaning, if the inset placement (\ref setInsetPlacement) is \ref ipBorderAligned.
*/
Qt::Alignment QCPLayoutInset::insetAlignment(int index) const
{
  if (elementAt(index))
    return mInsetAlignment.at(index);
  else
  {
    qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
    return 0;
  }
}

/*!
  Returns the rect of the element with the specified \a index. The rect only has a
  meaning, if the inset placement (\ref setInsetPlacement) is \ref ipFree.
*/
QRectF QCPLayoutInset::insetRect(int index) const
{
  if (elementAt(index))
    return mInsetRect.at(index);
  else
  {
    qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
    return QRectF();
  }
}

/*!
  Sets the inset placement type of the element with the specified \a index to \a placement.
  
  \see InsetPlacement
*/
void QCPLayoutInset::setInsetPlacement(int index, QCPLayoutInset::InsetPlacement placement)
{
  if (elementAt(index))
    mInsetPlacement[index] = placement;
  else
    qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
}

/*!
  If the inset placement (\ref setInsetPlacement) is \ref ipBorderAligned, this function
  is used to set the alignment of the element with the specified \a index to \a alignment.
  
  \a alignment is an or combination of the following alignment flags: Qt::AlignLeft,
  Qt::AlignHCenter, Qt::AlighRight, Qt::AlignTop, Qt::AlignVCenter, Qt::AlignBottom. Any other
  alignment flags will be ignored.
*/
void QCPLayoutInset::setInsetAlignment(int index, Qt::Alignment alignment)
{
  if (elementAt(index))
    mInsetAlignment[index] = alignment;
  else
    qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
}

/*!
  If the inset placement (\ref setInsetPlacement) is \ref ipFree, this function is used to set the
  position and size of the element with the specified \a index to \a rect.
  
  \a rect is given in fractions of the whole inset layout rect. So an inset with rect (0, 0, 1, 1)
  will span the entire layout. An inset with rect (0.6, 0.1, 0.35, 0.35) will be in the top right
  corner of the layout, with 35% width and height of the parent layout.
  
  Note that the minimum and maximum sizes of the embedded element (\ref
  QCPLayoutElement::setMinimumSize, \ref QCPLayoutElement::setMaximumSize) are enforced.
*/
void QCPLayoutInset::setInsetRect(int index, const QRectF &rect)
{
  if (elementAt(index))
    mInsetRect[index] = rect;
  else
    qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
}

/* inherits documentation from base class */
void QCPLayoutInset::updateLayout()
{
  for (int i=0; i<mElements.size(); ++i)
  {
    QRect insetRect;
    QSize finalMinSize, finalMaxSize;
    QSize minSizeHint = mElements.at(i)->minimumSizeHint();
    QSize maxSizeHint = mElements.at(i)->maximumSizeHint();
    finalMinSize.setWidth(mElements.at(i)->minimumSize().width() > 0 ? mElements.at(i)->minimumSize().width() : minSizeHint.width());
    finalMinSize.setHeight(mElements.at(i)->minimumSize().height() > 0 ? mElements.at(i)->minimumSize().height() : minSizeHint.height());
    finalMaxSize.setWidth(mElements.at(i)->maximumSize().width() < QWIDGETSIZE_MAX ? mElements.at(i)->maximumSize().width() : maxSizeHint.width());
    finalMaxSize.setHeight(mElements.at(i)->maximumSize().height() < QWIDGETSIZE_MAX ? mElements.at(i)->maximumSize().height() : maxSizeHint.height());
    if (mInsetPlacement.at(i) == ipFree)
    {
      insetRect = QRect(rect().x()+rect().width()*mInsetRect.at(i).x(),
                        rect().y()+rect().height()*mInsetRect.at(i).y(),
                        rect().width()*mInsetRect.at(i).width(),
                        rect().height()*mInsetRect.at(i).height());
      if (insetRect.size().width() < finalMinSize.width())
        insetRect.setWidth(finalMinSize.width());
      if (insetRect.size().height() < finalMinSize.height())
        insetRect.setHeight(finalMinSize.height());
      if (insetRect.size().width() > finalMaxSize.width())
        insetRect.setWidth(finalMaxSize.width());
      if (insetRect.size().height() > finalMaxSize.height())
        insetRect.setHeight(finalMaxSize.height());
    } else if (mInsetPlacement.at(i) == ipBorderAligned)
    {
      insetRect.setSize(finalMinSize);
      Qt::Alignment al = mInsetAlignment.at(i);
      if (al.testFlag(Qt::AlignLeft)) insetRect.moveLeft(rect().x());
      else if (al.testFlag(Qt::AlignRight)) insetRect.moveRight(rect().x()+rect().width());
      else insetRect.moveLeft(rect().x()+rect().width()*0.5-finalMinSize.width()*0.5); // default to Qt::AlignHCenter
      if (al.testFlag(Qt::AlignTop)) insetRect.moveTop(rect().y());
      else if (al.testFlag(Qt::AlignBottom)) insetRect.moveBottom(rect().y()+rect().height());
      else insetRect.moveTop(rect().y()+rect().height()*0.5-finalMinSize.height()*0.5); // default to Qt::AlignVCenter
    }
    mElements.at(i)->setOuterRect(insetRect);
  }
}

/* inherits documentation from base class */
int QCPLayoutInset::elementCount() const
{
  return mElements.size();
}

/* inherits documentation from base class */
QCPLayoutElement *QCPLayoutInset::elementAt(int index) const
{
  if (index >= 0 && index < mElements.size())
    return mElements.at(index);
  else
    return 0;
}

/* inherits documentation from base class */
QCPLayoutElement *QCPLayoutInset::takeAt(int index)
{
  if (QCPLayoutElement *el = elementAt(index))
  {
    releaseElement(el);
    mElements.removeAt(index);
    mInsetPlacement.removeAt(index);
    mInsetAlignment.removeAt(index);
    mInsetRect.removeAt(index);
    return el;
  } else
  {
    qDebug() << Q_FUNC_INFO << "Attempt to take invalid index:" << index;
    return 0;
  }
}

/* inherits documentation from base class */
bool QCPLayoutInset::take(QCPLayoutElement *element)
{
  if (element)
  {
    for (int i=0; i<elementCount(); ++i)
    {
      if (elementAt(i) == element)
      {
        takeAt(i);
        return true;
      }
    }
    qDebug() << Q_FUNC_INFO << "Element not in this layout, couldn't take";
  } else
    qDebug() << Q_FUNC_INFO << "Can't take null element";
  return false;
}

/*!
  The inset layout is sensitive to events only at areas where its (visible) child elements are
  sensitive. If the selectTest method of any of the child elements returns a positive number for \a
  pos, this method returns a value corresponding to 0.99 times the parent plot's selection
  tolerance. The inset layout is not selectable itself by default. So if \a onlySelectable is true,
  -1.0 is returned.
  
  See \ref QCPLayerable::selectTest for a general explanation of this virtual method.
*/
double QCPLayoutInset::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable)
    return -1;
  
  for (int i=0; i<mElements.size(); ++i)
  {
    // inset layout shall only return positive selectTest, if actually an inset object is at pos
    // else it would block the entire underlying QCPAxisRect with its surface.
    if (mElements.at(i)->realVisibility() && mElements.at(i)->selectTest(pos, onlySelectable) >= 0)
      return mParentPlot->selectionTolerance()*0.99;
  }
  return -1;
}

/*!
  Adds the specified \a element to the layout as an inset aligned at the border (\ref
  setInsetAlignment is initialized with \ref ipBorderAligned). The alignment is set to \a
  alignment.
  
  \a alignment is an or combination of the following alignment flags: Qt::AlignLeft,
  Qt::AlignHCenter, Qt::AlighRight, Qt::AlignTop, Qt::AlignVCenter, Qt::AlignBottom. Any other
  alignment flags will be ignored.
  
  \see addElement(QCPLayoutElement *element, const QRectF &rect)
*/
void QCPLayoutInset::addElement(QCPLayoutElement *element, Qt::Alignment alignment)
{
  if (element)
  {
    if (element->layout()) // remove from old layout first
      element->layout()->take(element);
    mElements.append(element);
    mInsetPlacement.append(ipBorderAligned);
    mInsetAlignment.append(alignment);
    mInsetRect.append(QRectF(0.6, 0.6, 0.4, 0.4));
    adoptElement(element);
  } else
    qDebug() << Q_FUNC_INFO << "Can't add null element";
}

/*!
  Adds the specified \a element to the layout as an inset with free positioning/sizing (\ref
  setInsetAlignment is initialized with \ref ipFree). The position and size is set to \a
  rect.
  
  \a rect is given in fractions of the whole inset layout rect. So an inset with rect (0, 0, 1, 1)
  will span the entire layout. An inset with rect (0.6, 0.1, 0.35, 0.35) will be in the top right
  corner of the layout, with 35% width and height of the parent layout.
  
  \see addElement(QCPLayoutElement *element, Qt::Alignment alignment)
*/
void QCPLayoutInset::addElement(QCPLayoutElement *element, const QRectF &rect)
{
  if (element)
  {
    if (element->layout()) // remove from old layout first
      element->layout()->take(element);
    mElements.append(element);
    mInsetPlacement.append(ipFree);
    mInsetAlignment.append(Qt::AlignRight|Qt::AlignTop);
    mInsetRect.append(rect);
    adoptElement(element);
  } else
    qDebug() << Q_FUNC_INFO << "Can't add null element";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLineEnding
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLineEnding
  \brief Handles the different ending decorations for line-like items
  
  \image html QCPLineEnding.png "The various ending styles currently supported"
  
  For every ending a line-like item has, an instance of this class exists. For example, QCPItemLine
  has two endings which can be set with QCPItemLine::setHead and QCPItemLine::setTail.
 
  The styles themselves are defined via the enum QCPLineEnding::EndingStyle. Most decorations can
  be modified regarding width and length, see \ref setWidth and \ref setLength. The direction of
  the ending decoration (e.g. direction an arrow is pointing) is controlled by the line-like item.
  For example, when both endings of a QCPItemLine are set to be arrows, they will point to opposite
  directions, e.g. "outward". This can be changed by \ref setInverted, which would make the
  respective arrow point inward.
  
  Note that due to the overloaded QCPLineEnding constructor, you may directly specify a
  QCPLineEnding::EndingStyle where actually a QCPLineEnding is expected, e.g. \code
  myItemLine->setHead(QCPLineEnding::esSpikeArrow) \endcode
*/

/*!
  Creates a QCPLineEnding instance with default values (style \ref esNone).
*/
QCPLineEnding::QCPLineEnding() :
  mStyle(esNone),
  mWidth(8),
  mLength(10),
  mInverted(false)
{
}

/*!
  Creates a QCPLineEnding instance with the specified values.
*/
QCPLineEnding::QCPLineEnding(QCPLineEnding::EndingStyle style, double width, double length, bool inverted) :
  mStyle(style),
  mWidth(width),
  mLength(length),
  mInverted(inverted)
{
}

/*!
  Sets the style of the ending decoration.
*/
void QCPLineEnding::setStyle(QCPLineEnding::EndingStyle style)
{
  mStyle = style;
}

/*!
  Sets the width of the ending decoration, if the style supports it. On arrows, for example, the
  width defines the size perpendicular to the arrow's pointing direction.
  
  \see setLength
*/
void QCPLineEnding::setWidth(double width)
{
  mWidth = width;
}

/*!
  Sets the length of the ending decoration, if the style supports it. On arrows, for example, the
  length defines the size in pointing direction.
  
  \see setWidth
*/
void QCPLineEnding::setLength(double length)
{
  mLength = length;
}

/*!
  Sets whether the ending decoration shall be inverted. For example, an arrow decoration will point
  inward when \a inverted is set to true.

  Note that also the \a width direction is inverted. For symmetrical ending styles like arrows or
  discs, this doesn't make a difference. However, asymmetric styles like \ref esHalfBar are
  affected by it, which can be used to control to which side the half bar points to.
*/
void QCPLineEnding::setInverted(bool inverted)
{
  mInverted = inverted;
}

/*! \internal
  
  Returns the maximum pixel radius the ending decoration might cover, starting from the position
  the decoration is drawn at (typically a line ending/\ref QCPItemPosition of an item).
  
  This is relevant for clipping. Only omit painting of the decoration when the position where the
  decoration is supposed to be drawn is farther away from the clipping rect than the returned
  distance.
*/
double QCPLineEnding::boundingDistance() const
{
  switch (mStyle)
  {
    case esNone:
      return 0;
      
    case esFlatArrow:
    case esSpikeArrow:
    case esLineArrow:
    case esSkewedBar:
      return qSqrt(mWidth*mWidth+mLength*mLength); // items that have width and length
      
    case esDisc:
    case esSquare:
    case esDiamond:
    case esBar:
    case esHalfBar:
      return mWidth*1.42; // items that only have a width -> width*sqrt(2)

  }
  return 0;
}

/*!
  Starting from the origin of this line ending (which is style specific), returns the length
  covered by the line ending symbol, in backward direction.
  
  For example, the \ref esSpikeArrow has a shorter real length than a \ref esFlatArrow, even if
  both have the same \ref setLength value, because the spike arrow has an inward curved back, which
  reduces the length along its center axis (the drawing origin for arrows is at the tip).
  
  This function is used for precise, style specific placement of line endings, for example in
  QCPAxes.
*/
double QCPLineEnding::realLength() const
{
  switch (mStyle)
  {
    case esNone:
    case esLineArrow:
    case esSkewedBar:
    case esBar:
    case esHalfBar:
      return 0;
      
    case esFlatArrow:
      return mLength;
      
    case esDisc:
    case esSquare:
    case esDiamond:
      return mWidth*0.5;
      
    case esSpikeArrow:
      return mLength*0.8;
  }
  return 0;
}

/*! \internal
  
  Draws the line ending with the specified \a painter at the position \a pos. The direction of the
  line ending is controlled with \a dir.
*/
void QCPLineEnding::draw(QCPPainter *painter, const QVector2D &pos, const QVector2D &dir) const
{
  if (mStyle == esNone)
    return;
  
  QVector2D lengthVec(dir.normalized());
  if (lengthVec.isNull())
    lengthVec = QVector2D(1, 0);
  QVector2D widthVec(-lengthVec.y(), lengthVec.x());
  lengthVec *= (float)(mLength*(mInverted ? -1 : 1));
  widthVec *= (float)(mWidth*0.5*(mInverted ? -1 : 1));
  
  QPen penBackup = painter->pen();
  QBrush brushBackup = painter->brush();
  QPen miterPen = penBackup;
  miterPen.setJoinStyle(Qt::MiterJoin); // to make arrow heads spikey
  QBrush brush(painter->pen().color(), Qt::SolidPattern);
  switch (mStyle)
  {
    case esNone: break;
    case esFlatArrow:
    {
      QPointF points[3] = {pos.toPointF(),
                           (pos-lengthVec+widthVec).toPointF(),
                           (pos-lengthVec-widthVec).toPointF()
                          };
      painter->setPen(miterPen);
      painter->setBrush(brush);
      painter->drawConvexPolygon(points, 3);
      painter->setBrush(brushBackup);
      painter->setPen(penBackup);
      break;
    }
    case esSpikeArrow:
    {
      QPointF points[4] = {pos.toPointF(),
                           (pos-lengthVec+widthVec).toPointF(),
                           (pos-lengthVec*0.8f).toPointF(),
                           (pos-lengthVec-widthVec).toPointF()
                          };
      painter->setPen(miterPen);
      painter->setBrush(brush);
      painter->drawConvexPolygon(points, 4);
      painter->setBrush(brushBackup);
      painter->setPen(penBackup);
      break;
    }
    case esLineArrow:
    {
      QPointF points[3] = {(pos-lengthVec+widthVec).toPointF(),
                           pos.toPointF(),
                           (pos-lengthVec-widthVec).toPointF()
                          };
      painter->setPen(miterPen);
      painter->drawPolyline(points, 3);
      painter->setPen(penBackup);
      break;
    }
    case esDisc:
    {
      painter->setBrush(brush);
      painter->drawEllipse(pos.toPointF(),  mWidth*0.5, mWidth*0.5);
      painter->setBrush(brushBackup);
      break;
    }
    case esSquare:
    {
      QVector2D widthVecPerp(-widthVec.y(), widthVec.x());
      QPointF points[4] = {(pos-widthVecPerp+widthVec).toPointF(),
                           (pos-widthVecPerp-widthVec).toPointF(),
                           (pos+widthVecPerp-widthVec).toPointF(),
                           (pos+widthVecPerp+widthVec).toPointF()
                          };
      painter->setPen(miterPen);
      painter->setBrush(brush);
      painter->drawConvexPolygon(points, 4);
      painter->setBrush(brushBackup);
      painter->setPen(penBackup);
      break;
    }
    case esDiamond:
    {
      QVector2D widthVecPerp(-widthVec.y(), widthVec.x());
      QPointF points[4] = {(pos-widthVecPerp).toPointF(),
                           (pos-widthVec).toPointF(),
                           (pos+widthVecPerp).toPointF(),
                           (pos+widthVec).toPointF()
                          };
      painter->setPen(miterPen);
      painter->setBrush(brush);
      painter->drawConvexPolygon(points, 4);
      painter->setBrush(brushBackup);
      painter->setPen(penBackup);
      break;
    }
    case esBar:
    {
      painter->drawLine((pos+widthVec).toPointF(), (pos-widthVec).toPointF());
      break;
    }
    case esHalfBar:
    {
      painter->drawLine((pos+widthVec).toPointF(), pos.toPointF());
      break;
    }
    case esSkewedBar:
    {
      if (qFuzzyIsNull(painter->pen().widthF()) && !painter->modes().testFlag(QCPPainter::pmNonCosmetic))
      {
        // if drawing with cosmetic pen (perfectly thin stroke, happens only in vector exports), draw bar exactly on tip of line
        painter->drawLine((pos+widthVec+lengthVec*0.2f*(mInverted?-1:1)).toPointF(),
                          (pos-widthVec-lengthVec*0.2f*(mInverted?-1:1)).toPointF());
      } else
      {
        // if drawing with thick (non-cosmetic) pen, shift bar a little in line direction to prevent line from sticking through bar slightly
        painter->drawLine((pos+widthVec+lengthVec*0.2f*(mInverted?-1:1)+dir.normalized()*qMax(1.0f, (float)painter->pen().widthF())*0.5f).toPointF(),
                          (pos-widthVec-lengthVec*0.2f*(mInverted?-1:1)+dir.normalized()*qMax(1.0f, (float)painter->pen().widthF())*0.5f).toPointF());
      }
      break;
    }
  }
}

/*! \internal
  \overload
  
  Draws the line ending. The direction is controlled with the \a angle parameter in radians.
*/
void QCPLineEnding::draw(QCPPainter *painter, const QVector2D &pos, double angle) const
{
  draw(painter, pos, QVector2D(qCos(angle), qSin(angle)));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPGrid
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPGrid
  \brief Responsible for drawing the grid of a QCPAxis.
  
  This class is tightly bound to QCPAxis. Every axis owns a grid instance and uses it to draw the
  grid lines, sub grid lines and zero-line. You can interact with the grid of an axis via \ref
  QCPAxis::grid. Normally, you don't need to create an instance of QCPGrid yourself.
  
  The axis and grid drawing was split into two classes to allow them to be placed on different
  layers (both QCPAxis and QCPGrid inherit from QCPLayerable). Thus it is possible to have the grid
  in the background and the axes in the foreground, and any plottables/items in between. This
  described situation is the default setup, see the QCPLayer documentation.
*/

/*!
  Creates a QCPGrid instance and sets default values.
  
  You shouldn't instantiate grids on their own, since every QCPAxis brings its own QCPGrid.
*/
QCPGrid::QCPGrid(QCPAxis *parentAxis) :
  QCPLayerable(parentAxis->parentPlot(), "", parentAxis),
  mParentAxis(parentAxis)
{
  // warning: this is called in QCPAxis constructor, so parentAxis members should not be accessed/called
  setParent(parentAxis);
  setPen(QPen(QColor(200,200,200), 0, Qt::DotLine));
  setSubGridPen(QPen(QColor(220,220,220), 0, Qt::DotLine));
  setZeroLinePen(QPen(QColor(200,200,200), 0, Qt::SolidLine));
  setSubGridVisible(false);
  setAntialiased(false);
  setAntialiasedSubGrid(false);
  setAntialiasedZeroLine(false);
}

/*!
  Sets whether grid lines at sub tick marks are drawn.
  
  \see setSubGridPen
*/
void QCPGrid::setSubGridVisible(bool visible)
{
  mSubGridVisible = visible;
}

/*!
  Sets whether sub grid lines are drawn antialiased.
*/
void QCPGrid::setAntialiasedSubGrid(bool enabled)
{
  mAntialiasedSubGrid = enabled;
}

/*!
  Sets whether zero lines are drawn antialiased.
*/
void QCPGrid::setAntialiasedZeroLine(bool enabled)
{
  mAntialiasedZeroLine = enabled;
}

/*!
  Sets the pen with which (major) grid lines are drawn.
*/
void QCPGrid::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  Sets the pen with which sub grid lines are drawn.
*/
void QCPGrid::setSubGridPen(const QPen &pen)
{
  mSubGridPen = pen;
}

/*!
  Sets the pen with which zero lines are drawn.
  
  Zero lines are lines at value coordinate 0 which may be drawn with a different pen than other grid
  lines. To disable zero lines and just draw normal grid lines at zero, set \a pen to Qt::NoPen.
*/
void QCPGrid::setZeroLinePen(const QPen &pen)
{
  mZeroLinePen = pen;
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing the major grid lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased
*/
void QCPGrid::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiased, QCP::aeGrid);
}

/*! \internal
  
  Draws grid lines and sub grid lines at the positions of (sub) ticks of the parent axis, spanning
  over the complete axis rect. Also draws the zero line, if appropriate (\ref setZeroLinePen).
*/
void QCPGrid::draw(QCPPainter *painter)
{
  if (!mParentAxis) { qDebug() << Q_FUNC_INFO << "invalid parent axis"; return; }
  
  if (mSubGridVisible)
    drawSubGridLines(painter);
  drawGridLines(painter);
}

/*! \internal
  
  Draws the main grid lines and possibly a zero line with the specified painter.
  
  This is a helper function called by \ref draw.
*/
void QCPGrid::drawGridLines(QCPPainter *painter) const
{
  if (!mParentAxis) { qDebug() << Q_FUNC_INFO << "invalid parent axis"; return; }
  
  int lowTick = mParentAxis->mLowestVisibleTick;
  int highTick = mParentAxis->mHighestVisibleTick;
  double t; // helper variable, result of coordinate-to-pixel transforms
  if (mParentAxis->orientation() == Qt::Horizontal)
  {
    // draw zeroline:
    int zeroLineIndex = -1;
    if (mZeroLinePen.style() != Qt::NoPen && mParentAxis->mRange.lower < 0 && mParentAxis->mRange.upper > 0)
    {
      applyAntialiasingHint(painter, mAntialiasedZeroLine, QCP::aeZeroLine);
      painter->setPen(mZeroLinePen);
      double epsilon = mParentAxis->range().size()*1E-6; // for comparing double to zero
      for (int i=lowTick; i <= highTick; ++i)
      {
        if (qAbs(mParentAxis->mTickVector.at(i)) < epsilon)
        {
          zeroLineIndex = i;
          t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // x
          painter->drawLine(QLineF(t, mParentAxis->mAxisRect->bottom(), t, mParentAxis->mAxisRect->top()));
          break;
        }
      }
    }
    // draw grid lines:
    applyDefaultAntialiasingHint(painter);
    painter->setPen(mPen);
    for (int i=lowTick; i <= highTick; ++i)
    {
      if (i == zeroLineIndex) continue; // don't draw a gridline on top of the zeroline
      t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // x
      painter->drawLine(QLineF(t, mParentAxis->mAxisRect->bottom(), t, mParentAxis->mAxisRect->top()));
    }
  } else
  {
    // draw zeroline:
    int zeroLineIndex = -1;
    if (mZeroLinePen.style() != Qt::NoPen && mParentAxis->mRange.lower < 0 && mParentAxis->mRange.upper > 0)
    {
      applyAntialiasingHint(painter, mAntialiasedZeroLine, QCP::aeZeroLine);
      painter->setPen(mZeroLinePen);
      double epsilon = mParentAxis->mRange.size()*1E-6; // for comparing double to zero
      for (int i=lowTick; i <= highTick; ++i)
      {
        if (qAbs(mParentAxis->mTickVector.at(i)) < epsilon)
        {
          zeroLineIndex = i;
          t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // y
          painter->drawLine(QLineF(mParentAxis->mAxisRect->left(), t, mParentAxis->mAxisRect->right(), t));
          break;
        }
      }
    }
    // draw grid lines:
    applyDefaultAntialiasingHint(painter);
    painter->setPen(mPen);
    for (int i=lowTick; i <= highTick; ++i)
    {
      if (i == zeroLineIndex) continue; // don't draw a gridline on top of the zeroline
      t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // y
      painter->drawLine(QLineF(mParentAxis->mAxisRect->left(), t, mParentAxis->mAxisRect->right(), t));
    }
  }
}

/*! \internal
  
  Draws the sub grid lines with the specified painter.
  
  This is a helper function called by \ref draw.
*/
void QCPGrid::drawSubGridLines(QCPPainter *painter) const
{
  if (!mParentAxis) { qDebug() << Q_FUNC_INFO << "invalid parent axis"; return; }
  
  applyAntialiasingHint(painter, mAntialiasedSubGrid, QCP::aeSubGrid);
  double t; // helper variable, result of coordinate-to-pixel transforms
  painter->setPen(mSubGridPen);
  if (mParentAxis->orientation() == Qt::Horizontal)
  {
    for (int i=0; i<mParentAxis->mSubTickVector.size(); ++i)
    {
      t = mParentAxis->coordToPixel(mParentAxis->mSubTickVector.at(i)); // x
      painter->drawLine(QLineF(t, mParentAxis->mAxisRect->bottom(), t, mParentAxis->mAxisRect->top()));
    }
  } else
  {
    for (int i=0; i<mParentAxis->mSubTickVector.size(); ++i)
    {
      t = mParentAxis->coordToPixel(mParentAxis->mSubTickVector.at(i)); // y
      painter->drawLine(QLineF(mParentAxis->mAxisRect->left(), t, mParentAxis->mAxisRect->right(), t));
    }
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxis
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAxis
  \brief Manages a single axis inside a QCustomPlot.

  Usually doesn't need to be instantiated externally. Access %QCustomPlot's default four axes via
  QCustomPlot::xAxis (bottom), QCustomPlot::yAxis (left), QCustomPlot::xAxis2 (top) and
  QCustomPlot::yAxis2 (right).
  
  Axes are always part of an axis rect, see QCPAxisRect.
  \image html AxisNamesOverview.png
  <center>Naming convention of axis parts</center>
  \n
    
  \image html AxisRectSpacingOverview.png
  <center>Overview of the spacings and paddings that define the geometry of an axis. The dashed gray line
  on the left represents the QCustomPlot widget border.</center>

*/

/* start of documentation of inline functions */

/*! \fn Qt::Orientation QCPAxis::orientation() const
  
  Returns the orientation of this axis. The axis orientation (horizontal or vertical) is deduced
  from the axis type (left, top, right or bottom).
  
  \see orientation(AxisType type)
*/

/*! \fn QCPGrid *QCPAxis::grid() const
  
  Returns the \ref QCPGrid instance belonging to this axis. Access it to set details about the way the
  grid is displayed.
*/

/*! \fn static Qt::Orientation QCPAxis::orientation(AxisType type)
  
  Returns the orientation of the specified axis type
  
  \see orientation()
*/

/* end of documentation of inline functions */
/* start of documentation of signals */

/*! \fn void QCPAxis::ticksRequest()
  
  This signal is emitted when \ref setAutoTicks is false and the axis is about to generate tick
  labels for a replot.
  
  Modifying the tick positions can be done with \ref setTickVector. If you also want to control the
  tick labels, set \ref setAutoTickLabels to false and also provide the labels with \ref
  setTickVectorLabels.
  
  If you only want static ticks you probably don't need this signal, since you can just set the
  tick vector (and possibly tick label vector) once. However, if you want to provide ticks (and
  maybe labels) dynamically, e.g. depending on the current axis range, connect a slot to this
  signal and set the vector/vectors there.
*/

/*! \fn void QCPAxis::rangeChanged(const QCPRange &newRange)

  This signal is emitted when the range of this axis has changed. You can connect it to the \ref
  setRange slot of another axis to communicate the new range to the other axis, in order for it to
  be synchronized.
*/

/*! \fn void QCPAxis::rangeChanged(const QCPRange &newRange, const QCPRange &oldRange)
  \overload
  
  Additionally to the new range, this signal also provides the previous range held by the axis as
  \a oldRange.
*/

/*! \fn void QCPAxis::scaleTypeChanged(QCPAxis::ScaleType scaleType);
  
  This signal is emitted when the scale type changes, by calls to \ref setScaleType
*/

/*! \fn void QCPAxis::selectionChanged(QCPAxis::SelectableParts selection)
  
  This signal is emitted when the selection state of this axis has changed, either by user interaction
  or by a direct call to \ref setSelectedParts.
*/

/*! \fn void QCPAxis::selectableChanged(const QCPAxis::SelectableParts &parts);
  
  This signal is emitted when the selectability changes, by calls to \ref setSelectableParts
*/

/* end of documentation of signals */

/*!
  Constructs an Axis instance of Type \a type for the axis rect \a parent.
  You shouldn't instantiate axes directly, rather use \ref QCPAxisRect::addAxis.
*/
QCPAxis::QCPAxis(QCPAxisRect *parent, AxisType type) :
  QCPLayerable(parent->parentPlot(), "", parent),
  // axis base:
  mAxisType(type),
  mAxisRect(parent),
  mPadding(5),
  mOrientation(orientation(type)),
  mSelectableParts(spAxis | spTickLabels | spAxisLabel),
  mSelectedParts(spNone),
  mBasePen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
  mSelectedBasePen(QPen(Qt::blue, 2)),
  // axis label:
  mLabel(""),
  mLabelFont(mParentPlot->font()),
  mSelectedLabelFont(QFont(mLabelFont.family(), mLabelFont.pointSize(), QFont::Bold)),
  mLabelColor(Qt::black),
  mSelectedLabelColor(Qt::blue),
  // tick labels:
  mTickLabels(true),
  mAutoTickLabels(true),
  mTickLabelType(ltNumber),
  mTickLabelFont(mParentPlot->font()),
  mSelectedTickLabelFont(QFont(mTickLabelFont.family(), mTickLabelFont.pointSize(), QFont::Bold)),
  mTickLabelColor(Qt::black),
  mSelectedTickLabelColor(Qt::blue),
  mDateTimeFormat("hh:mm:ss\ndd.MM.yy"),
  mDateTimeSpec(Qt::LocalTime),
  mNumberPrecision(6),
  mNumberFormatChar('g'),
  mNumberBeautifulPowers(true),
  // ticks and subticks:
  mTicks(true),
  mTickStep(1),
  mSubTickCount(4),
  mAutoTickCount(6),
  mAutoTicks(true),
  mAutoTickStep(true),
  mAutoSubTicks(true),
  mTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
  mSelectedTickPen(QPen(Qt::blue, 2)),
  mSubTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
  mSelectedSubTickPen(QPen(Qt::blue, 2)),
  // scale and range:
  mRange(0, 5),
  mRangeReversed(false),
  mScaleType(stLinear),
  mScaleLogBase(10),
  mScaleLogBaseLogInv(1.0/qLn(mScaleLogBase)),
  // internal members:
  mGrid(new QCPGrid(this)),
  mAxisPainter(new QCPAxisPainterPrivate(parent->parentPlot())),
  mLowestVisibleTick(0),
  mHighestVisibleTick(-1),
  mCachedMarginValid(false),
  mCachedMargin(0)
{
  mGrid->setVisible(false);
  setAntialiased(false);
  setLayer(mParentPlot->currentLayer()); // it's actually on that layer already, but we want it in front of the grid, so we place it on there again
  
  if (type == atTop)
  {
    setTickLabelPadding(3);
    setLabelPadding(6);
  } else if (type == atRight)
  {
    setTickLabelPadding(7);
    setLabelPadding(12);
  } else if (type == atBottom)
  {
    setTickLabelPadding(3);
    setLabelPadding(3);
  } else if (type == atLeft)
  {
    setTickLabelPadding(5);
    setLabelPadding(10);
  }
}

QCPAxis::~QCPAxis()
{
  delete mAxisPainter;
}

/* No documentation as it is a property getter */
int QCPAxis::tickLabelPadding() const
{
  return mAxisPainter->tickLabelPadding;
}

/* No documentation as it is a property getter */
double QCPAxis::tickLabelRotation() const
{
  return mAxisPainter->tickLabelRotation;
}

/* No documentation as it is a property getter */
QString QCPAxis::numberFormat() const
{
  QString result;
  result.append(mNumberFormatChar);
  if (mNumberBeautifulPowers)
  {
    result.append("b");
    if (mAxisPainter->numberMultiplyCross)
      result.append("c");
  }
  return result;
}

/* No documentation as it is a property getter */
int QCPAxis::tickLengthIn() const
{
  return mAxisPainter->tickLengthIn;
}

/* No documentation as it is a property getter */
int QCPAxis::tickLengthOut() const
{
  return mAxisPainter->tickLengthOut;
}

/* No documentation as it is a property getter */
int QCPAxis::subTickLengthIn() const
{
  return mAxisPainter->subTickLengthIn;
}

/* No documentation as it is a property getter */
int QCPAxis::subTickLengthOut() const
{
  return mAxisPainter->subTickLengthOut;
}

/* No documentation as it is a property getter */
int QCPAxis::labelPadding() const
{
  return mAxisPainter->labelPadding;
}

/* No documentation as it is a property getter */
int QCPAxis::offset() const
{
  return mAxisPainter->offset;
}

/* No documentation as it is a property getter */
QCPLineEnding QCPAxis::lowerEnding() const
{
  return mAxisPainter->lowerEnding;
}

/* No documentation as it is a property getter */
QCPLineEnding QCPAxis::upperEnding() const
{
  return mAxisPainter->upperEnding;
}

/*!
  Sets whether the axis uses a linear scale or a logarithmic scale. If \a type is set to \ref
  stLogarithmic, the logarithm base can be set with \ref setScaleLogBase. In logarithmic axis
  scaling, major tick marks appear at all powers of the logarithm base. Properties like tick step
  (\ref setTickStep) don't apply in logarithmic scaling. If you wish a decimal base but less major
  ticks, consider choosing a logarithm base of 100, 1000 or even higher.
  
  If \a type is \ref stLogarithmic and the number format (\ref setNumberFormat) uses the 'b' option
  (beautifully typeset decimal powers), the display usually is "1 [multiplication sign] 10
  [superscript] n", which looks unnatural for logarithmic scaling (the "1 [multiplication sign]"
  part). To only display the decimal power, set the number precision to zero with
  \ref setNumberPrecision.
*/
void QCPAxis::setScaleType(QCPAxis::ScaleType type)
{
  if (mScaleType != type)
  {
    mScaleType = type;
    if (mScaleType == stLogarithmic)
      setRange(mRange.sanitizedForLogScale());
    mCachedMarginValid = false;
    emit scaleTypeChanged(mScaleType);
  }
}

/*!
  If \ref setScaleType is set to \ref stLogarithmic, \a base will be the logarithm base of the
  scaling. In logarithmic axis scaling, major tick marks appear at all powers of \a base.
  
  Properties like tick step (\ref setTickStep) don't apply in logarithmic scaling. If you wish a decimal base but
  less major ticks, consider choosing \a base 100, 1000 or even higher.
*/
void QCPAxis::setScaleLogBase(double base)
{
  if (base > 1)
  {
    mScaleLogBase = base;
    mScaleLogBaseLogInv = 1.0/qLn(mScaleLogBase); // buffer for faster baseLog() calculation
    mCachedMarginValid = false;
  } else
    qDebug() << Q_FUNC_INFO << "Invalid logarithmic scale base (must be greater 1):" << base;
}

/*!
  Sets the range of the axis.
  
  This slot may be connected with the \ref rangeChanged signal of another axis so this axis
  is always synchronized with the other axis range, when it changes.
  
  To invert the direction of an axis, use \ref setRangeReversed.
*/
void QCPAxis::setRange(const QCPRange &range)
{
  if (range.lower == mRange.lower && range.upper == mRange.upper)
    return;
  
  if (!QCPRange::validRange(range)) return;
  QCPRange oldRange = mRange;
  if (mScaleType == stLogarithmic)
  {
    mRange = range.sanitizedForLogScale();
  } else
  {
    mRange = range.sanitizedForLinScale();
  }
  mCachedMarginValid = false;
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Sets whether the user can (de-)select the parts in \a selectable by clicking on the QCustomPlot surface.
  (When \ref QCustomPlot::setInteractions contains iSelectAxes.)
  
  However, even when \a selectable is set to a value not allowing the selection of a specific part,
  it is still possible to set the selection of this part manually, by calling \ref setSelectedParts
  directly.
  
  \see SelectablePart, setSelectedParts
*/
void QCPAxis::setSelectableParts(const SelectableParts &selectable)
{
  if (mSelectableParts != selectable)
  {
    mSelectableParts = selectable;
    emit selectableChanged(mSelectableParts);
  }
}

/*!
  Sets the selected state of the respective axis parts described by \ref SelectablePart. When a part
  is selected, it uses a different pen/font.
  
  The entire selection mechanism for axes is handled automatically when \ref
  QCustomPlot::setInteractions contains iSelectAxes. You only need to call this function when you
  wish to change the selection state manually.
  
  This function can change the selection state of a part, independent of the \ref setSelectableParts setting.
  
  emits the \ref selectionChanged signal when \a selected is different from the previous selection state.
  
  \see SelectablePart, setSelectableParts, selectTest, setSelectedBasePen, setSelectedTickPen, setSelectedSubTickPen,
  setSelectedTickLabelFont, setSelectedLabelFont, setSelectedTickLabelColor, setSelectedLabelColor
*/
void QCPAxis::setSelectedParts(const SelectableParts &selected)
{
  if (mSelectedParts != selected)
  {
    mSelectedParts = selected;
    emit selectionChanged(mSelectedParts);
  }
}

/*!
  \overload
  
  Sets the lower and upper bound of the axis range.
  
  To invert the direction of an axis, use \ref setRangeReversed.
  
  There is also a slot to set a range, see \ref setRange(const QCPRange &range).
*/
void QCPAxis::setRange(double lower, double upper)
{
  if (lower == mRange.lower && upper == mRange.upper)
    return;
  
  if (!QCPRange::validRange(lower, upper)) return;
  QCPRange oldRange = mRange;
  mRange.lower = lower;
  mRange.upper = upper;
  if (mScaleType == stLogarithmic)
  {
    mRange = mRange.sanitizedForLogScale();
  } else
  {
    mRange = mRange.sanitizedForLinScale();
  }
  mCachedMarginValid = false;
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  \overload
  
  Sets the range of the axis.
  
  The \a position coordinate indicates together with the \a alignment parameter, where the new
  range will be positioned. \a size defines the size of the new axis range. \a alignment may be
  Qt::AlignLeft, Qt::AlignRight or Qt::AlignCenter. This will cause the left border, right border,
  or center of the range to be aligned with \a position. Any other values of \a alignment will
  default to Qt::AlignCenter.
*/
void QCPAxis::setRange(double position, double size, Qt::AlignmentFlag alignment)
{
  if (alignment == Qt::AlignLeft)
    setRange(position, position+size);
  else if (alignment == Qt::AlignRight)
    setRange(position-size, position);
  else // alignment == Qt::AlignCenter
    setRange(position-size/2.0, position+size/2.0);
}

/*!
  Sets the lower bound of the axis range. The upper bound is not changed.
  \see setRange
*/
void QCPAxis::setRangeLower(double lower)
{
  if (mRange.lower == lower)
    return;
  
  QCPRange oldRange = mRange;
  mRange.lower = lower;
  if (mScaleType == stLogarithmic)
  {
    mRange = mRange.sanitizedForLogScale();
  } else
  {
    mRange = mRange.sanitizedForLinScale();
  }
  mCachedMarginValid = false;
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Sets the upper bound of the axis range. The lower bound is not changed.
  \see setRange
*/
void QCPAxis::setRangeUpper(double upper)
{
  if (mRange.upper == upper)
    return;
  
  QCPRange oldRange = mRange;
  mRange.upper = upper;
  if (mScaleType == stLogarithmic)
  {
    mRange = mRange.sanitizedForLogScale();
  } else
  {
    mRange = mRange.sanitizedForLinScale();
  }
  mCachedMarginValid = false;
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Sets whether the axis range (direction) is displayed reversed. Normally, the values on horizontal
  axes increase left to right, on vertical axes bottom to top. When \a reversed is set to true, the
  direction of increasing values is inverted.

  Note that the range and data interface stays the same for reversed axes, e.g. the \a lower part
  of the \ref setRange interface will still reference the mathematically smaller number than the \a
  upper part.
*/
void QCPAxis::setRangeReversed(bool reversed)
{
  if (mRangeReversed != reversed)
  {
    mRangeReversed = reversed;
    mCachedMarginValid = false;
  }
}

/*!
  Sets whether the tick positions should be calculated automatically (either from an automatically
  generated tick step or a tick step provided manually via \ref setTickStep, see \ref setAutoTickStep).
  
  If \a on is set to false, you must provide the tick positions manually via \ref setTickVector.
  For these manual ticks you may let QCPAxis generate the appropriate labels automatically by
  leaving \ref setAutoTickLabels set to true. If you also wish to control the displayed labels
  manually, set \ref setAutoTickLabels to false and provide the label strings with \ref
  setTickVectorLabels.
  
  If you need dynamically calculated tick vectors (and possibly tick label vectors), set the
  vectors in a slot connected to the \ref ticksRequest signal.
  
  \see setAutoTickLabels, setAutoSubTicks, setAutoTickCount, setAutoTickStep
*/
void QCPAxis::setAutoTicks(bool on)
{
  if (mAutoTicks != on)
  {
    mAutoTicks = on;
    mCachedMarginValid = false;
  }
}

/*!
  When \ref setAutoTickStep is true, \a approximateCount determines how many ticks should be
  generated in the visible range, approximately.
  
  It's not guaranteed that this number of ticks is met exactly, but approximately within a
  tolerance of about two.
  
  Only values greater than zero are accepted as \a approximateCount.
  
  \see setAutoTickStep, setAutoTicks, setAutoSubTicks
*/
void QCPAxis::setAutoTickCount(int approximateCount)
{
  if (mAutoTickCount != approximateCount)
  {
    if (approximateCount > 0)
    {
      mAutoTickCount = approximateCount;
      mCachedMarginValid = false;
    } else
      qDebug() << Q_FUNC_INFO << "approximateCount must be greater than zero:" << approximateCount;
  }
}

/*!
  Sets whether the tick labels are generated automatically. Depending on the tick label type (\ref
  ltNumber or \ref ltDateTime), the labels will either show the coordinate as floating point
  number (\ref setNumberFormat), or a date/time formatted according to \ref setDateTimeFormat.
  
  If \a on is set to false, you should provide the tick labels via \ref setTickVectorLabels. This
  is usually used in a combination with \ref setAutoTicks set to false for complete control over
  tick positions and labels, e.g. when the ticks should be at multiples of pi and show "2pi", "3pi"
  etc. as tick labels.
  
  If you need dynamically calculated tick vectors (and possibly tick label vectors), set the
  vectors in a slot connected to the \ref ticksRequest signal.
  
  \see setAutoTicks
*/
void QCPAxis::setAutoTickLabels(bool on)
{
  if (mAutoTickLabels != on)
  {
    mAutoTickLabels = on;
    mCachedMarginValid = false;
  }
}

/*!
  Sets whether the tick step, i.e. the interval between two (major) ticks, is calculated
  automatically. If \a on is set to true, the axis finds a tick step that is reasonable for human
  readable plots.

  The number of ticks the algorithm aims for within the visible range can be specified with \ref
  setAutoTickCount.
  
  If \a on is set to false, you may set the tick step manually with \ref setTickStep.
  
  \see setAutoTicks, setAutoSubTicks, setAutoTickCount
*/
void QCPAxis::setAutoTickStep(bool on)
{
  if (mAutoTickStep != on)
  {
    mAutoTickStep = on;
    mCachedMarginValid = false;
  }
}

/*!
  Sets whether the number of sub ticks in one tick interval is determined automatically. This
  works, as long as the tick step mantissa is a multiple of 0.5. When \ref setAutoTickStep is
  enabled, this is always the case.
  
  When \a on is set to false, you may set the sub tick count with \ref setSubTickCount manually.
  
  \see setAutoTickCount, setAutoTicks, setAutoTickStep
*/
void QCPAxis::setAutoSubTicks(bool on)
{
  if (mAutoSubTicks != on)
  {
    mAutoSubTicks = on;
    mCachedMarginValid = false;
  }
}

/*!
  Sets whether tick marks are displayed.

  Note that setting \a show to false does not imply that tick labels are invisible, too. To achieve
  that, see \ref setTickLabels.
*/
void QCPAxis::setTicks(bool show)
{
  if (mTicks != show)
  {
    mTicks = show;
    mCachedMarginValid = false;
  }
}

/*!
  Sets whether tick labels are displayed. Tick labels are the numbers drawn next to tick marks.
*/
void QCPAxis::setTickLabels(bool show)
{
  if (mTickLabels != show)
  {
    mTickLabels = show;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the distance between the axis base line (including any outward ticks) and the tick labels.
  \see setLabelPadding, setPadding
*/
void QCPAxis::setTickLabelPadding(int padding)
{
  if (mAxisPainter->tickLabelPadding != padding)
  {
    mAxisPainter->tickLabelPadding = padding;
    mCachedMarginValid = false;
  }
}

/*!
  Sets whether the tick labels display numbers or dates/times.
  
  If \a type is set to \ref ltNumber, the format specifications of \ref setNumberFormat apply.
  
  If \a type is set to \ref ltDateTime, the format specifications of \ref setDateTimeFormat apply.
  
  In QCustomPlot, date/time coordinates are <tt>double</tt> numbers representing the seconds since
  1970-01-01T00:00:00 UTC. This format can be retrieved from QDateTime objects with the
  QDateTime::toTime_t() function. Since this only gives a resolution of one second, there is also
  the QDateTime::toMSecsSinceEpoch() function which returns the timespan described above in
  milliseconds. Divide its return value by 1000.0 to get a value with the format needed for
  date/time plotting, with a resolution of one millisecond.
  
  Using the toMSecsSinceEpoch function allows dates that go back to 2nd January 4713 B.C.
  (represented by a negative number), unlike the toTime_t function, which works with unsigned
  integers and thus only goes back to 1st January 1970. So both for range and accuracy, use of
  toMSecsSinceEpoch()/1000.0 should be preferred as key coordinate for date/time axes.
  
  \see setTickLabels
*/
void QCPAxis::setTickLabelType(LabelType type)
{
  if (mTickLabelType != type)
  {
    mTickLabelType = type;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the font of the tick labels.
  
  \see setTickLabels, setTickLabelColor
*/
void QCPAxis::setTickLabelFont(const QFont &font)
{
  if (font != mTickLabelFont)
  {
    mTickLabelFont = font;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the color of the tick labels.
  
  \see setTickLabels, setTickLabelFont
*/
void QCPAxis::setTickLabelColor(const QColor &color)
{
  if (color != mTickLabelColor)
  {
    mTickLabelColor = color;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the rotation of the tick labels. If \a degrees is zero, the labels are drawn normally. Else,
  the tick labels are drawn rotated by \a degrees clockwise. The specified angle is bound to values
  from -90 to 90 degrees.
  
  If \a degrees is exactly -90, 0 or 90, the tick labels are centered on the tick coordinate. For
  other angles, the label is drawn with an offset such that it seems to point toward or away from
  the tick mark.
*/
void QCPAxis::setTickLabelRotation(double degrees)
{
  if (!qFuzzyIsNull(degrees-mAxisPainter->tickLabelRotation))
  {
    mAxisPainter->tickLabelRotation = qBound(-90.0, degrees, 90.0);
    mCachedMarginValid = false;
  }
}

/*!
  Sets the format in which dates and times are displayed as tick labels, if \ref setTickLabelType is \ref ltDateTime.
  for details about the \a format string, see the documentation of QDateTime::toString().
  
  Newlines can be inserted with "\n".
  
  \see setDateTimeSpec
*/
void QCPAxis::setDateTimeFormat(const QString &format)
{
  if (mDateTimeFormat != format)
  {
    mDateTimeFormat = format;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the time spec that is used for the date time values when \ref setTickLabelType is \ref
  ltDateTime.

  The default value of QDateTime objects (and also QCustomPlot) is <tt>Qt::LocalTime</tt>. However,
  if the date time values passed to QCustomPlot are given in the UTC spec, set \a
  timeSpec to <tt>Qt::UTC</tt> to get the correct axis labels.
  
  \see setDateTimeFormat
*/
void QCPAxis::setDateTimeSpec(const Qt::TimeSpec &timeSpec)
{
  mDateTimeSpec = timeSpec;
}

/*!
  Sets the number format for the numbers drawn as tick labels (if tick label type is \ref
  ltNumber). This \a formatCode is an extended version of the format code used e.g. by
  QString::number() and QLocale::toString(). For reference about that, see the "Argument Formats"
  section in the detailed description of the QString class. \a formatCode is a string of one, two
  or three characters. The first character is identical to the normal format code used by Qt. In
  short, this means: 'e'/'E' scientific format, 'f' fixed format, 'g'/'G' scientific or fixed,
  whichever is shorter.
  
  The second and third characters are optional and specific to QCustomPlot:\n
  If the first char was 'e' or 'g', numbers are/might be displayed in the scientific format, e.g.
  "5.5e9", which is ugly in a plot. So when the second char of \a formatCode is set to 'b' (for
  "beautiful"), those exponential numbers are formatted in a more natural way, i.e. "5.5
  [multiplication sign] 10 [superscript] 9". By default, the multiplication sign is a centered dot.
  If instead a cross should be shown (as is usual in the USA), the third char of \a formatCode can
  be set to 'c'. The inserted multiplication signs are the UTF-8 characters 215 (0xD7) for the
  cross and 183 (0xB7) for the dot.
  
  If the scale type (\ref setScaleType) is \ref stLogarithmic and the \a formatCode uses the 'b'
  option (beautifully typeset decimal powers), the display usually is "1 [multiplication sign] 10
  [superscript] n", which looks unnatural for logarithmic scaling (the "1 [multiplication sign]"
  part). To only display the decimal power, set the number precision to zero with \ref
  setNumberPrecision.
  
  Examples for \a formatCode:
  \li \c g normal format code behaviour. If number is small, fixed format is used, if number is large,
  normal scientific format is used
  \li \c gb If number is small, fixed format is used, if number is large, scientific format is used with
  beautifully typeset decimal powers and a dot as multiplication sign
  \li \c ebc All numbers are in scientific format with beautifully typeset decimal power and a cross as
  multiplication sign
  \li \c fb illegal format code, since fixed format doesn't support (or need) beautifully typeset decimal
  powers. Format code will be reduced to 'f'.
  \li \c hello illegal format code, since first char is not 'e', 'E', 'f', 'g' or 'G'. Current format
  code will not be changed.
*/
void QCPAxis::setNumberFormat(const QString &formatCode)
{
  if (formatCode.isEmpty())
  {
    qDebug() << Q_FUNC_INFO << "Passed formatCode is empty";
    return;
  }
  mCachedMarginValid = false;
  
  // interpret first char as number format char:
  QString allowedFormatChars = "eEfgG";
  if (allowedFormatChars.contains(formatCode.at(0)))
  {
    mNumberFormatChar = formatCode.at(0).toLatin1();
  } else
  {
    qDebug() << Q_FUNC_INFO << "Invalid number format code (first char not in 'eEfgG'):" << formatCode;
    return;
  }
  if (formatCode.length() < 2)
  {
    mNumberBeautifulPowers = false;
    mAxisPainter->numberMultiplyCross = false;
    return;
  }
  
  // interpret second char as indicator for beautiful decimal powers:
  if (formatCode.at(1) == 'b' && (mNumberFormatChar == 'e' || mNumberFormatChar == 'g'))
  {
    mNumberBeautifulPowers = true;
  } else
  {
    qDebug() << Q_FUNC_INFO << "Invalid number format code (second char not 'b' or first char neither 'e' nor 'g'):" << formatCode;
    return;
  }
  if (formatCode.length() < 3)
  {
    mAxisPainter->numberMultiplyCross = false;
    return;
  }
  
  // interpret third char as indicator for dot or cross multiplication symbol:
  if (formatCode.at(2) == 'c')
  {
    mAxisPainter->numberMultiplyCross = true;
  } else if (formatCode.at(2) == 'd')
  {
    mAxisPainter->numberMultiplyCross = false;
  } else
  {
    qDebug() << Q_FUNC_INFO << "Invalid number format code (third char neither 'c' nor 'd'):" << formatCode;
    return;
  }
}

/*!
  Sets the precision of the tick label numbers. See QLocale::toString(double i, char f, int prec)
  for details. The effect of precisions are most notably for number Formats starting with 'e', see
  \ref setNumberFormat

  If the scale type (\ref setScaleType) is \ref stLogarithmic and the number format (\ref
  setNumberFormat) uses the 'b' format code (beautifully typeset decimal powers), the display
  usually is "1 [multiplication sign] 10 [superscript] n", which looks unnatural for logarithmic
  scaling (the redundant "1 [multiplication sign]" part). To only display the decimal power "10
  [superscript] n", set \a precision to zero.
*/
void QCPAxis::setNumberPrecision(int precision)
{
  if (mNumberPrecision != precision)
  {
    mNumberPrecision = precision;
    mCachedMarginValid = false;
  }
}

/*!
  If \ref setAutoTickStep is set to false, use this function to set the tick step manually.
  The tick step is the interval between (major) ticks, in plot coordinates.
  \see setSubTickCount
*/
void QCPAxis::setTickStep(double step)
{
  if (mTickStep != step)
  {
    mTickStep = step;
    mCachedMarginValid = false;
  }
}

/*!
  If you want full control over what ticks (and possibly labels) the axes show, this function is
  used to set the coordinates at which ticks will appear.\ref setAutoTicks must be disabled, else
  the provided tick vector will be overwritten with automatically generated tick coordinates upon
  replot. The labels of the ticks can be generated automatically when \ref setAutoTickLabels is
  left enabled. If it is disabled, you can set the labels manually with \ref setTickVectorLabels.
  
  \a vec is a vector containing the positions of the ticks, in plot coordinates.
  
  \warning \a vec must be sorted in ascending order, no additional checks are made to ensure this.

  \see setTickVectorLabels
*/
void QCPAxis::setTickVector(const QVector<double> &vec)
{
  // don't check whether mTickVector != vec here, because it takes longer than we would save
  mTickVector = vec;
  mCachedMarginValid = false;
}

/*!
  If you want full control over what ticks and labels the axes show, this function is used to set a
  number of QStrings that will be displayed at the tick positions which you need to provide with
  \ref setTickVector. These two vectors should have the same size. (Note that you need to disable
  \ref setAutoTicks and \ref setAutoTickLabels first.)
  
  \a vec is a vector containing the labels of the ticks. The entries correspond to the respective
  indices in the tick vector, passed via \ref setTickVector.
  
  \see setTickVector
*/
void QCPAxis::setTickVectorLabels(const QVector<QString> &vec)
{
  // don't check whether mTickVectorLabels != vec here, because it takes longer than we would save
  mTickVectorLabels = vec;
  mCachedMarginValid = false;
}

/*!
  Sets the length of the ticks in pixels. \a inside is the length the ticks will reach inside the
  plot and \a outside is the length they will reach outside the plot. If \a outside is greater than
  zero, the tick labels and axis label will increase their distance to the axis accordingly, so
  they won't collide with the ticks.
  
  \see setSubTickLength, setTickLengthIn, setTickLengthOut
*/
void QCPAxis::setTickLength(int inside, int outside)
{
  setTickLengthIn(inside);
  setTickLengthOut(outside);
}

/*!
  Sets the length of the inward ticks in pixels. \a inside is the length the ticks will reach
  inside the plot.
  
  \see setTickLengthOut, setTickLength, setSubTickLength
*/
void QCPAxis::setTickLengthIn(int inside)
{
  if (mAxisPainter->tickLengthIn != inside)
  {
    mAxisPainter->tickLengthIn = inside;
  }
}

/*!
  Sets the length of the outward ticks in pixels. \a outside is the length the ticks will reach
  outside the plot. If \a outside is greater than zero, the tick labels and axis label will
  increase their distance to the axis accordingly, so they won't collide with the ticks.
  
  \see setTickLengthIn, setTickLength, setSubTickLength
*/
void QCPAxis::setTickLengthOut(int outside)
{
  if (mAxisPainter->tickLengthOut != outside)
  {
    mAxisPainter->tickLengthOut = outside;
    mCachedMarginValid = false; // only outside tick length can change margin
  }
}

/*!
  Sets the number of sub ticks in one (major) tick step. A sub tick count of three for example,
  divides the tick intervals in four sub intervals.
  
  By default, the number of sub ticks is chosen automatically in a reasonable manner as long as the
  mantissa of the tick step is a multiple of 0.5. When \ref setAutoTickStep is enabled, this is
  always the case.

  If you want to disable automatic sub tick count and use this function to set the count manually,
  see \ref setAutoSubTicks.
*/
void QCPAxis::setSubTickCount(int count)
{
  mSubTickCount = count;
}

/*!
  Sets the length of the subticks in pixels. \a inside is the length the subticks will reach inside
  the plot and \a outside is the length they will reach outside the plot. If \a outside is greater
  than zero, the tick labels and axis label will increase their distance to the axis accordingly,
  so they won't collide with the ticks.
  
  \see setTickLength, setSubTickLengthIn, setSubTickLengthOut
*/
void QCPAxis::setSubTickLength(int inside, int outside)
{
  setSubTickLengthIn(inside);
  setSubTickLengthOut(outside);
}

/*!
  Sets the length of the inward subticks in pixels. \a inside is the length the subticks will reach inside
  the plot.
  
  \see setSubTickLengthOut, setSubTickLength, setTickLength
*/
void QCPAxis::setSubTickLengthIn(int inside)
{
  if (mAxisPainter->subTickLengthIn != inside)
  {
    mAxisPainter->subTickLengthIn = inside;
  }
}

/*!
  Sets the length of the outward subticks in pixels. \a outside is the length the subticks will reach
  outside the plot. If \a outside is greater than zero, the tick labels will increase their
  distance to the axis accordingly, so they won't collide with the ticks.
  
  \see setSubTickLengthIn, setSubTickLength, setTickLength
*/
void QCPAxis::setSubTickLengthOut(int outside)
{
  if (mAxisPainter->subTickLengthOut != outside)
  {
    mAxisPainter->subTickLengthOut = outside;
    mCachedMarginValid = false; // only outside tick length can change margin
  }
}

/*!
  Sets the pen, the axis base line is drawn with.
  
  \see setTickPen, setSubTickPen
*/
void QCPAxis::setBasePen(const QPen &pen)
{
  mBasePen = pen;
}

/*!
  Sets the pen, tick marks will be drawn with.
  
  \see setTickLength, setBasePen
*/
void QCPAxis::setTickPen(const QPen &pen)
{
  mTickPen = pen;
}

/*!
  Sets the pen, subtick marks will be drawn with.
  
  \see setSubTickCount, setSubTickLength, setBasePen
*/
void QCPAxis::setSubTickPen(const QPen &pen)
{
  mSubTickPen = pen;
}

/*!
  Sets the font of the axis label.
  
  \see setLabelColor
*/
void QCPAxis::setLabelFont(const QFont &font)
{
  if (mLabelFont != font)
  {
    mLabelFont = font;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the color of the axis label.
  
  \see setLabelFont
*/
void QCPAxis::setLabelColor(const QColor &color)
{
  mLabelColor = color;
}

/*!
  Sets the text of the axis label that will be shown below/above or next to the axis, depending on
  its orientation. To disable axis labels, pass an empty string as \a str.
*/
void QCPAxis::setLabel(const QString &str)
{
  if (mLabel != str)
  {
    mLabel = str;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the distance between the tick labels and the axis label.
  
  \see setTickLabelPadding, setPadding
*/
void QCPAxis::setLabelPadding(int padding)
{
  if (mAxisPainter->labelPadding != padding)
  {
    mAxisPainter->labelPadding = padding;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the padding of the axis.

  When \ref QCPAxisRect::setAutoMargins is enabled, the padding is the additional outer most space,
  that is left blank.
  
  The axis padding has no meaning if \ref QCPAxisRect::setAutoMargins is disabled.
  
  \see setLabelPadding, setTickLabelPadding
*/
void QCPAxis::setPadding(int padding)
{
  if (mPadding != padding)
  {
    mPadding = padding;
    mCachedMarginValid = false;
  }
}

/*!
  Sets the offset the axis has to its axis rect side.
  
  If an axis rect side has multiple axes and automatic margin calculation is enabled for that side,
  only the offset of the inner most axis has meaning (even if it is set to be invisible). The
  offset of the other, outer axes is controlled automatically, to place them at appropriate
  positions.
*/
void QCPAxis::setOffset(int offset)
{
  mAxisPainter->offset = offset;
}

/*!
  Sets the font that is used for tick labels when they are selected.
  
  \see setTickLabelFont, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedTickLabelFont(const QFont &font)
{
  if (font != mSelectedTickLabelFont)
  {
    mSelectedTickLabelFont = font;
    // don't set mCachedMarginValid to false here because margin calculation is always done with non-selected fonts
  }
}

/*!
  Sets the font that is used for the axis label when it is selected.
  
  \see setLabelFont, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedLabelFont(const QFont &font)
{
  mSelectedLabelFont = font;
  // don't set mCachedMarginValid to false here because margin calculation is always done with non-selected fonts
}

/*!
  Sets the color that is used for tick labels when they are selected.
  
  \see setTickLabelColor, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedTickLabelColor(const QColor &color)
{
  if (color != mSelectedTickLabelColor)
  {
    mSelectedTickLabelColor = color;
  }
}

/*!
  Sets the color that is used for the axis label when it is selected.
  
  \see setLabelColor, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedLabelColor(const QColor &color)
{
  mSelectedLabelColor = color;
}

/*!
  Sets the pen that is used to draw the axis base line when selected.
  
  \see setBasePen, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedBasePen(const QPen &pen)
{
  mSelectedBasePen = pen;
}

/*!
  Sets the pen that is used to draw the (major) ticks when selected.
  
  \see setTickPen, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedTickPen(const QPen &pen)
{
  mSelectedTickPen = pen;
}

/*!
  Sets the pen that is used to draw the subticks when selected.
  
  \see setSubTickPen, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedSubTickPen(const QPen &pen)
{
  mSelectedSubTickPen = pen;
}

/*!
  Sets the style for the lower axis ending. See the documentation of QCPLineEnding for available
  styles.
  
  For horizontal axes, this method refers to the left ending, for vertical axes the bottom ending.
  Note that this meaning does not change when the axis range is reversed with \ref
  setRangeReversed.
  
  \see setUpperEnding
*/
void QCPAxis::setLowerEnding(const QCPLineEnding &ending)
{
  mAxisPainter->lowerEnding = ending;
}

/*!
  Sets the style for the upper axis ending. See the documentation of QCPLineEnding for available
  styles.
  
  For horizontal axes, this method refers to the right ending, for vertical axes the top ending.
  Note that this meaning does not change when the axis range is reversed with \ref
  setRangeReversed.
  
  \see setLowerEnding
*/
void QCPAxis::setUpperEnding(const QCPLineEnding &ending)
{
  mAxisPainter->upperEnding = ending;
}

/*!
  If the scale type (\ref setScaleType) is \ref stLinear, \a diff is added to the lower and upper
  bounds of the range. The range is simply moved by \a diff.
  
  If the scale type is \ref stLogarithmic, the range bounds are multiplied by \a diff. This
  corresponds to an apparent "linear" move in logarithmic scaling by a distance of log(diff).
*/
void QCPAxis::moveRange(double diff)
{
  QCPRange oldRange = mRange;
  if (mScaleType == stLinear)
  {
    mRange.lower += diff;
    mRange.upper += diff;
  } else // mScaleType == stLogarithmic
  {
    mRange.lower *= diff;
    mRange.upper *= diff;
  }
  mCachedMarginValid = false;
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Scales the range of this axis by \a factor around the coordinate \a center. For example, if \a
  factor is 2.0, \a center is 1.0, then the axis range will double its size, and the point at
  coordinate 1.0 won't have changed its position in the QCustomPlot widget (i.e. coordinates
  around 1.0 will have moved symmetrically closer to 1.0).
*/
void QCPAxis::scaleRange(double factor, double center)
{
  QCPRange oldRange = mRange;
  if (mScaleType == stLinear)
  {
    QCPRange newRange;
    newRange.lower = (mRange.lower-center)*factor + center;
    newRange.upper = (mRange.upper-center)*factor + center;
    if (QCPRange::validRange(newRange))
      mRange = newRange.sanitizedForLinScale();
  } else // mScaleType == stLogarithmic
  {
    if ((mRange.upper < 0 && center < 0) || (mRange.upper > 0 && center > 0)) // make sure center has same sign as range
    {
      QCPRange newRange;
      newRange.lower = pow(mRange.lower/center, factor)*center;
      newRange.upper = pow(mRange.upper/center, factor)*center;
      if (QCPRange::validRange(newRange))
        mRange = newRange.sanitizedForLogScale();
    } else
      qDebug() << Q_FUNC_INFO << "Center of scaling operation doesn't lie in same logarithmic sign domain as range:" << center;
  }
  mCachedMarginValid = false;
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Scales the range of this axis to have a certain scale \a ratio to \a otherAxis. The scaling will
  be done around the center of the current axis range.

  For example, if \a ratio is 1, this axis is the \a yAxis and \a otherAxis is \a xAxis, graphs
  plotted with those axes will appear in a 1:1 aspect ratio, independent of the aspect ratio the
  axis rect has.

  This is an operation that changes the range of this axis once, it doesn't fix the scale ratio
  indefinitely. Note that calling this function in the constructor of the QCustomPlot's parent
  won't have the desired effect, since the widget dimensions aren't defined yet, and a resizeEvent
  will follow.
*/
void QCPAxis::setScaleRatio(const QCPAxis *otherAxis, double ratio)
{
  int otherPixelSize, ownPixelSize;
  
  if (otherAxis->orientation() == Qt::Horizontal)
    otherPixelSize = otherAxis->axisRect()->width();
  else
    otherPixelSize = otherAxis->axisRect()->height();
  
  if (orientation() == Qt::Horizontal)
    ownPixelSize = axisRect()->width();
  else
    ownPixelSize = axisRect()->height();
  
  double newRangeSize = ratio*otherAxis->range().size()*ownPixelSize/(double)otherPixelSize;
  setRange(range().center(), newRangeSize, Qt::AlignCenter);
}

/*!
  Changes the axis range such that all plottables associated with this axis are fully visible in
  that dimension.
  
  \see QCPAbstractPlottable::rescaleAxes, QCustomPlot::rescaleAxes
*/
void QCPAxis::rescale(bool onlyVisiblePlottables)
{
  QList<QCPAbstractPlottable*> p = plottables();
  QCPRange newRange;
  bool haveRange = false;
  for (int i=0; i<p.size(); ++i)
  {
    if (!p.at(i)->realVisibility() && onlyVisiblePlottables)
      continue;
    QCPRange plottableRange;
    bool currentFoundRange;
    QCPAbstractPlottable::SignDomain signDomain = QCPAbstractPlottable::sdBoth;
    if (mScaleType == stLogarithmic)
      signDomain = (mRange.upper < 0 ? QCPAbstractPlottable::sdNegative : QCPAbstractPlottable::sdPositive);
    if (p.at(i)->keyAxis() == this)
      plottableRange = p.at(i)->getKeyRange(currentFoundRange, signDomain);
    else
      plottableRange = p.at(i)->getValueRange(currentFoundRange, signDomain);
    if (currentFoundRange)
    {
      if (!haveRange)
        newRange = plottableRange;
      else
        newRange.expand(plottableRange);
      haveRange = true;
    }
  }
  if (haveRange)
  {
    if (!QCPRange::validRange(newRange)) // likely due to range being zero (plottable has only constant data in this axis dimension), shift current range to at least center the plottable
    {
      double center = (newRange.lower+newRange.upper)*0.5; // upper and lower should be equal anyway, but just to make sure, incase validRange returned false for other reason
      if (mScaleType == stLinear)
      {
        newRange.lower = center-mRange.size()/2.0;
        newRange.upper = center+mRange.size()/2.0;
      } else // mScaleType == stLogarithmic
      {
        newRange.lower = center/qSqrt(mRange.upper/mRange.lower);
        newRange.upper = center*qSqrt(mRange.upper/mRange.lower);
      }
    }
    setRange(newRange);
  }
}

/*!
  Transforms \a value, in pixel coordinates of the QCustomPlot widget, to axis coordinates.
*/
double QCPAxis::pixelToCoord(double value) const
{
  if (orientation() == Qt::Horizontal)
  {
    if (mScaleType == stLinear)
    {
      if (!mRangeReversed)
        return (value-mAxisRect->left())/(double)mAxisRect->width()*mRange.size()+mRange.lower;
      else
        return -(value-mAxisRect->left())/(double)mAxisRect->width()*mRange.size()+mRange.upper;
    } else // mScaleType == stLogarithmic
    {
      if (!mRangeReversed)
        return pow(mRange.upper/mRange.lower, (value-mAxisRect->left())/(double)mAxisRect->width())*mRange.lower;
      else
        return pow(mRange.upper/mRange.lower, (mAxisRect->left()-value)/(double)mAxisRect->width())*mRange.upper;
    }
  } else // orientation() == Qt::Vertical
  {
    if (mScaleType == stLinear)
    {
      if (!mRangeReversed)
        return (mAxisRect->bottom()-value)/(double)mAxisRect->height()*mRange.size()+mRange.lower;
      else
        return -(mAxisRect->bottom()-value)/(double)mAxisRect->height()*mRange.size()+mRange.upper;
    } else // mScaleType == stLogarithmic
    {
      if (!mRangeReversed)
        return pow(mRange.upper/mRange.lower, (mAxisRect->bottom()-value)/(double)mAxisRect->height())*mRange.lower;
      else
        return pow(mRange.upper/mRange.lower, (value-mAxisRect->bottom())/(double)mAxisRect->height())*mRange.upper;
    }
  }
}

/*!
  Transforms \a value, in coordinates of the axis, to pixel coordinates of the QCustomPlot widget.
*/
double QCPAxis::coordToPixel(double value) const
{
  if (orientation() == Qt::Horizontal)
  {
    if (mScaleType == stLinear)
    {
      if (!mRangeReversed)
        return (value-mRange.lower)/mRange.size()*mAxisRect->width()+mAxisRect->left();
      else
        return (mRange.upper-value)/mRange.size()*mAxisRect->width()+mAxisRect->left();
    } else // mScaleType == stLogarithmic
    {
      if (value >= 0 && mRange.upper < 0) // invalid value for logarithmic scale, just draw it outside visible range
        return !mRangeReversed ? mAxisRect->right()+200 : mAxisRect->left()-200;
      else if (value <= 0 && mRange.upper > 0) // invalid value for logarithmic scale, just draw it outside visible range
        return !mRangeReversed ? mAxisRect->left()-200 : mAxisRect->right()+200;
      else
      {
        if (!mRangeReversed)
          return baseLog(value/mRange.lower)/baseLog(mRange.upper/mRange.lower)*mAxisRect->width()+mAxisRect->left();
        else
          return baseLog(mRange.upper/value)/baseLog(mRange.upper/mRange.lower)*mAxisRect->width()+mAxisRect->left();
      }
    }
  } else // orientation() == Qt::Vertical
  {
    if (mScaleType == stLinear)
    {
      if (!mRangeReversed)
        return mAxisRect->bottom()-(value-mRange.lower)/mRange.size()*mAxisRect->height();
      else
        return mAxisRect->bottom()-(mRange.upper-value)/mRange.size()*mAxisRect->height();
    } else // mScaleType == stLogarithmic
    {
      if (value >= 0 && mRange.upper < 0) // invalid value for logarithmic scale, just draw it outside visible range
        return !mRangeReversed ? mAxisRect->top()-200 : mAxisRect->bottom()+200;
      else if (value <= 0 && mRange.upper > 0) // invalid value for logarithmic scale, just draw it outside visible range
        return !mRangeReversed ? mAxisRect->bottom()+200 : mAxisRect->top()-200;
      else
      {
        if (!mRangeReversed)
          return mAxisRect->bottom()-baseLog(value/mRange.lower)/baseLog(mRange.upper/mRange.lower)*mAxisRect->height();
        else
          return mAxisRect->bottom()-baseLog(mRange.upper/value)/baseLog(mRange.upper/mRange.lower)*mAxisRect->height();
      }
    }
  }
}

/*!
  Returns the part of the axis that is hit by \a pos (in pixels). The return value of this function
  is independent of the user-selectable parts defined with \ref setSelectableParts. Further, this
  function does not change the current selection state of the axis.
  
  If the axis is not visible (\ref setVisible), this function always returns \ref spNone.
  
  \see setSelectedParts, setSelectableParts, QCustomPlot::setInteractions
*/
QCPAxis::SelectablePart QCPAxis::getPartAt(const QPointF &pos) const
{
  if (!mVisible)
    return spNone;
  
  if (mAxisPainter->axisSelectionBox().contains(pos.toPoint()))
    return spAxis;
  else if (mAxisPainter->tickLabelsSelectionBox().contains(pos.toPoint()))
    return spTickLabels;
  else if (mAxisPainter->labelSelectionBox().contains(pos.toPoint()))
    return spAxisLabel;
  else
    return spNone;
}

/* inherits documentation from base class */
double QCPAxis::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  if (!mParentPlot) return -1;
  SelectablePart part = getPartAt(pos);
  if ((onlySelectable && !mSelectableParts.testFlag(part)) || part == spNone)
    return -1;
  
  if (details)
    details->setValue(part);
  return mParentPlot->selectionTolerance()*0.99;
}

/*!
  Returns a list of all the plottables that have this axis as key or value axis.
  
  If you are only interested in plottables of type QCPGraph, see \ref graphs.
  
  \see graphs, items
*/
QList<QCPAbstractPlottable*> QCPAxis::plottables() const
{
  QList<QCPAbstractPlottable*> result;
  if (!mParentPlot) return result;
  
  for (int i=0; i<mParentPlot->mPlottables.size(); ++i)
  {
    if (mParentPlot->mPlottables.at(i)->keyAxis() == this ||mParentPlot->mPlottables.at(i)->valueAxis() == this)
      result.append(mParentPlot->mPlottables.at(i));
  }
  return result;
}

/*!
  Returns a list of all the graphs that have this axis as key or value axis.
  
  \see plottables, items
*/
QList<QCPGraph*> QCPAxis::graphs() const
{
  QList<QCPGraph*> result;
  if (!mParentPlot) return result;
  
  for (int i=0; i<mParentPlot->mGraphs.size(); ++i)
  {
    if (mParentPlot->mGraphs.at(i)->keyAxis() == this || mParentPlot->mGraphs.at(i)->valueAxis() == this)
      result.append(mParentPlot->mGraphs.at(i));
  }
  return result;
}

/*!
  Returns a list of all the items that are associated with this axis. An item is considered
  associated with an axis if at least one of its positions uses the axis as key or value axis.
  
  \see plottables, graphs
*/
QList<QCPAbstractItem*> QCPAxis::items() const
{
  QList<QCPAbstractItem*> result;
  if (!mParentPlot) return result;
  
  for (int itemId=0; itemId<mParentPlot->mItems.size(); ++itemId)
  {
    QList<QCPItemPosition*> positions = mParentPlot->mItems.at(itemId)->positions();
    for (int posId=0; posId<positions.size(); ++posId)
    {
      if (positions.at(posId)->keyAxis() == this || positions.at(posId)->valueAxis() == this)
      {
        result.append(mParentPlot->mItems.at(itemId));
        break;
      }
    }
  }
  return result;
}

/*!
  Transforms a margin side to the logically corresponding axis type. (QCP::msLeft to
  QCPAxis::atLeft, QCP::msRight to QCPAxis::atRight, etc.)
*/
QCPAxis::AxisType QCPAxis::marginSideToAxisType(QCP::MarginSide side)
{
  switch (side)
  {
    case QCP::msLeft: return atLeft;
    case QCP::msRight: return atRight;
    case QCP::msTop: return atTop;
    case QCP::msBottom: return atBottom;
    default: break;
  }
  qDebug() << Q_FUNC_INFO << "Invalid margin side passed:" << (int)side;
  return atLeft;
}

/*!
  Returns the axis type that describes the opposite axis of an axis with the specified \a type.
*/
QCPAxis::AxisType QCPAxis::opposite(QCPAxis::AxisType type)
{
  switch (type)
  {
    case atLeft: return atRight; break;
    case atRight: return atLeft; break;
    case atBottom: return atTop; break;
    case atTop: return atBottom; break;
    default: qDebug() << Q_FUNC_INFO << "invalid axis type"; return atLeft; break;
  }
}

/*! \internal
  
  This function is called to prepare the tick vector, sub tick vector and tick label vector. If
  \ref setAutoTicks is set to true, appropriate tick values are determined automatically via \ref
  generateAutoTicks. If it's set to false, the signal ticksRequest is emitted, which can be used to
  provide external tick positions. Then the sub tick vectors and tick label vectors are created.
*/
void QCPAxis::setupTickVectors()
{
  if (!mParentPlot) return;
  if ((!mTicks && !mTickLabels && !mGrid->visible()) || mRange.size() <= 0) return;
  
  // fill tick vectors, either by auto generating or by notifying user to fill the vectors himself
  if (mAutoTicks)
  {
    generateAutoTicks();
  } else
  {
    emit ticksRequest();
  }
  
  visibleTickBounds(mLowestVisibleTick, mHighestVisibleTick);
  if (mTickVector.isEmpty())
  {
    mSubTickVector.clear();
    return;
  }
  
  // generate subticks between ticks:
  mSubTickVector.resize((mTickVector.size()-1)*mSubTickCount);
  if (mSubTickCount > 0)
  {
    double subTickStep = 0;
    double subTickPosition = 0;
    int subTickIndex = 0;
    bool done = false;
    int lowTick = mLowestVisibleTick > 0 ? mLowestVisibleTick-1 : mLowestVisibleTick;
    int highTick = mHighestVisibleTick < mTickVector.size()-1 ? mHighestVisibleTick+1 : mHighestVisibleTick;
    for (int i=lowTick+1; i<=highTick; ++i)
    {
      subTickStep = (mTickVector.at(i)-mTickVector.at(i-1))/(double)(mSubTickCount+1);
      for (int k=1; k<=mSubTickCount; ++k)
      {
        subTickPosition = mTickVector.at(i-1) + k*subTickStep;
        if (subTickPosition < mRange.lower)
          continue;
        if (subTickPosition > mRange.upper)
        {
          done = true;
          break;
        }
        mSubTickVector[subTickIndex] = subTickPosition;
        subTickIndex++;
      }
      if (done) break;
    }
    mSubTickVector.resize(subTickIndex);
  }

  // generate tick labels according to tick positions:
  if (mAutoTickLabels)
  {
    int vecsize = mTickVector.size();
    mTickVectorLabels.resize(vecsize);
    if (mTickLabelType == ltNumber)
    {
      for (int i=mLowestVisibleTick; i<=mHighestVisibleTick; ++i)
        mTickVectorLabels[i] = mParentPlot->locale().toString(mTickVector.at(i), mNumberFormatChar, mNumberPrecision);
    } else if (mTickLabelType == ltDateTime)
    {
      for (int i=mLowestVisibleTick; i<=mHighestVisibleTick; ++i)
      {
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0) // use fromMSecsSinceEpoch function if available, to gain sub-second accuracy on tick labels (e.g. for format "hh:mm:ss:zzz")
        mTickVectorLabels[i] = mParentPlot->locale().toString(QDateTime::fromTime_t(mTickVector.at(i)).toTimeSpec(mDateTimeSpec), mDateTimeFormat);
#else
        mTickVectorLabels[i] = mParentPlot->locale().toString(QDateTime::fromMSecsSinceEpoch(mTickVector.at(i)*1000).toTimeSpec(mDateTimeSpec), mDateTimeFormat);
#endif
      }
    }
  } else // mAutoTickLabels == false
  {
    if (mAutoTicks) // ticks generated automatically, but not ticklabels, so emit ticksRequest here for labels
    {
      emit ticksRequest();
    }
    // make sure provided tick label vector has correct (minimal) length:
    if (mTickVectorLabels.size() < mTickVector.size())
      mTickVectorLabels.resize(mTickVector.size());
  }
}

/*! \internal
  
  If \ref setAutoTicks is set to true, this function is called by \ref setupTickVectors to
  generate reasonable tick positions (and subtick count). The algorithm tries to create
  approximately <tt>mAutoTickCount</tt> ticks (set via \ref setAutoTickCount).
 
  If the scale is logarithmic, \ref setAutoTickCount is ignored, and one tick is generated at every
  power of the current logarithm base, set via \ref setScaleLogBase.
*/
void QCPAxis::generateAutoTicks()
{
  if (mScaleType == stLinear)
  {
    if (mAutoTickStep)
    {
      // Generate tick positions according to linear scaling:
      mTickStep = mRange.size()/(double)(mAutoTickCount+1e-10); // mAutoTickCount ticks on average, the small addition is to prevent jitter on exact integers
      double magnitudeFactor = qPow(10.0, qFloor(qLn(mTickStep)/qLn(10.0))); // get magnitude factor e.g. 0.01, 1, 10, 1000 etc.
      double tickStepMantissa = mTickStep/magnitudeFactor;
      if (tickStepMantissa < 5)
      {
        // round digit after decimal point to 0.5
        mTickStep = (int)(tickStepMantissa*2)/2.0*magnitudeFactor;
      } else
      {
        // round to first digit in multiples of 2
        mTickStep = (int)(tickStepMantissa/2.0)*2.0*magnitudeFactor;
      }
    }
    if (mAutoSubTicks)
      mSubTickCount = calculateAutoSubTickCount(mTickStep);
    // Generate tick positions according to mTickStep:
    qint64 firstStep = floor(mRange.lower/mTickStep);
    qint64 lastStep = ceil(mRange.upper/mTickStep);
    int tickcount = lastStep-firstStep+1;
    if (tickcount < 0) tickcount = 0;
    mTickVector.resize(tickcount);
    for (int i=0; i<tickcount; ++i)
      mTickVector[i] = (firstStep+i)*mTickStep;
  } else // mScaleType == stLogarithmic
  {
    // Generate tick positions according to logbase scaling:
    if (mRange.lower > 0 && mRange.upper > 0) // positive range
    {
      double lowerMag = basePow((int)floor(baseLog(mRange.lower)));
      double currentMag = lowerMag;
      mTickVector.clear();
      mTickVector.append(currentMag);
      while (currentMag < mRange.upper && currentMag > 0) // currentMag might be zero for ranges ~1e-300, just cancel in that case
      {
        currentMag *= mScaleLogBase;
        mTickVector.append(currentMag);
      }
    } else if (mRange.lower < 0 && mRange.upper < 0) // negative range
    {
      double lowerMag = -basePow((int)ceil(baseLog(-mRange.lower)));
      double currentMag = lowerMag;
      mTickVector.clear();
      mTickVector.append(currentMag);
      while (currentMag < mRange.upper && currentMag < 0) // currentMag might be zero for ranges ~1e-300, just cancel in that case
      {
        currentMag /= mScaleLogBase;
        mTickVector.append(currentMag);
      }
    } else // invalid range for logarithmic scale, because lower and upper have different sign
    {
      mTickVector.clear();
      qDebug() << Q_FUNC_INFO << "Invalid range for logarithmic plot: " << mRange.lower << "-" << mRange.upper;
    }
  }
}

/*! \internal
  
  Called by generateAutoTicks when \ref setAutoSubTicks is set to true. Depending on the \a
  tickStep between two major ticks on the axis, a different number of sub ticks is appropriate. For
  Example taking 4 sub ticks for a \a tickStep of 1 makes more sense than taking 5 sub ticks,
  because this corresponds to a sub tick step of 0.2, instead of the less intuitive 0.16667. Note
  that a subtick count of 4 means dividing the major tick step into 5 sections.
  
  This is implemented by a hand made lookup for integer tick steps as well as fractional tick steps
  with a fractional part of (approximately) 0.5. If a tick step is different (i.e. has no
  fractional part close to 0.5), the currently set sub tick count (\ref setSubTickCount) is
  returned.
*/
int QCPAxis::calculateAutoSubTickCount(double tickStep) const
{
  int result = mSubTickCount; // default to current setting, if no proper value can be found
  
  // get mantissa of tickstep:
  double magnitudeFactor = qPow(10.0, qFloor(qLn(tickStep)/qLn(10.0))); // get magnitude factor e.g. 0.01, 1, 10, 1000 etc.
  double tickStepMantissa = tickStep/magnitudeFactor;
  
  // separate integer and fractional part of mantissa:
  double epsilon = 0.01;
  double intPartf;
  int intPart;
  double fracPart = modf(tickStepMantissa, &intPartf);
  intPart = intPartf;
  
  // handle cases with (almost) integer mantissa:
  if (fracPart < epsilon || 1.0-fracPart < epsilon)
  {
    if (1.0-fracPart < epsilon)
      ++intPart;
    switch (intPart)
    {
      case 1: result = 4; break; // 1.0 -> 0.2 substep
      case 2: result = 3; break; // 2.0 -> 0.5 substep
      case 3: result = 2; break; // 3.0 -> 1.0 substep
      case 4: result = 3; break; // 4.0 -> 1.0 substep
      case 5: result = 4; break; // 5.0 -> 1.0 substep
      case 6: result = 2; break; // 6.0 -> 2.0 substep
      case 7: result = 6; break; // 7.0 -> 1.0 substep
      case 8: result = 3; break; // 8.0 -> 2.0 substep
      case 9: result = 2; break; // 9.0 -> 3.0 substep
    }
  } else
  {
    // handle cases with significantly fractional mantissa:
    if (qAbs(fracPart-0.5) < epsilon) // *.5 mantissa
    {
      switch (intPart)
      {
        case 1: result = 2; break; // 1.5 -> 0.5 substep
        case 2: result = 4; break; // 2.5 -> 0.5 substep
        case 3: result = 4; break; // 3.5 -> 0.7 substep
        case 4: result = 2; break; // 4.5 -> 1.5 substep
        case 5: result = 4; break; // 5.5 -> 1.1 substep (won't occur with autoTickStep from here on)
        case 6: result = 4; break; // 6.5 -> 1.3 substep
        case 7: result = 2; break; // 7.5 -> 2.5 substep
        case 8: result = 4; break; // 8.5 -> 1.7 substep
        case 9: result = 4; break; // 9.5 -> 1.9 substep
      }
    }
    // if mantissa fraction isnt 0.0 or 0.5, don't bother finding good sub tick marks, leave default
  }
  
  return result;
}

/* inherits documentation from base class */
void QCPAxis::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
  Q_UNUSED(event)
  SelectablePart part = details.value<SelectablePart>();
  if (mSelectableParts.testFlag(part))
  {
    SelectableParts selBefore = mSelectedParts;
    setSelectedParts(additive ? mSelectedParts^part : part);
    if (selectionStateChanged)
      *selectionStateChanged = mSelectedParts != selBefore;
  }
}

/* inherits documentation from base class */
void QCPAxis::deselectEvent(bool *selectionStateChanged)
{
  SelectableParts selBefore = mSelectedParts;
  setSelectedParts(mSelectedParts & ~mSelectableParts);
  if (selectionStateChanged)
    *selectionStateChanged = mSelectedParts != selBefore;
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing axis lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased
*/
void QCPAxis::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiased, QCP::aeAxes);
}

/*! \internal
  
  Draws the axis with the specified \a painter, using the internal QCPAxisPainterPrivate instance.

*/
void QCPAxis::draw(QCPPainter *painter)
{
  const int lowTick = mLowestVisibleTick;
  const int highTick = mHighestVisibleTick;
  QVector<double> subTickPositions; // the final coordToPixel transformed vector passed to QCPAxisPainter
  QVector<double> tickPositions; // the final coordToPixel transformed vector passed to QCPAxisPainter
  QVector<QString> tickLabels; // the final vector passed to QCPAxisPainter
  tickPositions.reserve(highTick-lowTick+1);
  tickLabels.reserve(highTick-lowTick+1);
  subTickPositions.reserve(mSubTickVector.size());
  
  if (mTicks)
  {
    for (int i=lowTick; i<=highTick; ++i)
    {
      tickPositions.append(coordToPixel(mTickVector.at(i)));
      if (mTickLabels)
        tickLabels.append(mTickVectorLabels.at(i));
    }
    
    if (mSubTickCount > 0)
    {
      const int subTickCount = mSubTickVector.size();
      for (int i=0; i<subTickCount; ++i) // no need to check bounds because subticks are always only created inside current mRange
        subTickPositions.append(coordToPixel(mSubTickVector.at(i)));
    }
  }
  // transfer all properties of this axis to QCPAxisPainterPrivate which it needs to draw the axis.
  // Note that some axis painter properties are already set by direct feed-through with QCPAxis setters
  mAxisPainter->type = mAxisType;
  mAxisPainter->basePen = getBasePen();
  mAxisPainter->labelFont = getLabelFont();
  mAxisPainter->labelColor = getLabelColor();
  mAxisPainter->label = mLabel;
  mAxisPainter->substituteExponent = mAutoTickLabels && mNumberBeautifulPowers && mTickLabelType == ltNumber;
  mAxisPainter->tickPen = getTickPen();
  mAxisPainter->subTickPen = getSubTickPen();
  mAxisPainter->tickLabelFont = getTickLabelFont();
  mAxisPainter->tickLabelColor = getTickLabelColor();
  mAxisPainter->alignmentRect = mAxisRect->rect();
  mAxisPainter->viewportRect = mParentPlot->viewport();
  mAxisPainter->abbreviateDecimalPowers = mScaleType == stLogarithmic;
  mAxisPainter->reversedEndings = mRangeReversed;
  mAxisPainter->tickPositions = tickPositions;
  mAxisPainter->tickLabels = tickLabels;
  mAxisPainter->subTickPositions = subTickPositions;
  mAxisPainter->draw(painter);
}

/*! \internal
  
  Returns via \a lowIndex and \a highIndex, which ticks in the current tick vector are visible in
  the current range. The return values are indices of the tick vector, not the positions of the
  ticks themselves.
  
  The actual use of this function is when an external tick vector is provided, since it might
  exceed far beyond the currently displayed range, and would cause unnecessary calculations e.g. of
  subticks.
  
  If all ticks are outside the axis range, an inverted range is returned, i.e. highIndex will be
  smaller than lowIndex. There is one case, where this function returns indices that are not really
  visible in the current axis range: When the tick spacing is larger than the axis range size and
  one tick is below the axis range and the next tick is already above the axis range. Because in
  such cases it is usually desirable to know the tick pair, to draw proper subticks.
*/
void QCPAxis::visibleTickBounds(int &lowIndex, int &highIndex) const
{
  bool lowFound = false;
  bool highFound = false;
  lowIndex = 0;
  highIndex = -1;
  
  for (int i=0; i < mTickVector.size(); ++i)
  {
    if (mTickVector.at(i) >= mRange.lower)
    {
      lowFound = true;
      lowIndex = i;
      break;
    }
  }
  for (int i=mTickVector.size()-1; i >= 0; --i)
  {
    if (mTickVector.at(i) <= mRange.upper)
    {
      highFound = true;
      highIndex = i;
      break;
    }
  }
  
  if (!lowFound && highFound)
    lowIndex = highIndex+1;
  else if (lowFound && !highFound)
    highIndex = lowIndex-1;
}

/*! \internal
  
  A log function with the base mScaleLogBase, used mostly for coordinate transforms in logarithmic
  scales with arbitrary log base. Uses the buffered mScaleLogBaseLogInv for faster calculation.
  This is set to <tt>1.0/qLn(mScaleLogBase)</tt> in \ref setScaleLogBase.
  
  \see basePow, setScaleLogBase, setScaleType
*/
double QCPAxis::baseLog(double value) const
{
  return qLn(value)*mScaleLogBaseLogInv;
}

/*! \internal
  
  A power function with the base mScaleLogBase, used mostly for coordinate transforms in
  logarithmic scales with arbitrary log base.
  
  \see baseLog, setScaleLogBase, setScaleType
*/
double QCPAxis::basePow(double value) const
{
  return qPow(mScaleLogBase, value);
}

/*! \internal
  
  Returns the pen that is used to draw the axis base line. Depending on the selection state, this
  is either mSelectedBasePen or mBasePen.
*/
QPen QCPAxis::getBasePen() const
{
  return mSelectedParts.testFlag(spAxis) ? mSelectedBasePen : mBasePen;
}

/*! \internal
  
  Returns the pen that is used to draw the (major) ticks. Depending on the selection state, this
  is either mSelectedTickPen or mTickPen.
*/
QPen QCPAxis::getTickPen() const
{
  return mSelectedParts.testFlag(spAxis) ? mSelectedTickPen : mTickPen;
}

/*! \internal
  
  Returns the pen that is used to draw the subticks. Depending on the selection state, this
  is either mSelectedSubTickPen or mSubTickPen.
*/
QPen QCPAxis::getSubTickPen() const
{
  return mSelectedParts.testFlag(spAxis) ? mSelectedSubTickPen : mSubTickPen;
}

/*! \internal
  
  Returns the font that is used to draw the tick labels. Depending on the selection state, this
  is either mSelectedTickLabelFont or mTickLabelFont.
*/
QFont QCPAxis::getTickLabelFont() const
{
  return mSelectedParts.testFlag(spTickLabels) ? mSelectedTickLabelFont : mTickLabelFont;
}

/*! \internal
  
  Returns the font that is used to draw the axis label. Depending on the selection state, this
  is either mSelectedLabelFont or mLabelFont.
*/
QFont QCPAxis::getLabelFont() const
{
  return mSelectedParts.testFlag(spAxisLabel) ? mSelectedLabelFont : mLabelFont;
}

/*! \internal
  
  Returns the color that is used to draw the tick labels. Depending on the selection state, this
  is either mSelectedTickLabelColor or mTickLabelColor.
*/
QColor QCPAxis::getTickLabelColor() const
{
  return mSelectedParts.testFlag(spTickLabels) ? mSelectedTickLabelColor : mTickLabelColor;
}

/*! \internal
  
  Returns the color that is used to draw the axis label. Depending on the selection state, this
  is either mSelectedLabelColor or mLabelColor.
*/
QColor QCPAxis::getLabelColor() const
{
  return mSelectedParts.testFlag(spAxisLabel) ? mSelectedLabelColor : mLabelColor;
}

/*! \internal
  
  Returns the appropriate outward margin for this axis. It is needed if \ref
  QCPAxisRect::setAutoMargins is set to true on the parent axis rect. An axis with axis type \ref
  atLeft will return an appropriate left margin, \ref atBottom will return an appropriate bottom
  margin and so forth. For the calculation, this function goes through similar steps as \ref draw,
  so changing one function likely requires the modification of the other one as well.
  
  The margin consists of the outward tick length, tick label padding, tick label size, label
  padding, label size, and padding.
  
  The margin is cached internally, so repeated calls while leaving the axis range, fonts, etc.
  unchanged are very fast.
*/
int QCPAxis::calculateMargin()
{
  if (!mVisible) // if not visible, directly return 0, don't cache 0 because we can't react to setVisible in QCPAxis
    return 0;
  
  if (mCachedMarginValid)
    return mCachedMargin;
  
  // run through similar steps as QCPAxis::draw, and caluclate margin needed to fit axis and its labels
  int margin = 0;
  
  int lowTick, highTick;
  visibleTickBounds(lowTick, highTick);
  QVector<double> tickPositions; // the final coordToPixel transformed vector passed to QCPAxisPainter
  QVector<QString> tickLabels; // the final vector passed to QCPAxisPainter
  tickPositions.reserve(highTick-lowTick+1);
  tickLabels.reserve(highTick-lowTick+1);
  if (mTicks)
  {
    for (int i=lowTick; i<=highTick; ++i)
    {
      tickPositions.append(coordToPixel(mTickVector.at(i)));
      if (mTickLabels)
        tickLabels.append(mTickVectorLabels.at(i));
    }
  }
  // transfer all properties of this axis to QCPAxisPainterPrivate which it needs to calculate the size.
  // Note that some axis painter properties are already set by direct feed-through with QCPAxis setters
  mAxisPainter->type = mAxisType;
  mAxisPainter->labelFont = getLabelFont();
  mAxisPainter->label = mLabel;
  mAxisPainter->tickLabelFont = mTickLabelFont;
  mAxisPainter->alignmentRect = mAxisRect->rect();
  mAxisPainter->viewportRect = mParentPlot->viewport();
  mAxisPainter->tickPositions = tickPositions;
  mAxisPainter->tickLabels = tickLabels;
  margin += mAxisPainter->size();
  margin += mPadding;

  mCachedMargin = margin;
  mCachedMarginValid = true;
  return margin;
}

/* inherits documentation from base class */
QCP::Interaction QCPAxis::selectionCategory() const
{
  return QCP::iSelectAxes;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxisPainterPrivate
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAxisPainterPrivate

  \internal
  \brief (Private)
  
  This is a private class and not part of the public QCustomPlot interface.
  
  It is used by QCPAxis to do the low-level drawing of axis backbone, tick marks, tick labels and
  axis label. It also buffers the labels to reduce replot times. The parameters are configured by
  directly accessing the public member variables.
*/

/*!
  Constructs a QCPAxisPainterPrivate instance. Make sure to not create a new instance on every
  redraw, to utilize the caching mechanisms.
*/
QCPAxisPainterPrivate::QCPAxisPainterPrivate(QCustomPlot *parentPlot) :
  type(QCPAxis::atLeft),
  basePen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
  lowerEnding(QCPLineEnding::esNone),
  upperEnding(QCPLineEnding::esNone),
  labelPadding(0),
  tickLabelPadding(0),
  tickLabelRotation(0),
  substituteExponent(true),
  numberMultiplyCross(false),
  tickLengthIn(5),
  tickLengthOut(0),
  subTickLengthIn(2),
  subTickLengthOut(0),
  tickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
  subTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
  offset(0),
  abbreviateDecimalPowers(false),
  reversedEndings(false),
  mParentPlot(parentPlot),
  mLabelCache(16) // cache at most 16 (tick) labels
{
}

QCPAxisPainterPrivate::~QCPAxisPainterPrivate()
{
}

/*! \internal
  
  Draws the axis with the specified \a painter.
  
  The selection boxes (mAxisSelectionBox, mTickLabelsSelectionBox, mLabelSelectionBox) are set
  here, too.
*/
void QCPAxisPainterPrivate::draw(QCPPainter *painter)
{
  QByteArray newHash = generateLabelParameterHash();
  if (newHash != mLabelParameterHash)
  {
    mLabelCache.clear();
    mLabelParameterHash = newHash;
  }
  
  QPoint origin;
  switch (type)
  {
    case QCPAxis::atLeft:   origin = alignmentRect.bottomLeft() +QPoint(-offset, 0); break;
    case QCPAxis::atRight:  origin = alignmentRect.bottomRight()+QPoint(+offset, 0); break;
    case QCPAxis::atTop:    origin = alignmentRect.topLeft()    +QPoint(0, -offset); break;
    case QCPAxis::atBottom: origin = alignmentRect.bottomLeft() +QPoint(0, +offset); break;
  }

  double xCor = 0, yCor = 0; // paint system correction, for pixel exact matches (affects baselines and ticks of top/right axes)
  switch (type)
  {
    case QCPAxis::atTop: yCor = -1; break;
    case QCPAxis::atRight: xCor = 1; break;
    default: break;
  }

  int margin = 0;
  // draw baseline:
  QLineF baseLine;
  painter->setPen(basePen);
  if (QCPAxis::orientation(type) == Qt::Horizontal)
    baseLine.setPoints(origin+QPointF(xCor, yCor), origin+QPointF(alignmentRect.width()+xCor, yCor));
  else
    baseLine.setPoints(origin+QPointF(xCor, yCor), origin+QPointF(xCor, -alignmentRect.height()+yCor));
  if (reversedEndings)
    baseLine = QLineF(baseLine.p2(), baseLine.p1()); // won't make a difference for line itself, but for line endings later
  painter->drawLine(baseLine);
  
  // draw ticks:
  if (!tickPositions.isEmpty())
  {
    painter->setPen(tickPen);
    int tickDir = (type == QCPAxis::atBottom || type == QCPAxis::atRight) ? -1 : 1; // direction of ticks ("inward" is right for left axis and left for right axis)
    if (QCPAxis::orientation(type) == Qt::Horizontal)
    {
      for (int i=0; i<tickPositions.size(); ++i)
        painter->drawLine(QLineF(tickPositions.at(i)+xCor, origin.y()-tickLengthOut*tickDir+yCor, tickPositions.at(i)+xCor, origin.y()+tickLengthIn*tickDir+yCor));
    } else
    {
      for (int i=0; i<tickPositions.size(); ++i)
        painter->drawLine(QLineF(origin.x()-tickLengthOut*tickDir+xCor, tickPositions.at(i)+yCor, origin.x()+tickLengthIn*tickDir+xCor, tickPositions.at(i)+yCor));
    }
  }
  
  // draw subticks:
  if (!subTickPositions.isEmpty())
  {
    painter->setPen(subTickPen);
    // direction of ticks ("inward" is right for left axis and left for right axis)
    int tickDir = (type == QCPAxis::atBottom || type == QCPAxis::atRight) ? -1 : 1;
    if (QCPAxis::orientation(type) == Qt::Horizontal)
    {
      for (int i=0; i<subTickPositions.size(); ++i)
        painter->drawLine(QLineF(subTickPositions.at(i)+xCor, origin.y()-subTickLengthOut*tickDir+yCor, subTickPositions.at(i)+xCor, origin.y()+subTickLengthIn*tickDir+yCor));
    } else
    {
      for (int i=0; i<subTickPositions.size(); ++i)
        painter->drawLine(QLineF(origin.x()-subTickLengthOut*tickDir+xCor, subTickPositions.at(i)+yCor, origin.x()+subTickLengthIn*tickDir+xCor, subTickPositions.at(i)+yCor));
    }
  }
  margin += qMax(0, qMax(tickLengthOut, subTickLengthOut));
  
  // draw axis base endings:
  bool antialiasingBackup = painter->antialiasing();
  painter->setAntialiasing(true); // always want endings to be antialiased, even if base and ticks themselves aren't
  painter->setBrush(QBrush(basePen.color()));
  QVector2D baseLineVector(baseLine.dx(), baseLine.dy());
  if (lowerEnding.style() != QCPLineEnding::esNone)
    lowerEnding.draw(painter, QVector2D(baseLine.p1())-baseLineVector.normalized()*lowerEnding.realLength()*(lowerEnding.inverted()?-1:1), -baseLineVector);
  if (upperEnding.style() != QCPLineEnding::esNone)
    upperEnding.draw(painter, QVector2D(baseLine.p2())+baseLineVector.normalized()*upperEnding.realLength()*(upperEnding.inverted()?-1:1), baseLineVector);
  painter->setAntialiasing(antialiasingBackup);
  
  // tick labels:
  QSize tickLabelsSize(0, 0); // size of largest tick label, for offset calculation of axis label
  if (!tickLabels.isEmpty())
  {
    margin += tickLabelPadding;
    painter->setFont(tickLabelFont);
    painter->setPen(QPen(tickLabelColor));
    const int maxLabelIndex = qMin(tickPositions.size(), tickLabels.size());
    for (int i=0; i<maxLabelIndex; ++i)
      placeTickLabel(painter, tickPositions.at(i), margin, tickLabels.at(i), &tickLabelsSize);
    if (QCPAxis::orientation(type) == Qt::Horizontal)
      margin += tickLabelsSize.height();
    else
      margin += tickLabelsSize.width();
  }

  // axis label:
  QRect labelBounds;
  if (!label.isEmpty())
  {
    margin += labelPadding;
    painter->setFont(labelFont);
    painter->setPen(QPen(labelColor));
    labelBounds = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip, label);
    if (type == QCPAxis::atLeft)
    {
      QTransform oldTransform = painter->transform();
      painter->translate((origin.x()-margin-labelBounds.height()), origin.y());
      painter->rotate(-90);
      painter->drawText(0, 0, alignmentRect.height(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
      painter->setTransform(oldTransform);
    }
    else if (type == QCPAxis::atRight)
    {
      QTransform oldTransform = painter->transform();
      painter->translate((origin.x()+margin+labelBounds.height()), origin.y()-alignmentRect.height());
      painter->rotate(90);
      painter->drawText(0, 0, alignmentRect.height(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
      painter->setTransform(oldTransform);
    }
    else if (type == QCPAxis::atTop)
      painter->drawText(origin.x(), origin.y()-margin-labelBounds.height(), alignmentRect.width(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
    else if (type == QCPAxis::atBottom)
      painter->drawText(origin.x(), origin.y()+margin, alignmentRect.width(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
  }
  
  // set selection boxes:
  int selectionTolerance = 0;
  if (mParentPlot)
    selectionTolerance = mParentPlot->selectionTolerance();
  else
    qDebug() << Q_FUNC_INFO << "mParentPlot is null";
  int selAxisOutSize = qMax(qMax(tickLengthOut, subTickLengthOut), selectionTolerance);
  int selAxisInSize = selectionTolerance;
  int selTickLabelSize = (QCPAxis::orientation(type) == Qt::Horizontal ? tickLabelsSize.height() : tickLabelsSize.width());
  int selTickLabelOffset = qMax(tickLengthOut, subTickLengthOut)+tickLabelPadding;
  int selLabelSize = labelBounds.height();
  int selLabelOffset = selTickLabelOffset+selTickLabelSize+labelPadding;
  if (type == QCPAxis::atLeft)
  {
    mAxisSelectionBox.setCoords(origin.x()-selAxisOutSize, alignmentRect.top(), origin.x()+selAxisInSize, alignmentRect.bottom());
    mTickLabelsSelectionBox.setCoords(origin.x()-selTickLabelOffset-selTickLabelSize, alignmentRect.top(), origin.x()-selTickLabelOffset, alignmentRect.bottom());
    mLabelSelectionBox.setCoords(origin.x()-selLabelOffset-selLabelSize, alignmentRect.top(), origin.x()-selLabelOffset, alignmentRect.bottom());
  } else if (type == QCPAxis::atRight)
  {
    mAxisSelectionBox.setCoords(origin.x()-selAxisInSize, alignmentRect.top(), origin.x()+selAxisOutSize, alignmentRect.bottom());
    mTickLabelsSelectionBox.setCoords(origin.x()+selTickLabelOffset+selTickLabelSize, alignmentRect.top(), origin.x()+selTickLabelOffset, alignmentRect.bottom());
    mLabelSelectionBox.setCoords(origin.x()+selLabelOffset+selLabelSize, alignmentRect.top(), origin.x()+selLabelOffset, alignmentRect.bottom());
  } else if (type == QCPAxis::atTop)
  {
    mAxisSelectionBox.setCoords(alignmentRect.left(), origin.y()-selAxisOutSize, alignmentRect.right(), origin.y()+selAxisInSize);
    mTickLabelsSelectionBox.setCoords(alignmentRect.left(), origin.y()-selTickLabelOffset-selTickLabelSize, alignmentRect.right(), origin.y()-selTickLabelOffset);
    mLabelSelectionBox.setCoords(alignmentRect.left(), origin.y()-selLabelOffset-selLabelSize, alignmentRect.right(), origin.y()-selLabelOffset);
  } else if (type == QCPAxis::atBottom)
  {
    mAxisSelectionBox.setCoords(alignmentRect.left(), origin.y()-selAxisInSize, alignmentRect.right(), origin.y()+selAxisOutSize);
    mTickLabelsSelectionBox.setCoords(alignmentRect.left(), origin.y()+selTickLabelOffset+selTickLabelSize, alignmentRect.right(), origin.y()+selTickLabelOffset);
    mLabelSelectionBox.setCoords(alignmentRect.left(), origin.y()+selLabelOffset+selLabelSize, alignmentRect.right(), origin.y()+selLabelOffset);
  }
  // draw hitboxes for debug purposes:
  //painter->setBrush(Qt::NoBrush);
  //painter->drawRects(QVector<QRect>() << mAxisSelectionBox << mTickLabelsSelectionBox << mLabelSelectionBox);
}

/*! \internal
  
  Returns the size ("margin" in QCPAxisRect context, so measured perpendicular to the axis backbone
  direction) needed to fit the axis.
*/
int QCPAxisPainterPrivate::size() const
{
  int result = 0;
  
  // get length of tick marks pointing outwards:
  if (!tickPositions.isEmpty())
    result += qMax(0, qMax(tickLengthOut, subTickLengthOut));
  
  // calculate size of tick labels:
  QSize tickLabelsSize(0, 0);
  if (!tickLabels.isEmpty())
  {
    for (int i=0; i<tickLabels.size(); ++i)
      getMaxTickLabelSize(tickLabelFont, tickLabels.at(i), &tickLabelsSize);
    result += QCPAxis::orientation(type) == Qt::Horizontal ? tickLabelsSize.height() : tickLabelsSize.width();
    result += tickLabelPadding;
  }
  
  // calculate size of axis label (only height needed, because left/right labels are rotated by 90 degrees):
  if (!label.isEmpty())
  {
    QFontMetrics fontMetrics(labelFont);
    QRect bounds;
    bounds = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip | Qt::AlignHCenter | Qt::AlignVCenter, label);
    result += bounds.height() + labelPadding;
  }
  
  return result;
}

/*! \internal
  
  Clears the internal label cache. Upon the next \ref draw, all labels will be created new. This
  method is called automatically in \ref draw, if any parameters have changed that invalidate the
  cached labels, such as font, color, etc.
*/
void QCPAxisPainterPrivate::clearCache()
{
  mLabelCache.clear();
}

/*! \internal
  
  Returns a hash that allows uniquely identifying whether the label parameters have changed such
  that the cached labels must be refreshed (\ref clearCache). It is used in \ref draw. If the
  return value of this method hasn't changed since the last redraw, the respective label parameters
  haven't changed and cached labels may be used.
*/
QByteArray QCPAxisPainterPrivate::generateLabelParameterHash() const
{
  QByteArray result;
  result.append(QByteArray::number(tickLabelRotation));
  result.append(QByteArray::number((int)substituteExponent));
  result.append(QByteArray::number((int)numberMultiplyCross));
  result.append(tickLabelColor.name()+QByteArray::number(tickLabelColor.alpha(), 16));
  result.append(tickLabelFont.toString());
  return result;
}

/*! \internal
  
  Draws a single tick label with the provided \a painter, utilizing the internal label cache to
  significantly speed up drawing of labels that were drawn in previous calls. The tick label is
  always bound to an axis, the distance to the axis is controllable via \a distanceToAxis in
  pixels. The pixel position in the axis direction is passed in the \a position parameter. Hence
  for the bottom axis, \a position would indicate the horizontal pixel position (not coordinate),
  at which the label should be drawn.
  
  In order to later draw the axis label in a place that doesn't overlap with the tick labels, the
  largest tick label size is needed. This is acquired by passing a \a tickLabelsSize to the \ref
  drawTickLabel calls during the process of drawing all tick labels of one axis. In every call, \a
  tickLabelsSize is expanded, if the drawn label exceeds the value \a tickLabelsSize currently
  holds.
  
  The label is drawn with the font and pen that are currently set on the \a painter. To draw
  superscripted powers, the font is temporarily made smaller by a fixed factor (see \ref
  getTickLabelData).
*/
void QCPAxisPainterPrivate::placeTickLabel(QCPPainter *painter, double position, int distanceToAxis, const QString &text, QSize *tickLabelsSize)
{
  // warning: if you change anything here, also adapt getMaxTickLabelSize() accordingly!
  if (text.isEmpty()) return;
  QSize finalSize;
  QPointF labelAnchor;
  switch (type)
  {
    case QCPAxis::atLeft:   labelAnchor = QPointF(alignmentRect.left()-distanceToAxis-offset, position); break;
    case QCPAxis::atRight:  labelAnchor = QPointF(alignmentRect.right()+distanceToAxis+offset, position); break;
    case QCPAxis::atTop:    labelAnchor = QPointF(position, alignmentRect.top()-distanceToAxis-offset); break;
    case QCPAxis::atBottom: labelAnchor = QPointF(position, alignmentRect.bottom()+distanceToAxis+offset); break;
  }
  if (mParentPlot->plottingHints().testFlag(QCP::phCacheLabels) && !painter->modes().testFlag(QCPPainter::pmNoCaching)) // label caching enabled
  {
    if (!mLabelCache.contains(text))  // no cached label exists, create it
    {
      CachedLabel *newCachedLabel = new CachedLabel;
      TickLabelData labelData = getTickLabelData(painter->font(), text);
      QPointF drawOffset = getTickLabelDrawOffset(labelData);
      newCachedLabel->offset = drawOffset+labelData.rotatedTotalBounds.topLeft();
      newCachedLabel->pixmap = QPixmap(labelData.rotatedTotalBounds.size());
      newCachedLabel->pixmap.fill(Qt::transparent);
      QCPPainter cachePainter(&newCachedLabel->pixmap);
      cachePainter.setPen(painter->pen());
      drawTickLabel(&cachePainter, -labelData.rotatedTotalBounds.topLeft().x(), -labelData.rotatedTotalBounds.topLeft().y(), labelData);
      mLabelCache.insert(text, newCachedLabel, 1);
    }
    // draw cached label:
    const CachedLabel *cachedLabel = mLabelCache.object(text);
    // if label would be partly clipped by widget border on sides, don't draw it:
    if (QCPAxis::orientation(type) == Qt::Horizontal)
    {
      if (labelAnchor.x()+cachedLabel->offset.x()+cachedLabel->pixmap.width() > viewportRect.right() ||
          labelAnchor.x()+cachedLabel->offset.x() < viewportRect.left())
        return;
    } else
    {
      if (labelAnchor.y()+cachedLabel->offset.y()+cachedLabel->pixmap.height() >viewportRect.bottom() ||
          labelAnchor.y()+cachedLabel->offset.y() < viewportRect.top())
        return;
    }
    painter->drawPixmap(labelAnchor+cachedLabel->offset, cachedLabel->pixmap);
    finalSize = cachedLabel->pixmap.size();
  } else // label caching disabled, draw text directly on surface:
  {
    TickLabelData labelData = getTickLabelData(painter->font(), text);
    QPointF finalPosition = labelAnchor + getTickLabelDrawOffset(labelData);
    // if label would be partly clipped by widget border on sides, don't draw it:
    if (QCPAxis::orientation(type) == Qt::Horizontal)
    {
      if (finalPosition.x()+(labelData.rotatedTotalBounds.width()+labelData.rotatedTotalBounds.left()) > viewportRect.right() ||
          finalPosition.x()+labelData.rotatedTotalBounds.left() < viewportRect.left())
        return;
    } else
    {
      if (finalPosition.y()+(labelData.rotatedTotalBounds.height()+labelData.rotatedTotalBounds.top()) > viewportRect.bottom() ||
          finalPosition.y()+labelData.rotatedTotalBounds.top() < viewportRect.top())
        return;
    }
    drawTickLabel(painter, finalPosition.x(), finalPosition.y(), labelData);
    finalSize = labelData.rotatedTotalBounds.size();
  }
  
  // expand passed tickLabelsSize if current tick label is larger:
  if (finalSize.width() > tickLabelsSize->width())
    tickLabelsSize->setWidth(finalSize.width());
  if (finalSize.height() > tickLabelsSize->height())
    tickLabelsSize->setHeight(finalSize.height());
}

/*! \internal
  
  This is a \ref placeTickLabel helper function.
  
  Draws the tick label specified in \a labelData with \a painter at the pixel positions \a x and \a
  y. This function is used by \ref placeTickLabel to create new tick labels for the cache, or to
  directly draw the labels on the QCustomPlot surface when label caching is disabled, i.e. when
  QCP::phCacheLabels plotting hint is not set.
*/
void QCPAxisPainterPrivate::drawTickLabel(QCPPainter *painter, double x, double y, const TickLabelData &labelData) const
{
  // backup painter settings that we're about to change:
  QTransform oldTransform = painter->transform();
  QFont oldFont = painter->font();
  
  // transform painter to position/rotation:
  painter->translate(x, y);
  if (!qFuzzyIsNull(tickLabelRotation))
    painter->rotate(tickLabelRotation);
  
  // draw text:
  if (!labelData.expPart.isEmpty()) // indicator that beautiful powers must be used
  {
    painter->setFont(labelData.baseFont);
    painter->drawText(0, 0, 0, 0, Qt::TextDontClip, labelData.basePart);
    painter->setFont(labelData.expFont);
    painter->drawText(labelData.baseBounds.width()+1, 0, labelData.expBounds.width(), labelData.expBounds.height(), Qt::TextDontClip,  labelData.expPart);
  } else
  {
    painter->setFont(labelData.baseFont);
    painter->drawText(0, 0, labelData.totalBounds.width(), labelData.totalBounds.height(), Qt::TextDontClip | Qt::AlignHCenter, labelData.basePart);
  }
  
  // reset painter settings to what it was before:
  painter->setTransform(oldTransform);
  painter->setFont(oldFont);
}

/*! \internal
  
  This is a \ref placeTickLabel helper function.
  
  Transforms the passed \a text and \a font to a tickLabelData structure that can then be further
  processed by \ref getTickLabelDrawOffset and \ref drawTickLabel. It splits the text into base and
  exponent if necessary (member substituteExponent) and calculates appropriate bounding boxes.
*/
QCPAxisPainterPrivate::TickLabelData QCPAxisPainterPrivate::getTickLabelData(const QFont &font, const QString &text) const
{
  TickLabelData result;
  
  // determine whether beautiful decimal powers should be used
  bool useBeautifulPowers = false;
  int ePos = -1;
  if (substituteExponent)
  {
    ePos = text.indexOf('e');
    if (ePos > -1)
      useBeautifulPowers = true;
  }
  
  // calculate text bounding rects and do string preparation for beautiful decimal powers:
  result.baseFont = font;
  if (result.baseFont.pointSizeF() > 0) // On some rare systems, this sometimes is initialized with -1 (Qt bug?), so we check here before possibly setting a negative value in the next line
    result.baseFont.setPointSizeF(result.baseFont.pointSizeF()+0.05); // QFontMetrics.boundingRect has a bug for exact point sizes that make the results oscillate due to internal rounding
  if (useBeautifulPowers)
  {
    // split text into parts of number/symbol that will be drawn normally and part that will be drawn as exponent:
    result.basePart = text.left(ePos);
    // in log scaling, we want to turn "1*10^n" into "10^n", else add multiplication sign and decimal base:
    if (abbreviateDecimalPowers && result.basePart == "1")
      result.basePart = "10";
    else
      result.basePart += (numberMultiplyCross ? QString(QChar(215)) : QString(QChar(183))) + "10";
    result.expPart = text.mid(ePos+1);
    // clip "+" and leading zeros off expPart:
    while (result.expPart.length() > 2 && result.expPart.at(1) == '0') // length > 2 so we leave one zero when numberFormatChar is 'e'
      result.expPart.remove(1, 1);
    if (!result.expPart.isEmpty() && result.expPart.at(0) == '+')
      result.expPart.remove(0, 1);
    // prepare smaller font for exponent:
    result.expFont = font;
    result.expFont.setPointSize(result.expFont.pointSize()*0.75);
    // calculate bounding rects of base part, exponent part and total one:
    result.baseBounds = QFontMetrics(result.baseFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip, result.basePart);
    result.expBounds = QFontMetrics(result.expFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip, result.expPart);
    result.totalBounds = result.baseBounds.adjusted(0, 0, result.expBounds.width()+2, 0); // +2 consists of the 1 pixel spacing between base and exponent (see drawTickLabel) and an extra pixel to include AA
  } else // useBeautifulPowers == false
  {
    result.basePart = text;
    result.totalBounds = QFontMetrics(result.baseFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip | Qt::AlignHCenter, result.basePart);
  }
  result.totalBounds.moveTopLeft(QPoint(0, 0)); // want bounding box aligned top left at origin, independent of how it was created, to make further processing simpler
  
  // calculate possibly different bounding rect after rotation:
  result.rotatedTotalBounds = result.totalBounds;
  if (!qFuzzyIsNull(tickLabelRotation))
  {
    QTransform transform;
    transform.rotate(tickLabelRotation);
    result.rotatedTotalBounds = transform.mapRect(result.rotatedTotalBounds);
  }
  
  return result;
}

/*! \internal
  
  This is a \ref placeTickLabel helper function.
  
  Calculates the offset at which the top left corner of the specified tick label shall be drawn.
  The offset is relative to a point right next to the tick the label belongs to.
  
  This function is thus responsible for e.g. centering tick labels under ticks and positioning them
  appropriately when they are rotated.
*/
QPointF QCPAxisPainterPrivate::getTickLabelDrawOffset(const TickLabelData &labelData) const
{
  /*
    calculate label offset from base point at tick (non-trivial, for best visual appearance): short
    explanation for bottom axis: The anchor, i.e. the point in the label that is placed
    horizontally under the corresponding tick is always on the label side that is closer to the
    axis (e.g. the left side of the text when we're rotating clockwise). On that side, the height
    is halved and the resulting point is defined the anchor. This way, a 90 degree rotated text
    will be centered under the tick (i.e. displaced horizontally by half its height). At the same
    time, a 45 degree rotated text will "point toward" its tick, as is typical for rotated tick
    labels.
  */
  bool doRotation = !qFuzzyIsNull(tickLabelRotation);
  bool flip = qFuzzyCompare(qAbs(tickLabelRotation), 90.0); // perfect +/-90 degree flip. Indicates vertical label centering on vertical axes.
  double radians = tickLabelRotation/180.0*M_PI;
  int x=0, y=0;
  if (type == QCPAxis::atLeft)
  {
    if (doRotation)
    {
      if (tickLabelRotation > 0)
      {
        x = -qCos(radians)*labelData.totalBounds.width();
        y = flip ? -labelData.totalBounds.width()/2.0 : -qSin(radians)*labelData.totalBounds.width()-qCos(radians)*labelData.totalBounds.height()/2.0;
      } else
      {
        x = -qCos(-radians)*labelData.totalBounds.width()-qSin(-radians)*labelData.totalBounds.height();
        y = flip ? +labelData.totalBounds.width()/2.0 : +qSin(-radians)*labelData.totalBounds.width()-qCos(-radians)*labelData.totalBounds.height()/2.0;
      }
    } else
    {
      x = -labelData.totalBounds.width();
      y = -labelData.totalBounds.height()/2.0;
    }
  } else if (type == QCPAxis::atRight)
  {
    if (doRotation)
    {
      if (tickLabelRotation > 0)
      {
        x = +qSin(radians)*labelData.totalBounds.height();
        y = flip ? -labelData.totalBounds.width()/2.0 : -qCos(radians)*labelData.totalBounds.height()/2.0;
      } else
      {
        x = 0;
        y = flip ? +labelData.totalBounds.width()/2.0 : -qCos(-radians)*labelData.totalBounds.height()/2.0;
      }
    } else
    {
      x = 0;
      y = -labelData.totalBounds.height()/2.0;
    }
  } else if (type == QCPAxis::atTop)
  {
    if (doRotation)
    {
      if (tickLabelRotation > 0)
      {
        x = -qCos(radians)*labelData.totalBounds.width()+qSin(radians)*labelData.totalBounds.height()/2.0;
        y = -qSin(radians)*labelData.totalBounds.width()-qCos(radians)*labelData.totalBounds.height();
      } else
      {
        x = -qSin(-radians)*labelData.totalBounds.height()/2.0;
        y = -qCos(-radians)*labelData.totalBounds.height();
      }
    } else
    {
      x = -labelData.totalBounds.width()/2.0;
      y = -labelData.totalBounds.height();
    }
  } else if (type == QCPAxis::atBottom)
  {
    if (doRotation)
    {
      if (tickLabelRotation > 0)
      {
        x = +qSin(radians)*labelData.totalBounds.height()/2.0;
        y = 0;
      } else
      {
        x = -qCos(-radians)*labelData.totalBounds.width()-qSin(-radians)*labelData.totalBounds.height()/2.0;
        y = +qSin(-radians)*labelData.totalBounds.width();
      }
    } else
    {
      x = -labelData.totalBounds.width()/2.0;
      y = 0;
    }
  }
  
  return QPointF(x, y);
}

/*! \internal
  
  Simulates the steps done by \ref placeTickLabel by calculating bounding boxes of the text label
  to be drawn, depending on number format etc. Since only the largest tick label is wanted for the
  margin calculation, the passed \a tickLabelsSize is only expanded, if it's currently set to a
  smaller width/height.
*/
void QCPAxisPainterPrivate::getMaxTickLabelSize(const QFont &font, const QString &text,  QSize *tickLabelsSize) const
{
  // note: this function must return the same tick label sizes as the placeTickLabel function.
  QSize finalSize;
  if (mParentPlot->plottingHints().testFlag(QCP::phCacheLabels) && mLabelCache.contains(text)) // label caching enabled and have cached label
  {
    const CachedLabel *cachedLabel = mLabelCache.object(text);
    finalSize = cachedLabel->pixmap.size();
  } else // label caching disabled or no label with this text cached:
  {
    TickLabelData labelData = getTickLabelData(font, text);
    finalSize = labelData.rotatedTotalBounds.size();
  }
  
  // expand passed tickLabelsSize if current tick label is larger:
  if (finalSize.width() > tickLabelsSize->width())
    tickLabelsSize->setWidth(finalSize.width());
  if (finalSize.height() > tickLabelsSize->height())
    tickLabelsSize->setHeight(finalSize.height());
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAbstractPlottable
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAbstractPlottable
  \brief The abstract base class for all data representing objects in a plot.

  It defines a very basic interface like name, pen, brush, visibility etc. Since this class is
  abstract, it can't be instantiated. Use one of the subclasses or create a subclass yourself to
  create new ways of displaying data (see "Creating own plottables" below).
  
  All further specifics are in the subclasses, for example:
  \li A normal graph with possibly a line, scatter points and error bars is displayed by \ref QCPGraph
  (typically created with \ref QCustomPlot::addGraph).
  \li A parametric curve can be displayed with \ref QCPCurve.
  \li A stackable bar chart can be achieved with \ref QCPBars.
  \li A box of a statistical box plot is created with \ref QCPStatisticalBox.
  
  \section plottables-subclassing Creating own plottables
  
  To create an own plottable, you implement a subclass of QCPAbstractPlottable. These are the pure
  virtual functions, you must implement:
  \li \ref clearData
  \li \ref selectTest
  \li \ref draw
  \li \ref drawLegendIcon
  \li \ref getKeyRange
  \li \ref getValueRange
  
  See the documentation of those functions for what they need to do.
  
  For drawing your plot, you can use the \ref coordsToPixels functions to translate a point in plot
  coordinates to pixel coordinates. This function is quite convenient, because it takes the
  orientation of the key and value axes into account for you (x and y are swapped when the key axis
  is vertical and the value axis horizontal). If you are worried about performance (i.e. you need
  to translate many points in a loop like QCPGraph), you can directly use \ref
  QCPAxis::coordToPixel. However, you must then take care about the orientation of the axis
  yourself.
  
  Here are some important members you inherit from QCPAbstractPlottable:
  <table>
  <tr>
    <td>QCustomPlot *\b mParentPlot</td>
    <td>A pointer to the parent QCustomPlot instance. The parent plot is inferred from the axes that are passed in the constructor.</td>
  </tr><tr>
    <td>QString \b mName</td>
    <td>The name of the plottable.</td>
  </tr><tr>
    <td>QPen \b mPen</td>
    <td>The generic pen of the plottable. You should use this pen for the most prominent data representing lines in the plottable (e.g QCPGraph uses this pen for its graph lines and scatters)</td>
  </tr><tr>
    <td>QPen \b mSelectedPen</td>
    <td>The generic pen that should be used when the plottable is selected (hint: \ref mainPen gives you the right pen, depending on selection state).</td>
  </tr><tr>
    <td>QBrush \b mBrush</td>
    <td>The generic brush of the plottable. You should use this brush for the most prominent fillable structures in the plottable (e.g. QCPGraph uses this brush to control filling under the graph)</td>
  </tr><tr>
    <td>QBrush \b mSelectedBrush</td>
    <td>The generic brush that should be used when the plottable is selected (hint: \ref mainBrush gives you the right brush, depending on selection state).</td>
  </tr><tr>
    <td>QPointer<QCPAxis>\b mKeyAxis, \b mValueAxis</td>
    <td>The key and value axes this plottable is attached to. Call their QCPAxis::coordToPixel functions to translate coordinates to pixels in either the key or value dimension.
        Make sure to check whether the pointer is null before using it. If one of the axes is null, don't draw the plottable.</td>
  </tr><tr>
    <td>bool \b mSelected</td>
    <td>indicates whether the plottable is selected or not.</td>
  </tr>
  </table>
*/

/* start of documentation of pure virtual functions */

/*! \fn void QCPAbstractPlottable::clearData() = 0
  Clears all data in the plottable.
*/

/*! \fn void QCPAbstractPlottable::drawLegendIcon(QCPPainter *painter, const QRect &rect) const = 0
  \internal
  
  called by QCPLegend::draw (via QCPPlottableLegendItem::draw) to create a graphical representation
  of this plottable inside \a rect, next to the plottable name.
*/

/*! \fn QCPRange QCPAbstractPlottable::getKeyRange(bool &foundRange, SignDomain inSignDomain) const = 0
  \internal
  
  called by rescaleAxes functions to get the full data key bounds. For logarithmic plots, one can
  set \a inSignDomain to either \ref sdNegative or \ref sdPositive in order to restrict the
  returned range to that sign domain. E.g. when only negative range is wanted, set \a inSignDomain
  to \ref sdNegative and all positive points will be ignored for range calculation. For no
  restriction, just set \a inSignDomain to \ref sdBoth (default). \a foundRange is an output
  parameter that indicates whether a range could be found or not. If this is false, you shouldn't
  use the returned range (e.g. no points in data).

  Note that \a foundRange is not the same as \ref QCPRange::validRange, since the range returned by
  this function may have size zero, which wouldn't count as a valid range.
  
  \see rescaleAxes, getValueRange
*/

/*! \fn QCPRange QCPAbstractPlottable::getValueRange(bool &foundRange, SignDomain inSignDomain) const = 0
  \internal
  
  called by rescaleAxes functions to get the full data value bounds. For logarithmic plots, one can
  set \a inSignDomain to either \ref sdNegative or \ref sdPositive in order to restrict the
  returned range to that sign domain. E.g. when only negative range is wanted, set \a inSignDomain
  to \ref sdNegative and all positive points will be ignored for range calculation. For no
  restriction, just set \a inSignDomain to \ref sdBoth (default). \a foundRange is an output
  parameter that indicates whether a range could be found or not. If this is false, you shouldn't
  use the returned range (e.g. no points in data).

  Note that \a foundRange is not the same as \ref QCPRange::validRange, since the range returned by
  this function may have size zero, which wouldn't count as a valid range.
  
  \see rescaleAxes, getKeyRange
*/

/* end of documentation of pure virtual functions */
/* start of documentation of signals */

/*! \fn void QCPAbstractPlottable::selectionChanged(bool selected)
  
  This signal is emitted when the selection state of this plottable has changed, either by user
  interaction or by a direct call to \ref setSelected.
*/

/*! \fn void QCPAbstractPlottable::selectableChanged(bool selectable);
  
  This signal is emitted when the selectability of this plottable has changed.
  
  \see setSelectable
*/

/* end of documentation of signals */

/*!
  Constructs an abstract plottable which uses \a keyAxis as its key axis ("x") and \a valueAxis as
  its value axis ("y"). \a keyAxis and \a valueAxis must reside in the same QCustomPlot instance
  and have perpendicular orientations. If either of these restrictions is violated, a corresponding
  message is printed to the debug output (qDebug), the construction is not aborted, though.
  
  Since QCPAbstractPlottable is an abstract class that defines the basic interface to plottables,
  it can't be directly instantiated.
  
  You probably want one of the subclasses like \ref QCPGraph or \ref QCPCurve instead.
*/
QCPAbstractPlottable::QCPAbstractPlottable(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPLayerable(keyAxis->parentPlot(), "", keyAxis->axisRect()),
  mName(""),
  mAntialiasedFill(true),
  mAntialiasedScatters(true),
  mAntialiasedErrorBars(false),
  mPen(Qt::black),
  mSelectedPen(Qt::black),
  mBrush(Qt::NoBrush),
  mSelectedBrush(Qt::NoBrush),
  mKeyAxis(keyAxis),
  mValueAxis(valueAxis),
  mSelectable(true),
  mSelected(false)
{
  if (keyAxis->parentPlot() != valueAxis->parentPlot())
    qDebug() << Q_FUNC_INFO << "Parent plot of keyAxis is not the same as that of valueAxis.";
  if (keyAxis->orientation() == valueAxis->orientation())
    qDebug() << Q_FUNC_INFO << "keyAxis and valueAxis must be orthogonal to each other.";
}

/*!
   The name is the textual representation of this plottable as it is displayed in the legend
   (\ref QCPLegend). It may contain any UTF-8 characters, including newlines.
*/
void QCPAbstractPlottable::setName(const QString &name)
{
  mName = name;
}

/*!
  Sets whether fills of this plottable is drawn antialiased or not.
  
  Note that this setting may be overridden by \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
*/
void QCPAbstractPlottable::setAntialiasedFill(bool enabled)
{
  mAntialiasedFill = enabled;
}

/*!
  Sets whether the scatter symbols of this plottable are drawn antialiased or not.
  
  Note that this setting may be overridden by \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
*/
void QCPAbstractPlottable::setAntialiasedScatters(bool enabled)
{
  mAntialiasedScatters = enabled;
}

/*!
  Sets whether the error bars of this plottable are drawn antialiased or not.
  
  Note that this setting may be overridden by \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
*/
void QCPAbstractPlottable::setAntialiasedErrorBars(bool enabled)
{
  mAntialiasedErrorBars = enabled;
}


/*!
  The pen is used to draw basic lines that make up the plottable representation in the
  plot.
  
  For example, the \ref QCPGraph subclass draws its graph lines and scatter points
  with this pen.

  \see setBrush
*/
void QCPAbstractPlottable::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  When the plottable is selected, this pen is used to draw basic lines instead of the normal
  pen set via \ref setPen.

  \see setSelected, setSelectable, setSelectedBrush, selectTest
*/
void QCPAbstractPlottable::setSelectedPen(const QPen &pen)
{
  mSelectedPen = pen;
}

/*!
  The brush is used to draw basic fills of the plottable representation in the
  plot. The Fill can be a color, gradient or texture, see the usage of QBrush.
  
  For example, the \ref QCPGraph subclass draws the fill under the graph with this brush, when
  it's not set to Qt::NoBrush.

  \see setPen
*/
void QCPAbstractPlottable::setBrush(const QBrush &brush)
{
  mBrush = brush;
}

/*!
  When the plottable is selected, this brush is used to draw fills instead of the normal
  brush set via \ref setBrush.

  \see setSelected, setSelectable, setSelectedPen, selectTest
*/
void QCPAbstractPlottable::setSelectedBrush(const QBrush &brush)
{
  mSelectedBrush = brush;
}

/*!
  The key axis of a plottable can be set to any axis of a QCustomPlot, as long as it is orthogonal
  to the plottable's value axis. This function performs no checks to make sure this is the case.
  The typical mathematical choice is to use the x-axis (QCustomPlot::xAxis) as key axis and the
  y-axis (QCustomPlot::yAxis) as value axis.
  
  Normally, the key and value axes are set in the constructor of the plottable (or \ref
  QCustomPlot::addGraph when working with QCPGraphs through the dedicated graph interface).

  \see setValueAxis
*/
void QCPAbstractPlottable::setKeyAxis(QCPAxis *axis)
{
  mKeyAxis = axis;
}

/*!
  The value axis of a plottable can be set to any axis of a QCustomPlot, as long as it is
  orthogonal to the plottable's key axis. This function performs no checks to make sure this is the
  case. The typical mathematical choice is to use the x-axis (QCustomPlot::xAxis) as key axis and
  the y-axis (QCustomPlot::yAxis) as value axis.

  Normally, the key and value axes are set in the constructor of the plottable (or \ref
  QCustomPlot::addGraph when working with QCPGraphs through the dedicated graph interface).
  
  \see setKeyAxis
*/
void QCPAbstractPlottable::setValueAxis(QCPAxis *axis)
{
  mValueAxis = axis;
}

/*!
  Sets whether the user can (de-)select this plottable by clicking on the QCustomPlot surface.
  (When \ref QCustomPlot::setInteractions contains iSelectPlottables.)
  
  However, even when \a selectable was set to false, it is possible to set the selection manually,
  by calling \ref setSelected directly.
  
  \see setSelected
*/
void QCPAbstractPlottable::setSelectable(bool selectable)
{
  if (mSelectable != selectable)
  {
    mSelectable = selectable;
    emit selectableChanged(mSelectable);
  }
}

/*!
  Sets whether this plottable is selected or not. When selected, it uses a different pen and brush
  to draw its lines and fills, see \ref setSelectedPen and \ref setSelectedBrush.

  The entire selection mechanism for plottables is handled automatically when \ref
  QCustomPlot::setInteractions contains iSelectPlottables. You only need to call this function when
  you wish to change the selection state manually.
  
  This function can change the selection state even when \ref setSelectable was set to false.
  
  emits the \ref selectionChanged signal when \a selected is different from the previous selection state.
  
  \see setSelectable, selectTest
*/
void QCPAbstractPlottable::setSelected(bool selected)
{
  if (mSelected != selected)
  {
    mSelected = selected;
    emit selectionChanged(mSelected);
  }
}

/*!
  Rescales the key and value axes associated with this plottable to contain all displayed data, so
  the whole plottable is visible. If the scaling of an axis is logarithmic, rescaleAxes will make
  sure not to rescale to an illegal range i.e. a range containing different signs and/or zero.
  Instead it will stay in the current sign domain and ignore all parts of the plottable that lie
  outside of that domain.
  
  \a onlyEnlarge makes sure the ranges are only expanded, never reduced. So it's possible to show
  multiple plottables in their entirety by multiple calls to rescaleAxes where the first call has
  \a onlyEnlarge set to false (the default), and all subsequent set to true.
  
  \see rescaleKeyAxis, rescaleValueAxis, QCustomPlot::rescaleAxes, QCPAxis::rescale
*/
void QCPAbstractPlottable::rescaleAxes(bool onlyEnlarge) const
{
  rescaleKeyAxis(onlyEnlarge);
  rescaleValueAxis(onlyEnlarge);
}

/*!
  Rescales the key axis of the plottable so the whole plottable is visible.
  
  See \ref rescaleAxes for detailed behaviour.
*/
void QCPAbstractPlottable::rescaleKeyAxis(bool onlyEnlarge) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  if (!keyAxis) { qDebug() << Q_FUNC_INFO << "invalid key axis"; return; }
  
  SignDomain signDomain = sdBoth;
  if (keyAxis->scaleType() == QCPAxis::stLogarithmic)
    signDomain = (keyAxis->range().upper < 0 ? sdNegative : sdPositive);
  
  bool foundRange;
  QCPRange newRange = getKeyRange(foundRange, signDomain);
  if (foundRange)
  {
    if (onlyEnlarge)
      newRange.expand(keyAxis->range());
    if (!QCPRange::validRange(newRange)) // likely due to range being zero (plottable has only constant data in this axis dimension), shift current range to at least center the plottable
    {
      double center = (newRange.lower+newRange.upper)*0.5; // upper and lower should be equal anyway, but just to make sure, incase validRange returned false for other reason
      if (keyAxis->scaleType() == QCPAxis::stLinear)
      {
        newRange.lower = center-keyAxis->range().size()/2.0;
        newRange.upper = center+keyAxis->range().size()/2.0;
      } else // scaleType() == stLogarithmic
      {
        newRange.lower = center/qSqrt(keyAxis->range().upper/keyAxis->range().lower);
        newRange.upper = center*qSqrt(keyAxis->range().upper/keyAxis->range().lower);
      }
    }
    keyAxis->setRange(newRange);
  }
}

/*!
  Rescales the value axis of the plottable so the whole plottable is visible.
  
  Returns true if the axis was actually scaled. This might not be the case if this plottable has an
  invalid range, e.g. because it has no data points.
  
  See \ref rescaleAxes for detailed behaviour.
*/
void QCPAbstractPlottable::rescaleValueAxis(bool onlyEnlarge) const
{
  QCPAxis *valueAxis = mValueAxis.data();
  if (!valueAxis) { qDebug() << Q_FUNC_INFO << "invalid value axis"; return; }
  
  SignDomain signDomain = sdBoth;
  if (valueAxis->scaleType() == QCPAxis::stLogarithmic)
    signDomain = (valueAxis->range().upper < 0 ? sdNegative : sdPositive);
  
  bool foundRange;
  QCPRange newRange = getValueRange(foundRange, signDomain);
  if (foundRange)
  {
    if (onlyEnlarge)
      newRange.expand(valueAxis->range());
    if (!QCPRange::validRange(newRange)) // likely due to range being zero (plottable has only constant data in this axis dimension), shift current range to at least center the plottable
    {
      double center = (newRange.lower+newRange.upper)*0.5; // upper and lower should be equal anyway, but just to make sure, incase validRange returned false for other reason
      if (valueAxis->scaleType() == QCPAxis::stLinear)
      {
        newRange.lower = center-valueAxis->range().size()/2.0;
        newRange.upper = center+valueAxis->range().size()/2.0;
      } else // scaleType() == stLogarithmic
      {
        newRange.lower = center/qSqrt(valueAxis->range().upper/valueAxis->range().lower);
        newRange.upper = center*qSqrt(valueAxis->range().upper/valueAxis->range().lower);
      }
    }
    valueAxis->setRange(newRange);
  }
}

/*!
  Adds this plottable to the legend of the parent QCustomPlot (QCustomPlot::legend).
    
  Normally, a QCPPlottableLegendItem is created and inserted into the legend. If the plottable
  needs a more specialized representation in the legend, this function will take this into account
  and instead create the specialized subclass of QCPAbstractLegendItem.
    
  Returns true on success, i.e. when the legend exists and a legend item associated with this plottable isn't already in
  the legend.
    
  \see removeFromLegend, QCPLegend::addItem
*/
bool QCPAbstractPlottable::addToLegend()
{
  if (!mParentPlot || !mParentPlot->legend)
    return false;
  
  if (!mParentPlot->legend->hasItemWithPlottable(this))
  {
    mParentPlot->legend->addItem(new QCPPlottableLegendItem(mParentPlot->legend, this));
    return true;
  } else
    return false;
}

/*!
  Removes the plottable from the legend of the parent QCustomPlot. This means the
  QCPAbstractLegendItem (usually a QCPPlottableLegendItem) that is associated with this plottable
  is removed.
    
  Returns true on success, i.e. if the legend exists and a legend item associated with this
  plottable was found and removed.
    
  \see addToLegend, QCPLegend::removeItem
*/
bool QCPAbstractPlottable::removeFromLegend() const
{
  if (!mParentPlot->legend)
    return false;
  
  if (QCPPlottableLegendItem *lip = mParentPlot->legend->itemWithPlottable(this))
    return mParentPlot->legend->removeItem(lip);
  else
    return false;
}

/* inherits documentation from base class */
QRect QCPAbstractPlottable::clipRect() const
{
  if (mKeyAxis && mValueAxis)
    return mKeyAxis.data()->axisRect()->rect() & mValueAxis.data()->axisRect()->rect();
  else
    return QRect();
}

/* inherits documentation from base class */
QCP::Interaction QCPAbstractPlottable::selectionCategory() const
{
  return QCP::iSelectPlottables;
}

/*! \internal
  
  Convenience function for transforming a key/value pair to pixels on the QCustomPlot surface,
  taking the orientations of the axes associated with this plottable into account (e.g. whether key
  represents x or y).
  
  \a key and \a value are transformed to the coodinates in pixels and are written to \a x and \a y.
    
  \see pixelsToCoords, QCPAxis::coordToPixel
*/
void QCPAbstractPlottable::coordsToPixels(double key, double value, double &x, double &y) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  
  if (keyAxis->orientation() == Qt::Horizontal)
  {
    x = keyAxis->coordToPixel(key);
    y = valueAxis->coordToPixel(value);
  } else
  {
    y = keyAxis->coordToPixel(key);
    x = valueAxis->coordToPixel(value);
  }
}

/*! \internal
  \overload
  
  Returns the input as pixel coordinates in a QPointF.
*/
const QPointF QCPAbstractPlottable::coordsToPixels(double key, double value) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return QPointF(); }
  
  if (keyAxis->orientation() == Qt::Horizontal)
    return QPointF(keyAxis->coordToPixel(key), valueAxis->coordToPixel(value));
  else
    return QPointF(valueAxis->coordToPixel(value), keyAxis->coordToPixel(key));
}

/*! \internal
  
  Convenience function for transforming a x/y pixel pair on the QCustomPlot surface to plot coordinates,
  taking the orientations of the axes associated with this plottable into account (e.g. whether key
  represents x or y).
  
  \a x and \a y are transformed to the plot coodinates and are written to \a key and \a value.
    
  \see coordsToPixels, QCPAxis::coordToPixel
*/
void QCPAbstractPlottable::pixelsToCoords(double x, double y, double &key, double &value) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  
  if (keyAxis->orientation() == Qt::Horizontal)
  {
    key = keyAxis->pixelToCoord(x);
    value = valueAxis->pixelToCoord(y);
  } else
  {
    key = keyAxis->pixelToCoord(y);
    value = valueAxis->pixelToCoord(x);
  }
}

/*! \internal
  \overload

  Returns the pixel input \a pixelPos as plot coordinates \a key and \a value.
*/
void QCPAbstractPlottable::pixelsToCoords(const QPointF &pixelPos, double &key, double &value) const
{
  pixelsToCoords(pixelPos.x(), pixelPos.y(), key, value);
}

/*! \internal

  Returns the pen that should be used for drawing lines of the plottable. Returns mPen when the
  graph is not selected and mSelectedPen when it is.
*/
QPen QCPAbstractPlottable::mainPen() const
{
  return mSelected ? mSelectedPen : mPen;
}

/*! \internal

  Returns the brush that should be used for drawing fills of the plottable. Returns mBrush when the
  graph is not selected and mSelectedBrush when it is.
*/
QBrush QCPAbstractPlottable::mainBrush() const
{
  return mSelected ? mSelectedBrush : mBrush;
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing plottable lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased, applyFillAntialiasingHint, applyScattersAntialiasingHint, applyErrorBarsAntialiasingHint
*/
void QCPAbstractPlottable::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiased, QCP::aePlottables);
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing plottable fills.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased, applyDefaultAntialiasingHint, applyScattersAntialiasingHint, applyErrorBarsAntialiasingHint
*/
void QCPAbstractPlottable::applyFillAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiasedFill, QCP::aeFills);
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing plottable scatter points.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased, applyFillAntialiasingHint, applyDefaultAntialiasingHint, applyErrorBarsAntialiasingHint
*/
void QCPAbstractPlottable::applyScattersAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiasedScatters, QCP::aeScatters);
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing plottable error bars.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased, applyFillAntialiasingHint, applyScattersAntialiasingHint, applyDefaultAntialiasingHint
*/
void QCPAbstractPlottable::applyErrorBarsAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiasedErrorBars, QCP::aeErrorBars);
}

/*! \internal

  Finds the shortest squared distance of \a point to the line segment defined by \a start and \a
  end.
  
  This function may be used to help with the implementation of the \ref selectTest function for
  specific plottables.
  
  \note This function is identical to QCPAbstractItem::distSqrToLine
*/
double QCPAbstractPlottable::distSqrToLine(const QPointF &start, const QPointF &end, const QPointF &point) const
{
  QVector2D a(start);
  QVector2D b(end);
  QVector2D p(point);
  QVector2D v(b-a);
  
  double vLengthSqr = v.lengthSquared();
  if (!qFuzzyIsNull(vLengthSqr))
  {
    double mu = QVector2D::dotProduct(p-a, v)/vLengthSqr;
    if (mu < 0)
      return (a-p).lengthSquared();
    else if (mu > 1)
      return (b-p).lengthSquared();
    else
      return ((a + mu*v)-p).lengthSquared();
  } else
    return (a-p).lengthSquared();
}

/* inherits documentation from base class */
void QCPAbstractPlottable::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
  Q_UNUSED(event)
  Q_UNUSED(details)
  if (mSelectable)
  {
    bool selBefore = mSelected;
    setSelected(additive ? !mSelected : true);
    if (selectionStateChanged)
      *selectionStateChanged = mSelected != selBefore;
  }
}

/* inherits documentation from base class */
void QCPAbstractPlottable::deselectEvent(bool *selectionStateChanged)
{
  if (mSelectable)
  {
    bool selBefore = mSelected;
    setSelected(false);
    if (selectionStateChanged)
      *selectionStateChanged = mSelected != selBefore;
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemAnchor
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemAnchor
  \brief An anchor of an item to which positions can be attached to.
  
  An item (QCPAbstractItem) may have one or more anchors. Unlike QCPItemPosition, an anchor doesn't
  control anything on its item, but provides a way to tie other items via their positions to the
  anchor.

  For example, a QCPItemRect is defined by its positions \a topLeft and \a bottomRight.
  Additionally it has various anchors like \a top, \a topRight or \a bottomLeft etc. So you can
  attach the \a start (which is a QCPItemPosition) of a QCPItemLine to one of the anchors by
  calling QCPItemPosition::setParentAnchor on \a start, passing the wanted anchor of the
  QCPItemRect. This way the start of the line will now always follow the respective anchor location
  on the rect item.
  
  Note that QCPItemPosition derives from QCPItemAnchor, so every position can also serve as an
  anchor to other positions.
  
  To learn how to provide anchors in your own item subclasses, see the subclassing section of the
  QCPAbstractItem documentation.
*/

/* start documentation of inline functions */

/*! \fn virtual QCPItemPosition *QCPItemAnchor::toQCPItemPosition()
  
  Returns 0 if this instance is merely a QCPItemAnchor, and a valid pointer of type QCPItemPosition* if
  it actually is a QCPItemPosition (which is a subclass of QCPItemAnchor).
  
  This safe downcast functionality could also be achieved with a dynamic_cast. However, QCustomPlot avoids
  dynamic_cast to work with projects that don't have RTTI support enabled (e.g. -fno-rtti flag with
  gcc compiler).
*/

/* end documentation of inline functions */

/*!
  Creates a new QCPItemAnchor. You shouldn't create QCPItemAnchor instances directly, even if
  you want to make a new item subclass. Use \ref QCPAbstractItem::createAnchor instead, as
  explained in the subclassing section of the QCPAbstractItem documentation.
*/
QCPItemAnchor::QCPItemAnchor(QCustomPlot *parentPlot, QCPAbstractItem *parentItem, const QString name, int anchorId) :
  mName(name),
  mParentPlot(parentPlot),
  mParentItem(parentItem),
  mAnchorId(anchorId)
{
}

QCPItemAnchor::~QCPItemAnchor()
{
  // unregister as parent at children:
  QList<QCPItemPosition*> currentChildren(mChildren.toList());
  for (int i=0; i<currentChildren.size(); ++i)
    currentChildren.at(i)->setParentAnchor(0); // this acts back on this anchor and child removes itself from mChildren
}

/*!
  Returns the final absolute pixel position of the QCPItemAnchor on the QCustomPlot surface.
  
  The pixel information is internally retrieved via QCPAbstractItem::anchorPixelPosition of the
  parent item, QCPItemAnchor is just an intermediary.
*/
QPointF QCPItemAnchor::pixelPoint() const
{
  if (mParentItem)
  {
    if (mAnchorId > -1)
    {
      return mParentItem->anchorPixelPoint(mAnchorId);
    } else
    {
      qDebug() << Q_FUNC_INFO << "no valid anchor id set:" << mAnchorId;
      return QPointF();
    }
  } else
  {
    qDebug() << Q_FUNC_INFO << "no parent item set";
    return QPointF();
  }
}

/*! \internal

  Adds \a pos to the child list of this anchor. This is necessary to notify the children prior to
  destruction of the anchor.
  
  Note that this function does not change the parent setting in \a pos.
*/
void QCPItemAnchor::addChild(QCPItemPosition *pos)
{
  if (!mChildren.contains(pos))
    mChildren.insert(pos);
  else
    qDebug() << Q_FUNC_INFO << "provided pos is child already" << reinterpret_cast<quintptr>(pos);
}

/*! \internal

  Removes \a pos from the child list of this anchor.
  
  Note that this function does not change the parent setting in \a pos.
*/
void QCPItemAnchor::removeChild(QCPItemPosition *pos)
{
  if (!mChildren.remove(pos))
    qDebug() << Q_FUNC_INFO << "provided pos isn't child" << reinterpret_cast<quintptr>(pos);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemPosition
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemPosition
  \brief Manages the position of an item.
  
  Every item has at least one public QCPItemPosition member pointer which provides ways to position the
  item on the QCustomPlot surface. Some items have multiple positions, for example QCPItemRect has two:
  \a topLeft and \a bottomRight.

  QCPItemPosition has a type (\ref PositionType) that can be set with \ref setType. This type defines
  how coordinates passed to \ref setCoords are to be interpreted, e.g. as absolute pixel coordinates, as
  plot coordinates of certain axes, etc.

  Further, QCPItemPosition may have a parent QCPItemAnchor, see \ref setParentAnchor. (Note that every
  QCPItemPosition inherits from QCPItemAnchor and thus can itself be used as parent anchor for other
  positions.) This way you can tie multiple items together. If the QCPItemPosition has a parent, the
  coordinates set with \ref setCoords are considered to be absolute values in the reference frame of the
  parent anchor, where (0, 0) means directly ontop of the parent anchor. For example, You could attach
  the \a start position of a QCPItemLine to the \a bottom anchor of a QCPItemText to make the starting
  point of the line always be centered under the text label, no matter where the text is moved to, or is
  itself tied to.

  To set the apparent pixel position on the QCustomPlot surface directly, use \ref setPixelPoint. This
  works no matter what type this QCPItemPosition is or what parent-child situation it is in, as \ref
  setPixelPoint transforms the coordinates appropriately, to make the position appear at the specified
  pixel values.
*/

/*!
  Creates a new QCPItemPosition. You shouldn't create QCPItemPosition instances directly, even if
  you want to make a new item subclass. Use \ref QCPAbstractItem::createPosition instead, as
  explained in the subclassing section of the QCPAbstractItem documentation.
*/
QCPItemPosition::QCPItemPosition(QCustomPlot *parentPlot, QCPAbstractItem *parentItem, const QString name) :
  QCPItemAnchor(parentPlot, parentItem, name),
  mPositionType(ptAbsolute),
  mKey(0),
  mValue(0),
  mParentAnchor(0)
{
}

QCPItemPosition::~QCPItemPosition()
{
  // unregister as parent at children:
  // Note: this is done in ~QCPItemAnchor again, but it's important QCPItemPosition does it itself, because only then
  //       the setParentAnchor(0) call the correct QCPItemPosition::pixelPoint function instead of QCPItemAnchor::pixelPoint
  QList<QCPItemPosition*> currentChildren(mChildren.toList());
  for (int i=0; i<currentChildren.size(); ++i)
    currentChildren.at(i)->setParentAnchor(0); // this acts back on this anchor and child removes itself from mChildren
  // unregister as child in parent:
  if (mParentAnchor)
    mParentAnchor->removeChild(this);
}

/* can't make this a header inline function, because QPointer breaks with forward declared types, see QTBUG-29588 */
QCPAxisRect *QCPItemPosition::axisRect() const
{
  return mAxisRect.data();
}

/*!
  Sets the type of the position. The type defines how the coordinates passed to \ref setCoords
  should be handled and how the QCPItemPosition should behave in the plot.
  
  The possible values for \a type can be separated in two main categories:

  \li The position is regarded as a point in plot coordinates. This corresponds to \ref ptPlotCoords
  and requires two axes that define the plot coordinate system. They can be specified with \ref setAxes.
  By default, the QCustomPlot's x- and yAxis are used.
  
  \li The position is fixed on the QCustomPlot surface, i.e. independent of axis ranges. This
  corresponds to all other types, i.e. \ref ptAbsolute, \ref ptViewportRatio and \ref
  ptAxisRectRatio. They differ only in the way the absolute position is described, see the
  documentation of \ref PositionType for details. For \ref ptAxisRectRatio, note that you can specify
  the axis rect with \ref setAxisRect. By default this is set to the main axis rect.
  
  Note that the position type \ref ptPlotCoords is only available (and sensible) when the position
  has no parent anchor (\ref setParentAnchor).
  
  If the type is changed, the apparent pixel position on the plot is preserved. This means
  the coordinates as retrieved with coords() and set with \ref setCoords may change in the process.
*/
void QCPItemPosition::setType(QCPItemPosition::PositionType type)
{
  if (mPositionType != type)
  {
    // if switching from or to coordinate type that isn't valid (e.g. because axes or axis rect
    // were deleted), don't try to recover the pixelPoint() because it would output a qDebug warning.
    bool recoverPixelPosition = true;
    if ((mPositionType == ptPlotCoords || type == ptPlotCoords) && (!mKeyAxis || !mValueAxis))
      recoverPixelPosition = false;
    if ((mPositionType == ptAxisRectRatio || type == ptAxisRectRatio) && (!mAxisRect))
      recoverPixelPosition = false;
      
    QPointF pixelP;
    if (recoverPixelPosition)
      pixelP = pixelPoint();
    
    mPositionType = type;
    
    if (recoverPixelPosition)
      setPixelPoint(pixelP);
  }
}

/*!
  Sets the parent of this QCPItemPosition to \a parentAnchor. This means the position will now
  follow any position changes of the anchor. The local coordinate system of positions with a parent
  anchor always is absolute with (0, 0) being exactly on top of the parent anchor. (Hence the type
  shouldn't be \ref ptPlotCoords for positions with parent anchors.)
  
  if \a keepPixelPosition is true, the current pixel position of the QCPItemPosition is preserved
  during reparenting. If it's set to false, the coordinates are set to (0, 0), i.e. the position
  will be exactly on top of the parent anchor.
  
  To remove this QCPItemPosition from any parent anchor, set \a parentAnchor to 0.
  
  If the QCPItemPosition previously had no parent and the type is \ref ptPlotCoords, the type is
  set to \ref ptAbsolute, to keep the position in a valid state.
*/
bool QCPItemPosition::setParentAnchor(QCPItemAnchor *parentAnchor, bool keepPixelPosition)
{
  // make sure self is not assigned as parent:
  if (parentAnchor == this)
  {
    qDebug() << Q_FUNC_INFO << "can't set self as parent anchor" << reinterpret_cast<quintptr>(parentAnchor);
    return false;
  }
  // make sure no recursive parent-child-relationships are created:
  QCPItemAnchor *currentParent = parentAnchor;
  while (currentParent)
  {
    if (QCPItemPosition *currentParentPos = currentParent->toQCPItemPosition())
    {
      // is a QCPItemPosition, might have further parent, so keep iterating
      if (currentParentPos == this)
      {
        qDebug() << Q_FUNC_INFO << "can't create recursive parent-child-relationship" << reinterpret_cast<quintptr>(parentAnchor);
        return false;
      }
      currentParent = currentParentPos->mParentAnchor;
    } else
    {
      // is a QCPItemAnchor, can't have further parent. Now make sure the parent items aren't the
      // same, to prevent a position being child of an anchor which itself depends on the position,
      // because they're both on the same item:
      if (currentParent->mParentItem == mParentItem)
      {
        qDebug() << Q_FUNC_INFO << "can't set parent to be an anchor which itself depends on this position" << reinterpret_cast<quintptr>(parentAnchor);
        return false;
      }
      break;
    }
  }
  
  // if previously no parent set and PosType is still ptPlotCoords, set to ptAbsolute:
  if (!mParentAnchor && mPositionType == ptPlotCoords)
    setType(ptAbsolute);
  
  // save pixel position:
  QPointF pixelP;
  if (keepPixelPosition)
    pixelP = pixelPoint();
  // unregister at current parent anchor:
  if (mParentAnchor)
    mParentAnchor->removeChild(this);
  // register at new parent anchor:
  if (parentAnchor)
    parentAnchor->addChild(this);
  mParentAnchor = parentAnchor;
  // restore pixel position under new parent:
  if (keepPixelPosition)
    setPixelPoint(pixelP);
  else
    setCoords(0, 0);
  return true;
}

/*!
  Sets the coordinates of this QCPItemPosition. What the coordinates mean, is defined by the type
  (\ref setType).
  
  For example, if the type is \ref ptAbsolute, \a key and \a value mean the x and y pixel position
  on the QCustomPlot surface. In that case the origin (0, 0) is in the top left corner of the
  QCustomPlot viewport. If the type is \ref ptPlotCoords, \a key and \a value mean a point in the
  plot coordinate system defined by the axes set by \ref setAxes. By default those are the
  QCustomPlot's xAxis and yAxis. See the documentation of \ref setType for other available
  coordinate types and their meaning.

  \see setPixelPoint
*/
void QCPItemPosition::setCoords(double key, double value)
{
  mKey = key;
  mValue = value;
}

/*! \overload

  Sets the coordinates as a QPointF \a pos where pos.x has the meaning of \a key and pos.y the
  meaning of \a value of the \ref setCoords(double key, double value) method.
*/
void QCPItemPosition::setCoords(const QPointF &pos)
{
  setCoords(pos.x(), pos.y());
}

/*!
  Returns the final absolute pixel position of the QCPItemPosition on the QCustomPlot surface. It
  includes all effects of type (\ref setType) and possible parent anchors (\ref setParentAnchor).

  \see setPixelPoint
*/
QPointF QCPItemPosition::pixelPoint() const
{
  switch (mPositionType)
  {
    case ptAbsolute:
    {
      if (mParentAnchor)
        return QPointF(mKey, mValue) + mParentAnchor->pixelPoint();
      else
        return QPointF(mKey, mValue);
    }
    
    case ptViewportRatio:
    {
      if (mParentAnchor)
      {
        return QPointF(mKey*mParentPlot->viewport().width(),
                       mValue*mParentPlot->viewport().height()) + mParentAnchor->pixelPoint();
      } else
      {
        return QPointF(mKey*mParentPlot->viewport().width(),
                       mValue*mParentPlot->viewport().height()) + mParentPlot->viewport().topLeft();
      }
    }
      
    case ptAxisRectRatio:
    {
      if (mAxisRect)
      {
        if (mParentAnchor)
        {
          return QPointF(mKey*mAxisRect.data()->width(),
                         mValue*mAxisRect.data()->height()) + mParentAnchor->pixelPoint();
        } else
        {
          return QPointF(mKey*mAxisRect.data()->width(),
                       mValue*mAxisRect.data()->height()) + mAxisRect.data()->topLeft();
        }
      } else
      {
        qDebug() << Q_FUNC_INFO << "No axis rect defined";
        return QPointF(mKey, mValue);
      }
    }
    
    case ptPlotCoords:
    {
      double x, y;
      if (mKeyAxis && mValueAxis)
      {
        // both key and value axis are given, translate key/value to x/y coordinates:
        if (mKeyAxis.data()->orientation() == Qt::Horizontal)
        {
          x = mKeyAxis.data()->coordToPixel(mKey);
          y = mValueAxis.data()->coordToPixel(mValue);
        } else
        {
          y = mKeyAxis.data()->coordToPixel(mKey);
          x = mValueAxis.data()->coordToPixel(mValue);
        }
      } else if (mKeyAxis)
      {
        // only key axis is given, depending on orientation only transform x or y to key coordinate, other stays pixel:
        if (mKeyAxis.data()->orientation() == Qt::Horizontal)
        {
          x = mKeyAxis.data()->coordToPixel(mKey);
          y = mValue;
        } else
        {
          y = mKeyAxis.data()->coordToPixel(mKey);
          x = mValue;
        }
      } else if (mValueAxis)
      {
        // only value axis is given, depending on orientation only transform x or y to value coordinate, other stays pixel:
        if (mValueAxis.data()->orientation() == Qt::Horizontal)
        {
          x = mValueAxis.data()->coordToPixel(mValue);
          y = mKey;
        } else
        {
          y = mValueAxis.data()->coordToPixel(mValue);
          x = mKey;
        }
      } else
      {
        // no axis given, basically the same as if mPositionType were ptAbsolute
        qDebug() << Q_FUNC_INFO << "No axes defined";
        x = mKey;
        y = mValue;
      }
      return QPointF(x, y);
    }
  }
  return QPointF();
}

/*!
  When \ref setType is \ref ptPlotCoords, this function may be used to specify the axes the
  coordinates set with \ref setCoords relate to. By default they are set to the initial xAxis and
  yAxis of the QCustomPlot.
*/
void QCPItemPosition::setAxes(QCPAxis *keyAxis, QCPAxis *valueAxis)
{
  mKeyAxis = keyAxis;
  mValueAxis = valueAxis;
}

/*!
  When \ref setType is \ref ptAxisRectRatio, this function may be used to specify the axis rect the
  coordinates set with \ref setCoords relate to. By default this is set to the main axis rect of
  the QCustomPlot.
*/
void QCPItemPosition::setAxisRect(QCPAxisRect *axisRect)
{
  mAxisRect = axisRect;
}

/*!
  Sets the apparent pixel position. This works no matter what type (\ref setType) this
  QCPItemPosition is or what parent-child situation it is in, as coordinates are transformed
  appropriately, to make the position finally appear at the specified pixel values.

  Only if the type is \ref ptAbsolute and no parent anchor is set, this function's effect is
  identical to that of \ref setCoords.

  \see pixelPoint, setCoords
*/
void QCPItemPosition::setPixelPoint(const QPointF &pixelPoint)
{
  switch (mPositionType)
  {
    case ptAbsolute:
    {
      if (mParentAnchor)
        setCoords(pixelPoint-mParentAnchor->pixelPoint());
      else
        setCoords(pixelPoint);
      break;
    }
      
    case ptViewportRatio:
    {
      if (mParentAnchor)
      {
        QPointF p(pixelPoint-mParentAnchor->pixelPoint());
        p.rx() /= (double)mParentPlot->viewport().width();
        p.ry() /= (double)mParentPlot->viewport().height();
        setCoords(p);
      } else
      {
        QPointF p(pixelPoint-mParentPlot->viewport().topLeft());
        p.rx() /= (double)mParentPlot->viewport().width();
        p.ry() /= (double)mParentPlot->viewport().height();
        setCoords(p);
      }
      break;
    }
      
    case ptAxisRectRatio:
    {
      if (mAxisRect)
      {
        if (mParentAnchor)
        {
          QPointF p(pixelPoint-mParentAnchor->pixelPoint());
          p.rx() /= (double)mAxisRect.data()->width();
          p.ry() /= (double)mAxisRect.data()->height();
          setCoords(p);
        } else
        {
          QPointF p(pixelPoint-mAxisRect.data()->topLeft());
          p.rx() /= (double)mAxisRect.data()->width();
          p.ry() /= (double)mAxisRect.data()->height();
          setCoords(p);
        }
      } else
      {
        qDebug() << Q_FUNC_INFO << "No axis rect defined";
        setCoords(pixelPoint);
      }
      break;
    }
      
    case ptPlotCoords:
    {
      double newKey, newValue;
      if (mKeyAxis && mValueAxis)
      {
        // both key and value axis are given, translate point to key/value coordinates:
        if (mKeyAxis.data()->orientation() == Qt::Horizontal)
        {
          newKey = mKeyAxis.data()->pixelToCoord(pixelPoint.x());
          newValue = mValueAxis.data()->pixelToCoord(pixelPoint.y());
        } else
        {
          newKey = mKeyAxis.data()->pixelToCoord(pixelPoint.y());
          newValue = mValueAxis.data()->pixelToCoord(pixelPoint.x());
        }
      } else if (mKeyAxis)
      {
        // only key axis is given, depending on orientation only transform x or y to key coordinate, other stays pixel:
        if (mKeyAxis.data()->orientation() == Qt::Horizontal)
        {
          newKey = mKeyAxis.data()->pixelToCoord(pixelPoint.x());
          newValue = pixelPoint.y();
        } else
        {
          newKey = mKeyAxis.data()->pixelToCoord(pixelPoint.y());
          newValue = pixelPoint.x();
        }
      } else if (mValueAxis)
      {
        // only value axis is given, depending on orientation only transform x or y to value coordinate, other stays pixel:
        if (mValueAxis.data()->orientation() == Qt::Horizontal)
        {
          newKey = pixelPoint.y();
          newValue = mValueAxis.data()->pixelToCoord(pixelPoint.x());
        } else
        {
          newKey = pixelPoint.x();
          newValue = mValueAxis.data()->pixelToCoord(pixelPoint.y());
        }
      } else
      {
        // no axis given, basically the same as if mPositionType were ptAbsolute
        qDebug() << Q_FUNC_INFO << "No axes defined";
        newKey = pixelPoint.x();
        newValue = pixelPoint.y();
      }
      setCoords(newKey, newValue);
      break;
    }
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAbstractItem
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAbstractItem
  \brief The abstract base class for all items in a plot.
  
  In QCustomPlot, items are supplemental graphical elements that are neither plottables
  (QCPAbstractPlottable) nor axes (QCPAxis). While plottables are always tied to two axes and thus
  plot coordinates, items can also be placed in absolute coordinates independent of any axes. Each
  specific item has at least one QCPItemPosition member which controls the positioning. Some items
  are defined by more than one coordinate and thus have two or more QCPItemPosition members (For
  example, QCPItemRect has \a topLeft and \a bottomRight).
  
  This abstract base class defines a very basic interface like visibility and clipping. Since this
  class is abstract, it can't be instantiated. Use one of the subclasses or create a subclass
  yourself to create new items.
  
  The built-in items are:
  <table>
  <tr><td>QCPItemLine</td><td>A line defined by a start and an end point. May have different ending styles on each side (e.g. arrows).</td></tr>
  <tr><td>QCPItemStraightLine</td><td>A straight line defined by a start and a direction point. Unlike QCPItemLine, the straight line is infinitely long and has no endings.</td></tr>
  <tr><td>QCPItemCurve</td><td>A curve defined by start, end and two intermediate control points. May have different ending styles on each side (e.g. arrows).</td></tr>
  <tr><td>QCPItemRect</td><td>A rectangle</td></tr>
  <tr><td>QCPItemEllipse</td><td>An ellipse</td></tr>
  <tr><td>QCPItemPixmap</td><td>An arbitrary pixmap</td></tr>
  <tr><td>QCPItemText</td><td>A text label</td></tr>
  <tr><td>QCPItemBracket</td><td>A bracket which may be used to reference/highlight certain parts in the plot.</td></tr>
  <tr><td>QCPItemTracer</td><td>An item that can be attached to a QCPGraph and sticks to its data points, given a key coordinate.</td></tr>
  </table>
  
  Items are by default clipped to the main axis rect. To make an item visible outside that axis
  rect, disable clipping via \ref setClipToAxisRect.
  
  \section items-using Using items
  
  First you instantiate the item you want to use and add it to the plot:
  \code
  QCPItemLine *line = new QCPItemLine(customPlot);
  customPlot->addItem(line);
  \endcode
  by default, the positions of the item are bound to the x- and y-Axis of the plot. So we can just
  set the plot coordinates where the line should start/end:
  \code
  line->start->setCoords(-0.1, 0.8);
  line->end->setCoords(1.1, 0.2);
  \endcode
  If we don't want the line to be positioned in plot coordinates but a different coordinate system,
  e.g. absolute pixel positions on the QCustomPlot surface, we need to change the position type like this:
  \code
  line->start->setType(QCPItemPosition::ptAbsolute);
  line->end->setType(QCPItemPosition::ptAbsolute);
  \endcode
  Then we can set the coordinates, this time in pixels:
  \code
  line->start->setCoords(100, 200);
  line->end->setCoords(450, 320);
  \endcode
  
  \section items-subclassing Creating own items
  
  To create an own item, you implement a subclass of QCPAbstractItem. These are the pure
  virtual functions, you must implement:
  \li \ref selectTest
  \li \ref draw
  
  See the documentation of those functions for what they need to do.
  
  \subsection items-positioning Allowing the item to be positioned
  
  As mentioned, item positions are represented by QCPItemPosition members. Let's assume the new item shall
  have only one point as its position (as opposed to two like a rect or multiple like a polygon). You then add
  a public member of type QCPItemPosition like so:
  
  \code QCPItemPosition * const myPosition;\endcode
  
  the const makes sure the pointer itself can't be modified from the user of your new item (the QCPItemPosition
  instance it points to, can be modified, of course).
  The initialization of this pointer is made easy with the \ref createPosition function. Just assign
  the return value of this function to each QCPItemPosition in the constructor of your item. \ref createPosition
  takes a string which is the name of the position, typically this is identical to the variable name.
  For example, the constructor of QCPItemExample could look like this:
  
  \code
  QCPItemExample::QCPItemExample(QCustomPlot *parentPlot) :
    QCPAbstractItem(parentPlot),
    myPosition(createPosition("myPosition"))
  {
    // other constructor code
  }
  \endcode
  
  \subsection items-drawing The draw function
  
  To give your item a visual representation, reimplement the \ref draw function and use the passed
  QCPPainter to draw the item. You can retrieve the item position in pixel coordinates from the
  position member(s) via \ref QCPItemPosition::pixelPoint.

  To optimize performance you should calculate a bounding rect first (don't forget to take the pen
  width into account), check whether it intersects the \ref clipRect, and only draw the item at all
  if this is the case.
  
  \subsection items-selection The selectTest function
  
  Your implementation of the \ref selectTest function may use the helpers \ref distSqrToLine and
  \ref rectSelectTest. With these, the implementation of the selection test becomes significantly
  simpler for most items. See the documentation of \ref selectTest for what the function parameters
  mean and what the function should return.
  
  \subsection anchors Providing anchors
  
  Providing anchors (QCPItemAnchor) starts off like adding a position. First you create a public
  member, e.g.
  
  \code QCPItemAnchor * const bottom;\endcode

  and create it in the constructor with the \ref createAnchor function, assigning it a name and an
  anchor id (an integer enumerating all anchors on the item, you may create an own enum for this).
  Since anchors can be placed anywhere, relative to the item's position(s), your item needs to
  provide the position of every anchor with the reimplementation of the \ref anchorPixelPoint(int
  anchorId) function.
  
  In essence the QCPItemAnchor is merely an intermediary that itself asks your item for the pixel
  position when anything attached to the anchor needs to know the coordinates.
*/

/* start of documentation of inline functions */

/*! \fn QList<QCPItemPosition*> QCPAbstractItem::positions() const
  
  Returns all positions of the item in a list.
  
  \see anchors, position
*/

/*! \fn QList<QCPItemAnchor*> QCPAbstractItem::anchors() const
  
  Returns all anchors of the item in a list. Note that since a position (QCPItemPosition) is always
  also an anchor, the list will also contain the positions of this item.
  
  \see positions, anchor
*/

/* end of documentation of inline functions */
/* start documentation of pure virtual functions */

/*! \fn void QCPAbstractItem::draw(QCPPainter *painter) = 0
  \internal
  
  Draws this item with the provided \a painter.
  
  The cliprect of the provided painter is set to the rect returned by \ref clipRect before this
  function is called. The clipRect depends on the clipping settings defined by \ref
  setClipToAxisRect and \ref setClipAxisRect.
*/

/* end documentation of pure virtual functions */
/* start documentation of signals */

/*! \fn void QCPAbstractItem::selectionChanged(bool selected)
  This signal is emitted when the selection state of this item has changed, either by user interaction
  or by a direct call to \ref setSelected.
*/

/* end documentation of signals */

/*!
  Base class constructor which initializes base class members.
*/
QCPAbstractItem::QCPAbstractItem(QCustomPlot *parentPlot) :
  QCPLayerable(parentPlot),
  mClipToAxisRect(false),
  mSelectable(true),
  mSelected(false)
{
  QList<QCPAxisRect*> rects = parentPlot->axisRects();
  if (rects.size() > 0)
  {
    setClipToAxisRect(true);
    setClipAxisRect(rects.first());
  }
}

QCPAbstractItem::~QCPAbstractItem()
{
  // don't delete mPositions because every position is also an anchor and thus in mAnchors
  qDeleteAll(mAnchors);
}

/* can't make this a header inline function, because QPointer breaks with forward declared types, see QTBUG-29588 */
QCPAxisRect *QCPAbstractItem::clipAxisRect() const
{
  return mClipAxisRect.data();
}

/*!
  Sets whether the item shall be clipped to an axis rect or whether it shall be visible on the
  entire QCustomPlot. The axis rect can be set with \ref setClipAxisRect.
  
  \see setClipAxisRect
*/
void QCPAbstractItem::setClipToAxisRect(bool clip)
{
  mClipToAxisRect = clip;
  if (mClipToAxisRect)
    setParentLayerable(mClipAxisRect.data());
}

/*!
  Sets the clip axis rect. It defines the rect that will be used to clip the item when \ref
  setClipToAxisRect is set to true.
  
  \see setClipToAxisRect
*/
void QCPAbstractItem::setClipAxisRect(QCPAxisRect *rect)
{
  mClipAxisRect = rect;
  if (mClipToAxisRect)
    setParentLayerable(mClipAxisRect.data());
}

/*!
  Sets whether the user can (de-)select this item by clicking on the QCustomPlot surface.
  (When \ref QCustomPlot::setInteractions contains QCustomPlot::iSelectItems.)
  
  However, even when \a selectable was set to false, it is possible to set the selection manually,
  by calling \ref setSelected.
  
  \see QCustomPlot::setInteractions, setSelected
*/
void QCPAbstractItem::setSelectable(bool selectable)
{
  if (mSelectable != selectable)
  {
    mSelectable = selectable;
    emit selectableChanged(mSelectable);
  }
}

/*!
  Sets whether this item is selected or not. When selected, it might use a different visual
  appearance (e.g. pen and brush), this depends on the specific item though.

  The entire selection mechanism for items is handled automatically when \ref
  QCustomPlot::setInteractions contains QCustomPlot::iSelectItems. You only need to call this
  function when you wish to change the selection state manually.
  
  This function can change the selection state even when \ref setSelectable was set to false.
  
  emits the \ref selectionChanged signal when \a selected is different from the previous selection state.
  
  \see setSelectable, selectTest
*/
void QCPAbstractItem::setSelected(bool selected)
{
  if (mSelected != selected)
  {
    mSelected = selected;
    emit selectionChanged(mSelected);
  }
}

/*!
  Returns the QCPItemPosition with the specified \a name. If this item doesn't have a position by
  that name, returns 0.
  
  This function provides an alternative way to access item positions. Normally, you access
  positions direcly by their member pointers (which typically have the same variable name as \a
  name).
  
  \see positions, anchor
*/
QCPItemPosition *QCPAbstractItem::position(const QString &name) const
{
  for (int i=0; i<mPositions.size(); ++i)
  {
    if (mPositions.at(i)->name() == name)
      return mPositions.at(i);
  }
  qDebug() << Q_FUNC_INFO << "position with name not found:" << name;
  return 0;
}

/*!
  Returns the QCPItemAnchor with the specified \a name. If this item doesn't have an anchor by
  that name, returns 0.
  
  This function provides an alternative way to access item anchors. Normally, you access
  anchors direcly by their member pointers (which typically have the same variable name as \a
  name).
  
  \see anchors, position
*/
QCPItemAnchor *QCPAbstractItem::anchor(const QString &name) const
{
  for (int i=0; i<mAnchors.size(); ++i)
  {
    if (mAnchors.at(i)->name() == name)
      return mAnchors.at(i);
  }
  qDebug() << Q_FUNC_INFO << "anchor with name not found:" << name;
  return 0;
}

/*!
  Returns whether this item has an anchor with the specified \a name.
  
  Note that you can check for positions with this function, too. This is because every position is
  also an anchor (QCPItemPosition inherits from QCPItemAnchor).
  
  \see anchor, position
*/
bool QCPAbstractItem::hasAnchor(const QString &name) const
{
  for (int i=0; i<mAnchors.size(); ++i)
  {
    if (mAnchors.at(i)->name() == name)
      return true;
  }
  return false;
}

/*! \internal
  
  Returns the rect the visual representation of this item is clipped to. This depends on the
  current setting of \ref setClipToAxisRect as well as the axis rect set with \ref setClipAxisRect.
  
  If the item is not clipped to an axis rect, the \ref QCustomPlot::viewport rect is returned.
  
  \see draw
*/
QRect QCPAbstractItem::clipRect() const
{
  if (mClipToAxisRect && mClipAxisRect)
    return mClipAxisRect.data()->rect();
  else
    return mParentPlot->viewport();
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing item lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased
*/
void QCPAbstractItem::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiased, QCP::aeItems);
}

/*! \internal

  Finds the shortest squared distance of \a point to the line segment defined by \a start and \a
  end.
  
  This function may be used to help with the implementation of the \ref selectTest function for
  specific items.
  
  \note This function is identical to QCPAbstractPlottable::distSqrToLine
  
  \see rectSelectTest
*/
double QCPAbstractItem::distSqrToLine(const QPointF &start, const QPointF &end, const QPointF &point) const
{
  QVector2D a(start);
  QVector2D b(end);
  QVector2D p(point);
  QVector2D v(b-a);
  
  double vLengthSqr = v.lengthSquared();
  if (!qFuzzyIsNull(vLengthSqr))
  {
    double mu = QVector2D::dotProduct(p-a, v)/vLengthSqr;
    if (mu < 0)
      return (a-p).lengthSquared();
    else if (mu > 1)
      return (b-p).lengthSquared();
    else
      return ((a + mu*v)-p).lengthSquared();
  } else
    return (a-p).lengthSquared();
}

/*! \internal

  A convenience function which returns the selectTest value for a specified \a rect and a specified
  click position \a pos. \a filledRect defines whether a click inside the rect should also be
  considered a hit or whether only the rect border is sensitive to hits.
  
  This function may be used to help with the implementation of the \ref selectTest function for
  specific items.
  
  For example, if your item consists of four rects, call this function four times, once for each
  rect, in your \ref selectTest reimplementation. Finally, return the minimum of all four returned
  values which were greater or equal to zero. (Because this function may return -1.0 when \a pos
  doesn't hit \a rect at all). If all calls returned -1.0, return -1.0, too, because your item
  wasn't hit.
  
  \see distSqrToLine
*/
double QCPAbstractItem::rectSelectTest(const QRectF &rect, const QPointF &pos, bool filledRect) const
{
  double result = -1;

  // distance to border:
  QList<QLineF> lines;
  lines << QLineF(rect.topLeft(), rect.topRight()) << QLineF(rect.bottomLeft(), rect.bottomRight())
        << QLineF(rect.topLeft(), rect.bottomLeft()) << QLineF(rect.topRight(), rect.bottomRight());
  double minDistSqr = std::numeric_limits<double>::max();
  for (int i=0; i<lines.size(); ++i)
  {
    double distSqr = distSqrToLine(lines.at(i).p1(), lines.at(i).p2(), pos);
    if (distSqr < minDistSqr)
      minDistSqr = distSqr;
  }
  result = qSqrt(minDistSqr);
  
  // filled rect, allow click inside to count as hit:
  if (filledRect && result > mParentPlot->selectionTolerance()*0.99)
  {
    if (rect.contains(pos))
      result = mParentPlot->selectionTolerance()*0.99;
  }
  return result;
}

/*! \internal

  Returns the pixel position of the anchor with Id \a anchorId. This function must be reimplemented in
  item subclasses if they want to provide anchors (QCPItemAnchor).
  
  For example, if the item has two anchors with id 0 and 1, this function takes one of these anchor
  ids and returns the respective pixel points of the specified anchor.
  
  \see createAnchor
*/
QPointF QCPAbstractItem::anchorPixelPoint(int anchorId) const
{
  qDebug() << Q_FUNC_INFO << "called on item which shouldn't have any anchors (this method not reimplemented). anchorId" << anchorId;
  return QPointF();
}

/*! \internal

  Creates a QCPItemPosition, registers it with this item and returns a pointer to it. The specified
  \a name must be a unique string that is usually identical to the variable name of the position
  member (This is needed to provide the name-based \ref position access to positions).
  
  Don't delete positions created by this function manually, as the item will take care of it.
  
  Use this function in the constructor (initialization list) of the specific item subclass to
  create each position member. Don't create QCPItemPositions with \b new yourself, because they
  won't be registered with the item properly.
  
  \see createAnchor
*/
QCPItemPosition *QCPAbstractItem::createPosition(const QString &name)
{
  if (hasAnchor(name))
    qDebug() << Q_FUNC_INFO << "anchor/position with name exists already:" << name;
  QCPItemPosition *newPosition = new QCPItemPosition(mParentPlot, this, name);
  mPositions.append(newPosition);
  mAnchors.append(newPosition); // every position is also an anchor
  newPosition->setAxes(mParentPlot->xAxis, mParentPlot->yAxis);
  newPosition->setType(QCPItemPosition::ptPlotCoords);
  if (mParentPlot->axisRect())
    newPosition->setAxisRect(mParentPlot->axisRect());
  newPosition->setCoords(0, 0);
  return newPosition;
}

/*! \internal

  Creates a QCPItemAnchor, registers it with this item and returns a pointer to it. The specified
  \a name must be a unique string that is usually identical to the variable name of the anchor
  member (This is needed to provide the name based \ref anchor access to anchors).
  
  The \a anchorId must be a number identifying the created anchor. It is recommended to create an
  enum (e.g. "AnchorIndex") for this on each item that uses anchors. This id is used by the anchor
  to identify itself when it calls QCPAbstractItem::anchorPixelPoint. That function then returns
  the correct pixel coordinates for the passed anchor id.
  
  Don't delete anchors created by this function manually, as the item will take care of it.
  
  Use this function in the constructor (initialization list) of the specific item subclass to
  create each anchor member. Don't create QCPItemAnchors with \b new yourself, because then they
  won't be registered with the item properly.
  
  \see createPosition
*/
QCPItemAnchor *QCPAbstractItem::createAnchor(const QString &name, int anchorId)
{
  if (hasAnchor(name))
    qDebug() << Q_FUNC_INFO << "anchor/position with name exists already:" << name;
  QCPItemAnchor *newAnchor = new QCPItemAnchor(mParentPlot, this, name, anchorId);
  mAnchors.append(newAnchor);
  return newAnchor;
}

/* inherits documentation from base class */
void QCPAbstractItem::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
  Q_UNUSED(event)
  Q_UNUSED(details)
  if (mSelectable)
  {
    bool selBefore = mSelected;
    setSelected(additive ? !mSelected : true);
    if (selectionStateChanged)
      *selectionStateChanged = mSelected != selBefore;
  }
}

/* inherits documentation from base class */
void QCPAbstractItem::deselectEvent(bool *selectionStateChanged)
{
  if (mSelectable)
  {
    bool selBefore = mSelected;
    setSelected(false);
    if (selectionStateChanged)
      *selectionStateChanged = mSelected != selBefore;
  }
}

/* inherits documentation from base class */
QCP::Interaction QCPAbstractItem::selectionCategory() const
{
  return QCP::iSelectItems;
}


/*! \file */



/*! \mainpage %QCustomPlot 1.2.1 Documentation

  \image html qcp-doc-logo.png
  
  Below is a brief overview of and guide to the classes and their relations. If you are new to
  QCustomPlot and just want to start using it, it's recommended to look at the tutorials and
  examples at
 
  http://www.qcustomplot.com/
 
  This documentation is especially helpful as a reference, when you're familiar with the basic
  concept of how to use %QCustomPlot and you wish to learn more about specific functionality.
  See the \ref classoverview "class overview" for diagrams explaining the relationships between
  the most important classes of the QCustomPlot library.
  
  The central widget which displays the plottables and axes on its surface is QCustomPlot. Every
  QCustomPlot contains four axes by default. They can be accessed via the members \ref
  QCustomPlot::xAxis "xAxis", \ref QCustomPlot::yAxis "yAxis", \ref QCustomPlot::xAxis2 "xAxis2"
  and \ref QCustomPlot::yAxis2 "yAxis2", and are of type QCPAxis. QCustomPlot supports an arbitrary
  number of axes and axis rects, see the documentation of QCPAxisRect for details.

  \section mainpage-plottables Plottables
  
  \a Plottables are classes that display any kind of data inside the QCustomPlot. They all derive
  from QCPAbstractPlottable. For example, the QCPGraph class is a plottable that displays a graph
  inside the plot with different line styles, scatter styles, filling etc.
  
  Since plotting graphs is such a dominant use case, QCustomPlot has a special interface for working
  with QCPGraph plottables, that makes it very easy to handle them:\n
  You create a new graph with QCustomPlot::addGraph and access them with QCustomPlot::graph.
  
  For all other plottables, you need to use the normal plottable interface:\n
  First, you create an instance of the plottable you want, e.g.
  \code
  QCPCurve *newCurve = new QCPCurve(customPlot->xAxis, customPlot->yAxis);\endcode
  add it to the customPlot:
  \code
  customPlot->addPlottable(newCurve);\endcode
  and then modify the properties of the newly created plottable via the <tt>newCurve</tt> pointer.
  
  Plottables (including graphs) can be retrieved via QCustomPlot::plottable. Since the return type
  of that function is the abstract base class of all plottables, QCPAbstractPlottable, you will
  probably want to qobject_cast the returned pointer to the respective plottable subclass. (As
  usual, if the cast returns zero, the plottable wasn't of that specific subclass.)
  
  All further interfacing with plottables (e.g how to set data) is specific to the plottable type.
  See the documentations of the subclasses: QCPGraph, QCPCurve, QCPBars, QCPStatisticalBox,
  QCPColorMap.

  \section mainpage-axes Controlling the Axes
  
  As mentioned, QCustomPlot has four axes by default: \a xAxis (bottom), \a yAxis (left), \a xAxis2
  (top), \a yAxis2 (right).
  
  Their range is handled by the simple QCPRange class. You can set the range with the
  QCPAxis::setRange function. By default, the axes represent a linear scale. To set a logarithmic
  scale, set \ref QCPAxis::setScaleType to \ref QCPAxis::stLogarithmic. The logarithm base can be set freely
  with \ref QCPAxis::setScaleLogBase.
  
  By default, an axis automatically creates and labels ticks in a sensible manner. See the
  following functions for tick manipulation:\n QCPAxis::setTicks, QCPAxis::setAutoTicks,
  QCPAxis::setAutoTickCount, QCPAxis::setAutoTickStep, QCPAxis::setTickLabels,
  QCPAxis::setTickLabelType, QCPAxis::setTickLabelRotation, QCPAxis::setTickStep,
  QCPAxis::setTickLength,...
  
  Each axis can be given an axis label (e.g. "Voltage (mV)") with QCPAxis::setLabel.
  
  The distance of an axis backbone to the respective viewport border is called its margin.
  Normally, the margins are calculated automatically. To change this, set
  \ref QCPAxisRect::setAutoMargins to exclude the respective margin sides, set the margins manually with
  \ref QCPAxisRect::setMargins. The main axis rect can be reached with \ref QCustomPlot::axisRect().
  
  \section mainpage-legend Plot Legend
  
  Every QCustomPlot has one QCPLegend (as \ref QCustomPlot::legend) by default. A legend is a small
  layout element inside the plot which lists the plottables with an icon of the plottable
  line/symbol and a name (QCPAbstractPlottable::setName). Plottables can be added and removed from
  the main legend via \ref QCPAbstractPlottable::addToLegend and \ref
  QCPAbstractPlottable::removeFromLegend. By default, adding a plottable to QCustomPlot
  automatically adds it to the legend, too. This behaviour can be modified with the
  QCustomPlot::setAutoAddPlottableToLegend property.
  
  The QCPLegend provides an interface to access, add and remove legend items directly, too. See
  QCPLegend::item, QCPLegend::itemWithPlottable, QCPLegend::addItem, QCPLegend::removeItem for
  example.
  
  Multiple legends are supported via the \link thelayoutsystem layout system\endlink (as a
  QCPLegend simply is a normal layout element).
  
  \section mainpage-userinteraction User Interactions
  
  QCustomPlot supports dragging axis ranges with the mouse (\ref
  QCPAxisRect::setRangeDrag), zooming axis ranges with the mouse wheel (\ref
  QCPAxisRect::setRangeZoom) and a complete selection mechanism.
  
  The availability of these interactions is controlled with \ref QCustomPlot::setInteractions. For
  details about the interaction system, see the documentation there.
  
  Further, QCustomPlot always emits corresponding signals, when objects are clicked or
  doubleClicked. See \ref QCustomPlot::plottableClick, \ref QCustomPlot::plottableDoubleClick
  and \ref QCustomPlot::axisClick for example.
  
  \section mainpage-items Items
  
  Apart from plottables there is another category of plot objects that are important: Items. The
  base class of all items is QCPAbstractItem. An item sets itself apart from plottables in that
  it's not necessarily bound to any axes. This means it may also be positioned in absolute pixel
  coordinates or placed at a relative position on an axis rect. Further, it usually doesn't
  represent data directly, but acts as decoration, emphasis, description etc.
  
  Multiple items can be arranged in a parent-child-hierarchy allowing for dynamical behaviour. For
  example, you could place the head of an arrow at a fixed plot coordinate, so it always points to
  some important area in the plot. The tail of the arrow can be anchored to a text item which
  always resides in the top center of the axis rect, independent of where the user drags the axis
  ranges. This way the arrow stretches and turns so it always points from the label to the
  specified plot coordinate, without any further code necessary.
  
  For a more detailed introduction, see the QCPAbstractItem documentation, and from there the
  documentations of the individual built-in items, to find out how to use them.
  
  \section mainpage-layoutelements Layout elements and layouts
  
  QCustomPlot uses an internal layout system to provide dynamic sizing and positioning of objects like
  the axis rect(s), legends and the plot title. They are all based on \ref QCPLayoutElement and are arranged by
  placing them inside a \ref QCPLayout.
  
  Details on this topic are given on the dedicated page about \link thelayoutsystem the layout system\endlink.
  
  \section mainpage-performancetweaks Performance Tweaks
  
  Although QCustomPlot is quite fast, some features like translucent fills, antialiasing and thick
  lines can cause a significant slow down. If you notice this in your application, here are some
  thoughts on how to increase performance. By far the most time is spent in the drawing functions,
  specifically the drawing of graphs. For maximum performance, consider the following (most
  recommended/effective measures first):
  
  \li use Qt 4.8.0 and up. Performance has doubled or tripled with respect to Qt 4.7.4. However
  QPainter was broken and drawing pixel precise things, e.g. scatters, isn't possible with Qt >=
  4.8.0. So it's a performance vs. plot quality tradeoff when switching to Qt 4.8.
  \li To increase responsiveness during dragging, consider setting \ref QCustomPlot::setNoAntialiasingOnDrag to true.
  \li On X11 (GNU/Linux), avoid the slow native drawing system, use raster by supplying
  "-graphicssystem raster" as command line argument or calling QApplication::setGraphicsSystem("raster")
  before creating the QApplication object. (Only available for Qt versions before 5.0)
  \li On all operating systems, use OpenGL hardware acceleration by supplying "-graphicssystem
  opengl" as command line argument or calling QApplication::setGraphicsSystem("opengl") (Only
  available for Qt versions before 5.0). If OpenGL is available, this will slightly decrease the
  quality of antialiasing, but extremely increase performance especially with alpha
  (semi-transparent) fills, much antialiasing and a large QCustomPlot drawing surface. Note
  however, that the maximum frame rate might be constrained by the vertical sync frequency of your
  monitor (VSync can be disabled in the graphics card driver configuration). So for simple plots
  (where the potential framerate is far above 60 frames per second), OpenGL acceleration might
  achieve numerically lower frame rates than the other graphics systems, because they are not
  capped at the VSync frequency.
  \li Avoid any kind of alpha (transparency), especially in fills
  \li Avoid lines with a pen width greater than one
  \li Avoid any kind of antialiasing, especially in graph lines (see \ref QCustomPlot::setNotAntialiasedElements)
  \li Avoid repeatedly setting the complete data set with \ref QCPGraph::setData. Use \ref QCPGraph::addData instead, if most
  data points stay unchanged, e.g. in a running measurement.
  \li Set the \a copy parameter of the setData functions to false, so only pointers get
  transferred. (Relevant only if preparing data maps with a large number of points, i.e. over 10000)
  
  \section mainpage-flags Preprocessor Define Flags
  
  QCustomPlot understands some preprocessor defines that are useful for debugging and compilation:
  <dl>
  <dt>\c QCUSTOMPLOT_COMPILE_LIBRARY
  <dd>Define this flag when you compile QCustomPlot as a shared library (.so/.dll)
  <dt>\c QCUSTOMPLOT_USE_LIBRARY
  <dd>Define this flag before including the header, when using QCustomPlot as a shared library
  <dt>\c QCUSTOMPLOT_CHECK_DATA
  <dd>If this flag is defined, the QCustomPlot plottables will perform data validity checks on every redraw.
      This means they will give qDebug output when you plot \e inf or \e nan values, they will not
      fix your data.
  </dl>

*/

/*! \page classoverview Class Overview
  
  The following diagrams may help to gain a deeper understanding of the relationships between classes that make up
  the QCustomPlot library. The diagrams are not exhaustive, so only the classes deemed most relevant are shown.
  
  \section classoverview-relations Class Relationship Diagram
  \image html RelationOverview.png "Overview of most important classes and their relations"
  \section classoverview-inheritance Class Inheritance Tree
  \image html InheritanceOverview.png "Inheritance tree of most important classes"
  
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCustomPlot
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCustomPlot
  
  \brief The central class of the library. This is the QWidget which displays the plot and
  interacts with the user.
  
  For tutorials on how to use QCustomPlot, see the website\n
  http://www.qcustomplot.com/
*/

/* start of documentation of inline functions */

/*! \fn QRect QCustomPlot::viewport() const
  
  Returns the viewport rect of this QCustomPlot instance. The viewport is the area the plot is
  drawn in, all mechanisms, e.g. margin caluclation take the viewport to be the outer border of the
  plot. The viewport normally is the rect() of the QCustomPlot widget, i.e. a rect with top left
  (0, 0) and size of the QCustomPlot widget.
  
  Don't confuse the viewport with the axis rect (QCustomPlot::axisRect). An axis rect is typically
  an area enclosed by four axes, where the graphs/plottables are drawn in. The viewport is larger
  and contains also the axes themselves, their tick numbers, their labels, the plot title etc.
  
  Only when saving to a file (see \ref savePng, savePdf etc.) the viewport is temporarily modified
  to allow saving plots with sizes independent of the current widget size.
*/

/*! \fn QCPLayoutGrid *QCustomPlot::plotLayout() const
  
  Returns the top level layout of this QCustomPlot instance. It is a \ref QCPLayoutGrid, initially containing just
  one cell with the main QCPAxisRect inside.
*/

/* end of documentation of inline functions */
/* start of documentation of signals */

/*! \fn void QCustomPlot::mouseDoubleClick(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse double click event.
*/

/*! \fn void QCustomPlot::mousePress(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse press event.
  
  It is emitted before QCustomPlot handles any other mechanism like range dragging. So a slot
  connected to this signal can still influence the behaviour e.g. with \ref QCPAxisRect::setRangeDrag or \ref
  QCPAxisRect::setRangeDragAxes.
*/

/*! \fn void QCustomPlot::mouseMove(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse move event.
  
  It is emitted before QCustomPlot handles any other mechanism like range dragging. So a slot
  connected to this signal can still influence the behaviour e.g. with \ref QCPAxisRect::setRangeDrag or \ref
  QCPAxisRect::setRangeDragAxes.
  
  \warning It is discouraged to change the drag-axes with \ref QCPAxisRect::setRangeDragAxes here,
  because the dragging starting point was saved the moment the mouse was pressed. Thus it only has
  a meaning for the range drag axes that were set at that moment. If you want to change the drag
  axes, consider doing this in the \ref mousePress signal instead.
*/

/*! \fn void QCustomPlot::mouseRelease(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse release event.
  
  It is emitted before QCustomPlot handles any other mechanisms like object selection. So a
  slot connected to this signal can still influence the behaviour e.g. with \ref setInteractions or
  \ref QCPAbstractPlottable::setSelectable.
*/

/*! \fn void QCustomPlot::mouseWheel(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse wheel event.
  
  It is emitted before QCustomPlot handles any other mechanisms like range zooming. So a slot
  connected to this signal can still influence the behaviour e.g. with \ref QCPAxisRect::setRangeZoom, \ref
  QCPAxisRect::setRangeZoomAxes or \ref QCPAxisRect::setRangeZoomFactor.
*/

/*! \fn void QCustomPlot::plottableClick(QCPAbstractPlottable *plottable, QMouseEvent *event)
  
  This signal is emitted when a plottable is clicked.

  \a event is the mouse event that caused the click and \a plottable is the plottable that received
  the click.
  
  \see plottableDoubleClick
*/

/*! \fn void QCustomPlot::plottableDoubleClick(QCPAbstractPlottable *plottable, QMouseEvent *event)
  
  This signal is emitted when a plottable is double clicked.
  
  \a event is the mouse event that caused the click and \a plottable is the plottable that received
  the click.
  
  \see plottableClick
*/

/*! \fn void QCustomPlot::itemClick(QCPAbstractItem *item, QMouseEvent *event)
  
  This signal is emitted when an item is clicked.

  \a event is the mouse event that caused the click and \a item is the item that received the
  click.
  
  \see itemDoubleClick
*/

/*! \fn void QCustomPlot::itemDoubleClick(QCPAbstractItem *item, QMouseEvent *event)
  
  This signal is emitted when an item is double clicked.
  
  \a event is the mouse event that caused the click and \a item is the item that received the
  click.
  
  \see itemClick
*/

/*! \fn void QCustomPlot::axisClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event)
  
  This signal is emitted when an axis is clicked.
  
  \a event is the mouse event that caused the click, \a axis is the axis that received the click and
  \a part indicates the part of the axis that was clicked.
  
  \see axisDoubleClick
*/

/*! \fn void QCustomPlot::axisDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event)

  This signal is emitted when an axis is double clicked.
  
  \a event is the mouse event that caused the click, \a axis is the axis that received the click and
  \a part indicates the part of the axis that was clicked.
  
  \see axisClick
*/

/*! \fn void QCustomPlot::legendClick(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event)

  This signal is emitted when a legend (item) is clicked.
  
  \a event is the mouse event that caused the click, \a legend is the legend that received the
  click and \a item is the legend item that received the click. If only the legend and no item is
  clicked, \a item is 0. This happens for a click inside the legend padding or the space between
  two items.
  
  \see legendDoubleClick
*/

/*! \fn void QCustomPlot::legendDoubleClick(QCPLegend *legend,  QCPAbstractLegendItem *item, QMouseEvent *event)

  This signal is emitted when a legend (item) is double clicked.
  
  \a event is the mouse event that caused the click, \a legend is the legend that received the
  click and \a item is the legend item that received the click. If only the legend and no item is
  clicked, \a item is 0. This happens for a click inside the legend padding or the space between
  two items.
  
  \see legendClick
*/

/*! \fn void QCustomPlot:: titleClick(QMouseEvent *event, QCPPlotTitle *title)

  This signal is emitted when a plot title is clicked.
  
  \a event is the mouse event that caused the click and \a title is the plot title that received
  the click.
  
  \see titleDoubleClick
*/

/*! \fn void QCustomPlot::titleDoubleClick(QMouseEvent *event, QCPPlotTitle *title)

  This signal is emitted when a plot title is double clicked.
  
  \a event is the mouse event that caused the click and \a title is the plot title that received
  the click.
  
  \see titleClick
*/

/*! \fn void QCustomPlot::selectionChangedByUser()
  
  This signal is emitted after the user has changed the selection in the QCustomPlot, e.g. by
  clicking. It is not emitted when the selection state of an object has changed programmatically by
  a direct call to setSelected() on an object or by calling \ref deselectAll.
  
  In addition to this signal, selectable objects also provide individual signals, for example
  QCPAxis::selectionChanged or QCPAbstractPlottable::selectionChanged. Note that those signals are
  emitted even if the selection state is changed programmatically.
  
  See the documentation of \ref setInteractions for details about the selection mechanism.
  
  \see selectedPlottables, selectedGraphs, selectedItems, selectedAxes, selectedLegends
*/

/*! \fn void QCustomPlot::beforeReplot()
  
  This signal is emitted immediately before a replot takes place (caused by a call to the slot \ref
  replot).
  
  It is safe to mutually connect the replot slot with this signal on two QCustomPlots to make them
  replot synchronously, it won't cause an infinite recursion.
  
  \see replot, afterReplot
*/

/*! \fn void QCustomPlot::afterReplot()
  
  This signal is emitted immediately after a replot has taken place (caused by a call to the slot \ref
  replot).
  
  It is safe to mutually connect the replot slot with this signal on two QCustomPlots to make them
  replot synchronously, it won't cause an infinite recursion.
  
  \see replot, beforeReplot
*/

/* end of documentation of signals */
/* start of documentation of public members */

/*! \var QCPAxis *QCustomPlot::xAxis

  A pointer to the primary x Axis (bottom) of the main axis rect of the plot.
  
  QCustomPlot offers convenient pointers to the axes (\ref xAxis, \ref yAxis, \ref xAxis2, \ref
  yAxis2) and the \ref legend. They make it very easy working with plots that only have a single
  axis rect and at most one axis at each axis rect side. If you use \link thelayoutsystem the
  layout system\endlink to add multiple axis rects or multiple axes to one side, use the \ref
  QCPAxisRect::axis interface to access the new axes. If one of the four default axes or the
  default legend is removed due to manipulation of the layout system (e.g. by removing the main
  axis rect), the corresponding pointers become 0.
*/

/*! \var QCPAxis *QCustomPlot::yAxis

  A pointer to the primary y Axis (left) of the main axis rect of the plot.
  
  QCustomPlot offers convenient pointers to the axes (\ref xAxis, \ref yAxis, \ref xAxis2, \ref
  yAxis2) and the \ref legend. They make it very easy working with plots that only have a single
  axis rect and at most one axis at each axis rect side. If you use \link thelayoutsystem the
  layout system\endlink to add multiple axis rects or multiple axes to one side, use the \ref
  QCPAxisRect::axis interface to access the new axes. If one of the four default axes or the
  default legend is removed due to manipulation of the layout system (e.g. by removing the main
  axis rect), the corresponding pointers become 0.
*/

/*! \var QCPAxis *QCustomPlot::xAxis2

  A pointer to the secondary x Axis (top) of the main axis rect of the plot. Secondary axes are
  invisible by default. Use QCPAxis::setVisible to change this (or use \ref
  QCPAxisRect::setupFullAxesBox).
  
  QCustomPlot offers convenient pointers to the axes (\ref xAxis, \ref yAxis, \ref xAxis2, \ref
  yAxis2) and the \ref legend. They make it very easy working with plots that only have a single
  axis rect and at most one axis at each axis rect side. If you use \link thelayoutsystem the
  layout system\endlink to add multiple axis rects or multiple axes to one side, use the \ref
  QCPAxisRect::axis interface to access the new axes. If one of the four default axes or the
  default legend is removed due to manipulation of the layout system (e.g. by removing the main
  axis rect), the corresponding pointers become 0.
*/

/*! \var QCPAxis *QCustomPlot::yAxis2

  A pointer to the secondary y Axis (right) of the main axis rect of the plot. Secondary axes are
  invisible by default. Use QCPAxis::setVisible to change this (or use \ref
  QCPAxisRect::setupFullAxesBox).
  
  QCustomPlot offers convenient pointers to the axes (\ref xAxis, \ref yAxis, \ref xAxis2, \ref
  yAxis2) and the \ref legend. They make it very easy working with plots that only have a single
  axis rect and at most one axis at each axis rect side. If you use \link thelayoutsystem the
  layout system\endlink to add multiple axis rects or multiple axes to one side, use the \ref
  QCPAxisRect::axis interface to access the new axes. If one of the four default axes or the
  default legend is removed due to manipulation of the layout system (e.g. by removing the main
  axis rect), the corresponding pointers become 0.
*/

/*! \var QCPLegend *QCustomPlot::legend

  A pointer to the default legend of the main axis rect. The legend is invisible by default. Use
  QCPLegend::setVisible to change this.
  
  QCustomPlot offers convenient pointers to the axes (\ref xAxis, \ref yAxis, \ref xAxis2, \ref
  yAxis2) and the \ref legend. They make it very easy working with plots that only have a single
  axis rect and at most one axis at each axis rect side. If you use \link thelayoutsystem the
  layout system\endlink to add multiple legends to the plot, use the layout system interface to
  access the new legend. For example, legends can be placed inside an axis rect's \ref
  QCPAxisRect::insetLayout "inset layout", and must then also be accessed via the inset layout. If
  the default legend is removed due to manipulation of the layout system (e.g. by removing the main
  axis rect), the corresponding pointer becomes 0.
*/

/* end of documentation of public members */

/*!
  Constructs a QCustomPlot and sets reasonable default values.
*/
QCustomPlot::QCustomPlot(QWidget *parent) :
  QWidget(parent),
  xAxis(0),
  yAxis(0),
  xAxis2(0),
  yAxis2(0),
  legend(0),
  mPlotLayout(0),
  mAutoAddPlottableToLegend(true),
  mAntialiasedElements(QCP::aeNone),
  mNotAntialiasedElements(QCP::aeNone),
  mInteractions(0),
  mSelectionTolerance(8),
  mNoAntialiasingOnDrag(false),
  mBackgroundBrush(Qt::white, Qt::SolidPattern),
  mBackgroundScaled(true),
  mBackgroundScaledMode(Qt::KeepAspectRatioByExpanding),
  mCurrentLayer(0),
  mPlottingHints(QCP::phCacheLabels|QCP::phForceRepaint),
  mMultiSelectModifier(Qt::ControlModifier),
  mPaintBuffer(size()),
  mMouseEventElement(0),
  mReplotting(false)
{
  setAttribute(Qt::WA_NoMousePropagation);
  setAttribute(Qt::WA_OpaquePaintEvent);
  setMouseTracking(true);
  QLocale currentLocale = locale();
  currentLocale.setNumberOptions(QLocale::OmitGroupSeparator);
  setLocale(currentLocale);
  
  // create initial layers:
  mLayers.append(new QCPLayer(this, "background"));
  mLayers.append(new QCPLayer(this, "grid"));
  mLayers.append(new QCPLayer(this, "main"));
  mLayers.append(new QCPLayer(this, "axes"));
  mLayers.append(new QCPLayer(this, "legend"));
  updateLayerIndices();
  setCurrentLayer("main");
  
  // create initial layout, axis rect and legend:
  mPlotLayout = new QCPLayoutGrid;
  mPlotLayout->initializeParentPlot(this);
  mPlotLayout->setParent(this); // important because if parent is QWidget, QCPLayout::sizeConstraintsChanged will call QWidget::updateGeometry
  mPlotLayout->setLayer("main");
  QCPAxisRect *defaultAxisRect = new QCPAxisRect(this, true);
  mPlotLayout->addElement(0, 0, defaultAxisRect);
  xAxis = defaultAxisRect->axis(QCPAxis::atBottom);
  yAxis = defaultAxisRect->axis(QCPAxis::atLeft);
  xAxis2 = defaultAxisRect->axis(QCPAxis::atTop);
  yAxis2 = defaultAxisRect->axis(QCPAxis::atRight);
  legend = new QCPLegend;
  legend->setVisible(false);
  defaultAxisRect->insetLayout()->addElement(legend, Qt::AlignRight|Qt::AlignTop);
  defaultAxisRect->insetLayout()->setMargins(QMargins(12, 12, 12, 12));
  
  defaultAxisRect->setLayer("background");
  xAxis->setLayer("axes");
  yAxis->setLayer("axes");
  xAxis2->setLayer("axes");
  yAxis2->setLayer("axes");
  xAxis->grid()->setLayer("grid");
  yAxis->grid()->setLayer("grid");
  xAxis2->grid()->setLayer("grid");
  yAxis2->grid()->setLayer("grid");
  legend->setLayer("legend");
  
  setViewport(rect()); // needs to be called after mPlotLayout has been created
  
  replot();
}

QCustomPlot::~QCustomPlot()
{
  clearPlottables();
  clearItems();

  if (mPlotLayout)
  {
    delete mPlotLayout;
    mPlotLayout = 0;
  }
  
  mCurrentLayer = 0;
  qDeleteAll(mLayers); // don't use removeLayer, because it would prevent the last layer to be removed
  mLayers.clear();
}

/*!
  Sets which elements are forcibly drawn antialiased as an \a or combination of QCP::AntialiasedElement.
  
  This overrides the antialiasing settings for whole element groups, normally controlled with the
  \a setAntialiasing function on the individual elements. If an element is neither specified in
  \ref setAntialiasedElements nor in \ref setNotAntialiasedElements, the antialiasing setting on
  each individual element instance is used.
  
  For example, if \a antialiasedElements contains \ref QCP::aePlottables, all plottables will be
  drawn antialiased, no matter what the specific QCPAbstractPlottable::setAntialiased value was set
  to.
  
  if an element in \a antialiasedElements is already set in \ref setNotAntialiasedElements, it is
  removed from there.
  
  \see setNotAntialiasedElements
*/
void QCustomPlot::setAntialiasedElements(const QCP::AntialiasedElements &antialiasedElements)
{
  mAntialiasedElements = antialiasedElements;
  
  // make sure elements aren't in mNotAntialiasedElements and mAntialiasedElements simultaneously:
  if ((mNotAntialiasedElements & mAntialiasedElements) != 0)
    mNotAntialiasedElements |= ~mAntialiasedElements;
}

/*!
  Sets whether the specified \a antialiasedElement is forcibly drawn antialiased.
  
  See \ref setAntialiasedElements for details.
  
  \see setNotAntialiasedElement
*/
void QCustomPlot::setAntialiasedElement(QCP::AntialiasedElement antialiasedElement, bool enabled)
{
  if (!enabled && mAntialiasedElements.testFlag(antialiasedElement))
    mAntialiasedElements &= ~antialiasedElement;
  else if (enabled && !mAntialiasedElements.testFlag(antialiasedElement))
    mAntialiasedElements |= antialiasedElement;
  
  // make sure elements aren't in mNotAntialiasedElements and mAntialiasedElements simultaneously:
  if ((mNotAntialiasedElements & mAntialiasedElements) != 0)
    mNotAntialiasedElements |= ~mAntialiasedElements;
}

/*!
  Sets which elements are forcibly drawn not antialiased as an \a or combination of
  QCP::AntialiasedElement.
  
  This overrides the antialiasing settings for whole element groups, normally controlled with the
  \a setAntialiasing function on the individual elements. If an element is neither specified in
  \ref setAntialiasedElements nor in \ref setNotAntialiasedElements, the antialiasing setting on
  each individual element instance is used.
  
  For example, if \a notAntialiasedElements contains \ref QCP::aePlottables, no plottables will be
  drawn antialiased, no matter what the specific QCPAbstractPlottable::setAntialiased value was set
  to.
  
  if an element in \a notAntialiasedElements is already set in \ref setAntialiasedElements, it is
  removed from there.
  
  \see setAntialiasedElements
*/
void QCustomPlot::setNotAntialiasedElements(const QCP::AntialiasedElements &notAntialiasedElements)
{
  mNotAntialiasedElements = notAntialiasedElements;
  
  // make sure elements aren't in mNotAntialiasedElements and mAntialiasedElements simultaneously:
  if ((mNotAntialiasedElements & mAntialiasedElements) != 0)
    mAntialiasedElements |= ~mNotAntialiasedElements;
}

/*!
  Sets whether the specified \a notAntialiasedElement is forcibly drawn not antialiased.
  
  See \ref setNotAntialiasedElements for details.
  
  \see setAntialiasedElement
*/
void QCustomPlot::setNotAntialiasedElement(QCP::AntialiasedElement notAntialiasedElement, bool enabled)
{
  if (!enabled && mNotAntialiasedElements.testFlag(notAntialiasedElement))
    mNotAntialiasedElements &= ~notAntialiasedElement;
  else if (enabled && !mNotAntialiasedElements.testFlag(notAntialiasedElement))
    mNotAntialiasedElements |= notAntialiasedElement;
  
  // make sure elements aren't in mNotAntialiasedElements and mAntialiasedElements simultaneously:
  if ((mNotAntialiasedElements & mAntialiasedElements) != 0)
    mAntialiasedElements |= ~mNotAntialiasedElements;
}

/*!
  If set to true, adding a plottable (e.g. a graph) to the QCustomPlot automatically also adds the
  plottable to the legend (QCustomPlot::legend).
  
  \see addPlottable, addGraph, QCPLegend::addItem
*/
void QCustomPlot::setAutoAddPlottableToLegend(bool on)
{
  mAutoAddPlottableToLegend = on;
}

/*!
  Sets the possible interactions of this QCustomPlot as an or-combination of \ref QCP::Interaction
  enums. There are the following types of interactions:
  
  <b>Axis range manipulation</b> is controlled via \ref QCP::iRangeDrag and \ref QCP::iRangeZoom. When the
  respective interaction is enabled, the user may drag axes ranges and zoom with the mouse wheel.
  For details how to control which axes the user may drag/zoom and in what orientations, see \ref
  QCPAxisRect::setRangeDrag, \ref QCPAxisRect::setRangeZoom, \ref QCPAxisRect::setRangeDragAxes,
  \ref QCPAxisRect::setRangeZoomAxes.
  
  <b>Plottable selection</b> is controlled by \ref QCP::iSelectPlottables. If \ref QCP::iSelectPlottables is
  set, the user may select plottables (graphs, curves, bars,...) by clicking on them or in their
  vicinity (\ref setSelectionTolerance). Whether the user can actually select a plottable can
  further be restricted with the \ref QCPAbstractPlottable::setSelectable function on the specific
  plottable. To find out whether a specific plottable is selected, call
  QCPAbstractPlottable::selected(). To retrieve a list of all currently selected plottables, call
  \ref selectedPlottables. If you're only interested in QCPGraphs, you may use the convenience
  function \ref selectedGraphs.
  
  <b>Item selection</b> is controlled by \ref QCP::iSelectItems. If \ref QCP::iSelectItems is set, the user
  may select items (QCPItemLine, QCPItemText,...) by clicking on them or in their vicinity. To find
  out whether a specific item is selected, call QCPAbstractItem::selected(). To retrieve a list of
  all currently selected items, call \ref selectedItems.
  
  <b>Axis selection</b> is controlled with \ref QCP::iSelectAxes. If \ref QCP::iSelectAxes is set, the user
  may select parts of the axes by clicking on them. What parts exactly (e.g. Axis base line, tick
  labels, axis label) are selectable can be controlled via \ref QCPAxis::setSelectableParts for
  each axis. To retrieve a list of all axes that currently contain selected parts, call \ref
  selectedAxes. Which parts of an axis are selected, can be retrieved with QCPAxis::selectedParts().
  
  <b>Legend selection</b> is controlled with \ref QCP::iSelectLegend. If this is set, the user may
  select the legend itself or individual items by clicking on them. What parts exactly are
  selectable can be controlled via \ref QCPLegend::setSelectableParts. To find out whether the
  legend or any of its child items are selected, check the value of QCPLegend::selectedParts. To
  find out which child items are selected, call \ref QCPLegend::selectedItems.
  
  <b>All other selectable elements</b> The selection of all other selectable objects (e.g.
  QCPPlotTitle, or your own layerable subclasses) is controlled with \ref QCP::iSelectOther. If set, the
  user may select those objects by clicking on them. To find out which are currently selected, you
  need to check their selected state explicitly.
  
  If the selection state has changed by user interaction, the \ref selectionChangedByUser signal is
  emitted. Each selectable object additionally emits an individual selectionChanged signal whenever
  their selection state has changed, i.e. not only by user interaction.
  
  To allow multiple objects to be selected by holding the selection modifier (\ref
  setMultiSelectModifier), set the flag \ref QCP::iMultiSelect.
  
  \note In addition to the selection mechanism presented here, QCustomPlot always emits
  corresponding signals, when an object is clicked or double clicked. see \ref plottableClick and
  \ref plottableDoubleClick for example.
  
  \see setInteraction, setSelectionTolerance
*/
void QCustomPlot::setInteractions(const QCP::Interactions &interactions)
{
  mInteractions = interactions;
}

/*!
  Sets the single \a interaction of this QCustomPlot to \a enabled.
  
  For details about the interaction system, see \ref setInteractions.
  
  \see setInteractions
*/
void QCustomPlot::setInteraction(const QCP::Interaction &interaction, bool enabled)
{
  if (!enabled && mInteractions.testFlag(interaction))
    mInteractions &= ~interaction;
  else if (enabled && !mInteractions.testFlag(interaction))
    mInteractions |= interaction;
}

/*!
  Sets the tolerance that is used to decide whether a click selects an object (e.g. a plottable) or
  not.
  
  If the user clicks in the vicinity of the line of e.g. a QCPGraph, it's only regarded as a
  potential selection when the minimum distance between the click position and the graph line is
  smaller than \a pixels. Objects that are defined by an area (e.g. QCPBars) only react to clicks
  directly inside the area and ignore this selection tolerance. In other words, it only has meaning
  for parts of objects that are too thin to exactly hit with a click and thus need such a
  tolerance.
  
  \see setInteractions, QCPLayerable::selectTest
*/
void QCustomPlot::setSelectionTolerance(int pixels)
{
  mSelectionTolerance = pixels;
}

/*!
  Sets whether antialiasing is disabled for this QCustomPlot while the user is dragging axes
  ranges. If many objects, especially plottables, are drawn antialiased, this greatly improves
  performance during dragging. Thus it creates a more responsive user experience. As soon as the
  user stops dragging, the last replot is done with normal antialiasing, to restore high image
  quality.
  
  \see setAntialiasedElements, setNotAntialiasedElements
*/
void QCustomPlot::setNoAntialiasingOnDrag(bool enabled)
{
  mNoAntialiasingOnDrag = enabled;
}

/*!
  Sets the plotting hints for this QCustomPlot instance as an \a or combination of QCP::PlottingHint.
  
  \see setPlottingHint
*/
void QCustomPlot::setPlottingHints(const QCP::PlottingHints &hints)
{
  mPlottingHints = hints;
}

/*!
  Sets the specified plotting \a hint to \a enabled.
  
  \see setPlottingHints
*/
void QCustomPlot::setPlottingHint(QCP::PlottingHint hint, bool enabled)
{
  QCP::PlottingHints newHints = mPlottingHints;
  if (!enabled)
    newHints &= ~hint;
  else
    newHints |= hint;
  
  if (newHints != mPlottingHints)
    setPlottingHints(newHints);
}

/*!
  Sets the keyboard modifier that will be recognized as multi-select-modifier.
  
  If \ref QCP::iMultiSelect is specified in \ref setInteractions, the user may select multiple objects
  by clicking on them one after the other while holding down \a modifier.
  
  By default the multi-select-modifier is set to Qt::ControlModifier.
  
  \see setInteractions
*/
void QCustomPlot::setMultiSelectModifier(Qt::KeyboardModifier modifier)
{
  mMultiSelectModifier = modifier;
}

/*!
  Sets the viewport of this QCustomPlot. The Viewport is the area that the top level layout
  (QCustomPlot::plotLayout()) uses as its rect. Normally, the viewport is the entire widget rect.
  
  This function is used to allow arbitrary size exports with \ref toPixmap, \ref savePng, \ref
  savePdf, etc. by temporarily changing the viewport size.
*/
void QCustomPlot::setViewport(const QRect &rect)
{
  mViewport = rect;
  if (mPlotLayout)
    mPlotLayout->setOuterRect(mViewport);
}

/*!
  Sets \a pm as the viewport background pixmap (see \ref setViewport). The pixmap is always drawn
  below all other objects in the plot.

  For cases where the provided pixmap doesn't have the same size as the viewport, scaling can be
  enabled with \ref setBackgroundScaled and the scaling mode (whether and how the aspect ratio is
  preserved) can be set with \ref setBackgroundScaledMode. To set all these options in one call,
  consider using the overloaded version of this function.
  
  If a background brush was set with \ref setBackground(const QBrush &brush), the viewport will
  first be filled with that brush, before drawing the background pixmap. This can be useful for
  background pixmaps with translucent areas.

  \see setBackgroundScaled, setBackgroundScaledMode
*/
void QCustomPlot::setBackground(const QPixmap &pm)
{
  mBackgroundPixmap = pm;
  mScaledBackgroundPixmap = QPixmap();
}

/*!
  Sets the background brush of the viewport (see \ref setViewport).

  Before drawing everything else, the background is filled with \a brush. If a background pixmap
  was set with \ref setBackground(const QPixmap &pm), this brush will be used to fill the viewport
  before the background pixmap is drawn. This can be useful for background pixmaps with translucent
  areas.
  
  Set \a brush to Qt::NoBrush or Qt::Transparent to leave background transparent. This can be
  useful for exporting to image formats which support transparency, e.g. \ref savePng.

  \see setBackgroundScaled, setBackgroundScaledMode
*/
void QCustomPlot::setBackground(const QBrush &brush)
{
  mBackgroundBrush = brush;
}

/*! \overload
  
  Allows setting the background pixmap of the viewport, whether it shall be scaled and how it
  shall be scaled in one call.

  \see setBackground(const QPixmap &pm), setBackgroundScaled, setBackgroundScaledMode
*/
void QCustomPlot::setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode)
{
  mBackgroundPixmap = pm;
  mScaledBackgroundPixmap = QPixmap();
  mBackgroundScaled = scaled;
  mBackgroundScaledMode = mode;
}

/*!
  Sets whether the viewport background pixmap shall be scaled to fit the viewport. If \a scaled is
  set to true, control whether and how the aspect ratio of the original pixmap is preserved with
  \ref setBackgroundScaledMode.
  
  Note that the scaled version of the original pixmap is buffered, so there is no performance
  penalty on replots. (Except when the viewport dimensions are changed continuously.)
  
  \see setBackground, setBackgroundScaledMode
*/
void QCustomPlot::setBackgroundScaled(bool scaled)
{
  mBackgroundScaled = scaled;
}

/*!
  If scaling of the viewport background pixmap is enabled (\ref setBackgroundScaled), use this
  function to define whether and how the aspect ratio of the original pixmap is preserved.
  
  \see setBackground, setBackgroundScaled
*/
void QCustomPlot::setBackgroundScaledMode(Qt::AspectRatioMode mode)
{
  mBackgroundScaledMode = mode;
}

/*!
  Returns the plottable with \a index. If the index is invalid, returns 0.
  
  There is an overloaded version of this function with no parameter which returns the last added
  plottable, see QCustomPlot::plottable()
  
  \see plottableCount, addPlottable
*/
QCPAbstractPlottable *QCustomPlot::plottable(int index)
{
  if (index >= 0 && index < mPlottables.size())
  {
    return mPlottables.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return 0;
  }
}

/*! \overload
  
  Returns the last plottable that was added with \ref addPlottable. If there are no plottables in
  the plot, returns 0.
  
  \see plottableCount, addPlottable
*/
QCPAbstractPlottable *QCustomPlot::plottable()
{
  if (!mPlottables.isEmpty())
  {
    return mPlottables.last();
  } else
    return 0;
}

/*!
  Adds the specified plottable to the plot and, if \ref setAutoAddPlottableToLegend is enabled, to
  the legend (QCustomPlot::legend). QCustomPlot takes ownership of the plottable.
  
  Returns true on success, i.e. when \a plottable isn't already in the plot and the parent plot of
  \a plottable is this QCustomPlot (the latter is controlled by what axes were passed in the
  plottable's constructor).
  
  \see plottable, plottableCount, removePlottable, clearPlottables
*/
bool QCustomPlot::addPlottable(QCPAbstractPlottable *plottable)
{
  if (mPlottables.contains(plottable))
  {
    qDebug() << Q_FUNC_INFO << "plottable already added to this QCustomPlot:" << reinterpret_cast<quintptr>(plottable);
    return false;
  }
  if (plottable->parentPlot() != this)
  {
    qDebug() << Q_FUNC_INFO << "plottable not created with this QCustomPlot as parent:" << reinterpret_cast<quintptr>(plottable);
    return false;
  }
  
  mPlottables.append(plottable);
  // possibly add plottable to legend:
  if (mAutoAddPlottableToLegend)
    plottable->addToLegend();
  // special handling for QCPGraphs to maintain the simple graph interface:
  if (QCPGraph *graph = qobject_cast<QCPGraph*>(plottable))
    mGraphs.append(graph);
  if (!plottable->layer()) // usually the layer is already set in the constructor of the plottable (via QCPLayerable constructor)
    plottable->setLayer(currentLayer());
  return true;
}

/*!
  Removes the specified plottable from the plot and, if necessary, from the legend (QCustomPlot::legend).
  
  Returns true on success.
  
  \see addPlottable, clearPlottables
*/
bool QCustomPlot::removePlottable(QCPAbstractPlottable *plottable)
{
  if (!mPlottables.contains(plottable))
  {
    qDebug() << Q_FUNC_INFO << "plottable not in list:" << reinterpret_cast<quintptr>(plottable);
    return false;
  }
  
  // remove plottable from legend:
  plottable->removeFromLegend();
  // special handling for QCPGraphs to maintain the simple graph interface:
  if (QCPGraph *graph = qobject_cast<QCPGraph*>(plottable))
    mGraphs.removeOne(graph);
  // remove plottable:
  delete plottable;
  mPlottables.removeOne(plottable);
  return true;
}

/*! \overload
  
  Removes the plottable by its \a index.
*/
bool QCustomPlot::removePlottable(int index)
{
  if (index >= 0 && index < mPlottables.size())
    return removePlottable(mPlottables[index]);
  else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return false;
  }
}

/*!
  Removes all plottables from the plot (and the QCustomPlot::legend, if necessary).
  
  Returns the number of plottables removed.
  
  \see removePlottable
*/
int QCustomPlot::clearPlottables()
{
  int c = mPlottables.size();
  for (int i=c-1; i >= 0; --i)
    removePlottable(mPlottables[i]);
  return c;
}

/*!
  Returns the number of currently existing plottables in the plot
  
  \see plottable, addPlottable
*/
int QCustomPlot::plottableCount() const
{
  return mPlottables.size();
}

/*!
  Returns a list of the selected plottables. If no plottables are currently selected, the list is empty.
  
  There is a convenience function if you're only interested in selected graphs, see \ref selectedGraphs.
  
  \see setInteractions, QCPAbstractPlottable::setSelectable, QCPAbstractPlottable::setSelected
*/
QList<QCPAbstractPlottable*> QCustomPlot::selectedPlottables() const
{
  QList<QCPAbstractPlottable*> result;
  foreach (QCPAbstractPlottable *plottable, mPlottables)
  {
    if (plottable->selected())
      result.append(plottable);
  }
  return result;
}

/*!
  Returns the plottable at the pixel position \a pos. Plottables that only consist of single lines
  (like graphs) have a tolerance band around them, see \ref setSelectionTolerance. If multiple
  plottables come into consideration, the one closest to \a pos is returned.
  
  If \a onlySelectable is true, only plottables that are selectable
  (QCPAbstractPlottable::setSelectable) are considered.
  
  If there is no plottable at \a pos, the return value is 0.
  
  \see itemAt, layoutElementAt
*/
QCPAbstractPlottable *QCustomPlot::plottableAt(const QPointF &pos, bool onlySelectable) const
{
  QCPAbstractPlottable *resultPlottable = 0;
  double resultDistance = mSelectionTolerance; // only regard clicks with distances smaller than mSelectionTolerance as selections, so initialize with that value
  
  foreach (QCPAbstractPlottable *plottable, mPlottables)
  {
    if (onlySelectable && !plottable->selectable()) // we could have also passed onlySelectable to the selectTest function, but checking here is faster, because we have access to QCPabstractPlottable::selectable
      continue;
    if ((plottable->keyAxis()->axisRect()->rect() & plottable->valueAxis()->axisRect()->rect()).contains(pos.toPoint())) // only consider clicks inside the rect that is spanned by the plottable's key/value axes
    {
      double currentDistance = plottable->selectTest(pos, false);
      if (currentDistance >= 0 && currentDistance < resultDistance)
      {
        resultPlottable = plottable;
        resultDistance = currentDistance;
      }
    }
  }
  
  return resultPlottable;
}

/*!
  Returns whether this QCustomPlot instance contains the \a plottable.
  
  \see addPlottable
*/
bool QCustomPlot::hasPlottable(QCPAbstractPlottable *plottable) const
{
  return mPlottables.contains(plottable);
}

/*!
  Returns the graph with \a index. If the index is invalid, returns 0.
  
  There is an overloaded version of this function with no parameter which returns the last created
  graph, see QCustomPlot::graph()
  
  \see graphCount, addGraph
*/
QCPGraph *QCustomPlot::graph(int index) const
{
  if (index >= 0 && index < mGraphs.size())
  {
    return mGraphs.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return 0;
  }
}

/*! \overload
  
  Returns the last graph, that was created with \ref addGraph. If there are no graphs in the plot,
  returns 0.
  
  \see graphCount, addGraph
*/
QCPGraph *QCustomPlot::graph() const
{
  if (!mGraphs.isEmpty())
  {
    return mGraphs.last();
  } else
    return 0;
}

/*!
  Creates a new graph inside the plot. If \a keyAxis and \a valueAxis are left unspecified (0), the
  bottom (xAxis) is used as key and the left (yAxis) is used as value axis. If specified, \a
  keyAxis and \a valueAxis must reside in this QCustomPlot.
  
  \a keyAxis will be used as key axis (typically "x") and \a valueAxis as value axis (typically
  "y") for the graph.
  
  Returns a pointer to the newly created graph, or 0 if adding the graph failed.
  
  \see graph, graphCount, removeGraph, clearGraphs
*/
QCPGraph *QCustomPlot::addGraph(QCPAxis *keyAxis, QCPAxis *valueAxis)
{
  if (!keyAxis) keyAxis = xAxis;
  if (!valueAxis) valueAxis = yAxis;
  if (!keyAxis || !valueAxis)
  {
    qDebug() << Q_FUNC_INFO << "can't use default QCustomPlot xAxis or yAxis, because at least one is invalid (has been deleted)";
    return 0;
  }
  if (keyAxis->parentPlot() != this || valueAxis->parentPlot() != this)
  {
    qDebug() << Q_FUNC_INFO << "passed keyAxis or valueAxis doesn't have this QCustomPlot as parent";
    return 0;
  }
  
  QCPGraph *newGraph = new QCPGraph(keyAxis, valueAxis);
  if (addPlottable(newGraph))
  {
    newGraph->setName("Graph "+QString::number(mGraphs.size()));
    return newGraph;
  } else
  {
    delete newGraph;
    return 0;
  }
}

/*!
  Removes the specified \a graph from the plot and, if necessary, from the QCustomPlot::legend. If
  any other graphs in the plot have a channel fill set towards the removed graph, the channel fill
  property of those graphs is reset to zero (no channel fill).
  
  Returns true on success.
  
  \see clearGraphs
*/
bool QCustomPlot::removeGraph(QCPGraph *graph)
{
  return removePlottable(graph);
}

/*! \overload
  
  Removes the graph by its \a index.
*/
bool QCustomPlot::removeGraph(int index)
{
  if (index >= 0 && index < mGraphs.size())
    return removeGraph(mGraphs[index]);
  else
    return false;
}

/*!
  Removes all graphs from the plot (and the QCustomPlot::legend, if necessary).

  Returns the number of graphs removed.
  
  \see removeGraph
*/
int QCustomPlot::clearGraphs()
{
  int c = mGraphs.size();
  for (int i=c-1; i >= 0; --i)
    removeGraph(mGraphs[i]);
  return c;
}

/*!
  Returns the number of currently existing graphs in the plot
  
  \see graph, addGraph
*/
int QCustomPlot::graphCount() const
{
  return mGraphs.size();
}

/*!
  Returns a list of the selected graphs. If no graphs are currently selected, the list is empty.
  
  If you are not only interested in selected graphs but other plottables like QCPCurve, QCPBars,
  etc., use \ref selectedPlottables.
  
  \see setInteractions, selectedPlottables, QCPAbstractPlottable::setSelectable, QCPAbstractPlottable::setSelected
*/
QList<QCPGraph*> QCustomPlot::selectedGraphs() const
{
  QList<QCPGraph*> result;
  foreach (QCPGraph *graph, mGraphs)
  {
    if (graph->selected())
      result.append(graph);
  }
  return result;
}

/*!
  Returns the item with \a index. If the index is invalid, returns 0.
  
  There is an overloaded version of this function with no parameter which returns the last added
  item, see QCustomPlot::item()
  
  \see itemCount, addItem
*/
QCPAbstractItem *QCustomPlot::item(int index) const
{
  if (index >= 0 && index < mItems.size())
  {
    return mItems.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return 0;
  }
}

/*! \overload
  
  Returns the last item, that was added with \ref addItem. If there are no items in the plot,
  returns 0.
  
  \see itemCount, addItem
*/
QCPAbstractItem *QCustomPlot::item() const
{
  if (!mItems.isEmpty())
  {
    return mItems.last();
  } else
    return 0;
}

/*!
  Adds the specified item to the plot. QCustomPlot takes ownership of the item.
  
  Returns true on success, i.e. when \a item wasn't already in the plot and the parent plot of \a
  item is this QCustomPlot.
  
  \see item, itemCount, removeItem, clearItems
*/
bool QCustomPlot::addItem(QCPAbstractItem *item)
{
  if (!mItems.contains(item) && item->parentPlot() == this)
  {
    mItems.append(item);
    return true;
  } else
  {
    qDebug() << Q_FUNC_INFO << "item either already in list or not created with this QCustomPlot as parent:" << reinterpret_cast<quintptr>(item);
    return false;
  }
}

/*!
  Removes the specified item from the plot.
  
  Returns true on success.
  
  \see addItem, clearItems
*/
bool QCustomPlot::removeItem(QCPAbstractItem *item)
{
  if (mItems.contains(item))
  {
    delete item;
    mItems.removeOne(item);
    return true;
  } else
  {
    qDebug() << Q_FUNC_INFO << "item not in list:" << reinterpret_cast<quintptr>(item);
    return false;
  }
}

/*! \overload
  
  Removes the item by its \a index.
*/
bool QCustomPlot::removeItem(int index)
{
  if (index >= 0 && index < mItems.size())
    return removeItem(mItems[index]);
  else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return false;
  }
}

/*!
  Removes all items from the plot.
  
  Returns the number of items removed.
  
  \see removeItem
*/
int QCustomPlot::clearItems()
{
  int c = mItems.size();
  for (int i=c-1; i >= 0; --i)
    removeItem(mItems[i]);
  return c;
}

/*!
  Returns the number of currently existing items in the plot
  
  \see item, addItem
*/
int QCustomPlot::itemCount() const
{
  return mItems.size();
}

/*!
  Returns a list of the selected items. If no items are currently selected, the list is empty.
  
  \see setInteractions, QCPAbstractItem::setSelectable, QCPAbstractItem::setSelected
*/
QList<QCPAbstractItem*> QCustomPlot::selectedItems() const
{
  QList<QCPAbstractItem*> result;
  foreach (QCPAbstractItem *item, mItems)
  {
    if (item->selected())
      result.append(item);
  }
  return result;
}

/*!
  Returns the item at the pixel position \a pos. Items that only consist of single lines (e.g. \ref
  QCPItemLine or \ref QCPItemCurve) have a tolerance band around them, see \ref
  setSelectionTolerance. If multiple items come into consideration, the one closest to \a pos is
  returned.
  
  If \a onlySelectable is true, only items that are selectable (QCPAbstractItem::setSelectable) are
  considered.
  
  If there is no item at \a pos, the return value is 0.
  
  \see plottableAt, layoutElementAt
*/
QCPAbstractItem *QCustomPlot::itemAt(const QPointF &pos, bool onlySelectable) const
{
  QCPAbstractItem *resultItem = 0;
  double resultDistance = mSelectionTolerance; // only regard clicks with distances smaller than mSelectionTolerance as selections, so initialize with that value
  
  foreach (QCPAbstractItem *item, mItems)
  {
    if (onlySelectable && !item->selectable()) // we could have also passed onlySelectable to the selectTest function, but checking here is faster, because we have access to QCPAbstractItem::selectable
      continue;
    if (!item->clipToAxisRect() || item->clipRect().contains(pos.toPoint())) // only consider clicks inside axis cliprect of the item if actually clipped to it
    {
      double currentDistance = item->selectTest(pos, false);
      if (currentDistance >= 0 && currentDistance < resultDistance)
      {
        resultItem = item;
        resultDistance = currentDistance;
      }
    }
  }
  
  return resultItem;
}

/*!
  Returns whether this QCustomPlot contains the \a item.
  
  \see addItem
*/
bool QCustomPlot::hasItem(QCPAbstractItem *item) const
{
  return mItems.contains(item);
}

/*!
  Returns the layer with the specified \a name. If there is no layer with the specified name, 0 is
  returned.
  
  Layer names are case-sensitive.
  
  \see addLayer, moveLayer, removeLayer
*/
QCPLayer *QCustomPlot::layer(const QString &name) const
{
  foreach (QCPLayer *layer, mLayers)
  {
    if (layer->name() == name)
      return layer;
  }
  return 0;
}

/*! \overload
  
  Returns the layer by \a index. If the index is invalid, 0 is returned.
  
  \see addLayer, moveLayer, removeLayer
*/
QCPLayer *QCustomPlot::layer(int index) const
{
  if (index >= 0 && index < mLayers.size())
  {
    return mLayers.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return 0;
  }
}

/*!
  Returns the layer that is set as current layer (see \ref setCurrentLayer).
*/
QCPLayer *QCustomPlot::currentLayer() const
{
  return mCurrentLayer;
}

/*!
  Sets the layer with the specified \a name to be the current layer. All layerables (\ref
  QCPLayerable), e.g. plottables and items, are created on the current layer.
  
  Returns true on success, i.e. if there is a layer with the specified \a name in the QCustomPlot.
  
  Layer names are case-sensitive.
  
  \see addLayer, moveLayer, removeLayer, QCPLayerable::setLayer
*/
bool QCustomPlot::setCurrentLayer(const QString &name)
{
  if (QCPLayer *newCurrentLayer = layer(name))
  {
    return setCurrentLayer(newCurrentLayer);
  } else
  {
    qDebug() << Q_FUNC_INFO << "layer with name doesn't exist:" << name;
    return false;
  }
}

/*! \overload
  
  Sets the provided \a layer to be the current layer.
  
  Returns true on success, i.e. when \a layer is a valid layer in the QCustomPlot.
  
  \see addLayer, moveLayer, removeLayer
*/
bool QCustomPlot::setCurrentLayer(QCPLayer *layer)
{
  if (!mLayers.contains(layer))
  {
    qDebug() << Q_FUNC_INFO << "layer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(layer);
    return false;
  }
  
  mCurrentLayer = layer;
  return true;
}

/*!
  Returns the number of currently existing layers in the plot
  
  \see layer, addLayer
*/
int QCustomPlot::layerCount() const
{
  return mLayers.size();
}

/*!
  Adds a new layer to this QCustomPlot instance. The new layer will have the name \a name, which
  must be unique. Depending on \a insertMode, it is positioned either below or above \a otherLayer.
  
  Returns true on success, i.e. if there is no other layer named \a name and \a otherLayer is a
  valid layer inside this QCustomPlot.
  
  If \a otherLayer is 0, the highest layer in the QCustomPlot will be used.
  
  For an explanation of what layers are in QCustomPlot, see the documentation of \ref QCPLayer.
  
  \see layer, moveLayer, removeLayer
*/
bool QCustomPlot::addLayer(const QString &name, QCPLayer *otherLayer, QCustomPlot::LayerInsertMode insertMode)
{
  if (!otherLayer)
    otherLayer = mLayers.last();
  if (!mLayers.contains(otherLayer))
  {
    qDebug() << Q_FUNC_INFO << "otherLayer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(otherLayer);
    return false;
  }
  if (layer(name))
  {
    qDebug() << Q_FUNC_INFO << "A layer exists already with the name" << name;
    return false;
  }
    
  QCPLayer *newLayer = new QCPLayer(this, name);
  mLayers.insert(otherLayer->index() + (insertMode==limAbove ? 1:0), newLayer);
  updateLayerIndices();
  return true;
}

/*!
  Removes the specified \a layer and returns true on success.
  
  All layerables (e.g. plottables and items) on the removed layer will be moved to the layer below
  \a layer. If \a layer is the bottom layer, the layerables are moved to the layer above. In both
  cases, the total rendering order of all layerables in the QCustomPlot is preserved.
  
  If \a layer is the current layer (\ref setCurrentLayer), the layer below (or above, if bottom
  layer) becomes the new current layer.
  
  It is not possible to remove the last layer of the plot.
  
  \see layer, addLayer, moveLayer
*/
bool QCustomPlot::removeLayer(QCPLayer *layer)
{
  if (!mLayers.contains(layer))
  {
    qDebug() << Q_FUNC_INFO << "layer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(layer);
    return false;
  }
  if (mLayers.size() < 2)
  {
    qDebug() << Q_FUNC_INFO << "can't remove last layer";
    return false;
  }
  
  // append all children of this layer to layer below (if this is lowest layer, prepend to layer above)
  int removedIndex = layer->index();
  bool isFirstLayer = removedIndex==0;
  QCPLayer *targetLayer = isFirstLayer ? mLayers.at(removedIndex+1) : mLayers.at(removedIndex-1);
  QList<QCPLayerable*> children = layer->children();
  if (isFirstLayer) // prepend in reverse order (so order relative to each other stays the same)
  {
    for (int i=children.size()-1; i>=0; --i)
      children.at(i)->moveToLayer(targetLayer, true);
  } else  // append normally
  {
    for (int i=0; i<children.size(); ++i)
      children.at(i)->moveToLayer(targetLayer, false);
  }
  // if removed layer is current layer, change current layer to layer below/above:
  if (layer == mCurrentLayer)
    setCurrentLayer(targetLayer);
  // remove layer:
  delete layer;
  mLayers.removeOne(layer);
  updateLayerIndices();
  return true;
}

/*!
  Moves the specified \a layer either above or below \a otherLayer. Whether it's placed above or
  below is controlled with \a insertMode.
  
  Returns true on success, i.e. when both \a layer and \a otherLayer are valid layers in the
  QCustomPlot.
  
  \see layer, addLayer, moveLayer
*/
bool QCustomPlot::moveLayer(QCPLayer *layer, QCPLayer *otherLayer, QCustomPlot::LayerInsertMode insertMode)
{
  if (!mLayers.contains(layer))
  {
    qDebug() << Q_FUNC_INFO << "layer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(layer);
    return false;
  }
  if (!mLayers.contains(otherLayer))
  {
    qDebug() << Q_FUNC_INFO << "otherLayer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(otherLayer);
    return false;
  }
  
  mLayers.move(layer->index(), otherLayer->index() + (insertMode==limAbove ? 1:0));
  updateLayerIndices();
  return true;
}

/*!
  Returns the number of axis rects in the plot.
  
  All axis rects can be accessed via QCustomPlot::axisRect().
  
  Initially, only one axis rect exists in the plot.
  
  \see axisRect, axisRects
*/
int QCustomPlot::axisRectCount() const
{
  return axisRects().size();
}

/*!
  Returns the axis rect with \a index.
  
  Initially, only one axis rect (with index 0) exists in the plot. If multiple axis rects were
  added, all of them may be accessed with this function in a linear fashion (even when they are
  nested in a layout hierarchy or inside other axis rects via QCPAxisRect::insetLayout).
  
  \see axisRectCount, axisRects
*/
QCPAxisRect *QCustomPlot::axisRect(int index) const
{
  const QList<QCPAxisRect*> rectList = axisRects();
  if (index >= 0 && index < rectList.size())
  {
    return rectList.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "invalid axis rect index" << index;
    return 0;
  }
}

/*!
  Returns all axis rects in the plot.
  
  \see axisRectCount, axisRect
*/
QList<QCPAxisRect*> QCustomPlot::axisRects() const
{
  QList<QCPAxisRect*> result;
  QStack<QCPLayoutElement*> elementStack;
  if (mPlotLayout)
    elementStack.push(mPlotLayout);
  
  while (!elementStack.isEmpty())
  {
    foreach (QCPLayoutElement *element, elementStack.pop()->elements(false))
    {
      if (element)
      {
        elementStack.push(element);
        if (QCPAxisRect *ar = qobject_cast<QCPAxisRect*>(element))
          result.append(ar);
      }
    }
  }
  
  return result;
}

/*!
  Returns the layout element at pixel position \a pos. If there is no element at that position,
  returns 0.
  
  Only visible elements are used. If \ref QCPLayoutElement::setVisible on the element itself or on
  any of its parent elements is set to false, it will not be considered.
  
  \see itemAt, plottableAt
*/
QCPLayoutElement *QCustomPlot::layoutElementAt(const QPointF &pos) const
{
  QCPLayoutElement *currentElement = mPlotLayout;
  bool searchSubElements = true;
  while (searchSubElements && currentElement)
  {
    searchSubElements = false;
    foreach (QCPLayoutElement *subElement, currentElement->elements(false))
    {
      if (subElement && subElement->realVisibility() && subElement->selectTest(pos, false) >= 0)
      {
        currentElement = subElement;
        searchSubElements = true;
        break;
      }
    }
  }
  return currentElement;
}

/*!
  Returns the axes that currently have selected parts, i.e. whose selection state is not \ref
  QCPAxis::spNone.
  
  \see selectedPlottables, selectedLegends, setInteractions, QCPAxis::setSelectedParts,
  QCPAxis::setSelectableParts
*/
QList<QCPAxis*> QCustomPlot::selectedAxes() const
{
  QList<QCPAxis*> result, allAxes;
  foreach (QCPAxisRect *rect, axisRects())
    allAxes << rect->axes();
  
  foreach (QCPAxis *axis, allAxes)
  {
    if (axis->selectedParts() != QCPAxis::spNone)
      result.append(axis);
  }
  
  return result;
}

/*!
  Returns the legends that currently have selected parts, i.e. whose selection state is not \ref
  QCPLegend::spNone.
  
  \see selectedPlottables, selectedAxes, setInteractions, QCPLegend::setSelectedParts,
  QCPLegend::setSelectableParts, QCPLegend::selectedItems
*/
QList<QCPLegend*> QCustomPlot::selectedLegends() const
{
  QList<QCPLegend*> result;
  
  QStack<QCPLayoutElement*> elementStack;
  if (mPlotLayout)
    elementStack.push(mPlotLayout);
  
  while (!elementStack.isEmpty())
  {
    foreach (QCPLayoutElement *subElement, elementStack.pop()->elements(false))
    {
      if (subElement)
      {
        elementStack.push(subElement);
        if (QCPLegend *leg = qobject_cast<QCPLegend*>(subElement))
        {
          if (leg->selectedParts() != QCPLegend::spNone)
            result.append(leg);
        }
      }
    }
  }
  
  return result;
}

/*!
  Deselects all layerables (plottables, items, axes, legends,...) of the QCustomPlot.
  
  Since calling this function is not a user interaction, this does not emit the \ref
  selectionChangedByUser signal. The individual selectionChanged signals are emitted though, if the
  objects were previously selected.
  
  \see setInteractions, selectedPlottables, selectedItems, selectedAxes, selectedLegends
*/
void QCustomPlot::deselectAll()
{
  foreach (QCPLayer *layer, mLayers)
  {
    foreach (QCPLayerable *layerable, layer->children())
      layerable->deselectEvent(0);
  }
}

/*!
  Causes a complete replot into the internal buffer. Finally, update() is called, to redraw the
  buffer on the QCustomPlot widget surface. This is the method that must be called to make changes,
  for example on the axis ranges or data points of graphs, visible.
  
  Under a few circumstances, QCustomPlot causes a replot by itself. Those are resize events of the
  QCustomPlot widget and user interactions (object selection and range dragging/zooming).
  
  Before the replot happens, the signal \ref beforeReplot is emitted. After the replot, \ref
  afterReplot is emitted. It is safe to mutually connect the replot slot with any of those two
  signals on two QCustomPlots to make them replot synchronously, it won't cause an infinite
  recursion.
*/
void QCustomPlot::replot(QCustomPlot::RefreshPriority refreshPriority)
{
  if (mReplotting) // incase signals loop back to replot slot
    return;
  mReplotting = true;
  emit beforeReplot();
  
  mPaintBuffer.fill(mBackgroundBrush.style() == Qt::SolidPattern ? mBackgroundBrush.color() : Qt::transparent);
  QCPPainter painter;
  painter.begin(&mPaintBuffer);
  if (painter.isActive())
  {
    painter.setRenderHint(QPainter::HighQualityAntialiasing); // to make Antialiasing look good if using the OpenGL graphicssystem
    if (mBackgroundBrush.style() != Qt::SolidPattern && mBackgroundBrush.style() != Qt::NoBrush)
      painter.fillRect(mViewport, mBackgroundBrush);
    draw(&painter);
    painter.end();
    if ((refreshPriority == rpHint && mPlottingHints.testFlag(QCP::phForceRepaint)) || refreshPriority==rpImmediate)
      repaint();
    else
      update();
  } else // might happen if QCustomPlot has width or height zero
    qDebug() << Q_FUNC_INFO << "Couldn't activate painter on buffer";
  
  emit afterReplot();
  mReplotting = false;
}

/*!
  Rescales the axes such that all plottables (like graphs) in the plot are fully visible.
  
  if \a onlyVisiblePlottables is set to true, only the plottables that have their visibility set to true
  (QCPLayerable::setVisible), will be used to rescale the axes.
  
  \see QCPAbstractPlottable::rescaleAxes, QCPAxis::rescale
*/
void QCustomPlot::rescaleAxes(bool onlyVisiblePlottables)
{
  QList<QCPAxis*> allAxes;
  foreach (QCPAxisRect *rect, axisRects())
    allAxes << rect->axes();
  
  foreach (QCPAxis *axis, allAxes)
    axis->rescale(onlyVisiblePlottables);
}

/*!
  Saves a PDF with the vectorized plot to the file \a fileName. The axis ratio as well as the scale
  of texts and lines will be derived from the specified \a width and \a height. This means, the
  output will look like the normal on-screen output of a QCustomPlot widget with the corresponding
  pixel width and height. If either \a width or \a height is zero, the exported image will have the
  same dimensions as the QCustomPlot widget currently has.

  \a noCosmeticPen disables the use of cosmetic pens when drawing to the PDF file. Cosmetic pens
  are pens with numerical width 0, which are always drawn as a one pixel wide line, no matter what
  zoom factor is set in the PDF-Viewer. For more information about cosmetic pens, see the QPainter
  and QPen documentation.
  
  The objects of the plot will appear in the current selection state. If you don't want any
  selected objects to be painted in their selected look, deselect everything with \ref deselectAll
  before calling this function.

  Returns true on success.
  
  \warning
  \li If you plan on editing the exported PDF file with a vector graphics editor like
  Inkscape, it is advised to set \a noCosmeticPen to true to avoid losing those cosmetic lines
  (which might be quite many, because cosmetic pens are the default for e.g. axes and tick marks).
  \li If calling this function inside the constructor of the parent of the QCustomPlot widget
  (i.e. the MainWindow constructor, if QCustomPlot is inside the MainWindow), always provide
  explicit non-zero widths and heights. If you leave \a width or \a height as 0 (default), this
  function uses the current width and height of the QCustomPlot widget. However, in Qt, these
  aren't defined yet inside the constructor, so you would get an image that has strange
  widths/heights.
  
  \a pdfCreator and \a pdfTitle may be used to set the according metadata fields in the resulting
  PDF file.
  
  \note On Android systems, this method does nothing and issues an according qDebug warning
  message. This is also the case if for other reasons the define flag QT_NO_PRINTER is set.
  
  \see savePng, saveBmp, saveJpg, saveRastered
*/
bool QCustomPlot::savePdf(const QString &fileName, bool noCosmeticPen, int width, int height, const QString &pdfCreator, const QString &pdfTitle)
{
  bool success = false;
#ifdef QT_NO_PRINTER
  Q_UNUSED(fileName)
  Q_UNUSED(noCosmeticPen)
  Q_UNUSED(width)
  Q_UNUSED(height)
  qDebug() << Q_FUNC_INFO << "Qt was built without printer support (QT_NO_PRINTER). PDF not created.";
#else
  int newWidth, newHeight;
  if (width == 0 || height == 0)
  {
    newWidth = this->width();
    newHeight = this->height();
  } else
  {
    newWidth = width;
    newHeight = height;
  }
  
  QPrinter printer(QPrinter::ScreenResolution);
  printer.setOutputFileName(fileName);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setFullPage(true);
  printer.setColorMode(QPrinter::Color);
  printer.printEngine()->setProperty(QPrintEngine::PPK_Creator, pdfCreator);
  printer.printEngine()->setProperty(QPrintEngine::PPK_DocumentName, pdfTitle);
  QRect oldViewport = viewport();
  setViewport(QRect(0, 0, newWidth, newHeight));
  printer.setPaperSize(viewport().size(), QPrinter::DevicePixel);
  QCPPainter printpainter;
  if (printpainter.begin(&printer))
  {
    printpainter.setMode(QCPPainter::pmVectorized);
    printpainter.setMode(QCPPainter::pmNoCaching);
    printpainter.setMode(QCPPainter::pmNonCosmetic, noCosmeticPen);
    printpainter.setWindow(mViewport);
    if (mBackgroundBrush.style() != Qt::NoBrush &&
        mBackgroundBrush.color() != Qt::white &&
        mBackgroundBrush.color() != Qt::transparent &&
        mBackgroundBrush.color().alpha() > 0) // draw pdf background color if not white/transparent
      printpainter.fillRect(viewport(), mBackgroundBrush);
    draw(&printpainter);
    printpainter.end();
    success = true;
  }
  setViewport(oldViewport);
#endif // QT_NO_PRINTER
  return success;
}

/*!
  Saves a PNG image file to \a fileName on disc. The output plot will have the dimensions \a width
  and \a height in pixels. If either \a width or \a height is zero, the exported image will have
  the same dimensions as the QCustomPlot widget currently has. Line widths and texts etc. are not
  scaled up when larger widths/heights are used. If you want that effect, use the \a scale parameter.

  For example, if you set both \a width and \a height to 100 and \a scale to 2, you will end up with an
  image file of size 200*200 in which all graphical elements are scaled up by factor 2 (line widths,
  texts, etc.). This scaling is not done by stretching a 100*100 image, the result will have full
  200*200 pixel resolution.
  
  If you use a high scaling factor, it is recommended to enable antialiasing for all elements via
  temporarily setting \ref QCustomPlot::setAntialiasedElements to \ref QCP::aeAll as this allows
  QCustomPlot to place objects with sub-pixel accuracy.

  \warning If calling this function inside the constructor of the parent of the QCustomPlot widget
  (i.e. the MainWindow constructor, if QCustomPlot is inside the MainWindow), always provide
  explicit non-zero widths and heights. If you leave \a width or \a height as 0 (default), this
  function uses the current width and height of the QCustomPlot widget. However, in Qt, these
  aren't defined yet inside the constructor, so you would get an image that has strange
  widths/heights.
  
  The objects of the plot will appear in the current selection state. If you don't want any selected
  objects to be painted in their selected look, deselect everything with \ref deselectAll before calling
  this function.

  If you want the PNG to have a transparent background, call \ref setBackground(const QBrush
  &brush) with no brush (Qt::NoBrush) or a transparent color (Qt::transparent), before saving.

  PNG compression can be controlled with the \a quality parameter which must be between 0 and 100 or
  -1 to use the default setting.
  
  Returns true on success. If this function fails, most likely the PNG format isn't supported by
  the system, see Qt docs about QImageWriter::supportedImageFormats().

  \see savePdf, saveBmp, saveJpg, saveRastered
*/
bool QCustomPlot::savePng(const QString &fileName, int width, int height, double scale, int quality)
{
  return saveRastered(fileName, width, height, scale, "PNG", quality);
}

/*!
  Saves a JPG image file to \a fileName on disc. The output plot will have the dimensions \a width
  and \a height in pixels. If either \a width or \a height is zero, the exported image will have
  the same dimensions as the QCustomPlot widget currently has. Line widths and texts etc. are not
  scaled up when larger widths/heights are used. If you want that effect, use the \a scale parameter.

  For example, if you set both \a width and \a height to 100 and \a scale to 2, you will end up with an
  image file of size 200*200 in which all graphical elements are scaled up by factor 2 (line widths,
  texts, etc.). This scaling is not done by stretching a 100*100 image, the result will have full
  200*200 pixel resolution.
  
  If you use a high scaling factor, it is recommended to enable antialiasing for all elements via
  temporarily setting \ref QCustomPlot::setAntialiasedElements to \ref QCP::aeAll as this allows
  QCustomPlot to place objects with sub-pixel accuracy.

  \warning If calling this function inside the constructor of the parent of the QCustomPlot widget
  (i.e. the MainWindow constructor, if QCustomPlot is inside the MainWindow), always provide
  explicit non-zero widths and heights. If you leave \a width or \a height as 0 (default), this
  function uses the current width and height of the QCustomPlot widget. However, in Qt, these
  aren't defined yet inside the constructor, so you would get an image that has strange
  widths/heights.

  The objects of the plot will appear in the current selection state. If you don't want any selected
  objects to be painted in their selected look, deselect everything with \ref deselectAll before calling
  this function.

  JPG compression can be controlled with the \a quality parameter which must be between 0 and 100 or
  -1 to use the default setting.
  
  Returns true on success. If this function fails, most likely the JPG format isn't supported by
  the system, see Qt docs about QImageWriter::supportedImageFormats().

  \see savePdf, savePng, saveBmp, saveRastered
*/
bool QCustomPlot::saveJpg(const QString &fileName, int width, int height, double scale, int quality)
{
  return saveRastered(fileName, width, height, scale, "JPG", quality);
}

/*!
  Saves a BMP image file to \a fileName on disc. The output plot will have the dimensions \a width
  and \a height in pixels. If either \a width or \a height is zero, the exported image will have
  the same dimensions as the QCustomPlot widget currently has. Line widths and texts etc. are not
  scaled up when larger widths/heights are used. If you want that effect, use the \a scale parameter.

  For example, if you set both \a width and \a height to 100 and \a scale to 2, you will end up with an
  image file of size 200*200 in which all graphical elements are scaled up by factor 2 (line widths,
  texts, etc.). This scaling is not done by stretching a 100*100 image, the result will have full
  200*200 pixel resolution.
  
  If you use a high scaling factor, it is recommended to enable antialiasing for all elements via
  temporarily setting \ref QCustomPlot::setAntialiasedElements to \ref QCP::aeAll as this allows
  QCustomPlot to place objects with sub-pixel accuracy.

  \warning If calling this function inside the constructor of the parent of the QCustomPlot widget
  (i.e. the MainWindow constructor, if QCustomPlot is inside the MainWindow), always provide
  explicit non-zero widths and heights. If you leave \a width or \a height as 0 (default), this
  function uses the current width and height of the QCustomPlot widget. However, in Qt, these
  aren't defined yet inside the constructor, so you would get an image that has strange
  widths/heights.

  The objects of the plot will appear in the current selection state. If you don't want any selected
  objects to be painted in their selected look, deselect everything with \ref deselectAll before calling
  this function.
  
  Returns true on success. If this function fails, most likely the BMP format isn't supported by
  the system, see Qt docs about QImageWriter::supportedImageFormats().

  \see savePdf, savePng, saveJpg, saveRastered
*/
bool QCustomPlot::saveBmp(const QString &fileName, int width, int height, double scale)
{
  return saveRastered(fileName, width, height, scale, "BMP");
}

/*! \internal
  
  Returns a minimum size hint that corresponds to the minimum size of the top level layout
  (\ref plotLayout). To prevent QCustomPlot from being collapsed to size/width zero, set a minimum
  size (setMinimumSize) either on the whole QCustomPlot or on any layout elements inside the plot.
  This is especially important, when placed in a QLayout where other components try to take in as
  much space as possible (e.g. QMdiArea).
*/
QSize QCustomPlot::minimumSizeHint() const
{
  return mPlotLayout->minimumSizeHint();
}

/*! \internal
  
  Returns a size hint that is the same as \ref minimumSizeHint.
  
*/
QSize QCustomPlot::sizeHint() const
{
  return mPlotLayout->minimumSizeHint();
}

/*! \internal
  
  Event handler for when the QCustomPlot widget needs repainting. This does not cause a \ref replot, but
  draws the internal buffer on the widget surface.
*/
void QCustomPlot::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);
  QPainter painter(this);
  painter.drawPixmap(0, 0, mPaintBuffer);
}

/*! \internal
  
  Event handler for a resize of the QCustomPlot widget. Causes the internal buffer to be resized to
  the new size. The viewport (which becomes the outer rect of mPlotLayout) is resized
  appropriately. Finally a \ref replot is performed.
*/
void QCustomPlot::resizeEvent(QResizeEvent *event)
{
  // resize and repaint the buffer:
  mPaintBuffer = QPixmap(event->size());
  setViewport(rect());
  replot(rpQueued); // queued update is important here, to prevent painting issues in some contexts
}

/*! \internal
  
 Event handler for when a double click occurs. Emits the \ref mouseDoubleClick signal, then emits
 the specialized signals when certain objecs are clicked (e.g. \ref plottableDoubleClick, \ref
 axisDoubleClick, etc.). Finally determines the affected layout element and forwards the event to
 it.
 
 \see mousePressEvent, mouseReleaseEvent
*/
void QCustomPlot::mouseDoubleClickEvent(QMouseEvent *event)
{
  emit mouseDoubleClick(event);
  
  QVariant details;
  QCPLayerable *clickedLayerable = layerableAt(event->pos(), false, &details);
  
  // emit specialized object double click signals:
  if (QCPAbstractPlottable *ap = qobject_cast<QCPAbstractPlottable*>(clickedLayerable))
    emit plottableDoubleClick(ap, event);
  else if (QCPAxis *ax = qobject_cast<QCPAxis*>(clickedLayerable))
    emit axisDoubleClick(ax, details.value<QCPAxis::SelectablePart>(), event);
  else if (QCPAbstractItem *ai = qobject_cast<QCPAbstractItem*>(clickedLayerable))
    emit itemDoubleClick(ai, event);
  else if (QCPLegend *lg = qobject_cast<QCPLegend*>(clickedLayerable))
    emit legendDoubleClick(lg, 0, event);
  else if (QCPAbstractLegendItem *li = qobject_cast<QCPAbstractLegendItem*>(clickedLayerable))
    emit legendDoubleClick(li->parentLegend(), li, event);
  else if (QCPPlotTitle *pt = qobject_cast<QCPPlotTitle*>(clickedLayerable))
    emit titleDoubleClick(event, pt);
  
  // call double click event of affected layout element:
  if (QCPLayoutElement *el = layoutElementAt(event->pos()))
    el->mouseDoubleClickEvent(event);
  
  // call release event of affected layout element (as in mouseReleaseEvent, since the mouseDoubleClick replaces the second release event in double click case):
  if (mMouseEventElement)
  {
    mMouseEventElement->mouseReleaseEvent(event);
    mMouseEventElement = 0;
  }
  
  //QWidget::mouseDoubleClickEvent(event); don't call base class implementation because it would just cause a mousePress/ReleaseEvent, which we don't want.
}

/*! \internal
  
  Event handler for when a mouse button is pressed. Emits the mousePress signal. Then determines
  the affected layout element and forwards the event to it.
  
  \see mouseMoveEvent, mouseReleaseEvent
*/
void QCustomPlot::mousePressEvent(QMouseEvent *event)
{
  emit mousePress(event);
  mMousePressPos = event->pos(); // need this to determine in releaseEvent whether it was a click (no position change between press and release)
  
  // call event of affected layout element:
  mMouseEventElement = layoutElementAt(event->pos());
  if (mMouseEventElement)
    mMouseEventElement->mousePressEvent(event);
  
  QWidget::mousePressEvent(event);
}

/*! \internal
  
  Event handler for when the cursor is moved. Emits the \ref mouseMove signal.

  If a layout element has mouse capture focus (a mousePressEvent happened on top of the layout
  element before), the mouseMoveEvent is forwarded to that element.
  
  \see mousePressEvent, mouseReleaseEvent
*/
void QCustomPlot::mouseMoveEvent(QMouseEvent *event)
{
  emit mouseMove(event);

  // call event of affected layout element:
  if (mMouseEventElement)
    mMouseEventElement->mouseMoveEvent(event);
  
  QWidget::mouseMoveEvent(event);
}

/*! \internal
  
  Event handler for when a mouse button is released. Emits the \ref mouseRelease signal.
  
  If the mouse was moved less than a certain threshold in any direction since the \ref
  mousePressEvent, it is considered a click which causes the selection mechanism (if activated via
  \ref setInteractions) to possibly change selection states accordingly. Further, specialized mouse
  click signals are emitted (e.g. \ref plottableClick, \ref axisClick, etc.)
  
  If a layout element has mouse capture focus (a \ref mousePressEvent happened on top of the layout
  element before), the \ref mouseReleaseEvent is forwarded to that element.
  
  \see mousePressEvent, mouseMoveEvent
*/
void QCustomPlot::mouseReleaseEvent(QMouseEvent *event)
{
  emit mouseRelease(event);
  bool doReplot = false;
  
  if ((mMousePressPos-event->pos()).manhattanLength() < 5) // determine whether it was a click operation
  {
    if (event->button() == Qt::LeftButton)
    {
      // handle selection mechanism:
      QVariant details;
      QCPLayerable *clickedLayerable = layerableAt(event->pos(), true, &details);
      bool selectionStateChanged = false;
      bool additive = mInteractions.testFlag(QCP::iMultiSelect) && event->modifiers().testFlag(mMultiSelectModifier);
      // deselect all other layerables if not additive selection:
      if (!additive)
      {
        foreach (QCPLayer *layer, mLayers)
        {
          foreach (QCPLayerable *layerable, layer->children())
          {
            if (layerable != clickedLayerable && mInteractions.testFlag(layerable->selectionCategory()))
            {
              bool selChanged = false;
              layerable->deselectEvent(&selChanged);
              selectionStateChanged |= selChanged;
            }
          }
        }
      }
      if (clickedLayerable && mInteractions.testFlag(clickedLayerable->selectionCategory()))
      {
        // a layerable was actually clicked, call its selectEvent:
        bool selChanged = false;
        clickedLayerable->selectEvent(event, additive, details, &selChanged);
        selectionStateChanged |= selChanged;
      }
      doReplot = true;
      if (selectionStateChanged)
        emit selectionChangedByUser();
    }
    
    // emit specialized object click signals:
    QVariant details;
    QCPLayerable *clickedLayerable = layerableAt(event->pos(), false, &details); // for these signals, selectability is ignored, that's why we call this again with onlySelectable set to false
    if (QCPAbstractPlottable *ap = qobject_cast<QCPAbstractPlottable*>(clickedLayerable))
      emit plottableClick(ap, event);
    else if (QCPAxis *ax = qobject_cast<QCPAxis*>(clickedLayerable))
      emit axisClick(ax, details.value<QCPAxis::SelectablePart>(), event);
    else if (QCPAbstractItem *ai = qobject_cast<QCPAbstractItem*>(clickedLayerable))
      emit itemClick(ai, event);
    else if (QCPLegend *lg = qobject_cast<QCPLegend*>(clickedLayerable))
      emit legendClick(lg, 0, event);
    else if (QCPAbstractLegendItem *li = qobject_cast<QCPAbstractLegendItem*>(clickedLayerable))
      emit legendClick(li->parentLegend(), li, event);
    else if (QCPPlotTitle *pt = qobject_cast<QCPPlotTitle*>(clickedLayerable))
      emit titleClick(event, pt);
  }
  
  // call event of affected layout element:
  if (mMouseEventElement)
  {
    mMouseEventElement->mouseReleaseEvent(event);
    mMouseEventElement = 0;
  }
  
  if (doReplot || noAntialiasingOnDrag())
    replot();
  
  QWidget::mouseReleaseEvent(event);
}

/*! \internal
  
  Event handler for mouse wheel events. First, the \ref mouseWheel signal is emitted. Then
  determines the affected layout element and forwards the event to it.
  
*/
void QCustomPlot::wheelEvent(QWheelEvent *event)
{
  emit mouseWheel(event);
  
  // call event of affected layout element:
  if (QCPLayoutElement *el = layoutElementAt(event->pos()))
    el->wheelEvent(event);
  
  QWidget::wheelEvent(event);
}

/*! \internal
  
  This is the main draw function. It draws the entire plot, including background pixmap, with the
  specified \a painter. Note that it does not fill the background with the background brush (as the
  user may specify with \ref setBackground(const QBrush &brush)), this is up to the respective
  functions calling this method (e.g. \ref replot, \ref toPixmap and \ref toPainter).
*/
void QCustomPlot::draw(QCPPainter *painter)
{
  // run through layout phases:
  mPlotLayout->update(QCPLayoutElement::upPreparation);
  mPlotLayout->update(QCPLayoutElement::upMargins);
  mPlotLayout->update(QCPLayoutElement::upLayout);
  
  // draw viewport background pixmap:
  drawBackground(painter);

  // draw all layered objects (grid, axes, plottables, items, legend,...):
  foreach (QCPLayer *layer, mLayers)
  {
    foreach (QCPLayerable *child, layer->children())
    {
      if (child->realVisibility())
      {
        painter->save();
        painter->setClipRect(child->clipRect().translated(0, -1));
        child->applyDefaultAntialiasingHint(painter);
        child->draw(painter);
        painter->restore();
      }
    }
  }
  
  /* Debug code to draw all layout element rects
  foreach (QCPLayoutElement* el, findChildren<QCPLayoutElement*>())
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(0, 0, 0, 100), 0, Qt::DashLine));
    painter->drawRect(el->rect());
    painter->setPen(QPen(QColor(255, 0, 0, 100), 0, Qt::DashLine));
    painter->drawRect(el->outerRect());
  }
  */
}

/*! \internal
  
  Draws the viewport background pixmap of the plot.
  
  If a pixmap was provided via \ref setBackground, this function buffers the scaled version
  depending on \ref setBackgroundScaled and \ref setBackgroundScaledMode and then draws it inside
  the viewport with the provided \a painter. The scaled version is buffered in
  mScaledBackgroundPixmap to prevent expensive rescaling at every redraw. It is only updated, when
  the axis rect has changed in a way that requires a rescale of the background pixmap (this is
  dependent on the \ref setBackgroundScaledMode), or when a differend axis background pixmap was
  set.
  
  Note that this function does not draw a fill with the background brush (\ref setBackground(const
  QBrush &brush)) beneath the pixmap.
  
  \see setBackground, setBackgroundScaled, setBackgroundScaledMode
*/
void QCustomPlot::drawBackground(QCPPainter *painter)
{
  // Note: background color is handled in individual replot/save functions

  // draw background pixmap (on top of fill, if brush specified):
  if (!mBackgroundPixmap.isNull())
  {
    if (mBackgroundScaled)
    {
      // check whether mScaledBackground needs to be updated:
      QSize scaledSize(mBackgroundPixmap.size());
      scaledSize.scale(mViewport.size(), mBackgroundScaledMode);
      if (mScaledBackgroundPixmap.size() != scaledSize)
        mScaledBackgroundPixmap = mBackgroundPixmap.scaled(mViewport.size(), mBackgroundScaledMode, Qt::SmoothTransformation);
      painter->drawPixmap(mViewport.topLeft(), mScaledBackgroundPixmap, QRect(0, 0, mViewport.width(), mViewport.height()) & mScaledBackgroundPixmap.rect());
    } else
    {
      painter->drawPixmap(mViewport.topLeft(), mBackgroundPixmap, QRect(0, 0, mViewport.width(), mViewport.height()));
    }
  }
}


/*! \internal
  
  This method is used by \ref QCPAxisRect::removeAxis to report removed axes to the QCustomPlot
  so it may clear its QCustomPlot::xAxis, yAxis, xAxis2 and yAxis2 members accordingly.
*/
void QCustomPlot::axisRemoved(QCPAxis *axis)
{
  if (xAxis == axis)
    xAxis = 0;
  if (xAxis2 == axis)
    xAxis2 = 0;
  if (yAxis == axis)
    yAxis = 0;
  if (yAxis2 == axis)
    yAxis2 = 0;
  
  // Note: No need to take care of range drag axes and range zoom axes, because they are stored in smart pointers
}

/*! \internal
  
  This method is used by the QCPLegend destructor to report legend removal to the QCustomPlot so
  it may clear its QCustomPlot::legend member accordingly.
*/
void QCustomPlot::legendRemoved(QCPLegend *legend)
{
  if (this->legend == legend)
    this->legend = 0;
}

/*! \internal
  
  Assigns all layers their index (QCPLayer::mIndex) in the mLayers list. This method is thus called
  after every operation that changes the layer indices, like layer removal, layer creation, layer
  moving.
*/
void QCustomPlot::updateLayerIndices() const
{
  for (int i=0; i<mLayers.size(); ++i)
    mLayers.at(i)->mIndex = i;
}

/*! \internal
  
  Returns the layerable at pixel position \a pos. If \a onlySelectable is set to true, only those
  layerables that are selectable will be considered. (Layerable subclasses communicate their
  selectability via the QCPLayerable::selectTest method, by returning -1.)

  \a selectionDetails is an output parameter that contains selection specifics of the affected
  layerable. This is useful if the respective layerable shall be given a subsequent
  QCPLayerable::selectEvent (like in \ref mouseReleaseEvent). \a selectionDetails usually contains
  information about which part of the layerable was hit, in multi-part layerables (e.g.
  QCPAxis::SelectablePart).
*/
QCPLayerable *QCustomPlot::layerableAt(const QPointF &pos, bool onlySelectable, QVariant *selectionDetails) const
{
  for (int layerIndex=mLayers.size()-1; layerIndex>=0; --layerIndex)
  {
    const QList<QCPLayerable*> layerables = mLayers.at(layerIndex)->children();
    double minimumDistance = selectionTolerance()*1.1;
    QCPLayerable *minimumDistanceLayerable = 0;
    for (int i=layerables.size()-1; i>=0; --i)
    {
      if (!layerables.at(i)->realVisibility())
        continue;
      QVariant details;
      double dist = layerables.at(i)->selectTest(pos, onlySelectable, &details);
      if (dist >= 0 && dist < minimumDistance)
      {
        minimumDistance = dist;
        minimumDistanceLayerable = layerables.at(i);
        if (selectionDetails) *selectionDetails = details;
      }
    }
    if (minimumDistance < selectionTolerance())
      return minimumDistanceLayerable;
  }
  return 0;
}

/*!
  Saves the plot to a rastered image file \a fileName in the image format \a format. The plot is
  sized to \a width and \a height in pixels and scaled with \a scale. (width 100 and scale 2.0 lead
  to a full resolution file with width 200.) If the \a format supports compression, \a quality may
  be between 0 and 100 to control it.
  
  Returns true on success. If this function fails, most likely the given \a format isn't supported
  by the system, see Qt docs about QImageWriter::supportedImageFormats().
  
  \see saveBmp, saveJpg, savePng, savePdf
*/
bool QCustomPlot::saveRastered(const QString &fileName, int width, int height, double scale, const char *format, int quality)
{
  QPixmap buffer = toPixmap(width, height, scale);
  if (!buffer.isNull())
    return buffer.save(fileName, format, quality);
  else
    return false;
}

/*!
  Renders the plot to a pixmap and returns it.
  
  The plot is sized to \a width and \a height in pixels and scaled with \a scale. (width 100 and
  scale 2.0 lead to a full resolution pixmap with width 200.)
  
  \see toPainter, saveRastered, saveBmp, savePng, saveJpg, savePdf
*/
QPixmap QCustomPlot::toPixmap(int width, int height, double scale)
{
  // this method is somewhat similar to toPainter. Change something here, and a change in toPainter might be necessary, too.
  int newWidth, newHeight;
  if (width == 0 || height == 0)
  {
    newWidth = this->width();
    newHeight = this->height();
  } else
  {
    newWidth = width;
    newHeight = height;
  }
  int scaledWidth = qRound(scale*newWidth);
  int scaledHeight = qRound(scale*newHeight);

  QPixmap result(scaledWidth, scaledHeight);
  result.fill(mBackgroundBrush.style() == Qt::SolidPattern ? mBackgroundBrush.color() : Qt::transparent); // if using non-solid pattern, make transparent now and draw brush pattern later
  QCPPainter painter;
  painter.begin(&result);
  if (painter.isActive())
  {
    QRect oldViewport = viewport();
    setViewport(QRect(0, 0, newWidth, newHeight));
    painter.setMode(QCPPainter::pmNoCaching);
    if (!qFuzzyCompare(scale, 1.0))
    {
      if (scale > 1.0) // for scale < 1 we always want cosmetic pens where possible, because else lines might disappear for very small scales
        painter.setMode(QCPPainter::pmNonCosmetic);
      painter.scale(scale, scale);
    }
    if (mBackgroundBrush.style() != Qt::SolidPattern && mBackgroundBrush.style() != Qt::NoBrush)
      painter.fillRect(mViewport, mBackgroundBrush);
    draw(&painter);
    setViewport(oldViewport);
    painter.end();
  } else // might happen if pixmap has width or height zero
  {
    qDebug() << Q_FUNC_INFO << "Couldn't activate painter on pixmap";
    return QPixmap();
  }
  return result;
}

/*!
  Renders the plot using the passed \a painter.
  
  The plot is sized to \a width and \a height in pixels. If the \a painter's scale is not 1.0, the resulting plot will
  appear scaled accordingly.
  
  \note If you are restricted to using a QPainter (instead of QCPPainter), create a temporary QPicture and open a QCPPainter
  on it. Then call \ref toPainter with this QCPPainter. After ending the paint operation on the picture, draw it with
  the QPainter. This will reproduce the painter actions the QCPPainter took, with a QPainter.
  
  \see toPixmap
*/
void QCustomPlot::toPainter(QCPPainter *painter, int width, int height)
{
  // this method is somewhat similar to toPixmap. Change something here, and a change in toPixmap might be necessary, too.
  int newWidth, newHeight;
  if (width == 0 || height == 0)
  {
    newWidth = this->width();
    newHeight = this->height();
  } else
  {
    newWidth = width;
    newHeight = height;
  }

  if (painter->isActive())
  {
    QRect oldViewport = viewport();
    setViewport(QRect(0, 0, newWidth, newHeight));
    painter->setMode(QCPPainter::pmNoCaching);
    // warning: the following is different in toPixmap, because a solid background color is applied there via QPixmap::fill
    // here, we need to do this via QPainter::fillRect.
    if (mBackgroundBrush.style() != Qt::NoBrush)
      painter->fillRect(mViewport, mBackgroundBrush);
    draw(painter);
    setViewport(oldViewport);
  } else
    qDebug() << Q_FUNC_INFO << "Passed painter is not active";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorGradient
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorGradient
  \brief Defines a color gradient for use with e.g. \ref QCPColorMap
  
  This class describes a color gradient which can be used to encode data with color. For example,
  QCPColorMap and QCPColorScale have a \ref QCPColorMap::setGradient "setGradient" method which
  takes an instance of this class. Colors are set with \ref setColorStopAt(double position, const QColor &color)
  with a \a position from 0 to 1. In between these defined color positions, the
  color will be interpolated linearly either in RGB or HSV space, see \ref setColorInterpolation.

  Alternatively, load one of the preset color gradients shown in the image below, with \ref
  loadPreset, or by directly specifying the preset in the constructor.
  
  \image html QCPColorGradient.png
  
  The fact that the \ref QCPColorGradient(GradientPreset preset) constructor allows directly
  converting a \ref GradientPreset to a QCPColorGradient, you can also directly pass \ref
  GradientPreset to all the \a setGradient methods, e.g.:
  \code
  colorMap->setGradient(QCPColorGradient::gpHot);
  \endcode
  
  The total number of levels used in the gradient can be set with \ref setLevelCount. Whether the
  color gradient shall be applied periodically (wrapping around) to data values that lie outside
  the data range specified on the plottable instance can be controlled with \ref setPeriodic.
*/

/*!
  Constructs a new QCPColorGradient initialized with the colors and color interpolation according
  to \a preset.
  
  The color level count is initialized to 350.
*/
QCPColorGradient::QCPColorGradient(GradientPreset preset) :
  mLevelCount(350),
  mColorInterpolation(ciRGB),
  mPeriodic(false),
  mColorBufferInvalidated(true)
{
  mColorBuffer.fill(qRgb(0, 0, 0), mLevelCount);
  loadPreset(preset);
}

/* undocumented operator */
bool QCPColorGradient::operator==(const QCPColorGradient &other) const
{
  return ((other.mLevelCount == this->mLevelCount) &&
          (other.mColorInterpolation == this->mColorInterpolation) &&
          (other.mPeriodic == this->mPeriodic) &&
          (other.mColorStops == this->mColorStops));
}

/*!
  Sets the number of discretization levels of the color gradient to \a n. The default is 350 which
  is typically enough to create a smooth appearance.
  
  \image html QCPColorGradient-levelcount.png
*/
void QCPColorGradient::setLevelCount(int n)
{
  if (n < 2)
  {
    qDebug() << Q_FUNC_INFO << "n must be greater or equal 2 but was" << n;
    n = 2;
  }
  if (n != mLevelCount)
  {
    mLevelCount = n;
    mColorBufferInvalidated = true;
  }
}

/*!
  Sets at which positions from 0 to 1 which color shall occur. The positions are the keys, the
  colors are the values of the passed QMap \a colorStops. In between these color stops, the color
  is interpolated according to \ref setColorInterpolation.
  
  A more convenient way to create a custom gradient may be to clear all color stops with \ref
  clearColorStops and then adding them one by one with \ref setColorStopAt.
  
  \see clearColorStops
*/
void QCPColorGradient::setColorStops(const QMap<double, QColor> &colorStops)
{
  mColorStops = colorStops;
  mColorBufferInvalidated = true;
}

/*!
  Sets the \a color the gradient will have at the specified \a position (from 0 to 1). In between
  these color stops, the color is interpolated according to \ref setColorInterpolation.
  
  \see setColorStops, clearColorStops
*/
void QCPColorGradient::setColorStopAt(double position, const QColor &color)
{
  mColorStops.insert(position, color);
  mColorBufferInvalidated = true;
}

/*!
  Sets whether the colors in between the configured color stops (see \ref setColorStopAt) shall be
  interpolated linearly in RGB or in HSV color space.
  
  For example, a sweep in RGB space from red to green will have a muddy brown intermediate color,
  whereas in HSV space the intermediate color is yellow.
*/
void QCPColorGradient::setColorInterpolation(QCPColorGradient::ColorInterpolation interpolation)
{
  if (interpolation != mColorInterpolation)
  {
    mColorInterpolation = interpolation;
    mColorBufferInvalidated = true;
  }
}

/*!
  Sets whether data points that are outside the configured data range (e.g. \ref
  QCPColorMap::setDataRange) are colored by periodically repeating the color gradient or whether
  they all have the same color, corresponding to the respective gradient boundary color.
  
  \image html QCPColorGradient-periodic.png
  
  As shown in the image above, gradients that have the same start and end color are especially
  suitable for a periodic gradient mapping, since they produce smooth color transitions throughout
  the color map. A preset that has this property is \ref gpHues.
  
  In practice, using periodic color gradients makes sense when the data corresponds to a periodic
  dimension, such as an angle or a phase. If this is not the case, the color encoding might become
  ambiguous, because multiple different data values are shown as the same color.
*/
void QCPColorGradient::setPeriodic(bool enabled)
{
  mPeriodic = enabled;
}

/*!
  This method is used to quickly convert a \a data array to colors. The colors will be output in
  the array \a scanLine. Both \a data and \a scanLine must have the length \a n when passed to this
  function. The data range that shall be used for mapping the data value to the gradient is passed
  in \a range. \a logarithmic indicates whether the data values shall be mapped to colors
  logarithmically.
  
  if \a data actually contains 2D-data linearized via <tt>[row*columnCount + column]</tt>, you can
  set \a dataIndexFactor to <tt>columnCount</tt> to convert a column instead of a row of the data
  array, in \a scanLine. \a scanLine will remain a regular (1D) array. This works because \a data
  is addressed <tt>data[i*dataIndexFactor]</tt>.
*/
void QCPColorGradient::colorize(const double *data, const QCPRange &range, QRgb *scanLine, int n, int dataIndexFactor, bool logarithmic)
{
  // If you change something here, make sure to also adapt ::color()
  if (!data)
  {
    qDebug() << Q_FUNC_INFO << "null pointer given as data";
    return;
  }
  if (!scanLine)
  {
    qDebug() << Q_FUNC_INFO << "null pointer given as scanLine";
    return;
  }
  if (mColorBufferInvalidated)
    updateColorBuffer();
  
  if (!logarithmic)
  {
    const double posToIndexFactor = mLevelCount/range.size();
    if (mPeriodic)
    {
      for (int i=0; i<n; ++i)
      {
        int index = (int)((data[dataIndexFactor*i]-range.lower)*posToIndexFactor) % mLevelCount;
        if (index < 0)
          index += mLevelCount;
        scanLine[i] = mColorBuffer.at(index);
      }
    } else
    {
      for (int i=0; i<n; ++i)
      {
        int index = (data[dataIndexFactor*i]-range.lower)*posToIndexFactor;
        if (index < 0)
          index = 0;
        else if (index >= mLevelCount)
          index = mLevelCount-1;
        scanLine[i] = mColorBuffer.at(index);
      }
    }
  } else // logarithmic == true
  {
    if (mPeriodic)
    {
      for (int i=0; i<n; ++i)
      {
        int index = (int)(qLn(data[dataIndexFactor*i]/range.lower)/qLn(range.upper/range.lower)*mLevelCount) % mLevelCount;
        if (index < 0)
          index += mLevelCount;
        scanLine[i] = mColorBuffer.at(index);
      }
    } else
    {
      for (int i=0; i<n; ++i)
      {
        int index = qLn(data[dataIndexFactor*i]/range.lower)/qLn(range.upper/range.lower)*mLevelCount;
        if (index < 0)
          index = 0;
        else if (index >= mLevelCount)
          index = mLevelCount-1;
        scanLine[i] = mColorBuffer.at(index);
      }
    }
  }
}

/*! \internal
  
  This method is used to colorize a single data value given in \a position, to colors. The data
  range that shall be used for mapping the data value to the gradient is passed in \a range. \a
  logarithmic indicates whether the data value shall be mapped to a color logarithmically.
  
  If an entire array of data values shall be converted, rather use \ref colorize, for better
  performance.
*/
QRgb QCPColorGradient::color(double position, const QCPRange &range, bool logarithmic)
{
  // If you change something here, make sure to also adapt ::colorize()
  if (mColorBufferInvalidated)
    updateColorBuffer();
  int index = 0;
  if (!logarithmic)
    index = (position-range.lower)*mLevelCount/range.size();
  else
    index = qLn(position/range.lower)/qLn(range.upper/range.lower)*mLevelCount;
  if (mPeriodic)
  {
    index = index % mLevelCount;
    if (index < 0)
      index += mLevelCount;
  } else
  {
    if (index < 0)
      index = 0;
    else if (index >= mLevelCount)
      index = mLevelCount-1;
  }
  return mColorBuffer.at(index);
}

/*!
  Clears the current color stops and loads the specified \a preset. A preset consists of predefined
  color stops and the corresponding color interpolation method.
  
  The available presets are:
  \image html QCPColorGradient.png
*/
void QCPColorGradient::loadPreset(GradientPreset preset)
{
  clearColorStops();
  switch (preset)
  {
    case gpGrayscale:
      setColorInterpolation(ciRGB);
      setColorStopAt(0, Qt::black);
      setColorStopAt(1, Qt::white);
      break;
    case gpHot:
      setColorInterpolation(ciRGB);
      setColorStopAt(0, QColor(50, 0, 0));
      setColorStopAt(0.2, QColor(180, 10, 0));
      setColorStopAt(0.4, QColor(245, 50, 0));
      setColorStopAt(0.6, QColor(255, 150, 10));
      setColorStopAt(0.8, QColor(255, 255, 50));
      setColorStopAt(1, QColor(255, 255, 255));
      break;
    case gpCold:
      setColorInterpolation(ciRGB);
      setColorStopAt(0, QColor(0, 0, 50));
      setColorStopAt(0.2, QColor(0, 10, 180));
      setColorStopAt(0.4, QColor(0, 50, 245));
      setColorStopAt(0.6, QColor(10, 150, 255));
      setColorStopAt(0.8, QColor(50, 255, 255));
      setColorStopAt(1, QColor(255, 255, 255));
      break;
    case gpNight:
      setColorInterpolation(ciHSV);
      setColorStopAt(0, QColor(10, 20, 30));
      setColorStopAt(1, QColor(250, 255, 250));
      break;
    case gpCandy:
      setColorInterpolation(ciHSV);
      setColorStopAt(0, QColor(0, 0, 255));
      setColorStopAt(1, QColor(255, 250, 250));
      break;
    case gpGeography:
      setColorInterpolation(ciRGB);
      setColorStopAt(0, QColor(70, 170, 210));
      setColorStopAt(0.20, QColor(90, 160, 180));
      setColorStopAt(0.25, QColor(45, 130, 175));
      setColorStopAt(0.30, QColor(100, 140, 125));
      setColorStopAt(0.5, QColor(100, 140, 100));
      setColorStopAt(0.6, QColor(130, 145, 120));
      setColorStopAt(0.7, QColor(140, 130, 120));
      setColorStopAt(0.9, QColor(180, 190, 190));
      setColorStopAt(1, QColor(210, 210, 230));
      break;
    case gpIon:
      setColorInterpolation(ciHSV);
      setColorStopAt(0, QColor(50, 10, 10));
      setColorStopAt(0.45, QColor(0, 0, 255));
      setColorStopAt(0.8, QColor(0, 255, 255));
      setColorStopAt(1, QColor(0, 255, 0));
      break;
    case gpThermal:
      setColorInterpolation(ciRGB);
      setColorStopAt(0, QColor(0, 0, 50));
      setColorStopAt(0.15, QColor(20, 0, 120));
      setColorStopAt(0.33, QColor(200, 30, 140));
      setColorStopAt(0.6, QColor(255, 100, 0));
      setColorStopAt(0.85, QColor(255, 255, 40));
      setColorStopAt(1, QColor(255, 255, 255));
      break;
    case gpPolar:
      setColorInterpolation(ciRGB);
      setColorStopAt(0, QColor(50, 255, 255));
      setColorStopAt(0.18, QColor(10, 70, 255));
      setColorStopAt(0.28, QColor(10, 10, 190));
      setColorStopAt(0.5, QColor(0, 0, 0));
      setColorStopAt(0.72, QColor(190, 10, 10));
      setColorStopAt(0.82, QColor(255, 70, 10));
      setColorStopAt(1, QColor(255, 255, 50));
      break;
    case gpSpectrum:
      setColorInterpolation(ciHSV);
      setColorStopAt(0, QColor(50, 0, 50));
      setColorStopAt(0.15, QColor(0, 0, 255));
      setColorStopAt(0.35, QColor(0, 255, 255));
      setColorStopAt(0.6, QColor(255, 255, 0));
      setColorStopAt(0.75, QColor(255, 30, 0));
      setColorStopAt(1, QColor(50, 0, 0));
      break;
    case gpJet:
      setColorInterpolation(ciRGB);
      setColorStopAt(0, QColor(0, 0, 100));
      setColorStopAt(0.15, QColor(0, 50, 255));
      setColorStopAt(0.35, QColor(0, 255, 255));
      setColorStopAt(0.65, QColor(255, 255, 0));
      setColorStopAt(0.85, QColor(255, 30, 0));
      setColorStopAt(1, QColor(100, 0, 0));
      break;
    case gpHues:
      setColorInterpolation(ciHSV);
      setColorStopAt(0, QColor(255, 0, 0));
      setColorStopAt(1.0/3.0, QColor(0, 0, 255));
      setColorStopAt(2.0/3.0, QColor(0, 255, 0));
      setColorStopAt(1, QColor(255, 0, 0));
      break;
  }
}

/*!
  Clears all color stops.
  
  \see setColorStops, setColorStopAt
*/
void QCPColorGradient::clearColorStops()
{
  mColorStops.clear();
  mColorBufferInvalidated = true;
}

/*!
  Returns an inverted gradient. The inverted gradient has all properties as this \ref
  QCPColorGradient, but the order of the color stops is inverted.
  
  \see setColorStops, setColorStopAt
*/
QCPColorGradient QCPColorGradient::inverted() const
{
  QCPColorGradient result(*this);
  result.clearColorStops();
  for (QMap<double, QColor>::const_iterator it=mColorStops.constBegin(); it!=mColorStops.constEnd(); ++it)
    result.setColorStopAt(1.0-it.key(), it.value());
  return result;
}

/*! \internal
  
  Updates the internal color buffer which will be used by \ref colorize and \ref color, to quickly
  convert positions to colors. This is where the interpolation between color stops is calculated.
*/
void QCPColorGradient::updateColorBuffer()
{
  if (mColorBuffer.size() != mLevelCount)
    mColorBuffer.resize(mLevelCount);
  if (mColorStops.size() > 1)
  {
    double indexToPosFactor = 1.0/(double)(mLevelCount-1);
    for (int i=0; i<mLevelCount; ++i)
    {
      double position = i*indexToPosFactor;
      QMap<double, QColor>::const_iterator it = mColorStops.lowerBound(position);
      if (it == mColorStops.constEnd()) // position is on or after last stop, use color of last stop
      {
        mColorBuffer[i] = (it-1).value().rgb();
      } else if (it == mColorStops.constBegin()) // position is on or before first stop, use color of first stop
      {
        mColorBuffer[i] = it.value().rgb();
      } else // position is in between stops (or on an intermediate stop), interpolate color
      {
        QMap<double, QColor>::const_iterator high = it;
        QMap<double, QColor>::const_iterator low = it-1;
        double t = (position-low.key())/(high.key()-low.key()); // interpolation factor 0..1
        switch (mColorInterpolation)
        {
          case ciRGB:
          {
            mColorBuffer[i] = qRgb((1-t)*low.value().red() + t*high.value().red(),
                                   (1-t)*low.value().green() + t*high.value().green(),
                                   (1-t)*low.value().blue() + t*high.value().blue());
            break;
          }
          case ciHSV:
          {
            QColor lowHsv = low.value().toHsv();
            QColor highHsv = high.value().toHsv();
            double hue = 0;
            double hueDiff = highHsv.hueF()-lowHsv.hueF();
            if (hueDiff > 0.5)
              hue = lowHsv.hueF() - t*(1.0-hueDiff);
            else if (hueDiff < -0.5)
              hue = lowHsv.hueF() + t*(1.0+hueDiff);
            else
              hue = lowHsv.hueF() + t*hueDiff;
            if (hue < 0) hue += 1.0;
            else if (hue >= 1.0) hue -= 1.0;
            mColorBuffer[i] = QColor::fromHsvF(hue, (1-t)*lowHsv.saturationF() + t*highHsv.saturationF(), (1-t)*lowHsv.valueF() + t*highHsv.valueF()).rgb();
            break;
          }
        }
      }
    }
  } else if (mColorStops.size() == 1)
  {
    mColorBuffer.fill(mColorStops.constBegin().value().rgb());
  } else // mColorStops is empty, fill color buffer with black
  {
    mColorBuffer.fill(qRgb(0, 0, 0));
  }
  mColorBufferInvalidated = false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxisRect
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAxisRect
  \brief Holds multiple axes and arranges them in a rectangular shape.
  
  This class represents an axis rect, a rectangular area that is bounded on all sides with an
  arbitrary number of axes.
  
  Initially QCustomPlot has one axis rect, accessible via QCustomPlot::axisRect(). However, the
  layout system allows to have multiple axis rects, e.g. arranged in a grid layout
  (QCustomPlot::plotLayout).
  
  By default, QCPAxisRect comes with four axes, at bottom, top, left and right. They can be
  accessed via \ref axis by providing the respective axis type (\ref QCPAxis::AxisType) and index.
  If you need all axes in the axis rect, use \ref axes. The top and right axes are set to be
  invisible initially (QCPAxis::setVisible). To add more axes to a side, use \ref addAxis or \ref
  addAxes. To remove an axis, use \ref removeAxis.
  
  The axis rect layerable itself only draws a background pixmap or color, if specified (\ref
  setBackground). It is placed on the "background" layer initially (see \ref QCPLayer for an
  explanation of the QCustomPlot layer system). The axes that are held by the axis rect can be
  placed on other layers, independently of the axis rect.
  
  Every axis rect has a child layout of type \ref QCPLayoutInset. It is accessible via \ref
  insetLayout and can be used to have other layout elements (or even other layouts with multiple
  elements) hovering inside the axis rect.
  
  If an axis rect is clicked and dragged, it processes this by moving certain axis ranges. The
  behaviour can be controlled with \ref setRangeDrag and \ref setRangeDragAxes. If the mouse wheel
  is scrolled while the cursor is on the axis rect, certain axes are scaled. This is controllable
  via \ref setRangeZoom, \ref setRangeZoomAxes and \ref setRangeZoomFactor. These interactions are
  only enabled if \ref QCustomPlot::setInteractions contains \ref QCP::iRangeDrag and \ref
  QCP::iRangeZoom.
  
  \image html AxisRectSpacingOverview.png
  <center>Overview of the spacings and paddings that define the geometry of an axis. The dashed
  line on the far left indicates the viewport/widget border.</center>
*/

/* start documentation of inline functions */

/*! \fn QCPLayoutInset *QCPAxisRect::insetLayout() const
  
  Returns the inset layout of this axis rect. It can be used to place other layout elements (or
  even layouts with multiple other elements) inside/on top of an axis rect.
  
  \see QCPLayoutInset
*/

/*! \fn int QCPAxisRect::left() const
  
  Returns the pixel position of the left border of this axis rect. Margins are not taken into
  account here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn int QCPAxisRect::right() const
  
  Returns the pixel position of the right border of this axis rect. Margins are not taken into
  account here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn int QCPAxisRect::top() const
  
  Returns the pixel position of the top border of this axis rect. Margins are not taken into
  account here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn int QCPAxisRect::bottom() const
  
  Returns the pixel position of the bottom border of this axis rect. Margins are not taken into
  account here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn int QCPAxisRect::width() const
  
  Returns the pixel width of this axis rect. Margins are not taken into account here, so the
  returned value is with respect to the inner \ref rect.
*/

/*! \fn int QCPAxisRect::height() const
  
  Returns the pixel height of this axis rect. Margins are not taken into account here, so the
  returned value is with respect to the inner \ref rect.
*/

/*! \fn QSize QCPAxisRect::size() const
  
  Returns the pixel size of this axis rect. Margins are not taken into account here, so the
  returned value is with respect to the inner \ref rect.
*/

/*! \fn QPoint QCPAxisRect::topLeft() const
  
  Returns the top left corner of this axis rect in pixels. Margins are not taken into account here,
  so the returned value is with respect to the inner \ref rect.
*/

/*! \fn QPoint QCPAxisRect::topRight() const
  
  Returns the top right corner of this axis rect in pixels. Margins are not taken into account
  here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn QPoint QCPAxisRect::bottomLeft() const
  
  Returns the bottom left corner of this axis rect in pixels. Margins are not taken into account
  here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn QPoint QCPAxisRect::bottomRight() const
  
  Returns the bottom right corner of this axis rect in pixels. Margins are not taken into account
  here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn QPoint QCPAxisRect::center() const
  
  Returns the center of this axis rect in pixels. Margins are not taken into account here, so the
  returned value is with respect to the inner \ref rect.
*/

/* end documentation of inline functions */

/*!
  Creates a QCPAxisRect instance and sets default values. An axis is added for each of the four
  sides, the top and right axes are set invisible initially.
*/
QCPAxisRect::QCPAxisRect(QCustomPlot *parentPlot, bool setupDefaultAxes) :
  QCPLayoutElement(parentPlot),
  mBackgroundBrush(Qt::NoBrush),
  mBackgroundScaled(true),
  mBackgroundScaledMode(Qt::KeepAspectRatioByExpanding),
  mInsetLayout(new QCPLayoutInset),
  mRangeDrag(Qt::Horizontal|Qt::Vertical),
  mRangeZoom(Qt::Horizontal|Qt::Vertical),
  mRangeZoomFactorHorz(0.85),
  mRangeZoomFactorVert(0.85),
  mDragging(false)
{
  mInsetLayout->initializeParentPlot(mParentPlot);
  mInsetLayout->setParentLayerable(this);
  mInsetLayout->setParent(this);
  
  setMinimumSize(50, 50);
  setMinimumMargins(QMargins(15, 15, 15, 15));
  mAxes.insert(QCPAxis::atLeft, QList<QCPAxis*>());
  mAxes.insert(QCPAxis::atRight, QList<QCPAxis*>());
  mAxes.insert(QCPAxis::atTop, QList<QCPAxis*>());
  mAxes.insert(QCPAxis::atBottom, QList<QCPAxis*>());
  
  if (setupDefaultAxes)
  {
    QCPAxis *xAxis = addAxis(QCPAxis::atBottom);
    QCPAxis *yAxis = addAxis(QCPAxis::atLeft);
    QCPAxis *xAxis2 = addAxis(QCPAxis::atTop);
    QCPAxis *yAxis2 = addAxis(QCPAxis::atRight);
    setRangeDragAxes(xAxis, yAxis);
    setRangeZoomAxes(xAxis, yAxis);
    xAxis2->setVisible(false);
    yAxis2->setVisible(false);
    xAxis->grid()->setVisible(true);
    yAxis->grid()->setVisible(true);
    xAxis2->grid()->setVisible(false);
    yAxis2->grid()->setVisible(false);
    xAxis2->grid()->setZeroLinePen(Qt::NoPen);
    yAxis2->grid()->setZeroLinePen(Qt::NoPen);
    xAxis2->grid()->setVisible(false);
    yAxis2->grid()->setVisible(false);
  }
}

QCPAxisRect::~QCPAxisRect()
{
  delete mInsetLayout;
  mInsetLayout = 0;
  
  QList<QCPAxis*> axesList = axes();
  for (int i=0; i<axesList.size(); ++i)
    removeAxis(axesList.at(i));
}

/*!
  Returns the number of axes on the axis rect side specified with \a type.
  
  \see axis
*/
int QCPAxisRect::axisCount(QCPAxis::AxisType type) const
{
  return mAxes.value(type).size();
}

/*!
  Returns the axis with the given \a index on the axis rect side specified with \a type.
  
  \see axisCount, axes
*/
QCPAxis *QCPAxisRect::axis(QCPAxis::AxisType type, int index) const
{
  QList<QCPAxis*> ax(mAxes.value(type));
  if (index >= 0 && index < ax.size())
  {
    return ax.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "Axis index out of bounds:" << index;
    return 0;
  }
}

/*!
  Returns all axes on the axis rect sides specified with \a types.
  
  \a types may be a single \ref QCPAxis::AxisType or an <tt>or</tt>-combination, to get the axes of
  multiple sides.
  
  \see axis
*/
QList<QCPAxis*> QCPAxisRect::axes(QCPAxis::AxisTypes types) const
{
  QList<QCPAxis*> result;
  if (types.testFlag(QCPAxis::atLeft))
    result << mAxes.value(QCPAxis::atLeft);
  if (types.testFlag(QCPAxis::atRight))
    result << mAxes.value(QCPAxis::atRight);
  if (types.testFlag(QCPAxis::atTop))
    result << mAxes.value(QCPAxis::atTop);
  if (types.testFlag(QCPAxis::atBottom))
    result << mAxes.value(QCPAxis::atBottom);
  return result;
}

/*! \overload
  
  Returns all axes of this axis rect.
*/
QList<QCPAxis*> QCPAxisRect::axes() const
{
  QList<QCPAxis*> result;
  QHashIterator<QCPAxis::AxisType, QList<QCPAxis*> > it(mAxes);
  while (it.hasNext())
  {
    it.next();
    result << it.value();
  }
  return result;
}

/*!
  Adds a new axis to the axis rect side specified with \a type, and returns it.
  
  If an axis rect side already contains one or more axes, the lower and upper endings of the new
  axis (\ref QCPAxis::setLowerEnding, \ref QCPAxis::setUpperEnding) are initialized to \ref
  QCPLineEnding::esHalfBar.
  
  \see addAxes, setupFullAxesBox
*/
QCPAxis *QCPAxisRect::addAxis(QCPAxis::AxisType type)
{
  QCPAxis *newAxis = new QCPAxis(this, type);
  if (mAxes[type].size() > 0) // multiple axes on one side, add half-bar axis ending to additional axes with offset
  {
    bool invert = (type == QCPAxis::atRight) || (type == QCPAxis::atBottom);
    newAxis->setLowerEnding(QCPLineEnding(QCPLineEnding::esHalfBar, 6, 10, !invert));
    newAxis->setUpperEnding(QCPLineEnding(QCPLineEnding::esHalfBar, 6, 10, invert));
  }
  mAxes[type].append(newAxis);
  return newAxis;
}

/*!
  Adds a new axis with \ref addAxis to each axis rect side specified in \a types. This may be an
  <tt>or</tt>-combination of QCPAxis::AxisType, so axes can be added to multiple sides at once.
  
  Returns a list of the added axes.
  
  \see addAxis, setupFullAxesBox
*/
QList<QCPAxis*> QCPAxisRect::addAxes(QCPAxis::AxisTypes types)
{
  QList<QCPAxis*> result;
  if (types.testFlag(QCPAxis::atLeft))
    result << addAxis(QCPAxis::atLeft);
  if (types.testFlag(QCPAxis::atRight))
    result << addAxis(QCPAxis::atRight);
  if (types.testFlag(QCPAxis::atTop))
    result << addAxis(QCPAxis::atTop);
  if (types.testFlag(QCPAxis::atBottom))
    result << addAxis(QCPAxis::atBottom);
  return result;
}

/*!
  Removes the specified \a axis from the axis rect and deletes it.
  
  Returns true on success, i.e. if \a axis was a valid axis in this axis rect.
  
  \see addAxis
*/
bool QCPAxisRect::removeAxis(QCPAxis *axis)
{
  // don't access axis->axisType() to provide safety when axis is an invalid pointer, rather go through all axis containers:
  QHashIterator<QCPAxis::AxisType, QList<QCPAxis*> > it(mAxes);
  while (it.hasNext())
  {
    it.next();
    if (it.value().contains(axis))
    {
      mAxes[it.key()].removeOne(axis);
      if (qobject_cast<QCustomPlot*>(parentPlot())) // make sure this isn't called from QObject dtor when QCustomPlot is already destructed (happens when the axis rect is not in any layout and thus QObject-child of QCustomPlot)
        parentPlot()->axisRemoved(axis);
      delete axis;
      return true;
    }
  }
  qDebug() << Q_FUNC_INFO << "Axis isn't in axis rect:" << reinterpret_cast<quintptr>(axis);
  return false;
}

/*!
  Convenience function to create an axis on each side that doesn't have any axes yet and set their
  visibility to true. Further, the top/right axes are assigned the following properties of the
  bottom/left axes:
  
  \li range (\ref QCPAxis::setRange)
  \li range reversed (\ref QCPAxis::setRangeReversed)
  \li scale type (\ref QCPAxis::setScaleType)
  \li scale log base  (\ref QCPAxis::setScaleLogBase)
  \li ticks (\ref QCPAxis::setTicks)
  \li auto (major) tick count (\ref QCPAxis::setAutoTickCount)
  \li sub tick count (\ref QCPAxis::setSubTickCount)
  \li auto sub ticks (\ref QCPAxis::setAutoSubTicks)
  \li tick step (\ref QCPAxis::setTickStep)
  \li auto tick step (\ref QCPAxis::setAutoTickStep)
  \li number format (\ref QCPAxis::setNumberFormat)
  \li number precision (\ref QCPAxis::setNumberPrecision)
  \li tick label type (\ref QCPAxis::setTickLabelType)
  \li date time format (\ref QCPAxis::setDateTimeFormat)
  \li date time spec (\ref QCPAxis::setDateTimeSpec)
  
  Tick labels (\ref QCPAxis::setTickLabels) of the right and top axes are set to false.

  If \a connectRanges is true, the \ref QCPAxis::rangeChanged "rangeChanged" signals of the bottom
  and left axes are connected to the \ref QCPAxis::setRange slots of the top and right axes.
*/
void QCPAxisRect::setupFullAxesBox(bool connectRanges)
{
  QCPAxis *xAxis, *yAxis, *xAxis2, *yAxis2;
  if (axisCount(QCPAxis::atBottom) == 0)
    xAxis = addAxis(QCPAxis::atBottom);
  else
    xAxis = axis(QCPAxis::atBottom);
  
  if (axisCount(QCPAxis::atLeft) == 0)
    yAxis = addAxis(QCPAxis::atLeft);
  else
    yAxis = axis(QCPAxis::atLeft);
  
  if (axisCount(QCPAxis::atTop) == 0)
    xAxis2 = addAxis(QCPAxis::atTop);
  else
    xAxis2 = axis(QCPAxis::atTop);
  
  if (axisCount(QCPAxis::atRight) == 0)
    yAxis2 = addAxis(QCPAxis::atRight);
  else
    yAxis2 = axis(QCPAxis::atRight);
  
  xAxis->setVisible(true);
  yAxis->setVisible(true);
  xAxis2->setVisible(true);
  yAxis2->setVisible(true);
  xAxis2->setTickLabels(false);
  yAxis2->setTickLabels(false);
  
  xAxis2->setRange(xAxis->range());
  xAxis2->setRangeReversed(xAxis->rangeReversed());
  xAxis2->setScaleType(xAxis->scaleType());
  xAxis2->setScaleLogBase(xAxis->scaleLogBase());
  xAxis2->setTicks(xAxis->ticks());
  xAxis2->setAutoTickCount(xAxis->autoTickCount());
  xAxis2->setSubTickCount(xAxis->subTickCount());
  xAxis2->setAutoSubTicks(xAxis->autoSubTicks());
  xAxis2->setTickStep(xAxis->tickStep());
  xAxis2->setAutoTickStep(xAxis->autoTickStep());
  xAxis2->setNumberFormat(xAxis->numberFormat());
  xAxis2->setNumberPrecision(xAxis->numberPrecision());
  xAxis2->setTickLabelType(xAxis->tickLabelType());
  xAxis2->setDateTimeFormat(xAxis->dateTimeFormat());
  xAxis2->setDateTimeSpec(xAxis->dateTimeSpec());

  yAxis2->setRange(yAxis->range());
  yAxis2->setRangeReversed(yAxis->rangeReversed());
  yAxis2->setScaleType(yAxis->scaleType());
  yAxis2->setScaleLogBase(yAxis->scaleLogBase());
  yAxis2->setTicks(yAxis->ticks());
  yAxis2->setAutoTickCount(yAxis->autoTickCount());
  yAxis2->setSubTickCount(yAxis->subTickCount());
  yAxis2->setAutoSubTicks(yAxis->autoSubTicks());
  yAxis2->setTickStep(yAxis->tickStep());
  yAxis2->setAutoTickStep(yAxis->autoTickStep());
  yAxis2->setNumberFormat(yAxis->numberFormat());
  yAxis2->setNumberPrecision(yAxis->numberPrecision());
  yAxis2->setTickLabelType(yAxis->tickLabelType());
  yAxis2->setDateTimeFormat(yAxis->dateTimeFormat());
  yAxis2->setDateTimeSpec(yAxis->dateTimeSpec());
  
  if (connectRanges)
  {
    connect(xAxis, SIGNAL(rangeChanged(QCPRange)), xAxis2, SLOT(setRange(QCPRange)));
    connect(yAxis, SIGNAL(rangeChanged(QCPRange)), yAxis2, SLOT(setRange(QCPRange)));
  }
}

/*!
  Returns a list of all the plottables that are associated with this axis rect.
  
  A plottable is considered associated with an axis rect if its key or value axis (or both) is in
  this axis rect.
  
  \see graphs, items
*/
QList<QCPAbstractPlottable*> QCPAxisRect::plottables() const
{
  // Note: don't append all QCPAxis::plottables() into a list, because we might get duplicate entries
  QList<QCPAbstractPlottable*> result;
  for (int i=0; i<mParentPlot->mPlottables.size(); ++i)
  {
    if (mParentPlot->mPlottables.at(i)->keyAxis()->axisRect() == this ||mParentPlot->mPlottables.at(i)->valueAxis()->axisRect() == this)
      result.append(mParentPlot->mPlottables.at(i));
  }
  return result;
}

/*!
  Returns a list of all the graphs that are associated with this axis rect.
  
  A graph is considered associated with an axis rect if its key or value axis (or both) is in
  this axis rect.
  
  \see plottables, items
*/
QList<QCPGraph*> QCPAxisRect::graphs() const
{
  // Note: don't append all QCPAxis::graphs() into a list, because we might get duplicate entries
  QList<QCPGraph*> result;
  for (int i=0; i<mParentPlot->mGraphs.size(); ++i)
  {
    if (mParentPlot->mGraphs.at(i)->keyAxis()->axisRect() == this || mParentPlot->mGraphs.at(i)->valueAxis()->axisRect() == this)
      result.append(mParentPlot->mGraphs.at(i));
  }
  return result;
}

/*!
  Returns a list of all the items that are associated with this axis rect.
  
  An item is considered associated with an axis rect if any of its positions has key or value axis
  set to an axis that is in this axis rect, or if any of its positions has \ref
  QCPItemPosition::setAxisRect set to the axis rect, or if the clip axis rect (\ref
  QCPAbstractItem::setClipAxisRect) is set to this axis rect.
  
  \see plottables, graphs
*/
QList<QCPAbstractItem *> QCPAxisRect::items() const
{
  // Note: don't just append all QCPAxis::items() into a list, because we might get duplicate entries
  //       and miss those items that have this axis rect as clipAxisRect.
  QList<QCPAbstractItem*> result;
  for (int itemId=0; itemId<mParentPlot->mItems.size(); ++itemId)
  {
    if (mParentPlot->mItems.at(itemId)->clipAxisRect() == this)
    {
      result.append(mParentPlot->mItems.at(itemId));
      continue;
    }
    QList<QCPItemPosition*> positions = mParentPlot->mItems.at(itemId)->positions();
    for (int posId=0; posId<positions.size(); ++posId)
    {
      if (positions.at(posId)->axisRect() == this ||
          positions.at(posId)->keyAxis()->axisRect() == this ||
          positions.at(posId)->valueAxis()->axisRect() == this)
      {
        result.append(mParentPlot->mItems.at(itemId));
        break;
      }
    }
  }
  return result;
}

/*!
  This method is called automatically upon replot and doesn't need to be called by users of
  QCPAxisRect.
  
  Calls the base class implementation to update the margins (see \ref QCPLayoutElement::update),
  and finally passes the \ref rect to the inset layout (\ref insetLayout) and calls its
  QCPInsetLayout::update function.
*/
void QCPAxisRect::update(UpdatePhase phase)
{
  QCPLayoutElement::update(phase);
  
  switch (phase)
  {
    case upPreparation:
    {
      QList<QCPAxis*> allAxes = axes();
      for (int i=0; i<allAxes.size(); ++i)
        allAxes.at(i)->setupTickVectors();
      break;
    }
    case upLayout:
    {
      mInsetLayout->setOuterRect(rect());
      break;
    }
    default: break;
  }
  
  // pass update call on to inset layout (doesn't happen automatically, because QCPAxisRect doesn't derive from QCPLayout):
  mInsetLayout->update(phase);
}

/* inherits documentation from base class */
QList<QCPLayoutElement*> QCPAxisRect::elements(bool recursive) const
{
  QList<QCPLayoutElement*> result;
  if (mInsetLayout)
  {
    result << mInsetLayout;
    if (recursive)
      result << mInsetLayout->elements(recursive);
  }
  return result;
}

/* inherits documentation from base class */
void QCPAxisRect::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  painter->setAntialiasing(false);
}

/* inherits documentation from base class */
void QCPAxisRect::draw(QCPPainter *painter)
{
  drawBackground(painter);
}

/*!
  Sets \a pm as the axis background pixmap. The axis background pixmap will be drawn inside the
  axis rect. Since axis rects place themselves on the "background" layer by default, the axis rect
  backgrounds are usually drawn below everything else.

  For cases where the provided pixmap doesn't have the same size as the axis rect, scaling can be
  enabled with \ref setBackgroundScaled and the scaling mode (i.e. whether and how the aspect ratio
  is preserved) can be set with \ref setBackgroundScaledMode. To set all these options in one call,
  consider using the overloaded version of this function.

  Below the pixmap, the axis rect may be optionally filled with a brush, if specified with \ref
  setBackground(const QBrush &brush).
  
  \see setBackgroundScaled, setBackgroundScaledMode, setBackground(const QBrush &brush)
*/
void QCPAxisRect::setBackground(const QPixmap &pm)
{
  mBackgroundPixmap = pm;
  mScaledBackgroundPixmap = QPixmap();
}

/*! \overload
  
  Sets \a brush as the background brush. The axis rect background will be filled with this brush.
  Since axis rects place themselves on the "background" layer by default, the axis rect backgrounds
  are usually drawn below everything else.

  The brush will be drawn before (under) any background pixmap, which may be specified with \ref
  setBackground(const QPixmap &pm).

  To disable drawing of a background brush, set \a brush to Qt::NoBrush.
  
  \see setBackground(const QPixmap &pm)
*/
void QCPAxisRect::setBackground(const QBrush &brush)
{
  mBackgroundBrush = brush;
}

/*! \overload
  
  Allows setting the background pixmap of the axis rect, whether it shall be scaled and how it
  shall be scaled in one call.

  \see setBackground(const QPixmap &pm), setBackgroundScaled, setBackgroundScaledMode
*/
void QCPAxisRect::setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode)
{
  mBackgroundPixmap = pm;
  mScaledBackgroundPixmap = QPixmap();
  mBackgroundScaled = scaled;
  mBackgroundScaledMode = mode;
}

/*!
  Sets whether the axis background pixmap shall be scaled to fit the axis rect or not. If \a scaled
  is set to true, you may control whether and how the aspect ratio of the original pixmap is
  preserved with \ref setBackgroundScaledMode.
  
  Note that the scaled version of the original pixmap is buffered, so there is no performance
  penalty on replots. (Except when the axis rect dimensions are changed continuously.)
  
  \see setBackground, setBackgroundScaledMode
*/
void QCPAxisRect::setBackgroundScaled(bool scaled)
{
  mBackgroundScaled = scaled;
}

/*!
  If scaling of the axis background pixmap is enabled (\ref setBackgroundScaled), use this function to
  define whether and how the aspect ratio of the original pixmap passed to \ref setBackground is preserved.
  \see setBackground, setBackgroundScaled
*/
void QCPAxisRect::setBackgroundScaledMode(Qt::AspectRatioMode mode)
{
  mBackgroundScaledMode = mode;
}

/*!
  Returns the range drag axis of the \a orientation provided.
  
  \see setRangeDragAxes
*/
QCPAxis *QCPAxisRect::rangeDragAxis(Qt::Orientation orientation)
{
  return (orientation == Qt::Horizontal ? mRangeDragHorzAxis.data() : mRangeDragVertAxis.data());
}

/*!
  Returns the range zoom axis of the \a orientation provided.
  
  \see setRangeZoomAxes
*/
QCPAxis *QCPAxisRect::rangeZoomAxis(Qt::Orientation orientation)
{
  return (orientation == Qt::Horizontal ? mRangeZoomHorzAxis.data() : mRangeZoomVertAxis.data());
}

/*!
  Returns the range zoom factor of the \a orientation provided.
  
  \see setRangeZoomFactor
*/
double QCPAxisRect::rangeZoomFactor(Qt::Orientation orientation)
{
  return (orientation == Qt::Horizontal ? mRangeZoomFactorHorz : mRangeZoomFactorVert);
}

/*!
  Sets which axis orientation may be range dragged by the user with mouse interaction.
  What orientation corresponds to which specific axis can be set with
  \ref setRangeDragAxes(QCPAxis *horizontal, QCPAxis *vertical). By
  default, the horizontal axis is the bottom axis (xAxis) and the vertical axis
  is the left axis (yAxis).
  
  To disable range dragging entirely, pass 0 as \a orientations or remove \ref QCP::iRangeDrag from \ref
  QCustomPlot::setInteractions. To enable range dragging for both directions, pass <tt>Qt::Horizontal |
  Qt::Vertical</tt> as \a orientations.
  
  In addition to setting \a orientations to a non-zero value, make sure \ref QCustomPlot::setInteractions
  contains \ref QCP::iRangeDrag to enable the range dragging interaction.
  
  \see setRangeZoom, setRangeDragAxes, setNoAntialiasingOnDrag
*/
void QCPAxisRect::setRangeDrag(Qt::Orientations orientations)
{
  mRangeDrag = orientations;
}

/*!
  Sets which axis orientation may be zoomed by the user with the mouse wheel. What orientation
  corresponds to which specific axis can be set with \ref setRangeZoomAxes(QCPAxis *horizontal,
  QCPAxis *vertical). By default, the horizontal axis is the bottom axis (xAxis) and the vertical
  axis is the left axis (yAxis).

  To disable range zooming entirely, pass 0 as \a orientations or remove \ref QCP::iRangeZoom from \ref
  QCustomPlot::setInteractions. To enable range zooming for both directions, pass <tt>Qt::Horizontal |
  Qt::Vertical</tt> as \a orientations.
  
  In addition to setting \a orientations to a non-zero value, make sure \ref QCustomPlot::setInteractions
  contains \ref QCP::iRangeZoom to enable the range zooming interaction.
  
  \see setRangeZoomFactor, setRangeZoomAxes, setRangeDrag
*/
void QCPAxisRect::setRangeZoom(Qt::Orientations orientations)
{
  mRangeZoom = orientations;
}

/*!
  Sets the axes whose range will be dragged when \ref setRangeDrag enables mouse range dragging
  on the QCustomPlot widget.
  
  \see setRangeZoomAxes
*/
void QCPAxisRect::setRangeDragAxes(QCPAxis *horizontal, QCPAxis *vertical)
{
  mRangeDragHorzAxis = horizontal;
  mRangeDragVertAxis = vertical;
}

/*!
  Sets the axes whose range will be zoomed when \ref setRangeZoom enables mouse wheel zooming on the
  QCustomPlot widget. The two axes can be zoomed with different strengths, when different factors
  are passed to \ref setRangeZoomFactor(double horizontalFactor, double verticalFactor).
  
  \see setRangeDragAxes
*/
void QCPAxisRect::setRangeZoomAxes(QCPAxis *horizontal, QCPAxis *vertical)
{
  mRangeZoomHorzAxis = horizontal;
  mRangeZoomVertAxis = vertical;
}

/*!
  Sets how strong one rotation step of the mouse wheel zooms, when range zoom was activated with
  \ref setRangeZoom. The two parameters \a horizontalFactor and \a verticalFactor provide a way to
  let the horizontal axis zoom at different rates than the vertical axis. Which axis is horizontal
  and which is vertical, can be set with \ref setRangeZoomAxes.

  When the zoom factor is greater than one, scrolling the mouse wheel backwards (towards the user)
  will zoom in (make the currently visible range smaller). For zoom factors smaller than one, the
  same scrolling direction will zoom out.
*/
void QCPAxisRect::setRangeZoomFactor(double horizontalFactor, double verticalFactor)
{
  mRangeZoomFactorHorz = horizontalFactor;
  mRangeZoomFactorVert = verticalFactor;
}

/*! \overload
  
  Sets both the horizontal and vertical zoom \a factor.
*/
void QCPAxisRect::setRangeZoomFactor(double factor)
{
  mRangeZoomFactorHorz = factor;
  mRangeZoomFactorVert = factor;
}

/*! \internal
  
  Draws the background of this axis rect. It may consist of a background fill (a QBrush) and a
  pixmap.
  
  If a brush was given via \ref setBackground(const QBrush &brush), this function first draws an
  according filling inside the axis rect with the provided \a painter.
  
  Then, if a pixmap was provided via \ref setBackground, this function buffers the scaled version
  depending on \ref setBackgroundScaled and \ref setBackgroundScaledMode and then draws it inside
  the axis rect with the provided \a painter. The scaled version is buffered in
  mScaledBackgroundPixmap to prevent expensive rescaling at every redraw. It is only updated, when
  the axis rect has changed in a way that requires a rescale of the background pixmap (this is
  dependant on the \ref setBackgroundScaledMode), or when a differend axis backgroud pixmap was
  set.
  
  \see setBackground, setBackgroundScaled, setBackgroundScaledMode
*/
void QCPAxisRect::drawBackground(QCPPainter *painter)
{
  // draw background fill:
  if (mBackgroundBrush != Qt::NoBrush)
    painter->fillRect(mRect, mBackgroundBrush);
  
  // draw background pixmap (on top of fill, if brush specified):
  if (!mBackgroundPixmap.isNull())
  {
    if (mBackgroundScaled)
    {
      // check whether mScaledBackground needs to be updated:
      QSize scaledSize(mBackgroundPixmap.size());
      scaledSize.scale(mRect.size(), mBackgroundScaledMode);
      if (mScaledBackgroundPixmap.size() != scaledSize)
        mScaledBackgroundPixmap = mBackgroundPixmap.scaled(mRect.size(), mBackgroundScaledMode, Qt::SmoothTransformation);
      painter->drawPixmap(mRect.topLeft(), mScaledBackgroundPixmap, QRect(0, 0, mRect.width(), mRect.height()) & mScaledBackgroundPixmap.rect());
    } else
    {
      painter->drawPixmap(mRect.topLeft(), mBackgroundPixmap, QRect(0, 0, mRect.width(), mRect.height()));
    }
  }
}

/*! \internal
  
  This function makes sure multiple axes on the side specified with \a type don't collide, but are
  distributed according to their respective space requirement (QCPAxis::calculateMargin).
  
  It does this by setting an appropriate offset (\ref QCPAxis::setOffset) on all axes except the
  one with index zero.
  
  This function is called by \ref calculateAutoMargin.
*/
void QCPAxisRect::updateAxesOffset(QCPAxis::AxisType type)
{
  const QList<QCPAxis*> axesList = mAxes.value(type);
  if (axesList.isEmpty())
    return;
  
  bool isFirstVisible = !axesList.first()->visible(); // if the first axis is visible, the second axis (which is where the loop starts) isn't the first visible axis, so initialize with false
  for (int i=1; i<axesList.size(); ++i)
  {
    int offset = axesList.at(i-1)->offset() + axesList.at(i-1)->calculateMargin();
    if (axesList.at(i)->visible()) // only add inner tick length to offset if this axis is visible and it's not the first visible one (might happen if true first axis is invisible)
    {
      if (!isFirstVisible)
        offset += axesList.at(i)->tickLengthIn();
      isFirstVisible = false;
    }
    axesList.at(i)->setOffset(offset);
  }
}

/* inherits documentation from base class */
int QCPAxisRect::calculateAutoMargin(QCP::MarginSide side)
{
  if (!mAutoMargins.testFlag(side))
    qDebug() << Q_FUNC_INFO << "Called with side that isn't specified as auto margin";
  
  updateAxesOffset(QCPAxis::marginSideToAxisType(side));
  
  // note: only need to look at the last (outer most) axis to determine the total margin, due to updateAxisOffset call
  const QList<QCPAxis*> axesList = mAxes.value(QCPAxis::marginSideToAxisType(side));
  if (axesList.size() > 0)
    return axesList.last()->offset() + axesList.last()->calculateMargin();
  else
    return 0;
}

/*! \internal
  
  Event handler for when a mouse button is pressed on the axis rect. If the left mouse button is
  pressed, the range dragging interaction is initialized (the actual range manipulation happens in
  the \ref mouseMoveEvent).

  The mDragging flag is set to true and some anchor points are set that are needed to determine the
  distance the mouse was dragged in the mouse move/release events later.
  
  \see mouseMoveEvent, mouseReleaseEvent
*/
void QCPAxisRect::mousePressEvent(QMouseEvent *event)
{
  mDragStart = event->pos(); // need this even when not LeftButton is pressed, to determine in releaseEvent whether it was a full click (no position change between press and release)
  if (event->buttons() & Qt::LeftButton)
  {
    mDragging = true;
    // initialize antialiasing backup in case we start dragging:
    if (mParentPlot->noAntialiasingOnDrag())
    {
      mAADragBackup = mParentPlot->antialiasedElements();
      mNotAADragBackup = mParentPlot->notAntialiasedElements();
    }
    // Mouse range dragging interaction:
    if (mParentPlot->interactions().testFlag(QCP::iRangeDrag))
    {
      if (mRangeDragHorzAxis)
        mDragStartHorzRange = mRangeDragHorzAxis.data()->range();
      if (mRangeDragVertAxis)
        mDragStartVertRange = mRangeDragVertAxis.data()->range();
    }
  }
}

/*! \internal
  
  Event handler for when the mouse is moved on the axis rect. If range dragging was activated in a
  preceding \ref mousePressEvent, the range is moved accordingly.
  
  \see mousePressEvent, mouseReleaseEvent
*/
void QCPAxisRect::mouseMoveEvent(QMouseEvent *event)
{
  // Mouse range dragging interaction:
  if (mDragging && mParentPlot->interactions().testFlag(QCP::iRangeDrag))
  {
    if (mRangeDrag.testFlag(Qt::Horizontal))
    {
      if (QCPAxis *rangeDragHorzAxis = mRangeDragHorzAxis.data())
      {
        if (rangeDragHorzAxis->mScaleType == QCPAxis::stLinear)
        {
          double diff = rangeDragHorzAxis->pixelToCoord(mDragStart.x()) - rangeDragHorzAxis->pixelToCoord(event->pos().x());
          rangeDragHorzAxis->setRange(mDragStartHorzRange.lower+diff, mDragStartHorzRange.upper+diff);
        } else if (rangeDragHorzAxis->mScaleType == QCPAxis::stLogarithmic)
        {
          double diff = rangeDragHorzAxis->pixelToCoord(mDragStart.x()) / rangeDragHorzAxis->pixelToCoord(event->pos().x());
          rangeDragHorzAxis->setRange(mDragStartHorzRange.lower*diff, mDragStartHorzRange.upper*diff);
        }
      }
    }
    if (mRangeDrag.testFlag(Qt::Vertical))
    {
      if (QCPAxis *rangeDragVertAxis = mRangeDragVertAxis.data())
      {
        if (rangeDragVertAxis->mScaleType == QCPAxis::stLinear)
        {
          double diff = rangeDragVertAxis->pixelToCoord(mDragStart.y()) - rangeDragVertAxis->pixelToCoord(event->pos().y());
          rangeDragVertAxis->setRange(mDragStartVertRange.lower+diff, mDragStartVertRange.upper+diff);
        } else if (rangeDragVertAxis->mScaleType == QCPAxis::stLogarithmic)
        {
          double diff = rangeDragVertAxis->pixelToCoord(mDragStart.y()) / rangeDragVertAxis->pixelToCoord(event->pos().y());
          rangeDragVertAxis->setRange(mDragStartVertRange.lower*diff, mDragStartVertRange.upper*diff);
        }
      }
    }
    if (mRangeDrag != 0) // if either vertical or horizontal drag was enabled, do a replot
    {
      if (mParentPlot->noAntialiasingOnDrag())
        mParentPlot->setNotAntialiasedElements(QCP::aeAll);
      mParentPlot->replot();
    }
  }
}

/* inherits documentation from base class */
void QCPAxisRect::mouseReleaseEvent(QMouseEvent *event)
{
  Q_UNUSED(event)
  mDragging = false;
  if (mParentPlot->noAntialiasingOnDrag())
  {
    mParentPlot->setAntialiasedElements(mAADragBackup);
    mParentPlot->setNotAntialiasedElements(mNotAADragBackup);
  }
}

/*! \internal
  
  Event handler for mouse wheel events. If rangeZoom is Qt::Horizontal, Qt::Vertical or both, the
  ranges of the axes defined as rangeZoomHorzAxis and rangeZoomVertAxis are scaled. The center of
  the scaling operation is the current cursor position inside the axis rect. The scaling factor is
  dependant on the mouse wheel delta (which direction the wheel was rotated) to provide a natural
  zooming feel. The Strength of the zoom can be controlled via \ref setRangeZoomFactor.
  
  Note, that event->delta() is usually +/-120 for single rotation steps. However, if the mouse
  wheel is turned rapidly, many steps may bunch up to one event, so the event->delta() may then be
  multiples of 120. This is taken into account here, by calculating \a wheelSteps and using it as
  exponent of the range zoom factor. This takes care of the wheel direction automatically, by
  inverting the factor, when the wheel step is negative (f^-1 = 1/f).
*/
void QCPAxisRect::wheelEvent(QWheelEvent *event)
{
  // Mouse range zooming interaction:
  if (mParentPlot->interactions().testFlag(QCP::iRangeZoom))
  {
    if (mRangeZoom != 0)
    {
      double factor;
      double wheelSteps = event->delta()/120.0; // a single step delta is +/-120 usually
      if (mRangeZoom.testFlag(Qt::Horizontal))
      {
        factor = pow(mRangeZoomFactorHorz, wheelSteps);
        if (mRangeZoomHorzAxis.data())
          mRangeZoomHorzAxis.data()->scaleRange(factor, mRangeZoomHorzAxis.data()->pixelToCoord(event->pos().x()));
      }
      if (mRangeZoom.testFlag(Qt::Vertical))
      {
        factor = pow(mRangeZoomFactorVert, wheelSteps);
        if (mRangeZoomVertAxis.data())
          mRangeZoomVertAxis.data()->scaleRange(factor, mRangeZoomVertAxis.data()->pixelToCoord(event->pos().y()));
      }
      mParentPlot->replot();
    }
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAbstractLegendItem
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAbstractLegendItem
  \brief The abstract base class for all entries in a QCPLegend.
  
  It defines a very basic interface for entries in a QCPLegend. For representing plottables in the
  legend, the subclass \ref QCPPlottableLegendItem is more suitable.
  
  Only derive directly from this class when you need absolute freedom (e.g. a custom legend entry
  that's not even associated with a plottable).

  You must implement the following pure virtual functions:
  \li \ref draw (from QCPLayerable)
  
  You inherit the following members you may use:
  <table>
    <tr>
      <td>QCPLegend *\b mParentLegend</td>
      <td>A pointer to the parent QCPLegend.</td>
    </tr><tr>
      <td>QFont \b mFont</td>
      <td>The generic font of the item. You should use this font for all or at least the most prominent text of the item.</td>
    </tr>
  </table>
*/

/* start of documentation of signals */

/*! \fn void QCPAbstractLegendItem::selectionChanged(bool selected)
  
  This signal is emitted when the selection state of this legend item has changed, either by user
  interaction or by a direct call to \ref setSelected.
*/

/* end of documentation of signals */

/*!
  Constructs a QCPAbstractLegendItem and associates it with the QCPLegend \a parent. This does not
  cause the item to be added to \a parent, so \ref QCPLegend::addItem must be called separately.
*/
QCPAbstractLegendItem::QCPAbstractLegendItem(QCPLegend *parent) :
  QCPLayoutElement(parent->parentPlot()),
  mParentLegend(parent),
  mFont(parent->font()),
  mTextColor(parent->textColor()),
  mSelectedFont(parent->selectedFont()),
  mSelectedTextColor(parent->selectedTextColor()),
  mSelectable(true),
  mSelected(false)
{
  setLayer("legend");
  setMargins(QMargins(8, 2, 8, 2));
}

/*!
  Sets the default font of this specific legend item to \a font.
  
  \see setTextColor, QCPLegend::setFont
*/
void QCPAbstractLegendItem::setFont(const QFont &font)
{
  mFont = font;
}

/*!
  Sets the default text color of this specific legend item to \a color.
  
  \see setFont, QCPLegend::setTextColor
*/
void QCPAbstractLegendItem::setTextColor(const QColor &color)
{
  mTextColor = color;
}

/*!
  When this legend item is selected, \a font is used to draw generic text, instead of the normal
  font set with \ref setFont.
  
  \see setFont, QCPLegend::setSelectedFont
*/
void QCPAbstractLegendItem::setSelectedFont(const QFont &font)
{
  mSelectedFont = font;
}

/*!
  When this legend item is selected, \a color is used to draw generic text, instead of the normal
  color set with \ref setTextColor.
  
  \see setTextColor, QCPLegend::setSelectedTextColor
*/
void QCPAbstractLegendItem::setSelectedTextColor(const QColor &color)
{
  mSelectedTextColor = color;
}

/*!
  Sets whether this specific legend item is selectable.
  
  \see setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAbstractLegendItem::setSelectable(bool selectable)
{
  if (mSelectable != selectable)
  {
    mSelectable = selectable;
    emit selectableChanged(mSelectable);
  }
}

/*!
  Sets whether this specific legend item is selected.
  
  It is possible to set the selection state of this item by calling this function directly, even if
  setSelectable is set to false.
  
  \see setSelectableParts, QCustomPlot::setInteractions
*/
void QCPAbstractLegendItem::setSelected(bool selected)
{
  if (mSelected != selected)
  {
    mSelected = selected;
    emit selectionChanged(mSelected);
  }
}

/* inherits documentation from base class */
double QCPAbstractLegendItem::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (!mParentPlot) return -1;
  if (onlySelectable && (!mSelectable || !mParentLegend->selectableParts().testFlag(QCPLegend::spItems)))
    return -1;
  
  if (mRect.contains(pos.toPoint()))
    return mParentPlot->selectionTolerance()*0.99;
  else
    return -1;
}

/* inherits documentation from base class */
void QCPAbstractLegendItem::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiased, QCP::aeLegendItems);
}

/* inherits documentation from base class */
QRect QCPAbstractLegendItem::clipRect() const
{
  return mOuterRect;
}

/* inherits documentation from base class */
void QCPAbstractLegendItem::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
  Q_UNUSED(event)
  Q_UNUSED(details)
  if (mSelectable && mParentLegend->selectableParts().testFlag(QCPLegend::spItems))
  {
    bool selBefore = mSelected;
    setSelected(additive ? !mSelected : true);
    if (selectionStateChanged)
      *selectionStateChanged = mSelected != selBefore;
  }
}

/* inherits documentation from base class */
void QCPAbstractLegendItem::deselectEvent(bool *selectionStateChanged)
{
  if (mSelectable && mParentLegend->selectableParts().testFlag(QCPLegend::spItems))
  {
    bool selBefore = mSelected;
    setSelected(false);
    if (selectionStateChanged)
      *selectionStateChanged = mSelected != selBefore;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPlottableLegendItem
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPlottableLegendItem
  \brief A legend item representing a plottable with an icon and the plottable name.
  
  This is the standard legend item for plottables. It displays an icon of the plottable next to the
  plottable name. The icon is drawn by the respective plottable itself (\ref
  QCPAbstractPlottable::drawLegendIcon), and tries to give an intuitive symbol for the plottable.
  For example, the QCPGraph draws a centered horizontal line and/or a single scatter point in the
  middle.
  
  Legend items of this type are always associated with one plottable (retrievable via the
  plottable() function and settable with the constructor). You may change the font of the plottable
  name with \ref setFont. Icon padding and border pen is taken from the parent QCPLegend, see \ref
  QCPLegend::setIconBorderPen and \ref QCPLegend::setIconTextPadding.

  The function \ref QCPAbstractPlottable::addToLegend/\ref QCPAbstractPlottable::removeFromLegend
  creates/removes legend items of this type in the default implementation. However, these functions
  may be reimplemented such that a different kind of legend item (e.g a direct subclass of
  QCPAbstractLegendItem) is used for that plottable.
  
  Since QCPLegend is based on QCPLayoutGrid, a legend item itself is just a subclass of
  QCPLayoutElement. While it could be added to a legend (or any other layout) via the normal layout
  interface, QCPLegend has specialized functions for handling legend items conveniently, see the
  documentation of \ref QCPLegend.
*/

/*!
  Creates a new legend item associated with \a plottable.
  
  Once it's created, it can be added to the legend via \ref QCPLegend::addItem.
  
  A more convenient way of adding/removing a plottable to/from the legend is via the functions \ref
  QCPAbstractPlottable::addToLegend and \ref QCPAbstractPlottable::removeFromLegend.
*/
QCPPlottableLegendItem::QCPPlottableLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable) :
  QCPAbstractLegendItem(parent),
  mPlottable(plottable)
{
}

/*! \internal
  
  Returns the pen that shall be used to draw the icon border, taking into account the selection
  state of this item.
*/
QPen QCPPlottableLegendItem::getIconBorderPen() const
{
  return mSelected ? mParentLegend->selectedIconBorderPen() : mParentLegend->iconBorderPen();
}

/*! \internal
  
  Returns the text color that shall be used to draw text, taking into account the selection state
  of this item.
*/
QColor QCPPlottableLegendItem::getTextColor() const
{
  return mSelected ? mSelectedTextColor : mTextColor;
}

/*! \internal
  
  Returns the font that shall be used to draw text, taking into account the selection state of this
  item.
*/
QFont QCPPlottableLegendItem::getFont() const
{
  return mSelected ? mSelectedFont : mFont;
}

/*! \internal
  
  Draws the item with \a painter. The size and position of the drawn legend item is defined by the
  parent layout (typically a \ref QCPLegend) and the \ref minimumSizeHint and \ref maximumSizeHint
  of this legend item.
*/
void QCPPlottableLegendItem::draw(QCPPainter *painter)
{
  if (!mPlottable) return;
  painter->setFont(getFont());
  painter->setPen(QPen(getTextColor()));
  QSizeF iconSize = mParentLegend->iconSize();
  QRectF textRect = painter->fontMetrics().boundingRect(0, 0, 0, iconSize.height(), Qt::TextDontClip, mPlottable->name());
  QRectF iconRect(mRect.topLeft(), iconSize);
  int textHeight = qMax(textRect.height(), iconSize.height());  // if text has smaller height than icon, center text vertically in icon height, else align tops
  painter->drawText(mRect.x()+iconSize.width()+mParentLegend->iconTextPadding(), mRect.y(), textRect.width(), textHeight, Qt::TextDontClip, mPlottable->name());
  // draw icon:
  painter->save();
  painter->setClipRect(iconRect, Qt::IntersectClip);
  mPlottable->drawLegendIcon(painter, iconRect);
  painter->restore();
  // draw icon border:
  if (getIconBorderPen().style() != Qt::NoPen)
  {
    painter->setPen(getIconBorderPen());
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(iconRect);
  }
}

/*! \internal
  
  Calculates and returns the size of this item. This includes the icon, the text and the padding in
  between.
*/
QSize QCPPlottableLegendItem::minimumSizeHint() const
{
  if (!mPlottable) return QSize();
  QSize result(0, 0);
  QRect textRect;
  QFontMetrics fontMetrics(getFont());
  QSize iconSize = mParentLegend->iconSize();
  textRect = fontMetrics.boundingRect(0, 0, 0, iconSize.height(), Qt::TextDontClip, mPlottable->name());
  result.setWidth(iconSize.width() + mParentLegend->iconTextPadding() + textRect.width() + mMargins.left() + mMargins.right());
  result.setHeight(qMax(textRect.height(), iconSize.height()) + mMargins.top() + mMargins.bottom());
  return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLegend
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLegend
  \brief Manages a legend inside a QCustomPlot.

  A legend is a small box somewhere in the plot which lists plottables with their name and icon.
  
  Normally, the legend is populated by calling \ref QCPAbstractPlottable::addToLegend. The
  respective legend item can be removed with \ref QCPAbstractPlottable::removeFromLegend. However,
  QCPLegend also offers an interface to add and manipulate legend items directly: \ref item, \ref
  itemWithPlottable, \ref itemCount, \ref addItem, \ref removeItem, etc.
  
  The QCPLegend derives from QCPLayoutGrid and as such can be placed in any position a
  QCPLayoutElement may be positioned. The legend items are themselves QCPLayoutElements which are
  placed in the grid layout of the legend. QCPLegend only adds an interface specialized for
  handling child elements of type QCPAbstractLegendItem, as mentioned above. In principle, any
  other layout elements may also be added to a legend via the normal \ref QCPLayoutGrid interface.
  However, the QCPAbstractLegendItem-Interface will ignore those elements (e.g. \ref itemCount will
  only return the number of items with QCPAbstractLegendItems type).

  By default, every QCustomPlot has one legend (QCustomPlot::legend) which is placed in the inset
  layout of the main axis rect (\ref QCPAxisRect::insetLayout). To move the legend to another
  position inside the axis rect, use the methods of the \ref QCPLayoutInset. To move the legend
  outside of the axis rect, place it anywhere else with the QCPLayout/QCPLayoutElement interface.
*/

/* start of documentation of signals */

/*! \fn void QCPLegend::selectionChanged(QCPLegend::SelectableParts selection);

  This signal is emitted when the selection state of this legend has changed.
  
  \see setSelectedParts, setSelectableParts
*/

/* end of documentation of signals */

/*!
  Constructs a new QCPLegend instance with \a parentPlot as the containing plot and default values.
  
  Note that by default, QCustomPlot already contains a legend ready to be used as
  QCustomPlot::legend
*/
QCPLegend::QCPLegend()
{
  setRowSpacing(0);
  setColumnSpacing(10);
  setMargins(QMargins(2, 3, 2, 2));
  setAntialiased(false);
  setIconSize(32, 18);
  
  setIconTextPadding(7);
  
  setSelectableParts(spLegendBox | spItems);
  setSelectedParts(spNone);
  
  setBorderPen(QPen(Qt::black));
  setSelectedBorderPen(QPen(Qt::blue, 2));
  setIconBorderPen(Qt::NoPen);
  setSelectedIconBorderPen(QPen(Qt::blue, 2));
  setBrush(Qt::white);
  setSelectedBrush(Qt::white);
  setTextColor(Qt::black);
  setSelectedTextColor(Qt::blue);
}

QCPLegend::~QCPLegend()
{
  clearItems();
  if (mParentPlot)
    mParentPlot->legendRemoved(this);
}

/* no doc for getter, see setSelectedParts */
QCPLegend::SelectableParts QCPLegend::selectedParts() const
{
  // check whether any legend elements selected, if yes, add spItems to return value
  bool hasSelectedItems = false;
  for (int i=0; i<itemCount(); ++i)
  {
    if (item(i) && item(i)->selected())
    {
      hasSelectedItems = true;
      break;
    }
  }
  if (hasSelectedItems)
    return mSelectedParts | spItems;
  else
    return mSelectedParts & ~spItems;
}

/*!
  Sets the pen, the border of the entire legend is drawn with.
*/
void QCPLegend::setBorderPen(const QPen &pen)
{
  mBorderPen = pen;
}

/*!
  Sets the brush of the legend background.
*/
void QCPLegend::setBrush(const QBrush &brush)
{
  mBrush = brush;
}

/*!
  Sets the default font of legend text. Legend items that draw text (e.g. the name of a graph) will
  use this font by default. However, a different font can be specified on a per-item-basis by
  accessing the specific legend item.
  
  This function will also set \a font on all already existing legend items.
  
  \see QCPAbstractLegendItem::setFont
*/
void QCPLegend::setFont(const QFont &font)
{
  mFont = font;
  for (int i=0; i<itemCount(); ++i)
  {
    if (item(i))
      item(i)->setFont(mFont);
  }
}

/*!
  Sets the default color of legend text. Legend items that draw text (e.g. the name of a graph)
  will use this color by default. However, a different colors can be specified on a per-item-basis
  by accessing the specific legend item.
  
  This function will also set \a color on all already existing legend items.
  
  \see QCPAbstractLegendItem::setTextColor
*/
void QCPLegend::setTextColor(const QColor &color)
{
  mTextColor = color;
  for (int i=0; i<itemCount(); ++i)
  {
    if (item(i))
      item(i)->setTextColor(color);
  }
}

/*!
  Sets the size of legend icons. Legend items that draw an icon (e.g. a visual
  representation of the graph) will use this size by default.
*/
void QCPLegend::setIconSize(const QSize &size)
{
  mIconSize = size;
}

/*! \overload
*/
void QCPLegend::setIconSize(int width, int height)
{
  mIconSize.setWidth(width);
  mIconSize.setHeight(height);
}

/*!
  Sets the horizontal space in pixels between the legend icon and the text next to it.
  Legend items that draw an icon (e.g. a visual representation of the graph) and text (e.g. the
  name of the graph) will use this space by default.
*/
void QCPLegend::setIconTextPadding(int padding)
{
  mIconTextPadding = padding;
}

/*!
  Sets the pen used to draw a border around each legend icon. Legend items that draw an
  icon (e.g. a visual representation of the graph) will use this pen by default.
  
  If no border is wanted, set this to \a Qt::NoPen.
*/
void QCPLegend::setIconBorderPen(const QPen &pen)
{
  mIconBorderPen = pen;
}

/*!
  Sets whether the user can (de-)select the parts in \a selectable by clicking on the QCustomPlot surface.
  (When \ref QCustomPlot::setInteractions contains \ref QCP::iSelectLegend.)
  
  However, even when \a selectable is set to a value not allowing the selection of a specific part,
  it is still possible to set the selection of this part manually, by calling \ref setSelectedParts
  directly.
  
  \see SelectablePart, setSelectedParts
*/
void QCPLegend::setSelectableParts(const SelectableParts &selectable)
{
  if (mSelectableParts != selectable)
  {
    mSelectableParts = selectable;
    emit selectableChanged(mSelectableParts);
  }
}

/*!
  Sets the selected state of the respective legend parts described by \ref SelectablePart. When a part
  is selected, it uses a different pen/font and brush. If some legend items are selected and \a selected
  doesn't contain \ref spItems, those items become deselected.
  
  The entire selection mechanism is handled automatically when \ref QCustomPlot::setInteractions
  contains iSelectLegend. You only need to call this function when you wish to change the selection
  state manually.
  
  This function can change the selection state of a part even when \ref setSelectableParts was set to a
  value that actually excludes the part.
  
  emits the \ref selectionChanged signal when \a selected is different from the previous selection state.
  
  Note that it doesn't make sense to set the selected state \ref spItems here when it wasn't set
  before, because there's no way to specify which exact items to newly select. Do this by calling
  \ref QCPAbstractLegendItem::setSelected directly on the legend item you wish to select.
  
  \see SelectablePart, setSelectableParts, selectTest, setSelectedBorderPen, setSelectedIconBorderPen, setSelectedBrush,
  setSelectedFont
*/
void QCPLegend::setSelectedParts(const SelectableParts &selected)
{
  SelectableParts newSelected = selected;
  mSelectedParts = this->selectedParts(); // update mSelectedParts in case item selection changed

  if (mSelectedParts != newSelected)
  {
    if (!mSelectedParts.testFlag(spItems) && newSelected.testFlag(spItems)) // attempt to set spItems flag (can't do that)
    {
      qDebug() << Q_FUNC_INFO << "spItems flag can not be set, it can only be unset with this function";
      newSelected &= ~spItems;
    }
    if (mSelectedParts.testFlag(spItems) && !newSelected.testFlag(spItems)) // spItems flag was unset, so clear item selection
    {
      for (int i=0; i<itemCount(); ++i)
      {
        if (item(i))
          item(i)->setSelected(false);
      }
    }
    mSelectedParts = newSelected;
    emit selectionChanged(mSelectedParts);
  }
}

/*!
  When the legend box is selected, this pen is used to draw the border instead of the normal pen
  set via \ref setBorderPen.

  \see setSelectedParts, setSelectableParts, setSelectedBrush
*/
void QCPLegend::setSelectedBorderPen(const QPen &pen)
{
  mSelectedBorderPen = pen;
}

/*!
  Sets the pen legend items will use to draw their icon borders, when they are selected.

  \see setSelectedParts, setSelectableParts, setSelectedFont
*/
void QCPLegend::setSelectedIconBorderPen(const QPen &pen)
{
  mSelectedIconBorderPen = pen;
}

/*!
  When the legend box is selected, this brush is used to draw the legend background instead of the normal brush
  set via \ref setBrush.

  \see setSelectedParts, setSelectableParts, setSelectedBorderPen
*/
void QCPLegend::setSelectedBrush(const QBrush &brush)
{
  mSelectedBrush = brush;
}

/*!
  Sets the default font that is used by legend items when they are selected.
  
  This function will also set \a font on all already existing legend items.

  \see setFont, QCPAbstractLegendItem::setSelectedFont
*/
void QCPLegend::setSelectedFont(const QFont &font)
{
  mSelectedFont = font;
  for (int i=0; i<itemCount(); ++i)
  {
    if (item(i))
      item(i)->setSelectedFont(font);
  }
}

/*!
  Sets the default text color that is used by legend items when they are selected.
  
  This function will also set \a color on all already existing legend items.

  \see setTextColor, QCPAbstractLegendItem::setSelectedTextColor
*/
void QCPLegend::setSelectedTextColor(const QColor &color)
{
  mSelectedTextColor = color;
  for (int i=0; i<itemCount(); ++i)
  {
    if (item(i))
      item(i)->setSelectedTextColor(color);
  }
}

/*!
  Returns the item with index \a i.
  
  \see itemCount
*/
QCPAbstractLegendItem *QCPLegend::item(int index) const
{
  return qobject_cast<QCPAbstractLegendItem*>(elementAt(index));
}

/*!
  Returns the QCPPlottableLegendItem which is associated with \a plottable (e.g. a \ref QCPGraph*).
  If such an item isn't in the legend, returns 0.
  
  \see hasItemWithPlottable
*/
QCPPlottableLegendItem *QCPLegend::itemWithPlottable(const QCPAbstractPlottable *plottable) const
{
  for (int i=0; i<itemCount(); ++i)
  {
    if (QCPPlottableLegendItem *pli = qobject_cast<QCPPlottableLegendItem*>(item(i)))
    {
      if (pli->plottable() == plottable)
        return pli;
    }
  }
  return 0;
}

/*!
  Returns the number of items currently in the legend.
  \see item
*/
int QCPLegend::itemCount() const
{
  return elementCount();
}

/*!
  Returns whether the legend contains \a itm.
*/
bool QCPLegend::hasItem(QCPAbstractLegendItem *item) const
{
  for (int i=0; i<itemCount(); ++i)
  {
    if (item == this->item(i))
        return true;
  }
  return false;
}

/*!
  Returns whether the legend contains a QCPPlottableLegendItem which is associated with \a plottable (e.g. a \ref QCPGraph*).
  If such an item isn't in the legend, returns false.
  
  \see itemWithPlottable
*/
bool QCPLegend::hasItemWithPlottable(const QCPAbstractPlottable *plottable) const
{
  return itemWithPlottable(plottable);
}

/*!
  Adds \a item to the legend, if it's not present already.
  
  Returns true on sucess, i.e. if the item wasn't in the list already and has been successfuly added.
  
  The legend takes ownership of the item.
*/
bool QCPLegend::addItem(QCPAbstractLegendItem *item)
{
  if (!hasItem(item))
  {
    return addElement(rowCount(), 0, item);
  } else
    return false;
}

/*!
  Removes the item with index \a index from the legend.

  Returns true, if successful.
  
  \see itemCount, clearItems
*/
bool QCPLegend::removeItem(int index)
{
  if (QCPAbstractLegendItem *ali = item(index))
  {
    bool success = remove(ali);
    simplify();
    return success;
  } else
    return false;
}

/*! \overload
  
  Removes \a item from the legend.

  Returns true, if successful.
  
  \see clearItems
*/
bool QCPLegend::removeItem(QCPAbstractLegendItem *item)
{
  bool success = remove(item);
  simplify();
  return success;
}

/*!
  Removes all items from the legend.
*/
void QCPLegend::clearItems()
{
  for (int i=itemCount()-1; i>=0; --i)
    removeItem(i);
}

/*!
  Returns the legend items that are currently selected. If no items are selected,
  the list is empty.
  
  \see QCPAbstractLegendItem::setSelected, setSelectable
*/
QList<QCPAbstractLegendItem *> QCPLegend::selectedItems() const
{
  QList<QCPAbstractLegendItem*> result;
  for (int i=0; i<itemCount(); ++i)
  {
    if (QCPAbstractLegendItem *ali = item(i))
    {
      if (ali->selected())
        result.append(ali);
    }
  }
  return result;
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing main legend elements.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased
*/
void QCPLegend::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiased, QCP::aeLegend);
}

/*! \internal
  
  Returns the pen used to paint the border of the legend, taking into account the selection state
  of the legend box.
*/
QPen QCPLegend::getBorderPen() const
{
  return mSelectedParts.testFlag(spLegendBox) ? mSelectedBorderPen : mBorderPen;
}

/*! \internal
  
  Returns the brush used to paint the background of the legend, taking into account the selection
  state of the legend box.
*/
QBrush QCPLegend::getBrush() const
{
  return mSelectedParts.testFlag(spLegendBox) ? mSelectedBrush : mBrush;
}

/*! \internal
  
  Draws the legend box with the provided \a painter. The individual legend items are layerables
  themselves, thus are drawn independently.
*/
void QCPLegend::draw(QCPPainter *painter)
{
  // draw background rect:
  painter->setBrush(getBrush());
  painter->setPen(getBorderPen());
  painter->drawRect(mOuterRect);
}

/* inherits documentation from base class */
double QCPLegend::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  if (!mParentPlot) return -1;
  if (onlySelectable && !mSelectableParts.testFlag(spLegendBox))
    return -1;
  
  if (mOuterRect.contains(pos.toPoint()))
  {
    if (details) details->setValue(spLegendBox);
    return mParentPlot->selectionTolerance()*0.99;
  }
  return -1;
}

/* inherits documentation from base class */
void QCPLegend::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
  Q_UNUSED(event)
  mSelectedParts = selectedParts(); // in case item selection has changed
  if (details.value<SelectablePart>() == spLegendBox && mSelectableParts.testFlag(spLegendBox))
  {
    SelectableParts selBefore = mSelectedParts;
    setSelectedParts(additive ? mSelectedParts^spLegendBox : mSelectedParts|spLegendBox); // no need to unset spItems in !additive case, because they will be deselected by QCustomPlot (they're normal QCPLayerables with own deselectEvent)
    if (selectionStateChanged)
      *selectionStateChanged = mSelectedParts != selBefore;
  }
}

/* inherits documentation from base class */
void QCPLegend::deselectEvent(bool *selectionStateChanged)
{
  mSelectedParts = selectedParts(); // in case item selection has changed
  if (mSelectableParts.testFlag(spLegendBox))
  {
    SelectableParts selBefore = mSelectedParts;
    setSelectedParts(selectedParts() & ~spLegendBox);
    if (selectionStateChanged)
      *selectionStateChanged = mSelectedParts != selBefore;
  }
}

/* inherits documentation from base class */
QCP::Interaction QCPLegend::selectionCategory() const
{
  return QCP::iSelectLegend;
}

/* inherits documentation from base class */
QCP::Interaction QCPAbstractLegendItem::selectionCategory() const
{
  return QCP::iSelectLegend;
}

/* inherits documentation from base class */
void QCPLegend::parentPlotInitialized(QCustomPlot *parentPlot)
{
  Q_UNUSED(parentPlot)
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPlotTitle
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPlotTitle
  \brief A layout element displaying a plot title text
  
  The text may be specified with \ref setText, theformatting can be controlled with \ref setFont
  and \ref setTextColor.
  
  A plot title can be added as follows:
  \code
  customPlot->plotLayout()->insertRow(0); // inserts an empty row above the default axis rect
  customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, "Your Plot Title"));
  \endcode
  
  Since a plot title is a common requirement, QCustomPlot offers specialized selection signals for
  easy interaction with QCPPlotTitle. If a layout element of type QCPPlotTitle is clicked, the
  signal \ref QCustomPlot::titleClick is emitted. A double click emits the \ref
  QCustomPlot::titleDoubleClick signal.
*/

/* start documentation of signals */

/*! \fn void QCPPlotTitle::selectionChanged(bool selected)
  
  This signal is emitted when the selection state has changed to \a selected, either by user
  interaction or by a direct call to \ref setSelected.
  
  \see setSelected, setSelectable
*/

/* end documentation of signals */

/*!
  Creates a new QCPPlotTitle instance and sets default values. The initial text is empty (\ref setText).
  
  To set the title text in the constructor, rather use \ref QCPPlotTitle(QCustomPlot *parentPlot, const QString &text).
*/
QCPPlotTitle::QCPPlotTitle(QCustomPlot *parentPlot) :
  QCPLayoutElement(parentPlot),
  mFont(QFont("sans serif", 13*1.5, QFont::Bold)),
  mTextColor(Qt::black),
  mSelectedFont(QFont("sans serif", 13*1.6, QFont::Bold)),
  mSelectedTextColor(Qt::blue),
  mSelectable(false),
  mSelected(false)
{
  if (parentPlot)
  {
    setLayer(parentPlot->currentLayer());
    mFont = QFont(parentPlot->font().family(), parentPlot->font().pointSize()*1.5, QFont::Bold);
    mSelectedFont = QFont(parentPlot->font().family(), parentPlot->font().pointSize()*1.6, QFont::Bold);
  }
  setMargins(QMargins(5, 5, 5, 0));
}

/*! \overload
  
  Creates a new QCPPlotTitle instance and sets default values. The initial text is set to \a text.
*/
QCPPlotTitle::QCPPlotTitle(QCustomPlot *parentPlot, const QString &text) :
  QCPLayoutElement(parentPlot),
  mText(text),
  mFont(QFont(parentPlot->font().family(), parentPlot->font().pointSize()*1.5, QFont::Bold)),
  mTextColor(Qt::black),
  mSelectedFont(QFont(parentPlot->font().family(), parentPlot->font().pointSize()*1.6, QFont::Bold)),
  mSelectedTextColor(Qt::blue),
  mSelectable(false),
  mSelected(false)
{
  setLayer("axes");
  setMargins(QMargins(5, 5, 5, 0));
}

/*!
  Sets the text that will be displayed to \a text. Multiple lines can be created by insertion of "\n".
  
  \see setFont, setTextColor
*/
void QCPPlotTitle::setText(const QString &text)
{
  mText = text;
}

/*!
  Sets the \a font of the title text.
  
  \see setTextColor, setSelectedFont
*/
void QCPPlotTitle::setFont(const QFont &font)
{
  mFont = font;
}

/*!
  Sets the \a color of the title text.
  
  \see setFont, setSelectedTextColor
*/
void QCPPlotTitle::setTextColor(const QColor &color)
{
  mTextColor = color;
}

/*!
  Sets the \a font of the title text that will be used if the plot title is selected (\ref setSelected).
  
  \see setFont
*/
void QCPPlotTitle::setSelectedFont(const QFont &font)
{
  mSelectedFont = font;
}

/*!
  Sets the \a color of the title text that will be used if the plot title is selected (\ref setSelected).
  
  \see setTextColor
*/
void QCPPlotTitle::setSelectedTextColor(const QColor &color)
{
  mSelectedTextColor = color;
}

/*!
  Sets whether the user may select this plot title to \a selectable.

  Note that even when \a selectable is set to <tt>false</tt>, the selection state may be changed
  programmatically via \ref setSelected.
*/
void QCPPlotTitle::setSelectable(bool selectable)
{
  if (mSelectable != selectable)
  {
    mSelectable = selectable;
    emit selectableChanged(mSelectable);
  }
}

/*!
  Sets the selection state of this plot title to \a selected. If the selection has changed, \ref
  selectionChanged is emitted.
  
  Note that this function can change the selection state independently of the current \ref
  setSelectable state.
*/
void QCPPlotTitle::setSelected(bool selected)
{
  if (mSelected != selected)
  {
    mSelected = selected;
    emit selectionChanged(mSelected);
  }
}

/* inherits documentation from base class */
void QCPPlotTitle::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiased, QCP::aeNone);
}

/* inherits documentation from base class */
void QCPPlotTitle::draw(QCPPainter *painter)
{
  painter->setFont(mainFont());
  painter->setPen(QPen(mainTextColor()));
  painter->drawText(mRect, Qt::AlignCenter, mText, &mTextBoundingRect);
}

/* inherits documentation from base class */
QSize QCPPlotTitle::minimumSizeHint() const
{
  QFontMetrics metrics(mFont);
  QSize result = metrics.boundingRect(0, 0, 0, 0, Qt::AlignCenter, mText).size();
  result.rwidth() += mMargins.left() + mMargins.right();
  result.rheight() += mMargins.top() + mMargins.bottom();
  return result;
}

/* inherits documentation from base class */
QSize QCPPlotTitle::maximumSizeHint() const
{
  QFontMetrics metrics(mFont);
  QSize result = metrics.boundingRect(0, 0, 0, 0, Qt::AlignCenter, mText).size();
  result.rheight() += mMargins.top() + mMargins.bottom();
  result.setWidth(QWIDGETSIZE_MAX);
  return result;
}

/* inherits documentation from base class */
void QCPPlotTitle::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
  Q_UNUSED(event)
  Q_UNUSED(details)
  if (mSelectable)
  {
    bool selBefore = mSelected;
    setSelected(additive ? !mSelected : true);
    if (selectionStateChanged)
      *selectionStateChanged = mSelected != selBefore;
  }
}

/* inherits documentation from base class */
void QCPPlotTitle::deselectEvent(bool *selectionStateChanged)
{
  if (mSelectable)
  {
    bool selBefore = mSelected;
    setSelected(false);
    if (selectionStateChanged)
      *selectionStateChanged = mSelected != selBefore;
  }
}

/* inherits documentation from base class */
double QCPPlotTitle::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  
  if (mTextBoundingRect.contains(pos.toPoint()))
    return mParentPlot->selectionTolerance()*0.99;
  else
    return -1;
}

/*! \internal
  
  Returns the main font to be used. This is mSelectedFont if \ref setSelected is set to
  <tt>true</tt>, else mFont is returned.
*/
QFont QCPPlotTitle::mainFont() const
{
  return mSelected ? mSelectedFont : mFont;
}

/*! \internal
  
  Returns the main color to be used. This is mSelectedTextColor if \ref setSelected is set to
  <tt>true</tt>, else mTextColor is returned.
*/
QColor QCPPlotTitle::mainTextColor() const
{
  return mSelected ? mSelectedTextColor : mTextColor;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorScale
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorScale
  \brief A color scale for use with color coding data such as QCPColorMap
  
  This layout element can be placed on the plot to correlate a color gradient with data values. It
  is usually used in combination with one or multiple \ref QCPColorMap "QCPColorMaps".

  \image html QCPColorScale.png
  
  The color scale can be either horizontal or vertical, as shown in the image above. The
  orientation and the side where the numbers appear is controlled with \ref setType.
  
  Use \ref QCPColorMap::setColorScale to connect a color map with a color scale. Once they are
  connected, they share their gradient, data range and data scale type (\ref setGradient, \ref
  setDataRange, \ref setDataScaleType). Multiple color maps may be associated with a single color
  scale, to make them all synchronize these properties.
  
  To have finer control over the number display and axis behaviour, you can directly access the
  \ref axis. See the documentation of QCPAxis for details about configuring axes. For example, if
  you want to change the number of automatically generated ticks, call
  \code
  colorScale->axis()->setAutoTickCount(3);
  \endcode
  
  Placing a color scale next to the main axis rect works like with any other layout element:
  \code
  QCPColorScale *colorScale = new QCPColorScale(customPlot);
  customPlot->plotLayout()->addElement(0, 1, colorScale);
  colorScale->setLabel("Some Label Text");
  \endcode
  In this case we have placed it to the right of the default axis rect, so it wasn't necessary to
  call \ref setType, since \ref QCPAxis::atRight is already the default. The text next to the color
  scale can be set with \ref setLabel.
  
  For optimum appearance (like in the image above), it may be desirable to line up the axis rect and
  the borders of the color scale. Use a \ref QCPMarginGroup to achieve this:
  \code
  QCPMarginGroup *group = new QCPMarginGroup(customPlot);
  colorScale->setMarginGroup(QCP::msTop|QCP::msBottom, group);
  customPlot->axisRect()->setMarginGroup(QCP::msTop|QCP::msBottom, group);
  \endcode
  
  Color scales are initialized with a non-zero minimum top and bottom margin (\ref
  setMinimumMargins), because vertical color scales are most common and the minimum top/bottom
  margin makes sure it keeps some distance to the top/bottom widget border. So if you change to a
  horizontal color scale by setting \ref setType to \ref QCPAxis::atBottom or \ref QCPAxis::atTop, you
  might want to also change the minimum margins accordingly, e.g. \ref
  setMinimumMargins(QMargins(6, 0, 6, 0)).
*/

/* start documentation of inline functions */

/*! \fn QCPAxis *QCPColorScale::axis() const
  
  Returns the internal \ref QCPAxis instance of this color scale. You can access it to alter the
  appearance and behaviour of the axis. \ref QCPColorScale duplicates some properties in its
  interface for convenience. Those are \ref setDataRange (\ref QCPAxis::setRange), \ref
  setDataScaleType (\ref QCPAxis::setScaleType), and the method \ref setLabel (\ref
  QCPAxis::setLabel). As they each are connected, it does not matter whether you use the method on
  the QCPColorScale or on its QCPAxis.
  
  If the type of the color scale is changed with \ref setType, the axis returned by this method
  will change, too, to either the left, right, bottom or top axis, depending on which type was set.
*/

/* end documentation of signals */
/* start documentation of signals */

/*! \fn void QCPColorScale::dataRangeChanged(QCPRange newRange);
  
  This signal is emitted when the data range changes.
  
  \see setDataRange
*/

/*! \fn void QCPColorScale::dataScaleTypeChanged(QCPAxis::ScaleType scaleType);
  
  This signal is emitted when the data scale type changes.
  
  \see setDataScaleType
*/

/*! \fn void QCPColorScale::gradientChanged(QCPColorGradient newGradient);
  
  This signal is emitted when the gradient changes.
  
  \see setGradient
*/

/* end documentation of signals */

/*!
  Constructs a new QCPColorScale.
*/
QCPColorScale::QCPColorScale(QCustomPlot *parentPlot) :
  QCPLayoutElement(parentPlot),
  mType(QCPAxis::atTop), // set to atTop such that setType(QCPAxis::atRight) below doesn't skip work because it thinks it's already atRight
  mDataScaleType(QCPAxis::stLinear),
  mBarWidth(20),
  mAxisRect(new QCPColorScaleAxisRectPrivate(this))
{
  setMinimumMargins(QMargins(0, 6, 0, 6)); // for default right color scale types, keep some room at bottom and top (important if no margin group is used)
  setType(QCPAxis::atRight);
  setDataRange(QCPRange(0, 6));
}

QCPColorScale::~QCPColorScale()
{
  delete mAxisRect;
}

/* undocumented getter */
QString QCPColorScale::label() const
{
  if (!mColorAxis)
  {
    qDebug() << Q_FUNC_INFO << "internal color axis undefined";
    return QString();
  }
  
  return mColorAxis.data()->label();
}

/* undocumented getter */
bool QCPColorScale::rangeDrag() const
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return false;
  }
  
  return mAxisRect.data()->rangeDrag().testFlag(QCPAxis::orientation(mType)) &&
      mAxisRect.data()->rangeDragAxis(QCPAxis::orientation(mType)) &&
      mAxisRect.data()->rangeDragAxis(QCPAxis::orientation(mType))->orientation() == QCPAxis::orientation(mType);
}

/* undocumented getter */
bool QCPColorScale::rangeZoom() const
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return false;
  }
  
  return mAxisRect.data()->rangeZoom().testFlag(QCPAxis::orientation(mType)) &&
      mAxisRect.data()->rangeZoomAxis(QCPAxis::orientation(mType)) &&
      mAxisRect.data()->rangeZoomAxis(QCPAxis::orientation(mType))->orientation() == QCPAxis::orientation(mType);
}

/*!
  Sets at which side of the color scale the axis is placed, and thus also its orientation.
  
  Note that after setting \a type to a different value, the axis returned by \ref axis() will
  be a different one. The new axis will adopt the following properties from the previous axis: The
  range, scale type, log base and label.
*/
void QCPColorScale::setType(QCPAxis::AxisType type)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  if (mType != type)
  {
    mType = type;
    QCPRange rangeTransfer(0, 6);
    double logBaseTransfer = 10;
    QString labelTransfer;
    // revert some settings on old axis:
    if (mColorAxis)
    {
      rangeTransfer = mColorAxis.data()->range();
      labelTransfer = mColorAxis.data()->label();
      logBaseTransfer = mColorAxis.data()->scaleLogBase();
      mColorAxis.data()->setLabel("");
      disconnect(mColorAxis.data(), SIGNAL(rangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
      disconnect(mColorAxis.data(), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
    }
    foreach (QCPAxis::AxisType atype, QList<QCPAxis::AxisType>() << QCPAxis::atLeft << QCPAxis::atRight << QCPAxis::atBottom << QCPAxis::atTop)
    {
      mAxisRect.data()->axis(atype)->setTicks(atype == mType);
      mAxisRect.data()->axis(atype)->setTickLabels(atype== mType);
    }
    // set new mColorAxis pointer:
    mColorAxis = mAxisRect.data()->axis(mType);
    // transfer settings to new axis:
    mColorAxis.data()->setRange(rangeTransfer); // transfer range of old axis to new one (necessary if axis changes from vertical to horizontal or vice versa)
    mColorAxis.data()->setLabel(labelTransfer);
    mColorAxis.data()->setScaleLogBase(logBaseTransfer); // scaleType is synchronized among axes in realtime via signals (connected in QCPColorScale ctor), so we only need to take care of log base here
    connect(mColorAxis.data(), SIGNAL(rangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
    connect(mColorAxis.data(), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
    mAxisRect.data()->setRangeDragAxes(QCPAxis::orientation(mType) == Qt::Horizontal ? mColorAxis.data() : 0,
                                       QCPAxis::orientation(mType) == Qt::Vertical ? mColorAxis.data() : 0);
  }
}

/*!
  Sets the range spanned by the color gradient and that is shown by the axis in the color scale.
  
  It is equivalent to calling QCPColorMap::setDataRange on any of the connected color maps. It is
  also equivalent to directly accessing the \ref axis and setting its range with \ref
  QCPAxis::setRange.
  
  \see setDataScaleType, setGradient, rescaleDataRange
*/
void QCPColorScale::setDataRange(const QCPRange &dataRange)
{
  if (mDataRange.lower != dataRange.lower || mDataRange.upper != dataRange.upper)
  {
    mDataRange = dataRange;
    if (mColorAxis)
      mColorAxis.data()->setRange(mDataRange);
    emit dataRangeChanged(mDataRange);
  }
}

/*!
  Sets the scale type of the color scale, i.e. whether values are linearly associated with colors
  or logarithmically.
  
  It is equivalent to calling QCPColorMap::setDataScaleType on any of the connected color maps. It is
  also equivalent to directly accessing the \ref axis and setting its scale type with \ref
  QCPAxis::setScaleType.
  
  \see setDataRange, setGradient
*/
void QCPColorScale::setDataScaleType(QCPAxis::ScaleType scaleType)
{
  if (mDataScaleType != scaleType)
  {
    mDataScaleType = scaleType;
    if (mColorAxis)
      mColorAxis.data()->setScaleType(mDataScaleType);
    if (mDataScaleType == QCPAxis::stLogarithmic)
      setDataRange(mDataRange.sanitizedForLogScale());
    emit dataScaleTypeChanged(mDataScaleType);
  }
}

/*!
  Sets the color gradient that will be used to represent data values.
  
  It is equivalent to calling QCPColorMap::setGradient on any of the connected color maps.
  
  \see setDataRange, setDataScaleType
*/
void QCPColorScale::setGradient(const QCPColorGradient &gradient)
{
  if (mGradient != gradient)
  {
    mGradient = gradient;
    if (mAxisRect)
      mAxisRect.data()->mGradientImageInvalidated = true;
    emit gradientChanged(mGradient);
  }
}

/*!
  Sets the axis label of the color scale. This is equivalent to calling \ref QCPAxis::setLabel on
  the internal \ref axis.
*/
void QCPColorScale::setLabel(const QString &str)
{
  if (!mColorAxis)
  {
    qDebug() << Q_FUNC_INFO << "internal color axis undefined";
    return;
  }
  
  mColorAxis.data()->setLabel(str);
}

/*!
  Sets the width (or height, for horizontal color scales) the bar where the gradient is displayed
  will have.
*/
void QCPColorScale::setBarWidth(int width)
{
  mBarWidth = width;
}

/*!
  Sets whether the user can drag the data range (\ref setDataRange).
  
  Note that \ref QCP::iRangeDrag must be in the QCustomPlot's interactions (\ref
  QCustomPlot::setInteractions) to allow range dragging.
*/
void QCPColorScale::setRangeDrag(bool enabled)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  
  if (enabled)
    mAxisRect.data()->setRangeDrag(QCPAxis::orientation(mType));
  else
    mAxisRect.data()->setRangeDrag(0);
}

/*!
  Sets whether the user can zoom the data range (\ref setDataRange) by scrolling the mouse wheel.
  
  Note that \ref QCP::iRangeZoom must be in the QCustomPlot's interactions (\ref
  QCustomPlot::setInteractions) to allow range dragging.
*/
void QCPColorScale::setRangeZoom(bool enabled)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  
  if (enabled)
    mAxisRect.data()->setRangeZoom(QCPAxis::orientation(mType));
  else
    mAxisRect.data()->setRangeZoom(0);
}

/*!
  Returns a list of all the color maps associated with this color scale.
*/
QList<QCPColorMap*> QCPColorScale::colorMaps() const
{
  QList<QCPColorMap*> result;
  for (int i=0; i<mParentPlot->plottableCount(); ++i)
  {
    if (QCPColorMap *cm = qobject_cast<QCPColorMap*>(mParentPlot->plottable(i)))
      if (cm->colorScale() == this)
        result.append(cm);
  }
  return result;
}

/*!
  Changes the data range such that all color maps associated with this color scale are fully mapped
  to the gradient in the data dimension.
  
  \see setDataRange
*/
void QCPColorScale::rescaleDataRange(bool onlyVisibleMaps)
{
  QList<QCPColorMap*> maps = colorMaps();
  QCPRange newRange;
  bool haveRange = false;
  int sign = 0; // TODO: should change this to QCPAbstractPlottable::SignDomain later (currently is protected, maybe move to QCP namespace)
  if (mDataScaleType == QCPAxis::stLogarithmic)
    sign = (mDataRange.upper < 0 ? -1 : 1);
  for (int i=0; i<maps.size(); ++i)
  {
    if (!maps.at(i)->realVisibility() && onlyVisibleMaps)
      continue;
    QCPRange mapRange;
    if (maps.at(i)->colorScale() == this)
    {
      bool currentFoundRange = true;
      mapRange = maps.at(i)->data()->dataBounds();
      if (sign == 1)
      {
        if (mapRange.lower <= 0 && mapRange.upper > 0)
          mapRange.lower = mapRange.upper*1e-3;
        else if (mapRange.lower <= 0 && mapRange.upper <= 0)
          currentFoundRange = false;
      } else if (sign == -1)
      {
        if (mapRange.upper >= 0 && mapRange.lower < 0)
          mapRange.upper = mapRange.lower*1e-3;
        else if (mapRange.upper >= 0 && mapRange.lower >= 0)
          currentFoundRange = false;
      }
      if (currentFoundRange)
      {
        if (!haveRange)
          newRange = mapRange;
        else
          newRange.expand(mapRange);
        haveRange = true;
      }
    }
  }
  if (haveRange)
  {
    if (!QCPRange::validRange(newRange)) // likely due to range being zero (plottable has only constant data in this dimension), shift current range to at least center the data
    {
      double center = (newRange.lower+newRange.upper)*0.5; // upper and lower should be equal anyway, but just to make sure, incase validRange returned false for other reason
      if (mDataScaleType == QCPAxis::stLinear)
      {
        newRange.lower = center-mDataRange.size()/2.0;
        newRange.upper = center+mDataRange.size()/2.0;
      } else // mScaleType == stLogarithmic
      {
        newRange.lower = center/qSqrt(mDataRange.upper/mDataRange.lower);
        newRange.upper = center*qSqrt(mDataRange.upper/mDataRange.lower);
      }
    }
    setDataRange(newRange);
  }
}

/* inherits documentation from base class */
void QCPColorScale::update(UpdatePhase phase)
{
  QCPLayoutElement::update(phase);
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  
  mAxisRect.data()->update(phase);
  
  switch (phase)
  {
    case upMargins:
    {
      if (mType == QCPAxis::atBottom || mType == QCPAxis::atTop)
      {
        setMaximumSize(QWIDGETSIZE_MAX, mBarWidth+mAxisRect.data()->margins().top()+mAxisRect.data()->margins().bottom()+margins().top()+margins().bottom());
        setMinimumSize(0,               mBarWidth+mAxisRect.data()->margins().top()+mAxisRect.data()->margins().bottom()+margins().top()+margins().bottom());
      } else
      {
        setMaximumSize(mBarWidth+mAxisRect.data()->margins().left()+mAxisRect.data()->margins().right()+margins().left()+margins().right(), QWIDGETSIZE_MAX);
        setMinimumSize(mBarWidth+mAxisRect.data()->margins().left()+mAxisRect.data()->margins().right()+margins().left()+margins().right(), 0);
      }
      break;
    }
    case upLayout:
    {
      mAxisRect.data()->setOuterRect(rect());
      break;
    }
    default: break;
  }
}

/* inherits documentation from base class */
void QCPColorScale::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  painter->setAntialiasing(false);
}

/* inherits documentation from base class */
void QCPColorScale::mousePressEvent(QMouseEvent *event)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  mAxisRect.data()->mousePressEvent(event);
}

/* inherits documentation from base class */
void QCPColorScale::mouseMoveEvent(QMouseEvent *event)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  mAxisRect.data()->mouseMoveEvent(event);
}

/* inherits documentation from base class */
void QCPColorScale::mouseReleaseEvent(QMouseEvent *event)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  mAxisRect.data()->mouseReleaseEvent(event);
}

/* inherits documentation from base class */
void QCPColorScale::wheelEvent(QWheelEvent *event)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  mAxisRect.data()->wheelEvent(event);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorScaleAxisRectPrivate
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorScaleAxisRectPrivate

  \internal
  \brief An axis rect subclass for use in a QCPColorScale
  
  This is a private class and not part of the public QCustomPlot interface.
  
  It provides the axis rect functionality for the QCPColorScale class.
*/


/*!
  Creates a new instance, as a child of \a parentColorScale.
*/
QCPColorScaleAxisRectPrivate::QCPColorScaleAxisRectPrivate(QCPColorScale *parentColorScale) :
  QCPAxisRect(parentColorScale->parentPlot(), true),
  mParentColorScale(parentColorScale),
  mGradientImageInvalidated(true)
{
  setParentLayerable(parentColorScale);
  setMinimumMargins(QMargins(0, 0, 0, 0));
  foreach (QCPAxis::AxisType type, QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop << QCPAxis::atLeft << QCPAxis::atRight)
  {
    axis(type)->setVisible(true);
    axis(type)->grid()->setVisible(false);
    axis(type)->setPadding(0);
    connect(axis(type), SIGNAL(selectionChanged(QCPAxis::SelectableParts)), this, SLOT(axisSelectionChanged(QCPAxis::SelectableParts)));
    connect(axis(type), SIGNAL(selectableChanged(QCPAxis::SelectableParts)), this, SLOT(axisSelectableChanged(QCPAxis::SelectableParts)));
  }

  connect(axis(QCPAxis::atLeft), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atRight), SLOT(setRange(QCPRange)));
  connect(axis(QCPAxis::atRight), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atLeft), SLOT(setRange(QCPRange)));
  connect(axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atTop), SLOT(setRange(QCPRange)));
  connect(axis(QCPAxis::atTop), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
  connect(axis(QCPAxis::atLeft), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), axis(QCPAxis::atRight), SLOT(setScaleType(QCPAxis::ScaleType)));
  connect(axis(QCPAxis::atRight), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), axis(QCPAxis::atLeft), SLOT(setScaleType(QCPAxis::ScaleType)));
  connect(axis(QCPAxis::atBottom), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), axis(QCPAxis::atTop), SLOT(setScaleType(QCPAxis::ScaleType)));
  connect(axis(QCPAxis::atTop), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), axis(QCPAxis::atBottom), SLOT(setScaleType(QCPAxis::ScaleType)));
  
  // make layer transfers of color scale transfer to axis rect and axes
  // the axes must be set after axis rect, such that they appear above color gradient drawn by axis rect:
  connect(parentColorScale, SIGNAL(layerChanged(QCPLayer*)), this, SLOT(setLayer(QCPLayer*)));
  foreach (QCPAxis::AxisType type, QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop << QCPAxis::atLeft << QCPAxis::atRight)
    connect(parentColorScale, SIGNAL(layerChanged(QCPLayer*)), axis(type), SLOT(setLayer(QCPLayer*)));
}

/*! \internal
  Updates the color gradient image if necessary, by calling \ref updateGradientImage, then draws
  it. Then the axes are drawn by calling the \ref QCPAxisRect::draw base class implementation.
*/
void QCPColorScaleAxisRectPrivate::draw(QCPPainter *painter)
{
  if (mGradientImageInvalidated)
    updateGradientImage();
  
  bool mirrorHorz = false;
  bool mirrorVert = false;
  if (mParentColorScale->mColorAxis)
  {
    mirrorHorz = mParentColorScale->mColorAxis.data()->rangeReversed() && (mParentColorScale->type() == QCPAxis::atBottom || mParentColorScale->type() == QCPAxis::atTop);
    mirrorVert = mParentColorScale->mColorAxis.data()->rangeReversed() && (mParentColorScale->type() == QCPAxis::atLeft || mParentColorScale->type() == QCPAxis::atRight);
  }
  
  painter->drawImage(rect(), mGradientImage.mirrored(mirrorHorz, mirrorVert));
  QCPAxisRect::draw(painter);
}

/*! \internal

  Uses the current gradient of the parent \ref QCPColorScale (specified in the constructor) to
  generate a gradient image. This gradient image will be used in the \ref draw method.
*/
void QCPColorScaleAxisRectPrivate::updateGradientImage()
{
  if (rect().isEmpty())
    return;
  
  int n = mParentColorScale->mGradient.levelCount();
  int w, h;
  QVector<double> data(n);
  for (int i=0; i<n; ++i)
    data[i] = i;
  if (mParentColorScale->mType == QCPAxis::atBottom || mParentColorScale->mType == QCPAxis::atTop)
  {
    w = n;
    h = rect().height();
    mGradientImage = QImage(w, h, QImage::Format_RGB32);
    QVector<QRgb*> pixels;
    for (int y=0; y<h; ++y)
      pixels.append(reinterpret_cast<QRgb*>(mGradientImage.scanLine(y)));
    mParentColorScale->mGradient.colorize(data.constData(), QCPRange(0, n-1), pixels.first(), n);
    for (int y=1; y<h; ++y)
      memcpy(pixels.at(y), pixels.first(), n*sizeof(QRgb));
  } else
  {
    w = rect().width();
    h = n;
    mGradientImage = QImage(w, h, QImage::Format_RGB32);
    for (int y=0; y<h; ++y)
    {
      QRgb *pixels = reinterpret_cast<QRgb*>(mGradientImage.scanLine(y));
      const QRgb lineColor = mParentColorScale->mGradient.color(data[h-1-y], QCPRange(0, n-1));
      for (int x=0; x<w; ++x)
        pixels[x] = lineColor;
    }
  }
  mGradientImageInvalidated = false;
}

/*! \internal

  This slot is connected to the selectionChanged signals of the four axes in the constructor. It
  synchronizes the selection state of the axes.
*/
void QCPColorScaleAxisRectPrivate::axisSelectionChanged(QCPAxis::SelectableParts selectedParts)
{
  // axis bases of four axes shall always (de-)selected synchronously:
  foreach (QCPAxis::AxisType type, QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop << QCPAxis::atLeft << QCPAxis::atRight)
  {
    if (QCPAxis *senderAxis = qobject_cast<QCPAxis*>(sender()))
      if (senderAxis->axisType() == type)
        continue;
    
    if (axis(type)->selectableParts().testFlag(QCPAxis::spAxis))
    {
      if (selectedParts.testFlag(QCPAxis::spAxis))
        axis(type)->setSelectedParts(axis(type)->selectedParts() | QCPAxis::spAxis);
      else
        axis(type)->setSelectedParts(axis(type)->selectedParts() & ~QCPAxis::spAxis);
    }
  }
}

/*! \internal

  This slot is connected to the selectableChanged signals of the four axes in the constructor. It
  synchronizes the selectability of the axes.
*/
void QCPColorScaleAxisRectPrivate::axisSelectableChanged(QCPAxis::SelectableParts selectableParts)
{
  // synchronize axis base selectability:
  foreach (QCPAxis::AxisType type, QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop << QCPAxis::atLeft << QCPAxis::atRight)
  {
    if (QCPAxis *senderAxis = qobject_cast<QCPAxis*>(sender()))
      if (senderAxis->axisType() == type)
        continue;
    
    if (axis(type)->selectableParts().testFlag(QCPAxis::spAxis))
    {
      if (selectableParts.testFlag(QCPAxis::spAxis))
        axis(type)->setSelectableParts(axis(type)->selectableParts() | QCPAxis::spAxis);
      else
        axis(type)->setSelectableParts(axis(type)->selectableParts() & ~QCPAxis::spAxis);
    }
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPData
  \brief Holds the data of one single data point for QCPGraph.
  
  The container for storing multiple data points is \ref QCPDataMap.
  
  The stored data is:
  \li \a key: coordinate on the key axis of this data point
  \li \a value: coordinate on the value axis of this data point
  \li \a keyErrorMinus: negative error in the key dimension (for error bars)
  \li \a keyErrorPlus: positive error in the key dimension (for error bars)
  \li \a valueErrorMinus: negative error in the value dimension (for error bars)
  \li \a valueErrorPlus: positive error in the value dimension (for error bars)
  
  \see QCPDataMap
*/

/*!
  Constructs a data point with key, value and all errors set to zero.
*/
QCPData::QCPData() :
  key(0),
  value(0),
  keyErrorPlus(0),
  keyErrorMinus(0),
  valueErrorPlus(0),
  valueErrorMinus(0)
{
}

/*!
  Constructs a data point with the specified \a key and \a value. All errors are set to zero.
*/
QCPData::QCPData(double key, double value) :
  key(key),
  value(value),
  keyErrorPlus(0),
  keyErrorMinus(0),
  valueErrorPlus(0),
  valueErrorMinus(0)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPGraph
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPGraph
  \brief A plottable representing a graph in a plot.

  \image html QCPGraph.png
  
  Usually QCustomPlot creates graphs internally via QCustomPlot::addGraph and the resulting
  instance is accessed via QCustomPlot::graph.

  To plot data, assign it with the \ref setData or \ref addData functions. Alternatively, you can
  also access and modify the graph's data via the \ref data method, which returns a pointer to the
  internal \ref QCPDataMap.
  
  Graphs are used to display single-valued data. Single-valued means that there should only be one
  data point per unique key coordinate. In other words, the graph can't have \a loops. If you do
  want to plot non-single-valued curves, rather use the QCPCurve plottable.
  
  \section appearance Changing the appearance
  
  The appearance of the graph is mainly determined by the line style, scatter style, brush and pen
  of the graph (\ref setLineStyle, \ref setScatterStyle, \ref setBrush, \ref setPen).
  
  \subsection filling Filling under or between graphs
  
  QCPGraph knows two types of fills: Normal graph fills towards the zero-value-line parallel to
  the key axis of the graph, and fills between two graphs, called channel fills. To enable a fill,
  just set a brush with \ref setBrush which is neither Qt::NoBrush nor fully transparent.
  
  By default, a normal fill towards the zero-value-line will be drawn. To set up a channel fill
  between this graph and another one, call \ref setChannelFillGraph with the other graph as
  parameter.

  \see QCustomPlot::addGraph, QCustomPlot::graph, QCPLegend::addGraph
*/

/* start of documentation of inline functions */

/*! \fn QCPDataMap *QCPGraph::data() const
  
  Returns a pointer to the internal data storage of type \ref QCPDataMap. You may use it to
  directly manipulate the data, which may be more convenient and faster than using the regular \ref
  setData or \ref addData methods, in certain situations.
*/

/* end of documentation of inline functions */

/*!
  Constructs a graph which uses \a keyAxis as its key axis ("x") and \a valueAxis as its value
  axis ("y"). \a keyAxis and \a valueAxis must reside in the same QCustomPlot instance and not have
  the same orientation. If either of these restrictions is violated, a corresponding message is
  printed to the debug output (qDebug), the construction is not aborted, though.
  
  The constructed QCPGraph can be added to the plot with QCustomPlot::addPlottable, QCustomPlot
  then takes ownership of the graph.
  
  To directly create a graph inside a plot, you can also use the simpler QCustomPlot::addGraph function.
*/
QCPGraph::QCPGraph(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis)
{
  mData = new QCPDataMap;
  
  setPen(QPen(Qt::blue, 0));
  setErrorPen(QPen(Qt::black));
  setBrush(Qt::NoBrush);
  setSelectedPen(QPen(QColor(80, 80, 255), 2.5));
  setSelectedBrush(Qt::NoBrush);
  
  setLineStyle(lsLine);
  setErrorType(etNone);
  setErrorBarSize(6);
  setErrorBarSkipSymbol(true);
  setChannelFillGraph(0);
  setAdaptiveSampling(true);
}

QCPGraph::~QCPGraph()
{
  delete mData;
}

/*!
  Replaces the current data with the provided \a data.
  
  If \a copy is set to true, data points in \a data will only be copied. if false, the graph
  takes ownership of the passed data and replaces the internal data pointer with it. This is
  significantly faster than copying for large datasets.
  
  Alternatively, you can also access and modify the graph's data via the \ref data method, which
  returns a pointer to the internal \ref QCPDataMap.
*/
void QCPGraph::setData(QCPDataMap *data, bool copy)
{
  if (copy)
  {
    *mData = *data;
  } else
  {
    delete mData;
    mData = data;
  }
}

/*! \overload
  
  Replaces the current data with the provided points in \a key and \a value pairs. The provided
  vectors should have equal length. Else, the number of added points will be the size of the
  smallest vector.
*/
void QCPGraph::setData(const QVector<double> &key, const QVector<double> &value)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, value.size());
  QCPData newData;
  for (int i=0; i<n; ++i)
  {
    newData.key = key[i];
    newData.value = value[i];
    mData->insertMulti(newData.key, newData);
  }
}

/*!
  Replaces the current data with the provided points in \a key and \a value pairs. Additionally the
  symmetrical value error of the data points are set to the values in \a valueError.
  For error bars to show appropriately, see \ref setErrorType.
  The provided vectors should have equal length. Else, the number of added points will be the size of the
  smallest vector.
  
  For asymmetrical errors (plus different from minus), see the overloaded version of this function.
*/
void QCPGraph::setDataValueError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &valueError)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, value.size());
  n = qMin(n, valueError.size());
  QCPData newData;
  for (int i=0; i<n; ++i)
  {
    newData.key = key[i];
    newData.value = value[i];
    newData.valueErrorMinus = valueError[i];
    newData.valueErrorPlus = valueError[i];
    mData->insertMulti(key[i], newData);
  }
}

/*!
  \overload
  Replaces the current data with the provided points in \a key and \a value pairs. Additionally the
  negative value error of the data points are set to the values in \a valueErrorMinus, the positive
  value error to \a valueErrorPlus.
  For error bars to show appropriately, see \ref setErrorType.
  The provided vectors should have equal length. Else, the number of added points will be the size of the
  smallest vector.
*/
void QCPGraph::setDataValueError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &valueErrorMinus, const QVector<double> &valueErrorPlus)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, value.size());
  n = qMin(n, valueErrorMinus.size());
  n = qMin(n, valueErrorPlus.size());
  QCPData newData;
  for (int i=0; i<n; ++i)
  {
    newData.key = key[i];
    newData.value = value[i];
    newData.valueErrorMinus = valueErrorMinus[i];
    newData.valueErrorPlus = valueErrorPlus[i];
    mData->insertMulti(key[i], newData);
  }
}

/*!
  Replaces the current data with the provided points in \a key and \a value pairs. Additionally the
  symmetrical key error of the data points are set to the values in \a keyError.
  For error bars to show appropriately, see \ref setErrorType.
  The provided vectors should have equal length. Else, the number of added points will be the size of the
  smallest vector.
  
  For asymmetrical errors (plus different from minus), see the overloaded version of this function.
*/
void QCPGraph::setDataKeyError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyError)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, value.size());
  n = qMin(n, keyError.size());
  QCPData newData;
  for (int i=0; i<n; ++i)
  {
    newData.key = key[i];
    newData.value = value[i];
    newData.keyErrorMinus = keyError[i];
    newData.keyErrorPlus = keyError[i];
    mData->insertMulti(key[i], newData);
  }
}

/*!
  \overload
  Replaces the current data with the provided points in \a key and \a value pairs. Additionally the
  negative key error of the data points are set to the values in \a keyErrorMinus, the positive
  key error to \a keyErrorPlus.
  For error bars to show appropriately, see \ref setErrorType.
  The provided vectors should have equal length. Else, the number of added points will be the size of the
  smallest vector.
*/
void QCPGraph::setDataKeyError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyErrorMinus, const QVector<double> &keyErrorPlus)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, value.size());
  n = qMin(n, keyErrorMinus.size());
  n = qMin(n, keyErrorPlus.size());
  QCPData newData;
  for (int i=0; i<n; ++i)
  {
    newData.key = key[i];
    newData.value = value[i];
    newData.keyErrorMinus = keyErrorMinus[i];
    newData.keyErrorPlus = keyErrorPlus[i];
    mData->insertMulti(key[i], newData);
  }
}

/*!
  Replaces the current data with the provided points in \a key and \a value pairs. Additionally the
  symmetrical key and value errors of the data points are set to the values in \a keyError and \a valueError.
  For error bars to show appropriately, see \ref setErrorType.
  The provided vectors should have equal length. Else, the number of added points will be the size of the
  smallest vector.
  
  For asymmetrical errors (plus different from minus), see the overloaded version of this function.
*/
void QCPGraph::setDataBothError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyError, const QVector<double> &valueError)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, value.size());
  n = qMin(n, valueError.size());
  n = qMin(n, keyError.size());
  QCPData newData;
  for (int i=0; i<n; ++i)
  {
    newData.key = key[i];
    newData.value = value[i];
    newData.keyErrorMinus = keyError[i];
    newData.keyErrorPlus = keyError[i];
    newData.valueErrorMinus = valueError[i];
    newData.valueErrorPlus = valueError[i];
    mData->insertMulti(key[i], newData);
  }
}

/*!
  \overload
  Replaces the current data with the provided points in \a key and \a value pairs. Additionally the
  negative key and value errors of the data points are set to the values in \a keyErrorMinus and \a valueErrorMinus. The positive
  key and value errors are set to the values in \a keyErrorPlus \a valueErrorPlus.
  For error bars to show appropriately, see \ref setErrorType.
  The provided vectors should have equal length. Else, the number of added points will be the size of the
  smallest vector.
*/
void QCPGraph::setDataBothError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyErrorMinus, const QVector<double> &keyErrorPlus, const QVector<double> &valueErrorMinus, const QVector<double> &valueErrorPlus)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, value.size());
  n = qMin(n, valueErrorMinus.size());
  n = qMin(n, valueErrorPlus.size());
  n = qMin(n, keyErrorMinus.size());
  n = qMin(n, keyErrorPlus.size());
  QCPData newData;
  for (int i=0; i<n; ++i)
  {
    newData.key = key[i];
    newData.value = value[i];
    newData.keyErrorMinus = keyErrorMinus[i];
    newData.keyErrorPlus = keyErrorPlus[i];
    newData.valueErrorMinus = valueErrorMinus[i];
    newData.valueErrorPlus = valueErrorPlus[i];
    mData->insertMulti(key[i], newData);
  }
}


/*!
  Sets how the single data points are connected in the plot. For scatter-only plots, set \a ls to
  \ref lsNone and \ref setScatterStyle to the desired scatter style.
  
  \see setScatterStyle
*/
void QCPGraph::setLineStyle(LineStyle ls)
{
  mLineStyle = ls;
}

/*!
  Sets the visual appearance of single data points in the plot. If set to \ref QCPScatterStyle::ssNone, no scatter points
  are drawn (e.g. for line-only-plots with appropriate line style).
  
  \see QCPScatterStyle, setLineStyle
*/
void QCPGraph::setScatterStyle(const QCPScatterStyle &style)
{
  mScatterStyle = style;
}

/*!
  Sets which kind of error bars (Key Error, Value Error or both) should be drawn on each data
  point. If you set \a errorType to something other than \ref etNone, make sure to actually pass
  error data via the specific setData functions along with the data points (e.g. \ref
  setDataValueError, \ref setDataKeyError, \ref setDataBothError).

  \see ErrorType
*/
void QCPGraph::setErrorType(ErrorType errorType)
{
  mErrorType = errorType;
}

/*!
  Sets the pen with which the error bars will be drawn.
  \see setErrorBarSize, setErrorType
*/
void QCPGraph::setErrorPen(const QPen &pen)
{
  mErrorPen = pen;
}

/*!
  Sets the width of the handles at both ends of an error bar in pixels.
*/
void QCPGraph::setErrorBarSize(double size)
{
  mErrorBarSize = size;
}

/*!
  If \a enabled is set to true, the error bar will not be drawn as a solid line under the scatter symbol but
  leave some free space around the symbol.
  
  This feature uses the current scatter size (\ref QCPScatterStyle::setSize) to determine the size
  of the area to leave blank. So when drawing Pixmaps as scatter points (\ref
  QCPScatterStyle::ssPixmap), the scatter size must be set manually to a value corresponding to the
  size of the Pixmap, if the error bars should leave gaps to its boundaries.
  
  \ref setErrorType, setErrorBarSize, setScatterStyle
*/
void QCPGraph::setErrorBarSkipSymbol(bool enabled)
{
  mErrorBarSkipSymbol = enabled;
}

/*!
  Sets the target graph for filling the area between this graph and \a targetGraph with the current
  brush (\ref setBrush).
  
  When \a targetGraph is set to 0, a normal graph fill to the zero-value-line will be shown. To
  disable any filling, set the brush to Qt::NoBrush.

  \see setBrush
*/
void QCPGraph::setChannelFillGraph(QCPGraph *targetGraph)
{
  // prevent setting channel target to this graph itself:
  if (targetGraph == this)
  {
    qDebug() << Q_FUNC_INFO << "targetGraph is this graph itself";
    mChannelFillGraph = 0;
    return;
  }
  // prevent setting channel target to a graph not in the plot:
  if (targetGraph && targetGraph->mParentPlot != mParentPlot)
  {
    qDebug() << Q_FUNC_INFO << "targetGraph not in same plot";
    mChannelFillGraph = 0;
    return;
  }
  
  mChannelFillGraph = targetGraph;
}

/*!
  Sets whether adaptive sampling shall be used when plotting this graph. QCustomPlot's adaptive
  sampling technique can drastically improve the replot performance for graphs with a larger number
  of points (e.g. above 10,000), without notably changing the appearance of the graph.
  
  By default, adaptive sampling is enabled. Even if enabled, QCustomPlot decides whether adaptive
  sampling shall actually be used on a per-graph basis. So leaving adaptive sampling enabled has no
  disadvantage in almost all cases.
  
  \image html adaptive-sampling-line.png "A line plot of 500,000 points without and with adaptive sampling"
  
  As can be seen, line plots experience no visual degradation from adaptive sampling. Outliers are
  reproduced reliably, as well as the overall shape of the data set. The replot time reduces
  dramatically though. This allows QCustomPlot to display large amounts of data in realtime.
  
  \image html adaptive-sampling-scatter.png "A scatter plot of 100,000 points without and with adaptive sampling"
  
  Care must be taken when using high-density scatter plots in combination with adaptive sampling.
  The adaptive sampling algorithm treats scatter plots more carefully than line plots which still
  gives a significant reduction of replot times, but not quite as much as for line plots. This is
  because scatter plots inherently need more data points to be preserved in order to still resemble
  the original, non-adaptive-sampling plot. As shown above, the results still aren't quite
  identical, as banding occurs for the outer data points. This is in fact intentional, such that
  the boundaries of the data cloud stay visible to the viewer. How strong the banding appears,
  depends on the point density, i.e. the number of points in the plot.
  
  For some situations with scatter plots it might thus be desirable to manually turn adaptive
  sampling off. For example, when saving the plot to disk. This can be achieved by setting \a
  enabled to false before issuing a command like \ref QCustomPlot::savePng, and setting \a enabled
  back to true afterwards.
*/
void QCPGraph::setAdaptiveSampling(bool enabled)
{
  mAdaptiveSampling = enabled;
}

/*!
  Adds the provided data points in \a dataMap to the current data.
  
  Alternatively, you can also access and modify the graph's data via the \ref data method, which
  returns a pointer to the internal \ref QCPDataMap.
  
  \see removeData
*/
void QCPGraph::addData(const QCPDataMap &dataMap)
{
  mData->unite(dataMap);
}

/*! \overload
  Adds the provided single data point in \a data to the current data.
  
  Alternatively, you can also access and modify the graph's data via the \ref data method, which
  returns a pointer to the internal \ref QCPDataMap.
  
  \see removeData
*/
void QCPGraph::addData(const QCPData &data)
{
  mData->insertMulti(data.key, data);
}

/*! \overload
  Adds the provided single data point as \a key and \a value pair to the current data.
  
  Alternatively, you can also access and modify the graph's data via the \ref data method, which
  returns a pointer to the internal \ref QCPDataMap.
  
  \see removeData
*/
void QCPGraph::addData(double key, double value)
{
  QCPData newData;
  newData.key = key;
  newData.value = value;
  mData->insertMulti(newData.key, newData);
}

/*! \overload
  Adds the provided data points as \a key and \a value pairs to the current data.
  
  Alternatively, you can also access and modify the graph's data via the \ref data method, which
  returns a pointer to the internal \ref QCPDataMap.
  
  \see removeData
*/
void QCPGraph::addData(const QVector<double> &keys, const QVector<double> &values)
{
  int n = qMin(keys.size(), values.size());
  QCPData newData;
  for (int i=0; i<n; ++i)
  {
    newData.key = keys[i];
    newData.value = values[i];
    mData->insertMulti(newData.key, newData);
  }
}

/*!
  Removes all data points with keys smaller than \a key.
  \see addData, clearData
*/
void QCPGraph::removeDataBefore(double key)
{
  QCPDataMap::iterator it = mData->begin();
  while (it != mData->end() && it.key() < key)
    it = mData->erase(it);
}

/*!
  Removes all data points with keys greater than \a key.
  \see addData, clearData
*/
void QCPGraph::removeDataAfter(double key)
{
  if (mData->isEmpty()) return;
  QCPDataMap::iterator it = mData->upperBound(key);
  while (it != mData->end())
    it = mData->erase(it);
}

/*!
  Removes all data points with keys between \a fromKey and \a toKey.
  if \a fromKey is greater or equal to \a toKey, the function does nothing. To remove
  a single data point with known key, use \ref removeData(double key).
  
  \see addData, clearData
*/
void QCPGraph::removeData(double fromKey, double toKey)
{
  if (fromKey >= toKey || mData->isEmpty()) return;
  QCPDataMap::iterator it = mData->upperBound(fromKey);
  QCPDataMap::iterator itEnd = mData->upperBound(toKey);
  while (it != itEnd)
    it = mData->erase(it);
}

/*! \overload
  
  Removes a single data point at \a key. If the position is not known with absolute precision,
  consider using \ref removeData(double fromKey, double toKey) with a small fuzziness interval around
  the suspected position, depeding on the precision with which the key is known.

  \see addData, clearData
*/
void QCPGraph::removeData(double key)
{
  mData->remove(key);
}

/*!
  Removes all data points.
  \see removeData, removeDataAfter, removeDataBefore
*/
void QCPGraph::clearData()
{
  mData->clear();
}

/* inherits documentation from base class */
double QCPGraph::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if ((onlySelectable && !mSelectable) || mData->isEmpty())
    return -1;
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return -1; }
  
  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()))
    return pointDistance(pos);
  else
    return -1;
}

/*! \overload
  
  Allows to define whether error bars are taken into consideration when determining the new axis
  range.
  
  \see rescaleKeyAxis, rescaleValueAxis, QCPAbstractPlottable::rescaleAxes, QCustomPlot::rescaleAxes
*/
void QCPGraph::rescaleAxes(bool onlyEnlarge, bool includeErrorBars) const
{
  rescaleKeyAxis(onlyEnlarge, includeErrorBars);
  rescaleValueAxis(onlyEnlarge, includeErrorBars);
}

/*! \overload
  
  Allows to define whether error bars (of kind \ref QCPGraph::etKey) are taken into consideration
  when determining the new axis range.
  
  \see rescaleAxes, QCPAbstractPlottable::rescaleKeyAxis
*/
void QCPGraph::rescaleKeyAxis(bool onlyEnlarge, bool includeErrorBars) const
{
  // this code is a copy of QCPAbstractPlottable::rescaleKeyAxis with the only change
  // that getKeyRange is passed the includeErrorBars value.
  if (mData->isEmpty()) return;
  
  QCPAxis *keyAxis = mKeyAxis.data();
  if (!keyAxis) { qDebug() << Q_FUNC_INFO << "invalid key axis"; return; }

  SignDomain signDomain = sdBoth;
  if (keyAxis->scaleType() == QCPAxis::stLogarithmic)
    signDomain = (keyAxis->range().upper < 0 ? sdNegative : sdPositive);
  
  bool foundRange;
  QCPRange newRange = getKeyRange(foundRange, signDomain, includeErrorBars);
  
  if (foundRange)
  {
    if (onlyEnlarge)
    {
      if (keyAxis->range().lower < newRange.lower)
        newRange.lower = keyAxis->range().lower;
      if (keyAxis->range().upper > newRange.upper)
        newRange.upper = keyAxis->range().upper;
    }
    keyAxis->setRange(newRange);
  }
}

/*! \overload
  
  Allows to define whether error bars (of kind \ref QCPGraph::etValue) are taken into consideration
  when determining the new axis range.
  
  \see rescaleAxes, QCPAbstractPlottable::rescaleValueAxis
*/
void QCPGraph::rescaleValueAxis(bool onlyEnlarge, bool includeErrorBars) const
{
  // this code is a copy of QCPAbstractPlottable::rescaleValueAxis with the only change
  // is that getValueRange is passed the includeErrorBars value.
  if (mData->isEmpty()) return;
  
  QCPAxis *valueAxis = mValueAxis.data();
  if (!valueAxis) { qDebug() << Q_FUNC_INFO << "invalid value axis"; return; }

  SignDomain signDomain = sdBoth;
  if (valueAxis->scaleType() == QCPAxis::stLogarithmic)
    signDomain = (valueAxis->range().upper < 0 ? sdNegative : sdPositive);
  
  bool foundRange;
  QCPRange newRange = getValueRange(foundRange, signDomain, includeErrorBars);
  
  if (foundRange)
  {
    if (onlyEnlarge)
    {
      if (valueAxis->range().lower < newRange.lower)
        newRange.lower = valueAxis->range().lower;
      if (valueAxis->range().upper > newRange.upper)
        newRange.upper = valueAxis->range().upper;
    }
    valueAxis->setRange(newRange);
  }
}

/* inherits documentation from base class */
void QCPGraph::draw(QCPPainter *painter)
{
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  if (mKeyAxis.data()->range().size() <= 0 || mData->isEmpty()) return;
  if (mLineStyle == lsNone && mScatterStyle.isNone()) return;
  
  // allocate line and (if necessary) point vectors:
  QVector<QPointF> *lineData = new QVector<QPointF>;
  QVector<QCPData> *scatterData = 0;
  if (!mScatterStyle.isNone())
    scatterData = new QVector<QCPData>;
  
  // fill vectors with data appropriate to plot style:
  getPlotData(lineData, scatterData);
  
  // check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
  QCPDataMap::const_iterator it;
  for (it = mData->constBegin(); it != mData->constEnd(); ++it)
  {
    if (QCP::isInvalidData(it.value().key, it.value().value) ||
        QCP::isInvalidData(it.value().keyErrorPlus, it.value().keyErrorMinus) ||
        QCP::isInvalidData(it.value().valueErrorPlus, it.value().valueErrorPlus))
      qDebug() << Q_FUNC_INFO << "Data point at" << it.key() << "invalid." << "Plottable name:" << name();
  }
#endif

  // draw fill of graph:
  drawFill(painter, lineData);
  
  // draw line:
  if (mLineStyle == lsImpulse)
    drawImpulsePlot(painter, lineData);
  else if (mLineStyle != lsNone)
    drawLinePlot(painter, lineData); // also step plots can be drawn as a line plot
  
  // draw scatters:
  if (scatterData)
    drawScatterPlot(painter, scatterData);
  
  // free allocated line and point vectors:
  delete lineData;
  if (scatterData)
    delete scatterData;
}

/* inherits documentation from base class */
void QCPGraph::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
  // draw fill:
  if (mBrush.style() != Qt::NoBrush)
  {
    applyFillAntialiasingHint(painter);
    painter->fillRect(QRectF(rect.left(), rect.top()+rect.height()/2.0, rect.width(), rect.height()/3.0), mBrush);
  }
  // draw line vertically centered:
  if (mLineStyle != lsNone)
  {
    applyDefaultAntialiasingHint(painter);
    painter->setPen(mPen);
    painter->drawLine(QLineF(rect.left(), rect.top()+rect.height()/2.0, rect.right()+5, rect.top()+rect.height()/2.0)); // +5 on x2 else last segment is missing from dashed/dotted pens
  }
  // draw scatter symbol:
  if (!mScatterStyle.isNone())
  {
    applyScattersAntialiasingHint(painter);
    // scale scatter pixmap if it's too large to fit in legend icon rect:
    if (mScatterStyle.shape() == QCPScatterStyle::ssPixmap && (mScatterStyle.pixmap().size().width() > rect.width() || mScatterStyle.pixmap().size().height() > rect.height()))
    {
      QCPScatterStyle scaledStyle(mScatterStyle);
      scaledStyle.setPixmap(scaledStyle.pixmap().scaled(rect.size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
      scaledStyle.applyTo(painter, mPen);
      scaledStyle.drawShape(painter, QRectF(rect).center());
    } else
    {
      mScatterStyle.applyTo(painter, mPen);
      mScatterStyle.drawShape(painter, QRectF(rect).center());
    }
  }
}

/*! \internal

  This function branches out to the line style specific "get(...)PlotData" functions, according to
  the line style of the graph.
  
  \a lineData will be filled with raw points that will be drawn with the according draw functions,
  e.g. \ref drawLinePlot and \ref drawImpulsePlot. These aren't necessarily the original data
  points, since for step plots for example, additional points are needed for drawing lines that
  make up steps. If the line style of the graph is \ref lsNone, the \a lineData vector will be left
  untouched.
  
  \a scatterData will be filled with the original data points so \ref drawScatterPlot can draw the
  scatter symbols accordingly. If no scatters need to be drawn, i.e. the scatter style's shape is
  \ref QCPScatterStyle::ssNone, pass 0 as \a scatterData, and this step will be skipped.
  
  \see getScatterPlotData, getLinePlotData, getStepLeftPlotData, getStepRightPlotData,
  getStepCenterPlotData, getImpulsePlotData
*/
void QCPGraph::getPlotData(QVector<QPointF> *lineData, QVector<QCPData> *scatterData) const
{
  switch(mLineStyle)
  {
    case lsNone: getScatterPlotData(scatterData); break;
    case lsLine: getLinePlotData(lineData, scatterData); break;
    case lsStepLeft: getStepLeftPlotData(lineData, scatterData); break;
    case lsStepRight: getStepRightPlotData(lineData, scatterData); break;
    case lsStepCenter: getStepCenterPlotData(lineData, scatterData); break;
    case lsImpulse: getImpulsePlotData(lineData, scatterData); break;
  }
}

/*! \internal
  
  If line style is \ref lsNone and the scatter style's shape is not \ref QCPScatterStyle::ssNone,
  this function serves at providing the visible data points in \a scatterData, so the \ref
  drawScatterPlot function can draw the scatter points accordingly.
  
  If line style is not \ref lsNone, this function is not called and the data for the scatter points
  are (if needed) calculated inside the corresponding other "get(...)PlotData" functions.
  
  \see drawScatterPlot
*/
void QCPGraph::getScatterPlotData(QVector<QCPData> *scatterData) const
{
  getPreparedData(0, scatterData);
}

/*! \internal
  
  Places the raw data points needed for a normal linearly connected graph in \a linePixelData.

  As for all plot data retrieval functions, \a scatterData just contains all unaltered data (scatter)
  points that are visible for drawing scatter points, if necessary. If drawing scatter points is
  disabled (i.e. the scatter style's shape is \ref QCPScatterStyle::ssNone), pass 0 as \a
  scatterData, and the function will skip filling the vector.
  
  \see drawLinePlot
*/
void QCPGraph::getLinePlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  if (!linePixelData) { qDebug() << Q_FUNC_INFO << "null pointer passed as linePixelData"; return; }
  
  QVector<QCPData> lineData;
  getPreparedData(&lineData, scatterData);
  linePixelData->reserve(lineData.size()+2); // added 2 to reserve memory for lower/upper fill base points that might be needed for fill
  linePixelData->resize(lineData.size());
  
  // transform lineData points to pixels:
  if (keyAxis->orientation() == Qt::Vertical)
  {
    for (int i=0; i<lineData.size(); ++i)
    {
      (*linePixelData)[i].setX(valueAxis->coordToPixel(lineData.at(i).value));
      (*linePixelData)[i].setY(keyAxis->coordToPixel(lineData.at(i).key));
    }
  } else // key axis is horizontal
  {
    for (int i=0; i<lineData.size(); ++i)
    {
      (*linePixelData)[i].setX(keyAxis->coordToPixel(lineData.at(i).key));
      (*linePixelData)[i].setY(valueAxis->coordToPixel(lineData.at(i).value));
    }
  }
}

/*!
  \internal
  Places the raw data points needed for a step plot with left oriented steps in \a lineData.

  As for all plot data retrieval functions, \a scatterData just contains all unaltered data (scatter)
  points that are visible for drawing scatter points, if necessary. If drawing scatter points is
  disabled (i.e. the scatter style's shape is \ref QCPScatterStyle::ssNone), pass 0 as \a
  scatterData, and the function will skip filling the vector.
  
  \see drawLinePlot
*/
void QCPGraph::getStepLeftPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  if (!linePixelData) { qDebug() << Q_FUNC_INFO << "null pointer passed as lineData"; return; }
  
  QVector<QCPData> lineData;
  getPreparedData(&lineData, scatterData);
  linePixelData->reserve(lineData.size()*2+2); // added 2 to reserve memory for lower/upper fill base points that might be needed for fill
  linePixelData->resize(lineData.size()*2);
  
  // calculate steps from lineData and transform to pixel coordinates:
  if (keyAxis->orientation() == Qt::Vertical)
  {
    double lastValue = valueAxis->coordToPixel(lineData.first().value);
    double key;
    for (int i=0; i<lineData.size(); ++i)
    {
      key = keyAxis->coordToPixel(lineData.at(i).key);
      (*linePixelData)[i*2+0].setX(lastValue);
      (*linePixelData)[i*2+0].setY(key);
      lastValue = valueAxis->coordToPixel(lineData.at(i).value);
      (*linePixelData)[i*2+1].setX(lastValue);
      (*linePixelData)[i*2+1].setY(key);
    }
  } else // key axis is horizontal
  {
    double lastValue = valueAxis->coordToPixel(lineData.first().value);
    double key;
    for (int i=0; i<lineData.size(); ++i)
    {
      key = keyAxis->coordToPixel(lineData.at(i).key);
      (*linePixelData)[i*2+0].setX(key);
      (*linePixelData)[i*2+0].setY(lastValue);
      lastValue = valueAxis->coordToPixel(lineData.at(i).value);
      (*linePixelData)[i*2+1].setX(key);
      (*linePixelData)[i*2+1].setY(lastValue);
    }
  }
}

/*!
  \internal
  Places the raw data points needed for a step plot with right oriented steps in \a lineData.

  As for all plot data retrieval functions, \a scatterData just contains all unaltered data (scatter)
  points that are visible for drawing scatter points, if necessary. If drawing scatter points is
  disabled (i.e. the scatter style's shape is \ref QCPScatterStyle::ssNone), pass 0 as \a
  scatterData, and the function will skip filling the vector.
  
  \see drawLinePlot
*/
void QCPGraph::getStepRightPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  if (!linePixelData) { qDebug() << Q_FUNC_INFO << "null pointer passed as lineData"; return; }
  
  QVector<QCPData> lineData;
  getPreparedData(&lineData, scatterData);
  linePixelData->reserve(lineData.size()*2+2); // added 2 to reserve memory for lower/upper fill base points that might be needed for fill
  linePixelData->resize(lineData.size()*2);
  
  // calculate steps from lineData and transform to pixel coordinates:
  if (keyAxis->orientation() == Qt::Vertical)
  {
    double lastKey = keyAxis->coordToPixel(lineData.first().key);
    double value;
    for (int i=0; i<lineData.size(); ++i)
    {
      value = valueAxis->coordToPixel(lineData.at(i).value);
      (*linePixelData)[i*2+0].setX(value);
      (*linePixelData)[i*2+0].setY(lastKey);
      lastKey = keyAxis->coordToPixel(lineData.at(i).key);
      (*linePixelData)[i*2+1].setX(value);
      (*linePixelData)[i*2+1].setY(lastKey);
    }
  } else // key axis is horizontal
  {
    double lastKey = keyAxis->coordToPixel(lineData.first().key);
    double value;
    for (int i=0; i<lineData.size(); ++i)
    {
      value = valueAxis->coordToPixel(lineData.at(i).value);
      (*linePixelData)[i*2+0].setX(lastKey);
      (*linePixelData)[i*2+0].setY(value);
      lastKey = keyAxis->coordToPixel(lineData.at(i).key);
      (*linePixelData)[i*2+1].setX(lastKey);
      (*linePixelData)[i*2+1].setY(value);
    }
  }
}

/*!
  \internal
  Places the raw data points needed for a step plot with centered steps in \a lineData.

  As for all plot data retrieval functions, \a scatterData just contains all unaltered data (scatter)
  points that are visible for drawing scatter points, if necessary. If drawing scatter points is
  disabled (i.e. the scatter style's shape is \ref QCPScatterStyle::ssNone), pass 0 as \a
  scatterData, and the function will skip filling the vector.
  
  \see drawLinePlot
*/
void QCPGraph::getStepCenterPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  if (!linePixelData) { qDebug() << Q_FUNC_INFO << "null pointer passed as lineData"; return; }
  
  QVector<QCPData> lineData;
  getPreparedData(&lineData, scatterData);
  linePixelData->reserve(lineData.size()*2+2); // added 2 to reserve memory for lower/upper fill base points that might be needed for fill
  linePixelData->resize(lineData.size()*2);
  // calculate steps from lineData and transform to pixel coordinates:
  if (keyAxis->orientation() == Qt::Vertical)
  {
    double lastKey = keyAxis->coordToPixel(lineData.first().key);
    double lastValue = valueAxis->coordToPixel(lineData.first().value);
    double key;
    (*linePixelData)[0].setX(lastValue);
    (*linePixelData)[0].setY(lastKey);
    for (int i=1; i<lineData.size(); ++i)
    {
      key = (keyAxis->coordToPixel(lineData.at(i).key)+lastKey)*0.5;
      (*linePixelData)[i*2-1].setX(lastValue);
      (*linePixelData)[i*2-1].setY(key);
      lastValue = valueAxis->coordToPixel(lineData.at(i).value);
      lastKey = keyAxis->coordToPixel(lineData.at(i).key);
      (*linePixelData)[i*2+0].setX(lastValue);
      (*linePixelData)[i*2+0].setY(key);
    }
    (*linePixelData)[lineData.size()*2-1].setX(lastValue);
    (*linePixelData)[lineData.size()*2-1].setY(lastKey);
  } else // key axis is horizontal
  {
    double lastKey = keyAxis->coordToPixel(lineData.first().key);
    double lastValue = valueAxis->coordToPixel(lineData.first().value);
    double key;
    (*linePixelData)[0].setX(lastKey);
    (*linePixelData)[0].setY(lastValue);
    for (int i=1; i<lineData.size(); ++i)
    {
      key = (keyAxis->coordToPixel(lineData.at(i).key)+lastKey)*0.5;
      (*linePixelData)[i*2-1].setX(key);
      (*linePixelData)[i*2-1].setY(lastValue);
      lastValue = valueAxis->coordToPixel(lineData.at(i).value);
      lastKey = keyAxis->coordToPixel(lineData.at(i).key);
      (*linePixelData)[i*2+0].setX(key);
      (*linePixelData)[i*2+0].setY(lastValue);
    }
    (*linePixelData)[lineData.size()*2-1].setX(lastKey);
    (*linePixelData)[lineData.size()*2-1].setY(lastValue);
  }

}

/*!
  \internal
  Places the raw data points needed for an impulse plot in \a lineData.

  As for all plot data retrieval functions, \a scatterData just contains all unaltered data (scatter)
  points that are visible for drawing scatter points, if necessary. If drawing scatter points is
  disabled (i.e. the scatter style's shape is \ref QCPScatterStyle::ssNone), pass 0 as \a
  scatterData, and the function will skip filling the vector.
  
  \see drawImpulsePlot
*/
void QCPGraph::getImpulsePlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  if (!linePixelData) { qDebug() << Q_FUNC_INFO << "null pointer passed as linePixelData"; return; }
  
  QVector<QCPData> lineData;
  getPreparedData(&lineData, scatterData);
  linePixelData->resize(lineData.size()*2); // no need to reserve 2 extra points because impulse plot has no fill
  
  // transform lineData points to pixels:
  if (keyAxis->orientation() == Qt::Vertical)
  {
    double zeroPointX = valueAxis->coordToPixel(0);
    double key;
    for (int i=0; i<lineData.size(); ++i)
    {
      key = keyAxis->coordToPixel(lineData.at(i).key);
      (*linePixelData)[i*2+0].setX(zeroPointX);
      (*linePixelData)[i*2+0].setY(key);
      (*linePixelData)[i*2+1].setX(valueAxis->coordToPixel(lineData.at(i).value));
      (*linePixelData)[i*2+1].setY(key);
    }
  } else // key axis is horizontal
  {
    double zeroPointY = valueAxis->coordToPixel(0);
    double key;
    for (int i=0; i<lineData.size(); ++i)
    {
      key = keyAxis->coordToPixel(lineData.at(i).key);
      (*linePixelData)[i*2+0].setX(key);
      (*linePixelData)[i*2+0].setY(zeroPointY);
      (*linePixelData)[i*2+1].setX(key);
      (*linePixelData)[i*2+1].setY(valueAxis->coordToPixel(lineData.at(i).value));
    }
  }
}

/*! \internal
  
  Draws the fill of the graph with the specified brush.

  If the fill is a normal fill towards the zero-value-line, only the \a lineData is required (and
  two extra points at the zero-value-line, which are added by \ref addFillBasePoints and removed by
  \ref removeFillBasePoints after the fill drawing is done).
  
  If the fill is a channel fill between this QCPGraph and another QCPGraph (mChannelFillGraph), the
  more complex polygon is calculated with the \ref getChannelFillPolygon function.
  
  \see drawLinePlot
*/
void QCPGraph::drawFill(QCPPainter *painter, QVector<QPointF> *lineData) const
{
  if (mLineStyle == lsImpulse) return; // fill doesn't make sense for impulse plot
  if (mainBrush().style() == Qt::NoBrush || mainBrush().color().alpha() == 0) return;
  
  applyFillAntialiasingHint(painter);
  if (!mChannelFillGraph)
  {
    // draw base fill under graph, fill goes all the way to the zero-value-line:
    addFillBasePoints(lineData);
    painter->setPen(Qt::NoPen);
    painter->setBrush(mainBrush());
    painter->drawPolygon(QPolygonF(*lineData));
    removeFillBasePoints(lineData);
  } else
  {
    // draw channel fill between this graph and mChannelFillGraph:
    painter->setPen(Qt::NoPen);
    painter->setBrush(mainBrush());
    painter->drawPolygon(getChannelFillPolygon(lineData));
  }
}

/*! \internal
  
  Draws scatter symbols at every data point passed in \a scatterData. scatter symbols are independent
  of the line style and are always drawn if the scatter style's shape is not \ref
  QCPScatterStyle::ssNone. Hence, the \a scatterData vector is outputted by all "get(...)PlotData"
  functions, together with the (line style dependent) line data.
  
  \see drawLinePlot, drawImpulsePlot
*/
void QCPGraph::drawScatterPlot(QCPPainter *painter, QVector<QCPData> *scatterData) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  
  // draw error bars:
  if (mErrorType != etNone)
  {
    applyErrorBarsAntialiasingHint(painter);
    painter->setPen(mErrorPen);
    if (keyAxis->orientation() == Qt::Vertical)
    {
      for (int i=0; i<scatterData->size(); ++i)
        drawError(painter, valueAxis->coordToPixel(scatterData->at(i).value), keyAxis->coordToPixel(scatterData->at(i).key), scatterData->at(i));
    } else
    {
      for (int i=0; i<scatterData->size(); ++i)
        drawError(painter, keyAxis->coordToPixel(scatterData->at(i).key), valueAxis->coordToPixel(scatterData->at(i).value), scatterData->at(i));
    }
  }
  
  // draw scatter point symbols:
  applyScattersAntialiasingHint(painter);
  mScatterStyle.applyTo(painter, mPen);
  if (keyAxis->orientation() == Qt::Vertical)
  {
    for (int i=0; i<scatterData->size(); ++i)
      mScatterStyle.drawShape(painter, valueAxis->coordToPixel(scatterData->at(i).value), keyAxis->coordToPixel(scatterData->at(i).key));
  } else
  {
    for (int i=0; i<scatterData->size(); ++i)
      mScatterStyle.drawShape(painter, keyAxis->coordToPixel(scatterData->at(i).key), valueAxis->coordToPixel(scatterData->at(i).value));
  }
}

/*!  \internal
  
  Draws line graphs from the provided data. It connects all points in \a lineData, which was
  created by one of the "get(...)PlotData" functions for line styles that require simple line
  connections between the point vector they create. These are for example \ref getLinePlotData,
  \ref getStepLeftPlotData, \ref getStepRightPlotData and \ref getStepCenterPlotData.
  
  \see drawScatterPlot, drawImpulsePlot
*/
void QCPGraph::drawLinePlot(QCPPainter *painter, QVector<QPointF> *lineData) const
{
  // draw line of graph:
  if (mainPen().style() != Qt::NoPen && mainPen().color().alpha() != 0)
  {
    applyDefaultAntialiasingHint(painter);
    painter->setPen(mainPen());
    painter->setBrush(Qt::NoBrush);
    
    /* Draws polyline in batches, currently not used:
    int p = 0;
    while (p < lineData->size())
    {
      int batch = qMin(25, lineData->size()-p);
      if (p != 0)
      {
        ++batch;
        --p; // to draw the connection lines between two batches
      }
      painter->drawPolyline(lineData->constData()+p, batch);
      p += batch;
    }
    */
    
    // if drawing solid line and not in PDF, use much faster line drawing instead of polyline:
    if (mParentPlot->plottingHints().testFlag(QCP::phFastPolylines) &&
        painter->pen().style() == Qt::SolidLine &&
        !painter->modes().testFlag(QCPPainter::pmVectorized)&&
        !painter->modes().testFlag(QCPPainter::pmNoCaching))
    {
      for (int i=1; i<lineData->size(); ++i)
        painter->drawLine(lineData->at(i-1), lineData->at(i));
    } else
    {
      painter->drawPolyline(QPolygonF(*lineData));
    }
  }
}

/*! \internal
  
  Draws impulses from the provided data, i.e. it connects all line pairs in \a lineData, which was
  created by \ref getImpulsePlotData.
  
  \see drawScatterPlot, drawLinePlot
*/
void QCPGraph::drawImpulsePlot(QCPPainter *painter, QVector<QPointF> *lineData) const
{
  // draw impulses:
  if (mainPen().style() != Qt::NoPen && mainPen().color().alpha() != 0)
  {
    applyDefaultAntialiasingHint(painter);
    QPen pen = mainPen();
    pen.setCapStyle(Qt::FlatCap); // so impulse line doesn't reach beyond zero-line
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawLines(*lineData);
  }
}

/*! \internal
  
  Returns the \a lineData and \a scatterData that need to be plotted for this graph taking into
  consideration the current axis ranges and, if \ref setAdaptiveSampling is enabled, local point
  densities.
  
  0 may be passed as \a lineData or \a scatterData to indicate that the respective dataset isn't
  needed. For example, if the scatter style (\ref setScatterStyle) is \ref QCPScatterStyle::ssNone, \a
  scatterData should be 0 to prevent unnecessary calculations.
  
  This method is used by the various "get(...)PlotData" methods to get the basic working set of data.
*/
void QCPGraph::getPreparedData(QVector<QCPData> *lineData, QVector<QCPData> *scatterData) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  // get visible data range:
  QCPDataMap::const_iterator lower, upper; // note that upper is the actual upper point, and not 1 step after the upper point
  getVisibleDataBounds(lower, upper);
  if (lower == mData->constEnd() || upper == mData->constEnd())
    return;
  
  // count points in visible range, taking into account that we only need to count to the limit maxCount if using adaptive sampling:
  int maxCount = std::numeric_limits<int>::max();
  if (mAdaptiveSampling)
  {
    int keyPixelSpan = qAbs(keyAxis->coordToPixel(lower.key())-keyAxis->coordToPixel(upper.key()));
    maxCount = 2*keyPixelSpan+2;
  }
  int dataCount = countDataInBounds(lower, upper, maxCount);
  
  if (mAdaptiveSampling && dataCount >= maxCount) // use adaptive sampling only if there are at least two points per pixel on average
  {
    if (lineData)
    {
      QCPDataMap::const_iterator it = lower;
      QCPDataMap::const_iterator upperEnd = upper+1;
      double minValue = it.value().value;
      double maxValue = it.value().value;
      QCPDataMap::const_iterator currentIntervalFirstPoint = it;
      int reversedFactor = keyAxis->rangeReversed() ? -1 : 1; // is used to calculate keyEpsilon pixel into the correct direction
      int reversedRound = keyAxis->rangeReversed() ? 1 : 0; // is used to switch between floor (normal) and ceil (reversed) rounding of currentIntervalStartKey
      double currentIntervalStartKey = keyAxis->pixelToCoord((int)(keyAxis->coordToPixel(lower.key())+reversedRound));
      double lastIntervalEndKey = currentIntervalStartKey;
      double keyEpsilon = qAbs(currentIntervalStartKey-keyAxis->pixelToCoord(keyAxis->coordToPixel(currentIntervalStartKey)+1.0*reversedFactor)); // interval of one pixel on screen when mapped to plot key coordinates
      bool keyEpsilonVariable = keyAxis->scaleType() == QCPAxis::stLogarithmic; // indicates whether keyEpsilon needs to be updated after every interval (for log axes)
      int intervalDataCount = 1;
      ++it; // advance iterator to second data point because adaptive sampling works in 1 point retrospect
      while (it != upperEnd)
      {
        if (it.key() < currentIntervalStartKey+keyEpsilon) // data point is still within same pixel, so skip it and expand value span of this cluster if necessary
        {
          if (it.value().value < minValue)
            minValue = it.value().value;
          else if (it.value().value > maxValue)
            maxValue = it.value().value;
          ++intervalDataCount;
        } else // new pixel interval started
        {
          if (intervalDataCount >= 2) // last pixel had multiple data points, consolidate them to a cluster
          {
            if (lastIntervalEndKey < currentIntervalStartKey-keyEpsilon) // last point is further away, so first point of this cluster must be at a real data point
              lineData->append(QCPData(currentIntervalStartKey+keyEpsilon*0.2, currentIntervalFirstPoint.value().value));
            lineData->append(QCPData(currentIntervalStartKey+keyEpsilon*0.25, minValue));
            lineData->append(QCPData(currentIntervalStartKey+keyEpsilon*0.75, maxValue));
            if (it.key() > currentIntervalStartKey+keyEpsilon*2) // new pixel started further away from previous cluster, so make sure the last point of the cluster is at a real data point
              lineData->append(QCPData(currentIntervalStartKey+keyEpsilon*0.8, (it-1).value().value));
          } else
            lineData->append(QCPData(currentIntervalFirstPoint.key(), currentIntervalFirstPoint.value().value));
          lastIntervalEndKey = (it-1).value().key;
          minValue = it.value().value;
          maxValue = it.value().value;
          currentIntervalFirstPoint = it;
          currentIntervalStartKey = keyAxis->pixelToCoord((int)(keyAxis->coordToPixel(it.key())+reversedRound));
          if (keyEpsilonVariable)
            keyEpsilon = qAbs(currentIntervalStartKey-keyAxis->pixelToCoord(keyAxis->coordToPixel(currentIntervalStartKey)+1.0*reversedFactor));
          intervalDataCount = 1;
        }
        ++it;
      }
      // handle last interval:
      if (intervalDataCount >= 2) // last pixel had multiple data points, consolidate them to a cluster
      {
        if (lastIntervalEndKey < currentIntervalStartKey-keyEpsilon) // last point wasn't a cluster, so first point of this cluster must be at a real data point
          lineData->append(QCPData(currentIntervalStartKey+keyEpsilon*0.2, currentIntervalFirstPoint.value().value));
        lineData->append(QCPData(currentIntervalStartKey+keyEpsilon*0.25, minValue));
        lineData->append(QCPData(currentIntervalStartKey+keyEpsilon*0.75, maxValue));
      } else
        lineData->append(QCPData(currentIntervalFirstPoint.key(), currentIntervalFirstPoint.value().value));
    }
    
    if (scatterData)
    {
      double valueMaxRange = valueAxis->range().upper;
      double valueMinRange = valueAxis->range().lower;
      QCPDataMap::const_iterator it = lower;
      QCPDataMap::const_iterator upperEnd = upper+1;
      double minValue = it.value().value;
      double maxValue = it.value().value;
      QCPDataMap::const_iterator minValueIt = it;
      QCPDataMap::const_iterator maxValueIt = it;
      QCPDataMap::const_iterator currentIntervalStart = it;
      int reversedFactor = keyAxis->rangeReversed() ? -1 : 1; // is used to calculate keyEpsilon pixel into the correct direction
      int reversedRound = keyAxis->rangeReversed() ? 1 : 0; // is used to switch between floor (normal) and ceil (reversed) rounding of currentIntervalStartKey
      double currentIntervalStartKey = keyAxis->pixelToCoord((int)(keyAxis->coordToPixel(lower.key())+reversedRound));
      double keyEpsilon = qAbs(currentIntervalStartKey-keyAxis->pixelToCoord(keyAxis->coordToPixel(currentIntervalStartKey)+1.0*reversedFactor)); // interval of one pixel on screen when mapped to plot key coordinates
      bool keyEpsilonVariable = keyAxis->scaleType() == QCPAxis::stLogarithmic; // indicates whether keyEpsilon needs to be updated after every interval (for log axes)
      int intervalDataCount = 1;
      ++it; // advance iterator to second data point because adaptive sampling works in 1 point retrospect
      while (it != upperEnd)
      {
        if (it.key() < currentIntervalStartKey+keyEpsilon) // data point is still within same pixel, so skip it and expand value span of this pixel if necessary
        {
          if (it.value().value < minValue && it.value().value > valueMinRange && it.value().value < valueMaxRange)
          {
            minValue = it.value().value;
            minValueIt = it;
          } else if (it.value().value > maxValue && it.value().value > valueMinRange && it.value().value < valueMaxRange)
          {
            maxValue = it.value().value;
            maxValueIt = it;
          }
          ++intervalDataCount;
        } else // new pixel started
        {
          if (intervalDataCount >= 2) // last pixel had multiple data points, consolidate them
          {
            // determine value pixel span and add as many points in interval to maintain certain vertical data density (this is specific to scatter plot):
            double valuePixelSpan = qAbs(valueAxis->coordToPixel(minValue)-valueAxis->coordToPixel(maxValue));
            int dataModulo = qMax(1, qRound(intervalDataCount/(valuePixelSpan/4.0))); // approximately every 4 value pixels one data point on average
            QCPDataMap::const_iterator intervalIt = currentIntervalStart;
            int c = 0;
            while (intervalIt != it)
            {
              if ((c % dataModulo == 0 || intervalIt == minValueIt || intervalIt == maxValueIt) && intervalIt.value().value > valueMinRange && intervalIt.value().value < valueMaxRange)
                scatterData->append(intervalIt.value());
              ++c;
              ++intervalIt;
            }
          } else if (currentIntervalStart.value().value > valueMinRange && currentIntervalStart.value().value < valueMaxRange)
            scatterData->append(currentIntervalStart.value());
          minValue = it.value().value;
          maxValue = it.value().value;
          currentIntervalStart = it;
          currentIntervalStartKey = keyAxis->pixelToCoord((int)(keyAxis->coordToPixel(it.key())+reversedRound));
          if (keyEpsilonVariable)
            keyEpsilon = qAbs(currentIntervalStartKey-keyAxis->pixelToCoord(keyAxis->coordToPixel(currentIntervalStartKey)+1.0*reversedFactor));
          intervalDataCount = 1;
        }
        ++it;
      }
      // handle last interval:
      if (intervalDataCount >= 2) // last pixel had multiple data points, consolidate them
      {
        // determine value pixel span and add as many points in interval to maintain certain vertical data density (this is specific to scatter plot):
        double valuePixelSpan = qAbs(valueAxis->coordToPixel(minValue)-valueAxis->coordToPixel(maxValue));
        int dataModulo = qMax(1, qRound(intervalDataCount/(valuePixelSpan/4.0))); // approximately every 4 value pixels one data point on average
        QCPDataMap::const_iterator intervalIt = currentIntervalStart;
        int c = 0;
        while (intervalIt != it)
        {
          if ((c % dataModulo == 0 || intervalIt == minValueIt || intervalIt == maxValueIt) && intervalIt.value().value > valueMinRange && intervalIt.value().value < valueMaxRange)
            scatterData->append(intervalIt.value());
          ++c;
          ++intervalIt;
        }
      } else if (currentIntervalStart.value().value > valueMinRange && currentIntervalStart.value().value < valueMaxRange)
        scatterData->append(currentIntervalStart.value());
    }
  } else // don't use adaptive sampling algorithm, transfer points one-to-one from the map into the output parameters
  {
    QVector<QCPData> *dataVector = 0;
    if (lineData)
      dataVector = lineData;
    else if (scatterData)
      dataVector = scatterData;
    if (dataVector)
    {
      QCPDataMap::const_iterator it = lower;
      QCPDataMap::const_iterator upperEnd = upper+1;
      dataVector->reserve(dataCount+2); // +2 for possible fill end points
      while (it != upperEnd)
      {
        dataVector->append(it.value());
        ++it;
      }
    }
    if (lineData && scatterData)
      *scatterData = *dataVector;
  }
}

/*!  \internal
  
  called by the scatter drawing function (\ref drawScatterPlot) to draw the error bars on one data
  point. \a x and \a y pixel positions of the data point are passed since they are already known in
  pixel coordinates in the drawing function, so we save some extra coordToPixel transforms here. \a
  data is therefore only used for the errors, not key and value.
*/
void QCPGraph::drawError(QCPPainter *painter, double x, double y, const QCPData &data) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  
  double a, b; // positions of error bar bounds in pixels
  double barWidthHalf = mErrorBarSize*0.5;
  double skipSymbolMargin = mScatterStyle.size(); // pixels left blank per side, when mErrorBarSkipSymbol is true

  if (keyAxis->orientation() == Qt::Vertical)
  {
    // draw key error vertically and value error horizontally
    if (mErrorType == etKey || mErrorType == etBoth)
    {
      a = keyAxis->coordToPixel(data.key-data.keyErrorMinus);
      b = keyAxis->coordToPixel(data.key+data.keyErrorPlus);
      if (keyAxis->rangeReversed())
        qSwap(a,b);
      // draw spine:
      if (mErrorBarSkipSymbol)
      {
        if (a-y > skipSymbolMargin) // don't draw spine if error is so small it's within skipSymbolmargin
          painter->drawLine(QLineF(x, a, x, y+skipSymbolMargin));
        if (y-b > skipSymbolMargin)
          painter->drawLine(QLineF(x, y-skipSymbolMargin, x, b));
      } else
        painter->drawLine(QLineF(x, a, x, b));
      // draw handles:
      painter->drawLine(QLineF(x-barWidthHalf, a, x+barWidthHalf, a));
      painter->drawLine(QLineF(x-barWidthHalf, b, x+barWidthHalf, b));
    }
    if (mErrorType == etValue || mErrorType == etBoth)
    {
      a = valueAxis->coordToPixel(data.value-data.valueErrorMinus);
      b = valueAxis->coordToPixel(data.value+data.valueErrorPlus);
      if (valueAxis->rangeReversed())
        qSwap(a,b);
      // draw spine:
      if (mErrorBarSkipSymbol)
      {
        if (x-a > skipSymbolMargin) // don't draw spine if error is so small it's within skipSymbolmargin
          painter->drawLine(QLineF(a, y, x-skipSymbolMargin, y));
        if (b-x > skipSymbolMargin)
          painter->drawLine(QLineF(x+skipSymbolMargin, y, b, y));
      } else
        painter->drawLine(QLineF(a, y, b, y));
      // draw handles:
      painter->drawLine(QLineF(a, y-barWidthHalf, a, y+barWidthHalf));
      painter->drawLine(QLineF(b, y-barWidthHalf, b, y+barWidthHalf));
    }
  } else // mKeyAxis->orientation() is Qt::Horizontal
  {
    // draw value error vertically and key error horizontally
    if (mErrorType == etKey || mErrorType == etBoth)
    {
      a = keyAxis->coordToPixel(data.key-data.keyErrorMinus);
      b = keyAxis->coordToPixel(data.key+data.keyErrorPlus);
      if (keyAxis->rangeReversed())
        qSwap(a,b);
      // draw spine:
      if (mErrorBarSkipSymbol)
      {
        if (x-a > skipSymbolMargin) // don't draw spine if error is so small it's within skipSymbolmargin
          painter->drawLine(QLineF(a, y, x-skipSymbolMargin, y));
        if (b-x > skipSymbolMargin)
          painter->drawLine(QLineF(x+skipSymbolMargin, y, b, y));
      } else
        painter->drawLine(QLineF(a, y, b, y));
      // draw handles:
      painter->drawLine(QLineF(a, y-barWidthHalf, a, y+barWidthHalf));
      painter->drawLine(QLineF(b, y-barWidthHalf, b, y+barWidthHalf));
    }
    if (mErrorType == etValue || mErrorType == etBoth)
    {
      a = valueAxis->coordToPixel(data.value-data.valueErrorMinus);
      b = valueAxis->coordToPixel(data.value+data.valueErrorPlus);
      if (valueAxis->rangeReversed())
        qSwap(a,b);
      // draw spine:
      if (mErrorBarSkipSymbol)
      {
        if (a-y > skipSymbolMargin) // don't draw spine if error is so small it's within skipSymbolmargin
          painter->drawLine(QLineF(x, a, x, y+skipSymbolMargin));
        if (y-b > skipSymbolMargin)
          painter->drawLine(QLineF(x, y-skipSymbolMargin, x, b));
      } else
        painter->drawLine(QLineF(x, a, x, b));
      // draw handles:
      painter->drawLine(QLineF(x-barWidthHalf, a, x+barWidthHalf, a));
      painter->drawLine(QLineF(x-barWidthHalf, b, x+barWidthHalf, b));
    }
  }
}

/*!  \internal
  
  called by \ref getPreparedData to determine which data (key) range is visible at the current key
  axis range setting, so only that needs to be processed.
  
  \a lower returns an iterator to the lowest data point that needs to be taken into account when
  plotting. Note that in order to get a clean plot all the way to the edge of the axis rect, \a
  lower may still be just outside the visible range.
  
  \a upper returns an iterator to the highest data point. Same as before, \a upper may also lie
  just outside of the visible range.
  
  if the graph contains no data, both \a lower and \a upper point to constEnd.
*/
void QCPGraph::getVisibleDataBounds(QCPDataMap::const_iterator &lower, QCPDataMap::const_iterator &upper) const
{
  if (!mKeyAxis) { qDebug() << Q_FUNC_INFO << "invalid key axis"; return; }
  if (mData->isEmpty())
  {
    lower = mData->constEnd();
    upper = mData->constEnd();
    return;
  }
  
  // get visible data range as QMap iterators
  QCPDataMap::const_iterator lbound = mData->lowerBound(mKeyAxis.data()->range().lower);
  QCPDataMap::const_iterator ubound = mData->upperBound(mKeyAxis.data()->range().upper);
  bool lowoutlier = lbound != mData->constBegin(); // indicates whether there exist points below axis range
  bool highoutlier = ubound != mData->constEnd(); // indicates whether there exist points above axis range
  
  lower = (lowoutlier ? lbound-1 : lbound); // data point range that will be actually drawn
  upper = (highoutlier ? ubound : ubound-1); // data point range that will be actually drawn
}

/*!  \internal
  
  Counts the number of data points between \a lower and \a upper (including them), up to a maximum
  of \a maxCount.
  
  This function is used by \ref getPreparedData to determine whether adaptive sampling shall be
  used (if enabled via \ref setAdaptiveSampling) or not. This is also why counting of data points
  only needs to be done until \a maxCount is reached, which should be set to the number of data
  points at which adaptive sampling sets in.
*/
int QCPGraph::countDataInBounds(const QCPDataMap::const_iterator &lower, const QCPDataMap::const_iterator &upper, int maxCount) const
{
  if (upper == mData->constEnd() && lower == mData->constEnd())
    return 0;
  QCPDataMap::const_iterator it = lower;
  int count = 1;
  while (it != upper && count < maxCount)
  {
    ++it;
    ++count;
  }
  return count;
}

/*! \internal
  
  The line data vector generated by e.g. getLinePlotData contains only the line that connects the
  data points. If the graph needs to be filled, two additional points need to be added at the
  value-zero-line in the lower and upper key positions of the graph. This function calculates these
  points and adds them to the end of \a lineData. Since the fill is typically drawn before the line
  stroke, these added points need to be removed again after the fill is done, with the
  removeFillBasePoints function.
  
  The expanding of \a lineData by two points will not cause unnecessary memory reallocations,
  because the data vector generation functions (getLinePlotData etc.) reserve two extra points when
  they allocate memory for \a lineData.
  
  \see removeFillBasePoints, lowerFillBasePoint, upperFillBasePoint
*/
void QCPGraph::addFillBasePoints(QVector<QPointF> *lineData) const
{
  if (!mKeyAxis) { qDebug() << Q_FUNC_INFO << "invalid key axis"; return; }
  
  // append points that close the polygon fill at the key axis:
  if (mKeyAxis.data()->orientation() == Qt::Vertical)
  {
    *lineData << upperFillBasePoint(lineData->last().y());
    *lineData << lowerFillBasePoint(lineData->first().y());
  } else
  {
    *lineData << upperFillBasePoint(lineData->last().x());
    *lineData << lowerFillBasePoint(lineData->first().x());
  }
}

/*! \internal
  
  removes the two points from \a lineData that were added by \ref addFillBasePoints.
  
  \see addFillBasePoints, lowerFillBasePoint, upperFillBasePoint
*/
void QCPGraph::removeFillBasePoints(QVector<QPointF> *lineData) const
{
  lineData->remove(lineData->size()-2, 2);
}

/*! \internal
  
  called by \ref addFillBasePoints to conveniently assign the point which closes the fill polygon
  on the lower side of the zero-value-line parallel to the key axis. The logarithmic axis scale
  case is a bit special, since the zero-value-line in pixel coordinates is in positive or negative
  infinity. So this case is handled separately by just closing the fill polygon on the axis which
  lies in the direction towards the zero value.
  
  \a lowerKey will be the the key (in pixels) of the returned point. Depending on whether the key
  axis is horizontal or vertical, \a lowerKey will end up as the x or y value of the returned
  point, respectively.
  
  \see upperFillBasePoint, addFillBasePoints
*/
QPointF QCPGraph::lowerFillBasePoint(double lowerKey) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return QPointF(); }
  
  QPointF point;
  if (valueAxis->scaleType() == QCPAxis::stLinear)
  {
    if (keyAxis->axisType() == QCPAxis::atLeft)
    {
      point.setX(valueAxis->coordToPixel(0));
      point.setY(lowerKey);
    } else if (keyAxis->axisType() == QCPAxis::atRight)
    {
      point.setX(valueAxis->coordToPixel(0));
      point.setY(lowerKey);
    } else if (keyAxis->axisType() == QCPAxis::atTop)
    {
      point.setX(lowerKey);
      point.setY(valueAxis->coordToPixel(0));
    } else if (keyAxis->axisType() == QCPAxis::atBottom)
    {
      point.setX(lowerKey);
      point.setY(valueAxis->coordToPixel(0));
    }
  } else // valueAxis->mScaleType == QCPAxis::stLogarithmic
  {
    // In logarithmic scaling we can't just draw to value zero so we just fill all the way
    // to the axis which is in the direction towards zero
    if (keyAxis->orientation() == Qt::Vertical)
    {
      if ((valueAxis->range().upper < 0 && !valueAxis->rangeReversed()) ||
          (valueAxis->range().upper > 0 && valueAxis->rangeReversed())) // if range is negative, zero is on opposite side of key axis
        point.setX(keyAxis->axisRect()->right());
      else
        point.setX(keyAxis->axisRect()->left());
      point.setY(lowerKey);
    } else if (keyAxis->axisType() == QCPAxis::atTop || keyAxis->axisType() == QCPAxis::atBottom)
    {
      point.setX(lowerKey);
      if ((valueAxis->range().upper < 0 && !valueAxis->rangeReversed()) ||
          (valueAxis->range().upper > 0 && valueAxis->rangeReversed())) // if range is negative, zero is on opposite side of key axis
        point.setY(keyAxis->axisRect()->top());
      else
        point.setY(keyAxis->axisRect()->bottom());
    }
  }
  return point;
}

/*! \internal
  
  called by \ref addFillBasePoints to conveniently assign the point which closes the fill
  polygon on the upper side of the zero-value-line parallel to the key axis. The logarithmic axis
  scale case is a bit special, since the zero-value-line in pixel coordinates is in positive or
  negative infinity. So this case is handled separately by just closing the fill polygon on the
  axis which lies in the direction towards the zero value.

  \a upperKey will be the the key (in pixels) of the returned point. Depending on whether the key
  axis is horizontal or vertical, \a upperKey will end up as the x or y value of the returned
  point, respectively.
  
  \see lowerFillBasePoint, addFillBasePoints
*/
QPointF QCPGraph::upperFillBasePoint(double upperKey) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return QPointF(); }
  
  QPointF point;
  if (valueAxis->scaleType() == QCPAxis::stLinear)
  {
    if (keyAxis->axisType() == QCPAxis::atLeft)
    {
      point.setX(valueAxis->coordToPixel(0));
      point.setY(upperKey);
    } else if (keyAxis->axisType() == QCPAxis::atRight)
    {
      point.setX(valueAxis->coordToPixel(0));
      point.setY(upperKey);
    } else if (keyAxis->axisType() == QCPAxis::atTop)
    {
      point.setX(upperKey);
      point.setY(valueAxis->coordToPixel(0));
    } else if (keyAxis->axisType() == QCPAxis::atBottom)
    {
      point.setX(upperKey);
      point.setY(valueAxis->coordToPixel(0));
    }
  } else // valueAxis->mScaleType == QCPAxis::stLogarithmic
  {
    // In logarithmic scaling we can't just draw to value 0 so we just fill all the way
    // to the axis which is in the direction towards 0
    if (keyAxis->orientation() == Qt::Vertical)
    {
      if ((valueAxis->range().upper < 0 && !valueAxis->rangeReversed()) ||
          (valueAxis->range().upper > 0 && valueAxis->rangeReversed())) // if range is negative, zero is on opposite side of key axis
        point.setX(keyAxis->axisRect()->right());
      else
        point.setX(keyAxis->axisRect()->left());
      point.setY(upperKey);
    } else if (keyAxis->axisType() == QCPAxis::atTop || keyAxis->axisType() == QCPAxis::atBottom)
    {
      point.setX(upperKey);
      if ((valueAxis->range().upper < 0 && !valueAxis->rangeReversed()) ||
          (valueAxis->range().upper > 0 && valueAxis->rangeReversed())) // if range is negative, zero is on opposite side of key axis
        point.setY(keyAxis->axisRect()->top());
      else
        point.setY(keyAxis->axisRect()->bottom());
    }
  }
  return point;
}

/*! \internal
  
  Generates the polygon needed for drawing channel fills between this graph (data passed via \a
  lineData) and the graph specified by mChannelFillGraph (data generated by calling its \ref
  getPlotData function). May return an empty polygon if the key ranges have no overlap or fill
  target graph and this graph don't have same orientation (i.e. both key axes horizontal or both
  key axes vertical). For increased performance (due to implicit sharing), keep the returned
  QPolygonF const.
*/
const QPolygonF QCPGraph::getChannelFillPolygon(const QVector<QPointF> *lineData) const
{
  if (!mChannelFillGraph)
    return QPolygonF();
  
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return QPolygonF(); }
  if (!mChannelFillGraph.data()->mKeyAxis) { qDebug() << Q_FUNC_INFO << "channel fill target key axis invalid"; return QPolygonF(); }
  
  if (mChannelFillGraph.data()->mKeyAxis.data()->orientation() != keyAxis->orientation())
    return QPolygonF(); // don't have same axis orientation, can't fill that (Note: if keyAxis fits, valueAxis will fit too, because it's always orthogonal to keyAxis)
  
  if (lineData->isEmpty()) return QPolygonF();
  QVector<QPointF> otherData;
  mChannelFillGraph.data()->getPlotData(&otherData, 0);
  if (otherData.isEmpty()) return QPolygonF();
  QVector<QPointF> thisData;
  thisData.reserve(lineData->size()+otherData.size()); // because we will join both vectors at end of this function
  for (int i=0; i<lineData->size(); ++i) // don't use the vector<<(vector),  it squeezes internally, which ruins the performance tuning with reserve()
    thisData << lineData->at(i);
  
  // pointers to be able to swap them, depending which data range needs cropping:
  QVector<QPointF> *staticData = &thisData;
  QVector<QPointF> *croppedData = &otherData;
  
  // crop both vectors to ranges in which the keys overlap (which coord is key, depends on axisType):
  if (keyAxis->orientation() == Qt::Horizontal)
  {
    // x is key
    // if an axis range is reversed, the data point keys will be descending. Reverse them, since following algorithm assumes ascending keys:
    if (staticData->first().x() > staticData->last().x())
    {
      int size = staticData->size();
      for (int i=0; i<size/2; ++i)
        qSwap((*staticData)[i], (*staticData)[size-1-i]);
    }
    if (croppedData->first().x() > croppedData->last().x())
    {
      int size = croppedData->size();
      for (int i=0; i<size/2; ++i)
        qSwap((*croppedData)[i], (*croppedData)[size-1-i]);
    }
    // crop lower bound:
    if (staticData->first().x() < croppedData->first().x()) // other one must be cropped
      qSwap(staticData, croppedData);
    int lowBound = findIndexBelowX(croppedData, staticData->first().x());
    if (lowBound == -1) return QPolygonF(); // key ranges have no overlap
    croppedData->remove(0, lowBound);
    // set lowest point of cropped data to fit exactly key position of first static data
    // point via linear interpolation:
    if (croppedData->size() < 2) return QPolygonF(); // need at least two points for interpolation
    double slope;
    if (croppedData->at(1).x()-croppedData->at(0).x() != 0)
      slope = (croppedData->at(1).y()-croppedData->at(0).y())/(croppedData->at(1).x()-croppedData->at(0).x());
    else
      slope = 0;
    (*croppedData)[0].setY(croppedData->at(0).y()+slope*(staticData->first().x()-croppedData->at(0).x()));
    (*croppedData)[0].setX(staticData->first().x());
    
    // crop upper bound:
    if (staticData->last().x() > croppedData->last().x()) // other one must be cropped
      qSwap(staticData, croppedData);
    int highBound = findIndexAboveX(croppedData, staticData->last().x());
    if (highBound == -1) return QPolygonF(); // key ranges have no overlap
    croppedData->remove(highBound+1, croppedData->size()-(highBound+1));
    // set highest point of cropped data to fit exactly key position of last static data
    // point via linear interpolation:
    if (croppedData->size() < 2) return QPolygonF(); // need at least two points for interpolation
    int li = croppedData->size()-1; // last index
    if (croppedData->at(li).x()-croppedData->at(li-1).x() != 0)
      slope = (croppedData->at(li).y()-croppedData->at(li-1).y())/(croppedData->at(li).x()-croppedData->at(li-1).x());
    else
      slope = 0;
    (*croppedData)[li].setY(croppedData->at(li-1).y()+slope*(staticData->last().x()-croppedData->at(li-1).x()));
    (*croppedData)[li].setX(staticData->last().x());
  } else // mKeyAxis->orientation() == Qt::Vertical
  {
    // y is key
    // similar to "x is key" but switched x,y. Further, lower/upper meaning is inverted compared to x,
    // because in pixel coordinates, y increases from top to bottom, not bottom to top like data coordinate.
    // if an axis range is reversed, the data point keys will be descending. Reverse them, since following algorithm assumes ascending keys:
    if (staticData->first().y() < staticData->last().y())
    {
      int size = staticData->size();
      for (int i=0; i<size/2; ++i)
        qSwap((*staticData)[i], (*staticData)[size-1-i]);
    }
    if (croppedData->first().y() < croppedData->last().y())
    {
      int size = croppedData->size();
      for (int i=0; i<size/2; ++i)
        qSwap((*croppedData)[i], (*croppedData)[size-1-i]);
    }
    // crop lower bound:
    if (staticData->first().y() > croppedData->first().y()) // other one must be cropped
      qSwap(staticData, croppedData);
    int lowBound = findIndexAboveY(croppedData, staticData->first().y());
    if (lowBound == -1) return QPolygonF(); // key ranges have no overlap
    croppedData->remove(0, lowBound);
    // set lowest point of cropped data to fit exactly key position of first static data
    // point via linear interpolation:
    if (croppedData->size() < 2) return QPolygonF(); // need at least two points for interpolation
    double slope;
    if (croppedData->at(1).y()-croppedData->at(0).y() != 0) // avoid division by zero in step plots
      slope = (croppedData->at(1).x()-croppedData->at(0).x())/(croppedData->at(1).y()-croppedData->at(0).y());
    else
      slope = 0;
    (*croppedData)[0].setX(croppedData->at(0).x()+slope*(staticData->first().y()-croppedData->at(0).y()));
    (*croppedData)[0].setY(staticData->first().y());
    
    // crop upper bound:
    if (staticData->last().y() < croppedData->last().y()) // other one must be cropped
      qSwap(staticData, croppedData);
    int highBound = findIndexBelowY(croppedData, staticData->last().y());
    if (highBound == -1) return QPolygonF(); // key ranges have no overlap
    croppedData->remove(highBound+1, croppedData->size()-(highBound+1));
    // set highest point of cropped data to fit exactly key position of last static data
    // point via linear interpolation:
    if (croppedData->size() < 2) return QPolygonF(); // need at least two points for interpolation
    int li = croppedData->size()-1; // last index
    if (croppedData->at(li).y()-croppedData->at(li-1).y() != 0) // avoid division by zero in step plots
      slope = (croppedData->at(li).x()-croppedData->at(li-1).x())/(croppedData->at(li).y()-croppedData->at(li-1).y());
    else
      slope = 0;
    (*croppedData)[li].setX(croppedData->at(li-1).x()+slope*(staticData->last().y()-croppedData->at(li-1).y()));
    (*croppedData)[li].setY(staticData->last().y());
  }
  
  // return joined:
  for (int i=otherData.size()-1; i>=0; --i) // insert reversed, otherwise the polygon will be twisted
    thisData << otherData.at(i);
  return QPolygonF(thisData);
}

/*! \internal
  
  Finds the smallest index of \a data, whose points x value is just above \a x. Assumes x values in
  \a data points are ordered ascending, as is the case when plotting with horizontal key axis.

  Used to calculate the channel fill polygon, see \ref getChannelFillPolygon.
*/
int QCPGraph::findIndexAboveX(const QVector<QPointF> *data, double x) const
{
  for (int i=data->size()-1; i>=0; --i)
  {
    if (data->at(i).x() < x)
    {
      if (i<data->size()-1)
        return i+1;
      else
        return data->size()-1;
    }
  }
  return -1;
}

/*! \internal
  
  Finds the highest index of \a data, whose points x value is just below \a x. Assumes x values in
  \a data points are ordered ascending, as is the case when plotting with horizontal key axis.
  
  Used to calculate the channel fill polygon, see \ref getChannelFillPolygon.
*/
int QCPGraph::findIndexBelowX(const QVector<QPointF> *data, double x) const
{
  for (int i=0; i<data->size(); ++i)
  {
    if (data->at(i).x() > x)
    {
      if (i>0)
        return i-1;
      else
        return 0;
    }
  }
  return -1;
}

/*! \internal
  
  Finds the smallest index of \a data, whose points y value is just above \a y. Assumes y values in
  \a data points are ordered descending, as is the case when plotting with vertical key axis.
  
  Used to calculate the channel fill polygon, see \ref getChannelFillPolygon.
*/
int QCPGraph::findIndexAboveY(const QVector<QPointF> *data, double y) const
{
  for (int i=0; i<data->size(); ++i)
  {
    if (data->at(i).y() < y)
    {
      if (i>0)
        return i-1;
      else
        return 0;
    }
  }
  return -1;
}

/*! \internal
  
  Calculates the (minimum) distance (in pixels) the graph's representation has from the given \a
  pixelPoint in pixels. This is used to determine whether the graph was clicked or not, e.g. in
  \ref selectTest.
  
  If either the graph has no data or if the line style is \ref lsNone and the scatter style's shape
  is \ref QCPScatterStyle::ssNone (i.e. there is no visual representation of the graph), returns
  500.
*/
double QCPGraph::pointDistance(const QPointF &pixelPoint) const
{
  if (mData->isEmpty())
  {
    qDebug() << Q_FUNC_INFO << "requested point distance on graph" << mName << "without data";
    return 500;
  }
  if (mData->size() == 1)
  {
    QPointF dataPoint = coordsToPixels(mData->constBegin().key(), mData->constBegin().value().value);
    return QVector2D(dataPoint-pixelPoint).length();
  }
  
  if (mLineStyle == lsNone && mScatterStyle.isNone())
    return 500;
  
  // calculate minimum distances to graph representation:
  if (mLineStyle == lsNone)
  {
    // no line displayed, only calculate distance to scatter points:
    QVector<QCPData> *scatterData = new QVector<QCPData>;
    getScatterPlotData(scatterData);
    double minDistSqr = std::numeric_limits<double>::max();
    QPointF ptA;
    QPointF ptB = coordsToPixels(scatterData->at(0).key, scatterData->at(0).value); // getScatterPlotData returns in plot coordinates, so transform to pixels
    for (int i=1; i<scatterData->size(); ++i)
    {
      ptA = ptB;
      ptB = coordsToPixels(scatterData->at(i).key, scatterData->at(i).value);
      double currentDistSqr = distSqrToLine(ptA, ptB, pixelPoint);
      if (currentDistSqr < minDistSqr)
        minDistSqr = currentDistSqr;
    }
    delete scatterData;
    return sqrt(minDistSqr);
  } else
  {
    // line displayed calculate distance to line segments:
    QVector<QPointF> *lineData = new QVector<QPointF>;
    getPlotData(lineData, 0); // unlike with getScatterPlotData we get pixel coordinates here
    double minDistSqr = std::numeric_limits<double>::max();
    if (mLineStyle == lsImpulse)
    {
      // impulse plot differs from other line styles in that the lineData points are only pairwise connected:
      for (int i=0; i<lineData->size()-1; i+=2) // iterate pairs
      {
        double currentDistSqr = distSqrToLine(lineData->at(i), lineData->at(i+1), pixelPoint);
        if (currentDistSqr < minDistSqr)
          minDistSqr = currentDistSqr;
      }
    } else
    {
      // all other line plots (line and step) connect points directly:
      for (int i=0; i<lineData->size()-1; ++i)
      {
        double currentDistSqr = distSqrToLine(lineData->at(i), lineData->at(i+1), pixelPoint);
        if (currentDistSqr < minDistSqr)
          minDistSqr = currentDistSqr;
      }
    }
    delete lineData;
    return sqrt(minDistSqr);
  }
}

/*! \internal
  
  Finds the highest index of \a data, whose points y value is just below \a y. Assumes y values in
  \a data points are ordered descending, as is the case when plotting with vertical key axis (since
  keys are ordered ascending).

  Used to calculate the channel fill polygon, see \ref getChannelFillPolygon.
*/
int QCPGraph::findIndexBelowY(const QVector<QPointF> *data, double y) const
{
  for (int i=data->size()-1; i>=0; --i)
  {
    if (data->at(i).y() > y)
    {
      if (i<data->size()-1)
        return i+1;
      else
        return data->size()-1;
    }
  }
  return -1;
}

/* inherits documentation from base class */
QCPRange QCPGraph::getKeyRange(bool &foundRange, SignDomain inSignDomain) const
{
  // just call the specialized version which takes an additional argument whether error bars
  // should also be taken into consideration for range calculation. We set this to true here.
  return getKeyRange(foundRange, inSignDomain, true);
}

/* inherits documentation from base class */
QCPRange QCPGraph::getValueRange(bool &foundRange, SignDomain inSignDomain) const
{
  // just call the specialized version which takes an additional argument whether error bars
  // should also be taken into consideration for range calculation. We set this to true here.
  return getValueRange(foundRange, inSignDomain, true);
}

/*! \overload
  
  Allows to specify whether the error bars should be included in the range calculation.
  
  \see getKeyRange(bool &foundRange, SignDomain inSignDomain)
*/
QCPRange QCPGraph::getKeyRange(bool &foundRange, SignDomain inSignDomain, bool includeErrors) const
{
  QCPRange range;
  bool haveLower = false;
  bool haveUpper = false;
  
  double current, currentErrorMinus, currentErrorPlus;
  
  if (inSignDomain == sdBoth) // range may be anywhere
  {
    QCPDataMap::const_iterator it = mData->constBegin();
    while (it != mData->constEnd())
    {
      current = it.value().key;
      currentErrorMinus = (includeErrors ? it.value().keyErrorMinus : 0);
      currentErrorPlus = (includeErrors ? it.value().keyErrorPlus : 0);
      if (current-currentErrorMinus < range.lower || !haveLower)
      {
        range.lower = current-currentErrorMinus;
        haveLower = true;
      }
      if (current+currentErrorPlus > range.upper || !haveUpper)
      {
        range.upper = current+currentErrorPlus;
        haveUpper = true;
      }
      ++it;
    }
  } else if (inSignDomain == sdNegative) // range may only be in the negative sign domain
  {
    QCPDataMap::const_iterator it = mData->constBegin();
    while (it != mData->constEnd())
    {
      current = it.value().key;
      currentErrorMinus = (includeErrors ? it.value().keyErrorMinus : 0);
      currentErrorPlus = (includeErrors ? it.value().keyErrorPlus : 0);
      if ((current-currentErrorMinus < range.lower || !haveLower) && current-currentErrorMinus < 0)
      {
        range.lower = current-currentErrorMinus;
        haveLower = true;
      }
      if ((current+currentErrorPlus > range.upper || !haveUpper) && current+currentErrorPlus < 0)
      {
        range.upper = current+currentErrorPlus;
        haveUpper = true;
      }
      if (includeErrors) // in case point is in valid sign domain but errobars stretch beyond it, we still want to geht that point.
      {
        if ((current < range.lower || !haveLower) && current < 0)
        {
          range.lower = current;
          haveLower = true;
        }
        if ((current > range.upper || !haveUpper) && current < 0)
        {
          range.upper = current;
          haveUpper = true;
        }
      }
      ++it;
    }
  } else if (inSignDomain == sdPositive) // range may only be in the positive sign domain
  {
    QCPDataMap::const_iterator it = mData->constBegin();
    while (it != mData->constEnd())
    {
      current = it.value().key;
      currentErrorMinus = (includeErrors ? it.value().keyErrorMinus : 0);
      currentErrorPlus = (includeErrors ? it.value().keyErrorPlus : 0);
      if ((current-currentErrorMinus < range.lower || !haveLower) && current-currentErrorMinus > 0)
      {
        range.lower = current-currentErrorMinus;
        haveLower = true;
      }
      if ((current+currentErrorPlus > range.upper || !haveUpper) && current+currentErrorPlus > 0)
      {
        range.upper = current+currentErrorPlus;
        haveUpper = true;
      }
      if (includeErrors) // in case point is in valid sign domain but errobars stretch beyond it, we still want to get that point.
      {
        if ((current < range.lower || !haveLower) && current > 0)
        {
          range.lower = current;
          haveLower = true;
        }
        if ((current > range.upper || !haveUpper) && current > 0)
        {
          range.upper = current;
          haveUpper = true;
        }
      }
      ++it;
    }
  }
  
  foundRange = haveLower && haveUpper;
  return range;
}

/*! \overload
  
  Allows to specify whether the error bars should be included in the range calculation.
  
  \see getValueRange(bool &foundRange, SignDomain inSignDomain)
*/
QCPRange QCPGraph::getValueRange(bool &foundRange, SignDomain inSignDomain, bool includeErrors) const
{
  QCPRange range;
  bool haveLower = false;
  bool haveUpper = false;
  
  double current, currentErrorMinus, currentErrorPlus;
  
  if (inSignDomain == sdBoth) // range may be anywhere
  {
    QCPDataMap::const_iterator it = mData->constBegin();
    while (it != mData->constEnd())
    {
      current = it.value().value;
      currentErrorMinus = (includeErrors ? it.value().valueErrorMinus : 0);
      currentErrorPlus = (includeErrors ? it.value().valueErrorPlus : 0);
      if (current-currentErrorMinus < range.lower || !haveLower)
      {
        range.lower = current-currentErrorMinus;
        haveLower = true;
      }
      if (current+currentErrorPlus > range.upper || !haveUpper)
      {
        range.upper = current+currentErrorPlus;
        haveUpper = true;
      }
      ++it;
    }
  } else if (inSignDomain == sdNegative) // range may only be in the negative sign domain
  {
    QCPDataMap::const_iterator it = mData->constBegin();
    while (it != mData->constEnd())
    {
      current = it.value().value;
      currentErrorMinus = (includeErrors ? it.value().valueErrorMinus : 0);
      currentErrorPlus = (includeErrors ? it.value().valueErrorPlus : 0);
      if ((current-currentErrorMinus < range.lower || !haveLower) && current-currentErrorMinus < 0)
      {
        range.lower = current-currentErrorMinus;
        haveLower = true;
      }
      if ((current+currentErrorPlus > range.upper || !haveUpper) && current+currentErrorPlus < 0)
      {
        range.upper = current+currentErrorPlus;
        haveUpper = true;
      }
      if (includeErrors) // in case point is in valid sign domain but errobars stretch beyond it, we still want to get that point.
      {
        if ((current < range.lower || !haveLower) && current < 0)
        {
          range.lower = current;
          haveLower = true;
        }
        if ((current > range.upper || !haveUpper) && current < 0)
        {
          range.upper = current;
          haveUpper = true;
        }
      }
      ++it;
    }
  } else if (inSignDomain == sdPositive) // range may only be in the positive sign domain
  {
    QCPDataMap::const_iterator it = mData->constBegin();
    while (it != mData->constEnd())
    {
      current = it.value().value;
      currentErrorMinus = (includeErrors ? it.value().valueErrorMinus : 0);
      currentErrorPlus = (includeErrors ? it.value().valueErrorPlus : 0);
      if ((current-currentErrorMinus < range.lower || !haveLower) && current-currentErrorMinus > 0)
      {
        range.lower = current-currentErrorMinus;
        haveLower = true;
      }
      if ((current+currentErrorPlus > range.upper || !haveUpper) && current+currentErrorPlus > 0)
      {
        range.upper = current+currentErrorPlus;
        haveUpper = true;
      }
      if (includeErrors) // in case point is in valid sign domain but errobars stretch beyond it, we still want to geht that point.
      {
        if ((current < range.lower || !haveLower) && current > 0)
        {
          range.lower = current;
          haveLower = true;
        }
        if ((current > range.upper || !haveUpper) && current > 0)
        {
          range.upper = current;
          haveUpper = true;
        }
      }
      ++it;
    }
  }
  
  foundRange = haveLower && haveUpper;
  return range;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPCurveData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPCurveData
  \brief Holds the data of one single data point for QCPCurve.
  
  The container for storing multiple data points is \ref QCPCurveDataMap.
  
  The stored data is:
  \li \a t: the free parameter of the curve at this curve point (cp. the mathematical vector <em>(x(t), y(t))</em>)
  \li \a key: coordinate on the key axis of this curve point
  \li \a value: coordinate on the value axis of this curve point
  
  \see QCPCurveDataMap
*/

/*!
  Constructs a curve data point with t, key and value set to zero.
*/
QCPCurveData::QCPCurveData() :
  t(0),
  key(0),
  value(0)
{
}

/*!
  Constructs a curve data point with the specified \a t, \a key and \a value.
*/
QCPCurveData::QCPCurveData(double t, double key, double value) :
  t(t),
  key(key),
  value(value)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPCurve
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPCurve
  \brief A plottable representing a parametric curve in a plot.
  
  \image html QCPCurve.png
  
  Unlike QCPGraph, plottables of this type may have multiple points with the same key coordinate,
  so their visual representation can have \a loops. This is realized by introducing a third
  coordinate \a t, which defines the order of the points described by the other two coordinates \a
  x and \a y.

  To plot data, assign it with the \ref setData or \ref addData functions.
  
  \section appearance Changing the appearance
  
  The appearance of the curve is determined by the pen and the brush (\ref setPen, \ref setBrush).
  \section usage Usage
  
  Like all data representing objects in QCustomPlot, the QCPCurve is a plottable (QCPAbstractPlottable). So
  the plottable-interface of QCustomPlot applies (QCustomPlot::plottable, QCustomPlot::addPlottable, QCustomPlot::removePlottable, etc.)
  
  Usually, you first create an instance:
  \code
  QCPCurve *newCurve = new QCPCurve(customPlot->xAxis, customPlot->yAxis);\endcode
  add it to the customPlot with QCustomPlot::addPlottable:
  \code
  customPlot->addPlottable(newCurve);\endcode
  and then modify the properties of the newly created plottable, e.g.:
  \code
  newCurve->setName("Fermat's Spiral");
  newCurve->setData(tData, xData, yData);\endcode
*/

/*!
  Constructs a curve which uses \a keyAxis as its key axis ("x") and \a valueAxis as its value
  axis ("y"). \a keyAxis and \a valueAxis must reside in the same QCustomPlot instance and not have
  the same orientation. If either of these restrictions is violated, a corresponding message is
  printed to the debug output (qDebug), the construction is not aborted, though.
  
  The constructed QCPCurve can be added to the plot with QCustomPlot::addPlottable, QCustomPlot
  then takes ownership of the graph.
*/
QCPCurve::QCPCurve(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis)
{
  mData = new QCPCurveDataMap;
  mPen.setColor(Qt::blue);
  mPen.setStyle(Qt::SolidLine);
  mBrush.setColor(Qt::blue);
  mBrush.setStyle(Qt::NoBrush);
  mSelectedPen = mPen;
  mSelectedPen.setWidthF(2.5);
  mSelectedPen.setColor(QColor(80, 80, 255)); // lighter than Qt::blue of mPen
  mSelectedBrush = mBrush;
  
  setScatterStyle(QCPScatterStyle());
  setLineStyle(lsLine);
}

QCPCurve::~QCPCurve()
{
  delete mData;
}

/*!
  Replaces the current data with the provided \a data.
  
  If \a copy is set to true, data points in \a data will only be copied. if false, the plottable
  takes ownership of the passed data and replaces the internal data pointer with it. This is
  significantly faster than copying for large datasets.
*/
void QCPCurve::setData(QCPCurveDataMap *data, bool copy)
{
  if (copy)
  {
    *mData = *data;
  } else
  {
    delete mData;
    mData = data;
  }
}

/*! \overload
  
  Replaces the current data with the provided points in \a t, \a key and \a value tuples. The
  provided vectors should have equal length. Else, the number of added points will be the size of
  the smallest vector.
*/
void QCPCurve::setData(const QVector<double> &t, const QVector<double> &key, const QVector<double> &value)
{
  mData->clear();
  int n = t.size();
  n = qMin(n, key.size());
  n = qMin(n, value.size());
  QCPCurveData newData;
  for (int i=0; i<n; ++i)
  {
    newData.t = t[i];
    newData.key = key[i];
    newData.value = value[i];
    mData->insertMulti(newData.t, newData);
  }
}

/*! \overload
  
  Replaces the current data with the provided \a key and \a value pairs. The t parameter
  of each data point will be set to the integer index of the respective key/value pair.
*/
void QCPCurve::setData(const QVector<double> &key, const QVector<double> &value)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, value.size());
  QCPCurveData newData;
  for (int i=0; i<n; ++i)
  {
    newData.t = i; // no t vector given, so we assign t the index of the key/value pair
    newData.key = key[i];
    newData.value = value[i];
    mData->insertMulti(newData.t, newData);
  }
}

/*!
  Sets the visual appearance of single data points in the plot. If set to \ref
  QCPScatterStyle::ssNone, no scatter points are drawn (e.g. for line-only plots with appropriate
  line style).
  
  \see QCPScatterStyle, setLineStyle
*/
void QCPCurve::setScatterStyle(const QCPScatterStyle &style)
{
  mScatterStyle = style;
}

/*!
  Sets how the single data points are connected in the plot or how they are represented visually
  apart from the scatter symbol. For scatter-only plots, set \a style to \ref lsNone and \ref
  setScatterStyle to the desired scatter style.
  
  \see setScatterStyle
*/
void QCPCurve::setLineStyle(QCPCurve::LineStyle style)
{
  mLineStyle = style;
}

/*!
  Adds the provided data points in \a dataMap to the current data.
  \see removeData
*/
void QCPCurve::addData(const QCPCurveDataMap &dataMap)
{
  mData->unite(dataMap);
}

/*! \overload
  Adds the provided single data point in \a data to the current data.
  \see removeData
*/
void QCPCurve::addData(const QCPCurveData &data)
{
  mData->insertMulti(data.t, data);
}

/*! \overload
  Adds the provided single data point as \a t, \a key and \a value tuple to the current data
  \see removeData
*/
void QCPCurve::addData(double t, double key, double value)
{
  QCPCurveData newData;
  newData.t = t;
  newData.key = key;
  newData.value = value;
  mData->insertMulti(newData.t, newData);
}

/*! \overload
  
  Adds the provided single data point as \a key and \a value pair to the current data The t
  parameter of the data point is set to the t of the last data point plus 1. If there is no last
  data point, t will be set to 0.
  
  \see removeData
*/
void QCPCurve::addData(double key, double value)
{
  QCPCurveData newData;
  if (!mData->isEmpty())
    newData.t = (mData->constEnd()-1).key()+1;
  else
    newData.t = 0;
  newData.key = key;
  newData.value = value;
  mData->insertMulti(newData.t, newData);
}

/*! \overload
  Adds the provided data points as \a t, \a key and \a value tuples to the current data.
  \see removeData
*/
void QCPCurve::addData(const QVector<double> &ts, const QVector<double> &keys, const QVector<double> &values)
{
  int n = ts.size();
  n = qMin(n, keys.size());
  n = qMin(n, values.size());
  QCPCurveData newData;
  for (int i=0; i<n; ++i)
  {
    newData.t = ts[i];
    newData.key = keys[i];
    newData.value = values[i];
    mData->insertMulti(newData.t, newData);
  }
}

/*!
  Removes all data points with curve parameter t smaller than \a t.
  \see addData, clearData
*/
void QCPCurve::removeDataBefore(double t)
{
  QCPCurveDataMap::iterator it = mData->begin();
  while (it != mData->end() && it.key() < t)
    it = mData->erase(it);
}

/*!
  Removes all data points with curve parameter t greater than \a t.
  \see addData, clearData
*/
void QCPCurve::removeDataAfter(double t)
{
  if (mData->isEmpty()) return;
  QCPCurveDataMap::iterator it = mData->upperBound(t);
  while (it != mData->end())
    it = mData->erase(it);
}

/*!
  Removes all data points with curve parameter t between \a fromt and \a tot. if \a fromt is
  greater or equal to \a tot, the function does nothing. To remove a single data point with known
  t, use \ref removeData(double t).
  
  \see addData, clearData
*/
void QCPCurve::removeData(double fromt, double tot)
{
  if (fromt >= tot || mData->isEmpty()) return;
  QCPCurveDataMap::iterator it = mData->upperBound(fromt);
  QCPCurveDataMap::iterator itEnd = mData->upperBound(tot);
  while (it != itEnd)
    it = mData->erase(it);
}

/*! \overload
  
  Removes a single data point at curve parameter \a t. If the position is not known with absolute
  precision, consider using \ref removeData(double fromt, double tot) with a small fuzziness
  interval around the suspected position, depeding on the precision with which the curve parameter
  is known.
  
  \see addData, clearData
*/
void QCPCurve::removeData(double t)
{
  mData->remove(t);
}

/*!
  Removes all data points.
  \see removeData, removeDataAfter, removeDataBefore
*/
void QCPCurve::clearData()
{
  mData->clear();
}

/* inherits documentation from base class */
double QCPCurve::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if ((onlySelectable && !mSelectable) || mData->isEmpty())
    return -1;
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return -1; }
  
  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()))
    return pointDistance(pos);
  else
    return -1;
}

/* inherits documentation from base class */
void QCPCurve::draw(QCPPainter *painter)
{
  if (mData->isEmpty()) return;
  
  // allocate line vector:
  QVector<QPointF> *lineData = new QVector<QPointF>;
  
  // fill with curve data:
  getCurveData(lineData);
  
  // check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
  QCPCurveDataMap::const_iterator it;
  for (it = mData->constBegin(); it != mData->constEnd(); ++it)
  {
    if (QCP::isInvalidData(it.value().t) ||
        QCP::isInvalidData(it.value().key, it.value().value))
      qDebug() << Q_FUNC_INFO << "Data point at" << it.key() << "invalid." << "Plottable name:" << name();
  }
#endif
  
  // draw curve fill:
  if (mainBrush().style() != Qt::NoBrush && mainBrush().color().alpha() != 0)
  {
    applyFillAntialiasingHint(painter);
    painter->setPen(Qt::NoPen);
    painter->setBrush(mainBrush());
    painter->drawPolygon(QPolygonF(*lineData));
  }
  
  // draw curve line:
  if (mLineStyle != lsNone && mainPen().style() != Qt::NoPen && mainPen().color().alpha() != 0)
  {
    applyDefaultAntialiasingHint(painter);
    painter->setPen(mainPen());
    painter->setBrush(Qt::NoBrush);
    // if drawing solid line and not in PDF, use much faster line drawing instead of polyline:
    if (mParentPlot->plottingHints().testFlag(QCP::phFastPolylines) &&
        painter->pen().style() == Qt::SolidLine &&
        !painter->modes().testFlag(QCPPainter::pmVectorized) &&
        !painter->modes().testFlag(QCPPainter::pmNoCaching))
    {
      for (int i=1; i<lineData->size(); ++i)
        painter->drawLine(lineData->at(i-1), lineData->at(i));
    } else
    {
      painter->drawPolyline(QPolygonF(*lineData));
    }
  }
  
  // draw scatters:
  if (!mScatterStyle.isNone())
    drawScatterPlot(painter, lineData);
  
  // free allocated line data:
  delete lineData;
}

/* inherits documentation from base class */
void QCPCurve::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
  // draw fill:
  if (mBrush.style() != Qt::NoBrush)
  {
    applyFillAntialiasingHint(painter);
    painter->fillRect(QRectF(rect.left(), rect.top()+rect.height()/2.0, rect.width(), rect.height()/3.0), mBrush);
  }
  // draw line vertically centered:
  if (mLineStyle != lsNone)
  {
    applyDefaultAntialiasingHint(painter);
    painter->setPen(mPen);
    painter->drawLine(QLineF(rect.left(), rect.top()+rect.height()/2.0, rect.right()+5, rect.top()+rect.height()/2.0)); // +5 on x2 else last segment is missing from dashed/dotted pens
  }
  // draw scatter symbol:
  if (!mScatterStyle.isNone())
  {
    applyScattersAntialiasingHint(painter);
    // scale scatter pixmap if it's too large to fit in legend icon rect:
    if (mScatterStyle.shape() == QCPScatterStyle::ssPixmap && (mScatterStyle.pixmap().size().width() > rect.width() || mScatterStyle.pixmap().size().height() > rect.height()))
    {
      QCPScatterStyle scaledStyle(mScatterStyle);
      scaledStyle.setPixmap(scaledStyle.pixmap().scaled(rect.size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
      scaledStyle.applyTo(painter, mPen);
      scaledStyle.drawShape(painter, QRectF(rect).center());
    } else
    {
      mScatterStyle.applyTo(painter, mPen);
      mScatterStyle.drawShape(painter, QRectF(rect).center());
    }
  }
}

/*! \internal
  
  Draws scatter symbols at every data point passed in \a pointData. scatter symbols are independent of
  the line style and are always drawn if scatter shape is not \ref QCPScatterStyle::ssNone.
*/
void QCPCurve::drawScatterPlot(QCPPainter *painter, const QVector<QPointF> *pointData) const
{
  // draw scatter point symbols:
  applyScattersAntialiasingHint(painter);
  mScatterStyle.applyTo(painter, mPen);
  for (int i=0; i<pointData->size(); ++i)
    mScatterStyle.drawShape(painter,  pointData->at(i));
}

/*! \internal
  
  called by QCPCurve::draw to generate a point vector (pixels) which represents the line of the
  curve. Line segments that aren't visible in the current axis rect are handled in an optimized
  way.
*/
void QCPCurve::getCurveData(QVector<QPointF> *lineData) const
{
  /* Extended sides of axis rect R divide space into 9 regions:
     1__|_4_|__7
     2__|_R_|__8
     3  | 6 |  9
     General idea: If the two points of a line segment are in the same region (that is not R), the line segment corner is removed.
     Curves outside R become straight lines closely outside of R which greatly reduces drawing time, yet keeps the look of lines and
     fills inside R consistent.
     The region R has index 5.
  */
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  
  QRect axisRect = mKeyAxis.data()->axisRect()->rect() & mValueAxis.data()->axisRect()->rect();
  lineData->reserve(mData->size());
  QCPCurveDataMap::const_iterator it;
  int lastRegion = 5;
  int currentRegion = 5;
  double RLeft = keyAxis->range().lower;
  double RRight = keyAxis->range().upper;
  double RBottom = valueAxis->range().lower;
  double RTop = valueAxis->range().upper;
  double x, y; // current key/value
  bool addedLastAlready = true;
  bool firstPoint = true; // first point must always be drawn, to make sure fill works correctly
  for (it = mData->constBegin(); it != mData->constEnd(); ++it)
  {
    x = it.value().key;
    y = it.value().value;
    // determine current region:
    if (x < RLeft) // region 123
    {
      if (y > RTop)
        currentRegion = 1;
      else if (y < RBottom)
        currentRegion = 3;
      else
        currentRegion = 2;
    } else if (x > RRight) // region 789
    {
      if (y > RTop)
        currentRegion = 7;
      else if (y < RBottom)
        currentRegion = 9;
      else
        currentRegion = 8;
    } else // region 456
    {
      if (y > RTop)
        currentRegion = 4;
      else if (y < RBottom)
        currentRegion = 6;
      else
        currentRegion = 5;
    }
    
    /*
      Watch out, the next part is very tricky. It modifies the curve such that it seems like the
      whole thing is still drawn, but actually the points outside the axisRect are simplified
      ("optimized") greatly. There are some subtle special cases when line segments are large and
      thereby each subsequent point may be in a different region or even skip some.
    */
    // determine whether to keep current point:
    if (currentRegion == 5 || (firstPoint && mBrush.style() != Qt::NoBrush)) // current is in R, add current and last if it wasn't added already
    {
      if (!addedLastAlready) // in case curve just entered R, make sure the last point outside R is also drawn correctly
        lineData->append(coordsToPixels((it-1).value().key, (it-1).value().value)); // add last point to vector
      else if (lastRegion != 5) // added last already. If that's the case, we probably added it at optimized position. So go back and make sure it's at original position (else the angle changes under which this segment enters R)
      {
        if (!firstPoint) // because on firstPoint, currentRegion is 5 and addedLastAlready is true, although there is no last point
          lineData->replace(lineData->size()-1, coordsToPixels((it-1).value().key, (it-1).value().value));
      }
      lineData->append(coordsToPixels(it.value().key, it.value().value)); // add current point to vector
      addedLastAlready = true; // so in next iteration, we don't add this point twice
    } else if (currentRegion != lastRegion) // changed region, add current and last if not added already
    {
      // using outsideCoordsToPixels instead of coorsToPixels for optimized point placement (places points just outside axisRect instead of potentially far away)
      
      // if we're coming from R or we skip diagonally over the corner regions (so line might still be visible in R), we can't place points optimized
      if (lastRegion == 5 || // coming from R
          ((lastRegion==2 && currentRegion==4) || (lastRegion==4 && currentRegion==2)) || // skip top left diagonal
          ((lastRegion==4 && currentRegion==8) || (lastRegion==8 && currentRegion==4)) || // skip top right diagonal
          ((lastRegion==8 && currentRegion==6) || (lastRegion==6 && currentRegion==8)) || // skip bottom right diagonal
          ((lastRegion==6 && currentRegion==2) || (lastRegion==2 && currentRegion==6))    // skip bottom left diagonal
          )
      {
        // always add last point if not added already, original:
        if (!addedLastAlready)
          lineData->append(coordsToPixels((it-1).value().key, (it-1).value().value));
        // add current point, original:
        lineData->append(coordsToPixels(it.value().key, it.value().value));
      } else // no special case that forbids optimized point placement, so do it:
      {
        // always add last point if not added already, optimized:
        if (!addedLastAlready)
          lineData->append(outsideCoordsToPixels((it-1).value().key, (it-1).value().value, currentRegion, axisRect));
        // add current point, optimized:
        lineData->append(outsideCoordsToPixels(it.value().key, it.value().value, currentRegion, axisRect));
      }
      addedLastAlready = true; // so that if next point enters 5, or crosses another region boundary, we don't add this point twice
    } else // neither in R, nor crossed a region boundary, skip current point
    {
      addedLastAlready = false;
    }
    lastRegion = currentRegion;
    firstPoint = false;
  }
  // If curve ends outside R, we want to add very last point so the fill looks like it should when the curve started inside R:
  if (lastRegion != 5 && mBrush.style() != Qt::NoBrush && !mData->isEmpty())
    lineData->append(coordsToPixels((mData->constEnd()-1).value().key, (mData->constEnd()-1).value().value));
}

/*! \internal
  
  Calculates the (minimum) distance (in pixels) the curve's representation has from the given \a
  pixelPoint in pixels. This is used to determine whether the curve was clicked or not, e.g. in
  \ref selectTest.
*/
double QCPCurve::pointDistance(const QPointF &pixelPoint) const
{
  if (mData->isEmpty())
  {
    qDebug() << Q_FUNC_INFO << "requested point distance on curve" << mName << "without data";
    return 500;
  }
  if (mData->size() == 1)
  {
    QPointF dataPoint = coordsToPixels(mData->constBegin().key(), mData->constBegin().value().value);
    return QVector2D(dataPoint-pixelPoint).length();
  }
  
  // calculate minimum distance to line segments:
  QVector<QPointF> *lineData = new QVector<QPointF>;
  getCurveData(lineData);
  double minDistSqr = std::numeric_limits<double>::max();
  for (int i=0; i<lineData->size()-1; ++i)
  {
    double currentDistSqr = distSqrToLine(lineData->at(i), lineData->at(i+1), pixelPoint);
    if (currentDistSqr < minDistSqr)
      minDistSqr = currentDistSqr;
  }
  delete lineData;
  return sqrt(minDistSqr);
}

/*! \internal
  
  This is a specialized \ref coordsToPixels function for points that are outside the visible
  axisRect and just crossing a boundary (since \ref getCurveData reduces non-visible curve segments
  to those line segments that cross region boundaries, see documentation there). It only uses the
  coordinate parallel to the region boundary of the axisRect. The other coordinate is picked just
  outside the axisRect (how far is determined by the scatter size and the line width). Together
  with the optimization in \ref getCurveData this improves performance for large curves (or zoomed
  in ones) significantly while keeping the illusion the whole curve and its filling is still being
  drawn for the viewer.
*/
QPointF QCPCurve::outsideCoordsToPixels(double key, double value, int region, QRect axisRect) const
{
  int margin = qCeil(qMax(mScatterStyle.size(), (double)mPen.widthF())) + 2;
  QPointF result = coordsToPixels(key, value);
  switch (region)
  {
    case 2: result.setX(axisRect.left()-margin); break; // left
    case 8: result.setX(axisRect.right()+margin); break; // right
    case 4: result.setY(axisRect.top()-margin); break; // top
    case 6: result.setY(axisRect.bottom()+margin); break; // bottom
    case 1: result.setX(axisRect.left()-margin);
            result.setY(axisRect.top()-margin); break; // top left
    case 7: result.setX(axisRect.right()+margin);
            result.setY(axisRect.top()-margin); break; // top right
    case 9: result.setX(axisRect.right()+margin);
            result.setY(axisRect.bottom()+margin); break; // bottom right
    case 3: result.setX(axisRect.left()-margin);
            result.setY(axisRect.bottom()+margin); break; // bottom left
  }
  return result;
}

/* inherits documentation from base class */
QCPRange QCPCurve::getKeyRange(bool &foundRange, SignDomain inSignDomain) const
{
  QCPRange range;
  bool haveLower = false;
  bool haveUpper = false;
  
  double current;
  
  QCPCurveDataMap::const_iterator it = mData->constBegin();
  while (it != mData->constEnd())
  {
    current = it.value().key;
    if (inSignDomain == sdBoth || (inSignDomain == sdNegative && current < 0) || (inSignDomain == sdPositive && current > 0))
    {
      if (current < range.lower || !haveLower)
      {
        range.lower = current;
        haveLower = true;
      }
      if (current > range.upper || !haveUpper)
      {
        range.upper = current;
        haveUpper = true;
      }
    }
    ++it;
  }
  
  foundRange = haveLower && haveUpper;
  return range;
}

/* inherits documentation from base class */
QCPRange QCPCurve::getValueRange(bool &foundRange, SignDomain inSignDomain) const
{
  QCPRange range;
  bool haveLower = false;
  bool haveUpper = false;
  
  double current;
  
  QCPCurveDataMap::const_iterator it = mData->constBegin();
  while (it != mData->constEnd())
  {
    current = it.value().value;
    if (inSignDomain == sdBoth || (inSignDomain == sdNegative && current < 0) || (inSignDomain == sdPositive && current > 0))
    {
      if (current < range.lower || !haveLower)
      {
        range.lower = current;
        haveLower = true;
      }
      if (current > range.upper || !haveUpper)
      {
        range.upper = current;
        haveUpper = true;
      }
    }
    ++it;
  }
  
  foundRange = haveLower && haveUpper;
  return range;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPBarData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPBarData
  \brief Holds the data of one single data point (one bar) for QCPBars.
  
  The container for storing multiple data points is \ref QCPBarDataMap.
  
  The stored data is:
  \li \a key: coordinate on the key axis of this bar
  \li \a value: height coordinate on the value axis of this bar
  
  \see QCPBarDataaMap
*/

/*!
  Constructs a bar data point with key and value set to zero.
*/
QCPBarData::QCPBarData() :
  key(0),
  value(0)
{
}

/*!
  Constructs a bar data point with the specified \a key and \a value.
*/
QCPBarData::QCPBarData(double key, double value) :
  key(key),
  value(value)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPBars
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPBars
  \brief A plottable representing a bar chart in a plot.

  \image html QCPBars.png
  
  To plot data, assign it with the \ref setData or \ref addData functions.
  
  \section appearance Changing the appearance
  
  The appearance of the bars is determined by the pen and the brush (\ref setPen, \ref setBrush).
  
  Bar charts are stackable. This means, Two QCPBars plottables can be placed on top of each other
  (see \ref QCPBars::moveAbove). Then, when two bars are at the same key position, they will appear
  stacked.
  
  \section usage Usage
  
  Like all data representing objects in QCustomPlot, the QCPBars is a plottable
  (QCPAbstractPlottable). So the plottable-interface of QCustomPlot applies
  (QCustomPlot::plottable, QCustomPlot::addPlottable, QCustomPlot::removePlottable, etc.)
  
  Usually, you first create an instance:
  \code
  QCPBars *newBars = new QCPBars(customPlot->xAxis, customPlot->yAxis);\endcode
  add it to the customPlot with QCustomPlot::addPlottable:
  \code
  customPlot->addPlottable(newBars);\endcode
  and then modify the properties of the newly created plottable, e.g.:
  \code
  newBars->setName("Country population");
  newBars->setData(xData, yData);\endcode
*/

/*! \fn QCPBars *QCPBars::barBelow() const
  Returns the bars plottable that is directly below this bars plottable.
  If there is no such plottable, returns 0.
  
  \see barAbove, moveBelow, moveAbove
*/

/*! \fn QCPBars *QCPBars::barAbove() const
  Returns the bars plottable that is directly above this bars plottable.
  If there is no such plottable, returns 0.
  
  \see barBelow, moveBelow, moveAbove
*/

/*!
  Constructs a bar chart which uses \a keyAxis as its key axis ("x") and \a valueAxis as its value
  axis ("y"). \a keyAxis and \a valueAxis must reside in the same QCustomPlot instance and not have
  the same orientation. If either of these restrictions is violated, a corresponding message is
  printed to the debug output (qDebug), the construction is not aborted, though.
  
  The constructed QCPBars can be added to the plot with QCustomPlot::addPlottable, QCustomPlot
  then takes ownership of the bar chart.
*/
QCPBars::QCPBars(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis)
{
  mData = new QCPBarDataMap;
  mPen.setColor(Qt::blue);
  mPen.setStyle(Qt::SolidLine);
  mBrush.setColor(QColor(40, 50, 255, 30));
  mBrush.setStyle(Qt::SolidPattern);
  mSelectedPen = mPen;
  mSelectedPen.setWidthF(2.5);
  mSelectedPen.setColor(QColor(80, 80, 255)); // lighter than Qt::blue of mPen
  mSelectedBrush = mBrush;
  
  mWidth = 0.75;
}

QCPBars::~QCPBars()
{
  if (mBarBelow || mBarAbove)
    connectBars(mBarBelow.data(), mBarAbove.data()); // take this bar out of any stacking
  delete mData;
}

/*!
  Sets the width of the bars in plot (key) coordinates.
*/
void QCPBars::setWidth(double width)
{
  mWidth = width;
}

/*!
  Replaces the current data with the provided \a data.
  
  If \a copy is set to true, data points in \a data will only be copied. if false, the plottable
  takes ownership of the passed data and replaces the internal data pointer with it. This is
  significantly faster than copying for large datasets.
*/
void QCPBars::setData(QCPBarDataMap *data, bool copy)
{
  if (copy)
  {
    *mData = *data;
  } else
  {
    delete mData;
    mData = data;
  }
}

/*! \overload
  
  Replaces the current data with the provided points in \a key and \a value tuples. The
  provided vectors should have equal length. Else, the number of added points will be the size of
  the smallest vector.
*/
void QCPBars::setData(const QVector<double> &key, const QVector<double> &value)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, value.size());
  QCPBarData newData;
  for (int i=0; i<n; ++i)
  {
    newData.key = key[i];
    newData.value = value[i];
    mData->insertMulti(newData.key, newData);
  }
}

/*!
  Moves this bars plottable below \a bars. In other words, the bars of this plottable will appear
  below the bars of \a bars. The move target \a bars must use the same key and value axis as this
  plottable.
  
  Inserting into and removing from existing bar stacking is handled gracefully. If \a bars already
  has a bars object below itself, this bars object is inserted between the two. If this bars object
  is already between two other bars, the two other bars will be stacked on top of each other after
  the operation.
  
  To remove this bars plottable from any stacking, set \a bars to 0.
  
  \see moveBelow, barAbove, barBelow
*/
void QCPBars::moveBelow(QCPBars *bars)
{
  if (bars == this) return;
  if (bars && (bars->keyAxis() != mKeyAxis.data() || bars->valueAxis() != mValueAxis.data()))
  {
    qDebug() << Q_FUNC_INFO << "passed QCPBars* doesn't have same key and value axis as this QCPBars";
    return;
  }
  // remove from stacking:
  connectBars(mBarBelow.data(), mBarAbove.data()); // Note: also works if one (or both) of them is 0
  // if new bar given, insert this bar below it:
  if (bars)
  {
    if (bars->mBarBelow)
      connectBars(bars->mBarBelow.data(), this);
    connectBars(this, bars);
  }
}

/*!
  Moves this bars plottable above \a bars. In other words, the bars of this plottable will appear
  above the bars of \a bars. The move target \a bars must use the same key and value axis as this
  plottable.
  
  Inserting into and removing from existing bar stacking is handled gracefully. If \a bars already
  has a bars object below itself, this bars object is inserted between the two. If this bars object
  is already between two other bars, the two other bars will be stacked on top of each other after
  the operation.
  
  To remove this bars plottable from any stacking, set \a bars to 0.
  
  \see moveBelow, barBelow, barAbove
*/
void QCPBars::moveAbove(QCPBars *bars)
{
  if (bars == this) return;
  if (bars && (bars->keyAxis() != mKeyAxis.data() || bars->valueAxis() != mValueAxis.data()))
  {
    qDebug() << Q_FUNC_INFO << "passed QCPBars* doesn't have same key and value axis as this QCPBars";
    return;
  }
  // remove from stacking:
  connectBars(mBarBelow.data(), mBarAbove.data()); // Note: also works if one (or both) of them is 0
  // if new bar given, insert this bar above it:
  if (bars)
  {
    if (bars->mBarAbove)
      connectBars(this, bars->mBarAbove.data());
    connectBars(bars, this);
  }
}

/*!
  Adds the provided data points in \a dataMap to the current data.
  \see removeData
*/
void QCPBars::addData(const QCPBarDataMap &dataMap)
{
  mData->unite(dataMap);
}

/*! \overload
  Adds the provided single data point in \a data to the current data.
  \see removeData
*/
void QCPBars::addData(const QCPBarData &data)
{
  mData->insertMulti(data.key, data);
}

/*! \overload
  Adds the provided single data point as \a key and \a value tuple to the current data
  \see removeData
*/
void QCPBars::addData(double key, double value)
{
  QCPBarData newData;
  newData.key = key;
  newData.value = value;
  mData->insertMulti(newData.key, newData);
}

/*! \overload
  Adds the provided data points as \a key and \a value tuples to the current data.
  \see removeData
*/
void QCPBars::addData(const QVector<double> &keys, const QVector<double> &values)
{
  int n = keys.size();
  n = qMin(n, values.size());
  QCPBarData newData;
  for (int i=0; i<n; ++i)
  {
    newData.key = keys[i];
    newData.value = values[i];
    mData->insertMulti(newData.key, newData);
  }
}

/*!
  Removes all data points with key smaller than \a key.
  \see addData, clearData
*/
void QCPBars::removeDataBefore(double key)
{
  QCPBarDataMap::iterator it = mData->begin();
  while (it != mData->end() && it.key() < key)
    it = mData->erase(it);
}

/*!
  Removes all data points with key greater than \a key.
  \see addData, clearData
*/
void QCPBars::removeDataAfter(double key)
{
  if (mData->isEmpty()) return;
  QCPBarDataMap::iterator it = mData->upperBound(key);
  while (it != mData->end())
    it = mData->erase(it);
}

/*!
  Removes all data points with key between \a fromKey and \a toKey. if \a fromKey is
  greater or equal to \a toKey, the function does nothing. To remove a single data point with known
  key, use \ref removeData(double key).
  
  \see addData, clearData
*/
void QCPBars::removeData(double fromKey, double toKey)
{
  if (fromKey >= toKey || mData->isEmpty()) return;
  QCPBarDataMap::iterator it = mData->upperBound(fromKey);
  QCPBarDataMap::iterator itEnd = mData->upperBound(toKey);
  while (it != itEnd)
    it = mData->erase(it);
}

/*! \overload
  
  Removes a single data point at \a key. If the position is not known with absolute precision,
  consider using \ref removeData(double fromKey, double toKey) with a small fuzziness interval
  around the suspected position, depeding on the precision with which the key is known.
  
  \see addData, clearData
*/
void QCPBars::removeData(double key)
{
  mData->remove(key);
}

/*!
  Removes all data points.
  \see removeData, removeDataAfter, removeDataBefore
*/
void QCPBars::clearData()
{
  mData->clear();
}

/* inherits documentation from base class */
double QCPBars::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return -1; }
  
  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()))
  {
    QCPBarDataMap::ConstIterator it;
    double posKey, posValue;
    pixelsToCoords(pos, posKey, posValue);
    for (it = mData->constBegin(); it != mData->constEnd(); ++it)
    {
      double baseValue = getBaseValue(it.key(), it.value().value >=0);
      QCPRange keyRange(it.key()-mWidth*0.5, it.key()+mWidth*0.5);
      QCPRange valueRange(baseValue, baseValue+it.value().value);
      if (keyRange.contains(posKey) && valueRange.contains(posValue))
        return mParentPlot->selectionTolerance()*0.99;
    }
  }
  return -1;
}

/* inherits documentation from base class */
void QCPBars::draw(QCPPainter *painter)
{
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  if (mData->isEmpty()) return;
  
  QCPBarDataMap::const_iterator it;
  for (it = mData->constBegin(); it != mData->constEnd(); ++it)
  {
    // skip bar if not visible in key axis range:
    if (it.key()+mWidth*0.5 < mKeyAxis.data()->range().lower || it.key()-mWidth*0.5 > mKeyAxis.data()->range().upper)
      continue;
    // check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
    if (QCP::isInvalidData(it.value().key, it.value().value))
      qDebug() << Q_FUNC_INFO << "Data point at" << it.key() << "of drawn range invalid." << "Plottable name:" << name();
#endif
    QPolygonF barPolygon = getBarPolygon(it.key(), it.value().value);
    // draw bar fill:
    if (mainBrush().style() != Qt::NoBrush && mainBrush().color().alpha() != 0)
    {
      applyFillAntialiasingHint(painter);
      painter->setPen(Qt::NoPen);
      painter->setBrush(mainBrush());
      painter->drawPolygon(barPolygon);
    }
    // draw bar line:
    if (mainPen().style() != Qt::NoPen && mainPen().color().alpha() != 0)
    {
      applyDefaultAntialiasingHint(painter);
      painter->setPen(mainPen());
      painter->setBrush(Qt::NoBrush);
      painter->drawPolyline(barPolygon);
    }
  }
}

/* inherits documentation from base class */
void QCPBars::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
  // draw filled rect:
  applyDefaultAntialiasingHint(painter);
  painter->setBrush(mBrush);
  painter->setPen(mPen);
  QRectF r = QRectF(0, 0, rect.width()*0.67, rect.height()*0.67);
  r.moveCenter(rect.center());
  painter->drawRect(r);
}

/*! \internal
  
  Returns the polygon of a single bar with \a key and \a value. The Polygon is open at the bottom
  and shifted according to the bar stacking (see \ref moveAbove).
*/
QPolygonF QCPBars::getBarPolygon(double key, double value) const
{
  QPolygonF result;
  double baseValue = getBaseValue(key, value >= 0);
  result << coordsToPixels(key-mWidth*0.5, baseValue);
  result << coordsToPixels(key-mWidth*0.5, baseValue+value);
  result << coordsToPixels(key+mWidth*0.5, baseValue+value);
  result << coordsToPixels(key+mWidth*0.5, baseValue);
  return result;
}

/*! \internal
  
  This function is called to find at which value to start drawing the base of a bar at \a key, when
  it is stacked on top of another QCPBars (e.g. with \ref moveAbove).
  
  positive and negative bars are separated per stack (positive are stacked above 0-value upwards,
  negative are stacked below 0-value downwards). This can be indicated with \a positive. So if the
  bar for which we need the base value is negative, set \a positive to false.
*/
double QCPBars::getBaseValue(double key, bool positive) const
{
  if (mBarBelow)
  {
    double max = 0;
    // find bars of mBarBelow that are approximately at key and find largest one:
    QCPBarDataMap::const_iterator it = mBarBelow.data()->mData->lowerBound(key-mWidth*0.1);
    QCPBarDataMap::const_iterator itEnd = mBarBelow.data()->mData->upperBound(key+mWidth*0.1);
    while (it != itEnd)
    {
      if ((positive && it.value().value > max) ||
          (!positive && it.value().value < max))
        max = it.value().value;
      ++it;
    }
    // recurse down the bar-stack to find the total height:
    return max + mBarBelow.data()->getBaseValue(key, positive);
  } else
    return 0;
}

/*! \internal

  Connects \a below and \a above to each other via their mBarAbove/mBarBelow properties.
  The bar(s) currently below lower and upper will become disconnected to lower/upper.
  
  If lower is zero, upper will be disconnected at the bottom.
  If upper is zero, lower will be disconnected at the top.
*/
void QCPBars::connectBars(QCPBars *lower, QCPBars *upper)
{
  if (!lower && !upper) return;
  
  if (!lower) // disconnect upper at bottom
  {
    // disconnect old bar below upper:
    if (upper->mBarBelow && upper->mBarBelow.data()->mBarAbove.data() == upper)
      upper->mBarBelow.data()->mBarAbove = 0;
    upper->mBarBelow = 0;
  } else if (!upper) // disconnect lower at top
  {
    // disconnect old bar above lower:
    if (lower->mBarAbove && lower->mBarAbove.data()->mBarBelow.data() == lower)
      lower->mBarAbove.data()->mBarBelow = 0;
    lower->mBarAbove = 0;
  } else // connect lower and upper
  {
    // disconnect old bar above lower:
    if (lower->mBarAbove && lower->mBarAbove.data()->mBarBelow.data() == lower)
      lower->mBarAbove.data()->mBarBelow = 0;
    // disconnect old bar below upper:
    if (upper->mBarBelow && upper->mBarBelow.data()->mBarAbove.data() == upper)
      upper->mBarBelow.data()->mBarAbove = 0;
    lower->mBarAbove = upper;
    upper->mBarBelow = lower;
  }
}

/* inherits documentation from base class */
QCPRange QCPBars::getKeyRange(bool &foundRange, SignDomain inSignDomain) const
{
  QCPRange range;
  bool haveLower = false;
  bool haveUpper = false;
  
  double current;
  double barWidthHalf = mWidth*0.5;
  QCPBarDataMap::const_iterator it = mData->constBegin();
  while (it != mData->constEnd())
  {
    current = it.value().key;
    if (inSignDomain == sdBoth || (inSignDomain == sdNegative && current+barWidthHalf < 0) || (inSignDomain == sdPositive && current-barWidthHalf > 0))
    {
      if (current-barWidthHalf < range.lower || !haveLower)
      {
        range.lower = current-barWidthHalf;
        haveLower = true;
      }
      if (current+barWidthHalf > range.upper || !haveUpper)
      {
        range.upper = current+barWidthHalf;
        haveUpper = true;
      }
    }
    ++it;
  }
  
  foundRange = haveLower && haveUpper;
  return range;
}

/* inherits documentation from base class */
QCPRange QCPBars::getValueRange(bool &foundRange, SignDomain inSignDomain) const
{
  QCPRange range;
  bool haveLower = true; // set to true, because 0 should always be visible in bar charts
  bool haveUpper = true; // set to true, because 0 should always be visible in bar charts
  
  double current;
  
  QCPBarDataMap::const_iterator it = mData->constBegin();
  while (it != mData->constEnd())
  {
    current = it.value().value + getBaseValue(it.value().key, it.value().value >= 0);
    if (inSignDomain == sdBoth || (inSignDomain == sdNegative && current < 0) || (inSignDomain == sdPositive && current > 0))
    {
      if (current < range.lower || !haveLower)
      {
        range.lower = current;
        haveLower = true;
      }
      if (current > range.upper || !haveUpper)
      {
        range.upper = current;
        haveUpper = true;
      }
    }
    ++it;
  }
  
  foundRange = true; // return true because bar charts always have the 0-line visible
  return range;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPStatisticalBox
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPStatisticalBox
  \brief A plottable representing a single statistical box in a plot.

  \image html QCPStatisticalBox.png
  
  To plot data, assign it with the individual parameter functions or use \ref setData to set all
  parameters at once. The individual functions are:
  \li \ref setMinimum
  \li \ref setLowerQuartile
  \li \ref setMedian
  \li \ref setUpperQuartile
  \li \ref setMaximum
  
  Additionally you can define a list of outliers, drawn as scatter datapoints:
  \li \ref setOutliers
  
  \section appearance Changing the appearance
  
  The appearance of the box itself is controlled via \ref setPen and \ref setBrush. You may change
  the width of the box with \ref setWidth in plot coordinates (not pixels).

  Analog functions exist for the minimum/maximum-whiskers: \ref setWhiskerPen, \ref
  setWhiskerBarPen, \ref setWhiskerWidth. The whisker width is the width of the bar at the top
  (maximum) and bottom (minimum).
  
  The median indicator line has its own pen, \ref setMedianPen.
  
  If the whisker backbone pen is changed, make sure to set the capStyle to Qt::FlatCap. Else, the
  backbone line might exceed the whisker bars by a few pixels due to the pen cap being not
  perfectly flat.
  
  The Outlier data points are drawn as normal scatter points. Their look can be controlled with
  \ref setOutlierStyle
  
  \section usage Usage
  
  Like all data representing objects in QCustomPlot, the QCPStatisticalBox is a plottable
  (QCPAbstractPlottable). So the plottable-interface of QCustomPlot applies
  (QCustomPlot::plottable, QCustomPlot::addPlottable, QCustomPlot::removePlottable, etc.)
  
  Usually, you first create an instance:
  \code
  QCPStatisticalBox *newBox = new QCPStatisticalBox(customPlot->xAxis, customPlot->yAxis);\endcode
  add it to the customPlot with QCustomPlot::addPlottable:
  \code
  customPlot->addPlottable(newBox);\endcode
  and then modify the properties of the newly created plottable, e.g.:
  \code
  newBox->setName("Measurement Series 1");
  newBox->setData(1, 3, 4, 5, 7);
  newBox->setOutliers(QVector<double>() << 0.5 << 0.64 << 7.2 << 7.42);\endcode
*/

/*!
  Constructs a statistical box which uses \a keyAxis as its key axis ("x") and \a valueAxis as its
  value axis ("y"). \a keyAxis and \a valueAxis must reside in the same QCustomPlot instance and
  not have the same orientation. If either of these restrictions is violated, a corresponding
  message is printed to the debug output (qDebug), the construction is not aborted, though.
  
  The constructed statistical box can be added to the plot with QCustomPlot::addPlottable,
  QCustomPlot then takes ownership of the statistical box.
*/
QCPStatisticalBox::QCPStatisticalBox(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis),
  mKey(0),
  mMinimum(0),
  mLowerQuartile(0),
  mMedian(0),
  mUpperQuartile(0),
  mMaximum(0)
{
  setOutlierStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, 6));
  setWhiskerWidth(0.2);
  setWidth(0.5);
  
  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue, 2.5));
  setMedianPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap));
  setWhiskerPen(QPen(Qt::black, 0, Qt::DashLine, Qt::FlatCap));
  setWhiskerBarPen(QPen(Qt::black));
  setBrush(Qt::NoBrush);
  setSelectedBrush(Qt::NoBrush);
}

/*!
  Sets the key coordinate of the statistical box.
*/
void QCPStatisticalBox::setKey(double key)
{
  mKey = key;
}

/*!
  Sets the parameter "minimum" of the statistical box plot. This is the position of the lower
  whisker, typically the minimum measurement of the sample that's not considered an outlier.
  
  \see setMaximum, setWhiskerPen, setWhiskerBarPen, setWhiskerWidth
*/
void QCPStatisticalBox::setMinimum(double value)
{
  mMinimum = value;
}

/*!
  Sets the parameter "lower Quartile" of the statistical box plot. This is the lower end of the
  box. The lower and the upper quartiles are the two statistical quartiles around the median of the
  sample, they contain 50% of the sample data.
  
  \see setUpperQuartile, setPen, setBrush, setWidth
*/
void QCPStatisticalBox::setLowerQuartile(double value)
{
  mLowerQuartile = value;
}

/*!
  Sets the parameter "median" of the statistical box plot. This is the value of the median mark
  inside the quartile box. The median separates the sample data in half (50% of the sample data is
  below/above the median).
  
  \see setMedianPen
*/
void QCPStatisticalBox::setMedian(double value)
{
  mMedian = value;
}

/*!
  Sets the parameter "upper Quartile" of the statistical box plot. This is the upper end of the
  box. The lower and the upper quartiles are the two statistical quartiles around the median of the
  sample, they contain 50% of the sample data.
  
  \see setLowerQuartile, setPen, setBrush, setWidth
*/
void QCPStatisticalBox::setUpperQuartile(double value)
{
  mUpperQuartile = value;
}

/*!
  Sets the parameter "maximum" of the statistical box plot. This is the position of the upper
  whisker, typically the maximum measurement of the sample that's not considered an outlier.
  
  \see setMinimum, setWhiskerPen, setWhiskerBarPen, setWhiskerWidth
*/
void QCPStatisticalBox::setMaximum(double value)
{
  mMaximum = value;
}

/*!
  Sets a vector of outlier values that will be drawn as scatters. Any data points in the sample
  that are not within the whiskers (\ref setMinimum, \ref setMaximum) should be considered outliers
  and displayed as such.
  
  \see setOutlierStyle
*/
void QCPStatisticalBox::setOutliers(const QVector<double> &values)
{
  mOutliers = values;
}

/*!
  Sets all parameters of the statistical box plot at once.
  
  \see setKey, setMinimum, setLowerQuartile, setMedian, setUpperQuartile, setMaximum
*/
void QCPStatisticalBox::setData(double key, double minimum, double lowerQuartile, double median, double upperQuartile, double maximum)
{
  setKey(key);
  setMinimum(minimum);
  setLowerQuartile(lowerQuartile);
  setMedian(median);
  setUpperQuartile(upperQuartile);
  setMaximum(maximum);
}

/*!
  Sets the width of the box in key coordinates.
  
  \see setWhiskerWidth
*/
void QCPStatisticalBox::setWidth(double width)
{
  mWidth = width;
}

/*!
  Sets the width of the whiskers (\ref setMinimum, \ref setMaximum) in key coordinates.
  
  \see setWidth
*/
void QCPStatisticalBox::setWhiskerWidth(double width)
{
  mWhiskerWidth = width;
}

/*!
  Sets the pen used for drawing the whisker backbone (That's the line parallel to the value axis).
  
  Make sure to set the \a pen capStyle to Qt::FlatCap to prevent the whisker backbone from reaching
  a few pixels past the whisker bars, when using a non-zero pen width.
  
  \see setWhiskerBarPen
*/
void QCPStatisticalBox::setWhiskerPen(const QPen &pen)
{
  mWhiskerPen = pen;
}

/*!
  Sets the pen used for drawing the whisker bars (Those are the lines parallel to the key axis at
  each end of the whisker backbone).
  
  \see setWhiskerPen
*/
void QCPStatisticalBox::setWhiskerBarPen(const QPen &pen)
{
  mWhiskerBarPen = pen;
}

/*!
  Sets the pen used for drawing the median indicator line inside the statistical box.
*/
void QCPStatisticalBox::setMedianPen(const QPen &pen)
{
  mMedianPen = pen;
}

/*!
  Sets the appearance of the outlier data points.
  
  \see setOutliers
*/
void QCPStatisticalBox::setOutlierStyle(const QCPScatterStyle &style)
{
  mOutlierStyle = style;
}

/* inherits documentation from base class */
void QCPStatisticalBox::clearData()
{
  setOutliers(QVector<double>());
  setKey(0);
  setMinimum(0);
  setLowerQuartile(0);
  setMedian(0);
  setUpperQuartile(0);
  setMaximum(0);
}

/* inherits documentation from base class */
double QCPStatisticalBox::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return -1; }
  
  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()))
  {
    double posKey, posValue;
    pixelsToCoords(pos, posKey, posValue);
    // quartile box:
    QCPRange keyRange(mKey-mWidth*0.5, mKey+mWidth*0.5);
    QCPRange valueRange(mLowerQuartile, mUpperQuartile);
    if (keyRange.contains(posKey) && valueRange.contains(posValue))
      return mParentPlot->selectionTolerance()*0.99;
    
    // min/max whiskers:
    if (QCPRange(mMinimum, mMaximum).contains(posValue))
      return qAbs(mKeyAxis.data()->coordToPixel(mKey)-mKeyAxis.data()->coordToPixel(posKey));
  }
  return -1;
}

/* inherits documentation from base class */
void QCPStatisticalBox::draw(QCPPainter *painter)
{
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }

  // check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
  if (QCP::isInvalidData(mKey, mMedian) ||
      QCP::isInvalidData(mLowerQuartile, mUpperQuartile) ||
      QCP::isInvalidData(mMinimum, mMaximum))
    qDebug() << Q_FUNC_INFO << "Data point at" << mKey << "of drawn range has invalid data." << "Plottable name:" << name();
  for (int i=0; i<mOutliers.size(); ++i)
    if (QCP::isInvalidData(mOutliers.at(i)))
      qDebug() << Q_FUNC_INFO << "Data point outlier at" << mKey << "of drawn range invalid." << "Plottable name:" << name();
#endif
  
  QRectF quartileBox;
  drawQuartileBox(painter, &quartileBox);
  
  painter->save();
  painter->setClipRect(quartileBox, Qt::IntersectClip);
  drawMedian(painter);
  painter->restore();
  
  drawWhiskers(painter);
  drawOutliers(painter);
}

/* inherits documentation from base class */
void QCPStatisticalBox::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
  // draw filled rect:
  applyDefaultAntialiasingHint(painter);
  painter->setPen(mPen);
  painter->setBrush(mBrush);
  QRectF r = QRectF(0, 0, rect.width()*0.67, rect.height()*0.67);
  r.moveCenter(rect.center());
  painter->drawRect(r);
}

/*! \internal
  
  Draws the quartile box. \a box is an output parameter that returns the quartile box (in pixel
  coordinates) which is used to set the clip rect of the painter before calling \ref drawMedian (so
  the median doesn't draw outside the quartile box).
*/
void QCPStatisticalBox::drawQuartileBox(QCPPainter *painter, QRectF *quartileBox) const
{
  QRectF box;
  box.setTopLeft(coordsToPixels(mKey-mWidth*0.5, mUpperQuartile));
  box.setBottomRight(coordsToPixels(mKey+mWidth*0.5, mLowerQuartile));
  applyDefaultAntialiasingHint(painter);
  painter->setPen(mainPen());
  painter->setBrush(mainBrush());
  painter->drawRect(box);
  if (quartileBox)
    *quartileBox = box;
}

/*! \internal
  
  Draws the median line inside the quartile box.
*/
void QCPStatisticalBox::drawMedian(QCPPainter *painter) const
{
  QLineF medianLine;
  medianLine.setP1(coordsToPixels(mKey-mWidth*0.5, mMedian));
  medianLine.setP2(coordsToPixels(mKey+mWidth*0.5, mMedian));
  applyDefaultAntialiasingHint(painter);
  painter->setPen(mMedianPen);
  painter->drawLine(medianLine);
}

/*! \internal
  
  Draws both whisker backbones and bars.
*/
void QCPStatisticalBox::drawWhiskers(QCPPainter *painter) const
{
  QLineF backboneMin, backboneMax, barMin, barMax;
  backboneMax.setPoints(coordsToPixels(mKey, mUpperQuartile), coordsToPixels(mKey, mMaximum));
  backboneMin.setPoints(coordsToPixels(mKey, mLowerQuartile), coordsToPixels(mKey, mMinimum));
  barMax.setPoints(coordsToPixels(mKey-mWhiskerWidth*0.5, mMaximum), coordsToPixels(mKey+mWhiskerWidth*0.5, mMaximum));
  barMin.setPoints(coordsToPixels(mKey-mWhiskerWidth*0.5, mMinimum), coordsToPixels(mKey+mWhiskerWidth*0.5, mMinimum));
  applyErrorBarsAntialiasingHint(painter);
  painter->setPen(mWhiskerPen);
  painter->drawLine(backboneMin);
  painter->drawLine(backboneMax);
  painter->setPen(mWhiskerBarPen);
  painter->drawLine(barMin);
  painter->drawLine(barMax);
}

/*! \internal
  
  Draws the outlier scatter points.
*/
void QCPStatisticalBox::drawOutliers(QCPPainter *painter) const
{
  applyScattersAntialiasingHint(painter);
  mOutlierStyle.applyTo(painter, mPen);
  for (int i=0; i<mOutliers.size(); ++i)
    mOutlierStyle.drawShape(painter, coordsToPixels(mKey, mOutliers.at(i)));
}

/* inherits documentation from base class */
QCPRange QCPStatisticalBox::getKeyRange(bool &foundRange, SignDomain inSignDomain) const
{
  foundRange = true;
  if (inSignDomain == sdBoth)
  {
    return QCPRange(mKey-mWidth*0.5, mKey+mWidth*0.5);
  } else if (inSignDomain == sdNegative)
  {
    if (mKey+mWidth*0.5 < 0)
      return QCPRange(mKey-mWidth*0.5, mKey+mWidth*0.5);
    else if (mKey < 0)
      return QCPRange(mKey-mWidth*0.5, mKey);
    else
    {
      foundRange = false;
      return QCPRange();
    }
  } else if (inSignDomain == sdPositive)
  {
    if (mKey-mWidth*0.5 > 0)
      return QCPRange(mKey-mWidth*0.5, mKey+mWidth*0.5);
    else if (mKey > 0)
      return QCPRange(mKey, mKey+mWidth*0.5);
    else
    {
      foundRange = false;
      return QCPRange();
    }
  }
  foundRange = false;
  return QCPRange();
}

/* inherits documentation from base class */
QCPRange QCPStatisticalBox::getValueRange(bool &foundRange, SignDomain inSignDomain) const
{
  QVector<double> values; // values that must be considered (i.e. all outliers and the five box-parameters)
  values.reserve(mOutliers.size() + 5);
  values << mMaximum << mUpperQuartile << mMedian << mLowerQuartile << mMinimum;
  values << mOutliers;
  // go through values and find the ones in legal range:
  bool haveUpper = false;
  bool haveLower = false;
  double upper = 0;
  double lower = 0;
  for (int i=0; i<values.size(); ++i)
  {
    if ((inSignDomain == sdNegative && values.at(i) < 0) ||
        (inSignDomain == sdPositive && values.at(i) > 0) ||
        (inSignDomain == sdBoth))
    {
      if (values.at(i) > upper || !haveUpper)
      {
        upper = values.at(i);
        haveUpper = true;
      }
      if (values.at(i) < lower || !haveLower)
      {
        lower = values.at(i);
        haveLower = true;
      }
    }
  }
  // return the bounds if we found some sensible values:
  if (haveLower && haveUpper)
  {
    foundRange = true;
    return QCPRange(lower, upper);
  } else // might happen if all values are in other sign domain
  {
    foundRange = false;
    return QCPRange();
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorMapData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorMapData
  \brief Holds the two-dimensional data of a QCPColorMap plottable.
  
  This class is a data storage for \ref QCPColorMap. It holds a two-dimensional array, which \ref
  QCPColorMap then displays as a 2D image in the plot, where the array values are represented by a
  color, depending on the value.
  
  The size of the array can be controlled via \ref setSize (or \ref setKeySize, \ref setValueSize).
  Which plot coordinates these cells correspond to can be configured with \ref setRange (or \ref
  setKeyRange, \ref setValueRange).
  
  The data cells can be accessed in two ways: They can be directly addressed by an integer index
  with \ref setCell. This is the fastest method. Alternatively, they can be addressed by their plot
  coordinate with \ref setData. plot coordinate to cell index transformations and vice versa are
  provided by the functions \ref coordToCell and \ref cellToCoord.
  
  This class also buffers the minimum and maximum values that are in the data set, to provide
  QCPColorMap::rescaleDataRange with the necessary information quickly. Setting a cell to a value
  that is greater than the current maximum increases this maximum to the new value. However,
  setting the cell that currently holds the maximum value to a smaller value doesn't decrease the
  maximum again, because finding the true new maximum would require going through the entire data
  array, which might be time consuming. The same holds for the data minimum. This functionality is
  given by \ref recalculateDataBounds, such that you can decide when it is sensible to find the
  true current minimum and maximum. The method QCPColorMap::rescaleDataRange offers a convenience
  parameter \a recalculateDataBounds which may be set to true to automatically call \ref
  recalculateDataBounds internally.
*/

/* start of documentation of inline functions */

/*! \fn bool QCPColorMapData::isEmpty() const
  
  Returns whether this instance carries no data. This is equivalent to having a size where at least
  one of the dimensions is 0 (see \ref setSize).
*/

/* end of documentation of inline functions */

/*!
  Constructs a new QCPColorMapData instance. The instance has \a keySize cells in the key direction
  and \a valueSize cells in the value direction. These cells will be displayed by the \ref QCPColorMap
  at the coordinates \a keyRange and \a valueRange.
  
  \see setSize, setKeySize, setValueSize, setRange, setKeyRange, setValueRange
*/
QCPColorMapData::QCPColorMapData(int keySize, int valueSize, const QCPRange &keyRange, const QCPRange &valueRange) :
  mKeySize(0),
  mValueSize(0),
  mKeyRange(keyRange),
  mValueRange(valueRange),
  mIsEmpty(true),
  mData(0),
  mDataModified(true)
{
  setSize(keySize, valueSize);
  fill(0);
}

QCPColorMapData::~QCPColorMapData()
{
  if (mData)
    delete[] mData;
}

/*!
  Constructs a new QCPColorMapData instance copying the data and range of \a other.
*/
QCPColorMapData::QCPColorMapData(const QCPColorMapData &other) :
  mKeySize(0),
  mValueSize(0),
  mIsEmpty(true),
  mData(0),
  mDataModified(true)
{
  *this = other;
}

/*!
  Overwrites this color map data instance with the data stored in \a other.
*/
QCPColorMapData &QCPColorMapData::operator=(const QCPColorMapData &other)
{
  if (&other != this)
  {
    const int keySize = other.keySize();
    const int valueSize = other.valueSize();
    setSize(keySize, valueSize);
    setRange(other.keyRange(), other.valueRange());
    if (!mIsEmpty)
      memcpy(mData, other.mData, sizeof(mData[0])*keySize*valueSize);
    mDataBounds = other.mDataBounds;
    mDataModified = true;
  }
  return *this;
}

/* undocumented getter */
double QCPColorMapData::data(double key, double value)
{
  int keyCell = (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*(mKeySize-1)+0.5;
  int valueCell = (1.0-(value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower))*(mValueSize-1)+0.5;
  if (keyCell >= 0 && keyCell < mKeySize && valueCell >= 0 && valueCell < mValueSize)
    return mData[valueCell*mKeySize + keyCell];
  else
    return 0;
}

/* undocumented getter */
double QCPColorMapData::cell(int keyIndex, int valueIndex)
{
  if (keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 && valueIndex < mValueSize)
    return mData[valueIndex*mKeySize + keyIndex];
  else
    return 0;
}

/*!
  Resizes the data array to have \a keySize cells in the key dimension and \a valueSize cells in
  the value dimension.

  The current data is discarded and the map cells are set to 0, unless the map had already the
  requested size.
  
  Setting at least one of \a keySize or \a valueSize to zero frees the internal data array and \ref
  isEmpty returns true.

  \see setRange, setKeySize, setValueSize
*/
void QCPColorMapData::setSize(int keySize, int valueSize)
{
  if (keySize != mKeySize || valueSize != mValueSize)
  {
    mKeySize = keySize;
    mValueSize = valueSize;
    if (mData)
      delete[] mData;
    mIsEmpty = mKeySize == 0 || mValueSize == 0;
    if (!mIsEmpty)
    {
#ifdef __EXCEPTIONS
      try { // 2D arrays get memory intensive fast. So if the allocation fails, at least output debug message
#endif
      mData = new double[mKeySize*mValueSize];
#ifdef __EXCEPTIONS
      } catch (...) { mData = 0; }
#endif
      if (mData)
        fill(0);
      else
        qDebug() << Q_FUNC_INFO << "out of memory for data dimensions "<< mKeySize << "*" << mValueSize;
    } else
      mData = 0;
    mDataModified = true;
  }
}

/*!
  Resizes the data array to have \a keySize cells in the key dimension.

  The current data is discarded and the map cells are set to 0, unless the map had already the
  requested size.
  
  Setting \a keySize to zero frees the internal data array and \ref isEmpty returns true.

  \see setKeyRange, setSize, setValueSize
*/
void QCPColorMapData::setKeySize(int keySize)
{
  setSize(keySize, mValueSize);
}

/*!
  Resizes the data array to have \a valueSize cells in the value dimension.

  The current data is discarded and the map cells are set to 0, unless the map had already the
  requested size.
  
  Setting \a valueSize to zero frees the internal data array and \ref isEmpty returns true.

  \see setValueRange, setSize, setKeySize
*/
void QCPColorMapData::setValueSize(int valueSize)
{
  setSize(mKeySize, valueSize);
}

/*!
  Sets the coordinate ranges the data shall be distributed over. This defines the rectangular area
  covered by the color map in plot coordinates.
  
  The outer cells will be centered on the range boundaries given to this function. For example, if
  the key size (\ref setKeySize) is 3 and \a keyRange is set to <tt>QCPRange(2, 3)</tt> there will
  be cells centered on the key coordinates 2, 2.5 and 3.
 
  \see setSize
*/
void QCPColorMapData::setRange(const QCPRange &keyRange, const QCPRange &valueRange)
{
  setKeyRange(keyRange);
  setValueRange(valueRange);
}

/*!
  Sets the coordinate range the data shall be distributed over in the key dimension. Together with
  the value range, This defines the rectangular area covered by the color map in plot coordinates.
  
  The outer cells will be centered on the range boundaries given to this function. For example, if
  the key size (\ref setKeySize) is 3 and \a keyRange is set to <tt>QCPRange(2, 3)</tt> there will
  be cells centered on the key coordinates 2, 2.5 and 3.
 
  \see setRange, setValueRange, setSize
*/
void QCPColorMapData::setKeyRange(const QCPRange &keyRange)
{
  mKeyRange = keyRange;
}

/*!
  Sets the coordinate range the data shall be distributed over in the value dimension. Together with
  the key range, This defines the rectangular area covered by the color map in plot coordinates.
  
  The outer cells will be centered on the range boundaries given to this function. For example, if
  the value size (\ref setValueSize) is 3 and \a valueRange is set to <tt>QCPRange(2, 3)</tt> there
  will be cells centered on the value coordinates 2, 2.5 and 3.
 
  \see setRange, setKeyRange, setSize
*/
void QCPColorMapData::setValueRange(const QCPRange &valueRange)
{
  mValueRange = valueRange;
}

/*!
  Sets the data of the cell, which lies at the plot coordinates given by \a key and \a value, to \a
  z.
 
  \see setCell, setRange
*/
void QCPColorMapData::setData(double key, double value, double z)
{
  int keyCell = (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*(mKeySize-1)+0.5;
  int valueCell = (value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower)*(mValueSize-1)+0.5;
  if (keyCell >= 0 && keyCell < mKeySize && valueCell >= 0 && valueCell < mValueSize)
  {
    mData[valueCell*mKeySize + keyCell] = z;
    if (z < mDataBounds.lower)
      mDataBounds.lower = z;
    if (z > mDataBounds.upper)
      mDataBounds.upper = z;
     mDataModified = true;
  }
}

/*!
  Sets the data of the cell with indices \a keyIndex and \a valueIndex to \a z. The indices
  enumerate the cells starting from zero, up to the map's size-1 in the respective dimension (see
  \ref setSize).
  
  In the standard plot configuration (horizontal key axis and vertical value axis, both not
  range-reversed), the cell with indices (0, 0) is in the bottom left corner and the cell with
  indices (keySize-1, valueSize-1) is in the top right corner of the color map.
  
  \see setData, setSize
*/
void QCPColorMapData::setCell(int keyIndex, int valueIndex, double z)
{
  if (keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 && valueIndex < mValueSize)
  {
    mData[valueIndex*mKeySize + keyIndex] = z;
    if (z < mDataBounds.lower)
      mDataBounds.lower = z;
    if (z > mDataBounds.upper)
      mDataBounds.upper = z;
     mDataModified = true;
  }
}

/*!
  Goes through the data and updates the buffered minimum and maximum data values.
  
  Calling this method is only advised if you are about to call \ref QCPColorMap::rescaleDataRange
  and can not guarantee that the cells holding the maximum or minimum data haven't been overwritten
  with a smaller or larger value respectively, since the buffered maximum/minimum values have been
  updated the last time. Why this is the case is explained in the class description (\ref
  QCPColorMapData).
  
  Note that the method \ref QCPColorMap::rescaleDataRange provides a parameter \a
  recalculateDataBounds for convenience. Setting this to true will call this method for you, before
  doing the rescale.
*/
void QCPColorMapData::recalculateDataBounds()
{
  if (mKeySize > 0 && mValueSize > 0)
  {
    double minHeight = mData[0];
    double maxHeight = mData[0];
    const int dataCount = mValueSize*mKeySize;
    for (int i=0; i<dataCount; ++i)
    {
      if (mData[i] > maxHeight)
        maxHeight = mData[i];
      if (mData[i] < minHeight)
        minHeight = mData[i];
    }
    mDataBounds.lower = minHeight;
    mDataBounds.upper = maxHeight;
  }
}

/*!
  Frees the internal data memory.
  
  This is equivalent to calling \ref setSize "setSize(0, 0)".
*/
void QCPColorMapData::clear()
{
  setSize(0, 0);
}

/*!
  Sets all cells to the value \a z.
*/
void QCPColorMapData::fill(double z)
{
  const int dataCount = mValueSize*mKeySize;
  for (int i=0; i<dataCount; ++i)
    mData[i] = z;
  mDataBounds = QCPRange(z, z);
}

/*!
  Transforms plot coordinates given by \a key and \a value to cell indices of this QCPColorMapData
  instance. The resulting cell indices are returned via the output parameters \a keyIndex and \a
  valueIndex.
  
  The retrieved key/value cell indices can then be used for example with \ref setCell.
  
  If you are only interested in a key or value index, you may pass 0 as \a valueIndex or \a
  keyIndex.
  
  \see cellToCoord, QCPAxis::coordToPixel
*/
void QCPColorMapData::coordToCell(double key, double value, int *keyIndex, int *valueIndex) const
{
  if (keyIndex)
    *keyIndex = (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*(mKeySize-1)+0.5;
  if (valueIndex)
    *valueIndex = (value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower)*(mValueSize-1)+0.5;
}

/*!
  Transforms cell indices given by \a keyIndex and \a valueIndex to cell indices of this QCPColorMapData
  instance. The resulting coordinates are returned via the output parameters \a key and \a
  value.
  
  If you are only interested in a key or value coordinate, you may pass 0 as \a key or \a
  value.
  
  \see coordToCell, QCPAxis::pixelToCoord
*/
void QCPColorMapData::cellToCoord(int keyIndex, int valueIndex, double *key, double *value) const
{
  if (key)
    *key = keyIndex/(double)(mKeySize-1)*(mKeyRange.upper-mKeyRange.lower)+mKeyRange.lower;
  if (value)
    *value = valueIndex/(double)(mValueSize-1)*(mValueRange.upper-mValueRange.lower)+mValueRange.lower;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorMap
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorMap
  \brief A plottable representing a two-dimensional color map in a plot.

  \image html QCPColorMap.png
  
  The data is stored in the class \ref QCPColorMapData, which can be accessed via the data()
  method.
  
  A color map has three dimensions to represent a data point: The \a key dimension, the \a value
  dimension and the \a data dimension. As with other plottables such as graphs, \a key and \a value
  correspond to two orthogonal axes on the QCustomPlot surface that you specify in the QColorMap
  constructor. The \a data dimension however is encoded as the color of the point at (\a key, \a
  value).

  Set the number of points (or \a cells) in the key/value dimension via \ref
  QCPColorMapData::setSize. The plot coordinate range over which these points will be displayed is
  specified via \ref QCPColorMapData::setRange. The first cell will be centered on the lower range
  boundary and the last cell will be centered on the upper range boundary. The data can be set by
  either accessing the cells directly with QCPColorMapData::setCell or by addressing the cells via
  their plot coordinates with \ref QCPColorMapData::setData. If possible, you should prefer
  setCell, since it doesn't need to do any coordinate transformation and thus performs a bit
  better.
  
  The cell with index (0, 0) is at the bottom left, if the color map uses normal (i.e. not reversed)
  key and value axes.
  
  To show the user which colors correspond to which \a data values, a \ref QCPColorScale is
  typically placed to the right of the axis rect. See the documentation there for details on how to
  add and use a color scale.
  
  \section appearance Changing the appearance
  
  The central part of the appearance is the color gradient, which can be specified via \ref
  setGradient. See the documentation of \ref QCPColorGradient for details on configuring a color
  gradient.
  
  The \a data range that is mapped to the colors of the gradient can be specified with \ref
  setDataRange. To make the data range encompass the whole data set minimum to maximum, call \ref
  rescaleDataRange.
  
  \section usage Usage
  
  Like all data representing objects in QCustomPlot, the QCPColorMap is a plottable
  (QCPAbstractPlottable). So the plottable-interface of QCustomPlot applies
  (QCustomPlot::plottable, QCustomPlot::addPlottable, QCustomPlot::removePlottable, etc.)
  
  Usually, you first create an instance:
  \code
  QCPColorMap *colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);\endcode
  add it to the customPlot with QCustomPlot::addPlottable:
  \code
  customPlot->addPlottable(colorMap);\endcode
  and then modify the properties of the newly created color map, e.g.:
  \code
  colorMap->data()->setSize(50, 50);
  colorMap->data()->setRange(QCPRange(0, 2), QCPRange(0, 2));
  for (int x=0; x<50; ++x)
    for (int y=0; y<50; ++y)
      colorMap->data()->setCell(x, y, qCos(x/10.0)+qSin(y/10.0));
  colorMap->setGradient(QCPColorGradient::gpPolar);
  colorMap->rescaleDataRange(true);
  customPlot->rescaleAxes();
  customPlot->replot();
  \endcode
  
  \note The QCPColorMap always displays the data at equal key/value intervals, even if the key or
  value axis is set to a logarithmic scaling. If you want to use QCPColorMap with logarithmic axes,
  you shouldn't use the \ref QCPColorMapData::setData method as it uses a linear transformation to
  determine the cell index. Rather directly access the cell index with \ref
  QCPColorMapData::setCell.
*/

/* start documentation of inline functions */

/*! \fn QCPColorMapData *QCPColorMap::data() const
  
  Returns a pointer to the internal data storage of type \ref QCPColorMapData. Access this to
  modify data points (cells) and the color map key/value range.
  
  \see setData
*/

/* end documentation of inline functions */

/* start documentation of signals */

/*! \fn void QCPColorMap::dataRangeChanged(QCPRange newRange);
  
  This signal is emitted when the data range changes.
  
  \see setDataRange
*/

/*! \fn void QCPColorMap::dataScaleTypeChanged(QCPAxis::ScaleType scaleType);
  
  This signal is emitted when the data scale type changes.
  
  \see setDataScaleType
*/

/*! \fn void QCPColorMap::gradientChanged(QCPColorGradient newGradient);
  
  This signal is emitted when the gradient changes.
  
  \see setGradient
*/

/* end documentation of signals */

/*!
  Constructs a color map with the specified \a keyAxis and \a valueAxis.
  
  The constructed QCPColorMap can be added to the plot with QCustomPlot::addPlottable, QCustomPlot
  then takes ownership of the color map.
*/
QCPColorMap::QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis),
  mDataScaleType(QCPAxis::stLinear),
  mMapData(new QCPColorMapData(10, 10, QCPRange(0, 5), QCPRange(0, 5))),
  mInterpolate(true),
  mTightBoundary(false),
  mMapImageInvalidated(true)
{
}

QCPColorMap::~QCPColorMap()
{
  delete mMapData;
}

/*!
  Replaces the current \ref data with the provided \a data.
  
  If \a copy is set to true, the \a data object will only be copied. if false, the color map
  takes ownership of the passed data and replaces the internal data pointer with it. This is
  significantly faster than copying for large datasets.
*/
void QCPColorMap::setData(QCPColorMapData *data, bool copy)
{
  if (copy)
  {
    *mMapData = *data;
  } else
  {
    delete mMapData;
    mMapData = data;
  }
  mMapImageInvalidated = true;
}

/*!
  Sets the data range of this color map to \a dataRange. The data range defines which data values
  are mapped to the color gradient.
  
  To make the data range span the full range of the data set, use \ref rescaleDataRange.
  
  \see QCPColorScale::setDataRange
*/
void QCPColorMap::setDataRange(const QCPRange &dataRange)
{
  if (!QCPRange::validRange(dataRange)) return;
  if (mDataRange.lower != dataRange.lower || mDataRange.upper != dataRange.upper)
  {
    if (mDataScaleType == QCPAxis::stLogarithmic)
      mDataRange = dataRange.sanitizedForLogScale();
    else
      mDataRange = dataRange.sanitizedForLinScale();
    mMapImageInvalidated = true;
    emit dataRangeChanged(mDataRange);
  }
}

/*!
  Sets whether the data is correlated with the color gradient linearly or logarithmically.
  
  \see QCPColorScale::setDataScaleType
*/
void QCPColorMap::setDataScaleType(QCPAxis::ScaleType scaleType)
{
  if (mDataScaleType != scaleType)
  {
    mDataScaleType = scaleType;
    mMapImageInvalidated = true;
    emit dataScaleTypeChanged(mDataScaleType);
    if (mDataScaleType == QCPAxis::stLogarithmic)
      setDataRange(mDataRange.sanitizedForLogScale());
  }
}

/*!
  Sets the color gradient that is used to represent the data. For more details on how to create an
  own gradient or use one of the preset gradients, see \ref QCPColorGradient.
  
  The colors defined by the gradient will be used to represent data values in the currently set
  data range, see \ref setDataRange. Data points that are outside this data range will either be
  colored uniformly with the respective gradient boundary color, or the gradient will repeat,
  depending on \ref QCPColorGradient::setPeriodic.
  
  \see QCPColorScale::setGradient
*/
void QCPColorMap::setGradient(const QCPColorGradient &gradient)
{
  if (mGradient != gradient)
  {
    mGradient = gradient;
    mMapImageInvalidated = true;
    emit gradientChanged(mGradient);
  }
}

/*!
  Sets whether the color map image shall use bicubic interpolation when displaying the color map
  shrinked or expanded, and not at a 1:1 pixel-to-data scale.
  
  \image html QCPColorMap-interpolate.png "A 10*10 color map, with interpolation and without interpolation enabled"
*/
void QCPColorMap::setInterpolate(bool enabled)
{
  mInterpolate = enabled;
}

/*!
  Sets whether the outer most data rows and columns are clipped to the specified key and value
  range (see \ref QCPColorMapData::setKeyRange, \ref QCPColorMapData::setValueRange).
  
  if \a enabled is set to false, the data points at the border of the color map are drawn with the
  same width and height as all other data points. Since the data points are represented by
  rectangles of one color centered on the data coordinate, this means that the shown color map
  extends by half a data point over the specified key/value range in each direction.
  
  \image html QCPColorMap-tightboundary.png "A color map, with tight boundary enabled and disabled"
*/
void QCPColorMap::setTightBoundary(bool enabled)
{
  mTightBoundary = enabled;
}

/*!
  Associates the color scale \a colorScale with this color map.
  
  This means that both the color scale and the color map synchronize their gradient, data range and
  data scale type (\ref setGradient, \ref setDataRange, \ref setDataScaleType). Multiple color maps
  can be associated with one single color scale. This causes the color maps to also synchronize
  those properties, via the mutual color scale.
  
  This function causes the color map to adopt the current color gradient, data range and data scale
  type of \a colorScale. After this call, you may change these properties at either the color map
  or the color scale, and the setting will be applied to both.
  
  Pass 0 as \a colorScale to disconnect the color scale from this color map again.
*/
void QCPColorMap::setColorScale(QCPColorScale *colorScale)
{
  if (mColorScale) // unconnect signals from old color scale
  {
    disconnect(this, SIGNAL(dataRangeChanged(QCPRange)), mColorScale.data(), SLOT(setDataRange(QCPRange)));
    disconnect(this, SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), mColorScale.data(), SLOT(setDataScaleType(QCPAxis::ScaleType)));
    disconnect(this, SIGNAL(gradientChanged(QCPColorGradient)), mColorScale.data(), SLOT(setGradient(QCPColorGradient)));
    disconnect(mColorScale.data(), SIGNAL(dataRangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
    disconnect(mColorScale.data(), SIGNAL(gradientChanged(QCPColorGradient)), this, SLOT(setGradient(QCPColorGradient)));
    disconnect(mColorScale.data(), SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
  }
  mColorScale = colorScale;
  if (mColorScale) // connect signals to new color scale
  {
    setGradient(mColorScale.data()->gradient());
    setDataRange(mColorScale.data()->dataRange());
    setDataScaleType(mColorScale.data()->dataScaleType());
    connect(this, SIGNAL(dataRangeChanged(QCPRange)), mColorScale.data(), SLOT(setDataRange(QCPRange)));
    connect(this, SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), mColorScale.data(), SLOT(setDataScaleType(QCPAxis::ScaleType)));
    connect(this, SIGNAL(gradientChanged(QCPColorGradient)), mColorScale.data(), SLOT(setGradient(QCPColorGradient)));
    connect(mColorScale.data(), SIGNAL(dataRangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
    connect(mColorScale.data(), SIGNAL(gradientChanged(QCPColorGradient)), this, SLOT(setGradient(QCPColorGradient)));
    connect(mColorScale.data(), SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
  }
}

/*!
  Sets the data range (\ref setDataRange) to span the minimum and maximum values that occur in the
  current data set. This corresponds to the \ref rescaleKeyAxis or \ref rescaleValueAxis methods,
  only for the third data dimension of the color map.
  
  The minimum and maximum values of the data set are buffered in the internal QCPColorMapData
  instance (\ref data). As data is updated via its \ref QCPColorMapData::setCell or \ref
  QCPColorMapData::setData, the buffered minimum and maximum values are updated, too. For
  performance reasons, however, they are only updated in an expanding fashion. So the buffered
  maximum can only increase and the buffered minimum can only decrease. In consequence, changes to
  the data that actually lower the maximum of the data set (by overwriting the cell holding the
  current maximum with a smaller value), aren't recognized and the buffered maximum overestimates
  the true maximum of the data set. The same happens for the buffered minimum. To recalculate the
  true minimum and maximum by explicitly looking at each cell, the method
  QCPColorMapData::recalculateDataBounds can be used. For convenience, setting the parameter \a
  recalculateDataBounds calls this method before setting the data range to the buffered minimum and
  maximum.
  
  \see setDataRange
*/
void QCPColorMap::rescaleDataRange(bool recalculateDataBounds)
{
  if (recalculateDataBounds)
    mMapData->recalculateDataBounds();
  setDataRange(mMapData->dataBounds());
}

/*!
  Takes the current appearance of the color map and updates the legend icon, which is used to
  represent this color map in the legend (see \ref QCPLegend).
  
  The \a transformMode specifies whether the rescaling is done by a faster, low quality image
  scaling algorithm (Qt::FastTransformation) or by a slower, higher quality algorithm
  (Qt::SmoothTransformation).
  
  The current color map appearance is scaled down to \a thumbSize. Ideally, this should be equal to
  the size of the legend icon (see \ref QCPLegend::setIconSize). If it isn't exactly the configured
  legend icon size, the thumb will be rescaled during drawing of the legend item.
  
  \see setDataRange
*/
void QCPColorMap::updateLegendIcon(Qt::TransformationMode transformMode, const QSize &thumbSize)
{
  if (mMapImage.isNull() && !data()->isEmpty())
    updateMapImage(); // try to update map image if it's null (happens if no draw has happened yet)
  
  if (!mMapImage.isNull()) // might still be null, e.g. if data is empty, so check here again
  {
    bool mirrorX = (keyAxis()->orientation() == Qt::Horizontal ? keyAxis() : valueAxis())->rangeReversed();
    bool mirrorY = (valueAxis()->orientation() == Qt::Vertical ? valueAxis() : keyAxis())->rangeReversed();
    mLegendIcon = QPixmap::fromImage(mMapImage.mirrored(mirrorX, mirrorY)).scaled(thumbSize, Qt::KeepAspectRatio, transformMode);
  }
}

/*!
  Clears the colormap data by calling \ref QCPColorMapData::clear() on the internal data. This also
  resizes the map to 0x0 cells.
*/
void QCPColorMap::clearData()
{
  mMapData->clear();
}

/* inherits documentation from base class */
double QCPColorMap::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return -1; }
  
  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()))
  {
    double posKey, posValue;
    pixelsToCoords(pos, posKey, posValue);
    if (mMapData->keyRange().contains(posKey) && mMapData->valueRange().contains(posValue))
      return mParentPlot->selectionTolerance()*0.99;
  }
  return -1;
}

/*! \internal
  
  Updates the internal map image buffer by going through the internal \ref QCPColorMapData and
  turning the data values into color pixels with \ref QCPColorGradient::colorize.
  
  This method is called by \ref QCPColorMap::draw if either the data has been modified or the map image
  has been invalidated for a different reason (e.g. a change of the data range with \ref
  setDataRange).
*/
void QCPColorMap::updateMapImage()
{
  QCPAxis *keyAxis = mKeyAxis.data();
  if (!keyAxis) return;
  
  // resize mMapImage to correct dimensions, according to key/value axes orientation:
  if (keyAxis->orientation() == Qt::Horizontal && (mMapImage.size().width() != mMapData->keySize() || mMapImage.size().height() != mMapData->valueSize()))
    mMapImage = QImage(QSize(mMapData->keySize(), mMapData->valueSize()), QImage::Format_RGB32);
  else if (keyAxis->orientation() == Qt::Vertical && (mMapImage.size().width() != mMapData->valueSize() || mMapImage.size().height() != mMapData->keySize()))
    mMapImage = QImage(QSize(mMapData->valueSize(), mMapData->keySize()), QImage::Format_RGB32);
  
  const int keySize = mMapData->keySize();
  const int valueSize = mMapData->valueSize();
  const double *rawData = mMapData->mData;
  
  if (keyAxis->orientation() == Qt::Horizontal)
  {
    const int lineCount = valueSize;
    const int rowCount = keySize;
    for (int line=0; line<lineCount; ++line)
    {
      QRgb* pixels = reinterpret_cast<QRgb*>(mMapImage.scanLine(lineCount-1-line)); // invert scanline index because QImage counts scanlines from top, but our vertical index counts from bottom (mathematical coordinate system)
      mGradient.colorize(rawData+line*rowCount, mDataRange, pixels, rowCount, 1, mDataScaleType==QCPAxis::stLogarithmic);
    }
  } else // keyAxis->orientation() == Qt::Vertical
  {
    const int lineCount = keySize;
    const int rowCount = valueSize;
    for (int line=0; line<lineCount; ++line)
    {
      QRgb* pixels = reinterpret_cast<QRgb*>(mMapImage.scanLine(lineCount-1-line)); // invert scanline index because QImage counts scanlines from top, but our vertical index counts from bottom (mathematical coordinate system)
      mGradient.colorize(rawData+line, mDataRange, pixels, rowCount, lineCount, mDataScaleType==QCPAxis::stLogarithmic);
    }
  }
  
  mMapData->mDataModified = false;
  mMapImageInvalidated = false;
}

/* inherits documentation from base class */
void QCPColorMap::draw(QCPPainter *painter)
{
  if (mMapData->isEmpty()) return;
  if (!mKeyAxis || !mValueAxis) return;
  applyDefaultAntialiasingHint(painter);
  
  if (mMapData->mDataModified || mMapImageInvalidated)
    updateMapImage();
  
  double halfSampleKey = 0;
  double halfSampleValue = 0;
  if (mMapData->keySize() > 1)
    halfSampleKey = 0.5*mMapData->keyRange().size()/(double)(mMapData->keySize()-1);
  if (mMapData->valueSize() > 1)
    halfSampleValue = 0.5*mMapData->valueRange().size()/(double)(mMapData->valueSize()-1);
  QRectF imageRect(coordsToPixels(mMapData->keyRange().lower-halfSampleKey, mMapData->valueRange().lower-halfSampleValue),
                   coordsToPixels(mMapData->keyRange().upper+halfSampleKey, mMapData->valueRange().upper+halfSampleValue));
  imageRect = imageRect.normalized();
  bool mirrorX = (keyAxis()->orientation() == Qt::Horizontal ? keyAxis() : valueAxis())->rangeReversed();
  bool mirrorY = (valueAxis()->orientation() == Qt::Vertical ? valueAxis() : keyAxis())->rangeReversed();
  bool smoothBackup = painter->renderHints().testFlag(QPainter::SmoothPixmapTransform);
  painter->setRenderHint(QPainter::SmoothPixmapTransform, mInterpolate);
  QRegion clipBackup;
  if (mTightBoundary)
  {
    clipBackup = painter->clipRegion();
    painter->setClipRect(QRectF(coordsToPixels(mMapData->keyRange().lower, mMapData->valueRange().lower),
                                coordsToPixels(mMapData->keyRange().upper, mMapData->valueRange().upper)).normalized(), Qt::IntersectClip);
  }
  painter->drawImage(imageRect, mMapImage.mirrored(mirrorX, mirrorY));
  if (mTightBoundary)
    painter->setClipRegion(clipBackup);
  painter->setRenderHint(QPainter::SmoothPixmapTransform, smoothBackup);
}

/* inherits documentation from base class */
void QCPColorMap::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
  applyDefaultAntialiasingHint(painter);
  // draw map thumbnail:
  if (!mLegendIcon.isNull())
  {
    QPixmap scaledIcon = mLegendIcon.scaled(rect.size().toSize(), Qt::KeepAspectRatio, Qt::FastTransformation);
    QRectF iconRect = QRectF(0, 0, scaledIcon.width(), scaledIcon.height());
    iconRect.moveCenter(rect.center());
    painter->drawPixmap(iconRect.topLeft(), scaledIcon);
  }
  /*
  // draw frame:
  painter->setBrush(Qt::NoBrush);
  painter->setPen(Qt::black);
  painter->drawRect(rect.adjusted(1, 1, 0, 0));
  */
}

/* inherits documentation from base class */
QCPRange QCPColorMap::getKeyRange(bool &foundRange, SignDomain inSignDomain) const
{
  foundRange = true;
  QCPRange result = mMapData->keyRange();
  result.normalize();
  if (inSignDomain == QCPAbstractPlottable::sdPositive)
  {
    if (result.lower <= 0 && result.upper > 0)
      result.lower = result.upper*1e-3;
    else if (result.lower <= 0 && result.upper <= 0)
      foundRange = false;
  } else if (inSignDomain == QCPAbstractPlottable::sdNegative)
  {
    if (result.upper >= 0 && result.lower < 0)
      result.upper = result.lower*1e-3;
    else if (result.upper >= 0 && result.lower >= 0)
      foundRange = false;
  }
  return result;
}

/* inherits documentation from base class */
QCPRange QCPColorMap::getValueRange(bool &foundRange, SignDomain inSignDomain) const
{
  foundRange = true;
  QCPRange result = mMapData->valueRange();
  result.normalize();
  if (inSignDomain == QCPAbstractPlottable::sdPositive)
  {
    if (result.lower <= 0 && result.upper > 0)
      result.lower = result.upper*1e-3;
    else if (result.lower <= 0 && result.upper <= 0)
      foundRange = false;
  } else if (inSignDomain == QCPAbstractPlottable::sdNegative)
  {
    if (result.upper >= 0 && result.lower < 0)
      result.upper = result.lower*1e-3;
    else if (result.upper >= 0 && result.lower >= 0)
      foundRange = false;
  }
  return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemStraightLine
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemStraightLine
  \brief A straight line that spans infinitely in both directions

  \image html QCPItemStraightLine.png "Straight line example. Blue dotted circles are anchors, solid blue discs are positions."

  It has two positions, \a point1 and \a point2, which define the straight line.
*/

/*!
  Creates a straight line item and sets default values.
  
  The constructed item can be added to the plot with QCustomPlot::addItem.
*/
QCPItemStraightLine::QCPItemStraightLine(QCustomPlot *parentPlot) :
  QCPAbstractItem(parentPlot),
  point1(createPosition("point1")),
  point2(createPosition("point2"))
{
  point1->setCoords(0, 0);
  point2->setCoords(1, 1);
  
  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue,2));
}

QCPItemStraightLine::~QCPItemStraightLine()
{
}

/*!
  Sets the pen that will be used to draw the line
  
  \see setSelectedPen
*/
void QCPItemStraightLine::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  Sets the pen that will be used to draw the line when selected
  
  \see setPen, setSelected
*/
void QCPItemStraightLine::setSelectedPen(const QPen &pen)
{
  mSelectedPen = pen;
}

/* inherits documentation from base class */
double QCPItemStraightLine::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  
  return distToStraightLine(QVector2D(point1->pixelPoint()), QVector2D(point2->pixelPoint()-point1->pixelPoint()), QVector2D(pos));
}

/* inherits documentation from base class */
void QCPItemStraightLine::draw(QCPPainter *painter)
{
  QVector2D start(point1->pixelPoint());
  QVector2D end(point2->pixelPoint());
  // get visible segment of straight line inside clipRect:
  double clipPad = mainPen().widthF();
  QLineF line = getRectClippedStraightLine(start, end-start, clipRect().adjusted(-clipPad, -clipPad, clipPad, clipPad));
  // paint visible segment, if existent:
  if (!line.isNull())
  {
    painter->setPen(mainPen());
    painter->drawLine(line);
  }
}

/*! \internal

  finds the shortest distance of \a point to the straight line defined by the base point \a
  base and the direction vector \a vec.
  
  This is a helper function for \ref selectTest.
*/
double QCPItemStraightLine::distToStraightLine(const QVector2D &base, const QVector2D &vec, const QVector2D &point) const
{
  return qAbs((base.y()-point.y())*vec.x()-(base.x()-point.x())*vec.y())/vec.length();
}

/*! \internal

  Returns the section of the straight line defined by \a base and direction vector \a
  vec, that is visible in the specified \a rect.
  
  This is a helper function for \ref draw.
*/
QLineF QCPItemStraightLine::getRectClippedStraightLine(const QVector2D &base, const QVector2D &vec, const QRect &rect) const
{
  double bx, by;
  double gamma;
  QLineF result;
  if (vec.x() == 0 && vec.y() == 0)
    return result;
  if (qFuzzyIsNull(vec.x())) // line is vertical
  {
    // check top of rect:
    bx = rect.left();
    by = rect.top();
    gamma = base.x()-bx + (by-base.y())*vec.x()/vec.y();
    if (gamma >= 0 && gamma <= rect.width())
      result.setLine(bx+gamma, rect.top(), bx+gamma, rect.bottom()); // no need to check bottom because we know line is vertical
  } else if (qFuzzyIsNull(vec.y())) // line is horizontal
  {
    // check left of rect:
    bx = rect.left();
    by = rect.top();
    gamma = base.y()-by + (bx-base.x())*vec.y()/vec.x();
    if (gamma >= 0 && gamma <= rect.height())
      result.setLine(rect.left(), by+gamma, rect.right(), by+gamma); // no need to check right because we know line is horizontal
  } else // line is skewed
  {
    QList<QVector2D> pointVectors;
    // check top of rect:
    bx = rect.left();
    by = rect.top();
    gamma = base.x()-bx + (by-base.y())*vec.x()/vec.y();
    if (gamma >= 0 && gamma <= rect.width())
      pointVectors.append(QVector2D(bx+gamma, by));
    // check bottom of rect:
    bx = rect.left();
    by = rect.bottom();
    gamma = base.x()-bx + (by-base.y())*vec.x()/vec.y();
    if (gamma >= 0 && gamma <= rect.width())
      pointVectors.append(QVector2D(bx+gamma, by));
    // check left of rect:
    bx = rect.left();
    by = rect.top();
    gamma = base.y()-by + (bx-base.x())*vec.y()/vec.x();
    if (gamma >= 0 && gamma <= rect.height())
      pointVectors.append(QVector2D(bx, by+gamma));
    // check right of rect:
    bx = rect.right();
    by = rect.top();
    gamma = base.y()-by + (bx-base.x())*vec.y()/vec.x();
    if (gamma >= 0 && gamma <= rect.height())
      pointVectors.append(QVector2D(bx, by+gamma));
    
    // evaluate points:
    if (pointVectors.size() == 2)
    {
      result.setPoints(pointVectors.at(0).toPointF(), pointVectors.at(1).toPointF());
    } else if (pointVectors.size() > 2)
    {
      // line probably goes through corner of rect, and we got two points there. single out the point pair with greatest distance:
      double distSqrMax = 0;
      QVector2D pv1, pv2;
      for (int i=0; i<pointVectors.size()-1; ++i)
      {
        for (int k=i+1; k<pointVectors.size(); ++k)
        {
          double distSqr = (pointVectors.at(i)-pointVectors.at(k)).lengthSquared();
          if (distSqr > distSqrMax)
          {
            pv1 = pointVectors.at(i);
            pv2 = pointVectors.at(k);
            distSqrMax = distSqr;
          }
        }
      }
      result.setPoints(pv1.toPointF(), pv2.toPointF());
    }
  }
  return result;
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemStraightLine::mainPen() const
{
  return mSelected ? mSelectedPen : mPen;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemLine
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemLine
  \brief A line from one point to another

  \image html QCPItemLine.png "Line example. Blue dotted circles are anchors, solid blue discs are positions."

  It has two positions, \a start and \a end, which define the end points of the line.
  
  With \ref setHead and \ref setTail you may set different line ending styles, e.g. to create an arrow.
*/

/*!
  Creates a line item and sets default values.
  
  The constructed item can be added to the plot with QCustomPlot::addItem.
*/
QCPItemLine::QCPItemLine(QCustomPlot *parentPlot) :
  QCPAbstractItem(parentPlot),
  start(createPosition("start")),
  end(createPosition("end"))
{
  start->setCoords(0, 0);
  end->setCoords(1, 1);
  
  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue,2));
}

QCPItemLine::~QCPItemLine()
{
}

/*!
  Sets the pen that will be used to draw the line
  
  \see setSelectedPen
*/
void QCPItemLine::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  Sets the pen that will be used to draw the line when selected
  
  \see setPen, setSelected
*/
void QCPItemLine::setSelectedPen(const QPen &pen)
{
  mSelectedPen = pen;
}

/*!
  Sets the line ending style of the head. The head corresponds to the \a end position.
  
  Note that due to the overloaded QCPLineEnding constructor, you may directly specify
  a QCPLineEnding::EndingStyle here, e.g. \code setHead(QCPLineEnding::esSpikeArrow) \endcode
  
  \see setTail
*/
void QCPItemLine::setHead(const QCPLineEnding &head)
{
  mHead = head;
}

/*!
  Sets the line ending style of the tail. The tail corresponds to the \a start position.
  
  Note that due to the overloaded QCPLineEnding constructor, you may directly specify
  a QCPLineEnding::EndingStyle here, e.g. \code setTail(QCPLineEnding::esSpikeArrow) \endcode
  
  \see setHead
*/
void QCPItemLine::setTail(const QCPLineEnding &tail)
{
  mTail = tail;
}

/* inherits documentation from base class */
double QCPItemLine::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  
  return qSqrt(distSqrToLine(start->pixelPoint(), end->pixelPoint(), pos));
}

/* inherits documentation from base class */
void QCPItemLine::draw(QCPPainter *painter)
{
  QVector2D startVec(start->pixelPoint());
  QVector2D endVec(end->pixelPoint());
  if (startVec.toPoint() == endVec.toPoint())
    return;
  // get visible segment of straight line inside clipRect:
  double clipPad = qMax(mHead.boundingDistance(), mTail.boundingDistance());
  clipPad = qMax(clipPad, (double)mainPen().widthF());
  QLineF line = getRectClippedLine(startVec, endVec, clipRect().adjusted(-clipPad, -clipPad, clipPad, clipPad));
  // paint visible segment, if existent:
  if (!line.isNull())
  {
    painter->setPen(mainPen());
    painter->drawLine(line);
    painter->setBrush(Qt::SolidPattern);
    if (mTail.style() != QCPLineEnding::esNone)
      mTail.draw(painter, startVec, startVec-endVec);
    if (mHead.style() != QCPLineEnding::esNone)
      mHead.draw(painter, endVec, endVec-startVec);
  }
}

/*! \internal

  Returns the section of the line defined by \a start and \a end, that is visible in the specified
  \a rect.
  
  This is a helper function for \ref draw.
*/
QLineF QCPItemLine::getRectClippedLine(const QVector2D &start, const QVector2D &end, const QRect &rect) const
{
  bool containsStart = rect.contains(start.x(), start.y());
  bool containsEnd = rect.contains(end.x(), end.y());
  if (containsStart && containsEnd)
    return QLineF(start.toPointF(), end.toPointF());
  
  QVector2D base = start;
  QVector2D vec = end-start;
  double bx, by;
  double gamma, mu;
  QLineF result;
  QList<QVector2D> pointVectors;

  if (!qFuzzyIsNull(vec.y())) // line is not horizontal
  {
    // check top of rect:
    bx = rect.left();
    by = rect.top();
    mu = (by-base.y())/vec.y();
    if (mu >= 0 && mu <= 1)
    {
      gamma = base.x()-bx + mu*vec.x();
      if (gamma >= 0 && gamma <= rect.width())
        pointVectors.append(QVector2D(bx+gamma, by));
    }
    // check bottom of rect:
    bx = rect.left();
    by = rect.bottom();
    mu = (by-base.y())/vec.y();
    if (mu >= 0 && mu <= 1)
    {
      gamma = base.x()-bx + mu*vec.x();
      if (gamma >= 0 && gamma <= rect.width())
        pointVectors.append(QVector2D(bx+gamma, by));
    }
  }
  if (!qFuzzyIsNull(vec.x())) // line is not vertical
  {
    // check left of rect:
    bx = rect.left();
    by = rect.top();
    mu = (bx-base.x())/vec.x();
    if (mu >= 0 && mu <= 1)
    {
      gamma = base.y()-by + mu*vec.y();
      if (gamma >= 0 && gamma <= rect.height())
        pointVectors.append(QVector2D(bx, by+gamma));
    }
    // check right of rect:
    bx = rect.right();
    by = rect.top();
    mu = (bx-base.x())/vec.x();
    if (mu >= 0 && mu <= 1)
    {
      gamma = base.y()-by + mu*vec.y();
      if (gamma >= 0 && gamma <= rect.height())
        pointVectors.append(QVector2D(bx, by+gamma));
    }
  }
  
  if (containsStart)
    pointVectors.append(start);
  if (containsEnd)
    pointVectors.append(end);
  
  // evaluate points:
  if (pointVectors.size() == 2)
  {
    result.setPoints(pointVectors.at(0).toPointF(), pointVectors.at(1).toPointF());
  } else if (pointVectors.size() > 2)
  {
    // line probably goes through corner of rect, and we got two points there. single out the point pair with greatest distance:
    double distSqrMax = 0;
    QVector2D pv1, pv2;
    for (int i=0; i<pointVectors.size()-1; ++i)
    {
      for (int k=i+1; k<pointVectors.size(); ++k)
      {
        double distSqr = (pointVectors.at(i)-pointVectors.at(k)).lengthSquared();
        if (distSqr > distSqrMax)
        {
          pv1 = pointVectors.at(i);
          pv2 = pointVectors.at(k);
          distSqrMax = distSqr;
        }
      }
    }
    result.setPoints(pv1.toPointF(), pv2.toPointF());
  }
  return result;
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemLine::mainPen() const
{
  return mSelected ? mSelectedPen : mPen;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemCurve
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemCurve
  \brief A curved line from one point to another

  \image html QCPItemCurve.png "Curve example. Blue dotted circles are anchors, solid blue discs are positions."

  It has four positions, \a start and \a end, which define the end points of the line, and two
  control points which define the direction the line exits from the start and the direction from
  which it approaches the end: \a startDir and \a endDir.
  
  With \ref setHead and \ref setTail you may set different line ending styles, e.g. to create an
  arrow.
  
  Often it is desirable for the control points to stay at fixed relative positions to the start/end
  point. This can be achieved by setting the parent anchor e.g. of \a startDir simply to \a start,
  and then specify the desired pixel offset with QCPItemPosition::setCoords on \a startDir.
*/

/*!
  Creates a curve item and sets default values.
  
  The constructed item can be added to the plot with QCustomPlot::addItem.
*/
QCPItemCurve::QCPItemCurve(QCustomPlot *parentPlot) :
  QCPAbstractItem(parentPlot),
  start(createPosition("start")),
  startDir(createPosition("startDir")),
  endDir(createPosition("endDir")),
  end(createPosition("end"))
{
  start->setCoords(0, 0);
  startDir->setCoords(0.5, 0);
  endDir->setCoords(0, 0.5);
  end->setCoords(1, 1);
  
  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue,2));
}

QCPItemCurve::~QCPItemCurve()
{
}

/*!
  Sets the pen that will be used to draw the line
  
  \see setSelectedPen
*/
void QCPItemCurve::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  Sets the pen that will be used to draw the line when selected
  
  \see setPen, setSelected
*/
void QCPItemCurve::setSelectedPen(const QPen &pen)
{
  mSelectedPen = pen;
}

/*!
  Sets the line ending style of the head. The head corresponds to the \a end position.
  
  Note that due to the overloaded QCPLineEnding constructor, you may directly specify
  a QCPLineEnding::EndingStyle here, e.g. \code setHead(QCPLineEnding::esSpikeArrow) \endcode
  
  \see setTail
*/
void QCPItemCurve::setHead(const QCPLineEnding &head)
{
  mHead = head;
}

/*!
  Sets the line ending style of the tail. The tail corresponds to the \a start position.
  
  Note that due to the overloaded QCPLineEnding constructor, you may directly specify
  a QCPLineEnding::EndingStyle here, e.g. \code setTail(QCPLineEnding::esSpikeArrow) \endcode
  
  \see setHead
*/
void QCPItemCurve::setTail(const QCPLineEnding &tail)
{
  mTail = tail;
}

/* inherits documentation from base class */
double QCPItemCurve::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  
  QPointF startVec(start->pixelPoint());
  QPointF startDirVec(startDir->pixelPoint());
  QPointF endDirVec(endDir->pixelPoint());
  QPointF endVec(end->pixelPoint());

  QPainterPath cubicPath(startVec);
  cubicPath.cubicTo(startDirVec, endDirVec, endVec);
  
  QPolygonF polygon = cubicPath.toSubpathPolygons().first();
  double minDistSqr = std::numeric_limits<double>::max();
  for (int i=1; i<polygon.size(); ++i)
  {
    double distSqr = distSqrToLine(polygon.at(i-1), polygon.at(i), pos);
    if (distSqr < minDistSqr)
      minDistSqr = distSqr;
  }
  return qSqrt(minDistSqr);
}

/* inherits documentation from base class */
void QCPItemCurve::draw(QCPPainter *painter)
{
  QPointF startVec(start->pixelPoint());
  QPointF startDirVec(startDir->pixelPoint());
  QPointF endDirVec(endDir->pixelPoint());
  QPointF endVec(end->pixelPoint());
  if (QVector2D(endVec-startVec).length() > 1e10f) // too large curves cause crash
    return;

  QPainterPath cubicPath(startVec);
  cubicPath.cubicTo(startDirVec, endDirVec, endVec);

  // paint visible segment, if existent:
  QRect clip = clipRect().adjusted(-mainPen().widthF(), -mainPen().widthF(), mainPen().widthF(), mainPen().widthF());
  QRect cubicRect = cubicPath.controlPointRect().toRect();
  if (cubicRect.isEmpty()) // may happen when start and end exactly on same x or y position
    cubicRect.adjust(0, 0, 1, 1);
  if (clip.intersects(cubicRect))
  {
    painter->setPen(mainPen());
    painter->drawPath(cubicPath);
    painter->setBrush(Qt::SolidPattern);
    if (mTail.style() != QCPLineEnding::esNone)
      mTail.draw(painter, QVector2D(startVec), M_PI-cubicPath.angleAtPercent(0)/180.0*M_PI);
    if (mHead.style() != QCPLineEnding::esNone)
      mHead.draw(painter, QVector2D(endVec), -cubicPath.angleAtPercent(1)/180.0*M_PI);
  }
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemCurve::mainPen() const
{
  return mSelected ? mSelectedPen : mPen;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemRect
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemRect
  \brief A rectangle

  \image html QCPItemRect.png "Rectangle example. Blue dotted circles are anchors, solid blue discs are positions."

  It has two positions, \a topLeft and \a bottomRight, which define the rectangle.
*/

/*!
  Creates a rectangle item and sets default values.
  
  The constructed item can be added to the plot with QCustomPlot::addItem.
*/
QCPItemRect::QCPItemRect(QCustomPlot *parentPlot) :
  QCPAbstractItem(parentPlot),
  topLeft(createPosition("topLeft")),
  bottomRight(createPosition("bottomRight")),
  top(createAnchor("top", aiTop)),
  topRight(createAnchor("topRight", aiTopRight)),
  right(createAnchor("right", aiRight)),
  bottom(createAnchor("bottom", aiBottom)),
  bottomLeft(createAnchor("bottomLeft", aiBottomLeft)),
  left(createAnchor("left", aiLeft))
{
  topLeft->setCoords(0, 1);
  bottomRight->setCoords(1, 0);
  
  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue,2));
  setBrush(Qt::NoBrush);
  setSelectedBrush(Qt::NoBrush);
}

QCPItemRect::~QCPItemRect()
{
}

/*!
  Sets the pen that will be used to draw the line of the rectangle
  
  \see setSelectedPen, setBrush
*/
void QCPItemRect::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  Sets the pen that will be used to draw the line of the rectangle when selected
  
  \see setPen, setSelected
*/
void QCPItemRect::setSelectedPen(const QPen &pen)
{
  mSelectedPen = pen;
}

/*!
  Sets the brush that will be used to fill the rectangle. To disable filling, set \a brush to
  Qt::NoBrush.
  
  \see setSelectedBrush, setPen
*/
void QCPItemRect::setBrush(const QBrush &brush)
{
  mBrush = brush;
}

/*!
  Sets the brush that will be used to fill the rectangle when selected. To disable filling, set \a
  brush to Qt::NoBrush.
  
  \see setBrush
*/
void QCPItemRect::setSelectedBrush(const QBrush &brush)
{
  mSelectedBrush = brush;
}

/* inherits documentation from base class */
double QCPItemRect::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  
  QRectF rect = QRectF(topLeft->pixelPoint(), bottomRight->pixelPoint()).normalized();
  bool filledRect = mBrush.style() != Qt::NoBrush && mBrush.color().alpha() != 0;
  return rectSelectTest(rect, pos, filledRect);
}

/* inherits documentation from base class */
void QCPItemRect::draw(QCPPainter *painter)
{
  QPointF p1 = topLeft->pixelPoint();
  QPointF p2 = bottomRight->pixelPoint();
  if (p1.toPoint() == p2.toPoint())
    return;
  QRectF rect = QRectF(p1, p2).normalized();
  double clipPad = mainPen().widthF();
  QRectF boundingRect = rect.adjusted(-clipPad, -clipPad, clipPad, clipPad);
  if (boundingRect.intersects(clipRect())) // only draw if bounding rect of rect item is visible in cliprect
  {
    painter->setPen(mainPen());
    painter->setBrush(mainBrush());
    painter->drawRect(rect);
  }
}

/* inherits documentation from base class */
QPointF QCPItemRect::anchorPixelPoint(int anchorId) const
{
  QRectF rect = QRectF(topLeft->pixelPoint(), bottomRight->pixelPoint());
  switch (anchorId)
  {
    case aiTop:         return (rect.topLeft()+rect.topRight())*0.5;
    case aiTopRight:    return rect.topRight();
    case aiRight:       return (rect.topRight()+rect.bottomRight())*0.5;
    case aiBottom:      return (rect.bottomLeft()+rect.bottomRight())*0.5;
    case aiBottomLeft:  return rect.bottomLeft();
    case aiLeft:        return (rect.topLeft()+rect.bottomLeft())*0.5;
  }
  
  qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
  return QPointF();
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the item is not selected
  and mSelectedPen when it is.
*/
QPen QCPItemRect::mainPen() const
{
  return mSelected ? mSelectedPen : mPen;
}

/*! \internal

  Returns the brush that should be used for drawing fills of the item. Returns mBrush when the item
  is not selected and mSelectedBrush when it is.
*/
QBrush QCPItemRect::mainBrush() const
{
  return mSelected ? mSelectedBrush : mBrush;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemText
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemText
  \brief A text label

  \image html QCPItemText.png "Text example. Blue dotted circles are anchors, solid blue discs are positions."

  Its position is defined by the member \a position and the setting of \ref setPositionAlignment.
  The latter controls which part of the text rect shall be aligned with \a position.
  
  The text alignment itself (i.e. left, center, right) can be controlled with \ref
  setTextAlignment.
  
  The text may be rotated around the \a position point with \ref setRotation.
*/

/*!
  Creates a text item and sets default values.
  
  The constructed item can be added to the plot with QCustomPlot::addItem.
*/
QCPItemText::QCPItemText(QCustomPlot *parentPlot) :
  QCPAbstractItem(parentPlot),
  position(createPosition("position")),
  topLeft(createAnchor("topLeft", aiTopLeft)),
  top(createAnchor("top", aiTop)),
  topRight(createAnchor("topRight", aiTopRight)),
  right(createAnchor("right", aiRight)),
  bottomRight(createAnchor("bottomRight", aiBottomRight)),
  bottom(createAnchor("bottom", aiBottom)),
  bottomLeft(createAnchor("bottomLeft", aiBottomLeft)),
  left(createAnchor("left", aiLeft))
{
  position->setCoords(0, 0);
  
  setRotation(0);
  setTextAlignment(Qt::AlignTop|Qt::AlignHCenter);
  setPositionAlignment(Qt::AlignCenter);
  setText("text");
  
  setPen(Qt::NoPen);
  setSelectedPen(Qt::NoPen);
  setBrush(Qt::NoBrush);
  setSelectedBrush(Qt::NoBrush);
  setColor(Qt::black);
  setSelectedColor(Qt::blue);
}

QCPItemText::~QCPItemText()
{
}

/*!
  Sets the color of the text.
*/
void QCPItemText::setColor(const QColor &color)
{
  mColor = color;
}

/*!
  Sets the color of the text that will be used when the item is selected.
*/
void QCPItemText::setSelectedColor(const QColor &color)
{
  mSelectedColor = color;
}

/*!
  Sets the pen that will be used do draw a rectangular border around the text. To disable the
  border, set \a pen to Qt::NoPen.
  
  \see setSelectedPen, setBrush, setPadding
*/
void QCPItemText::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  Sets the pen that will be used do draw a rectangular border around the text, when the item is
  selected. To disable the border, set \a pen to Qt::NoPen.
  
  \see setPen
*/
void QCPItemText::setSelectedPen(const QPen &pen)
{
  mSelectedPen = pen;
}

/*!
  Sets the brush that will be used do fill the background of the text. To disable the
  background, set \a brush to Qt::NoBrush.
  
  \see setSelectedBrush, setPen, setPadding
*/
void QCPItemText::setBrush(const QBrush &brush)
{
  mBrush = brush;
}

/*!
  Sets the brush that will be used do fill the background of the text, when the item is selected. To disable the
  background, set \a brush to Qt::NoBrush.
  
  \see setBrush
*/
void QCPItemText::setSelectedBrush(const QBrush &brush)
{
  mSelectedBrush = brush;
}

/*!
  Sets the font of the text.
  
  \see setSelectedFont, setColor
*/
void QCPItemText::setFont(const QFont &font)
{
  mFont = font;
}

/*!
  Sets the font of the text that will be used when the item is selected.
  
  \see setFont
*/
void QCPItemText::setSelectedFont(const QFont &font)
{
  mSelectedFont = font;
}

/*!
  Sets the text that will be displayed. Multi-line texts are supported by inserting a line break
  character, e.g. '\n'.
  
  \see setFont, setColor, setTextAlignment
*/
void QCPItemText::setText(const QString &text)
{
  mText = text;
}

/*!
  Sets which point of the text rect shall be aligned with \a position.
  
  Examples:
  \li If \a alignment is <tt>Qt::AlignHCenter | Qt::AlignTop</tt>, the text will be positioned such
  that the top of the text rect will be horizontally centered on \a position.
  \li If \a alignment is <tt>Qt::AlignLeft | Qt::AlignBottom</tt>, \a position will indicate the
  bottom left corner of the text rect.
  
  If you want to control the alignment of (multi-lined) text within the text rect, use \ref
  setTextAlignment.
*/
void QCPItemText::setPositionAlignment(Qt::Alignment alignment)
{
  mPositionAlignment = alignment;
}

/*!
  Controls how (multi-lined) text is aligned inside the text rect (typically Qt::AlignLeft, Qt::AlignCenter or Qt::AlignRight).
*/
void QCPItemText::setTextAlignment(Qt::Alignment alignment)
{
  mTextAlignment = alignment;
}

/*!
  Sets the angle in degrees by which the text (and the text rectangle, if visible) will be rotated
  around \a position.
*/
void QCPItemText::setRotation(double degrees)
{
  mRotation = degrees;
}

/*!
  Sets the distance between the border of the text rectangle and the text. The appearance (and
  visibility) of the text rectangle can be controlled with \ref setPen and \ref setBrush.
*/
void QCPItemText::setPadding(const QMargins &padding)
{
  mPadding = padding;
}

/* inherits documentation from base class */
double QCPItemText::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  
  // The rect may be rotated, so we transform the actual clicked pos to the rotated
  // coordinate system, so we can use the normal rectSelectTest function for non-rotated rects:
  QPointF positionPixels(position->pixelPoint());
  QTransform inputTransform;
  inputTransform.translate(positionPixels.x(), positionPixels.y());
  inputTransform.rotate(-mRotation);
  inputTransform.translate(-positionPixels.x(), -positionPixels.y());
  QPointF rotatedPos = inputTransform.map(pos);
  QFontMetrics fontMetrics(mFont);
  QRect textRect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip|mTextAlignment, mText);
  QRect textBoxRect = textRect.adjusted(-mPadding.left(), -mPadding.top(), mPadding.right(), mPadding.bottom());
  QPointF textPos = getTextDrawPoint(positionPixels, textBoxRect, mPositionAlignment);
  textBoxRect.moveTopLeft(textPos.toPoint());

  return rectSelectTest(textBoxRect, rotatedPos, true);
}

/* inherits documentation from base class */
void QCPItemText::draw(QCPPainter *painter)
{
  QPointF pos(position->pixelPoint());
  QTransform transform = painter->transform();
  transform.translate(pos.x(), pos.y());
  if (!qFuzzyIsNull(mRotation))
    transform.rotate(mRotation);
  painter->setFont(mainFont());
  QRect textRect = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip|mTextAlignment, mText);
  QRect textBoxRect = textRect.adjusted(-mPadding.left(), -mPadding.top(), mPadding.right(), mPadding.bottom());
  QPointF textPos = getTextDrawPoint(QPointF(0, 0), textBoxRect, mPositionAlignment); // 0, 0 because the transform does the translation
  textRect.moveTopLeft(textPos.toPoint()+QPoint(mPadding.left(), mPadding.top()));
  textBoxRect.moveTopLeft(textPos.toPoint());
  double clipPad = mainPen().widthF();
  QRect boundingRect = textBoxRect.adjusted(-clipPad, -clipPad, clipPad, clipPad);
  if (transform.mapRect(boundingRect).intersects(painter->transform().mapRect(clipRect())))
  {
    painter->setTransform(transform);
    if ((mainBrush().style() != Qt::NoBrush && mainBrush().color().alpha() != 0) ||
        (mainPen().style() != Qt::NoPen && mainPen().color().alpha() != 0))
    {
      painter->setPen(mainPen());
      painter->setBrush(mainBrush());
      painter->drawRect(textBoxRect);
    }
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(mainColor()));
    painter->drawText(textRect, Qt::TextDontClip|mTextAlignment, mText);
  }
}

/* inherits documentation from base class */
QPointF QCPItemText::anchorPixelPoint(int anchorId) const
{
  // get actual rect points (pretty much copied from draw function):
  QPointF pos(position->pixelPoint());
  QTransform transform;
  transform.translate(pos.x(), pos.y());
  if (!qFuzzyIsNull(mRotation))
    transform.rotate(mRotation);
  QFontMetrics fontMetrics(mainFont());
  QRect textRect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip|mTextAlignment, mText);
  QRectF textBoxRect = textRect.adjusted(-mPadding.left(), -mPadding.top(), mPadding.right(), mPadding.bottom());
  QPointF textPos = getTextDrawPoint(QPointF(0, 0), textBoxRect, mPositionAlignment); // 0, 0 because the transform does the translation
  textBoxRect.moveTopLeft(textPos.toPoint());
  QPolygonF rectPoly = transform.map(QPolygonF(textBoxRect));
  
  switch (anchorId)
  {
    case aiTopLeft:     return rectPoly.at(0);
    case aiTop:         return (rectPoly.at(0)+rectPoly.at(1))*0.5;
    case aiTopRight:    return rectPoly.at(1);
    case aiRight:       return (rectPoly.at(1)+rectPoly.at(2))*0.5;
    case aiBottomRight: return rectPoly.at(2);
    case aiBottom:      return (rectPoly.at(2)+rectPoly.at(3))*0.5;
    case aiBottomLeft:  return rectPoly.at(3);
    case aiLeft:        return (rectPoly.at(3)+rectPoly.at(0))*0.5;
  }
  
  qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
  return QPointF();
}

/*! \internal
  
  Returns the point that must be given to the QPainter::drawText function (which expects the top
  left point of the text rect), according to the position \a pos, the text bounding box \a rect and
  the requested \a positionAlignment.
  
  For example, if \a positionAlignment is <tt>Qt::AlignLeft | Qt::AlignBottom</tt> the returned point
  will be shifted upward by the height of \a rect, starting from \a pos. So if the text is finally
  drawn at that point, the lower left corner of the resulting text rect is at \a pos.
*/
QPointF QCPItemText::getTextDrawPoint(const QPointF &pos, const QRectF &rect, Qt::Alignment positionAlignment) const
{
  if (positionAlignment == 0 || positionAlignment == (Qt::AlignLeft|Qt::AlignTop))
    return pos;
  
  QPointF result = pos; // start at top left
  if (positionAlignment.testFlag(Qt::AlignHCenter))
    result.rx() -= rect.width()/2.0;
  else if (positionAlignment.testFlag(Qt::AlignRight))
    result.rx() -= rect.width();
  if (positionAlignment.testFlag(Qt::AlignVCenter))
    result.ry() -= rect.height()/2.0;
  else if (positionAlignment.testFlag(Qt::AlignBottom))
    result.ry() -= rect.height();
  return result;
}

/*! \internal

  Returns the font that should be used for drawing text. Returns mFont when the item is not selected
  and mSelectedFont when it is.
*/
QFont QCPItemText::mainFont() const
{
  return mSelected ? mSelectedFont : mFont;
}

/*! \internal

  Returns the color that should be used for drawing text. Returns mColor when the item is not
  selected and mSelectedColor when it is.
*/
QColor QCPItemText::mainColor() const
{
  return mSelected ? mSelectedColor : mColor;
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the item is not selected
  and mSelectedPen when it is.
*/
QPen QCPItemText::mainPen() const
{
  return mSelected ? mSelectedPen : mPen;
}

/*! \internal

  Returns the brush that should be used for drawing fills of the item. Returns mBrush when the item
  is not selected and mSelectedBrush when it is.
*/
QBrush QCPItemText::mainBrush() const
{
  return mSelected ? mSelectedBrush : mBrush;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemEllipse
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemEllipse
  \brief An ellipse

  \image html QCPItemEllipse.png "Ellipse example. Blue dotted circles are anchors, solid blue discs are positions."

  It has two positions, \a topLeft and \a bottomRight, which define the rect the ellipse will be drawn in.
*/

/*!
  Creates an ellipse item and sets default values.
  
  The constructed item can be added to the plot with QCustomPlot::addItem.
*/
QCPItemEllipse::QCPItemEllipse(QCustomPlot *parentPlot) :
  QCPAbstractItem(parentPlot),
  topLeft(createPosition("topLeft")),
  bottomRight(createPosition("bottomRight")),
  topLeftRim(createAnchor("topLeftRim", aiTopLeftRim)),
  top(createAnchor("top", aiTop)),
  topRightRim(createAnchor("topRightRim", aiTopRightRim)),
  right(createAnchor("right", aiRight)),
  bottomRightRim(createAnchor("bottomRightRim", aiBottomRightRim)),
  bottom(createAnchor("bottom", aiBottom)),
  bottomLeftRim(createAnchor("bottomLeftRim", aiBottomLeftRim)),
  left(createAnchor("left", aiLeft)),
  center(createAnchor("center", aiCenter))
{
  topLeft->setCoords(0, 1);
  bottomRight->setCoords(1, 0);
  
  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue, 2));
  setBrush(Qt::NoBrush);
  setSelectedBrush(Qt::NoBrush);
}

QCPItemEllipse::~QCPItemEllipse()
{
}

/*!
  Sets the pen that will be used to draw the line of the ellipse
  
  \see setSelectedPen, setBrush
*/
void QCPItemEllipse::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  Sets the pen that will be used to draw the line of the ellipse when selected
  
  \see setPen, setSelected
*/
void QCPItemEllipse::setSelectedPen(const QPen &pen)
{
  mSelectedPen = pen;
}

/*!
  Sets the brush that will be used to fill the ellipse. To disable filling, set \a brush to
  Qt::NoBrush.
  
  \see setSelectedBrush, setPen
*/
void QCPItemEllipse::setBrush(const QBrush &brush)
{
  mBrush = brush;
}

/*!
  Sets the brush that will be used to fill the ellipse when selected. To disable filling, set \a
  brush to Qt::NoBrush.
  
  \see setBrush
*/
void QCPItemEllipse::setSelectedBrush(const QBrush &brush)
{
  mSelectedBrush = brush;
}

/* inherits documentation from base class */
double QCPItemEllipse::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  
  double result = -1;
  QPointF p1 = topLeft->pixelPoint();
  QPointF p2 = bottomRight->pixelPoint();
  QPointF center((p1+p2)/2.0);
  double a = qAbs(p1.x()-p2.x())/2.0;
  double b = qAbs(p1.y()-p2.y())/2.0;
  double x = pos.x()-center.x();
  double y = pos.y()-center.y();
  
  // distance to border:
  double c = 1.0/qSqrt(x*x/(a*a)+y*y/(b*b));
  result = qAbs(c-1)*qSqrt(x*x+y*y);
  // filled ellipse, allow click inside to count as hit:
  if (result > mParentPlot->selectionTolerance()*0.99 && mBrush.style() != Qt::NoBrush && mBrush.color().alpha() != 0)
  {
    if (x*x/(a*a) + y*y/(b*b) <= 1)
      result = mParentPlot->selectionTolerance()*0.99;
  }
  return result;
}

/* inherits documentation from base class */
void QCPItemEllipse::draw(QCPPainter *painter)
{
  QPointF p1 = topLeft->pixelPoint();
  QPointF p2 = bottomRight->pixelPoint();
  if (p1.toPoint() == p2.toPoint())
    return;
  QRectF ellipseRect = QRectF(p1, p2).normalized();
  QRect clip = clipRect().adjusted(-mainPen().widthF(), -mainPen().widthF(), mainPen().widthF(), mainPen().widthF());
  if (ellipseRect.intersects(clip)) // only draw if bounding rect of ellipse is visible in cliprect
  {
    painter->setPen(mainPen());
    painter->setBrush(mainBrush());
#ifdef __EXCEPTIONS
    try // drawEllipse sometimes throws exceptions if ellipse is too big
    {
#endif
      painter->drawEllipse(ellipseRect);
#ifdef __EXCEPTIONS
    } catch (...)
    {
      qDebug() << Q_FUNC_INFO << "Item too large for memory, setting invisible";
      setVisible(false);
    }
#endif
  }
}

/* inherits documentation from base class */
QPointF QCPItemEllipse::anchorPixelPoint(int anchorId) const
{
  QRectF rect = QRectF(topLeft->pixelPoint(), bottomRight->pixelPoint());
  switch (anchorId)
  {
    case aiTopLeftRim:     return rect.center()+(rect.topLeft()-rect.center())*1/qSqrt(2);
    case aiTop:            return (rect.topLeft()+rect.topRight())*0.5;
    case aiTopRightRim:    return rect.center()+(rect.topRight()-rect.center())*1/qSqrt(2);
    case aiRight:          return (rect.topRight()+rect.bottomRight())*0.5;
    case aiBottomRightRim: return rect.center()+(rect.bottomRight()-rect.center())*1/qSqrt(2);
    case aiBottom:         return (rect.bottomLeft()+rect.bottomRight())*0.5;
    case aiBottomLeftRim:  return rect.center()+(rect.bottomLeft()-rect.center())*1/qSqrt(2);
    case aiLeft:           return (rect.topLeft()+rect.bottomLeft())*0.5;
    case aiCenter:         return (rect.topLeft()+rect.bottomRight())*0.5;
  }
  
  qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
  return QPointF();
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the item is not selected
  and mSelectedPen when it is.
*/
QPen QCPItemEllipse::mainPen() const
{
  return mSelected ? mSelectedPen : mPen;
}

/*! \internal

  Returns the brush that should be used for drawing fills of the item. Returns mBrush when the item
  is not selected and mSelectedBrush when it is.
*/
QBrush QCPItemEllipse::mainBrush() const
{
  return mSelected ? mSelectedBrush : mBrush;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemPixmap
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemPixmap
  \brief An arbitrary pixmap

  \image html QCPItemPixmap.png "Pixmap example. Blue dotted circles are anchors, solid blue discs are positions."

  It has two positions, \a topLeft and \a bottomRight, which define the rectangle the pixmap will
  be drawn in. Depending on the scale setting (\ref setScaled), the pixmap will be either scaled to
  fit the rectangle or be drawn aligned to the topLeft position.
  
  If scaling is enabled and \a topLeft is further to the bottom/right than \a bottomRight (as shown
  on the right side of the example image), the pixmap will be flipped in the respective
  orientations.
*/

/*!
  Creates a rectangle item and sets default values.
  
  The constructed item can be added to the plot with QCustomPlot::addItem.
*/
QCPItemPixmap::QCPItemPixmap(QCustomPlot *parentPlot) :
  QCPAbstractItem(parentPlot),
  topLeft(createPosition("topLeft")),
  bottomRight(createPosition("bottomRight")),
  top(createAnchor("top", aiTop)),
  topRight(createAnchor("topRight", aiTopRight)),
  right(createAnchor("right", aiRight)),
  bottom(createAnchor("bottom", aiBottom)),
  bottomLeft(createAnchor("bottomLeft", aiBottomLeft)),
  left(createAnchor("left", aiLeft))
{
  topLeft->setCoords(0, 1);
  bottomRight->setCoords(1, 0);
  
  setPen(Qt::NoPen);
  setSelectedPen(QPen(Qt::blue));
  setScaled(false, Qt::KeepAspectRatio);
}

QCPItemPixmap::~QCPItemPixmap()
{
}

/*!
  Sets the pixmap that will be displayed.
*/
void QCPItemPixmap::setPixmap(const QPixmap &pixmap)
{
  mPixmap = pixmap;
  if (mPixmap.isNull())
    qDebug() << Q_FUNC_INFO << "pixmap is null";
}

/*!
  Sets whether the pixmap will be scaled to fit the rectangle defined by the \a topLeft and \a
  bottomRight positions.
*/
void QCPItemPixmap::setScaled(bool scaled, Qt::AspectRatioMode aspectRatioMode)
{
  mScaled = scaled;
  mAspectRatioMode = aspectRatioMode;
  updateScaledPixmap();
}

/*!
  Sets the pen that will be used to draw a border around the pixmap.
  
  \see setSelectedPen, setBrush
*/
void QCPItemPixmap::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  Sets the pen that will be used to draw a border around the pixmap when selected
  
  \see setPen, setSelected
*/
void QCPItemPixmap::setSelectedPen(const QPen &pen)
{
  mSelectedPen = pen;
}

/* inherits documentation from base class */
double QCPItemPixmap::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  
  return rectSelectTest(getFinalRect(), pos, true);
}

/* inherits documentation from base class */
void QCPItemPixmap::draw(QCPPainter *painter)
{
  bool flipHorz = false;
  bool flipVert = false;
  QRect rect = getFinalRect(&flipHorz, &flipVert);
  double clipPad = mainPen().style() == Qt::NoPen ? 0 : mainPen().widthF();
  QRect boundingRect = rect.adjusted(-clipPad, -clipPad, clipPad, clipPad);
  if (boundingRect.intersects(clipRect()))
  {
    updateScaledPixmap(rect, flipHorz, flipVert);
    painter->drawPixmap(rect.topLeft(), mScaled ? mScaledPixmap : mPixmap);
    QPen pen = mainPen();
    if (pen.style() != Qt::NoPen)
    {
      painter->setPen(pen);
      painter->setBrush(Qt::NoBrush);
      painter->drawRect(rect);
    }
  }
}

/* inherits documentation from base class */
QPointF QCPItemPixmap::anchorPixelPoint(int anchorId) const
{
  bool flipHorz;
  bool flipVert;
  QRect rect = getFinalRect(&flipHorz, &flipVert);
  // we actually want denormal rects (negative width/height) here, so restore
  // the flipped state:
  if (flipHorz)
    rect.adjust(rect.width(), 0, -rect.width(), 0);
  if (flipVert)
    rect.adjust(0, rect.height(), 0, -rect.height());
  
  switch (anchorId)
  {
    case aiTop:         return (rect.topLeft()+rect.topRight())*0.5;
    case aiTopRight:    return rect.topRight();
    case aiRight:       return (rect.topRight()+rect.bottomRight())*0.5;
    case aiBottom:      return (rect.bottomLeft()+rect.bottomRight())*0.5;
    case aiBottomLeft:  return rect.bottomLeft();
    case aiLeft:        return (rect.topLeft()+rect.bottomLeft())*0.5;;
  }
  
  qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
  return QPointF();
}

/*! \internal
  
  Creates the buffered scaled image (\a mScaledPixmap) to fit the specified \a finalRect. The
  parameters \a flipHorz and \a flipVert control whether the resulting image shall be flipped
  horizontally or vertically. (This is used when \a topLeft is further to the bottom/right than \a
  bottomRight.)
  
  This function only creates the scaled pixmap when the buffered pixmap has a different size than
  the expected result, so calling this function repeatedly, e.g. in the \ref draw function, does
  not cause expensive rescaling every time.
  
  If scaling is disabled, sets mScaledPixmap to a null QPixmap.
*/
void QCPItemPixmap::updateScaledPixmap(QRect finalRect, bool flipHorz, bool flipVert)
{
  if (mPixmap.isNull())
    return;
  
  if (mScaled)
  {
    if (finalRect.isNull())
      finalRect = getFinalRect(&flipHorz, &flipVert);
    if (finalRect.size() != mScaledPixmap.size())
    {
      mScaledPixmap = mPixmap.scaled(finalRect.size(), mAspectRatioMode, Qt::SmoothTransformation);
      if (flipHorz || flipVert)
        mScaledPixmap = QPixmap::fromImage(mScaledPixmap.toImage().mirrored(flipHorz, flipVert));
    }
  } else if (!mScaledPixmap.isNull())
    mScaledPixmap = QPixmap();
}

/*! \internal
  
  Returns the final (tight) rect the pixmap is drawn in, depending on the current item positions
  and scaling settings.
  
  The output parameters \a flippedHorz and \a flippedVert return whether the pixmap should be drawn
  flipped horizontally or vertically in the returned rect. (The returned rect itself is always
  normalized, i.e. the top left corner of the rect is actually further to the top/left than the
  bottom right corner). This is the case when the item position \a topLeft is further to the
  bottom/right than \a bottomRight.
  
  If scaling is disabled, returns a rect with size of the original pixmap and the top left corner
  aligned with the item position \a topLeft. The position \a bottomRight is ignored.
*/
QRect QCPItemPixmap::getFinalRect(bool *flippedHorz, bool *flippedVert) const
{
  QRect result;
  bool flipHorz = false;
  bool flipVert = false;
  QPoint p1 = topLeft->pixelPoint().toPoint();
  QPoint p2 = bottomRight->pixelPoint().toPoint();
  if (p1 == p2)
    return QRect(p1, QSize(0, 0));
  if (mScaled)
  {
    QSize newSize = QSize(p2.x()-p1.x(), p2.y()-p1.y());
    QPoint topLeft = p1;
    if (newSize.width() < 0)
    {
      flipHorz = true;
      newSize.rwidth() *= -1;
      topLeft.setX(p2.x());
    }
    if (newSize.height() < 0)
    {
      flipVert = true;
      newSize.rheight() *= -1;
      topLeft.setY(p2.y());
    }
    QSize scaledSize = mPixmap.size();
    scaledSize.scale(newSize, mAspectRatioMode);
    result = QRect(topLeft, scaledSize);
  } else
  {
    result = QRect(p1, mPixmap.size());
  }
  if (flippedHorz)
    *flippedHorz = flipHorz;
  if (flippedVert)
    *flippedVert = flipVert;
  return result;
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the item is not selected
  and mSelectedPen when it is.
*/
QPen QCPItemPixmap::mainPen() const
{
  return mSelected ? mSelectedPen : mPen;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemTracer
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemTracer
  \brief Item that sticks to QCPGraph data points

  \image html QCPItemTracer.png "Tracer example. Blue dotted circles are anchors, solid blue discs are positions."

  The tracer can be connected with a QCPGraph via \ref setGraph. Then it will automatically adopt
  the coordinate axes of the graph and update its \a position to be on the graph's data. This means
  the key stays controllable via \ref setGraphKey, but the value will follow the graph data. If a
  QCPGraph is connected, note that setting the coordinates of the tracer item directly via \a
  position will have no effect because they will be overriden in the next redraw (this is when the
  coordinate update happens).
  
  If the specified key in \ref setGraphKey is outside the key bounds of the graph, the tracer will
  stay at the corresponding end of the graph.
  
  With \ref setInterpolating you may specify whether the tracer may only stay exactly on data
  points or whether it interpolates data points linearly, if given a key that lies between two data
  points of the graph.
  
  The tracer has different visual styles, see \ref setStyle. It is also possible to make the tracer
  have no own visual appearance (set the style to \ref tsNone), and just connect other item
  positions to the tracer \a position (used as an anchor) via \ref
  QCPItemPosition::setParentAnchor.
  
  \note The tracer position is only automatically updated upon redraws. So when the data of the
  graph changes and immediately afterwards (without a redraw) the a position coordinates of the
  tracer are retrieved, they will not reflect the updated data of the graph. In this case \ref
  updatePosition must be called manually, prior to reading the tracer coordinates.
*/

/*!
  Creates a tracer item and sets default values.
  
  The constructed item can be added to the plot with QCustomPlot::addItem.
*/
QCPItemTracer::QCPItemTracer(QCustomPlot *parentPlot) :
  QCPAbstractItem(parentPlot),
  position(createPosition("position")),
  mGraph(0)
{
  position->setCoords(0, 0);

  setBrush(Qt::NoBrush);
  setSelectedBrush(Qt::NoBrush);
  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue, 2));
  setStyle(tsCrosshair);
  setSize(6);
  setInterpolating(false);
  setGraphKey(0);
}

QCPItemTracer::~QCPItemTracer()
{
}

/*!
  Sets the pen that will be used to draw the line of the tracer
  
  \see setSelectedPen, setBrush
*/
void QCPItemTracer::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  Sets the pen that will be used to draw the line of the tracer when selected
  
  \see setPen, setSelected
*/
void QCPItemTracer::setSelectedPen(const QPen &pen)
{
  mSelectedPen = pen;
}

/*!
  Sets the brush that will be used to draw any fills of the tracer
  
  \see setSelectedBrush, setPen
*/
void QCPItemTracer::setBrush(const QBrush &brush)
{
  mBrush = brush;
}

/*!
  Sets the brush that will be used to draw any fills of the tracer, when selected.
  
  \see setBrush, setSelected
*/
void QCPItemTracer::setSelectedBrush(const QBrush &brush)
{
  mSelectedBrush = brush;
}

/*!
  Sets the size of the tracer in pixels, if the style supports setting a size (e.g. \ref tsSquare
  does, \ref tsCrosshair does not).
*/
void QCPItemTracer::setSize(double size)
{
  mSize = size;
}

/*!
  Sets the style/visual appearance of the tracer.
  
  If you only want to use the tracer \a position as an anchor for other items, set \a style to
  \ref tsNone.
*/
void QCPItemTracer::setStyle(QCPItemTracer::TracerStyle style)
{
  mStyle = style;
}

/*!
  Sets the QCPGraph this tracer sticks to. The tracer \a position will be set to type
  QCPItemPosition::ptPlotCoords and the axes will be set to the axes of \a graph.
  
  To free the tracer from any graph, set \a graph to 0. The tracer \a position can then be placed
  freely like any other item position. This is the state the tracer will assume when its graph gets
  deleted while still attached to it.
  
  \see setGraphKey
*/
void QCPItemTracer::setGraph(QCPGraph *graph)
{
  if (graph)
  {
    if (graph->parentPlot() == mParentPlot)
    {
      position->setType(QCPItemPosition::ptPlotCoords);
      position->setAxes(graph->keyAxis(), graph->valueAxis());
      mGraph = graph;
      updatePosition();
    } else
      qDebug() << Q_FUNC_INFO << "graph isn't in same QCustomPlot instance as this item";
  } else
  {
    mGraph = 0;
  }
}

/*!
  Sets the key of the graph's data point the tracer will be positioned at. This is the only free
  coordinate of a tracer when attached to a graph.
  
  Depending on \ref setInterpolating, the tracer will be either positioned on the data point
  closest to \a key, or will stay exactly at \a key and interpolate the value linearly.
  
  \see setGraph, setInterpolating
*/
void QCPItemTracer::setGraphKey(double key)
{
  mGraphKey = key;
}

/*!
  Sets whether the value of the graph's data points shall be interpolated, when positioning the
  tracer.
  
  If \a enabled is set to false and a key is given with \ref setGraphKey, the tracer is placed on
  the data point of the graph which is closest to the key, but which is not necessarily exactly
  there. If \a enabled is true, the tracer will be positioned exactly at the specified key, and
  the appropriate value will be interpolated from the graph's data points linearly.
  
  \see setGraph, setGraphKey
*/
void QCPItemTracer::setInterpolating(bool enabled)
{
  mInterpolating = enabled;
}

/* inherits documentation from base class */
double QCPItemTracer::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;

  QPointF center(position->pixelPoint());
  double w = mSize/2.0;
  QRect clip = clipRect();
  switch (mStyle)
  {
    case tsNone: return -1;
    case tsPlus:
    {
      if (clipRect().intersects(QRectF(center-QPointF(w, w), center+QPointF(w, w)).toRect()))
        return qSqrt(qMin(distSqrToLine(center+QPointF(-w, 0), center+QPointF(w, 0), pos),
                          distSqrToLine(center+QPointF(0, -w), center+QPointF(0, w), pos)));
      break;
    }
    case tsCrosshair:
    {
      return qSqrt(qMin(distSqrToLine(QPointF(clip.left(), center.y()), QPointF(clip.right(), center.y()), pos),
                        distSqrToLine(QPointF(center.x(), clip.top()), QPointF(center.x(), clip.bottom()), pos)));
    }
    case tsCircle:
    {
      if (clip.intersects(QRectF(center-QPointF(w, w), center+QPointF(w, w)).toRect()))
      {
        // distance to border:
        double centerDist = QVector2D(center-pos).length();
        double circleLine = w;
        double result = qAbs(centerDist-circleLine);
        // filled ellipse, allow click inside to count as hit:
        if (result > mParentPlot->selectionTolerance()*0.99 && mBrush.style() != Qt::NoBrush && mBrush.color().alpha() != 0)
        {
          if (centerDist <= circleLine)
            result = mParentPlot->selectionTolerance()*0.99;
        }
        return result;
      }
      break;
    }
    case tsSquare:
    {
      if (clip.intersects(QRectF(center-QPointF(w, w), center+QPointF(w, w)).toRect()))
      {
        QRectF rect = QRectF(center-QPointF(w, w), center+QPointF(w, w));
        bool filledRect = mBrush.style() != Qt::NoBrush && mBrush.color().alpha() != 0;
        return rectSelectTest(rect, pos, filledRect);
      }
      break;
    }
  }
  return -1;
}

/* inherits documentation from base class */
void QCPItemTracer::draw(QCPPainter *painter)
{
  updatePosition();
  if (mStyle == tsNone)
    return;

  painter->setPen(mainPen());
  painter->setBrush(mainBrush());
  QPointF center(position->pixelPoint());
  double w = mSize/2.0;
  QRect clip = clipRect();
  switch (mStyle)
  {
    case tsNone: return;
    case tsPlus:
    {
      if (clip.intersects(QRectF(center-QPointF(w, w), center+QPointF(w, w)).toRect()))
      {
        painter->drawLine(QLineF(center+QPointF(-w, 0), center+QPointF(w, 0)));
        painter->drawLine(QLineF(center+QPointF(0, -w), center+QPointF(0, w)));
      }
      break;
    }
    case tsCrosshair:
    {
      if (center.y() > clip.top() && center.y() < clip.bottom())
        painter->drawLine(QLineF(clip.left(), center.y(), clip.right(), center.y()));
      if (center.x() > clip.left() && center.x() < clip.right())
        painter->drawLine(QLineF(center.x(), clip.top(), center.x(), clip.bottom()));
      break;
    }
    case tsCircle:
    {
      if (clip.intersects(QRectF(center-QPointF(w, w), center+QPointF(w, w)).toRect()))
        painter->drawEllipse(center, w, w);
      break;
    }
    case tsSquare:
    {
      if (clip.intersects(QRectF(center-QPointF(w, w), center+QPointF(w, w)).toRect()))
        painter->drawRect(QRectF(center-QPointF(w, w), center+QPointF(w, w)));
      break;
    }
  }
}

/*!
  If the tracer is connected with a graph (\ref setGraph), this function updates the tracer's \a
  position to reside on the graph data, depending on the configured key (\ref setGraphKey).
  
  It is called automatically on every redraw and normally doesn't need to be called manually. One
  exception is when you want to read the tracer coordinates via \a position and are not sure that
  the graph's data (or the tracer key with \ref setGraphKey) hasn't changed since the last redraw.
  In that situation, call this function before accessing \a position, to make sure you don't get
  out-of-date coordinates.
  
  If there is no graph set on this tracer, this function does nothing.
*/
void QCPItemTracer::updatePosition()
{
  if (mGraph)
  {
    if (mParentPlot->hasPlottable(mGraph))
    {
      if (mGraph->data()->size() > 1)
      {
        QCPDataMap::const_iterator first = mGraph->data()->constBegin();
        QCPDataMap::const_iterator last = mGraph->data()->constEnd()-1;
        if (mGraphKey < first.key())
          position->setCoords(first.key(), first.value().value);
        else if (mGraphKey > last.key())
          position->setCoords(last.key(), last.value().value);
        else
        {
          QCPDataMap::const_iterator it = mGraph->data()->lowerBound(mGraphKey);
          if (it != first) // mGraphKey is somewhere between iterators
          {
            QCPDataMap::const_iterator prevIt = it-1;
            if (mInterpolating)
            {
              // interpolate between iterators around mGraphKey:
              double slope = 0;
              if (!qFuzzyCompare((double)it.key(), (double)prevIt.key()))
                slope = (it.value().value-prevIt.value().value)/(it.key()-prevIt.key());
              position->setCoords(mGraphKey, (mGraphKey-prevIt.key())*slope+prevIt.value().value);
            } else
            {
              // find iterator with key closest to mGraphKey:
              if (mGraphKey < (prevIt.key()+it.key())*0.5)
                it = prevIt;
              position->setCoords(it.key(), it.value().value);
            }
          } else // mGraphKey is exactly on first iterator
            position->setCoords(it.key(), it.value().value);
        }
      } else if (mGraph->data()->size() == 1)
      {
        QCPDataMap::const_iterator it = mGraph->data()->constBegin();
        position->setCoords(it.key(), it.value().value);
      } else
        qDebug() << Q_FUNC_INFO << "graph has no data";
    } else
      qDebug() << Q_FUNC_INFO << "graph not contained in QCustomPlot instance (anymore)";
  }
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the item is not selected
  and mSelectedPen when it is.
*/
QPen QCPItemTracer::mainPen() const
{
  return mSelected ? mSelectedPen : mPen;
}

/*! \internal

  Returns the brush that should be used for drawing fills of the item. Returns mBrush when the item
  is not selected and mSelectedBrush when it is.
*/
QBrush QCPItemTracer::mainBrush() const
{
  return mSelected ? mSelectedBrush : mBrush;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemBracket
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemBracket
  \brief A bracket for referencing/highlighting certain parts in the plot.

  \image html QCPItemBracket.png "Bracket example. Blue dotted circles are anchors, solid blue discs are positions."

  It has two positions, \a left and \a right, which define the span of the bracket. If \a left is
  actually farther to the left than \a right, the bracket is opened to the bottom, as shown in the
  example image.
  
  The bracket supports multiple styles via \ref setStyle. The length, i.e. how far the bracket
  stretches away from the embraced span, can be controlled with \ref setLength.
  
  \image html QCPItemBracket-length.png
  <center>Demonstrating the effect of different values for \ref setLength, for styles \ref
  bsCalligraphic and \ref bsSquare. Anchors and positions are displayed for reference.</center>
  
  It provides an anchor \a center, to allow connection of other items, e.g. an arrow (QCPItemLine
  or QCPItemCurve) or a text label (QCPItemText), to the bracket.
*/

/*!
  Creates a bracket item and sets default values.
  
  The constructed item can be added to the plot with QCustomPlot::addItem.
*/
QCPItemBracket::QCPItemBracket(QCustomPlot *parentPlot) :
  QCPAbstractItem(parentPlot),
  left(createPosition("left")),
  right(createPosition("right")),
  center(createAnchor("center", aiCenter))
{
  left->setCoords(0, 0);
  right->setCoords(1, 1);
  
  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue, 2));
  setLength(8);
  setStyle(bsCalligraphic);
}

QCPItemBracket::~QCPItemBracket()
{
}

/*!
  Sets the pen that will be used to draw the bracket.
  
  Note that when the style is \ref bsCalligraphic, only the color will be taken from the pen, the
  stroke and width are ignored. To change the apparent stroke width of a calligraphic bracket, use
  \ref setLength, which has a similar effect.
  
  \see setSelectedPen
*/
void QCPItemBracket::setPen(const QPen &pen)
{
  mPen = pen;
}

/*!
  Sets the pen that will be used to draw the bracket when selected
  
  \see setPen, setSelected
*/
void QCPItemBracket::setSelectedPen(const QPen &pen)
{
  mSelectedPen = pen;
}

/*!
  Sets the \a length in pixels how far the bracket extends in the direction towards the embraced
  span of the bracket (i.e. perpendicular to the <i>left</i>-<i>right</i>-direction)
  
  \image html QCPItemBracket-length.png
  <center>Demonstrating the effect of different values for \ref setLength, for styles \ref
  bsCalligraphic and \ref bsSquare. Anchors and positions are displayed for reference.</center>
*/
void QCPItemBracket::setLength(double length)
{
  mLength = length;
}

/*!
  Sets the style of the bracket, i.e. the shape/visual appearance.
  
  \see setPen
*/
void QCPItemBracket::setStyle(QCPItemBracket::BracketStyle style)
{
  mStyle = style;
}

/* inherits documentation from base class */
double QCPItemBracket::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  
  QVector2D leftVec(left->pixelPoint());
  QVector2D rightVec(right->pixelPoint());
  if (leftVec.toPoint() == rightVec.toPoint())
    return -1;
  
  QVector2D widthVec = (rightVec-leftVec)*0.5f;
  QVector2D lengthVec(-widthVec.y(), widthVec.x());
  lengthVec = lengthVec.normalized()*mLength;
  QVector2D centerVec = (rightVec+leftVec)*0.5f-lengthVec;
  
  return qSqrt(distSqrToLine((centerVec-widthVec).toPointF(), (centerVec+widthVec).toPointF(), pos));
}

/* inherits documentation from base class */
void QCPItemBracket::draw(QCPPainter *painter)
{
  QVector2D leftVec(left->pixelPoint());
  QVector2D rightVec(right->pixelPoint());
  if (leftVec.toPoint() == rightVec.toPoint())
    return;
  
  QVector2D widthVec = (rightVec-leftVec)*0.5f;
  QVector2D lengthVec(-widthVec.y(), widthVec.x());
  lengthVec = lengthVec.normalized()*mLength;
  QVector2D centerVec = (rightVec+leftVec)*0.5f-lengthVec;

  QPolygon boundingPoly;
  boundingPoly << leftVec.toPoint() << rightVec.toPoint()
               << (rightVec-lengthVec).toPoint() << (leftVec-lengthVec).toPoint();
  QRect clip = clipRect().adjusted(-mainPen().widthF(), -mainPen().widthF(), mainPen().widthF(), mainPen().widthF());
  if (clip.intersects(boundingPoly.boundingRect()))
  {
    painter->setPen(mainPen());
    switch (mStyle)
    {
      case bsSquare:
      {
        painter->drawLine((centerVec+widthVec).toPointF(), (centerVec-widthVec).toPointF());
        painter->drawLine((centerVec+widthVec).toPointF(), (centerVec+widthVec+lengthVec).toPointF());
        painter->drawLine((centerVec-widthVec).toPointF(), (centerVec-widthVec+lengthVec).toPointF());
        break;
      }
      case bsRound:
      {
        painter->setBrush(Qt::NoBrush);
        QPainterPath path;
        path.moveTo((centerVec+widthVec+lengthVec).toPointF());
        path.cubicTo((centerVec+widthVec).toPointF(), (centerVec+widthVec).toPointF(), centerVec.toPointF());
        path.cubicTo((centerVec-widthVec).toPointF(), (centerVec-widthVec).toPointF(), (centerVec-widthVec+lengthVec).toPointF());
        painter->drawPath(path);
        break;
      }
      case bsCurly:
      {
        painter->setBrush(Qt::NoBrush);
        QPainterPath path;
        path.moveTo((centerVec+widthVec+lengthVec).toPointF());
        path.cubicTo((centerVec+widthVec-lengthVec*0.8f).toPointF(), (centerVec+0.4f*widthVec+lengthVec).toPointF(), centerVec.toPointF());
        path.cubicTo((centerVec-0.4f*widthVec+lengthVec).toPointF(), (centerVec-widthVec-lengthVec*0.8f).toPointF(), (centerVec-widthVec+lengthVec).toPointF());
        painter->drawPath(path);
        break;
      }
      case bsCalligraphic:
      {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(mainPen().color()));
        QPainterPath path;
        path.moveTo((centerVec+widthVec+lengthVec).toPointF());
        
        path.cubicTo((centerVec+widthVec-lengthVec*0.8f).toPointF(), (centerVec+0.4f*widthVec+0.8f*lengthVec).toPointF(), centerVec.toPointF());
        path.cubicTo((centerVec-0.4f*widthVec+0.8f*lengthVec).toPointF(), (centerVec-widthVec-lengthVec*0.8f).toPointF(), (centerVec-widthVec+lengthVec).toPointF());
        
        path.cubicTo((centerVec-widthVec-lengthVec*0.5f).toPointF(), (centerVec-0.2f*widthVec+1.2f*lengthVec).toPointF(), (centerVec+lengthVec*0.2f).toPointF());
        path.cubicTo((centerVec+0.2f*widthVec+1.2f*lengthVec).toPointF(), (centerVec+widthVec-lengthVec*0.5f).toPointF(), (centerVec+widthVec+lengthVec).toPointF());
        
        painter->drawPath(path);
        break;
      }
    }
  }
}

/* inherits documentation from base class */
QPointF QCPItemBracket::anchorPixelPoint(int anchorId) const
{
  QVector2D leftVec(left->pixelPoint());
  QVector2D rightVec(right->pixelPoint());
  if (leftVec.toPoint() == rightVec.toPoint())
    return leftVec.toPointF();
  
  QVector2D widthVec = (rightVec-leftVec)*0.5f;
  QVector2D lengthVec(-widthVec.y(), widthVec.x());
  lengthVec = lengthVec.normalized()*mLength;
  QVector2D centerVec = (rightVec+leftVec)*0.5f-lengthVec;
  
  switch (anchorId)
  {
    case aiCenter:
      return centerVec.toPointF();
  }
  qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
  return QPointF();
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemBracket::mainPen() const
{
    return mSelected ? mSelectedPen : mPen;
}

