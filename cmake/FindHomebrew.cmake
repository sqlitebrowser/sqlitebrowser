execute_process(
    COMMAND brew --prefix
    RESULT_VARIABLE STATUS
    OUTPUT_VARIABLE HOMEBREW_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET)

# If you are using the ARM64 architecture, it is assumed that you have installed
# formula, which supports universal binary building.
if (customTap)
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "(arm64)|(ARM64)")
        set (Qt5_DIR "${HOMEBREW_PATH}/opt/db4subqt@5")
        set (SQLite_DIR "${HOMEBREW_PATH}/opt/db4subsqlitefts@5")
        if (sqlcipher)
            set (SQLCipher_INCLUDE_DIR "${HOMEBREW_PATH}/include")
            set (SQLCIpher_LIBRARY "${HOMEBREW_PATH}/lib/libsqlcipher.0.dylib")
        endif()
    else()
        set (Qt5_DIR "${HOMEBREW_PATH}/opt/db4sqt@5")
        set (SQLite_DIR "${HOMEBREW_PATH}/opt/db4ssqlitefts@5")
        if (sqlcipher)
            set (SQLCipher_INCLUDE_DIR "${HOMEBREW_PATH}/include")
            set (SQLCIpher_LIBRARY "${HOMEBREW_PATH}/lib/libsqlcipher.0.dylib")
        endif()
    endif()
else()
    set (Qt5_DIR "${HOMEBREW_PATH}/opt/qt@5")
    set (SQLite_DIR "${HOMEBREW_PATH}/opt/db4ssqlitefts@5")
    if (sqlcipher)
        set (SQLCipher_INCLUDE_DIR "${HOMEBREW_PATH}/include")
        set (SQLCIpher_LIBRARY "${HOMEBREW_PATH}/lib/libsqlcipher.0.dylib")
    endif()
endif()