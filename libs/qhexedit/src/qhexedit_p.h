#ifndef QHEXEDIT_P_H
#define QHEXEDIT_P_H

/** \cond docNever */


#include <QtGui>
#include "xbytearray.h"

class QHexEditPrivate : public QWidget
{
Q_OBJECT

public:
    QHexEditPrivate(QScrollArea *parent);

    void setAddressAreaColor(QColor const &color);
    QColor addressAreaColor();

    void setAddressOffset(int offset);
    int addressOffset();

    void setCursorPos(int position);
    int cursorPos();

    void setData(QByteArray const &data);
    QByteArray data();

    void setHighlightingColor(QColor const &color);
    QColor highlightingColor();

    void setOverwriteMode(bool overwriteMode);
    bool overwriteMode();

    void setReadOnly(bool readOnly);
    bool isReadOnly();

    void setSelectionColor(QColor const &color);
    QColor selectionColor();

    XByteArray & xData();

    int indexOf(const QByteArray & ba, int from = 0);
    void insert(int index, const QByteArray & ba);
    void insert(int index, char ch);
    int lastIndexOf(const QByteArray & ba, int from = 0);
    void remove(int index, int len=1);
    void replace(int index, char ch);
    void replace(int index, const QByteArray & ba);
    void replace(int pos, int len, const QByteArray & after);

    void setAddressArea(bool addressArea);
    void setAddressWidth(int addressWidth);
    void setAsciiArea(bool asciiArea);
    void setHighlighting(bool mode);
    virtual void setFont(const QFont &font);

    void undo();
    void redo();

    QString toRedableString();
    QString selectionToReadableString();

signals:
    void currentAddressChanged(int address);
    void currentSizeChanged(int size);
    void dataChanged();
    void overwriteModeChanged(bool state);

protected:
    void keyPressEvent(QKeyEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);

    void paintEvent(QPaintEvent *event);

    int cursorPos(QPoint pos);          // calc cursorpos from graphics position. DOES NOT STORE POSITION

    void resetSelection(int pos);       // set selectionStart and selectionEnd to pos
    void resetSelection();              // set selectionEnd to selectionStart
    void setSelection(int pos);         // set min (if below init) or max (if greater init)
    int getSelectionBegin();
    int getSelectionEnd();


private slots:
    void updateCursor();

private:
    void adjust();
    void ensureVisible();

    QColor _addressAreaColor;
    QColor _highlightingColor;
    QColor _selectionColor;
    QScrollArea *_scrollArea;
    QTimer _cursorTimer;
    QUndoStack *_undoStack;

    XByteArray _xData;                      // Hält den Inhalt des Hex Editors

    bool _blink;                            // true: then cursor blinks
    bool _renderingRequired;                // Flag to store that rendering is necessary
    bool _addressArea;                      // left area of QHexEdit
    bool _asciiArea;                        // medium area
    bool _highlighting;                     // highlighting of changed bytes
    bool _overwriteMode;
    bool _readOnly;                         // true: the user can only look and navigate

    int _charWidth, _charHeight;            // char dimensions (dpendend on font)
    int _cursorX, _cursorY;                 // graphics position of the cursor
    int _cursorPosition;                    // character positioin in stream (on byte ends in to steps)
    int _xPosAdr, _xPosHex, _xPosAscii;     // graphics x-position of the areas

    int _selectionBegin;                    // First selected char
    int _selectionEnd;                      // Last selected char
    int _selectionInit;                     // That's, where we pressed the mouse button

    int _size;
};

/** \endcond docNever */

#endif

