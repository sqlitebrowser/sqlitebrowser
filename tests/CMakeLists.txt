project(sqlb-unittests)
cmake_minimum_required(VERSION 2.8.9)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(ANTLR_DIR ../libs/antlr-2.7.7)
add_subdirectory(${ANTLR_DIR} ${CMAKE_CURRENT_BINARY_DIR}/antlr)

find_package(Qt4 COMPONENTS QtCore QtTest REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(SQLB_SRC
	../src/sqlitetypes.cpp
	../src/grammar/Sqlite3Lexer.cpp
	../src/grammar/Sqlite3Parser.cpp
	../src/tests/testsqlobjects.cpp)

set(SQLB_HDR
	../src/grammar/sqlite3TokenTypes.hpp
	../src/grammar/Sqlite3Lexer.hpp
	../src/grammar/Sqlite3Parser.hpp
	../src/sqlitetypes.h)

set(SQLB_MOC_HDR
	../src/tests/testsqlobjects.h)

QT4_WRAP_CPP(SQLB_MOC ${SQLB_MOC_HDR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/${ANTLR_DIR})

add_executable(${PROJECT_NAME} ${SQLB_MOC} ${SQLB_HDR} ${SQLB_SRC})
add_dependencies(${PROJECT_NAME} antlr)
target_link_libraries(${PROJECT_NAME} antlr ${QT_LIBRARIES})

MESSAGE(STATUS "Build type is ${CMAKE_BUILD_TYPE}")
