// This module implements the QsciLexerMarkdown class.
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


#include "Qsci/qscilexermarkdown.h"

#include <qfont.h>


// The ctor.
QsciLexerMarkdown::QsciLexerMarkdown(QObject *parent)
    : QsciLexer(parent)
{
}


// The dtor.
QsciLexerMarkdown::~QsciLexerMarkdown()
{
}


// Returns the language name.
const char *QsciLexerMarkdown::language() const
{
    return "Markdown";
}


// Returns the lexer name.
const char *QsciLexerMarkdown::lexer() const
{
    return "markdown";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerMarkdown::defaultColor(int style) const
{
    switch (style)
    {
    case Special:
        return QColor(0xcc, 0x00, 0xff);

    case StrongEmphasisAsterisks:
    case StrongEmphasisUnderscores:
        return QColor(0x22, 0x44, 0x66);

    case EmphasisAsterisks:
    case EmphasisUnderscores:
        return QColor(0x88, 0x00, 0x88);

    case Header1:
        return QColor(0xff, 0x77, 0x00);

    case Header2:
        return QColor(0xdd, 0x66, 0x00);

    case Header3:
        return QColor(0xbb, 0x55, 0x00);

    case Header4:
        return QColor(0x99, 0x44, 0x00);

    case Header5:
        return QColor(0x77, 0x33, 0x00);

    case Header6:
        return QColor(0x55, 0x22, 0x00);

    case Prechar:
        return QColor(0x00, 0x00, 0x00);

    case UnorderedListItem:
        return QColor(0x82, 0x5d, 0x00);

    case OrderedListItem:
        return QColor(0x00, 0x00, 0x70);

    case BlockQuote:
        return QColor(0x00, 0x66, 0x00);

    case StrikeOut:
        return QColor(0xdd, 0xdd, 0xdd);

    case HorizontalRule:
        return QColor(0x1f, 0x1c, 0x1b);

    case Link:
        return QColor(0x00, 0x00, 0xaa);

    case CodeBackticks:
    case CodeDoubleBackticks:
        return QColor(0x7f, 0x00, 0x7f);

    case CodeBlock:
        return QColor(0x00, 0x45, 0x8a);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont QsciLexerMarkdown::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case StrongEmphasisAsterisks:
    case StrongEmphasisUnderscores:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case EmphasisAsterisks:
    case EmphasisUnderscores:
        f = QsciLexer::defaultFont(style);
        f.setItalic(true);
        break;

    case Header1:
    case Header2:
    case Header3:
    case Header4:
    case Header5:
    case Header6:
#if defined(Q_OS_WIN)
        f = QFont("Courier New", 10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono", 9);
#endif
        f.setBold(true);
        break;

    case HorizontalRule:
    case CodeBackticks:
    case CodeDoubleBackticks:
    case CodeBlock:
#if defined(Q_OS_WIN)
        f = QFont("Courier New", 10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono", 9);
#endif
        break;

    case Link:
        f = QsciLexer::defaultFont(style);
        f.setUnderline(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the background colour of the text for a style.
QColor QsciLexerMarkdown::defaultPaper(int style) const
{
    switch (style)
    {
    case Prechar:
        return QColor(0xee, 0xee, 0xaa);

    case UnorderedListItem:
        return QColor(0xde, 0xd8, 0xc3);

    case OrderedListItem:
        return QColor(0xb8, 0xc3, 0xe1);

    case BlockQuote:
        return QColor(0xcb, 0xdc, 0xcb);

    case StrikeOut:
        return QColor(0xaa, 0x00, 0x00);

    case HorizontalRule:
        return QColor(0xe7, 0xd1, 0xc9);

    case CodeBackticks:
    case CodeDoubleBackticks:
        return QColor(0xef, 0xff, 0xef);

    case CodeBlock:
        return QColor(0xc5, 0xe0, 0xf5);
    }

    return QsciLexer::defaultPaper(style);
}


// Returns the user name of a style.
QString QsciLexerMarkdown::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Special:
        return tr("Special");

	case StrongEmphasisAsterisks:
        return tr("Strong emphasis using double asterisks");

	case StrongEmphasisUnderscores:
        return tr("Strong emphasis using double underscores");

	case EmphasisAsterisks:
        return tr("Emphasis using single asterisks");

	case EmphasisUnderscores:
        return tr("Emphasis using single underscores");

	case Header1:
        return tr("Level 1 header");

	case Header2:
        return tr("Level 2 header");

	case Header3:
        return tr("Level 3 header");

	case Header4:
        return tr("Level 4 header");

	case Header5:
        return tr("Level 5 header");

	case Header6:
        return tr("Level 6 header");

	case Prechar:
        return tr("Pre-char");

	case UnorderedListItem:
        return tr("Unordered list item");

	case OrderedListItem:
        return tr("Ordered list item");

	case BlockQuote:
        return tr("Block quote");

	case StrikeOut:
        return tr("Strike out");

	case HorizontalRule:
        return tr("Horizontal rule");

	case Link:
        return tr("Link");

	case CodeBackticks:
        return tr("Code between backticks");

	case CodeDoubleBackticks:
        return tr("Code between double backticks");

	case CodeBlock:
        return tr("Code block");
    }

    return QString();
}
