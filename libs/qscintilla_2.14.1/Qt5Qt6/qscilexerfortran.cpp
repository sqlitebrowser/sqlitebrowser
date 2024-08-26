// This module implements the QsciLexerFortran class.
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


#include "Qsci/qscilexerfortran.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerFortran::QsciLexerFortran(QObject *parent)
    : QsciLexerFortran77(parent)
{
}


// The dtor.
QsciLexerFortran::~QsciLexerFortran()
{
}


// Returns the language name.
const char *QsciLexerFortran::language() const
{
    return "Fortran";
}


// Returns the lexer name.
const char *QsciLexerFortran::lexer() const
{
    return "fortran";
}


// Returns the set of keywords.
const char *QsciLexerFortran::keywords(int set) const
{
    if (set == 2)
        return
            "abs achar acos acosd adjustl adjustr aimag aimax0 aimin0 aint "
            "ajmax0 ajmin0 akmax0 akmin0 all allocated alog alog10 amax0 "
            "amax1 amin0 amin1 amod anint any asin asind associated atan "
            "atan2 atan2d atand bitest bitl bitlr bitrl bjtest bit_size "
            "bktest break btest cabs ccos cdabs cdcos cdexp cdlog cdsin "
            "cdsqrt ceiling cexp char clog cmplx conjg cos cosd cosh count "
            "cpu_time cshift csin csqrt dabs dacos dacosd dasin dasind datan "
            "datan2 datan2d datand date date_and_time dble dcmplx dconjg dcos "
            "dcosd dcosh dcotan ddim dexp dfloat dflotk dfloti dflotj digits "
            "dim dimag dint dlog dlog10 dmax1 dmin1 dmod dnint dot_product "
            "dprod dreal dsign dsin dsind dsinh dsqrt dtan dtand dtanh "
            "eoshift epsilon errsns exp exponent float floati floatj floatk "
            "floor fraction free huge iabs iachar iand ibclr ibits ibset "
            "ichar idate idim idint idnint ieor ifix iiabs iiand iibclr "
            "iibits iibset iidim iidint iidnnt iieor iifix iint iior iiqint "
            "iiqnnt iishft iishftc iisign ilen imax0 imax1 imin0 imin1 imod "
            "index inint inot int int1 int2 int4 int8 iqint iqnint ior ishft "
            "ishftc isign isnan izext jiand jibclr jibits jibset jidim jidint "
            "jidnnt jieor jifix jint jior jiqint jiqnnt jishft jishftc jisign "
            "jmax0 jmax1 jmin0 jmin1 jmod jnint jnot jzext kiabs kiand kibclr "
            "kibits kibset kidim kidint kidnnt kieor kifix kind kint kior "
            "kishft kishftc kisign kmax0 kmax1 kmin0 kmin1 kmod knint knot "
            "kzext lbound leadz len len_trim lenlge lge lgt lle llt log log10 "
            "logical lshift malloc matmul max max0 max1 maxexponent maxloc "
            "maxval merge min min0 min1 minexponent minloc minval mod modulo "
            "mvbits nearest nint not nworkers number_of_processors pack "
            "popcnt poppar precision present product radix random "
            "random_number random_seed range real repeat reshape rrspacing "
            "rshift scale scan secnds selected_int_kind selected_real_kind "
            "set_exponent shape sign sin sind sinh size sizeof sngl snglq "
            "spacing spread sqrt sum system_clock tan tand tanh tiny transfer "
            "transpose trim ubound unpack verify";

    if (set == 3)
        return
            "cdabs cdcos cdexp cdlog cdsin cdsqrt cotan cotand dcmplx dconjg "
            "dcotan dcotand decode dimag dll_export dll_import doublecomplex "
            "dreal dvchk encode find flen flush getarg getcharqq getcl getdat "
            "getenv gettim hfix ibchng identifier imag int1 int2 int4 intc "
            "intrup invalop iostat_msg isha ishc ishl jfix lacfar locking "
            "locnear map nargs nbreak ndperr ndpexc offset ovefl peekcharqq "
            "precfill prompt qabs qacos qacosd qasin qasind qatan qatand "
            "qatan2 qcmplx qconjg qcos qcosd qcosh qdim qexp qext qextd "
            "qfloat qimag qlog qlog10 qmax1 qmin1 qmod qreal qsign qsin qsind "
            "qsinh qsqrt qtan qtand qtanh ran rand randu rewrite segment "
            "setdat settim system timer undfl unlock union val virtual "
            "volatile zabs zcos zexp zlog zsin zsqrt";

    return QsciLexerFortran77::keywords(set);
}
