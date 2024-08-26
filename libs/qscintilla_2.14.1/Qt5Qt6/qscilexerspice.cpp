// This module implements the QsciLexerSpice class.
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


#include "Qsci/qscilexerspice.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
QsciLexerSpice::QsciLexerSpice(QObject *parent)
    : QsciLexer(parent)
{
}


// The dtor.
QsciLexerSpice::~QsciLexerSpice()
{
}


// Returns the language name.
const char *QsciLexerSpice::language() const
{
    return "Spice";
}


// Returns the lexer name.
const char *QsciLexerSpice::lexer() const
{
    return "spice";
}


// Return the style used for braces.
int QsciLexerSpice::braceStyle() const
{
    return Parameter;
}


// Returns the set of keywords.
const char *QsciLexerSpice::keywords(int set) const
{
    if (set == 1)
        return
            "ac alias alter alterparam append askvalues assertvalid "
            "autoscale break compose copy copytodoc dc delete destroy "
            "destroyvec diff display disto dowhile echo else end errorstop "
            "fftinit filter foreach fourier freqtotime function "
            "functionundef goto homecursors if isdisplayed label let "
            "linearize listing load loadaccumulator makelabel movelabel "
            "makesmithplot movecursorleft movecursorright msgbox nameplot "
            "newplot nextparam noise nopoints op plot plotf plotref poly "
            "print printcursors printevent printname printplot printstatus "
            "printtext printtol printunits printval printvector pwl pz quit "
            "removesmithplot rename repeat resume rotate runs rusage save "
            "sendplot sendscript sens set setcursor setdoc setlabel "
            "setlabeltype setmargins setnthtrigger setunits setvec setparam "
            "setplot setquery setscaletype settracecolor settracestyle "
            "setsource settrigger setvec setxlimits setylimits show showmod "
            "sort status step stop switch tf timetofreq timetowave tran "
            "unalias unlet unset unalterparam update version view wavefilter "
            "wavetotime where while write";

    if (set == 2)
        return
            "abs askvalue atan average ceil cos db differentiate "
            "differentiatex exp finalvalue floor getcursorx getcursory "
            "getcursory0 getcursory1 getparam im ln initialvalue integrate "
            "integratex interpolate isdef isdisplayed j log length mag max "
            "maxscale mean meanpts min minscale nextplot nextvector norm "
            "operatingpoint ph phase phaseextend pk_pk pos pulse re rms "
            "rmspts rnd sameplot sin sqrt stddev stddevpts tan tfall "
            "tolerance trise unitvec vector";

    if (set == 3)
        return "param nodeset include options dcconv subckt ends model";

    return 0;
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerSpice::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Command:
    case Function:
        return QColor(0x00,0x00,0x7f);

    case Parameter:
        return QColor(0x00,0x40,0xe0);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case Delimiter:
        return QColor(0x00,0x00,0x00);

    case Value:
        return QColor(0x7f,0x00,0x7f);

    case Comment:
        return QColor(0x00,0x7f,0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont QsciLexerSpice::defaultFont(int style) const
{
    QFont f;

    if (style == Comment)
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
    else
    {
        f = QsciLexer::defaultFont(style);

        if (style == Function || style == Delimiter)
            f.setBold(true);
    }

    return f;
}


// Returns the user name of a style.
QString QsciLexerSpice::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Identifier:
        return tr("Identifier");

    case Command:
        return tr("Command");

    case Function:
        return tr("Function");

    case Parameter:
        return tr("Parameter");

    case Number:
        return tr("Number");

    case Delimiter:
        return tr("Delimiter");

    case Value:
        return tr("Value");

    case Comment:
        return tr("Comment");
    }

    return QString();
}
