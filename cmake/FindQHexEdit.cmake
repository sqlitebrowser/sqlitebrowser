# Attempt to locate QHexEdit
# Once done this will define:
#
#  QHEXEDIT_FOUND - system has QHexEdit
#  QHEXEDIT_INCLUDE_DIRS - the include directories for QHexEdit
#  QHEXEDIT_LIBRARIES - Link these to use QHexEdit
#
# Copyright (C) 2019, Scott Furry, <scott.wl.furry@gmail.com>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


find_library(QHexEdit_LIBRARY NAMES qhexedit qhexedit-qt5)
set(QHexEdit_LIBRARIES "${QHexEdit_LIBRARY}")

find_path(QHexEdit_INCLUDE_DIR qhexedit.h PATH_SUFFIXES qhexedit2)
set(QHexEdit_INCLUDE_DIRS "${QHexEdit_INCLUDE_DIR}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    QHexEdit
    DEFAULT_MSG
    QHexEdit_LIBRARIES
    QHexEdit_INCLUDE_DIRS
)

mark_as_advanced(
  QHexEdit_INCLUDE_DIRS
  QHexEdit_LIBRARIES
)

if (QHexEdit_FOUND AND NOT TARGET QHexEdit::QHexEdit)
  add_library(QHexEdit::QHexEdit UNKNOWN IMPORTED)
  set_target_properties(QHexEdit::QHexEdit PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${QHexEdit_INCLUDE_DIRS}
    IMPORTED_LOCATION ${QHexEdit_LIBRARIES}
  )
endif()