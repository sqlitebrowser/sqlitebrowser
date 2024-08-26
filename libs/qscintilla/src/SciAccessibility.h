// The definition of the class that implements accessibility support.
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


#ifndef _SCIACCESSIBILITY_H
#define _SCIACCESSIBILITY_H

#include <qglobal.h>

#if !defined(QT_NO_ACCESSIBILITY)

#include <QAccessible>
#include <QAccessibleEditableTextInterface>
#include <QAccessibleTextInterface>
#include <QAccessibleWidget>
#include <QByteArray>
#include <QFont>
#include <QList>
#include <QString>


class QsciScintillaBase;


// The implementation of accessibility support.
class QsciAccessibleScintillaBase : public QAccessibleWidget,
        public QAccessibleTextInterface,
        public QAccessibleEditableTextInterface
{
public:
    explicit QsciAccessibleScintillaBase(QWidget *widget);
    ~QsciAccessibleScintillaBase();

    static void initialise();

    static void selectionChanged(QsciScintillaBase *sb, bool selection);
    static void textInserted(QsciScintillaBase *sb, int position,
            const char *text, int length);
    static void textDeleted(QsciScintillaBase *sb, int position,
            const char *text, int length);
    static void updated(QsciScintillaBase *sb);

    void selection(int selectionIndex, int *startOffset, int *endOffset) const;
    int selectionCount() const;
    void addSelection(int startOffset, int endOffset);
    void removeSelection(int selectionIndex);
    void setSelection(int selectionIndex, int startOffset, int endOffset);

    int cursorPosition() const;
    void setCursorPosition(int position);

    QString text(int startOffset, int endOffset) const;
    QString textBeforeOffset(int offset,
            QAccessible::TextBoundaryType boundaryType, int *startOffset,
            int *endOffset) const;
    QString textAfterOffset(int offset,
            QAccessible::TextBoundaryType boundaryType, int *startOffset,
            int *endOffset) const;
    QString textAtOffset(int offset,
            QAccessible::TextBoundaryType boundaryType, int *startOffset,
            int *endOffset) const;
    int characterCount() const;
    QRect characterRect(int offset) const;
    int offsetAtPoint(const QPoint &point) const;
    void scrollToSubstring(int startIndex, int endIndex);
    QString attributes(int offset, int *startOffset, int *endOffset) const;

    void deleteText(int startOffset, int endOffset);
    void insertText(int offset, const QString &text);
    void replaceText(int startOffset, int endOffset, const QString &text);

    QAccessible::State state() const;
    void *interface_cast(QAccessible::InterfaceType t);

private:
    static bool needs_initialising;
    static QList<QsciAccessibleScintillaBase *> all_accessibles;
    int current_cursor_offset;
    bool is_selection;

    static QsciAccessibleScintillaBase *findAccessible(QsciScintillaBase *sb);
    QsciScintillaBase *sciWidget() const;
    int validPosition(int offset) const;
    static bool boundaries(QsciScintillaBase *sb, int position,
            QAccessible::TextBoundaryType boundaryType, int *start_position,
            int *end_position);
    static QString textRange(QsciScintillaBase *sb, int start_position,
            int end_position);
    static int positionAsOffset(QsciScintillaBase *sb, int position);
    static void positionRangeAsOffsetRange(QsciScintillaBase *sb,
            int start_position, int end_position, int *startOffset,
            int *endOffset);
    static int offsetAsPosition(QsciScintillaBase *sb, int offset);
    static QString colourAsRGB(int colour);
    static void addAttribute(QString &attrs, const char *name,
            const QString &value);
    QFont fontForStyle(int style) const;
};


#endif

#endif
