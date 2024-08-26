// This module implements the QsciLexerOctave class.
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


#include "Qsci/qscilexeroctave.h"

#include <qcolor.h>
#include <qfont.h>


// The ctor.
QsciLexerOctave::QsciLexerOctave(QObject *parent)
    : QsciLexerMatlab(parent)
{
}


// The dtor.
QsciLexerOctave::~QsciLexerOctave()
{
}


// Returns the language name.
const char *QsciLexerOctave::language() const
{
    return "Octave";
}


// Returns the lexer name.
const char *QsciLexerOctave::lexer() const
{
    return "octave";
}


// Returns the set of keywords.
const char *QsciLexerOctave::keywords(int set) const
{
    if (set == 1)
        return
            "__FILE__ __LINE__ break case catch classdef continue do else "
            "elseif end end_try_catch end_unwind_protect endclassdef "
            "endenumeration endevents endfor endfunction endif endmethods "
            "endparfor endproperties endswitch endwhile enumeration events "
            "for function get global if methods otherwise parfor persistent "
            "properties return set static switch try until unwind_protect "
            "unwind_protect_cleanup while";

    return 0;
}
