#ifndef _H_SQLBROWSERUTIL
#define _H_SQLBROWSERUTIL

#ifdef __cplusplus
extern "C" {
#endif

#include <sqlite3.h>
#include <stdio.h>

int load_database(sqlite3 * db, FILE * infile, int * lineErr);

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

#endif
