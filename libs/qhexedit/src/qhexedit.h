#ifndef QHEXEDIT_H
#define QHEXEDIT_H

#include <QtGui>
#include "qhexedit_p.h"

/** \mainpage
QHexEdit is a binary editor widget for Qt.

\version Version 0.6.3
\image html hexedit.png
*/


/*! QHexEdit is a hex editor widget written in C++ for the Qt (Qt4) framework.
It is a simple editor for binary data, just like QPlainTextEdit is for text
data. There are sip configuration files included, so it is easy to create
bindings for PyQt and you can use this widget also in python.

QHexEdit takes the data of a QByteArray (setData()) and shows it. You can use
the mouse or the keyboard to navigate inside the widget. If you hit the keys
(0..9, a..f) you will change the data. Changed data is highlighted and can be
accessed via data().

Normaly QHexEdit works in the overwrite Mode. You can set overwriteMode(false)
and insert data. In this case the size of data() increases. It is also possible
to delete bytes (del or backspace), here the size of data decreases.

You can select data with keyboard hits or mouse movements. The copy-key will
copy the selected data into the clipboard. The cut-key copies also but delets
it afterwards. In overwrite mode, the paste function overwrites the content of
the (does not change the length) data. In insert mode, clipboard data will be
inserted. The clipboard content is expected in ASCII Hex notation. Unknown
characters will be ignored.

QHexEdit comes with undo/redo functionality. All changes can be undone, by
pressing the undo-key (usually ctr-z). They can also be redone afterwards.
The undo/redo framework is cleared, when setData() sets up a new
content for the editor. You can search data inside the content with indexOf()
and lastIndexOf(). The replace() function is to change located subdata. This
'replaced' data can also be undone by the undo/redo framework.

This widget can only handle small amounts of data. The size has to be below 10
megabytes, otherwise the scroll sliders ard not shown and you can't scroll any
more.
*/
        class QHexEdit : public QScrollArea
{
    Q_OBJECT
    /*! Property data holds the content of QHexEdit. Call setData() to set the
    content of QHexEdit, data() returns the actual content.
    */
    Q_PROPERTY(QByteArray data READ data WRITE setData)

    /*! Property addressOffset is added to the Numbers of the Address Area.
    A offset in the address area (left side) is sometimes usefull, whe you show
    only a segment of a complete memory picture. With setAddressOffset() you set
    this property - with addressOffset() you get the actual value.
    */
    Q_PROPERTY(int addressOffset READ addressOffset WRITE setAddressOffset)

    /*! Property address area color sets (setAddressAreaColor()) the backgorund
    color of address areas. You can also read the color (addressaAreaColor()).
    */
    Q_PROPERTY(QColor addressAreaColor READ addressAreaColor WRITE setAddressAreaColor)

    /*! Porperty cursorPosition sets or gets the position of the editor cursor
    in QHexEdit.
    */
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition)

    /*! Property highlighting color sets (setHighlightingColor()) the backgorund
    color of highlighted text areas. You can also read the color
    (highlightingColor()).
    */
    Q_PROPERTY(QColor highlightingColor READ highlightingColor WRITE setHighlightingColor)

    /*! Property selection color sets (setSelectionColor()) the backgorund
    color of selected text areas. You can also read the color
    (selectionColor()).
    */
    Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor)

    /*! Porperty overwrite mode sets (setOverwriteMode()) or gets (overwriteMode()) the mode
    in which the editor works. In overwrite mode the user will overwrite existing data. The
    size of data will be constant. In insert mode the size will grow, when inserting
    new data.
    */
    Q_PROPERTY(bool overwriteMode READ overwriteMode WRITE setOverwriteMode)

    /*! Porperty readOnly sets (setReadOnly()) or gets (isReadOnly) the mode
    in which the editor works. In readonly mode the the user can only navigate
    through the data and select data; modifying is not possible. This
    property's default is false.
    */
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)

    /*! Set the font of the widget. Please use fixed width fonts like Mono or Courier.*/
    Q_PROPERTY(QFont font READ font WRITE setFont)


public:
    /*! Creates an instance of QHexEdit.
    \param parent Parent widget of QHexEdit.
    */
    QHexEdit(QWidget *parent = 0);

    /*! Returns the index position of the first occurrence
    of the byte array ba in this byte array, searching forward from index position
    from. Returns -1 if ba could not be found. In addition to this functionality
    of QByteArray the cursorposition is set to the end of found bytearray and
    it will be selected.

    */
    int indexOf(const QByteArray & ba, int from = 0) const;

    /*! Inserts a byte array.
    \param i Index position, where to insert
    \param ba byte array, which is to insert
    In overwrite mode, the existing data will be overwritten, in insertmode ba will be
    inserted and size of data grows.
    */
    void insert(int i, const QByteArray & ba);

    /*! Inserts a char.
    \param i Index position, where to insert
    \param ch Char, which is to insert
    In overwrite mode, the existing data will be overwritten, in insertmode ba will be
    inserted and size of data grows.
    */
    void insert(int i, char ch);

    /*! Returns the index position of the last occurrence
    of the byte array ba in this byte array, searching backwards from index position
    from. Returns -1 if ba could not be found. In addition to this functionality
    of QByteArray the cursorposition is set to the beginning of found bytearray and
    it will be selected.

    */
    int lastIndexOf(const QByteArray & ba, int from = 0) const;

    /*! Removes len bytes from the content.
    \param pos Index position, where to remove
    \param len Amount of bytes to remove
    In overwrite mode, the existing bytes will be overwriten with 0x00.
    */
    void remove(int pos, int len=1);

    /*! Replaces len bytes from index position pos with the byte array after.
    */
    void replace( int pos, int len, const QByteArray & after);

    /*! Gives back a formatted image of the content of QHexEdit
    */
    QString toReadableString();

    /*! Gives back a formatted image of the selected content of QHexEdit
    */
    QString selectionToReadableString();

    /*! \cond docNever */
    void setAddressOffset(int offset);
    int addressOffset();
    void setCursorPosition(int cusorPos);
    int cursorPosition();
    void setData(QByteArray const &data);
    QByteArray data();
    void setAddressAreaColor(QColor const &color);
    QColor addressAreaColor();
    void setHighlightingColor(QColor const &color);
    QColor highlightingColor();
    void setSelectionColor(QColor const &color);
    QColor selectionColor();
    void setOverwriteMode(bool);
    bool overwriteMode();
    void setReadOnly(bool);
    bool isReadOnly();
    const QFont &font() const;
    void setFont(const QFont &);
    /*! \endcond docNever */

public slots:
    /*! Redoes the last operation. If there is no operation to redo, i.e.
      there is no redo step in the undo/redo history, nothing happens.
      */
    void redo();

    /*! Set the minimum width of the address area.
      \param addressWidth Width in characters.
      */
    void setAddressWidth(int addressWidth);

    /*! Switch the address area on or off.
      \param addressArea true (show it), false (hide it).
      */
    void setAddressArea(bool addressArea);

    /*! Switch the ascii area on or off.
      \param asciiArea true (show it), false (hide it).
      */
    void setAsciiArea(bool asciiArea);

    /*! Switch the highlighting feature on or of.
      \param mode true (show it), false (hide it).
      */
    void setHighlighting(bool mode);

    /*! Undoes the last operation. If there is no operation to undo, i.e.
      there is no undo step in the undo/redo history, nothing happens.
      */
    void undo();

signals:

    /*! Contains the address, where the cursor is located. */
    void currentAddressChanged(int address);

    /*! Contains the size of the data to edit. */
    void currentSizeChanged(int size);

    /*! The signal is emited every time, the data is changed. */
    void dataChanged();

    /*! The signal is emited every time, the overwrite mode is changed. */
    void overwriteModeChanged(bool state);

private:
    /*! \cond docNever */
    QHexEditPrivate *qHexEdit_p;
    QHBoxLayout *layout;
    QScrollArea *scrollArea;
    /*! \endcond docNever */
};

#endif

