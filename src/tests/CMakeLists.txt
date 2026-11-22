include_directories("${CMAKE_CURRENT_BINARY_DIR}" ..)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(${QT_MAJOR} REQUIRED COMPONENTS Test Widgets)
if(QT_MAJOR STREQUAL "Qt6")
    find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
    set(QT5_COMPAT Qt6::Core5Compat)
endif()

# test-sqlobjects

set(TESTSQLOBJECTS_SRC
    ../sql/sqlitetypes.cpp
    ../sql/Query.cpp
    ../sql/ObjectIdentifier.cpp
    testsqlobjects.cpp
    ../sql/parser/ParserDriver.cpp
    ../sql/parser/sqlite3_lexer.cpp
    ../sql/parser/sqlite3_parser.cpp
)

set(TESTSQLOBJECTS_HDR
    ../sql/sqlitetypes.h
    ../sql/Query.h
    ../sql/ObjectIdentifier.h
    ../sql/parser/ParserDriver.h
    ../sql/parser/sqlite3_lexer.h
    ../sql/parser/sqlite3_location.h
    ../sql/parser/sqlite3_parser.hpp
    testsqlobjects.h
)

add_executable(test-sqlobjects ${TESTSQLOBJECTS_HDR} ${TESTSQLOBJECTS_SRC})
target_link_libraries(test-sqlobjects ${QT_MAJOR}::Test)
add_test(NAME test-sqlobjects COMMAND $<TARGET_FILE:test-sqlobjects>)

# test-import

set(TESTIMPORT_SRC
    ../csvparser.cpp
    TestImport.cpp
)

set(TESTIMPORT_MOC_HDR
    TestImport.h
)

add_executable(test-import ${TESTIMPORT_MOC_HDR} ${TESTIMPORT_SRC})
target_link_libraries(test-import ${QT_MAJOR}::Test)
add_test(NAME test-import COMMAND $<TARGET_FILE:test-import>)

# test regex

set(TESTREGEX_SRC
    TestRegex.cpp
    ../Data.cpp
)

set(TESTREGEX_HDR
    ../Data.h
    TestRegex.h
)

add_executable(test-regex ${TESTREGEX_HDR} ${TESTREGEX_SRC})
target_link_libraries(test-regex ${QT_MAJOR}::Test ${QT_MAJOR}::Widgets ${QT5_COMPAT})
add_test(NAME test-regex COMMAND $<TARGET_FILE:test-regex>)

# test global filter

set(TESTGLOBALFILTER_SRC
    TestGlobalFilter.cpp
    GlobalFilterTestSupport.cpp
    ../GlobalFilter.cpp
    ../CondFormat.cpp
    ../Data.cpp
    ../Settings.cpp
    ../sql/Query.cpp
    ../sql/ObjectIdentifier.cpp
)

set(TESTGLOBALFILTER_HDR
    TestGlobalFilter.h
    ../GlobalFilter.h
    ../CondFormat.h
    ../Data.h
    ../Settings.h
    ../sql/Query.h
    ../sql/ObjectIdentifier.h
)

add_executable(test-global-filter ${TESTGLOBALFILTER_HDR} ${TESTGLOBALFILTER_SRC})
target_link_libraries(test-global-filter ${QT_MAJOR}::Test ${QT_MAJOR}::Widgets ${LIBSQLITE_NAME} ${QT5_COMPAT})
add_test(NAME test-global-filter COMMAND $<TARGET_FILE:test-global-filter>)
if(WIN32)
    set(_gf_extra_paths "")
    foreach(_p IN LISTS CMAKE_PREFIX_PATH)
        list(APPEND _gf_extra_paths "${_p}" "${_p}/bin")
        # OpenSSL 3 Universal installer layout: bin/x86, bin/x64
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            list(APPEND _gf_extra_paths "${_p}/bin/x64")
        else()
            list(APPEND _gf_extra_paths "${_p}/bin/x86")
        endif()
    endforeach()
    # SQLite and SQLCipher are built as DLLs in the CMake prefix by the Windows CI jobs.
    string(REPLACE ";" "\\;" TESTGLOBALFILTER_PATH "${_gf_extra_paths};$ENV{PATH}")
    set_tests_properties(test-global-filter PROPERTIES ENVIRONMENT "PATH=${TESTGLOBALFILTER_PATH}")
endif()

# test cache

set(TESTCACHE_SRC
    TestRowCache.cpp
)

set(TESTCACHE_HDR
    TestRowCache.h
)

add_executable(test-cache ${TESTCACHE_HDR} ${TESTCACHE_SRC})
target_link_libraries(test-cache ${QT_MAJOR}::Test)
add_test(NAME test-cache COMMAND $<TARGET_FILE:test-cache>)

# test sql editor

set(TESTSQLEDITOR_SRC
    TestSqlEditor.cpp
    SqlEditorTestSupport.cpp
    ../sqltextedit.cpp
    ../docktextedit.cpp
    ../ExtendedScintilla.cpp
    ../FindReplaceDialog.cpp
    ../SqlUiLexer.cpp
    ../Settings.cpp
    ../sql/sqlitetypes.cpp
    ../sql/ObjectIdentifier.cpp
    ../sql/Query.cpp
    ../sql/parser/ParserDriver.cpp
    ../sql/parser/sqlite3_lexer.cpp
    ../sql/parser/sqlite3_parser.cpp
)

set(TESTSQLEDITOR_HDR
    TestSqlEditor.h
    ../sqltextedit.h
    ../docktextedit.h
    ../ExtendedScintilla.h
    ../FindReplaceDialog.h
    ../SqlUiLexer.h
    ../Settings.h
    ../sql/sqlitetypes.h
    ../sql/Query.h
    ../sql/ObjectIdentifier.h
    ../sql/parser/ParserDriver.h
    ../sql/parser/sqlite3_lexer.h
    ../sql/parser/sqlite3_location.h
    ../sql/parser/sqlite3_parser.hpp
)

add_executable(test-sql-editor ${TESTSQLEDITOR_HDR} ${TESTSQLEDITOR_SRC} ../FindReplaceDialog.ui)
if(QT_MAJOR STREQUAL "Qt6")
    set_target_properties(test-sql-editor PROPERTIES AUTOUIC_OPTIONS "--connections=string")
endif()
target_link_libraries(test-sql-editor ${QT_MAJOR}::Test ${QT_MAJOR}::Widgets ${QT_MAJOR}::PrintSupport QScintilla::QScintilla ${LIBSQLITE_NAME} ${QT5_COMPAT})
add_test(NAME test-sql-editor COMMAND $<TARGET_FILE:test-sql-editor>)
if(WIN32)
    set(_se_extra_paths "")
    foreach(_p IN LISTS CMAKE_PREFIX_PATH)
        list(APPEND _se_extra_paths "${_p}" "${_p}/bin")
        # OpenSSL 3 Universal installer layout: bin/x86, bin/x64
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            list(APPEND _se_extra_paths "${_p}/bin/x64")
        else()
            list(APPEND _se_extra_paths "${_p}/bin/x86")
        endif()
    endforeach()
    # SQLite and SQLCipher are built as DLLs in the CMake prefix by the Windows CI jobs.
    string(REPLACE ";" "\\;" TESTSQLEDITOR_PATH "${_se_extra_paths};$ENV{PATH}")
    set_tests_properties(test-sql-editor PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen;PATH=${TESTSQLEDITOR_PATH}")
else()
    set_tests_properties(test-sql-editor PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endif()
