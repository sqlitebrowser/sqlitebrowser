// This module implements the QsciLexerVHDL class.
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


#include "Qsci/qscilexervhdl.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerVHDL::QsciLexerVHDL(QObject *parent)
    : QsciLexer(parent),
      fold_comments(true), fold_compact(true), fold_atelse(true),
      fold_atbegin(true), fold_atparenth(true)
{
}


// The dtor.
QsciLexerVHDL::~QsciLexerVHDL()
{
}


// Returns the language name.
const char *QsciLexerVHDL::language() const
{
    return "VHDL";
}


// Returns the lexer name.
const char *QsciLexerVHDL::lexer() const
{
    return "vhdl";
}


// Return the style used for braces.
int QsciLexerVHDL::braceStyle() const
{
    return Attribute;
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerVHDL::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x00,0x80);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case CommentLine:
        return QColor(0x3f,0x7f,0x3f);

    case Number:
    case StandardOperator:
        return QColor(0x00,0x7f,0x7f);

    case String:
        return QColor(0x7f,0x00,0x7f);

    case UnclosedString:
        return QColor(0x00,0x00,0x00);

    case Keyword:
        return QColor(0x00,0x00,0x7f);

    case Attribute:
        return QColor(0x80,0x40,0x20);

    case StandardFunction:
        return QColor(0x80,0x80,0x20);

    case StandardPackage:
        return QColor(0x20,0x80,0x20);

    case StandardType:
        return QColor(0x20,0x80,0x80);

    case KeywordSet7:
        return QColor(0x80,0x40,0x20);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerVHDL::defaultEolFill(int style) const
{
    if (style == UnclosedString)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerVHDL::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
    case CommentLine:
    case KeywordSet7:
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
const char *QsciLexerVHDL::keywords(int set) const
{
    if (set == 1)
        return
            "access after alias all architecture array assert attribute begin "
            "block body buffer bus case component configuration constant "
            "disconnect downto else elsif end entity exit file for function "
            "generate generic group guarded if impure in inertial inout is "
            "label library linkage literal loop map new next null of on open "
            "others out package port postponed procedure process pure range "
            "record register reject report return select severity shared "
            "signal subtype then to transport type unaffected units until use "
            "variable wait when while with";

    if (set == 2)
        return
            "abs and mod nand nor not or rem rol ror sla sll sra srl xnor xor";

    if (set == 3)
        return
            "left right low high ascending image value pos val succ pred "
            "leftof rightof base range reverse_range length delayed stable "
            "quiet transaction event active last_event last_active last_value "
            "driving driving_value simple_name path_name instance_name";

    if (set == 4)
        return
            "now readline read writeline write endfile resolved to_bit "
            "to_bitvector to_stdulogic to_stdlogicvector to_stdulogicvector "
            "to_x01 to_x01z to_UX01 rising_edge falling_edge is_x shift_left "
            "shift_right rotate_left rotate_right resize to_integer "
            "to_unsigned to_signed std_match to_01";

    if (set == 5)
        return
            "std ieee work standard textio std_logic_1164 std_logic_arith "
            "std_logic_misc std_logic_signed std_logic_textio "
            "std_logic_unsigned numeric_bit numeric_std math_complex "
            "math_real vital_primitives vital_timing";

    if (set == 6)
        return
            "boolean bit character severity_level integer real time "
            "delay_length natural positive string bit_vector file_open_kind "
            "file_open_status line text side width std_ulogic "
            "std_ulogic_vector std_logic std_logic_vector X01 X01Z UX01 UX01Z "
            "unsigned signed";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerVHDL::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case CommentLine:
        return tr("Comment line");

    case Number:
        return tr("Number");

    case String:
        return tr("String");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case UnclosedString:
        return tr("Unclosed string");

    case Keyword:
        return tr("Keyword");

    case StandardOperator:
        return tr("Standard operator");

    case Attribute:
        return tr("Attribute");

    case StandardFunction:
        return tr("Standard function");

    case StandardPackage:
        return tr("Standard package");

    case StandardType:
        return tr("Standard type");

    case KeywordSet7:
        return tr("User defined");

    case CommentBlock:
        return tr("Comment block");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerVHDL::defaultPaper(int style) const
{
    if (style == UnclosedString)
        return QColor(0xe0,0xc0,0xe0);

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerVHDL::refreshProperties()
{
    setCommentProp();
    setCompactProp();
    setAtElseProp();
    setAtBeginProp();
    setAtParenthProp();
}


// Read properties from the settings.
bool QsciLexerVHDL::readProperties(QSettings &qs,const QString &prefix)
{
    fold_comments = qs.value(prefix + "foldcomments", true).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    fold_atelse = qs.value(prefix + "foldatelse", true).toBool();
    fold_atbegin = qs.value(prefix + "foldatbegin", true).toBool();
    fold_atparenth = qs.value(prefix + "foldatparenthesis", true).toBool();

    return true;
}


// Write properties to the settings.
bool QsciLexerVHDL::writeProperties(QSettings &qs,const QString &prefix) const
{
    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "foldatelse", fold_atelse);
    qs.setValue(prefix + "foldatbegin", fold_atbegin);
    qs.setValue(prefix + "foldatparenthesis", fold_atparenth);

    return true;
}


// Return true if comments can be folded.
bool QsciLexerVHDL::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void QsciLexerVHDL::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerVHDL::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool QsciLexerVHDL::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerVHDL::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerVHDL::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}


// Return true if else blocks can be folded.
bool QsciLexerVHDL::foldAtElse() const
{
    return fold_atelse;
}


// Set if else blocks can be folded.
void QsciLexerVHDL::setFoldAtElse(bool fold)
{
    fold_atelse = fold;

    setAtElseProp();
}


// Set the "fold.at.else" property.
void QsciLexerVHDL::setAtElseProp()
{
    emit propertyChanged("fold.at.else",(fold_atelse ? "1" : "0"));
}


// Return true if begin blocks can be folded.
bool QsciLexerVHDL::foldAtBegin() const
{
    return fold_atbegin;
}


// Set if begin blocks can be folded.
void QsciLexerVHDL::setFoldAtBegin(bool fold)
{
    fold_atbegin = fold;

    setAtBeginProp();
}


// Set the "fold.at.Begin" property.
void QsciLexerVHDL::setAtBeginProp()
{
    emit propertyChanged("fold.at.Begin",(fold_atelse ? "1" : "0"));
}


// Return true if blocks can be folded at a parenthesis.
bool QsciLexerVHDL::foldAtParenthesis() const
{
    return fold_atparenth;
}


// Set if blocks can be folded at a parenthesis.
void QsciLexerVHDL::setFoldAtParenthesis(bool fold)
{
    fold_atparenth = fold;

    setAtParenthProp();
}


// Set the "fold.at.Parenthese" property.
void QsciLexerVHDL::setAtParenthProp()
{
    emit propertyChanged("fold.at.Parenthese",(fold_atparenth ? "1" : "0"));
}
