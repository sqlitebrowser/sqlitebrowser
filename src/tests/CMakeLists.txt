include_directories("${CMAKE_CURRENT_BINARY_DIR}" ..)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(${QT_MAJOR} REQUIRED COMPONENTS Concurrent PrintSupport Test Widgets)
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

# test edit table dialog

set(TESTEDITTABLE_SRC
    TestEditTableDialog.cpp
    ../EditTableDialog.cpp
    ../ForeignKeyEditorDelegate.cpp
    ../SelectItemsPopup.cpp
    ../sqlitedb.cpp
    ../sqlitetablemodel.cpp
    ../RowLoader.cpp
    ../Settings.cpp
    ../CondFormat.cpp
    ../Data.cpp
    ../CipherSettings.cpp
    ../sqltextedit.cpp
    ../SqlUiLexer.cpp
    ../ExtendedScintilla.cpp
    ../FindReplaceDialog.cpp
    ../sql/sqlitetypes.cpp
    ../sql/Query.cpp
    ../sql/ObjectIdentifier.cpp
    ../sql/parser/ParserDriver.cpp
    ../sql/parser/sqlite3_lexer.cpp
    ../sql/parser/sqlite3_parser.cpp
)

set(TESTEDITTABLE_HDR
    TestEditTableDialog.h
    ../EditTableDialog.h
    ../ForeignKeyEditorDelegate.h
    ../SelectItemsPopup.h
    ../sqlitedb.h
    ../sqlitetablemodel.h
    ../RowLoader.h
    ../Settings.h
    ../CondFormat.h
    ../Data.h
)

add_executable(test-edit-table ${TESTEDITTABLE_HDR} ${TESTEDITTABLE_SRC})
target_include_directories(test-edit-table PRIVATE "${PROJECT_BINARY_DIR}")
target_link_libraries(test-edit-table ${QT_MAJOR}::Test ${QT_MAJOR}::Widgets ${QT_MAJOR}::Concurrent ${QT_MAJOR}::PrintSupport QScintilla::QScintilla ${LIBSQLITE_NAME} ${QT5_COMPAT})
add_test(NAME test-edit-table COMMAND $<TARGET_FILE:test-edit-table>)
