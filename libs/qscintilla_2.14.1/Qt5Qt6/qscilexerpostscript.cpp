// This module implements the QsciLexerPostScript class.
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


#include "Qsci/qscilexerpostscript.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerPostScript::QsciLexerPostScript(QObject *parent)
    : QsciLexer(parent),
      ps_tokenize(false), ps_level(3), fold_compact(true), fold_atelse(false)
{
}


// The dtor.
QsciLexerPostScript::~QsciLexerPostScript()
{
}


// Returns the language name.
const char *QsciLexerPostScript::language() const
{
    return "PostScript";
}


// Returns the lexer name.
const char *QsciLexerPostScript::lexer() const
{
    return "ps";
}


// Return the style used for braces.
int QsciLexerPostScript::braceStyle() const
{
    return ProcedureParenthesis;
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerPostScript::defaultColor(int style) const
{
    switch (style)
    {
    case Comment:
        return QColor(0x00,0x7f,0x00);

    case DSCComment:
        return QColor(0x3f,0x70,0x3f);

    case DSCCommentValue:
    case DictionaryParenthesis:
        return QColor(0x30,0x60,0xa0);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case Name:
    case ProcedureParenthesis:
        return QColor(0x00,0x00,0x00);

    case Keyword:
    case ArrayParenthesis:
        return QColor(0x00,0x00,0x7f);

    case Literal:
    case ImmediateEvalLiteral:
        return QColor(0x7f,0x7f,0x00);

    case Text:
    case Base85String:
        return QColor(0x7f,0x00,0x7f);

    case HexString:
        return QColor(0x3f,0x7f,0x3f);

    case BadStringCharacter:
        return QColor(0xff,0xff,0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont QsciLexerPostScript::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
    case DSCComment:
    case DSCCommentValue:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case Keyword:
    case ProcedureParenthesis:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);

    case Text:
#if defined(Q_OS_WIN)
        f = QFont("Times New Roman", 11);
#elif defined(Q_OS_MAC)
        f = QFont("Times New Roman", 12);
#else
        f = QFont("Bitstream Charter", 10);
#endif
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerPostScript::keywords(int set) const
{
    if (set == 1)
        return
            "$error = == FontDirectory StandardEncoding UserObjects abs add "
            "aload anchorsearch and arc arcn arcto array ashow astore atan "
            "awidthshow begin bind bitshift bytesavailable cachestatus "
            "ceiling charpath clear cleardictstack cleartomark clip clippath "
            "closefile closepath concat concatmatrix copy copypage cos count "
            "countdictstack countexecstack counttomark currentcmykcolor "
            "currentcolorspace currentdash currentdict currentfile "
            "currentflat currentfont currentgray currenthsbcolor "
            "currentlinecap currentlinejoin currentlinewidth currentmatrix "
            "currentmiterlimit currentpagedevice currentpoint currentrgbcolor "
            "currentscreen currenttransfer cvi cvlit cvn cvr cvrs cvs cvx def "
            "defaultmatrix definefont dict dictstack div dtransform dup echo "
            "end eoclip eofill eq erasepage errordict exch exec execstack "
            "executeonly executive exit exp false file fill findfont "
            "flattenpath floor flush flushfile for forall ge get getinterval "
            "grestore grestoreall gsave gt idetmatrix idiv idtransform if "
            "ifelse image imagemask index initclip initgraphics initmatrix "
            "inustroke invertmatrix itransform known kshow le length lineto "
            "ln load log loop lt makefont mark matrix maxlength mod moveto "
            "mul ne neg newpath noaccess nor not null nulldevice or pathbbox "
            "pathforall pop print prompt pstack put putinterval quit rand "
            "rcheck rcurveto read readhexstring readline readonly readstring "
            "rectstroke repeat resetfile restore reversepath rlineto rmoveto "
            "roll rotate round rrand run save scale scalefont search "
            "setblackgeneration setcachedevice setcachelimit setcharwidth "
            "setcolorscreen setcolortransfer setdash setflat setfont setgray "
            "sethsbcolor setlinecap setlinejoin setlinewidth setmatrix "
            "setmiterlimit setpagedevice setrgbcolor setscreen settransfer "
            "setvmthreshold show showpage sin sqrt srand stack start status "
            "statusdict stop stopped store string stringwidth stroke "
            "strokepath sub systemdict token token transform translate true "
            "truncate type ueofill undefineresource userdict usertime version "
            "vmstatus wcheck where widthshow write writehexstring writestring "
            "xcheck xor";

    if (set == 2)
        return
            "GlobalFontDirectory ISOLatin1Encoding SharedFontDirectory "
            "UserObject arct colorimage cshow currentblackgeneration "
            "currentcacheparams currentcmykcolor currentcolor "
            "currentcolorrendering currentcolorscreen currentcolorspace "
            "currentcolortransfer currentdevparams currentglobal "
            "currentgstate currenthalftone currentobjectformat "
            "currentoverprint currentpacking currentpagedevice currentshared "
            "currentstrokeadjust currentsystemparams currentundercolorremoval "
            "currentuserparams defineresource defineuserobject deletefile "
            "execform execuserobject filenameforall fileposition filter "
            "findencoding findresource gcheck globaldict glyphshow gstate "
            "ineofill infill instroke inueofill inufill inustroke "
            "languagelevel makepattern packedarray printobject product "
            "realtime rectclip rectfill rectstroke renamefile resourceforall "
            "resourcestatus revision rootfont scheck selectfont serialnumber "
            "setbbox setblackgeneration setcachedevice2 setcacheparams "
            "setcmykcolor setcolor setcolorrendering setcolorscreen "
            "setcolorspace setcolortranfer setdevparams setfileposition "
            "setglobal setgstate sethalftone setobjectformat setoverprint "
            "setpacking setpagedevice setpattern setshared setstrokeadjust "
            "setsystemparams setucacheparams setundercolorremoval "
            "setuserparams setvmthreshold shareddict startjob uappend ucache "
            "ucachestatus ueofill ufill undef undefinefont undefineresource "
            "undefineuserobject upath ustroke ustrokepath vmreclaim "
            "writeobject xshow xyshow yshow";

    if (set == 3)
        return
            "cliprestore clipsave composefont currentsmoothness "
            "findcolorrendering setsmoothness shfill";

    if (set == 4)
        return
            ".begintransparencygroup .begintransparencymask .bytestring "
            ".charboxpath .currentaccuratecurves .currentblendmode "
            ".currentcurvejoin .currentdashadapt .currentdotlength "
            ".currentfilladjust2 .currentlimitclamp .currentopacityalpha "
            ".currentoverprintmode .currentrasterop .currentshapealpha "
            ".currentsourcetransparent .currenttextknockout "
            ".currenttexturetransparent .dashpath .dicttomark "
            ".discardtransparencygroup .discardtransparencymask "
            ".endtransparencygroup .endtransparencymask .execn .filename "
            ".filename .fileposition .forceput .forceundef .forgetsave "
            ".getbitsrect .getdevice .inittransparencymask .knownget "
            ".locksafe .makeoperator .namestring .oserrno .oserrorstring "
            ".peekstring .rectappend .runandhide .setaccuratecurves "
            ".setblendmode .setcurvejoin .setdashadapt .setdebug "
            ".setdefaultmatrix .setdotlength .setfilladjust2 .setlimitclamp "
            ".setmaxlength .setopacityalpha .setoverprintmode .setrasterop "
            ".setsafe .setshapealpha .setsourcetransparent .settextknockout "
            ".settexturetransparent .stringbreak .stringmatch .tempfile "
            ".type1decrypt .type1encrypt .type1execchar .unread arccos "
            "arcsin copydevice copyscanlines currentdevice finddevice "
            "findlibfile findprotodevice flushpage getdeviceprops getenv "
            "makeimagedevice makewordimagedevice max min putdeviceprops "
            "setdevice";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerPostScript::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case DSCComment:
        return tr("DSC comment");

    case DSCCommentValue:
        return tr("DSC comment value");

    case Number:
        return tr("Number");

    case Name:
        return tr("Name");

    case Keyword:
        return tr("Keyword");

    case Literal:
        return tr("Literal");

    case ImmediateEvalLiteral:
        return tr("Immediately evaluated literal");

    case ArrayParenthesis:
        return tr("Array parenthesis");

    case DictionaryParenthesis:
        return tr("Dictionary parenthesis");

    case ProcedureParenthesis:
        return tr("Procedure parenthesis");

    case Text:
        return tr("Text");

    case HexString:
        return tr("Hexadecimal string");

    case Base85String:
        return tr("Base85 string");

    case BadStringCharacter:
        return tr("Bad string character");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerPostScript::defaultPaper(int style) const
{
    if (style == BadStringCharacter)
        return QColor(0xff,0x00,0x00);

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerPostScript::refreshProperties()
{
    setTokenizeProp();
    setLevelProp();
    setCompactProp();
    setAtElseProp();
}


// Read properties from the settings.
bool QsciLexerPostScript::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    ps_tokenize = qs.value(prefix + "pstokenize", false).toBool();
    ps_level = qs.value(prefix + "pslevel", 3).toInt();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    fold_atelse = qs.value(prefix + "foldatelse", false).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerPostScript::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "pstokenize", ps_tokenize);
    qs.setValue(prefix + "pslevel", ps_level);
    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "foldatelse", fold_atelse);

    return rc;
}


// Return true if tokens are marked.
bool QsciLexerPostScript::tokenize() const
{
    return ps_tokenize;
}


// Set if tokens are marked.
void QsciLexerPostScript::setTokenize(bool tokenize)
{
    ps_tokenize = tokenize;

    setTokenizeProp();
}


// Set the "ps.tokenize" property.
void QsciLexerPostScript::setTokenizeProp()
{
    emit propertyChanged("ps.tokenize",(ps_tokenize ? "1" : "0"));
}


// Return the PostScript level.
int QsciLexerPostScript::level() const
{
    return ps_level;
}


// Set the PostScript level.
void QsciLexerPostScript::setLevel(int level)
{
    ps_level = level;

    setLevelProp();
}


// Set the "ps.level" property.
void QsciLexerPostScript::setLevelProp()
{
    emit propertyChanged("ps.level", QByteArray::number(ps_level));
}


// Return true if folds are compact.
bool QsciLexerPostScript::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerPostScript::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerPostScript::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}


// Return true if else blocks can be folded.
bool QsciLexerPostScript::foldAtElse() const
{
    return fold_atelse;
}


// Set if else blocks can be folded.
void QsciLexerPostScript::setFoldAtElse(bool fold)
{
    fold_atelse = fold;

    setAtElseProp();
}


// Set the "fold.at.else" property.
void QsciLexerPostScript::setAtElseProp()
{
    emit propertyChanged("fold.at.else",(fold_atelse ? "1" : "0"));
}
