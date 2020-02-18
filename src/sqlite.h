#ifndef SQLITE_H
#define SQLITE_H

#ifdef ENABLE_SQLCIPHER
    #define SQLITE_TEMP_STORE 2
    #define SQLITE_HAS_CODEC
#ifdef Q_OS_WIN32
    #include <sqlite3.h>
#else
    #include <sqlcipher/sqlite3.h>
#endif
#else
    #include <sqlite3.h>
#endif

// For older versions of the SQLite library which do not have the SQLITE_DETERMINISTIC flag
// yet (which introduced in this commit: https://www.sqlite.org/src/info/5716fc2341ddd8cf), we
// define it here with a value of 0. Because it is ORed with other constants a value of 0 is a no-op.
#ifndef SQLITE_DETERMINISTIC
#define SQLITE_DETERMINISTIC 0
#endif

#endif
