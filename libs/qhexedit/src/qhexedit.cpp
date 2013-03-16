#include <QtGui>

#include "qhexedit.h"


QHexEdit::QHexEdit(QWidget *parent) : QScrollArea(parent)
{
    qHexEdit_p = new QHexEditPrivate(this);
    setWidget(qHexEdit_p);
    setWidgetResizable(true);

    connect(qHexEdit_p, SIGNAL(currentAddressChanged(int)), this, SIGNAL(currentAddressChanged(int)));
    connect(qHexEdit_p, SIGNAL(currentSizeChanged(int)), this, SIGNAL(currentSizeChanged(int)));
    connect(qHexEdit_p, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    connect(qHexEdit_p, SIGNAL(overwriteModeChanged(bool)), this, SIGNAL(overwriteModeChanged(bool)));
    setFocusPolicy(Qt::NoFocus);
}

int QHexEdit::indexOf(const QByteArray & ba, int from) const
{
    return qHexEdit_p->indexOf(ba, from);
}

void QHexEdit::insert(int i, const QByteArray & ba)
{
    qHexEdit_p->insert(i, ba);
}

void QHexEdit::insert(int i, char ch)
{
    qHexEdit_p->insert(i, ch);
}

int QHexEdit::lastIndexOf(const QByteArray & ba, int from) const
{
    return qHexEdit_p->lastIndexOf(ba, from);
}

void QHexEdit::remove(int pos, int len)
{
    qHexEdit_p->remove(pos, len);
}

void QHexEdit::replace( int pos, int len, const QByteArray & after)
{
    qHexEdit_p->replace(pos, len, after);
}

QString QHexEdit::toReadableString()
{
    return qHexEdit_p->toRedableString();
}

QString QHexEdit::selectionToReadableString()
{
    return qHexEdit_p->selectionToReadableString();
}

void QHexEdit::setAddressArea(bool addressArea)
{
    qHexEdit_p->setAddressArea(addressArea);
}

void QHexEdit::redo()
{
    qHexEdit_p->redo();
}

void QHexEdit::undo()
{
    qHexEdit_p->undo();
}

void QHexEdit::setAddressWidth(int addressWidth)
{
    qHexEdit_p->setAddressWidth(addressWidth);
}

void QHexEdit::setAsciiArea(bool asciiArea)
{
    qHexEdit_p->setAsciiArea(asciiArea);
}

void QHexEdit::setHighlighting(bool mode)
{
    qHexEdit_p->setHighlighting(mode);
}

void QHexEdit::setAddressOffset(int offset)
{
    qHexEdit_p->setAddressOffset(offset);
}

int QHexEdit::addressOffset()
{
    return qHexEdit_p->addressOffset();
}

void QHexEdit::setCursorPosition(int cursorPos)
{
    // cursorPos in QHexEditPrivate is the position of the textcoursor without
    // blanks, means bytePos*2
    qHexEdit_p->setCursorPos(cursorPos*2);
}

int QHexEdit::cursorPosition()
{
    return qHexEdit_p->cursorPos() / 2;
}


void QHexEdit::setData(const QByteArray &data)
{
    qHexEdit_p->setData(data);
}

QByteArray QHexEdit::data()
{
    return qHexEdit_p->data();
}

void QHexEdit::setAddressAreaColor(const QColor &color)
{
    qHexEdit_p->setAddressAreaColor(color);
}

QColor QHexEdit::addressAreaColor()
{
    return qHexEdit_p->addressAreaColor();
}

void QHexEdit::setHighlightingColor(const QColor &color)
{
    qHexEdit_p->setHighlightingColor(color);
}

QColor QHexEdit::highlightingColor()
{
    return qHexEdit_p->highlightingColor();
}

void QHexEdit::setSelectionColor(const QColor &color)
{
    qHexEdit_p->setSelectionColor(color);
}

QColor QHexEdit::selectionColor()
{
    return qHexEdit_p->selectionColor();
}

void QHexEdit::setOverwriteMode(bool overwriteMode)
{
    qHexEdit_p->setOverwriteMode(overwriteMode);
}

bool QHexEdit::overwriteMode()
{
    return qHexEdit_p->overwriteMode();
}

void QHexEdit::setReadOnly(bool readOnly)
{
    qHexEdit_p->setReadOnly(readOnly);
}

bool QHexEdit::isReadOnly()
{
    return qHexEdit_p->isReadOnly();
}

void QHexEdit::setFont(const QFont &font)
{
    qHexEdit_p->setFont(font);
}

const QFont & QHexEdit::font() const
{
    return qHexEdit_p->font();
}
