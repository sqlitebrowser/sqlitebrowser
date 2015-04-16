// This class defines the "official" low-level API.
//
// Copyright (c) 2015 Riverbank Computing Limited <info@riverbankcomputing.com>
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


#ifndef QSCISCINTILLABASE_H
#define QSCISCINTILLABASE_H

#ifdef __APPLE__
extern "C++" {
#endif

#include <qglobal.h>

#include <QAbstractScrollArea>
#include <QByteArray>
#include <QPoint>
#include <QTimer>

#include <Qsci/qsciglobal.h>


QT_BEGIN_NAMESPACE
class QColor;
class QImage;
class QMimeData;
class QPainter;
class QPixmap;
QT_END_NAMESPACE

class QsciScintillaQt;


//! \brief The QsciScintillaBase class implements the Scintilla editor widget
//! and its low-level API.
//!
//! Scintilla (http://www.scintilla.org) is a powerful C++ editor class that
//! supports many features including syntax styling, error indicators, code
//! completion and call tips. It is particularly useful as a programmer's
//! editor.
//!
//! QsciScintillaBase is a port to Qt of Scintilla. It implements the standard
//! Scintilla API which consists of a number of messages each taking up to
//! two arguments.
//!
//! See QsciScintilla for the implementation of a higher level API that is more
//! consistent with the rest of the Qt toolkit.
class QSCINTILLA_EXPORT QsciScintillaBase : public QAbstractScrollArea
{
    Q_OBJECT

public:
    //! The low-level Scintilla API is implemented as a set of messages each of
    //! which takes up to two parameters (\a wParam and \a lParam) and
    //! optionally return a value. This enum defines all the possible messages.
    enum
    {
        //!
        SCI_START = 2000,

        //!
        SCI_OPTIONAL_START = 3000,

        //!
        SCI_LEXER_START = 4000,

        //! This message appends some text to the end of the document.
        //! \a wParam is the length of the text.
        //! \a lParam is the text to be appended.
        SCI_ADDTEXT = 2001,

        //!
        SCI_ADDSTYLEDTEXT = 2002,

        //!
        SCI_INSERTTEXT = 2003,

        //!
        SCI_CLEARALL = 2004,

        //!
        SCI_CLEARDOCUMENTSTYLE = 2005,

        //!
        SCI_GETLENGTH = 2006,

        //!
        SCI_GETCHARAT = 2007,

        //! This message returns the current position.
        //! 
        //! \sa SCI_SETCURRENTPOS
        SCI_GETCURRENTPOS = 2008,

        //! This message returns the anchor.
        //! 
        //! \sa SCI_SETANCHOR
        SCI_GETANCHOR = 2009,

        //!
        SCI_GETSTYLEAT = 2010,

        //!
        SCI_REDO = 2011,

        //!
        SCI_SETUNDOCOLLECTION = 2012,

        //!
        SCI_SELECTALL = 2013,

        //! This message marks the current state of the text as the the save
        //! point. This is usually done when the text is saved or loaded.
        //! 
        //! \sa SCN_SAVEPOINTREACHED(), SCN_SAVEPOINTLEFT()
        SCI_SETSAVEPOINT = 2014,

        //!
        SCI_GETSTYLEDTEXT = 2015,

        //!
        SCI_CANREDO = 2016,

        //! This message returns the line that contains a particular instance
        //! of a marker.
        //! \a wParam is the handle of the marker.
        //!
        //! \sa SCI_MARKERADD
        SCI_MARKERLINEFROMHANDLE = 2017,

        //! This message removes a particular instance of a marker.
        //! \a wParam is the handle of the marker.
        //!
        //! \sa SCI_MARKERADD
        SCI_MARKERDELETEHANDLE = 2018,

        //!
        SCI_GETUNDOCOLLECTION = 2019,

        //!
        SCI_GETVIEWWS = 2020,

        //!
        SCI_SETVIEWWS = 2021,

        //!
        SCI_POSITIONFROMPOINT = 2022,

        //!
        SCI_POSITIONFROMPOINTCLOSE = 2023,

        //!
        SCI_GOTOLINE = 2024,

        //! This message clears the current selection and sets the current
        //! position.
        //! \a wParam is the new current position.
        //! 
        //! \sa SCI_SETCURRENTPOS
        SCI_GOTOPOS = 2025,

        //! This message sets the anchor.
        //! \a wParam is the new anchor.
        //! 
        //! \sa SCI_GETANCHOR
        SCI_SETANCHOR = 2026,

        //!
        SCI_GETCURLINE = 2027,

        //! This message returns the character position of the start of the
        //! text that needs to be syntax styled.
        //! 
        //! \sa SCN_STYLENEEDED()
        SCI_GETENDSTYLED = 2028,

        //!
        SCI_CONVERTEOLS = 2029,

        //!
        SCI_GETEOLMODE = 2030,

        //!
        SCI_SETEOLMODE = 2031,

        //!
        SCI_STARTSTYLING = 2032,

        //!
        SCI_SETSTYLING = 2033,

        //!
        SCI_GETBUFFEREDDRAW = 2034,

        //!
        SCI_SETBUFFEREDDRAW = 2035,

        //!
        SCI_SETTABWIDTH = 2036,

        //!
        SCI_GETTABWIDTH = 2121,

        //!
        SCI_SETCODEPAGE = 2037,

        //! This message sets the symbol used to draw one of 32 markers.  Some
        //! markers have pre-defined uses, see the SC_MARKNUM_* values.
        //! \a wParam is the number of the marker.
        //! \a lParam is the marker symbol and is one of the SC_MARK_* values.
        //!
        //! \sa SCI_MARKERADD, SCI_MARKERDEFINEPIXMAP,
        //! SCI_MARKERDEFINERGBAIMAGE
        SCI_MARKERDEFINE = 2040,

        //! This message sets the foreground colour used to draw a marker.  A
        //! colour is represented as a 24 bit value.  The 8 least significant
        //! bits correspond to red, the middle 8 bits correspond to green, and
        //! the 8 most significant bits correspond to blue.  The default value
        //! is 0x000000.
        //! \a wParam is the number of the marker.
        //! \a lParam is the colour.
        //!
        //! \sa SCI_MARKERSETBACK
        SCI_MARKERSETFORE = 2041,

        //! This message sets the background colour used to draw a marker.  A
        //! colour is represented as a 24 bit value.  The 8 least significant
        //! bits correspond to red, the middle 8 bits correspond to green, and
        //! the 8 most significant bits correspond to blue.  The default value
        //! is 0xffffff.
        //! \a wParam is the number of the marker.
        //! \a lParam is the colour.
        //!
        //! \sa SCI_MARKERSETFORE
        SCI_MARKERSETBACK = 2042,

        //! This message adds a marker to a line.  A handle for the marker is
        //! returned which can be used to track the marker's position.
        //! \a wParam is the line number.
        //! \a lParam is the number of the marker.
        //!
        //! \sa SCI_MARKERDELETE, SCI_MARKERDELETEALL,
        //! SCI_MARKERDELETEHANDLE
        SCI_MARKERADD = 2043,

        //! This message deletes a marker from a line.
        //! \a wParam is the line number.
        //! \a lParam is the number of the marker.
        //!
        //! \sa SCI_MARKERADD, SCI_MARKERDELETEALL
        SCI_MARKERDELETE = 2044,

        //! This message deletes all occurences of a marker.
        //! \a wParam is the number of the marker.  If \a wParam is -1 then all
        //! markers are removed.
        //!
        //! \sa SCI_MARKERADD, SCI_MARKERDELETE
        SCI_MARKERDELETEALL = 2045,

        //! This message returns the 32 bit mask of markers at a line.
        //! \a wParam is the line number.
        SCI_MARKERGET = 2046,

        //! This message looks for the next line to contain at least one marker
        //! contained in a 32 bit mask of markers and returns the line number.
        //! \a wParam is the line number to start the search from.
        //! \a lParam is the mask of markers to search for.
        //!
        //! \sa SCI_MARKERPREVIOUS
        SCI_MARKERNEXT = 2047,

        //! This message looks for the previous line to contain at least one
        //! marker contained in a 32 bit mask of markers and returns the line
        //! number.
        //! \a wParam is the line number to start the search from.
        //! \a lParam is the mask of markers to search for.
        //!
        //! \sa SCI_MARKERNEXT
        SCI_MARKERPREVIOUS = 2048,

        //! This message sets the symbol used to draw one of the 32 markers to
        //! a pixmap.  Pixmaps use the SC_MARK_PIXMAP marker symbol.
        //! \a wParam is the number of the marker.
        //! \a lParam is a pointer to a QPixmap instance.  Note that in other
        //! ports of Scintilla this is a pointer to either raw or textual XPM
        //! image data.
        //!
        //! \sa SCI_MARKERDEFINE, SCI_MARKERDEFINERGBAIMAGE
        SCI_MARKERDEFINEPIXMAP = 2049,

        //! This message sets what can be displayed in a margin.
        //! \a wParam is the number of the margin.
        //! \a lParam is the logical or of the SC_MARGIN_* values.
        //!
        //! \sa SCI_GETMARGINTYPEN
        SCI_SETMARGINTYPEN = 2240,

        //! This message returns what can be displayed in a margin.
        //! \a wParam is the number of the margin.
        //!
        //! \sa SCI_SETMARGINTYPEN
        SCI_GETMARGINTYPEN = 2241,

        //! This message sets the width of a margin in pixels.
        //! \a wParam is the number of the margin.
        //! \a lParam is the new margin width.
        //!
        //! \sa SCI_GETMARGINWIDTHN
        SCI_SETMARGINWIDTHN = 2242,

        //! This message returns the width of a margin in pixels.
        //! \a wParam is the number of the margin.
        //!
        //! \sa SCI_SETMARGINWIDTHN
        SCI_GETMARGINWIDTHN = 2243,

        //! This message sets the mask of a margin.  The mask is a 32 value
        //! with one bit for each possible marker.  If a bit is set then the
        //! corresponding marker is displayed.  By default, all markers are
        //! displayed.
        //! \a wParam is the number of the margin.
        //! \a lParam is the new margin mask.
        //!
        //! \sa SCI_GETMARGINMASKN, SCI_MARKERDEFINE
        SCI_SETMARGINMASKN = 2244,

        //! This message returns the mask of a margin.
        //! \a wParam is the number of the margin.
        //!
        //! \sa SCI_SETMARGINMASKN
        SCI_GETMARGINMASKN = 2245,

        //! This message sets the sensitivity of a margin to mouse clicks.
        //! \a wParam is the number of the margin.
        //! \a lParam is non-zero to make the margin sensitive to mouse clicks.
        //! When the mouse is clicked the SCN_MARGINCLICK() signal is emitted.
        //!
        //! \sa SCI_GETMARGINSENSITIVEN, SCN_MARGINCLICK()
        SCI_SETMARGINSENSITIVEN = 2246,

        //! This message returns the sensitivity of a margin to mouse clicks.
        //! \a wParam is the number of the margin.
        //!
        //! \sa SCI_SETMARGINSENSITIVEN, SCN_MARGINCLICK()
        SCI_GETMARGINSENSITIVEN = 2247,

        //! This message sets the cursor shape displayed over a margin.
        //! \a wParam is the number of the margin.
        //! \a lParam is the cursor shape, normally either SC_CURSORARROW or
        //! SC_CURSORREVERSEARROW.  Note that, currently, QScintilla implements
        //! both of these as Qt::ArrowCursor.
        //!
        //! \sa SCI_GETMARGINCURSORN
        SCI_SETMARGINCURSORN = 2248,

        //! This message returns the cursor shape displayed over a margin.
        //! \a wParam is the number of the margin.
        //!
        //! \sa SCI_SETMARGINCURSORN
        SCI_GETMARGINCURSORN = 2249,

        //!
        SCI_STYLECLEARALL = 2050,

        //!
        SCI_STYLESETFORE = 2051,

        //!
        SCI_STYLESETBACK = 2052,

        //!
        SCI_STYLESETBOLD = 2053,

        //!
        SCI_STYLESETITALIC = 2054,

        //!
        SCI_STYLESETSIZE = 2055,

        //!
        SCI_STYLESETFONT = 2056,

        //!
        SCI_STYLESETEOLFILLED = 2057,

        //!
        SCI_STYLERESETDEFAULT = 2058,

        //!
        SCI_STYLESETUNDERLINE = 2059,

        //!
        SCI_STYLESETCASE = 2060,

        //!
        SCI_STYLESETSIZEFRACTIONAL = 2061,

        //!
        SCI_STYLEGETSIZEFRACTIONAL = 2062,

        //!
        SCI_STYLESETWEIGHT = 2063,

        //!
        SCI_STYLEGETWEIGHT = 2064,

        //!
        SCI_STYLESETCHARACTERSET = 2066,

        //!
        SCI_SETSELFORE = 2067,

        //!
        SCI_SETSELBACK = 2068,

        //!
        SCI_SETCARETFORE = 2069,

        //!
        SCI_ASSIGNCMDKEY = 2070,

        //!
        SCI_CLEARCMDKEY = 2071,

        //!
        SCI_CLEARALLCMDKEYS = 2072,

        //!
        SCI_SETSTYLINGEX = 2073,

        //!
        SCI_STYLESETVISIBLE = 2074,

        //!
        SCI_GETCARETPERIOD = 2075,

        //!
        SCI_SETCARETPERIOD = 2076,

        //!
        SCI_SETWORDCHARS = 2077,

        //!
        SCI_BEGINUNDOACTION = 2078,

        //!
        SCI_ENDUNDOACTION = 2079,

        //!
        SCI_INDICSETSTYLE = 2080,

        //!
        SCI_INDICGETSTYLE = 2081,

        //!
        SCI_INDICSETFORE = 2082,

        //!
        SCI_INDICGETFORE = 2083,

        //!
        SCI_SETWHITESPACEFORE = 2084,

        //!
        SCI_SETWHITESPACEBACK = 2085,

        //!
        SCI_SETWHITESPACESIZE = 2086,

        //!
        SCI_GETWHITESPACESIZE = 2087,

        //!
        SCI_SETSTYLEBITS = 2090,

        //!
        SCI_GETSTYLEBITS = 2091,

        //!
        SCI_SETLINESTATE = 2092,

        //!
        SCI_GETLINESTATE = 2093,

        //!
        SCI_GETMAXLINESTATE = 2094,

        //!
        SCI_GETCARETLINEVISIBLE = 2095,

        //!
        SCI_SETCARETLINEVISIBLE = 2096,

        //!
        SCI_GETCARETLINEBACK = 2097,

        //!
        SCI_SETCARETLINEBACK = 2098,

        //!
        SCI_STYLESETCHANGEABLE = 2099,

        //!
        SCI_AUTOCSHOW = 2100,

        //!
        SCI_AUTOCCANCEL = 2101,

        //!
        SCI_AUTOCACTIVE = 2102,

        //!
        SCI_AUTOCPOSSTART = 2103,

        //!
        SCI_AUTOCCOMPLETE = 2104,

        //!
        SCI_AUTOCSTOPS = 2105,

        //!
        SCI_AUTOCSETSEPARATOR = 2106,

        //!
        SCI_AUTOCGETSEPARATOR = 2107,

        //!
        SCI_AUTOCSELECT = 2108,

        //!
        SCI_AUTOCSETCANCELATSTART = 2110,

        //!
        SCI_AUTOCGETCANCELATSTART = 2111,

        //!
        SCI_AUTOCSETFILLUPS = 2112,

        //!
        SCI_AUTOCSETCHOOSESINGLE = 2113,

        //!
        SCI_AUTOCGETCHOOSESINGLE = 2114,

        //!
        SCI_AUTOCSETIGNORECASE = 2115,

        //!
        SCI_AUTOCGETIGNORECASE = 2116,

        //!
        SCI_USERLISTSHOW = 2117,

        //!
        SCI_AUTOCSETAUTOHIDE = 2118,

        //!
        SCI_AUTOCGETAUTOHIDE = 2119,

        //!
        SCI_AUTOCSETDROPRESTOFWORD = 2270,

        //!
        SCI_AUTOCGETDROPRESTOFWORD = 2271,

        //!
        SCI_SETINDENT = 2122,

        //!
        SCI_GETINDENT = 2123,

        //!
        SCI_SETUSETABS = 2124,

        //!
        SCI_GETUSETABS = 2125,

        //!
        SCI_SETLINEINDENTATION = 2126,

        //!
        SCI_GETLINEINDENTATION = 2127,

        //!
        SCI_GETLINEINDENTPOSITION = 2128,

        //!
        SCI_GETCOLUMN = 2129,

        //!
        SCI_SETHSCROLLBAR = 2130,

        //!
        SCI_GETHSCROLLBAR = 2131,

        //!
        SCI_SETINDENTATIONGUIDES = 2132,

        //!
        SCI_GETINDENTATIONGUIDES = 2133,

        //!
        SCI_SETHIGHLIGHTGUIDE = 2134,

        //!
        SCI_GETHIGHLIGHTGUIDE = 2135,

        //!
        SCI_GETLINEENDPOSITION = 2136,

        //!
        SCI_GETCODEPAGE = 2137,

        //!
        SCI_GETCARETFORE = 2138,

        //! This message returns a non-zero value if the document is read-only.
        //! 
        //! \sa SCI_SETREADONLY
        SCI_GETREADONLY = 2140,

        //! This message sets the current position.
        //! \a wParam is the new current position.
        //! 
        //! \sa SCI_GETCURRENTPOS
        SCI_SETCURRENTPOS = 2141,

        //!
        SCI_SETSELECTIONSTART = 2142,

        //!
        SCI_GETSELECTIONSTART = 2143,

        //!
        SCI_SETSELECTIONEND = 2144,

        //!
        SCI_GETSELECTIONEND = 2145,

        //!
        SCI_SETPRINTMAGNIFICATION = 2146,

        //!
        SCI_GETPRINTMAGNIFICATION = 2147,

        //!
        SCI_SETPRINTCOLOURMODE = 2148,

        //!
        SCI_GETPRINTCOLOURMODE = 2149,

        //!
        SCI_FINDTEXT = 2150,

        //!
        SCI_FORMATRANGE = 2151,

        //!
        SCI_GETFIRSTVISIBLELINE = 2152,

        //!
        SCI_GETLINE = 2153,

        //!
        SCI_GETLINECOUNT = 2154,

        //!
        SCI_SETMARGINLEFT = 2155,

        //!
        SCI_GETMARGINLEFT = 2156,

        //!
        SCI_SETMARGINRIGHT = 2157,

        //!
        SCI_GETMARGINRIGHT = 2158,

        //! This message returns a non-zero value if the document has been
        //! modified.
        SCI_GETMODIFY = 2159,

        //!
        SCI_SETSEL = 2160,

        //!
        SCI_GETSELTEXT = 2161,

        //!
        SCI_GETTEXTRANGE = 2162,

        //!
        SCI_HIDESELECTION = 2163,

        //!
        SCI_POINTXFROMPOSITION = 2164,

        //!
        SCI_POINTYFROMPOSITION = 2165,

        //!
        SCI_LINEFROMPOSITION = 2166,

        //!
        SCI_POSITIONFROMLINE = 2167,

        //!
        SCI_LINESCROLL = 2168,

        //!
        SCI_SCROLLCARET = 2169,

        //!
        SCI_REPLACESEL = 2170,

        //! This message sets the read-only state of the document.
        //! \a wParam is the new read-only state of the document.
        //! 
        //! \sa SCI_GETREADONLY
        SCI_SETREADONLY = 2171,

        //!
        SCI_NULL = 2172,

        //!
        SCI_CANPASTE = 2173,

        //!
        SCI_CANUNDO = 2174,

        //! This message empties the undo buffer.
        SCI_EMPTYUNDOBUFFER = 2175,

        //!
        SCI_UNDO = 2176,

        //!
        SCI_CUT = 2177,

        //!
        SCI_COPY = 2178,

        //!
        SCI_PASTE = 2179,

        //!
        SCI_CLEAR = 2180,

        //! This message sets the text of the document.
        //! \a wParam is unused.
        //! \a lParam is the new text of the document.
        //! 
        //! \sa SCI_GETTEXT
        SCI_SETTEXT = 2181,

        //! This message gets the text of the document.
        //! \a wParam is size of the buffer that the text is copied to.
        //! \a lParam is the address of the buffer that the text is copied to.
        //! 
        //! \sa SCI_SETTEXT
        SCI_GETTEXT = 2182,

        //! This message returns the length of the document.
        SCI_GETTEXTLENGTH = 2183,

        //!
        SCI_GETDIRECTFUNCTION = 2184,

        //!
        SCI_GETDIRECTPOINTER = 2185,

        //!
        SCI_SETOVERTYPE = 2186,

        //!
        SCI_GETOVERTYPE = 2187,

        //!
        SCI_SETCARETWIDTH = 2188,

        //!
        SCI_GETCARETWIDTH = 2189,

        //!
        SCI_SETTARGETSTART = 2190,

        //!
        SCI_GETTARGETSTART = 2191,

        //!
        SCI_SETTARGETEND = 2192,

        //!
        SCI_GETTARGETEND = 2193,

        //!
        SCI_REPLACETARGET = 2194,

        //!
        SCI_REPLACETARGETRE = 2195,

        //!
        SCI_SEARCHINTARGET = 2197,

        //!
        SCI_SETSEARCHFLAGS = 2198,

        //!
        SCI_GETSEARCHFLAGS = 2199,

        //!
        SCI_CALLTIPSHOW = 2200,

        //!
        SCI_CALLTIPCANCEL = 2201,

        //!
        SCI_CALLTIPACTIVE = 2202,

        //!
        SCI_CALLTIPPOSSTART = 2203,

        //!
        SCI_CALLTIPSETHLT = 2204,

        //!
        SCI_CALLTIPSETBACK = 2205,

        //!
        SCI_CALLTIPSETFORE = 2206,

        //!
        SCI_CALLTIPSETFOREHLT = 2207,

        //!
        SCI_AUTOCSETMAXWIDTH = 2208,

        //!
        SCI_AUTOCGETMAXWIDTH = 2209,

        //! This message is not implemented.
        SCI_AUTOCSETMAXHEIGHT = 2210,

        //!
        SCI_AUTOCGETMAXHEIGHT = 2211,

        //!
        SCI_CALLTIPUSESTYLE = 2212,

        //!
        SCI_CALLTIPSETPOSITION = 2213,

        //!
        SCI_CALLTIPSETPOSSTART = 2214,

        //!
        SCI_VISIBLEFROMDOCLINE = 2220,

        //!
        SCI_DOCLINEFROMVISIBLE = 2221,

        //!
        SCI_SETFOLDLEVEL = 2222,

        //!
        SCI_GETFOLDLEVEL = 2223,

        //!
        SCI_GETLASTCHILD = 2224,

        //!
        SCI_GETFOLDPARENT = 2225,

        //!
        SCI_SHOWLINES = 2226,

        //!
        SCI_HIDELINES = 2227,

        //!
        SCI_GETLINEVISIBLE = 2228,

        //!
        SCI_SETFOLDEXPANDED = 2229,

        //!
        SCI_GETFOLDEXPANDED = 2230,

        //!
        SCI_TOGGLEFOLD = 2231,

        //!
        SCI_ENSUREVISIBLE = 2232,

        //!
        SCI_SETFOLDFLAGS = 2233,

        //!
        SCI_ENSUREVISIBLEENFORCEPOLICY = 2234,

        //!
        SCI_WRAPCOUNT = 2235,

        //!
        SCI_GETALLLINESVISIBLE = 2236,

        //!
        SCI_FOLDLINE = 2237,

        //!
        SCI_FOLDCHILDREN = 2238,

        //!
        SCI_EXPANDCHILDREN = 2239,

        //!
        SCI_SETTABINDENTS = 2260,

        //!
        SCI_GETTABINDENTS = 2261,

        //!
        SCI_SETBACKSPACEUNINDENTS = 2262,

        //!
        SCI_GETBACKSPACEUNINDENTS = 2263,

        //!
        SCI_SETMOUSEDWELLTIME = 2264,

        //!
        SCI_GETMOUSEDWELLTIME = 2265,

        //!
        SCI_WORDSTARTPOSITION = 2266,

        //!
        SCI_WORDENDPOSITION = 2267,

        //!
        SCI_SETWRAPMODE = 2268,

        //!
        SCI_GETWRAPMODE = 2269,

        //!
        SCI_SETLAYOUTCACHE = 2272,

        //!
        SCI_GETLAYOUTCACHE = 2273,

        //!
        SCI_SETSCROLLWIDTH = 2274,

        //!
        SCI_GETSCROLLWIDTH = 2275,

        //! This message returns the width of some text when rendered in a
        //! particular style.
        //! \a wParam is the style number and is one of the STYLE_* values or
        //! one of the styles defined by a lexer.
        //! \a lParam is a pointer to the text.
        SCI_TEXTWIDTH = 2276,

        //!
        SCI_SETENDATLASTLINE = 2277,

        //!
        SCI_GETENDATLASTLINE = 2278,

        //!
        SCI_TEXTHEIGHT = 2279,

        //!
        SCI_SETVSCROLLBAR = 2280,

        //!
        SCI_GETVSCROLLBAR = 2281,

        //!
        SCI_APPENDTEXT = 2282,

        //!
        SCI_GETTWOPHASEDRAW = 2283,

        //!
        SCI_SETTWOPHASEDRAW = 2284,

        //!
        SCI_AUTOCGETTYPESEPARATOR = 2285,

        //!
        SCI_AUTOCSETTYPESEPARATOR = 2286,

        //!
        SCI_TARGETFROMSELECTION = 2287,

        //!
        SCI_LINESJOIN = 2288,

        //!
        SCI_LINESSPLIT = 2289,

        //!
        SCI_SETFOLDMARGINCOLOUR = 2290,

        //!
        SCI_SETFOLDMARGINHICOLOUR = 2291,

        //!
        SCI_MARKERSETBACKSELECTED = 2292,

        //!
        SCI_MARKERENABLEHIGHLIGHT = 2293,

        //!
        SCI_LINEDOWN = 2300,

        //!
        SCI_LINEDOWNEXTEND = 2301,

        //!
        SCI_LINEUP = 2302,

        //!
        SCI_LINEUPEXTEND = 2303,

        //!
        SCI_CHARLEFT = 2304,

        //!
        SCI_CHARLEFTEXTEND = 2305,

        //!
        SCI_CHARRIGHT = 2306,

        //!
        SCI_CHARRIGHTEXTEND = 2307,

        //!
        SCI_WORDLEFT = 2308,

        //!
        SCI_WORDLEFTEXTEND = 2309,

        //!
        SCI_WORDRIGHT = 2310,

        //!
        SCI_WORDRIGHTEXTEND = 2311,

        //!
        SCI_HOME = 2312,

        //!
        SCI_HOMEEXTEND = 2313,

        //!
        SCI_LINEEND = 2314,

        //!
        SCI_LINEENDEXTEND = 2315,

        //!
        SCI_DOCUMENTSTART = 2316,

        //!
        SCI_DOCUMENTSTARTEXTEND = 2317,

        //!
        SCI_DOCUMENTEND = 2318,

        //!
        SCI_DOCUMENTENDEXTEND = 2319,

        //!
        SCI_PAGEUP = 2320,

        //!
        SCI_PAGEUPEXTEND = 2321,

        //!
        SCI_PAGEDOWN = 2322,

        //!
        SCI_PAGEDOWNEXTEND = 2323,

        //!
        SCI_EDITTOGGLEOVERTYPE = 2324,

        //!
        SCI_CANCEL = 2325,

        //!
        SCI_DELETEBACK = 2326,

        //!
        SCI_TAB = 2327,

        //!
        SCI_BACKTAB = 2328,

        //!
        SCI_NEWLINE = 2329,

        //!
        SCI_FORMFEED = 2330,

        //!
        SCI_VCHOME = 2331,

        //!
        SCI_VCHOMEEXTEND = 2332,

        //!
        SCI_ZOOMIN = 2333,

        //!
        SCI_ZOOMOUT = 2334,

        //!
        SCI_DELWORDLEFT = 2335,

        //!
        SCI_DELWORDRIGHT = 2336,

        //!
        SCI_LINECUT = 2337,

        //!
        SCI_LINEDELETE = 2338,

        //!
        SCI_LINETRANSPOSE = 2339,

        //!
        SCI_LOWERCASE = 2340,

        //!
        SCI_UPPERCASE = 2341,

        //!
        SCI_LINESCROLLDOWN = 2342,

        //!
        SCI_LINESCROLLUP = 2343,

        //!
        SCI_DELETEBACKNOTLINE = 2344,

        //!
        SCI_HOMEDISPLAY = 2345,

        //!
        SCI_HOMEDISPLAYEXTEND = 2346,

        //!
        SCI_LINEENDDISPLAY = 2347,

        //!
        SCI_LINEENDDISPLAYEXTEND = 2348,

        //!
        SCI_MOVECARETINSIDEVIEW = 2401,

        //!
        SCI_LINELENGTH = 2350,

        //!
        SCI_BRACEHIGHLIGHT = 2351,

        //!
        SCI_BRACEBADLIGHT = 2352,

        //!
        SCI_BRACEMATCH = 2353,

        //!
        SCI_GETVIEWEOL = 2355,

        //!
        SCI_SETVIEWEOL = 2356,

        //!
        SCI_GETDOCPOINTER = 2357,

        //!
        SCI_SETDOCPOINTER = 2358,

        //!
        SCI_SETMODEVENTMASK = 2359,

        //!
        SCI_GETEDGECOLUMN = 2360,

        //!
        SCI_SETEDGECOLUMN = 2361,

        //!
        SCI_GETEDGEMODE = 2362,

        //!
        SCI_SETEDGEMODE = 2363,

        //!
        SCI_GETEDGECOLOUR = 2364,

        //!
        SCI_SETEDGECOLOUR = 2365,

        //!
        SCI_SEARCHANCHOR = 2366,

        //!
        SCI_SEARCHNEXT = 2367,

        //!
        SCI_SEARCHPREV = 2368,

        //!
        SCI_LINESONSCREEN = 2370,

        //!
        SCI_USEPOPUP = 2371,

        //!
        SCI_SELECTIONISRECTANGLE = 2372,

        //!
        SCI_SETZOOM = 2373,

        //!
        SCI_GETZOOM = 2374,

        //!
        SCI_CREATEDOCUMENT = 2375,

        //!
        SCI_ADDREFDOCUMENT = 2376,

        //!
        SCI_RELEASEDOCUMENT = 2377,

        //!
        SCI_GETMODEVENTMASK = 2378,

        //!
        SCI_SETFOCUS = 2380,

        //!
        SCI_GETFOCUS = 2381,

        //!
        SCI_SETSTATUS = 2382,

        //!
        SCI_GETSTATUS = 2383,

        //!
        SCI_SETMOUSEDOWNCAPTURES = 2384,

        //!
        SCI_GETMOUSEDOWNCAPTURES = 2385,

        //!
        SCI_SETCURSOR = 2386,

        //!
        SCI_GETCURSOR = 2387,

        //!
        SCI_SETCONTROLCHARSYMBOL = 2388,

        //!
        SCI_GETCONTROLCHARSYMBOL = 2389,

        //!
        SCI_WORDPARTLEFT = 2390,

        //!
        SCI_WORDPARTLEFTEXTEND = 2391,

        //!
        SCI_WORDPARTRIGHT = 2392,

        //!
        SCI_WORDPARTRIGHTEXTEND = 2393,

        //!
        SCI_SETVISIBLEPOLICY = 2394,

        //!
        SCI_DELLINELEFT = 2395,

        //!
        SCI_DELLINERIGHT = 2396,

        //!
        SCI_SETXOFFSET = 2397,

        //!
        SCI_GETXOFFSET = 2398,

        //!
        SCI_CHOOSECARETX = 2399,

        //!
        SCI_GRABFOCUS = 2400,

        //!
        SCI_SETXCARETPOLICY = 2402,

        //!
        SCI_SETYCARETPOLICY = 2403,

        //!
        SCI_LINEDUPLICATE = 2404,

        //! This message takes a copy of an image and registers it so that it
        //! can be refered to by a unique integer identifier.
        //! \a wParam is the image's identifier.
        //! \a lParam is a pointer to a QPixmap instance.  Note that in other
        //! ports of Scintilla this is a pointer to either raw or textual XPM
        //! image data.
        //!
        //! \sa SCI_CLEARREGISTEREDIMAGES, SCI_REGISTERRGBAIMAGE
        SCI_REGISTERIMAGE = 2405,

        //!
        SCI_SETPRINTWRAPMODE = 2406,

        //!
        SCI_GETPRINTWRAPMODE = 2407,

        //! This message de-registers all currently registered images.
        //!
        //! \sa SCI_REGISTERIMAGE, SCI_REGISTERRGBAIMAGE
        SCI_CLEARREGISTEREDIMAGES = 2408,

        //!
        SCI_STYLESETHOTSPOT = 2409,

        //!
        SCI_SETHOTSPOTACTIVEFORE = 2410,

        //!
        SCI_SETHOTSPOTACTIVEBACK = 2411,

        //!
        SCI_SETHOTSPOTACTIVEUNDERLINE = 2412,

        //!
        SCI_PARADOWN = 2413,

        //!
        SCI_PARADOWNEXTEND = 2414,

        //!
        SCI_PARAUP = 2415,

        //!
        SCI_PARAUPEXTEND = 2416,

        //!
        SCI_POSITIONBEFORE = 2417,

        //!
        SCI_POSITIONAFTER = 2418,

        //!
        SCI_COPYRANGE = 2419,

        //!
        SCI_COPYTEXT = 2420,

        //!
        SCI_SETHOTSPOTSINGLELINE = 2421,

        //!
        SCI_SETSELECTIONMODE = 2422,

        //!
        SCI_GETSELECTIONMODE = 2423,

        //!
        SCI_GETLINESELSTARTPOSITION = 2424,

        //!
        SCI_GETLINESELENDPOSITION = 2425,

        //!
        SCI_LINEDOWNRECTEXTEND = 2426,

        //!
        SCI_LINEUPRECTEXTEND = 2427,

        //!
        SCI_CHARLEFTRECTEXTEND = 2428,

        //!
        SCI_CHARRIGHTRECTEXTEND = 2429,

        //!
        SCI_HOMERECTEXTEND = 2430,

        //!
        SCI_VCHOMERECTEXTEND = 2431,

        //!
        SCI_LINEENDRECTEXTEND = 2432,

        //!
        SCI_PAGEUPRECTEXTEND = 2433,

        //!
        SCI_PAGEDOWNRECTEXTEND = 2434,

        //!
        SCI_STUTTEREDPAGEUP = 2435,

        //!
        SCI_STUTTEREDPAGEUPEXTEND = 2436,

        //!
        SCI_STUTTEREDPAGEDOWN = 2437,

        //!
        SCI_STUTTEREDPAGEDOWNEXTEND = 2438,

        //!
        SCI_WORDLEFTEND = 2439,

        //!
        SCI_WORDLEFTENDEXTEND = 2440,

        //!
        SCI_WORDRIGHTEND = 2441,

        //!
        SCI_WORDRIGHTENDEXTEND = 2442,

        //!
        SCI_SETWHITESPACECHARS = 2443,

        //!
        SCI_SETCHARSDEFAULT = 2444,

        //!
        SCI_AUTOCGETCURRENT = 2445,

        //!
        SCI_ALLOCATE = 2446,

        //!
        SCI_HOMEWRAP = 2349,

        //!
        SCI_HOMEWRAPEXTEND = 2450,

        //!
        SCI_LINEENDWRAP = 2451,

        //!
        SCI_LINEENDWRAPEXTEND = 2452,

        //!
        SCI_VCHOMEWRAP = 2453,

        //!
        SCI_VCHOMEWRAPEXTEND = 2454,

        //!
        SCI_LINECOPY = 2455,

        //!
        SCI_FINDCOLUMN = 2456,

        //!
        SCI_GETCARETSTICKY = 2457,

        //!
        SCI_SETCARETSTICKY = 2458,

        //!
        SCI_TOGGLECARETSTICKY = 2459,

        //!
        SCI_SETWRAPVISUALFLAGS = 2460,

        //!
        SCI_GETWRAPVISUALFLAGS = 2461,

        //!
        SCI_SETWRAPVISUALFLAGSLOCATION = 2462,

        //!
        SCI_GETWRAPVISUALFLAGSLOCATION = 2463,

        //!
        SCI_SETWRAPSTARTINDENT = 2464,

        //!
        SCI_GETWRAPSTARTINDENT = 2465,

        //!
        SCI_MARKERADDSET = 2466,

        //!
        SCI_SETPASTECONVERTENDINGS = 2467,

        //!
        SCI_GETPASTECONVERTENDINGS = 2468,

        //!
        SCI_SELECTIONDUPLICATE = 2469,

        //!
        SCI_SETCARETLINEBACKALPHA = 2470,

        //!
        SCI_GETCARETLINEBACKALPHA = 2471,

        //!
        SCI_SETWRAPINDENTMODE = 2472,

        //!
        SCI_GETWRAPINDENTMODE = 2473,

        //!
        SCI_MARKERSETALPHA = 2476,

        //!
        SCI_GETSELALPHA = 2477,

        //!
        SCI_SETSELALPHA = 2478,

        //!
        SCI_GETSELEOLFILLED = 2479,

        //!
        SCI_SETSELEOLFILLED = 2480,

        //!
        SCI_STYLEGETFORE = 2481,

        //!
        SCI_STYLEGETBACK = 2482,

        //!
        SCI_STYLEGETBOLD = 2483,

        //!
        SCI_STYLEGETITALIC = 2484,

        //!
        SCI_STYLEGETSIZE = 2485,

        //!
        SCI_STYLEGETFONT = 2486,

        //!
        SCI_STYLEGETEOLFILLED = 2487,

        //!
        SCI_STYLEGETUNDERLINE = 2488,

        //!
        SCI_STYLEGETCASE = 2489,

        //!
        SCI_STYLEGETCHARACTERSET = 2490,

        //!
        SCI_STYLEGETVISIBLE = 2491,

        //!
        SCI_STYLEGETCHANGEABLE = 2492,

        //!
        SCI_STYLEGETHOTSPOT = 2493,

        //!
        SCI_GETHOTSPOTACTIVEFORE = 2494,

        //!
        SCI_GETHOTSPOTACTIVEBACK = 2495,

        //!
        SCI_GETHOTSPOTACTIVEUNDERLINE = 2496,

        //!
        SCI_GETHOTSPOTSINGLELINE = 2497,

        //!
        SCI_BRACEHIGHLIGHTINDICATOR = 2498,

        //!
        SCI_BRACEBADLIGHTINDICATOR = 2499,

        //!
        SCI_SETINDICATORCURRENT = 2500,

        //!
        SCI_GETINDICATORCURRENT = 2501,

        //!
        SCI_SETINDICATORVALUE = 2502,

        //!
        SCI_GETINDICATORVALUE = 2503,

        //!
        SCI_INDICATORFILLRANGE = 2504,

        //!
        SCI_INDICATORCLEARRANGE = 2505,

        //!
        SCI_INDICATORALLONFOR = 2506,

        //!
        SCI_INDICATORVALUEAT = 2507,

        //!
        SCI_INDICATORSTART = 2508,

        //!
        SCI_INDICATOREND = 2509,

        //!
        SCI_INDICSETUNDER = 2510,

        //!
        SCI_INDICGETUNDER = 2511,

        //!
        SCI_SETCARETSTYLE = 2512,

        //!
        SCI_GETCARETSTYLE = 2513,

        //!
        SCI_SETPOSITIONCACHE = 2514,

        //!
        SCI_GETPOSITIONCACHE = 2515,

        //!
        SCI_SETSCROLLWIDTHTRACKING = 2516,

        //!
        SCI_GETSCROLLWIDTHTRACKING = 2517,

        //!
        SCI_DELWORDRIGHTEND = 2518,

        //! This message copies the selection.  If the selection is empty then
        //! copy the line with the caret.
        SCI_COPYALLOWLINE = 2519,

        //! This message returns a pointer to the document text.  Any
        //! subsequent message will invalidate the pointer.
        SCI_GETCHARACTERPOINTER = 2520,

        //!
        SCI_SETKEYSUNICODE = 2521,

        //!
        SCI_GETKEYSUNICODE = 2522,

        //!
        SCI_INDICSETALPHA = 2523,

        //!
        SCI_INDICGETALPHA = 2524,

        //!
        SCI_SETEXTRAASCENT = 2525,

        //!
        SCI_GETEXTRAASCENT = 2526,

        //!
        SCI_SETEXTRADESCENT = 2527,

        //!
        SCI_GETEXTRADESCENT = 2528,

        //!
        SCI_MARKERSYMBOLDEFINED = 2529,

        //!
        SCI_MARGINSETTEXT = 2530,

        //!
        SCI_MARGINGETTEXT = 2531,

        //!
        SCI_MARGINSETSTYLE = 2532,

        //!
        SCI_MARGINGETSTYLE = 2533,

        //!
        SCI_MARGINSETSTYLES = 2534,

        //!
        SCI_MARGINGETSTYLES = 2535,

        //!
        SCI_MARGINTEXTCLEARALL = 2536,

        //!
        SCI_MARGINSETSTYLEOFFSET = 2537,

        //!
        SCI_MARGINGETSTYLEOFFSET = 2538,

        //!
        SCI_SETMARGINOPTIONS = 2539,

        //!
        SCI_ANNOTATIONSETTEXT = 2540,

        //!
        SCI_ANNOTATIONGETTEXT = 2541,

        //!
        SCI_ANNOTATIONSETSTYLE = 2542,

        //!
        SCI_ANNOTATIONGETSTYLE = 2543,

        //!
        SCI_ANNOTATIONSETSTYLES = 2544,

        //!
        SCI_ANNOTATIONGETSTYLES = 2545,

        //!
        SCI_ANNOTATIONGETLINES = 2546,

        //!
        SCI_ANNOTATIONCLEARALL = 2547,

        //!
        SCI_ANNOTATIONSETVISIBLE = 2548,

        //!
        SCI_ANNOTATIONGETVISIBLE = 2549,

        //!
        SCI_ANNOTATIONSETSTYLEOFFSET = 2550,

        //!
        SCI_ANNOTATIONGETSTYLEOFFSET = 2551,

        //!
        SCI_RELEASEALLEXTENDEDSTYLES = 2552,

        //!
        SCI_ALLOCATEEXTENDEDSTYLES = 2553,

        //!
        SCI_SETEMPTYSELECTION = 2556,

        //!
        SCI_GETMARGINOPTIONS = 2557,

        //!
        SCI_INDICSETOUTLINEALPHA = 2558,

        //!
        SCI_INDICGETOUTLINEALPHA = 2559,

        //!
        SCI_ADDUNDOACTION = 2560,

        //!
        SCI_CHARPOSITIONFROMPOINT = 2561,

        //!
        SCI_CHARPOSITIONFROMPOINTCLOSE = 2562,

        //!
        SCI_SETMULTIPLESELECTION = 2563,

        //!
        SCI_GETMULTIPLESELECTION = 2564,

        //!
        SCI_SETADDITIONALSELECTIONTYPING = 2565,

        //!
        SCI_GETADDITIONALSELECTIONTYPING = 2566,

        //!
        SCI_SETADDITIONALCARETSBLINK = 2567,

        //!
        SCI_GETADDITIONALCARETSBLINK = 2568,

        //!
        SCI_SCROLLRANGE = 2569,

        //!
        SCI_GETSELECTIONS = 2570,

        //!
        SCI_CLEARSELECTIONS = 2571,

        //!
        SCI_SETSELECTION = 2572,

        //!
        SCI_ADDSELECTION = 2573,

        //!
        SCI_SETMAINSELECTION = 2574,

        //!
        SCI_GETMAINSELECTION = 2575,

        //!
        SCI_SETSELECTIONNCARET = 2576,

        //!
        SCI_GETSELECTIONNCARET = 2577,

        //!
        SCI_SETSELECTIONNANCHOR = 2578,

        //!
        SCI_GETSELECTIONNANCHOR = 2579,

        //!
        SCI_SETSELECTIONNCARETVIRTUALSPACE = 2580,

        //!
        SCI_GETSELECTIONNCARETVIRTUALSPACE = 2581,

        //!
        SCI_SETSELECTIONNANCHORVIRTUALSPACE = 2582,

        //!
        SCI_GETSELECTIONNANCHORVIRTUALSPACE = 2583,

        //!
        SCI_SETSELECTIONNSTART = 2584,

        //!
        SCI_GETSELECTIONNSTART = 2585,

        //!
        SCI_SETSELECTIONNEND = 2586,

        //!
        SCI_GETSELECTIONNEND = 2587,

        //!
        SCI_SETRECTANGULARSELECTIONCARET = 2588,

        //!
        SCI_GETRECTANGULARSELECTIONCARET = 2589,

        //!
        SCI_SETRECTANGULARSELECTIONANCHOR = 2590,

        //!
        SCI_GETRECTANGULARSELECTIONANCHOR = 2591,

        //!
        SCI_SETRECTANGULARSELECTIONCARETVIRTUALSPACE = 2592,

        //!
        SCI_GETRECTANGULARSELECTIONCARETVIRTUALSPACE = 2593,

        //!
        SCI_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE = 2594,

        //!
        SCI_GETRECTANGULARSELECTIONANCHORVIRTUALSPACE = 2595,

        //!
        SCI_SETVIRTUALSPACEOPTIONS = 2596,

        //!
        SCI_GETVIRTUALSPACEOPTIONS = 2597,

        //!
        SCI_SETRECTANGULARSELECTIONMODIFIER = 2598,

        //!
        SCI_GETRECTANGULARSELECTIONMODIFIER = 2599,

        //!
        SCI_SETADDITIONALSELFORE = 2600,

        //!
        SCI_SETADDITIONALSELBACK = 2601,

        //!
        SCI_SETADDITIONALSELALPHA = 2602,

        //!
        SCI_GETADDITIONALSELALPHA = 2603,

        //!
        SCI_SETADDITIONALCARETFORE = 2604,

        //!
        SCI_GETADDITIONALCARETFORE = 2605,

        //!
        SCI_ROTATESELECTION = 2606,

        //!
        SCI_SWAPMAINANCHORCARET = 2607,

        //!
        SCI_SETADDITIONALCARETSVISIBLE = 2608,

        //!
        SCI_GETADDITIONALCARETSVISIBLE = 2609,

        //!
        SCI_AUTOCGETCURRENTTEXT = 2610,

        //!
        SCI_SETFONTQUALITY = 2611,

        //!
        SCI_GETFONTQUALITY = 2612,

        //!
        SCI_SETFIRSTVISIBLELINE = 2613,

        //!
        SCI_SETMULTIPASTE = 2614,

        //!
        SCI_GETMULTIPASTE = 2615,

        //!
        SCI_GETTAG = 2616,

        //!
        SCI_CHANGELEXERSTATE = 2617,

        //!
        SCI_CONTRACTEDFOLDNEXT = 2618,

        //!
        SCI_VERTICALCENTRECARET = 2619,

        //!
        SCI_MOVESELECTEDLINESUP = 2620,

        //!
        SCI_MOVESELECTEDLINESDOWN = 2621,

        //!
        SCI_SETIDENTIFIER = 2622,

        //!
        SCI_GETIDENTIFIER = 2623,

        //! This message sets the width of an RGBA image specified by a future
        //! call to SCI_MARKERDEFINERGBAIMAGE or SCI_REGISTERRGBAIMAGE.
        //!
        //! \sa SCI_RGBAIMAGESETHEIGHT, SCI_MARKERDEFINERGBAIMAGE,
        //! SCI_REGISTERRGBAIMAGE.
        SCI_RGBAIMAGESETWIDTH = 2624,

        //! This message sets the height of an RGBA image specified by a future
        //! call to SCI_MARKERDEFINERGBAIMAGE or SCI_REGISTERRGBAIMAGE.
        //!
        //! \sa SCI_RGBAIMAGESETWIDTH, SCI_MARKERDEFINERGBAIMAGE,
        //! SCI_REGISTERRGBAIMAGE.
        SCI_RGBAIMAGESETHEIGHT = 2625,

        //! This message sets the symbol used to draw one of the 32 markers to
        //! an RGBA image.  RGBA images use the SC_MARK_RGBAIMAGE marker
        //! symbol.
        //! \a wParam is the number of the marker.
        //! \a lParam is a pointer to a QImage instance.  Note that in other
        //! ports of Scintilla this is a pointer to raw RGBA image data.
        //!
        //! \sa SCI_MARKERDEFINE, SCI_MARKERDEFINEPIXMAP
        SCI_MARKERDEFINERGBAIMAGE = 2626,

        //! This message takes a copy of an image and registers it so that it
        //! can be refered to by a unique integer identifier.
        //! \a wParam is the image's identifier.
        //! \a lParam is a pointer to a QImage instance.  Note that in other
        //! ports of Scintilla this is a pointer to raw RGBA image data.
        //!
        //! \sa SCI_CLEARREGISTEREDIMAGES, SCI_REGISTERIMAGE
        SCI_REGISTERRGBAIMAGE = 2627,

        //!
        SCI_SCROLLTOSTART = 2628,

        //!
        SCI_SCROLLTOEND = 2629,

        //!
        SCI_SETTECHNOLOGY = 2630,

        //!
        SCI_GETTECHNOLOGY = 2631,

        //!
        SCI_CREATELOADER = 2632,

        //!
        SCI_COUNTCHARACTERS = 2633,

        //!
        SCI_AUTOCSETCASEINSENSITIVEBEHAVIOUR = 2634,

        //!
        SCI_AUTOCGETCASEINSENSITIVEBEHAVIOUR = 2635,

        //!
        SCI_AUTOCSETMULTI = 2636,

        //!
        SCI_AUTOCGETMULTI = 2637,

        //!
        SCI_FINDINDICATORSHOW = 2640,

        //!
        SCI_FINDINDICATORFLASH = 2641,

        //!
        SCI_FINDINDICATORHIDE = 2642,

        //!
        SCI_GETRANGEPOINTER = 2643,

        //!
        SCI_GETGAPPOSITION = 2644,

        //!
        SCI_DELETERANGE = 2645,

        //!
        SCI_GETWORDCHARS = 2646,

        //!
        SCI_GETWHITESPACECHARS = 2647,

        //!
        SCI_SETPUNCTUATIONCHARS = 2648,

        //!
        SCI_GETPUNCTUATIONCHARS = 2649,

        //!
        SCI_GETSELECTIONEMPTY = 2650,

        //!
        SCI_RGBAIMAGESETSCALE = 2651,

        //!
        SCI_VCHOMEDISPLAY = 2652,

        //!
        SCI_VCHOMEDISPLAYEXTEND = 2653,

        //!
        SCI_GETCARETLINEVISIBLEALWAYS = 2654,

        //!
        SCI_SETCARETLINEVISIBLEALWAYS = 2655,

        //!
        SCI_SETLINEENDTYPESALLOWED = 2656,

        //!
        SCI_GETLINEENDTYPESALLOWED = 2657,

        //!
        SCI_GETLINEENDTYPESACTIVE = 2658,

        //!
        SCI_AUTOCSETORDER = 2660,

        //!
        SCI_AUTOCGETORDER = 2661,

        //!
        SCI_FOLDALL = 2662,

        //!
        SCI_SETAUTOMATICFOLD = 2663,

        //!
        SCI_GETAUTOMATICFOLD = 2664,

        //!
        SCI_SETREPRESENTATION = 2665,

        //!
        SCI_GETREPRESENTATION = 2666,

        //!
        SCI_CLEARREPRESENTATION = 2667,

        //!
        SCI_SETMOUSESELECTIONRECTANGULARSWITCH = 2668,

        //!
        SCI_GETMOUSESELECTIONRECTANGULARSWITCH = 2669,

        //!
        SCI_POSITIONRELATIVE = 2670,

        //!
        SCI_DROPSELECTIONN = 2671,

        //!
        SCI_CHANGEINSERTION = 2672,

        //!
        SCI_GETPHASESDRAW = 2673,

        //!
        SCI_SETPHASESDRAW = 2674,

        //!
        SCI_CLEARTABSTOPS = 2675,

        //!
        SCI_ADDTABSTOP = 2676,

        //!
        SCI_GETNEXTTABSTOP = 2677,

        //!
        SCI_GETIMEINTERACTION = 2678,

        //!
        SCI_SETIMEINTERACTION = 2679,

        //!
        SCI_INDICSETHOVERSTYLE = 2680,

        //!
        SCI_INDICGETHOVERSTYLE = 2681,

        //!
        SCI_INDICSETHOVERFORE = 2682,

        //!
        SCI_INDICGETHOVERFORE = 2683,

        //!
        SCI_INDICSETFLAGS = 2684,

        //!
        SCI_INDICGETFLAGS = 2685,

        //!
        SCI_SETTARGETRANGE = 2686,

        //!
        SCI_GETTARGETRANGE = 2687,

        //!
        SCI_STARTRECORD = 3001,

        //!
        SCI_STOPRECORD = 3002,

        //! This message sets the number of the lexer to use for syntax
        //! styling.
        //! \a wParam is the number of the lexer and is one of the SCLEX_*
        //! values.
        SCI_SETLEXER = 4001,

        //! This message returns the number of the lexer being used for syntax
        //! styling.
        SCI_GETLEXER = 4002,

        //!
        SCI_COLOURISE = 4003,

        //!
        SCI_SETPROPERTY = 4004,

        //!
        SCI_SETKEYWORDS = 4005,

        //! This message sets the name of the lexer to use for syntax styling.
        //! \a wParam is unused.
        //! \a lParam is the name of the lexer.
        SCI_SETLEXERLANGUAGE = 4006,

        //!
        SCI_LOADLEXERLIBRARY = 4007,

        //!
        SCI_GETPROPERTY = 4008,

        //!
        SCI_GETPROPERTYEXPANDED = 4009,

        //!
        SCI_GETPROPERTYINT = 4010,

        //!
        SCI_GETSTYLEBITSNEEDED = 4011,

        //!
        SCI_GETLEXERLANGUAGE = 4012,

        //!
        SCI_PRIVATELEXERCALL = 4013,

        //!
        SCI_PROPERTYNAMES = 4014,

        //!
        SCI_PROPERTYTYPE = 4015,

        //!
        SCI_DESCRIBEPROPERTY = 4016,

        //!
        SCI_DESCRIBEKEYWORDSETS = 4017,

        //!
        SCI_GETLINEENDTYPESSUPPORTED = 4018,

        //!
        SCI_ALLOCATESUBSTYLES = 4020,

        //!
        SCI_GETSUBSTYLESSTART = 4021,

        //!
        SCI_GETSUBSTYLESLENGTH = 4022,

        //!
        SCI_GETSTYLEFROMSUBSTYLE = 4027,

        //!
        SCI_GETPRIMARYSTYLEFROMSTYLE = 4028,

        //!
        SCI_FREESUBSTYLES = 4023,

        //!
        SCI_SETIDENTIFIERS = 4024,

        //!
        SCI_DISTANCETOSECONDARYSTYLES = 4025,

        //!
        SCI_GETSUBSTYLEBASES = 4026,
    };

    enum
    {
        SC_ALPHA_TRANSPARENT = 0,
        SC_ALPHA_OPAQUE = 255,
        SC_ALPHA_NOALPHA = 256
    };

    enum
    {
        SC_CARETSTICKY_OFF = 0,
        SC_CARETSTICKY_ON = 1,
        SC_CARETSTICKY_WHITESPACE = 2
    };

    enum
    {
        SC_EFF_QUALITY_MASK = 0x0f,
        SC_EFF_QUALITY_DEFAULT = 0,
        SC_EFF_QUALITY_NON_ANTIALIASED = 1,
        SC_EFF_QUALITY_ANTIALIASED = 2,
        SC_EFF_QUALITY_LCD_OPTIMIZED = 3
    };

    enum
    {
        SC_IME_WINDOWED = 0,
        SC_IME_INLINE = 1,
    };

    enum
    {
        SC_MARGINOPTION_NONE = 0x00,
        SC_MARGINOPTION_SUBLINESELECT = 0x01
    };

    enum
    {
        SC_MULTIAUTOC_ONCE = 0,
        SC_MULTIAUTOC_EACH = 1
    };

    enum
    {
        SC_MULTIPASTE_ONCE = 0,
        SC_MULTIPASTE_EACH = 1
    };

    //! This enum defines the different selection modes.
    //!
    //! \sa SCI_GETSELECTIONMODE, SCI_SETSELECTIONMODE
    enum
    {
        SC_SEL_STREAM = 0,
        SC_SEL_RECTANGLE = 1,
        SC_SEL_LINES = 2,
        SC_SEL_THIN = 3
    };

    enum
    {
        SC_STATUS_OK = 0,
        SC_STATUS_FAILURE = 1,
        SC_STATUS_BADALLOC = 2,
        SC_STATUS_WARN_START = 1000,
        SC_STATUS_WARNREGEX = 1001,
    };

    enum
    {
        SC_TYPE_BOOLEAN = 0,
        SC_TYPE_INTEGER = 1,
        SC_TYPE_STRING = 2
    };

    enum
    {
        SC_UPDATE_CONTENT = 0x01,
        SC_UPDATE_SELECTION = 0x02,
        SC_UPDATE_V_SCROLL = 0x04,
        SC_UPDATE_H_SCROLL = 0x08
    };

    enum
    {
        SC_WRAPVISUALFLAG_NONE = 0x0000,
        SC_WRAPVISUALFLAG_END = 0x0001,
        SC_WRAPVISUALFLAG_START = 0x0002,
        SC_WRAPVISUALFLAG_MARGIN = 0x0004
    };

    enum
    {
        SC_WRAPVISUALFLAGLOC_DEFAULT = 0x0000,
        SC_WRAPVISUALFLAGLOC_END_BY_TEXT = 0x0001,
        SC_WRAPVISUALFLAGLOC_START_BY_TEXT = 0x0002
    };

    enum
    {
        SCVS_NONE = 0,
        SCVS_RECTANGULARSELECTION = 1,
        SCVS_USERACCESSIBLE = 2
    };

    enum
    {
        SCWS_INVISIBLE = 0,
        SCWS_VISIBLEALWAYS = 1,
        SCWS_VISIBLEAFTERINDENT = 2
    };

    enum
    {
        SC_EOL_CRLF = 0,
        SC_EOL_CR = 1,
        SC_EOL_LF = 2
    };

    enum
    {
        SC_CP_DBCS = 1,
        SC_CP_UTF8 = 65001
    };

    //! This enum defines the different marker symbols.
    //!
    //! \sa SCI_MARKERDEFINE
    enum
    {
        //! A circle.
        SC_MARK_CIRCLE = 0,

        //! A rectangle.
        SC_MARK_ROUNDRECT = 1,

        //! A triangle pointing to the right.
        SC_MARK_ARROW = 2,

        //! A smaller rectangle.
        SC_MARK_SMALLRECT = 3,

        //! An arrow pointing to the right.
        SC_MARK_SHORTARROW = 4,

        //! An invisible marker that allows code to track the movement
        //! of lines.
        SC_MARK_EMPTY = 5,

        //! A triangle pointing down.
        SC_MARK_ARROWDOWN = 6,

        //! A drawn minus sign.
        SC_MARK_MINUS = 7,

        //! A drawn plus sign.
        SC_MARK_PLUS = 8,

        //! A vertical line drawn in the background colour.
        SC_MARK_VLINE = 9,

        //! A bottom left corner drawn in the background colour.
        SC_MARK_LCORNER = 10,

        //! A vertical line with a centre right horizontal line drawn
        //! in the background colour.
        SC_MARK_TCORNER = 11,

        //! A drawn plus sign in a box.
        SC_MARK_BOXPLUS = 12,

        //! A drawn plus sign in a connected box.
        SC_MARK_BOXPLUSCONNECTED = 13,

        //! A drawn minus sign in a box.
        SC_MARK_BOXMINUS = 14,

        //! A drawn minus sign in a connected box.
        SC_MARK_BOXMINUSCONNECTED = 15,

        //! A rounded bottom left corner drawn in the background
        //! colour.
        SC_MARK_LCORNERCURVE = 16,

        //! A vertical line with a centre right curved line drawn in
        //! the background colour.
        SC_MARK_TCORNERCURVE = 17,

        //! A drawn plus sign in a circle.
        SC_MARK_CIRCLEPLUS = 18,

        //! A drawn plus sign in a connected box.
        SC_MARK_CIRCLEPLUSCONNECTED = 19,

        //! A drawn minus sign in a circle.
        SC_MARK_CIRCLEMINUS = 20,

        //! A drawn minus sign in a connected circle.
        SC_MARK_CIRCLEMINUSCONNECTED = 21,

        //! No symbol is drawn but the line is drawn with the same background
        //! color as the marker's.
        SC_MARK_BACKGROUND = 22,

        //! Three drawn dots.
        SC_MARK_DOTDOTDOT = 23,

        //! Three drawn arrows pointing right.
        SC_MARK_ARROWS = 24,

        //! An XPM format pixmap.
        SC_MARK_PIXMAP = 25,

        //! A full rectangle (ie. the margin background) using the marker's
        //! background color.
        SC_MARK_FULLRECT = 26,

        //! A left rectangle (ie. the left part of the margin background) using
        //! the marker's background color.
        SC_MARK_LEFTRECT = 27,

        //! The value is available for plugins to use.
        SC_MARK_AVAILABLE = 28,

        //! The line is underlined using the marker's background color.
        SC_MARK_UNDERLINE = 29,

        //! A RGBA format image.
        SC_MARK_RGBAIMAGE = 30,

        //! A bookmark.
        SC_MARK_BOOKMARK = 31,

        //! Characters can be used as symbols by adding this to the ASCII value
        //! of the character.
        SC_MARK_CHARACTER = 10000
    };

    enum
    {
        SC_MARKNUM_FOLDEREND = 25,
        SC_MARKNUM_FOLDEROPENMID = 26,
        SC_MARKNUM_FOLDERMIDTAIL = 27,
        SC_MARKNUM_FOLDERTAIL = 28,
        SC_MARKNUM_FOLDERSUB = 29,
        SC_MARKNUM_FOLDER = 30,
        SC_MARKNUM_FOLDEROPEN = 31,
        SC_MASK_FOLDERS = 0xfe000000
    };

    //! This enum defines what can be displayed in a margin.
    //!
    //! \sa SCI_GETMARGINTYPEN, SCI_SETMARGINTYPEN
    enum
    {
        //! The margin can display symbols.  Note that all margins can display
        //! symbols.
        SC_MARGIN_SYMBOL = 0,

        //! The margin will display line numbers.
        SC_MARGIN_NUMBER = 1,

        //! The margin's background color will be set to the default background
        //! color.
        SC_MARGIN_BACK = 2,

        //! The margin's background color will be set to the default foreground
        //! color.
        SC_MARGIN_FORE = 3,

        //! The margin will display text.
        SC_MARGIN_TEXT = 4,

        //! The margin will display right justified text.
        SC_MARGIN_RTEXT = 5
    };

    enum
    {
        STYLE_DEFAULT = 32,
        STYLE_LINENUMBER = 33,
        STYLE_BRACELIGHT = 34,
        STYLE_BRACEBAD = 35,
        STYLE_CONTROLCHAR = 36,
        STYLE_INDENTGUIDE = 37,
        STYLE_CALLTIP = 38,
        STYLE_LASTPREDEFINED = 39,
        STYLE_MAX = 255
    };

    enum
    {
        SC_CHARSET_ANSI = 0,
        SC_CHARSET_DEFAULT = 1,
        SC_CHARSET_BALTIC = 186,
        SC_CHARSET_CHINESEBIG5 = 136,
        SC_CHARSET_EASTEUROPE = 238,
        SC_CHARSET_GB2312 = 134,
        SC_CHARSET_GREEK = 161,
        SC_CHARSET_HANGUL = 129,
        SC_CHARSET_MAC = 77,
        SC_CHARSET_OEM = 255,
        SC_CHARSET_RUSSIAN = 204,
        SC_CHARSET_SHIFTJIS = 128,
        SC_CHARSET_SYMBOL = 2,
        SC_CHARSET_TURKISH = 162,
        SC_CHARSET_JOHAB = 130,
        SC_CHARSET_HEBREW = 177,
        SC_CHARSET_ARABIC = 178,
        SC_CHARSET_VIETNAMESE = 163,
        SC_CHARSET_THAI = 222,
        SC_CHARSET_8859_15 = 1000
    };

    enum
    {
        SC_CASE_MIXED = 0,
        SC_CASE_UPPER = 1,
        SC_CASE_LOWER = 2
    };

    //! This enum defines the different indentation guide views.
    //!
    //! \sa SCI_GETINDENTATIONGUIDES, SCI_SETINDENTATIONGUIDES
    enum
    {
        //! No indentation guides are shown.
        SC_IV_NONE = 0,

        //! Indentation guides are shown inside real indentation white space.
        SC_IV_REAL = 1,

        //! Indentation guides are shown beyond the actual indentation up to
        //! the level of the next non-empty line.  If the previous non-empty
        //! line was a fold header then indentation guides are shown for one
        //! more level of indent than that line.  This setting is good for
        //! Python.
        SC_IV_LOOKFORWARD = 2,

        //! Indentation guides are shown beyond the actual indentation up to
        //! the level of the next non-empty line or previous non-empty line
        //! whichever is the greater.  This setting is good for most languages.
        SC_IV_LOOKBOTH = 3
    };

    enum
    {
        INDIC_PLAIN = 0,
        INDIC_SQUIGGLE = 1,
        INDIC_TT = 2,
        INDIC_DIAGONAL = 3,
        INDIC_STRIKE = 4,
        INDIC_HIDDEN = 5,
        INDIC_BOX = 6,
        INDIC_ROUNDBOX = 7,
        INDIC_STRAIGHTBOX = 8,
        INDIC_DASH = 9,
        INDIC_DOTS = 10,
        INDIC_SQUIGGLELOW = 11,
        INDIC_DOTBOX = 12,
        INDIC_SQUIGGLEPIXMAP = 13,
        INDIC_COMPOSITIONTHICK = 14,
        INDIC_COMPOSITIONTHIN = 15,
        INDIC_FULLBOX = 16,
        INDIC_TEXTFORE = 17,

        INDIC_IME = 32,
        INDIC_IME_MAX = 35,

        INDIC_CONTAINER = 8,
        INDIC_MAX = 35,
        INDIC0_MASK = 0x20,
        INDIC1_MASK = 0x40,
        INDIC2_MASK = 0x80,
        INDICS_MASK = 0xe0,

        SC_INDICVALUEBIT = 0x01000000,
        SC_INDICVALUEMASK = 0x00ffffff,
        SC_INDICFLAG_VALUEBEFORE = 1,
    };

    enum
    {
        SC_PRINT_NORMAL = 0,
        SC_PRINT_INVERTLIGHT = 1,
        SC_PRINT_BLACKONWHITE = 2,
        SC_PRINT_COLOURONWHITE = 3,
        SC_PRINT_COLOURONWHITEDEFAULTBG = 4
    };

    enum
    {
        SCFIND_WHOLEWORD = 2,
        SCFIND_MATCHCASE = 4,
        SCFIND_WORDSTART = 0x00100000,
        SCFIND_REGEXP = 0x00200000,
        SCFIND_POSIX = 0x00400000,
        SCFIND_CXX11REGEX = 0x00800000,
    };

    enum
    {
        SC_FOLDLEVELBASE = 0x00400,
        SC_FOLDLEVELWHITEFLAG = 0x01000,
        SC_FOLDLEVELHEADERFLAG = 0x02000,
        SC_FOLDLEVELNUMBERMASK = 0x00fff
    };

    enum
    {
        SC_FOLDFLAG_LINEBEFORE_EXPANDED = 0x0002,
        SC_FOLDFLAG_LINEBEFORE_CONTRACTED = 0x0004,
        SC_FOLDFLAG_LINEAFTER_EXPANDED = 0x0008,
        SC_FOLDFLAG_LINEAFTER_CONTRACTED = 0x0010,
        SC_FOLDFLAG_LEVELNUMBERS = 0x0040,
        SC_FOLDFLAG_LINESTATE = 0x0080,
    };

    enum
    {
        SC_LINE_END_TYPE_DEFAULT = 0,
        SC_LINE_END_TYPE_UNICODE = 1,
    };

    enum
    {
        SC_TIME_FOREVER = 10000000
    };

    enum
    {
        SC_WRAP_NONE = 0,
        SC_WRAP_WORD = 1,
        SC_WRAP_CHAR = 2,
        SC_WRAP_WHITESPACE = 3,
    };

    enum
    {
        SC_WRAPINDENT_FIXED = 0,
        SC_WRAPINDENT_SAME = 1,
        SC_WRAPINDENT_INDENT = 2
    };

    enum
    {
        SC_CACHE_NONE = 0,
        SC_CACHE_CARET = 1,
        SC_CACHE_PAGE = 2,
        SC_CACHE_DOCUMENT = 3
    };

    enum
    {
        SC_PHASES_ONE = 0,
        SC_PHASES_TWO = 1,
        SC_PHASES_MULTIPLE = 2,
    };

    enum
    {
        ANNOTATION_HIDDEN = 0,
        ANNOTATION_STANDARD = 1,
        ANNOTATION_BOXED = 2,
        ANNOTATION_INDENTED = 3,
    };

    enum
    {
        EDGE_NONE = 0,
        EDGE_LINE = 1,
        EDGE_BACKGROUND = 2
    };

    enum
    {
        SC_CURSORNORMAL = -1,
        SC_CURSORARROW = 2,
        SC_CURSORWAIT = 4,
        SC_CURSORREVERSEARROW = 7
    };

    enum
    {
        UNDO_MAY_COALESCE = 1
    };

    enum
    {
        VISIBLE_SLOP = 0x01,
        VISIBLE_STRICT = 0x04
    };

    enum
    {
        CARET_SLOP = 0x01,
        CARET_STRICT = 0x04,
        CARET_JUMPS = 0x10,
        CARET_EVEN = 0x08
    };

    enum
    {
        CARETSTYLE_INVISIBLE = 0,
        CARETSTYLE_LINE = 1,
        CARETSTYLE_BLOCK = 2
    };

    enum
    {
        SC_MOD_INSERTTEXT = 0x1,
        SC_MOD_DELETETEXT = 0x2,
        SC_MOD_CHANGESTYLE = 0x4,
        SC_MOD_CHANGEFOLD = 0x8,
        SC_PERFORMED_USER = 0x10,
        SC_PERFORMED_UNDO = 0x20,
        SC_PERFORMED_REDO = 0x40,
        SC_MULTISTEPUNDOREDO = 0x80,
        SC_LASTSTEPINUNDOREDO = 0x100,
        SC_MOD_CHANGEMARKER = 0x200,
        SC_MOD_BEFOREINSERT = 0x400,
        SC_MOD_BEFOREDELETE = 0x800,
        SC_MULTILINEUNDOREDO = 0x1000,
        SC_STARTACTION = 0x2000,
        SC_MOD_CHANGEINDICATOR = 0x4000,
        SC_MOD_CHANGELINESTATE = 0x8000,
        SC_MOD_CHANGEMARGIN = 0x10000,
        SC_MOD_CHANGEANNOTATION = 0x20000,
        SC_MOD_CONTAINER = 0x40000,
        SC_MOD_LEXERSTATE = 0x80000,
        SC_MOD_INSERTCHECK = 0x100000,
        SC_MOD_CHANGETABSTOPS = 0x200000,
        SC_MODEVENTMASKALL = 0x3fffff
    };

    enum
    {
        SCK_DOWN = 300,
        SCK_UP = 301,
        SCK_LEFT = 302,
        SCK_RIGHT = 303,
        SCK_HOME = 304,
        SCK_END = 305,
        SCK_PRIOR = 306,
        SCK_NEXT = 307,
        SCK_DELETE = 308,
        SCK_INSERT = 309,
        SCK_ESCAPE = 7,
        SCK_BACK = 8,
        SCK_TAB = 9,
        SCK_RETURN = 13,
        SCK_ADD = 310,
        SCK_SUBTRACT = 311,
        SCK_DIVIDE = 312,
        SCK_WIN = 313,
        SCK_RWIN = 314,
        SCK_MENU = 315
    };

    //! This enum defines the different modifier keys.
    enum
    {
        //! No modifier key.
        SCMOD_NORM = 0,

        //! Shift key.
        SCMOD_SHIFT = 1,

        //! Control key (the Command key on OS/X, the Ctrl key on other
        //! platforms).
        SCMOD_CTRL = 2,

        //! Alt key.
        SCMOD_ALT = 4,

        //! This is the same as SCMOD_META on all platforms.
        SCMOD_SUPER = 8,

        //! Meta key (the Ctrl key on OS/X, the Windows key on other
        //! platforms).
        SCMOD_META = 16
    };

    //! This enum defines the different language lexers.
    //!
    //! \sa SCI_GETLEXER, SCI_SETLEXER
    enum
    {
        //! No lexer is selected and the SCN_STYLENEEDED signal is emitted so
        //! that the application can style the text as needed.  This is the
        //! default.
        SCLEX_CONTAINER = 0,

        //! Select the null lexer that does no syntax styling.
        SCLEX_NULL = 1,

        //! Select the Python lexer.
        SCLEX_PYTHON = 2,

        //! Select the C++ lexer.
        SCLEX_CPP = 3,

        //! Select the HTML lexer.
        SCLEX_HTML = 4,

        //! Select the XML lexer.
        SCLEX_XML = 5,

        //! Select the Perl lexer.
        SCLEX_PERL = 6,

        //! Select the SQL lexer.
        SCLEX_SQL = 7,

        //! Select the Visual Basic lexer.
        SCLEX_VB = 8,

        //! Select the lexer for properties style files.
        SCLEX_PROPERTIES = 9,

        //! Select the lexer for error list style files.
        SCLEX_ERRORLIST = 10,

        //! Select the Makefile lexer.
        SCLEX_MAKEFILE = 11,

        //! Select the Windows batch file lexer.
        SCLEX_BATCH = 12,

        //! Select the LaTex lexer.
        SCLEX_LATEX = 14,

        //! Select the Lua lexer.
        SCLEX_LUA = 15,

        //! Select the lexer for diff output.
        SCLEX_DIFF = 16,

        //! Select the lexer for Apache configuration files.
        SCLEX_CONF = 17,

        //! Select the Pascal lexer.
        SCLEX_PASCAL = 18,

        //! Select the Avenue lexer.
        SCLEX_AVE = 19,

        //! Select the Ada lexer.
        SCLEX_ADA = 20,

        //! Select the Lisp lexer.
        SCLEX_LISP = 21,

        //! Select the Ruby lexer.
        SCLEX_RUBY = 22,

        //! Select the Eiffel lexer.
        SCLEX_EIFFEL = 23,

        //! Select the Eiffel lexer folding at keywords.
        SCLEX_EIFFELKW = 24,

        //! Select the Tcl lexer.
        SCLEX_TCL = 25,

        //! Select the lexer for nnCron files.
        SCLEX_NNCRONTAB = 26,

        //! Select the Bullant lexer.
        SCLEX_BULLANT = 27,

        //! Select the VBScript lexer.
        SCLEX_VBSCRIPT = 28,

        //! Select the ASP lexer.
        SCLEX_ASP = SCLEX_HTML,

        //! Select the PHP lexer.
        SCLEX_PHP = SCLEX_HTML,

        //! Select the Baan lexer.
        SCLEX_BAAN = 31,

        //! Select the Matlab lexer.
        SCLEX_MATLAB = 32,

        //! Select the Scriptol lexer.
        SCLEX_SCRIPTOL = 33,

        //! Select the assembler lexer (';' comment character).
        SCLEX_ASM = 34,

        //! Select the C++ lexer with case insensitive keywords.
        SCLEX_CPPNOCASE = 35,

        //! Select the FORTRAN lexer.
        SCLEX_FORTRAN = 36,

        //! Select the FORTRAN77 lexer.
        SCLEX_F77 = 37,

        //! Select the CSS lexer.
        SCLEX_CSS = 38,

        //! Select the POV lexer.
        SCLEX_POV = 39,

        //! Select the Basser Lout typesetting language lexer.
        SCLEX_LOUT = 40,

        //! Select the EScript lexer.
        SCLEX_ESCRIPT = 41,

        //! Select the PostScript lexer.
        SCLEX_PS = 42,

        //! Select the NSIS lexer.
        SCLEX_NSIS = 43,

        //! Select the MMIX assembly language lexer.
        SCLEX_MMIXAL = 44,

        //! Select the Clarion lexer.
        SCLEX_CLW = 45,

        //! Select the Clarion lexer with case insensitive keywords.
        SCLEX_CLWNOCASE = 46,

        //! Select the MPT text log file lexer.
        SCLEX_LOT = 47,

        //! Select the YAML lexer.
        SCLEX_YAML = 48,

        //! Select the TeX lexer.
        SCLEX_TEX = 49,

        //! Select the Metapost lexer.
        SCLEX_METAPOST = 50,

        //! Select the PowerBASIC lexer.
        SCLEX_POWERBASIC = 51,

        //! Select the Forth lexer.
        SCLEX_FORTH = 52,

        //! Select the Erlang lexer.
        SCLEX_ERLANG = 53,

        //! Select the Octave lexer.
        SCLEX_OCTAVE = 54,

        //! Select the MS SQL lexer.
        SCLEX_MSSQL = 55,

        //! Select the Verilog lexer.
        SCLEX_VERILOG = 56,

        //! Select the KIX-Scripts lexer.
        SCLEX_KIX = 57,

        //! Select the Gui4Cli lexer.
        SCLEX_GUI4CLI = 58,

        //! Select the Specman E lexer.
        SCLEX_SPECMAN = 59,

        //! Select the AutoIt3 lexer.
        SCLEX_AU3 = 60,

        //! Select the APDL lexer.
        SCLEX_APDL = 61,

        //! Select the Bash lexer.
        SCLEX_BASH = 62,

        //! Select the ASN.1 lexer.
        SCLEX_ASN1 = 63,

        //! Select the VHDL lexer.
        SCLEX_VHDL = 64,

        //! Select the Caml lexer.
        SCLEX_CAML = 65,

        //! Select the BlitzBasic lexer.
        SCLEX_BLITZBASIC = 66,

        //! Select the PureBasic lexer.
        SCLEX_PUREBASIC = 67,

        //! Select the Haskell lexer.
        SCLEX_HASKELL = 68,

        //! Select the PHPScript lexer.
        SCLEX_PHPSCRIPT = 69,

        //! Select the TADS3 lexer.
        SCLEX_TADS3 = 70,

        //! Select the REBOL lexer.
        SCLEX_REBOL = 71,

        //! Select the Smalltalk lexer.
        SCLEX_SMALLTALK = 72,

        //! Select the FlagShip lexer.
        SCLEX_FLAGSHIP = 73,

        //! Select the Csound lexer.
        SCLEX_CSOUND = 74,

        //! Select the FreeBasic lexer.
        SCLEX_FREEBASIC = 75,

        //! Select the InnoSetup lexer.
        SCLEX_INNOSETUP = 76,

        //! Select the Opal lexer.
        SCLEX_OPAL = 77,

        //! Select the Spice lexer.
        SCLEX_SPICE = 78,

        //! Select the D lexer.
        SCLEX_D = 79,

        //! Select the CMake lexer.
        SCLEX_CMAKE = 80,

        //! Select the GAP lexer.
        SCLEX_GAP = 81,

        //! Select the PLM lexer.
        SCLEX_PLM = 82,

        //! Select the Progress lexer.
        SCLEX_PROGRESS = 83,

        //! Select the Abaqus lexer.
        SCLEX_ABAQUS = 84,

        //! Select the Asymptote lexer.
        SCLEX_ASYMPTOTE = 85,

        //! Select the R lexer.
        SCLEX_R = 86,

        //! Select the MagikSF lexer.
        SCLEX_MAGIK = 87,

        //! Select the PowerShell lexer.
        SCLEX_POWERSHELL = 88,

        //! Select the MySQL lexer.
        SCLEX_MYSQL = 89,

        //! Select the gettext .po file lexer.
        SCLEX_PO = 90,

        //! Select the TAL lexer.
        SCLEX_TAL = 91,

        //! Select the COBOL lexer.
        SCLEX_COBOL = 92,

        //! Select the TACL lexer.
        SCLEX_TACL = 93,

        //! Select the Sorcus lexer.
        SCLEX_SORCUS = 94,

        //! Select the PowerPro lexer.
        SCLEX_POWERPRO = 95,

        //! Select the Nimrod lexer.
        SCLEX_NIMROD = 96,

        //! Select the SML lexer.
        SCLEX_SML = 97,

        //! Select the Markdown lexer.
        SCLEX_MARKDOWN = 98,

        //! Select the txt2tags lexer.
        SCLEX_TXT2TAGS = 99,

        //! Select the 68000 assembler lexer.
        SCLEX_A68K = 100,

        //! Select the Modula 3 lexer.
        SCLEX_MODULA = 101,

        //! Select the CoffeeScript lexer.
        SCLEX_COFFEESCRIPT = 102,

        //! Select the Take Command lexer.
        SCLEX_TCMD = 103,

        //! Select the AviSynth lexer.
        SCLEX_AVS = 104,

        //! Select the ECL lexer.
        SCLEX_ECL = 105,

        //! Select the OScript lexer.
        SCLEX_OSCRIPT = 106,

        //! Select the Visual Prolog lexer.
        SCLEX_VISUALPROLOG = 107,

        //! Select the Literal Haskell lexer.
        SCLEX_LITERATEHASKELL = 108,

        //! Select the Structured Text lexer.
        SCLEX_STTXT = 109,

        //! Select the KVIrc lexer.
        SCLEX_KVIRC = 110,

        //! Select the Rust lexer.
        SCLEX_RUST = 111,

        //! Select the MSC Nastran DMAP lexer.
        SCLEX_DMAP = 112,

        //! Select the assembler lexer ('#' comment character).
        SCLEX_AS = 113,

        //! Select the DMIS lexer.
        SCLEX_DMIS = 114,

        //! Select the lexer for Windows registry files.
        SCLEX_REGISTRY = 115,

        //! Select the BibTex lexer.
        SCLEX_BIBTEX = 116,

        //! Select the Motorola S-Record hex lexer.
        SCLEX_SREC = 117,

        //! Select the Intel hex lexer.
        SCLEX_IHEX = 118,

        //! Select the Tektronix extended hex lexer.
        SCLEX_TEHEX = 119,
    };

    enum
    {
        SC_WEIGHT_NORMAL = 400,
        SC_WEIGHT_SEMIBOLD = 600,
        SC_WEIGHT_BOLD = 700,
    };

    enum
    {
        SC_TECHNOLOGY_DEFAULT = 0,
        SC_TECHNOLOGY_DIRECTWRITE = 1,
        SC_TECHNOLOGY_DIRECTWRITERETAIN = 2,
        SC_TECHNOLOGY_DIRECTWRITEDC = 3,
    };

    enum
    {
        SC_CASEINSENSITIVEBEHAVIOUR_RESPECTCASE = 0,
        SC_CASEINSENSITIVEBEHAVIOUR_IGNORECASE = 1,
    };

    enum
    {
        SC_FONT_SIZE_MULTIPLIER = 100,
    };

    enum
    {
        SC_FOLDACTION_CONTRACT = 0,
        SC_FOLDACTION_EXPAND = 1,
        SC_FOLDACTION_TOGGLE = 2,
    };

    enum
    {
        SC_AUTOMATICFOLD_SHOW = 0x0001,
        SC_AUTOMATICFOLD_CLICK = 0x0002,
        SC_AUTOMATICFOLD_CHANGE = 0x0004,
    };

    enum
    {
        SC_ORDER_PRESORTED = 0,
        SC_ORDER_PERFORMSORT = 1,
        SC_ORDER_CUSTOM = 2,
    };

    //! Construct an empty QsciScintillaBase with parent \a parent.
    explicit QsciScintillaBase(QWidget *parent = 0);

    //! Destroys the QsciScintillaBase instance.
    virtual ~QsciScintillaBase();

    //! Returns a pointer to a QsciScintillaBase instance, or 0 if there isn't
    //! one.  This can be used by the higher level API to send messages that
    //! aren't associated with a particular instance.
    static QsciScintillaBase *pool();

    //! Send the Scintilla message \a msg with the optional parameters \a
    //! wParam and \a lParam.
    long SendScintilla(unsigned int msg, unsigned long wParam = 0,
            long lParam = 0) const;

    //! \overload
    long SendScintilla(unsigned int msg, unsigned long wParam,
            void *lParam) const;

    //! \overload
    long SendScintilla(unsigned int msg, unsigned long wParam,
            const char *lParam) const;

    //! \overload
    long SendScintilla(unsigned int msg, const char *lParam) const;

    //! \overload
    long SendScintilla(unsigned int msg, const char *wParam,
            const char *lParam) const;

    //! \overload
    long SendScintilla(unsigned int msg, long wParam) const;

    //! \overload
    long SendScintilla(unsigned int msg, int wParam) const;

    //! \overload
    long SendScintilla(unsigned int msg, long cpMin, long cpMax,
            char *lpstrText) const;

    //! \overload
    long SendScintilla(unsigned int msg, unsigned long wParam,
            const QColor &col) const;

    //! \overload
    long SendScintilla(unsigned int msg, const QColor &col) const;

    //! \overload
    long SendScintilla(unsigned int msg, unsigned long wParam, QPainter *hdc,
            const QRect &rc, long cpMin, long cpMax) const;

    //! \overload
    long SendScintilla(unsigned int msg, unsigned long wParam,
            const QPixmap &lParam) const;

    //! \overload
    long SendScintilla(unsigned int msg, unsigned long wParam,
            const QImage &lParam) const;

    //! Send the Scintilla message \a msg and return a pointer result.
    void *SendScintillaPtrResult(unsigned int msg) const;

    //! \internal
    static int commandKey(int qt_key, int &modifiers);

signals:
    //! This signal is emitted when text is selected or de-selected.
    //! \a yes is true if text has been selected and false if text has been
    //! deselected.
    void QSCN_SELCHANGED(bool yes);

    //! This signal is emitted when the user cancels an auto-completion list.
    //!
    //! \sa SCN_AUTOCSELECTION()
    void SCN_AUTOCCANCELLED();

    //! This signal is emitted when the user deletes a character when an
    //! auto-completion list is active.
    void SCN_AUTOCCHARDELETED();

    //! This signal is emitted when the user selects an item in an
    //! auto-completion list.  It is emitted before the selection is inserted.
    //! The insertion can be cancelled by sending an SCI_AUTOCANCEL message
    //! from a connected slot.
    //! \a selection is the text of the selection.
    //! \a position is the start position of the word being completed.
    //!
    //! \sa SCN_AUTOCCANCELLED()
    void SCN_AUTOCSELECTION(const char *selection, int position);

    //! This signal is emitted when the document has changed for any reason.
    void SCEN_CHANGE();

    //! This signal ir emitted when the user clicks on a calltip.
    //! \a direction is 1 if the user clicked on the up arrow, 2 if the user
    //! clicked on the down arrow, and 0 if the user clicked elsewhere.
    void SCN_CALLTIPCLICK(int direction);

    //! This signal is emitted whenever the user enters an ordinary character
    //! into the text.
    //! \a charadded is the character. It can be used to decide to display a
    //! call tip or an auto-completion list.
    void SCN_CHARADDED(int charadded);

    //! This signal is emitted when the user double clicks.
    //! \a position is the position in the text where the click occured.
    //! \a line is the number of the line in the text where the click occured.
    //! \a modifiers is the logical or of the modifier keys that were pressed
    //! when the user double clicked.
    void SCN_DOUBLECLICK(int position, int line, int modifiers);

    //!
    void SCN_DWELLEND(int, int, int);

    //!
    void SCN_DWELLSTART(int, int, int);

    //! This signal is emitted when focus is received.
    void SCN_FOCUSIN();

    //! This signal is emitted when focus is lost.
    void SCN_FOCUSOUT();

    //! This signal is emitted when the user clicks on text in a style with the
    //! hotspot attribute set.
    //! \a position is the position in the text where the click occured.
    //! \a modifiers is the logical or of the modifier keys that were pressed
    //! when the user clicked.
    void SCN_HOTSPOTCLICK(int position, int modifiers);

    //! This signal is emitted when the user double clicks on text in a style
    //! with the hotspot attribute set.
    //! \a position is the position in the text where the double click occured.
    //! \a modifiers is the logical or of the modifier keys that were pressed
    //! when the user double clicked.
    void SCN_HOTSPOTDOUBLECLICK(int position, int modifiers);

    //! This signal is emitted when the user releases the mouse button on text
    //! in a style with the hotspot attribute set.
    //! \a position is the position in the text where the release occured.
    //! \a modifiers is the logical or of the modifier keys that were pressed
    //! when the user released the button.
    void SCN_HOTSPOTRELEASECLICK(int position, int modifiers);

    //! This signal is emitted when the user clicks on text that has an
    //! indicator.
    //! \a position is the position in the text where the click occured.
    //! \a modifiers is the logical or of the modifier keys that were pressed
    //! when the user clicked.
    void SCN_INDICATORCLICK(int position, int modifiers);

    //! This signal is emitted when the user releases the mouse button on text
    //! that has an indicator.
    //! \a position is the position in the text where the release occured.
    //! \a modifiers is the logical or of the modifier keys that were pressed
    //! when the user released.
    void SCN_INDICATORRELEASE(int position, int modifiers);

    //! This signal is emitted when a recordable editor command has been
    //! executed.
    void SCN_MACRORECORD(unsigned int, unsigned long, void *);

    //! This signal is emitted when the user clicks on a sensitive margin.
    //! \a position is the position of the start of the line against which the
    //! user clicked.
    //! \a modifiers is the logical or of the modifier keys that were pressed
    //! when the user clicked.
    //! \a margin is the number of the margin the user clicked in: 0, 1 or 2.
    //! 
    //! \sa SCI_GETMARGINSENSITIVEN, SCI_SETMARGINSENSITIVEN
    void SCN_MARGINCLICK(int position, int modifiers, int margin);

    //!
    void SCN_MODIFIED(int, int, const char *, int, int, int, int, int, int, int);

    //! This signal is emitted when the user attempts to modify read-only
    //! text.
    void SCN_MODIFYATTEMPTRO();

    //!
    void SCN_NEEDSHOWN(int, int);

    //! This signal is emitted when painting has been completed.  It is useful
    //! to trigger some other change but to have the paint be done first to
    //! appear more reponsive to the user.
    void SCN_PAINTED();

    //! This signal is emitted when the current state of the text no longer
    //! corresponds to the state of the text at the save point.
    //! 
    //! \sa SCI_SETSAVEPOINT, SCN_SAVEPOINTREACHED()
    void SCN_SAVEPOINTLEFT();

    //! This signal is emitted when the current state of the text corresponds
    //! to the state of the text at the save point. This allows feedback to be
    //! given to the user as to whether the text has been modified since it was
    //! last saved.
    //! 
    //! \sa SCI_SETSAVEPOINT, SCN_SAVEPOINTLEFT()
    void SCN_SAVEPOINTREACHED();

    //! This signal is emitted when a range of text needs to be syntax styled.
    //! The range is from the value returned by the SCI_GETENDSTYLED message
    //! and \a position.  It is only emitted if the currently selected lexer is
    //! SCLEX_CONTAINER.
    //!
    //! \sa SCI_COLOURISE, SCI_GETENDSTYLED
    void SCN_STYLENEEDED(int position);

    //! This signal is emitted when either the text or styling of the text has
    //! changed or the selection range or scroll position has changed.
    //! \a updated contains the set of SC_UPDATE_* flags describing the changes
    //! since the signal was last emitted.
    void SCN_UPDATEUI(int updated);

    //!
    void SCN_USERLISTSELECTION(const char *, int);

    //!
    void SCN_ZOOM();

protected:
    //! Returns true if the contents of a MIME data object can be decoded and
    //! inserted into the document.  It is called during drag and paste
    //! operations.
    //! \a source is the MIME data object.
    //!
    //! \sa fromMimeData(), toMimeData()
    virtual bool canInsertFromMimeData(const QMimeData *source) const;

    //! Returns the text of a MIME data object.  It is called when a drag and
    //! drop is completed and when text is pasted from the clipboard.
    //! \a source is the MIME data object.  On return \a rectangular is set if
    //! the text corresponds to a rectangular selection.
    //!
    //! \sa canInsertFromMimeData(), toMimeData()
    virtual QByteArray fromMimeData(const QMimeData *source, bool &rectangular) const;

    //! Returns a new MIME data object containing some text and whether it
    //! corresponds to a rectangular selection.  It is called when a drag and
    //! drop is started and when the selection is copied to the clipboard.
    //! Ownership of the object is passed to the caller.  \a text is the text.
    //! \a rectangular is set if the text corresponds to a rectangular
    //! selection.
    //!
    //! \sa canInsertFromMimeData(), fromMimeData()
    virtual QMimeData *toMimeData(const QByteArray &text, bool rectangular) const;

    //! Re-implemented to handle the context menu.
    virtual void contextMenuEvent(QContextMenuEvent *e);

    //! Re-implemented to handle drag enters.
    virtual void dragEnterEvent(QDragEnterEvent *e);

    //! Re-implemented to handle drag leaves.
    virtual void dragLeaveEvent(QDragLeaveEvent *e);

    //! Re-implemented to handle drag moves.
    virtual void dragMoveEvent(QDragMoveEvent *e);

    //! Re-implemented to handle drops.
    virtual void dropEvent(QDropEvent *e);

    //! Re-implemented to tell Scintilla it has the focus.
    virtual void focusInEvent(QFocusEvent *e);

    //! Re-implemented to tell Scintilla it has lost the focus.
    virtual void focusOutEvent(QFocusEvent *e);

    //! Re-implemented to allow tabs to be entered as text.
    virtual bool focusNextPrevChild(bool next);

    //! Re-implemented to handle key presses.
    virtual void keyPressEvent(QKeyEvent *e);

    //! Re-implemented to handle composed characters.
    virtual void inputMethodEvent(QInputMethodEvent *event);
    virtual QVariant inputMethodQuery(Qt::InputMethodQuery query) const;

    //! Re-implemented to handle mouse double-clicks.
    virtual void mouseDoubleClickEvent(QMouseEvent *e);

    //! Re-implemented to handle mouse moves.
    virtual void mouseMoveEvent(QMouseEvent *e);

    //! Re-implemented to handle mouse presses.
    virtual void mousePressEvent(QMouseEvent *e);

    //! Re-implemented to handle mouse releases.
    virtual void mouseReleaseEvent(QMouseEvent *e);

    //! Re-implemented to paint the viewport.
    virtual void paintEvent(QPaintEvent *e);

    //! Re-implemented to handle resizes.
    virtual void resizeEvent(QResizeEvent *e);

    //! \internal Re-implemented to handle scrolling.
    virtual void scrollContentsBy(int dx, int dy);

    //! \internal This helps to work around some Scintilla bugs.
    void setScrollBars();

    //! \internal Qt4, Qt5 portability.
    typedef QByteArray ScintillaBytes;

#define ScintillaBytesConstData(b)  (b).constData()

    //! \internal Convert a QString to encoded bytes.
    ScintillaBytes textAsBytes(const QString &text) const;

    //! \internal Convert encoded bytes to a QString.
    QString bytesAsText(const char *bytes) const;

private slots:
    void handleVSb(int value);
    void handleHSb(int value);
    void handleSelection();

private:
    // This is needed to allow QsciScintillaQt to emit this class's signals.
    friend class QsciScintillaQt;

    QsciScintillaQt *sci;
    QPoint triple_click_at;
    QTimer triple_click;
    int preeditPos;
    int preeditNrBytes;
    QString preeditString;
#if QT_VERSION >= 0x050000
    bool clickCausedFocus;
#endif

    void acceptAction(QDropEvent *e);

    QsciScintillaBase(const QsciScintillaBase &);
    QsciScintillaBase &operator=(const QsciScintillaBase &);
};

#ifdef __APPLE__
}
#endif

#endif
