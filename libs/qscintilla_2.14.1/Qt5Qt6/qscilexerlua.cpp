// This module implements the QsciLexerLua class.
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


#include "Qsci/qscilexerlua.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerLua::QsciLexerLua(QObject *parent)
    : QsciLexer(parent), fold_compact(true)
{
}


// The dtor.
QsciLexerLua::~QsciLexerLua()
{
}


// Returns the language name.
const char *QsciLexerLua::language() const
{
    return "Lua";
}


// Returns the lexer name.
const char *QsciLexerLua::lexer() const
{
    return "lua";
}


// Return the set of character sequences that can separate auto-completion
// words.
QStringList QsciLexerLua::autoCompletionWordSeparators() const
{
    QStringList wl;

    wl << ":" << ".";

    return wl;
}


// Return the list of characters that can start a block.
const char *QsciLexerLua::blockStart(int *style) const
{
    if (style)
        *style = Operator;

    return "";
}


// Return the style used for braces.
int QsciLexerLua::braceStyle() const
{
    return Operator;
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerLua::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x00,0x00,0x00);

    case Comment:
    case LineComment:
        return QColor(0x00,0x7f,0x00);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case Keyword:
    case BasicFunctions:
    case StringTableMathsFunctions:
    case CoroutinesIOSystemFacilities:
        return QColor(0x00,0x00,0x7f);

    case String:
    case Character:
    case LiteralString:
        return QColor(0x7f,0x00,0x7f);

    case Preprocessor:
    case Label:
        return QColor(0x7f,0x7f,0x00);

    case Operator:
    case Identifier:
        break;
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerLua::defaultEolFill(int style) const
{
    if (style == Comment || style == UnclosedString)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerLua::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
    case LineComment:
    case LiteralString:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerLua::keywords(int set) const
{
    if (set == 1)
        // Keywords.
        return
            "and break do else elseif end false for function if "
            "in local nil not or repeat return then true until "
            "while";

    if (set == 2)
        // Basic functions.
        return
            "_ALERT _ERRORMESSAGE _INPUT _PROMPT _OUTPUT _STDERR "
            "_STDIN _STDOUT call dostring foreach foreachi getn "
            "globals newtype rawget rawset require sort tinsert "
            "tremove "

            "G getfenv getmetatable ipairs loadlib next pairs "
            "pcall rawegal rawget rawset require setfenv "
            "setmetatable xpcall string table math coroutine io "
            "os debug";

    if (set == 3)
        // String, table and maths functions.
        return
            "abs acos asin atan atan2 ceil cos deg exp floor "
            "format frexp gsub ldexp log log10 max min mod rad "
            "random randomseed sin sqrt strbyte strchar strfind "
            "strlen strlower strrep strsub strupper tan "

            "string.byte string.char string.dump string.find "
            "string.len string.lower string.rep string.sub "
            "string.upper string.format string.gfind string.gsub "
            "table.concat table.foreach table.foreachi table.getn "
            "table.sort table.insert table.remove table.setn "
            "math.abs math.acos math.asin math.atan math.atan2 "
            "math.ceil math.cos math.deg math.exp math.floor "
            "math.frexp math.ldexp math.log math.log10 math.max "
            "math.min math.mod math.pi math.rad math.random "
            "math.randomseed math.sin math.sqrt math.tan";

    if (set == 4)
        // Coroutine, I/O and system facilities.
        return
            "openfile closefile readfrom writeto appendto remove "
            "rename flush seek tmpfile tmpname read write clock "
            "date difftime execute exit getenv setlocale time "

            "coroutine.create coroutine.resume coroutine.status "
            "coroutine.wrap coroutine.yield io.close io.flush "
            "io.input io.lines io.open io.output io.read "
            "io.tmpfile io.type io.write io.stdin io.stdout "
            "io.stderr os.clock os.date os.difftime os.execute "
            "os.exit os.getenv os.remove os.rename os.setlocale "
            "os.time os.tmpname";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerLua::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case LineComment:
        return tr("Line comment");

    case Number:
        return tr("Number");

    case Keyword:
        return tr("Keyword");

    case String:
        return tr("String");

    case Character:
        return tr("Character");

    case LiteralString:
        return tr("Literal string");

    case Preprocessor:
        return tr("Preprocessor");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case UnclosedString:
        return tr("Unclosed string");

    case BasicFunctions:
        return tr("Basic functions");

    case StringTableMathsFunctions:
        return tr("String, table and maths functions");

    case CoroutinesIOSystemFacilities:
        return tr("Coroutines, i/o and system facilities");

    case KeywordSet5:
        return tr("User defined 1");

    case KeywordSet6:
        return tr("User defined 2");

    case KeywordSet7:
        return tr("User defined 3");

    case KeywordSet8:
        return tr("User defined 4");

    case Label:
        return tr("Label");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerLua::defaultPaper(int style) const
{
    switch (style)
    {
    case Comment:
        return QColor(0xd0,0xf0,0xf0);

    case LiteralString:
        return QColor(0xe0,0xff,0xff);

    case UnclosedString:
        return QColor(0xe0,0xc0,0xe0);

    case BasicFunctions:
        return QColor(0xd0,0xff,0xd0);

    case StringTableMathsFunctions:
        return QColor(0xd0,0xd0,0xff);

    case CoroutinesIOSystemFacilities:
        return QColor(0xff,0xd0,0xd0);
    }

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerLua::refreshProperties()
{
    setCompactProp();
}


// Read properties from the settings.
bool QsciLexerLua::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_compact = qs.value(prefix + "foldcompact", true).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerLua::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcompact", fold_compact);

    return rc;
}


// Return true if folds are compact.
bool QsciLexerLua::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact.
void QsciLexerLua::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerLua::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}
