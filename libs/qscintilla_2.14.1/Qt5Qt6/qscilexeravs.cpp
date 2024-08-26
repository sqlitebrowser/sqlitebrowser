// This module implements the QsciLexerAVS class.
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


#include "Qsci/qscilexeravs.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerAVS::QsciLexerAVS(QObject *parent)
    : QsciLexer(parent),
      fold_comments(false), fold_compact(true)
{
}


// The dtor.
QsciLexerAVS::~QsciLexerAVS()
{
}


// Returns the language name.
const char *QsciLexerAVS::language() const
{
    return "AVS";
}


// Returns the lexer name.
const char *QsciLexerAVS::lexer() const
{
    return "avs";
}


// Return the style used for braces.
int QsciLexerAVS::braceStyle() const
{
    return Operator;
}


// Return the string of characters that comprise a word.
const char *QsciLexerAVS::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_#";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerAVS::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
    case Operator:
        return QColor(0x00, 0x00, 0x00);

    case BlockComment:
    case NestedBlockComment:
    case LineComment:
        return QColor(0x00, 0x7f, 0x00);

    case Number:
    case Function:
        return QColor(0x00, 0x7f, 0x7f);

    case String:
    case TripleString:
        return QColor(0x7f, 0x00, 0x7f);

    case Keyword:
    case Filter:
    case ClipProperty:
        return QColor(0x00, 0x00, 0x7f);

    case Plugin:
        return QColor(0x00, 0x80, 0xc0);

    case KeywordSet6:
        return QColor(0x80, 0x00, 0xff);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont QsciLexerAVS::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case BlockComment:
    case NestedBlockComment:
    case LineComment:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS", 9);
#elif defined(Q_OS_MAC)
        f = QFont("Georgia", 13);
#else
        f = QFont("Bitstream Vera Serif", 9);
#endif
        break;

    case Keyword:
    case Filter:
    case Plugin:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerAVS::keywords(int set) const
{
    if (set == 1)
        return "true false return global";

    if (set == 2)
        return
            "addborders alignedsplice amplify amplifydb animate applyrange "
            "assumebff assumefieldbased assumefps assumeframebased "
            "assumesamplerate assumescaledfps assumetff audiodub audiodubex "
            "avifilesource avisource bicubicresize bilinearresize "
            "blackmanresize blackness blankclip blur bob cache changefps "
            "colorbars colorkeymask coloryuv compare complementparity "
            "conditionalfilter conditionalreader convertaudio "
            "convertaudioto16bit convertaudioto24bit convertaudioto32bit "
            "convertaudioto8bit convertaudiotofloat convertbacktoyuy2 "
            "convertfps converttobackyuy2 converttomono converttorgb "
            "converttorgb24 converttorgb32 converttoy8 converttoyv16 "
            "converttoyv24 converttoyv411 converttoyuy2 converttoyv12 crop "
            "cropbottom delayaudio deleteframe dissolve distributor "
            "doubleweave duplicateframe ensurevbrmp3sync fadein fadein0 "
            "fadein2 fadeio fadeio0 fadeio2 fadeout fadeout0 fadeout2 "
            "fixbrokenchromaupsampling fixluminance fliphorizontal "
            "flipvertical frameevaluate freezeframe gaussresize "
            "generalconvolution getchannel getchannels getmtmode getparity "
            "grayscale greyscale histogram horizontalreduceby2 imagereader "
            "imagesource imagewriter info interleave internalcache "
            "internalcachemt invert killaudio killvideo lanczos4resize "
            "lanczosresize layer letterbox levels limiter loop mask maskhs "
            "max merge mergeargb mergechannels mergechroma mergeluma mergergb "
            "messageclip min mixaudio monotostereo normalize null "
            "opendmlsource overlay peculiarblend pointresize pulldown "
            "reduceby2 resampleaudio resetmask reverse rgbadjust scriptclip "
            "segmentedavisource segmenteddirectshowsource selecteven "
            "selectevery selectodd selectrangeevery separatefields setmtmode "
            "sharpen showalpha showblue showfiveversions showframenumber "
            "showgreen showred showsmpte showtime sincresize skewrows "
            "spatialsoften spline16resize spline36resize spline64resize ssrc "
            "stackhorizontal stackvertical subtitle subtract supereq "
            "swapfields swapuv temporalsoften timestretch tone trim turn180 "
            "turnleft turnright tweak unalignedsplice utoy utoy8 version "
            "verticalreduceby2 vtoy vtoy8 wavsource weave writefile "
            "writefileend writefileif writefilestart ytouv";

    if (set == 3)
        return
            "addgrain addgrainc agc_hdragc analyzelogo animeivtc asharp "
            "audiograph autocrop autoyuy2 avsrecursion awarpsharp "
            "bassaudiosource bicublinresize bifrost binarize blendfields "
            "blindpp blockbuster bordercontrol cfielddiff cframediff "
            "chromashift cnr2 colormatrix combmask contra convolution3d "
            "convolution3dyv12 dctfilter ddcc deblendlogo deblock deblock_qed "
            "decimate decomb dedup deen deflate degrainmedian depan "
            "depanestimate depaninterleave depanscenes depanstabilize "
            "descratch despot dfttest dgbob dgsource directshowsource "
            "distancefunction dss2 dup dupmc edeen edgemask ediupsizer eedi2 "
            "eedi3 eedi3_rpow2 expand faerydust fastbicubicresize "
            "fastbilinearresize fastediupsizer dedgemask fdecimate "
            "ffaudiosource ffdshow ffindex ffmpegsource ffmpegsource2 "
            "fft3dfilter fft3dgpu ffvideosource fielddeinterlace fielddiff "
            "fillmargins fity2uv fity2u fity2v fitu2y fitv2y fluxsmooth "
            "fluxsmoothst fluxsmootht framediff framenumber frfun3b frfun7 "
            "gicocu golddust gradfun2db grapesmoother greedyhma grid "
            "guavacomb hqdn3d hybridfupp hysteresymask ibob "
            "improvesceneswitch inflate inpand inpaintlogo interframe "
            "interlacedresize interlacedwarpedresize interleaved2planar "
            "iscombed iscombedt iscombedtivtc kerneldeint leakkernelbob "
            "leakkerneldeint limitedsharpen limitedsharpenfaster logic lsfmod "
            "lumafilter lumayv12 manalyse maskeddeinterlace maskedmerge "
            "maskedmix mblockfps mcompensate mctemporaldenoise "
            "mctemporaldenoisepp mdegrain1 mdegrain2 mdegrain3 mdepan "
            "medianblur mergehints mflow mflowblur mflowfps mflowinter "
            "minblur mipsmooth mmask moderatesharpen monitorfilter motionmask "
            "mpasource mpeg2source mrecalculate mscdetection msharpen mshow "
            "msmooth msu_fieldshiftfixer msu_frc msuper mt mt_adddiff "
            "mt_average mt_binarize mt_circle mt_clamp mt_convolution "
            "mt_deflate mt_diamond mt_edge mt_ellipse mt_expand "
            "mt_freeellipse mt_freelosange mt_freerectangle mt_hysteresis "
            "mt_infix mt_inflate mt_inpand mt_invert mt_logic mt_losange "
            "mt_lut mt_lutf mt_luts mt_lutspa mt_lutsx mt_lutxy mt_lutxyz "
            "mt_makediff mt_mappedblur mt_merge mt_motion mt_polish "
            "mt_rectangle mt_square mti mtsource multidecimate mvanalyse "
            "mvblockfps mvchangecompensate mvcompensate mvdegrain1 mvdegrain2 "
            "mvdegrain3 mvdenoise mvdepan mvflow mvflowblur mvflowfps "
            "mvflowfps2 mvflowinter mvincrease mvmask mvrecalculate "
            "mvscdetection mvshow nicac3source nicdtssource niclpcmsource "
            "nicmpasource nicmpg123source nnedi nnedi2 nnedi2_rpow2 nnedi3 "
            "nnedi3_rpow2 nomosmooth overlaymask peachsmoother pixiedust "
            "planar2interleaved qtgmc qtinput rawavsource rawsource "
            "reduceflicker reinterpolate411 removedirt removedust removegrain "
            "removegrainhd removetemporalgrain repair requestlinear "
            "reversefielddominance rgb3dlut rgdeinterlace rgsdeinterlace "
            "rgblut rotate sangnom seesaw sharpen2 showchannels "
            "showcombedtivtc smartdecimate smartdeinterlace smdegrain "
            "smoothdeinterlace smoothuv soothess soxfilter spacedust sshiq "
            "ssim ssiq stmedianfilter t3dlut tanisotropic tbilateral tcanny "
            "tcomb tcombmask tcpserver tcpsource tdecimate tdeint tedgemask "
            "telecide temporalcleaner temporalrepair temporalsmoother "
            "tfieldblank tfm tisophote tivtc tmaskblank tmaskedmerge "
            "tmaskedmerge3 tmm tmonitor tnlmeans tomsmocomp toon textsub "
            "ttempsmooth ttempsmoothf tunsharp unblock uncomb undot unfilter "
            "unsharpmask vaguedenoiser variableblur verticalcleaner "
            "videoscope vinverse vobsub vqmcalc warpedresize warpsharp "
            "xsharpen yadif yadifmod yuy2lut yv12convolution "
            "yv12interlacedreduceby2 yv12interlacedselecttopfields yv12layer "
            "yv12lut yv12lutxy yv12substract yv12torgb24 yv12toyuy2";

    if (set == 4)
        return
            "abs apply assert bool ceil chr clip continueddenominator "
            "continuednumerator cos default defined eval averagechromau "
            "averagechromav averageluma chromaudifference chromavdifference "
            "lumadifference exist exp findstr float floor frac hexvalue "
            "import int isbool isclip isfloat isint isstring lcase leftstr "
            "load_stdcall_plugin loadcplugin loadplugin loadvfapiplugin "
            "loadvirtualdubplugin log midstr muldiv nop opt_allowfloataudio "
            "opt_avipadscanlines opt_dwchannelmask opt_usewaveextensible "
            "opt_vdubplanarhack pi pow rand revstr rightstr round scriptdir "
            "scriptfile scriptname select setmemorymax "
            "setplanarlegacyalignment rgbdifference rgbdifferencefromprevious "
            "rgbdifferencetonext udifferencefromprevious udifferencetonext "
            "setworkingdir sign sin spline sqrt string strlen time ucase "
            "undefined value versionnumber versionstring uplanemax "
            "uplanemedian uplanemin uplaneminmaxdifference "
            "vdifferencefromprevious vdifferencetonext vplanemax vplanemedian "
            "vplanemin vplaneminmaxdifference ydifferencefromprevious "
            "ydifferencetonext yplanemax yplanemedian yplanemin "
            "yplaneminmaxdifference";

    if (set == 5)
        return
            "audiobits audiochannels audiolength audiolengthf audiorate "
            "framecount framerate frameratedenominator frameratenumerator "
            "getleftchannel getrightchannel hasaudio hasvideo height "
            "isaudiofloat isaudioint isfieldbased isframebased isinterleaved "
            "isplanar isrgb isrgb24 isrgb32 isyuv isyuy2 isyv12 width";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerAVS::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case BlockComment:
        return tr("Block comment");

    case NestedBlockComment:
        return tr("Nested block comment");

    case LineComment:
        return tr("Line comment");

    case Number:
        return tr("Number");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case String:
        return tr("Double-quoted string");

    case TripleString:
        return tr("Triple double-quoted string");

    case Keyword:
        return tr("Keyword");

    case Filter:
        return tr("Filter");

    case Plugin:
        return tr("Plugin");

    case Function:
        return tr("Function");

    case ClipProperty:
        return tr("Clip property");

    case KeywordSet6:
        return tr("User defined");
    }

    return QString();
}


// Refresh all properties.
void QsciLexerAVS::refreshProperties()
{
    setCommentProp();
    setCompactProp();
}


// Read properties from the settings.
bool QsciLexerAVS::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerAVS::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);

    return rc;
}


// Return true if comments can be folded.
bool QsciLexerAVS::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void QsciLexerAVS::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerAVS::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool QsciLexerAVS::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerAVS::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerAVS::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}
