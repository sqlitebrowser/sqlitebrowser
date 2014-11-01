#ifndef SQLITE_H
#define SQLITE_H

#ifdef SQLCIPHER
    #define SQLITE_TEMP_STORE 2
    #define SQLITE_HAS_CODEC
    #include <sqlcipher/sqlite3.h>
#else
    #include <sqlite3.h>
#endif

#endif
