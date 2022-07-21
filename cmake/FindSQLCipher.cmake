# - Try to find SQLCipher
# Once done this will define
#
#  SQLCIPHER_FOUND - system has SQLCipher
#  SQLCIPHER_INCLUDE_DIR - the SQLCipher include directory
#  SQLCIPHER_LIBRARIES - Link these to use SQLCipher
#  SQLCIPHER_DEFINITIONS - Compiler switches required for using SQLCipher
#  SQLCIPHER_VERSION - This is set to major.minor.revision (e.g. 3.4.1)
#
# Hints to find SQLCipher
#
#  Set SQLCIPHER_ROOT_DIR to the root directory of a SQLCipher installation
#
# The following variables may be set
#
#  SQLCIPHER_USE_OPENSSL - Set to ON/OFF to specify whether to search and use OpenSSL.
#                          Default is OFF.
#  SQLCIPHER_OPENSSL_USE_ZLIB - Set to ON/OFF to specify whether to search and use Zlib in OpenSSL
#                               Default is OFF.

# Redistribution and use is allowed according to the terms of the BSD license.

# Copyright (c) 2008, Gilles Caulier, <caulier.gilles@gmail.com>
# Copyright (c) 2014, Christian Dávid, <christian-david@web.de>
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

# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
if( NOT WIN32 )
  find_package(PkgConfig)

  pkg_check_modules(PC_SQLCIPHER QUIET sqlcipher)

  set(SQLCIPHER_DEFINITIONS ${PC_SQLCIPHER_CFLAGS_OTHER})
endif( NOT WIN32 )

find_path(SQLCIPHER_INCLUDE_DIR NAMES sqlcipher/sqlite3.h
  PATHS
  ${SQLCIPHER_ROOT_DIR}
  ${PC_SQLCIPHER_INCLUDEDIR}
  ${PC_SQLCIPHER_INCLUDE_DIRS}
  ${CMAKE_INCLUDE_PATH}
  PATH_SUFFIXES "include"
)

find_library(SQLCIPHER_LIBRARY NAMES sqlcipher
  PATHS
  ${PC_SQLCIPHER_LIBDIR}
  ${PC_SQLCIPHER_LIBRARY_DIRS}
  ${SQLCIPHER_ROOT_DIR}
  PATH_SUFFIXES "lib"
)

set(SQLCIPHER_LIBRARIES ${SQLCIPHER_LIBRARY})
set(SQLCIPHER_INCLUDE_DIRS ${SQLCIPHER_INCLUDE_DIR})

if (SQLCIPHER_USE_OPENSSL)
    find_package(OpenSSL REQUIRED COMPONENTS Crypto)
    if (SQLCIPHER_OPENSSL_USE_ZLIB)
        find_package(ZLIB REQUIRED)
        # Official FindOpenSSL.cmake does not support Zlib
        set_target_properties(OpenSSL::Crypto PROPERTIES INTERFACE_LINK_LIBRARIES ZLIB::ZLIB)
    endif()

    list(APPEND SQLCIPHER_LIBRARIES ${OPENSSL_LIBRARIES})
    list(APPEND SQLCIPHER_INCLUDE_DIRS ${OPENSSL_INCLUDE_DIRS})
endif()


include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(SQLCipher
    DEFAULT_MSG SQLCIPHER_INCLUDE_DIR SQLCIPHER_LIBRARY)

# show the SQLCIPHER_INCLUDE_DIR and SQLCIPHER_LIBRARIES variables only in the advanced view
mark_as_advanced(SQLCIPHER_INCLUDE_DIR SQLCIPHER_LIBRARY)

if (NOT TARGET SQLCipher::SQLCipher)
    add_library(SQLCipher::SQLCipher UNKNOWN IMPORTED)

    set_property(TARGET SQLCipher::SQLCipher PROPERTY INTERFACE_COMPILE_DEFINITIONS SQLITE_HAS_CODEC)
    set_property(TARGET SQLCipher::SQLCipher APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS "SQLITE_TEMPSTORE=2")
    set_target_properties(SQLCipher::SQLCipher PROPERTIES
                          INTERFACE_INCLUDE_DIRECTORIES "${SQLCIPHER_INCLUDE_DIRS}"
                          IMPORTED_INTERFACE_LINK_LANGUAGES "C"
                          IMPORTED_LOCATION "${SQLCIPHER_LIBRARY}")

    if (SQLCIPHER_USE_OPENSSL)
        set_target_properties(SQLCipher::SQLCipher PROPERTIES
                              INTERFACE_LINK_LIBRARIES OpenSSL::Crypto)
        set_property(TARGET SQLCipher::SQLCipher APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS "SQLCIPHER_CRYPTO_OPENSSL")
    endif()
endif()
