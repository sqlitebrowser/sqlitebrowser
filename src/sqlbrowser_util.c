#include "sqlbrowser_util.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*following routines extracted from shell.c for dump support*/

/*
** Return TRUE if the last non-whitespace character in z[] is a semicolon.
** z[] is N characters long.
*/
static int _ends_with_semicolon(const char *z, int N){
  while( N>0 && isspace(z[N-1]) ){ N--; }
  return N>0 && z[N-1]==';';
}

/*
** Test to see if a line consists entirely of whitespace.
*/
static int _all_whitespace(const char *z){
  for(; *z; z++){
    if( isspace(*z) ) continue;
    if( *z=='/' && z[1]=='*' ){
      z += 2;
      while( *z && (*z!='*' || z[1]!='/') ){ z++; }
      if( *z==0 ) return 0;
      z++;
      continue;
    }
    if( *z=='-' && z[1]=='-' ){
      z += 2;
      while( *z && *z!='\n' ){ z++; }
      if( *z==0 ) return 1;
      continue;
    }
    return 0;
  }
  return 1;
}


static char *sqlbrowser_getline(FILE *in){
  char *zLine;
  int nLine;
  int n;
  int eol;

  nLine = 100;
  zLine = malloc( nLine );
  if( zLine==0 ) return 0;
  n = 0;
  eol = 0;
  while( !eol ){
    if( n+100>nLine ){
      nLine = nLine*2 + 100;
      zLine = realloc(zLine, nLine);
      if( zLine==0 ) 
        return 0;
    }
    if( fgets(&zLine[n], nLine - n, in)==0 ){
      if( n==0 ){
        free(zLine);
        return 0;
      }
      zLine[n] = 0;
      eol = 1;
      break;
    }
    while( zLine[n] ){ n++; }
    if( n>0 && zLine[n-1]=='\n' ){
      n--;
      zLine[n] = 0;
      eol = 1;
    }
  }
  zLine = realloc( zLine, n+1 );
  return zLine;
}

static void process_input(sqlite3 * db, FILE *in, int * lineErr){
  char *zLine;
  char *zSql = 0;
  char * zErrMsg = 0;
  int nSql = 0;
  int rc;
  while((zLine = sqlbrowser_getline(in))!=0 ){
    if( (zSql==0 || zSql[0]==0) && _all_whitespace(zLine) ) continue;
    (*lineErr)++;
    if( zSql==0 ){
      int i;
      for(i=0; zLine[i] && isspace(zLine[i]); i++){}
      if( zLine[i]!=0 ){
        nSql = strlen(zLine);
        zSql = malloc( nSql+1 );
        strcpy(zSql, zLine);
      }
    }else{
      int len = strlen(zLine);
      zSql = realloc( zSql, nSql + len + 2 );
      /*if( zSql==0 ){
        fprintf(stderr,"%s: out of memory!\n", Argv0);
        exit(1);
      }*/
      strcpy(&zSql[nSql++], "\n");
      strcpy(&zSql[nSql], zLine);
      nSql += len;
    }
    free(zLine);
    if( zSql && _ends_with_semicolon(zSql, nSql) && sqlite3_complete(zSql) ){
      rc = sqlite3_exec(db, zSql, NULL, NULL, &zErrMsg);//&zErrMsg
      if( rc || zErrMsg ){
        //if( in!=0 && !p->echoOn ) printf("%s\n",zSql);
        if( zErrMsg!=0 ){
          /*printf("SQL error: %s\n", zErrMsg);*/
          free(zErrMsg);
          zErrMsg = 0;
          if( zSql ){
            free(zSql);
           }
           return;
        }/*else{
          printf("SQL error: %s\n", sqlite3_error_string(rc));
        }*/
      }
      free(zSql);
      zSql = 0;
      nSql = 0;
    }
  }
  if( zSql ){
    /*if( !_all_whitespace(zSql) ) printf("Incomplete SQL: %s\n", zSql);*/
    free(zSql);
  }
  /*normal exit, clear err*/
  *lineErr = 0;
}

/*
/Dump database to a file
*/
int load_database(sqlite3 * db, FILE * infile, int * lineErr){
  int rc = 0;
  process_input(db, infile, lineErr);

  return rc;
}

/* end of shell.c routines*/
