// Scintilla source code edit control
/** @file FontQuality.h
 ** Definitions to control font anti-aliasing.
 ** Redefine constants from Scintilla.h to avoid including Scintilla.h in PlatWin.cxx.
 **/
// Copyright 1998-2009 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef FONTQUALITY_H
#define FONTQUALITY_H

namespace Scintilla {

// These definitions match Scintilla.h
#define SC_EFF_QUALITY_MASK            0xF
#define SC_EFF_QUALITY_DEFAULT           0
#define SC_EFF_QUALITY_NON_ANTIALIASED   1
#define SC_EFF_QUALITY_ANTIALIASED       2
#define SC_EFF_QUALITY_LCD_OPTIMIZED     3

// These definitions must match SC_TECHNOLOGY_* in Scintilla.h
#define SCWIN_TECH_GDI 0
#define SCWIN_TECH_DIRECTWRITE 1

}

#endif
