// Support for building the Scintilla code in the Scintilla namespace using the
// -DSCI_NAMESPACE compiler flag.
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


#ifndef _SCINAMESPACE_H
#define _SCINAMESPACE_H

#ifdef SCI_NAMESPACE
#define QSCI_SCI_NAMESPACE(name)    Scintilla::name
#define QSCI_BEGIN_SCI_NAMESPACE    namespace Scintilla {
#define QSCI_END_SCI_NAMESPACE      };
#else
#define QSCI_SCI_NAMESPACE(name)    name
#define QSCI_BEGIN_SCI_NAMESPACE
#define QSCI_END_SCI_NAMESPACE
#endif

#endif
