// This module implements the QsciLexerD class.
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


#include "Qsci/qscilexerd.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerD::QsciLexerD(QObject *parent)
    : QsciLexer(parent),
      fold_atelse(false), fold_comments(false), fold_compact(true)
{
}


// The dtor.
QsciLexerD::~QsciLexerD()
{
}


// Returns the language name.
const char *QsciLexerD::language() const
{
    return "D";
}


// Returns the lexer name.
const char *QsciLexerD::lexer() const
{
    return "d";
}


// Return the set of character sequences that can separate auto-completion
// words.
QStringList QsciLexerD::autoCompletionWordSeparators() const
{
    QStringList wl;

    wl << ".";

    return wl;
}


// Return the list of keywords that can start a block.
const char *QsciLexerD::blockStartKeyword(int *style) const
{
    if (style)
        *style = Keyword;

    return "case catch class default do else finally for foreach "
           "foreach_reverse if private protected public struct try union "
           "while";
}


// Return the list of characters that can start a block.
const char *QsciLexerD::blockStart(int *style) const
{
    if (style)
        *style = Operator;

    return "{";
}


// Return the list of characters that can end a block.
const char *QsciLexerD::blockEnd(int *style) const
{
    if (style)
        *style = Operator;

    return "}";
}


// Return the style used for braces.
int QsciLexerD::braceStyle() const
{
    return Operator;
}


// Return the string of characters that comprise a word.
const char *QsciLexerD::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_#";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerD::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Comment:
    case CommentLine:
        return QColor(0x00,0x7f,0x00);

    case CommentDoc:
    case CommentLineDoc:
        return QColor(0x3f,0x70,0x3f);

    case CommentNested:
        return QColor(0xa0,0xc0,0xa0);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case Keyword:
    case KeywordSecondary:
    case KeywordDoc:
    case Typedefs:
        return QColor(0x00,0x00,0x7f);

    case String:
        return QColor(0x7f,0x00,0x7f);

    case Character:
        return QColor(0x7f,0x00,0x7f);

    case Operator:
    case UnclosedString:
        return QColor(0x00,0x00,0x00);

    case Identifier:
        break;

    case CommentDocKeyword:
        return QColor(0x30,0x60,0xa0);

    case CommentDocKeywordError:
        return QColor(0x80,0x40,0x20);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerD::defaultEolFill(int style) const
{
    if (style == UnclosedString)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerD::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
    case CommentLine:
    case CommentDoc:
    case CommentNested:
    case CommentLineDoc:
    case CommentDocKeyword:
    case CommentDocKeywordError:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case Keyword:
    case KeywordSecondary:
    case KeywordDoc:
    case Typedefs:
    case Operator:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case String:
    case UnclosedString:
#if defined(Q_OS_WIN)
        f = QFont("Courier New",10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono",9);
#endif
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerD::keywords(int set) const
{
    if (set == 1)
        return
            "abstract alias align asm assert auto body bool break byte case "
            "cast catch cdouble cent cfloat char class const continue creal "
            "dchar debug default delegate delete deprecated do double else "
            "enum export extern false final finally float for foreach "
            "foreach_reverse function goto idouble if ifloat import in inout "
            "int interface invariant ireal is lazy long mixin module new null "
            "out override package pragma private protected public real return "
            "scope short static struct super switch synchronized template "
            "this throw true try typedef typeid typeof ubyte ucent uint ulong "
            "union unittest ushort version void volatile wchar while with";

    if (set == 3)
        return
            "a addindex addtogroup anchor arg attention author b brief bug c "
            "class code date def defgroup deprecated dontinclude e em endcode "
            "endhtmlonly endif endlatexonly endlink endverbatim enum example "
            "exception f$ f[ f] file fn hideinitializer htmlinclude htmlonly "
            "if image include ingroup internal invariant interface latexonly "
            "li line link mainpage name namespace nosubgrouping note overload "
            "p page par param post pre ref relates remarks return retval sa "
            "section see showinitializer since skip skipline struct "
            "subsection test throw todo typedef union until var verbatim "
            "verbinclude version warning weakgroup $ @ \\ & < > # { }";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerD::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Block comment");

    case CommentLine:
        return tr("Line comment");

    case CommentDoc:
        return tr("DDoc style block comment");

    case CommentNested:
        return tr("Nesting comment");

    case Number:
        return tr("Number");

    case Keyword:
        return tr("Keyword");

    case KeywordSecondary:
        return tr("Secondary keyword");

    case KeywordDoc:
        return tr("Documentation keyword");

    case Typedefs:
        return tr("Type definition");

    case String:
        return tr("String");

    case UnclosedString:
        return tr("Unclosed string");

    case Character:
        return tr("Character");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case CommentLineDoc:
        return tr("DDoc style line comment");

    case CommentDocKeyword:
        return tr("DDoc keyword");

    case CommentDocKeywordError:
        return tr("DDoc keyword error");

    case BackquoteString:
        return tr("Backquoted string");

    case RawString:
        return tr("Raw string");

    case KeywordSet5:
        return tr("User defined 1");

    case KeywordSet6:
        return tr("User defined 2");

    case KeywordSet7:
        return tr("User defined 3");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerD::defaultPaper(int style) const
{
    if (style == UnclosedString)
        return QColor(0xe0,0xc0,0xe0);

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerD::refreshProperties()
{
    setAtElseProp();
    setCommentProp();
    setCompactProp();
}


// Read properties from the settings.
bool QsciLexerD::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_atelse = qs.value(prefix + "foldatelse", false).toBool();
    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerD::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldatelse", fold_atelse);
    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);

    return rc;
}


// Return true if else can be folded.
bool QsciLexerD::foldAtElse() const
{
    return fold_atelse;
}


// Set if else can be folded.
void QsciLexerD::setFoldAtElse(bool fold)
{
    fold_atelse = fold;

    setAtElseProp();
}


// Set the "fold.at.else" property.
void QsciLexerD::setAtElseProp()
{
    emit propertyChanged("fold.at.else",(fold_atelse ? "1" : "0"));
}


// Return true if comments can be folded.
bool QsciLexerD::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void QsciLexerD::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerD::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool QsciLexerD::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerD::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerD::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}
