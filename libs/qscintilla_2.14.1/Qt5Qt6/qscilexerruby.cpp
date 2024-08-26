// This module implements the QsciLexerRuby class.
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


#include "Qsci/qscilexerruby.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerRuby::QsciLexerRuby(QObject *parent)
    : QsciLexer(parent), fold_comments(false), fold_compact(true)
{
}


// The dtor.
QsciLexerRuby::~QsciLexerRuby()
{
}


// Returns the language name.
const char *QsciLexerRuby::language() const
{
    return "Ruby";
}


// Returns the lexer name.
const char *QsciLexerRuby::lexer() const
{
    return "ruby";
}


// Return the list of words that can start a block.
const char *QsciLexerRuby::blockStart(int *style) const
{
    if (style)
        *style = Keyword;

    return "do";
}


// Return the list of words that can start end a block.
const char *QsciLexerRuby::blockEnd(int *style) const
{
    if (style)
        *style = Keyword;

    return "end";
}


// Return the list of words that can start end a block.
const char *QsciLexerRuby::blockStartKeyword(int *style) const
{
    if (style)
        *style = Keyword;

    return "def class if do elsif else case while for";
}


// Return the style used for braces.
int QsciLexerRuby::braceStyle() const
{
    return Operator;
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerRuby::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case POD:
        return QColor(0x00,0x40,0x00);

    case Number:
    case FunctionMethodName:
        return QColor(0x00,0x7f,0x7f);

    case Keyword:
    case DemotedKeyword:
        return QColor(0x00,0x00,0x7f);

    case DoubleQuotedString:
    case SingleQuotedString:
    case HereDocument:
    case PercentStringq:
    case PercentStringQ:
        return QColor(0x7f,0x00,0x7f);

    case ClassName:
        return QColor(0x00,0x00,0xff);

    case Regex:
    case HereDocumentDelimiter:
    case PercentStringr:
    case PercentStringw:
        return QColor(0x00,0x00,0x00);

    case Global:
        return QColor(0x80,0x00,0x80);

    case Symbol:
        return QColor(0xc0,0xa0,0x30);

    case ModuleName:
        return QColor(0xa0,0x00,0xa0);

    case InstanceVariable:
        return QColor(0xb0,0x00,0x80);

    case ClassVariable:
        return QColor(0x80,0x00,0xb0);

    case Backticks:
    case PercentStringx:
        return QColor(0xff,0xff,0x00);

    case DataSection:
        return QColor(0x60,0x00,0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerRuby::defaultEolFill(int style) const
{
    bool fill;

    switch (style)
    {
    case POD:
    case DataSection:
    case HereDocument:
        fill = true;
        break;

    default:
        fill = QsciLexer::defaultEolFill(style);
    }

    return fill;
}


// Returns the font of the text for a style.
QFont QsciLexerRuby::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case POD:
    case DoubleQuotedString:
    case SingleQuotedString:
    case PercentStringq:
    case PercentStringQ:
#if defined(Q_OS_WIN)
        f = QFont("Courier New",10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono",9);
#endif
        break;

    case Keyword:
    case ClassName:
    case FunctionMethodName:
    case Operator:
    case ModuleName:
    case DemotedKeyword:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerRuby::keywords(int set) const
{
    if (set == 1)
        return
            "__FILE__ and def end in or self unless __LINE__ "
            "begin defined? ensure module redo super until BEGIN "
            "break do false next rescue then when END case else "
            "for nil require retry true while alias class elsif "
            "if not return undef yield";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerRuby::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Error:
        return tr("Error");

    case Comment:
        return tr("Comment");

    case POD:
        return tr("POD");

    case Number:
        return tr("Number");

    case Keyword:
        return tr("Keyword");

    case DoubleQuotedString:
        return tr("Double-quoted string");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case ClassName:
        return tr("Class name");

    case FunctionMethodName:
        return tr("Function or method name");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case Regex:
        return tr("Regular expression");

    case Global:
        return tr("Global");

    case Symbol:
        return tr("Symbol");

    case ModuleName:
        return tr("Module name");

    case InstanceVariable:
        return tr("Instance variable");

    case ClassVariable:
        return tr("Class variable");

    case Backticks:
        return tr("Backticks");

    case DataSection:
        return tr("Data section");

    case HereDocumentDelimiter:
        return tr("Here document delimiter");

    case HereDocument:
        return tr("Here document");

    case PercentStringq:
        return tr("%q string");

    case PercentStringQ:
        return tr("%Q string");

    case PercentStringx:
        return tr("%x string");

    case PercentStringr:
        return tr("%r string");

    case PercentStringw:
        return tr("%w string");

    case DemotedKeyword:
        return tr("Demoted keyword");

    case Stdin:
        return tr("stdin");

    case Stdout:
        return tr("stdout");

    case Stderr:
        return tr("stderr");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerRuby::defaultPaper(int style) const
{
    switch (style)
    {
    case Error:
        return QColor(0xff,0x00,0x00);

    case POD:
        return QColor(0xc0,0xff,0xc0);

    case Regex:
    case PercentStringr:
        return QColor(0xa0,0xff,0xa0);

    case Backticks:
    case PercentStringx:
        return QColor(0xa0,0x80,0x80);

    case DataSection:
        return QColor(0xff,0xf0,0xd8);

    case HereDocumentDelimiter:
    case HereDocument:
        return QColor(0xdd,0xd0,0xdd);

    case PercentStringw:
        return QColor(0xff,0xff,0xe0);

    case Stdin:
    case Stdout:
    case Stderr:
        return QColor(0xff,0x80,0x80);
    }

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerRuby::refreshProperties()
{
    setCommentProp();
    setCompactProp();
}


// Read properties from the settings.
bool QsciLexerRuby::readProperties(QSettings &qs, const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerRuby::writeProperties(QSettings &qs, const QString &prefix) const
{
    int rc = true;

    qs.value(prefix + "foldcomments", fold_comments);
    qs.value(prefix + "foldcompact", fold_compact);

    return rc;
}


// Set if comments can be folded.
void QsciLexerRuby::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerRuby::setCommentProp()
{
    emit propertyChanged("fold.comment", (fold_comments ? "1" : "0"));
}


// Set if folds are compact
void QsciLexerRuby::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerRuby::setCompactProp()
{
    emit propertyChanged("fold.compact", (fold_compact ? "1" : "0"));
}
