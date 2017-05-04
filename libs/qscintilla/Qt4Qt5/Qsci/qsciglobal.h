// This module defines various things common to all of the Scintilla Qt port.
//
// Copyright (c) 2017 Riverbank Computing Limited <info@riverbankcomputing.com>
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

#include <qglobal.h>


#define QSCINTILLA_VERSION      0x020a00
#define QSCINTILLA_VERSION_STR  "2.10"


// Define QSCINTILLA_MAKE_DLL to create a QScintilla shared library, or
// define QSCINTILLA_DLL to link against a QScintilla shared library, or define
// neither to either build or link against a static QScintilla library.
#if defined(QSCINTILLA_DLL)
#define QSCINTILLA_EXPORT       Q_DECL_IMPORT
#elif defined(QSCINTILLA_MAKE_DLL)
#define QSCINTILLA_EXPORT       Q_DECL_EXPORT
#else
#define QSCINTILLA_EXPORT
#endif


#if !defined(QT_BEGIN_NAMESPACE)
#define QT_BEGIN_NAMESPACE
#define QT_END_NAMESPACE
#endif

#endif
