// This module defines various things common to all of the Scintilla Qt port.
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


#ifndef QSCIGLOBAL_H
#define QSCIGLOBAL_H

#ifdef __APPLE__
extern "C++" {
#endif

#include <qglobal.h>


#define QSCINTILLA_VERSION      0x020900
#define QSCINTILLA_VERSION_STR  "2.9"


// Under Windows, define QSCINTILLA_MAKE_DLL to create a Scintilla DLL, or
// define QSCINTILLA_DLL to link against a Scintilla DLL, or define neither
// to either build or link against a static Scintilla library.
#if defined(Q_OS_WIN)

#if defined(QSCINTILLA_DLL)
#define QSCINTILLA_EXPORT       __declspec(dllimport)
#elif defined(QSCINTILLA_MAKE_DLL)
#define QSCINTILLA_EXPORT       __declspec(dllexport)
#endif

#endif

#if !defined(QSCINTILLA_EXPORT)
#define QSCINTILLA_EXPORT
#endif


#if !defined(QT_BEGIN_NAMESPACE)
#define QT_BEGIN_NAMESPACE
#define QT_END_NAMESPACE
#endif

#ifdef __APPLE__
}
#endif

#endif
