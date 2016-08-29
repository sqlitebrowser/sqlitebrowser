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

#endif
