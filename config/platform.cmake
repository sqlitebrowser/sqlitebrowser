if(WIN32)
    include(${CMAKE_CURRENT_LIST_DIR}/platform_win.cmake)
    add_definitions(-DCHECKNEWVERSION)
elseif(APPLE)
    include(${CMAKE_CURRENT_LIST_DIR}/platform_apple.cmake)
    add_definitions(-DCHECKNEWVERSION)
endif()

if(NOT WIN32)
    list(APPEND PLATFORM_LIBS pthread)
endif()

if(UNIX AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "OpenBSD")
    list(APPEND PLATFORM_LIBS dl)
endif()

# add extra library path for MacOS and FreeBSD
set(EXTRAPATH APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
if(EXTRAPATH)
    list(PREPEND CMAKE_PREFIX_PATH /usr/local/opt/sqlite/lib)
    list(PREPEND CMAKE_PREFIX_PATH /usr/local/opt/sqlitefts5/lib)
endif()
