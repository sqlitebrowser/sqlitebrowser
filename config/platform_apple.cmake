if(QT_MAJOR STREQUAL "Qt5")
    # For Intel Mac's
    if(EXISTS /usr/local/opt/qt5)
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
    endif()

    # For Apple Silicon Mac's
    if(EXISTS /opt/homebrew/opt/qt5)
        list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt5")
    endif()
    if(EXISTS /opt/homebrew/opt/sqlitefts5)
        list(PREPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sqlitefts5")
    endif()

    # For Apple Silicon Mac's and install dependencies via our Homebrew tap(sqlitebrowser/homebrew-tap)
    if(customTap AND EXISTS /opt/homebrew/opt/)
        list(PREPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sqlb-qt@5")
        list(PREPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sqlb-sqlite")

        if(sqlcipher)
            list(APPEND SQLCIPHER_INCLUDE_DIR "/opt/homebrew/include")
            list(APPEND SQLCIPHER_LIBRARY "/opt/homebrew/opt/sqlb-sqlcipher/lib/libsqlcipher.0.dylib")
        endif()
    endif()
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "DB Browser for SQLite"
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/src/app.plist
)
