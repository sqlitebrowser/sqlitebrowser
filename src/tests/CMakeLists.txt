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
add_test(test-sqlobjects test-sqlobjects)

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
add_test(test-import test-import)

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
add_test(test-regex test-regex)

# test cache

set(TESTCACHE_SRC
    TestRowCache.cpp
)

set(TESTCACHE_HDR
    TestRowCache.h
)

add_executable(test-cache ${TESTCACHE_HDR} ${TESTCACHE_SRC})
target_link_libraries(test-cache ${QT_MAJOR}::Test)
add_test(test-cache test-cache)
