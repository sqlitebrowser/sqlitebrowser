// This module implements the QsciLexerPascal class.
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


#include "Qsci/qscilexerpascal.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerPascal::QsciLexerPascal(QObject *parent)
    : QsciLexer(parent),
      fold_comments(false), fold_compact(true), fold_preproc(false),
      smart_highlight(true)
{
}


// The dtor.
QsciLexerPascal::~QsciLexerPascal()
{
}


// Returns the language name.
const char *QsciLexerPascal::language() const
{
    return "Pascal";
}


// Returns the lexer name.
const char *QsciLexerPascal::lexer() const
{
    return "pascal";
}


// Return the set of character sequences that can separate auto-completion
// words.
QStringList QsciLexerPascal::autoCompletionWordSeparators() const
{
    QStringList wl;

    wl << "." << "^";

    return wl;
}


// Return the list of keywords that can start a block.
const char *QsciLexerPascal::blockStartKeyword(int *style) const
{
    if (style)
        *style = Keyword;

    return
        "case class do else for then private protected public published "
        "repeat try while type";
}


// Return the list of characters that can start a block.
const char *QsciLexerPascal::blockStart(int *style) const
{
    if (style)
        *style = Operator;

    return "begin";
}


// Return the list of characters that can end a block.
const char *QsciLexerPascal::blockEnd(int *style) const
{
    if (style)
        *style = Operator;

    return "end";
}


// Return the style used for braces.
int QsciLexerPascal::braceStyle() const
{
    return Operator;
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerPascal::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Identifier:
        break;

    case Comment:
    case CommentParenthesis:
    case CommentLine:
        return QColor(0x00,0x7f,0x00);

    case PreProcessor:
    case PreProcessorParenthesis:
        return QColor(0x7f,0x7f,0x00);

    case Number:
    case HexNumber:
        return QColor(0x00,0x7f,0x7f);

    case Keyword:
        return QColor(0x00,0x00,0x7f);

    case SingleQuotedString:
    case Character:
        return QColor(0x7f,0x00,0x7f);

    case UnclosedString:
    case Operator:
        return QColor(0x00,0x00,0x00);

    case Asm:
        return QColor(0x80,0x40,0x80);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerPascal::defaultEolFill(int style) const
{
    if (style == UnclosedString)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerPascal::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
    case CommentParenthesis:
    case CommentLine:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case Keyword:
    case Operator:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case SingleQuotedString:
#if defined(Q_OS_WIN)
        f = QFont("Times New Roman", 11);
#elif defined(Q_OS_MAC)
        f = QFont("Times New Roman", 12);
#else
        f = QFont("Bitstream Charter", 10);
#endif
        f.setItalic(true);
        break;

    case UnclosedString:
#if defined(Q_OS_WIN)
        f = QFont("Courier New", 10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono", 9);
#endif
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the background colour of the text for a style.
QColor QsciLexerPascal::defaultPaper(int style) const
{
    if (style == UnclosedString)
        return QColor(0xe0,0xc0,0xe0);

    return QsciLexer::defaultPaper(style);
}


// Returns the set of keywords.
const char *QsciLexerPascal::keywords(int set) const
{
    if (set == 1)
        return
            "absolute abstract and array as asm assembler automated begin "
            "case cdecl class const constructor delayed deprecated destructor "
            "dispid dispinterface div do downto dynamic else end except "
            "experimental export exports external far file final finalization "
            "finally for forward function goto helper if implementation in "
            "inherited initialization inline interface is label library "
            "message mod near nil not object of on operator or out overload "
            "override packed pascal platform private procedure program "
            "property protected public published raise record reference "
            "register reintroduce repeat resourcestring safecall sealed set "
            "shl shr static stdcall strict string then threadvar to try type "
            "unit unsafe until uses var varargs virtual while winapi with xor"
            "add default implements index name nodefault read readonly remove "
            "stored write writeonly"
            "package contains requires";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerPascal::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Identifier:
        return tr("Identifier");

    case Comment:
        return tr("'{ ... }' style comment");

    case CommentParenthesis:
        return tr("'(* ... *)' style comment");

    case CommentLine:
        return tr("Line comment");

    case PreProcessor:
        return tr("'{$ ... }' style pre-processor block");

    case PreProcessorParenthesis:
        return tr("'(*$ ... *)' style pre-processor block");

    case Number:
        return tr("Number");

    case HexNumber:
        return tr("Hexadecimal number");

    case Keyword:
        return tr("Keyword");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case UnclosedString:
        return tr("Unclosed string");

    case Character:
        return tr("Character");

    case Operator:
        return tr("Operator");

    case Asm:
        return tr("Inline asm");
    }

    return QString();
}


// Refresh all properties.
void QsciLexerPascal::refreshProperties()
{
    setCommentProp();
    setCompactProp();
    setPreprocProp();
    setSmartHighlightProp();
}


// Read properties from the settings.
bool QsciLexerPascal::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    fold_preproc = qs.value(prefix + "foldpreprocessor", true).toBool();
    smart_highlight = qs.value(prefix + "smarthighlight", true).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerPascal::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "foldpreprocessor", fold_preproc);
    qs.setValue(prefix + "smarthighlight", smart_highlight);

    return rc;
}


// Return true if comments can be folded.
bool QsciLexerPascal::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void QsciLexerPascal::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerPascal::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool QsciLexerPascal::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerPascal::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerPascal::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}


// Return true if preprocessor blocks can be folded.
bool QsciLexerPascal::foldPreprocessor() const
{
    return fold_preproc;
}


// Set if preprocessor blocks can be folded.
void QsciLexerPascal::setFoldPreprocessor(bool fold)
{
    fold_preproc = fold;

    setPreprocProp();
}


// Set the "fold.preprocessor" property.
void QsciLexerPascal::setPreprocProp()
{
    emit propertyChanged("fold.preprocessor",(fold_preproc ? "1" : "0"));
}


// Return true if smart highlighting is enabled.
bool QsciLexerPascal::smartHighlighting() const
{
    return smart_highlight;
}


// Set if smart highlighting is enabled.
void QsciLexerPascal::setSmartHighlighting(bool enabled)
{
    smart_highlight = enabled;

    setSmartHighlightProp();
}


// Set the "lexer.pascal.smart.highlighting" property.
void QsciLexerPascal::setSmartHighlightProp()
{
    emit propertyChanged("lexer.pascal.smart.highlighting", (smart_highlight ? "1" : "0"));
}
