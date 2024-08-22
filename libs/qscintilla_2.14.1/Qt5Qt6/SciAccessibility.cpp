// The implementation of the class that implements accessibility support.
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


#include <qglobal.h>

#if !defined(QT_NO_ACCESSIBILITY)

#include "SciAccessibility.h"

#include <QAccessible>
#include <QFont>
#include <QFontMetrics>
#include <QRect>
#include <QWidget>

#include "Qsci/qsciscintillabase.h"


// Set if the accessibility support needs initialising.
bool QsciAccessibleScintillaBase::needs_initialising = true;

// The list of all accessibles.
QList<QsciAccessibleScintillaBase *> QsciAccessibleScintillaBase::all_accessibles;


// Forward declarations.
static QAccessibleInterface *factory(const QString &classname, QObject *object);


// The ctor.
QsciAccessibleScintillaBase::QsciAccessibleScintillaBase(QWidget *widget) :
        QAccessibleWidget(widget, QAccessible::EditableText),
        current_cursor_offset(-1), is_selection(false)
{
    all_accessibles.append(this);
}


// The dtor.
QsciAccessibleScintillaBase::~QsciAccessibleScintillaBase()
{
    all_accessibles.removeOne(this);
}


// Initialise the accessibility support.
void QsciAccessibleScintillaBase::initialise()
{
    if (needs_initialising)
    {
        QAccessible::installFactory(factory);
        needs_initialising = false;
    }
}


// Find the accessible for a widget.
QsciAccessibleScintillaBase *QsciAccessibleScintillaBase::findAccessible(
        QsciScintillaBase *sb)
{
    for (int i = 0; i < all_accessibles.size(); ++i)
    {
        QsciAccessibleScintillaBase *acc_sb = all_accessibles.at(i);

        if (acc_sb->sciWidget() == sb)
            return acc_sb;
    }

    return 0;
}


// Return the QsciScintillaBase instance.
QsciScintillaBase *QsciAccessibleScintillaBase::sciWidget() const
{
    return static_cast<QsciScintillaBase *>(widget());
}


// Update the accessible when the selection has changed.
void QsciAccessibleScintillaBase::selectionChanged(QsciScintillaBase *sb,
        bool selection)
{
    QsciAccessibleScintillaBase *acc_sb = findAccessible(sb);

    if (!acc_sb)
        return;

    acc_sb->is_selection = selection;
}


// Update the accessibility when text has been inserted.
void QsciAccessibleScintillaBase::textInserted(QsciScintillaBase *sb,
        int position, const char *text, int length)
{
    Q_ASSERT(text);

    QString new_text = sb->bytesAsText(text, length);
    int offset = positionAsOffset(sb, position);

    QAccessibleTextInsertEvent ev(sb, offset, new_text);
    QAccessible::updateAccessibility(&ev);
}


// Return the fragment of text before an offset.
QString QsciAccessibleScintillaBase::textBeforeOffset(int offset,
        QAccessible::TextBoundaryType boundaryType, int *startOffset,
        int *endOffset) const
{
    QsciScintillaBase *sb = sciWidget();

    // Initialise in case of errors.
    *startOffset = *endOffset = -1;

    int position = validPosition(offset);

    if (position < 0)
        return QString();

    int start_position, end_position;

    if (!boundaries(sb, position, boundaryType, &start_position, &end_position))
        return QString();

    if (start_position == 0)
        return QString();

    if (!boundaries(sb, start_position - 1, boundaryType, &start_position, &end_position))
        return QString();

    positionRangeAsOffsetRange(sb, start_position, end_position, startOffset,
            endOffset);

    return textRange(sb, start_position, end_position);
}


// Return the fragment of text after an offset.
QString QsciAccessibleScintillaBase::textAfterOffset(int offset,
        QAccessible::TextBoundaryType boundaryType, int *startOffset,
        int *endOffset) const
{
    QsciScintillaBase *sb = sciWidget();

    // Initialise in case of errors.
    *startOffset = *endOffset = -1;

    int position = validPosition(offset);

    if (position < 0)
        return QString();

    int start_position, end_position;

    if (!boundaries(sb, position, boundaryType, &start_position, &end_position))
        return QString();

    if (end_position >= sb->SendScintilla(QsciScintillaBase::SCI_GETTEXTLENGTH))
        return QString();

    if (!boundaries(sb, end_position, boundaryType, &start_position, &end_position))
        return QString();

    positionRangeAsOffsetRange(sb, start_position, end_position, startOffset,
            endOffset);

    return textRange(sb, start_position, end_position);
}


// Return the fragment of text at an offset.
QString QsciAccessibleScintillaBase::textAtOffset(int offset,
        QAccessible::TextBoundaryType boundaryType, int *startOffset,
        int *endOffset) const
{
    QsciScintillaBase *sb = sciWidget();

    // Initialise in case of errors.
    *startOffset = *endOffset = -1;

    int position = validPosition(offset);

    if (position < 0)
        return QString();

    int start_position, end_position;

    if (!boundaries(sb, position, boundaryType, &start_position, &end_position))
        return QString();

    positionRangeAsOffsetRange(sb, start_position, end_position, startOffset,
            endOffset);

    return textRange(sb, start_position, end_position);
}


// Update the accessibility when text has been deleted.
void QsciAccessibleScintillaBase::textDeleted(QsciScintillaBase *sb,
        int position, const char *text, int length)
{
    Q_ASSERT(text);

    QString old_text = sb->bytesAsText(text, length);
    int offset = positionAsOffset(sb, position);

    QAccessibleTextRemoveEvent ev(sb, offset, old_text);
    QAccessible::updateAccessibility(&ev);
}


// Update the accessibility when the UI has been updated.
void QsciAccessibleScintillaBase::updated(QsciScintillaBase *sb)
{
    QsciAccessibleScintillaBase *acc_sb = findAccessible(sb);

    if (!acc_sb)
        return;

    int cursor_offset = positionAsOffset(sb,
            sb->SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS));

    if (acc_sb->current_cursor_offset != cursor_offset)
    {
        acc_sb->current_cursor_offset = cursor_offset;

        QAccessibleTextCursorEvent ev(sb, cursor_offset);
        QAccessible::updateAccessibility(&ev);
    }
}


// Return a valid position from an offset or -1 if it was invalid.
int QsciAccessibleScintillaBase::validPosition(int offset) const
{
    // An offset of -1 is interpreted as the length of the text.
    int nr_chars = characterCount();

    if (offset == -1)
        offset = nr_chars;

    // Check there is some text and the offset is within range.
    if (nr_chars == 0 || offset < 0 || offset > nr_chars)
        return -1;

    return offsetAsPosition(sciWidget(), offset);
}


// Get the start and end boundary positions for a type of boundary.  true is
// returned if the boundary positions are valid.
bool QsciAccessibleScintillaBase::boundaries(QsciScintillaBase *sb,
        int position, QAccessible::TextBoundaryType boundaryType,
        int *start_position, int *end_position)
{
    // This implementation is based on what Qt does although that may itself be
    // wrong.  The cursor is in a word if it is before or after any character
    // in the word.  If the cursor is not in a word (eg. is has a space each
    // side) then the previous word is current.

    switch (boundaryType)
    {
    case QAccessible::CharBoundary:
        *start_position = position;
        *end_position = sb->SendScintilla(
                QsciScintillaBase::SCI_POSITIONAFTER, position);
        break;

    case QAccessible::WordBoundary:
        *start_position = sb->SendScintilla(
                QsciScintillaBase::SCI_WORDSTARTPOSITION, position, 1);
        *end_position = sb->SendScintilla(
                QsciScintillaBase::SCI_WORDENDPOSITION, position, 1);

        // If the start and end positions are the same then we are not in a
        // word.
        if (*start_position == *end_position)
        {
            // We need the immediately preceding word.  Note that Qt behaves
            // differently as it will not move before the current line.

            // Find the end of the preceding word.
            *end_position = sb->SendScintilla(
                    QsciScintillaBase::SCI_WORDSTARTPOSITION, position, 0L);

            // If the end is 0 then there isn't a preceding word.
            if (*end_position == 0)
                return false;

            // Now find the start.
            *start_position = sb->SendScintilla(
                    QsciScintillaBase::SCI_WORDSTARTPOSITION, *end_position,
                    1);
        }

        break;

    case QAccessible::SentenceBoundary:
        return false;

    case QAccessible::ParagraphBoundary:
        // Paragraph boundaries are supposed to be supported but it isn't clear
        // what this means in a code editor.
        return false;

    case QAccessible::LineBoundary:
        {
            int line = sb->SendScintilla(
                    QsciScintillaBase::SCI_LINEFROMPOSITION, position);

            *start_position = sb->SendScintilla(
                    QsciScintillaBase::SCI_POSITIONFROMLINE, line);
            *end_position = sb->SendScintilla(
                    QsciScintillaBase::SCI_POSITIONFROMLINE, line + 1);

            // See if we are after the last end-of-line character.
            if (*start_position == *end_position)
                return false;
        }

        break;

    case QAccessible::NoBoundary:
        *start_position = 0;
        *end_position = sb->SendScintilla(
                QsciScintillaBase::SCI_GETTEXTLENGTH);
        break;
    }

    return true;
}


// Return the text between two positions.
QString QsciAccessibleScintillaBase::textRange(QsciScintillaBase *sb,
        int start_position, int end_position)
{
    QByteArray bytes(end_position - start_position + 1, '\0');

    sb->SendScintilla(QsciScintillaBase::SCI_GETTEXTRANGE, start_position,
            end_position, bytes.data());

    return sb->bytesAsText(bytes.constData(), bytes.size() - 1);
}


// Convert a byte position to a character offset.
int QsciAccessibleScintillaBase::positionAsOffset(QsciScintillaBase *sb,
        int position)
{
    return sb->SendScintilla(QsciScintillaBase::SCI_COUNTCHARACTERS, 0,
            position);
}


// Convert a range of byte poisitions to character offsets.
void QsciAccessibleScintillaBase::positionRangeAsOffsetRange(
        QsciScintillaBase *sb, int start_position, int end_position,
        int *startOffset, int *endOffset)
{
    *startOffset = positionAsOffset(sb, start_position);
    *endOffset = positionAsOffset(sb, end_position);
}


// Convert character offset position to a byte position.
int QsciAccessibleScintillaBase::offsetAsPosition(QsciScintillaBase *sb,
        int offset)
{
    return sb->SendScintilla(QsciScintillaBase::SCI_POSITIONRELATIVE, 0,
            offset);
}


// Get the current selection if any.
void QsciAccessibleScintillaBase::selection(int selectionIndex,
        int *startOffset, int *endOffset) const
{
    int start, end;

    if (selectionIndex == 0 && is_selection)
    {
        QsciScintillaBase *sb = sciWidget();
        int start_position = sb->SendScintilla(
                QsciScintillaBase::SCI_GETSELECTIONSTART);
        int end_position = sb->SendScintilla(
                QsciScintillaBase::SCI_GETSELECTIONEND);

        start = positionAsOffset(sb, start_position);
        end = positionAsOffset(sb, end_position);
    }
    else
    {
        start = end = 0;
    }

    *startOffset = start;
    *endOffset = end;
}


// Return the number of selections.
int QsciAccessibleScintillaBase::selectionCount() const
{
    return (is_selection ? 1 : 0);
}


// Add a selection.
void QsciAccessibleScintillaBase::addSelection(int startOffset, int endOffset)
{
    setSelection(0, startOffset, endOffset);
}


// Remove a selection.
void QsciAccessibleScintillaBase::removeSelection(int selectionIndex)
{
    if (selectionIndex == 0)
        sciWidget()->SendScintilla(QsciScintillaBase::SCI_CLEARSELECTIONS);
}


// Set the selection.
void QsciAccessibleScintillaBase::setSelection(int selectionIndex,
        int startOffset, int endOffset)
{
    if (selectionIndex == 0)
    {
        QsciScintillaBase *sb = sciWidget();
        sb->SendScintilla(QsciScintillaBase::SCI_SETSELECTIONSTART,
                offsetAsPosition(sb, startOffset));
        sb->SendScintilla(QsciScintillaBase::SCI_SETSELECTIONEND,
                offsetAsPosition(sb, endOffset));
    }
}


// Return the current cursor offset.
int QsciAccessibleScintillaBase::cursorPosition() const
{
    return current_cursor_offset;
}


// Set the cursor offset.
void QsciAccessibleScintillaBase::setCursorPosition(int position)
{
    QsciScintillaBase *sb = sciWidget();

    sb->SendScintilla(QsciScintillaBase::SCI_GOTOPOS,
            offsetAsPosition(sb, position));
}


// Return the text between two offsets.
QString QsciAccessibleScintillaBase::text(int startOffset, int endOffset) const
{
    QsciScintillaBase *sb = sciWidget();

    return textRange(sb, offsetAsPosition(sb, startOffset),
            offsetAsPosition(sb, endOffset));
}


// Return the number of characters in the text.
int QsciAccessibleScintillaBase::characterCount() const
{
    QsciScintillaBase *sb = sciWidget();

    return sb->SendScintilla(QsciScintillaBase::SCI_COUNTCHARACTERS, 0,
            sb->SendScintilla(QsciScintillaBase::SCI_GETTEXTLENGTH));
}


QRect QsciAccessibleScintillaBase::characterRect(int offset) const
{
    QsciScintillaBase *sb = sciWidget();
    int position = offsetAsPosition(sb, offset);
    int x_vport = sb->SendScintilla(QsciScintillaBase::SCI_POINTXFROMPOSITION,
            position);
    int y_vport = sb->SendScintilla(QsciScintillaBase::SCI_POINTYFROMPOSITION,
            position);
    const QString ch = text(offset, offset + 1);

    // Get the character's font metrics.
    int style = sb->SendScintilla(QsciScintillaBase::SCI_GETSTYLEAT, position);
    QFontMetrics metrics(fontForStyle(style));

    QRect rect(x_vport, y_vport, metrics.horizontalAdvance(ch),
            metrics.height());
    rect.moveTo(sb->viewport()->mapToGlobal(rect.topLeft()));

    return rect;
}


// Return the offset of the character at the given screen coordinates.
int QsciAccessibleScintillaBase::offsetAtPoint(const QPoint &point) const
{
    QsciScintillaBase *sb = sciWidget();
    QPoint p = sb->viewport()->mapFromGlobal(point);
    int position = sb->SendScintilla(QsciScintillaBase::SCI_POSITIONFROMPOINT,
            p.x(), p.y());

    return (position >= 0) ? positionAsOffset(sb, position) : -1;
}


// Scroll to make sure an area of text is visible.
void QsciAccessibleScintillaBase::scrollToSubstring(int startIndex,
        int endIndex)
{
    QsciScintillaBase *sb = sciWidget();
    int start = offsetAsPosition(sb, startIndex);
    int end = offsetAsPosition(sb, endIndex);

    sb->SendScintilla(QsciScintillaBase::SCI_SCROLLRANGE, end, start);
}


// Return the attributes of a character and surrounding text.
QString QsciAccessibleScintillaBase::attributes(int offset, int *startOffset,
        int *endOffset) const
{
    QsciScintillaBase *sb = sciWidget();
    int position = offsetAsPosition(sb, offset);
    int style = sb->SendScintilla(QsciScintillaBase::SCI_GETSTYLEAT, position);

    // Find the start of the text with this style.
    int start_position = position;
    int start_text_position = offset;

    while (start_position > 0)
    {
        int before = sb->SendScintilla(QsciScintillaBase::SCI_POSITIONBEFORE,
                start_position);
        int s = sb->SendScintilla(QsciScintillaBase::SCI_GETSTYLEAT, before);

        if (s != style)
            break;

        start_position = before;
        --start_text_position;
    }

    *startOffset = start_text_position;

    // Find the end of the text with this style.
    int end_position = sb->SendScintilla(QsciScintillaBase::SCI_POSITIONAFTER,
            position);
    int end_text_position = offset + 1;
    int last_position = sb->SendScintilla(
            QsciScintillaBase::SCI_GETTEXTLENGTH);

    while (end_position < last_position)
    {
        int s = sb->SendScintilla(QsciScintillaBase::SCI_GETSTYLEAT,
                end_position);

        if (s != style)
            break;

        end_position = sb->SendScintilla(QsciScintillaBase::SCI_POSITIONAFTER,
                end_position);
        ++end_text_position;
    }

    *endOffset = end_text_position;

    // Convert the style to attributes.
    QString attrs;

    int back = sb->SendScintilla(QsciScintillaBase::SCI_STYLEGETBACK, style);
    addAttribute(attrs, "background-color", colourAsRGB(back));

    int fore = sb->SendScintilla(QsciScintillaBase::SCI_STYLEGETFORE, style);
    addAttribute(attrs, "color", colourAsRGB(fore));

    QFont font = fontForStyle(style);

    QString family = font.family();
    family = family.replace('\\', QLatin1String("\\\\"));
    family = family.replace(':', QLatin1String("\\:"));
    family = family.replace(',', QLatin1String("\\,"));
    family = family.replace('=', QLatin1String("\\="));
    family = family.replace(';', QLatin1String("\\;"));
    family = family.replace('\"', QLatin1String("\\\""));
    addAttribute(attrs, "font-familly",
            QLatin1Char('"') + family + QLatin1Char('"'));

    int font_size = int(font.pointSize());
    addAttribute(attrs, "font-size",
            QString::fromLatin1("%1pt").arg(font_size));

    QFont::Style font_style = font.style();
    addAttribute(attrs, "font-style",
            QString::fromLatin1((font_style == QFont::StyleItalic) ? "italic" : ((font_style == QFont::StyleOblique) ? "oblique": "normal")));

    int font_weight = font.weight();
    addAttribute(attrs, "font-weight",
            QString::fromLatin1(
                    (font_weight > QFont::Normal) ? "bold" : "normal"));

    int underline = sb->SendScintilla(QsciScintillaBase::SCI_STYLEGETUNDERLINE,
            style);
    if (underline)
        addAttribute(attrs, "text-underline-type",
                QString::fromLatin1("single"));

    return attrs;
}


// Add an attribute name/value pair.
void QsciAccessibleScintillaBase::addAttribute(QString &attrs,
        const char *name, const QString &value)
{
    attrs.append(QLatin1String(name));
    attrs.append(QChar(':'));
    attrs.append(value);
    attrs.append(QChar(';'));
}


// Convert a integer colour to an RGB string.
QString QsciAccessibleScintillaBase::colourAsRGB(int colour)
{
    return QString::fromLatin1("rgb(%1,%2,%3)").arg(colour & 0xff).arg((colour >> 8) & 0xff).arg((colour >> 16) & 0xff);
}


// Convert a integer colour to an RGB string.
QFont QsciAccessibleScintillaBase::fontForStyle(int style) const
{
    QsciScintillaBase *sb = sciWidget();
    char fontName[64];
    int len = sb->SendScintilla(QsciScintillaBase::SCI_STYLEGETFONT, style,
            fontName);
    int size = sb->SendScintilla(QsciScintillaBase::SCI_STYLEGETSIZE, style);
    bool italic = sb->SendScintilla(QsciScintillaBase::SCI_STYLEGETITALIC,
            style);
    int weight = sb->SendScintilla(QsciScintillaBase::SCI_STYLEGETWEIGHT,
            style);

    return QFont(QString::fromUtf8(fontName, len), size, weight, italic);
}


// Delete some text.
void QsciAccessibleScintillaBase::deleteText(int startOffset, int endOffset)
{
    addSelection(startOffset, endOffset);
    sciWidget()->SendScintilla(QsciScintillaBase::SCI_REPLACESEL, "");
}


// Insert some text.
void QsciAccessibleScintillaBase::insertText(int offset, const QString &text)
{
    QsciScintillaBase *sb = sciWidget();

    sb->SendScintilla(QsciScintillaBase::SCI_INSERTTEXT,
            offsetAsPosition(sb, offset), sb->textAsBytes(text).constData());
}


// Replace some text.
void QsciAccessibleScintillaBase::replaceText(int startOffset, int endOffset,
        const QString &text)
{
    QsciScintillaBase *sb = sciWidget();

    addSelection(startOffset, endOffset);
    sb->SendScintilla(QsciScintillaBase::SCI_REPLACESEL,
            sb->textAsBytes(text).constData());
}


// Return the state.
QAccessible::State QsciAccessibleScintillaBase::state() const
{
    QAccessible::State st = QAccessibleWidget::state();

    st.selectableText = true;
    st.multiLine = true;

    if (sciWidget()->SendScintilla(QsciScintillaBase::SCI_GETREADONLY))
        st.readOnly = true;
    else
        st.editable = true;

    return st;
}


// Provide access to the indivual interfaces.
void *QsciAccessibleScintillaBase::interface_cast(QAccessible::InterfaceType t)
{
    if (t == QAccessible::TextInterface)
        return static_cast<QAccessibleTextInterface *>(this);

    if (t == QAccessible::EditableTextInterface)
        return static_cast<QAccessibleEditableTextInterface *>(this);

    return QAccessibleWidget::interface_cast(t);
}


// The accessibility interface factory.
static QAccessibleInterface *factory(const QString &classname, QObject *object)
{
    if (classname == QLatin1String("QsciScintillaBase") && object && object->isWidgetType())
        return new QsciAccessibleScintillaBase(static_cast<QWidget *>(object));

    return 0;
}


#endif
