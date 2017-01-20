# QScintilla is a port to Qt of Neil Hodgson's Scintilla C++ editor control
# available at http://www.riverbankcomputing.com/software/qscintilla/
#
# The module defines the following variables:
#  QSCINTILLA_FOUND - the system has QScintilla
#  QSCINTILLA_INCLUDE_DIR - where to find qsciscintilla.h
#  QSCINTILLA_INCLUDE_DIRS - qscintilla includes
#  QSCINTILLA_LIBRARY - where to find the QScintilla library
#  QSCINTILLA_LIBRARIES - aditional libraries
#  QSCINTILLA_MAJOR_VERSION - major version
#  QSCINTILLA_MINOR_VERSION - minor version
#  QSCINTILLA_PATCH_VERSION - patch version
#  QSCINTILLA_VERSION_STRING - version (ex. 2.6.2)
#  QSCINTILLA_ROOT_DIR - root dir (ex. /usr/local)

#=============================================================================
# Copyright 2010-2013, Julien Schueller
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met: 
# 
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer. 
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution. 
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# The views and conclusions contained in the software and documentation are those
# of the authors and should not be interpreted as representing official policies, 
# either expressed or implied, of the FreeBSD Project.
#=============================================================================


find_path ( QSCINTILLA_INCLUDE_DIR
  NAMES qsciscintilla.h
  HINTS ${QT_INCLUDE_DIR}
  PATH_SUFFIXES Qsci
)

set ( QSCINTILLA_INCLUDE_DIRS ${QSCINTILLA_INCLUDE_DIR} )

# version
set ( _VERSION_FILE ${QSCINTILLA_INCLUDE_DIR}/Qsci/qsciglobal.h )
if ( EXISTS ${_VERSION_FILE} )
  file ( STRINGS ${_VERSION_FILE} _VERSION_LINE REGEX "define[ ]+QSCINTILLA_VERSION_STR" )
  if ( _VERSION_LINE )
    string ( REGEX REPLACE ".*define[ ]+QSCINTILLA_VERSION_STR[ ]+\"(.*)\".*" "\\1" QSCINTILLA_VERSION_STRING "${_VERSION_LINE}" )
    string ( REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" QSCINTILLA_MAJOR_VERSION "${QSCINTILLA_VERSION_STRING}" )
    string ( REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" QSCINTILLA_MINOR_VERSION "${QSCINTILLA_VERSION_STRING}" )
    string ( REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3" QSCINTILLA_PATCH_VERSION "${QSCINTILLA_VERSION_STRING}" )
  endif ()
endif ()


# check version
set ( _QSCINTILLA_VERSION_MATCH TRUE )
if ( QScintilla_FIND_VERSION AND QSCINTILLA_VERSION_STRING )
  if ( QScintilla_FIND_VERSION_EXACT )
    if ( NOT QScintilla_FIND_VERSION VERSION_EQUAL QSCINTILLA_VERSION_STRING )
      set ( _QSCINTILLA_VERSION_MATCH FALSE )
    endif ()
  else ()
    if ( QSCINTILLA_VERSION_STRING VERSION_LESS QScintilla_FIND_VERSION )
      set ( _QSCINTILLA_VERSION_MATCH FALSE )
    endif ()
  endif ()
endif ()


find_library ( QSCINTILLA_LIBRARY
  NAMES qscintilla qscintilla2 libqscintilla2
  HINTS ${QT_LIBRARY_DIR}
)

set ( QSCINTILLA_LIBRARIES ${QSCINTILLA_LIBRARY} )


# try to guess root dir from include dir
if ( QSCINTILLA_INCLUDE_DIR )
  string ( REGEX REPLACE "(.*)/include.*" "\\1" QSCINTILLA_ROOT_DIR ${QSCINTILLA_INCLUDE_DIR} )
# try to guess root dir from library dir
elseif ( QSCINTILLA_LIBRARY )
  string ( REGEX REPLACE "(.*)/lib[/|32|64].*" "\\1" QSCINTILLA_ROOT_DIR ${QSCINTILLA_LIBRARY} )
endif ()


# handle the QUIETLY and REQUIRED arguments
include ( FindPackageHandleStandardArgs )
if ( CMAKE_VERSION VERSION_LESS 2.8.3 )
  find_package_handle_standard_args( QScintilla DEFAULT_MSG QSCINTILLA_LIBRARY QSCINTILLA_INCLUDE_DIR _QSCINTILLA_VERSION_MATCH )
else ()
  find_package_handle_standard_args( QScintilla REQUIRED_VARS QSCINTILLA_LIBRARY QSCINTILLA_INCLUDE_DIR _QSCINTILLA_VERSION_MATCH VERSION_VAR QSCINTILLA_VERSION_STRING )
endif ()

mark_as_advanced (
  QSCINTILLA_LIBRARY
  QSCINTILLA_LIBRARIES
  QSCINTILLA_INCLUDE_DIR
  QSCINTILLA_INCLUDE_DIRS
  QSCINTILLA_MAJOR_VERSION
  QSCINTILLA_MINOR_VERSION
  QSCINTILLA_PATCH_VERSION
  QSCINTILLA_VERSION_STRING
  QSCINTILLA_ROOT_DIR
)
