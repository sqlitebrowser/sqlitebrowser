if(MSVC)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # Paths for 64-bit windows builds
        set(OPENSSL_PATH "C:/dev/OpenSSL-Win64" CACHE PATH "OpenSSL Path")
        set(QT5_PATH "C:/dev/Qt/5.12.12/msvc2017_64" CACHE PATH "Qt5 Path")

        # Choose between SQLCipher or SQLite, depending whether
        # -Dsqlcipher=on is passed on the command line
        if(sqlcipher)
            set(SQLITE3_PATH "C:/git_repos/SQLCipher-Win64" CACHE PATH "SQLCipher Path")
        else()
            set(SQLITE3_PATH "C:/dev/SQLite-Win64" CACHE PATH "SQLite Path")
        endif()

        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS,5.02 /ENTRY:mainCRTStartup")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS,5.02")
    else()
        # Paths for 32-bit windows builds
        set(OPENSSL_PATH "C:/dev/OpenSSL-Win32" CACHE PATH "OpenSSL Path")
        set(QT5_PATH "C:/dev/Qt/5.12.12/msvc2017" CACHE PATH "Qt5 Path")

        # Choose between SQLCipher or SQLite, depending whether
        # -Dsqlcipher=on is passed on the command line
        if(sqlcipher)
            set(SQLITE3_PATH "C:/git_repos/SQLCipher-Win32" CACHE PATH "SQLCipher Path")
        else()
            set(SQLITE3_PATH "C:/dev/SQLite-Win32" CACHE PATH "SQLite Path")
        endif()

        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS,5.01 /ENTRY:mainCRTStartup")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS,5.01")
    endif()

    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "DB Browser for SQLite")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")

    list(PREPEND CMAKE_PREFIX_PATH ${QT5_PATH} ${SQLITE3_PATH})
    list(APPEND ADDITIONAL_PLATFORM_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/winapp.rc")
elseif(MINGW)
    # resource compilation for MinGW
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/sqlbicon.o"
        COMMAND windres "-I${CMAKE_CURRENT_BINARY_DIR}" "-i${CMAKE_CURRENT_SOURCE_DIR}/src/winapp.rc" -o "${CMAKE_CURRENT_BINARY_DIR}/sqlbicon.o" VERBATIM
    )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
    set(WIN32_STATIC_LINK -Wl,-Bstatic -lssl -lcrypto -lws2_32)
    set(ADDITIONAL_LIBS lzma)
    list(APPEND ADDITIONAL_PLATFORM_FILES "${CMAKE_CURRENT_BINARY_DIR}/sqlbicon.o")
endif()
