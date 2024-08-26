// This module implements the QsciLexerFortran77 class.
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


#include "Qsci/qscilexerfortran77.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerFortran77::QsciLexerFortran77(QObject *parent)
    : QsciLexer(parent), fold_compact(true)
{
}


// The dtor.
QsciLexerFortran77::~QsciLexerFortran77()
{
}


// Returns the language name.
const char *QsciLexerFortran77::language() const
{
    return "Fortran77";
}


// Returns the lexer name.
const char *QsciLexerFortran77::lexer() const
{
    return "f77";
}


// Return the style used for braces.
int QsciLexerFortran77::braceStyle() const
{
    return Default;
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerFortran77::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case SingleQuotedString:
    case DoubleQuotedString:
        return QColor(0x7f,0x00,0x7f);

    case UnclosedString:
    case Operator:
    case DottedOperator:
    case Continuation:
        return QColor(0x00,0x00,0x00);

    case Identifier:
        break;

    case Keyword:
        return QColor(0x00,0x00,0x7f);

    case IntrinsicFunction:
        return QColor(0xb0,0x00,0x40);

    case ExtendedFunction:
        return QColor(0xb0,0x40,0x80);

    case PreProcessor:
        return QColor(0x7f,0x7f,0x00);

    case Label:
        return QColor(0xe0,0xc0,0xe0);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerFortran77::defaultEolFill(int style) const
{
    if (style == UnclosedString)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerFortran77::defaultFont(int style) const
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

    case Operator:
    case DottedOperator:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerFortran77::keywords(int set) const
{
    if (set == 1)
        return
            "access action advance allocatable allocate apostrophe assign "
            "assignment associate asynchronous backspace bind blank blockdata "
            "call case character class close common complex contains continue "
            "cycle data deallocate decimal delim default dimension direct do "
            "dowhile double doubleprecision else elseif elsewhere encoding "
            "end endassociate endblockdata enddo endfile endforall "
            "endfunction endif endinterface endmodule endprogram endselect "
            "endsubroutine endtype endwhere entry eor equivalence err errmsg "
            "exist exit external file flush fmt forall form format formatted "
            "function go goto id if implicit in include inout integer inquire "
            "intent interface intrinsic iomsg iolength iostat kind len "
            "logical module name named namelist nextrec nml none nullify "
            "number only open opened operator optional out pad parameter pass "
            "pause pending pointer pos position precision print private "
            "program protected public quote read readwrite real rec recl "
            "recursive result return rewind save select selectcase selecttype "
            "sequential sign size stat status stop stream subroutine target "
            "then to type unformatted unit use value volatile wait where "
            "while write";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerFortran77::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Number:
        return tr("Number");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case DoubleQuotedString:
        return tr("Double-quoted string");

    case UnclosedString:
        return tr("Unclosed string");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case Keyword:
        return tr("Keyword");

    case IntrinsicFunction:
        return tr("Intrinsic function");

    case ExtendedFunction:
        return tr("Extended function");

    case PreProcessor:
        return tr("Pre-processor block");

    case DottedOperator:
        return tr("Dotted operator");

    case Label:
        return tr("Label");

    case Continuation:
        return tr("Continuation");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerFortran77::defaultPaper(int style) const
{
    if (style == UnclosedString)
        return QColor(0xe0,0xc0,0xe0);

    if (style == Continuation)
        return QColor(0xf0,0xe0,0x80);

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerFortran77::refreshProperties()
{
    setCompactProp();
}


// Read properties from the settings.
bool QsciLexerFortran77::readProperties(QSettings &qs, const QString &prefix)
{
    int rc = true;

    fold_compact = qs.value(prefix + "foldcompact", true).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerFortran77::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcompact", fold_compact);

    return rc;
}


// Return true if folds are compact.
bool QsciLexerFortran77::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerFortran77::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerFortran77::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}
