// This module implements the QsciLexerCMake class.
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


#include "Qsci/qscilexercmake.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerCMake::QsciLexerCMake(QObject *parent)
    : QsciLexer(parent), fold_atelse(false)
{
}


// The dtor.
QsciLexerCMake::~QsciLexerCMake()
{
}


// Returns the language name.
const char *QsciLexerCMake::language() const
{
    return "CMake";
}


// Returns the lexer name.
const char *QsciLexerCMake::lexer() const
{
    return "cmake";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerCMake::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
    case KeywordSet3:
        return QColor(0x00,0x00,0x00);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case String:
    case StringLeftQuote:
    case StringRightQuote:
        return QColor(0x7f,0x00,0x7f);

    case Function:
    case BlockWhile:
    case BlockForeach:
    case BlockIf:
    case BlockMacro:
        return QColor(0x00,0x00,0x7f);

    case Variable:
        return QColor(0x80,0x00,0x00);

    case Label:
    case StringVariable:
        return QColor(0xcc,0x33,0x00);

    case Number:
        return QColor(0x00,0x7f,0x7f);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the font of the text for a style.
QFont QsciLexerCMake::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case Function:
    case BlockWhile:
    case BlockForeach:
    case BlockIf:
    case BlockMacro:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerCMake::keywords(int set) const
{
    if (set == 1)
        return
            "add_custom_command add_custom_target add_definitions "
            "add_dependencies add_executable add_library add_subdirectory "
            "add_test aux_source_directory build_command build_name "
            "cmake_minimum_required configure_file create_test_sourcelist "
            "else elseif enable_language enable_testing endforeach endif "
            "endmacro endwhile exec_program execute_process "
            "export_library_dependencies file find_file find_library "
            "find_package find_path find_program fltk_wrap_ui foreach "
            "get_cmake_property get_directory_property get_filename_component "
            "get_source_file_property get_target_property get_test_property "
            "if include include_directories include_external_msproject "
            "include_regular_expression install install_files "
            "install_programs install_targets link_directories link_libraries "
            "list load_cache load_command macro make_directory "
            "mark_as_advanced math message option output_required_files "
            "project qt_wrap_cpp qt_wrap_ui remove remove_definitions "
            "separate_arguments set set_directory_properties "
            "set_source_files_properties set_target_properties "
            "set_tests_properties site_name source_group string "
            "subdir_depends subdirs target_link_libraries try_compile try_run "
            "use_mangled_mesa utility_source variable_requires "
            "vtk_make_instantiator vtk_wrap_java vtk_wrap_python vtk_wrap_tcl "
            "while write_file";

    if (set == 2)
        return
            "ABSOLUTE ABSTRACT ADDITIONAL_MAKE_CLEAN_FILES ALL AND APPEND "
            "ARGS ASCII BEFORE CACHE CACHE_VARIABLES CLEAR COMMAND COMMANDS "
            "COMMAND_NAME COMMENT COMPARE COMPILE_FLAGS COPYONLY DEFINED "
            "DEFINE_SYMBOL DEPENDS DOC EQUAL ESCAPE_QUOTES EXCLUDE "
            "EXCLUDE_FROM_ALL EXISTS EXPORT_MACRO EXT EXTRA_INCLUDE "
            "FATAL_ERROR FILE FILES FORCE FUNCTION GENERATED GLOB "
            "GLOB_RECURSE GREATER GROUP_SIZE HEADER_FILE_ONLY HEADER_LOCATION "
            "IMMEDIATE INCLUDES INCLUDE_DIRECTORIES INCLUDE_INTERNALS "
            "INCLUDE_REGULAR_EXPRESSION LESS LINK_DIRECTORIES LINK_FLAGS "
            "LOCATION MACOSX_BUNDLE MACROS MAIN_DEPENDENCY MAKE_DIRECTORY "
            "MATCH MATCHALL MATCHES MODULE NAME NAME_WE NOT NOTEQUAL "
            "NO_SYSTEM_PATH OBJECT_DEPENDS OPTIONAL OR OUTPUT OUTPUT_VARIABLE "
            "PATH PATHS POST_BUILD POST_INSTALL_SCRIPT PREFIX PREORDER "
            "PRE_BUILD PRE_INSTALL_SCRIPT PRE_LINK PROGRAM PROGRAM_ARGS "
            "PROPERTIES QUIET RANGE READ REGEX REGULAR_EXPRESSION REPLACE "
            "REQUIRED RETURN_VALUE RUNTIME_DIRECTORY SEND_ERROR SHARED "
            "SOURCES STATIC STATUS STREQUAL STRGREATER STRLESS SUFFIX TARGET "
            "TOLOWER TOUPPER VAR VARIABLES VERSION WIN32 WRAP_EXCLUDE WRITE "
            "APPLE MINGW MSYS CYGWIN BORLAND WATCOM MSVC MSVC_IDE MSVC60 "
            "MSVC70 MSVC71 MSVC80 CMAKE_COMPILER_2005 OFF ON";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerCMake::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case String:
        return tr("String");

    case StringLeftQuote:
        return tr("Left quoted string");

    case StringRightQuote:
        return tr("Right quoted string");

    case Function:
        return tr("Function");

    case Variable:
        return tr("Variable");

    case Label:
        return tr("Label");

    case KeywordSet3:
        return tr("User defined");

    case BlockWhile:
        return tr("WHILE block");

    case BlockForeach:
        return tr("FOREACH block");

    case BlockIf:
        return tr("IF block");

    case BlockMacro:
        return tr("MACRO block");

    case StringVariable:
        return tr("Variable within a string");

    case Number:
        return tr("Number");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerCMake::defaultPaper(int style) const
{
    switch (style)
    {
    case String:
    case StringLeftQuote:
    case StringRightQuote:
    case StringVariable:
        return QColor(0xee,0xee,0xee);
    }

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerCMake::refreshProperties()
{
    setAtElseProp();
}


// Read properties from the settings.
bool QsciLexerCMake::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_atelse = qs.value(prefix + "foldatelse", false).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerCMake::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldatelse", fold_atelse);

    return rc;
}


// Return true if ELSE blocks can be folded.
bool QsciLexerCMake::foldAtElse() const
{
    return fold_atelse;
}


// Set if ELSE blocks can be folded.
void QsciLexerCMake::setFoldAtElse(bool fold)
{
    fold_atelse = fold;

    setAtElseProp();
}


// Set the "fold.at.else" property.
void QsciLexerCMake::setAtElseProp()
{
    emit propertyChanged("fold.at.else",(fold_atelse ? "1" : "0"));
}
