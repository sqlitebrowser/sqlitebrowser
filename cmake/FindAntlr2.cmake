# - try to find Antlr v2
# Once done this will define:
#
#  ANTLR2_FOUND - system has antlr2
#  ANTLR2_INCLUDE_DIRS - the include directories for antlr2
#  ANTLR2_LIBRARIES - Link these to use antl2
#  ANTLR2_EXECUTABLE - The 'antlr' or 'runantlr' executable

# Copyright (C) 2015, Pino Toscano, <toscano.pino@tiscali.it>
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


find_library(ANTLR2_LIBRARY antlr)
set(ANTLR2_LIBRARIES "${ANTLR2_LIBRARY}")

find_path(ANTLR2_INCLUDE_DIR antlr/AST.hpp)
set(ANTLR2_INCLUDE_DIRS "${ANTLR2_INCLUDE_DIR}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Antlr2 DEFAULT_MSG ANTLR2_LIBRARIES ANTLR2_INCLUDE_DIRS)

find_program(ANTLR2_EXECUTABLE NAMES runantlr runantlr2 antlr)

mark_as_advanced(
  ANTLR2_INCLUDE_DIRS
  ANTLR2_LIBRARIES
  ANTLR2_EXECUTABLE
)
