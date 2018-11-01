include_directories("${CMAKE_CURRENT_BINARY_DIR}" ..)

if(NOT WIN32)
	set(LPTHREAD pthread)
endif()

# test-sqlobjects

set(TESTSQLOBJECTS_SRC
    ../sqlitedb.cpp
    ../sqlitetablemodel.cpp
    ../RowLoader.cpp
    ../sql/sqlitetypes.cpp
    ../sql/Query.cpp
    ../csvparser.cpp
    ../grammar/Sqlite3Lexer.cpp
    ../grammar/Sqlite3Parser.cpp
    ../Settings.cpp
    testsqlobjects.cpp
    ../Data.cpp
    ../CipherSettings.cpp
    ../DotenvFormat.cpp
    ../CondFormat.cpp
)

set(TESTSQLOBJECTS_HDR
    ../grammar/sqlite3TokenTypes.hpp
    ../grammar/Sqlite3Lexer.hpp
    ../grammar/Sqlite3Parser.hpp
    ../sql/sqlitetypes.h
    ../sql/Query.h
    ../Data.h
)

set(TESTSQLOBJECTS_MOC_HDR
    ../sqlitedb.h
    ../sqlitetablemodel.h
    ../Settings.h
    testsqlobjects.h
    ../CipherSettings.h
    ../DotenvFormat.h
    ../CondFormat.h
)

if(sqlcipher)
    list(APPEND TESTSQLOBJECTS_SRC ../CipherDialog.cpp)
    list(APPEND TESTSQLOBJECTS_FORMS ../CipherDialog.ui)
    list(APPEND TESTSQLOBJECTS_MOC_HDR ../CipherDialog.h)
endif()

QT5_WRAP_UI(TESTSQLOBJECTS_FORM_HDR ${TESTSQLOBJECTS_FORMS})

add_executable(test-sqlobjects ${TESTSQLOBJECTS_MOC} ${TESTSQLOBJECTS_HDR} ${TESTSQLOBJECTS_SRC} ${TESTSQLOBJECTS_FORM_HDR})

find_package(Qt5 REQUIRED COMPONENTS Test Widgets Gui)
target_link_libraries(test-sqlobjects Qt5::Test Qt5::Widgets Qt5::Gui)

set(QT_LIBRARIES "")

if(NOT ANTLR2_FOUND)
    add_dependencies(test-sqlobjects antlr)
endif()
target_link_libraries(test-sqlobjects ${QT_LIBRARIES} ${LIBSQLITE})
if(ANTLR2_FOUND)
    target_link_libraries(test-sqlobjects ${ANTLR2_LIBRARIES})
else()
    target_link_libraries(test-sqlobjects antlr)
endif()
target_link_libraries(test-sqlobjects ${LPTHREAD})
add_test(test-sqlobjects test-sqlobjects)

# test-import

set(TESTIMPORT_SRC
    ../csvparser.cpp
    TestImport.cpp
)

set(TESTIMPORT_MOC_HDR
    TestImport.h
)

add_executable(test-import ${TESTIMPORT_MOC} ${TESTIMPORT_SRC})

find_package(Qt5 REQUIRED COMPONENTS Core)
target_link_libraries(test-import Qt5::Test Qt5::Core)

set(QT_LIBRARIES "")

target_link_libraries(test-import ${QT_LIBRARIES})
add_test(test-import test-import)

# test regex

set(TESTREGEX_SRC
    ../sqlitedb.cpp
    ../sqlitetablemodel.cpp
    ../RowLoader.cpp
    ../sql/sqlitetypes.cpp
    ../sql/Query.cpp
    ../grammar/Sqlite3Lexer.cpp
    ../grammar/Sqlite3Parser.cpp
    ../Settings.cpp
    TestRegex.cpp
    ../Data.cpp
    ../CipherSettings.cpp
    ../DotenvFormat.cpp
    ../CondFormat.cpp
)

set(TESTREGEX_HDR
    ../grammar/sqlite3TokenTypes.hpp
    ../grammar/Sqlite3Lexer.hpp
    ../grammar/Sqlite3Parser.hpp
    ../sql/sqlitetypes.h
    ../sql/Query.h
    ../Data.h
)

set(TESTREGEX_MOC_HDR
    ../sqlitedb.h
    ../sqlitetablemodel.h
    ../Settings.h
    TestRegex.h
    ../CipherSettings.h
    ../DotenvFormat.h
    ../CondFormat.h
)

if(sqlcipher)
    list(APPEND TESTREGEX_SRC ../CipherDialog.cpp)
    list(APPEND TESTREGEX_MOC_HDR ../CipherDialog.h)
endif()

add_executable(test-regex ${TESTREGEX_MOC} ${TESTREGEX_HDR} ${TESTREGEX_SRC})

target_link_libraries(test-regex Qt5::Test Qt5::Core Qt5::Gui Qt5::Widgets)

set(QT_LIBRARIES "")

if(NOT ANTLR2_FOUND)
    add_dependencies(test-regex antlr)
endif()
target_link_libraries(test-regex ${QT_LIBRARIES} ${LIBSQLITE})
if(ANTLR2_FOUND)
    target_link_libraries(test-regex ${ANTLR2_LIBRARIES})
else()
    target_link_libraries(test-regex antlr)
endif()
target_link_libraries(test-regex ${LPTHREAD})
add_test(test-regex test-regex)

# test cache

set(TESTCACHE_SRC
    TestRowCache.cpp
)

set(TESTCACHE_MOC_HDR
    TestRowCache.h
)

add_executable(test-cache ${TESTCACHE_MOC} ${TESTCACHE_SRC})

target_link_libraries(test-cache Qt5::Test Qt5::Core)

set(QT_LIBRARIES "")

target_link_libraries(test-cache ${QT_LIBRARIES})
add_test(test-cache test-cache)
