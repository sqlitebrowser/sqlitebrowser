#include "sqlbrowser_util.h"
#include "sqlite_source/sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlite_source/sqlite3.h"

/*following routines extracted from shell.c for dump support*/

/*
** Determines if a string is a number of not.
*/
static int isNumber(const char *z, int *realnum){
  if( *z=='-' || *z=='+' ) z++;
  if( !isdigit(*z) ){
    return 0;
  }
  z++;
  if( realnum ) *realnum = 0;
  while( isdigit(*z) ){ z++; }
  if( *z=='.' ){
    z++;
    if( !isdigit(*z) ) return 0;
    while( isdigit(*z) ){ z++; }
    if( realnum ) *realnum = 1;
  }
  if( *z=='e' || *z=='E' ){
    z++;
    if( *z=='+' || *z=='-' ) z++;
    if( !isdigit(*z) ) return 0;
    while( isdigit(*z) ){ z++; }
    if( realnum ) *realnum = 1;
  }
  return *z==0;
}


char *modeDescr[MODE_NUM_OF] = {
  "line",
  "column",
  "list",
  "semi",
  "html",
  "insert"
};

/*
** Number of elements in an array
*/
#define ArraySize(X)  (sizeof(X)/sizeof(X[0]))

/*
** Output the given string as a quoted string using SQL quoting conventions.
*/
static void output_quoted_string(FILE *out, const char *z){
  int i;
  int nSingle = 0;
  for(i=0; z[i]; i++){
    if( z[i]=='\'' ) nSingle++;
  }
  if( nSingle==0 ){
    fprintf(out,"'%s'",z);
  }else{
    fprintf(out,"'");
    while( *z ){
      for(i=0; z[i] && z[i]!='\''; i++){}
      if( i==0 ){
        fprintf(out,"''");
        z++;
      }else if( z[i]=='\'' ){
        fprintf(out,"%.*s''",i,z);
        z += i+1;
      }else{
        fprintf(out,"%s",z);
        break;
      }
    }
    fprintf(out,"'");
  }
}

/*
** Output the given string with characters that are special to
** HTML escaped.
*/
static void output_html_string(FILE *out, const char *z){
  int i;
  while( *z ){
    for(i=0; z[i] && z[i]!='<' && z[i]!='&'; i++){}
    if( i>0 ){
      fprintf(out,"%.*s",i,z);
    }
    if( z[i]=='<' ){
      fprintf(out,"&lt;");
    }else if( z[i]=='&' ){
      fprintf(out,"&amp;");
    }else{
      break;
    }
    z += i + 1;
  }
}

/*
** This is the callback routine that the SQLite library
** invokes for each row of a query result.
*/
static int callback(void *pArg, int nArg, char **azArg, char **azCol){
  int i;
  struct callback_data *p = (struct callback_data*)pArg;
  switch( p->mode ){
    case MODE_Line: {
      int w = 5;
      if( azArg==0 ) break;
      for(i=0; i<nArg; i++){
        int len = strlen(azCol[i]);
        if( len>w ) w = len;
      }
      if( p->cnt++>0 ) fprintf(p->out,"\n");
      for(i=0; i<nArg; i++){
        fprintf(p->out,"%*s = %s\n", w, azCol[i], 
                azArg[i] ? azArg[i] : p->nullvalue);
      }
      break;
    }
    case MODE_Column: {
      if( p->cnt++==0 ){
        for(i=0; i<nArg; i++){
          int w, n;
          if( i<ArraySize(p->colWidth) ){
             w = p->colWidth[i];
          }else{
             w = 0;
          }
          if( w<=0 ){
            w = strlen(azCol[i] ? azCol[i] : "");
            if( w<10 ) w = 10;
            n = strlen(azArg && azArg[i] ? azArg[i] : p->nullvalue);
            if( w<n ) w = n;
          }
          if( i<ArraySize(p->actualWidth) ){
            p->actualWidth[i] = w;
          }
          if( p->showHeader ){
            fprintf(p->out,"%-*.*s%s",w,w,azCol[i], i==nArg-1 ? "\n": "  ");
          }
        }
        if( p->showHeader ){
          for(i=0; i<nArg; i++){
            int w;
            if( i<ArraySize(p->actualWidth) ){
               w = p->actualWidth[i];
            }else{
               w = 10;
            }
            fprintf(p->out,"%-*.*s%s",w,w,"-----------------------------------"
                   "----------------------------------------------------------",
                    i==nArg-1 ? "\n": "  ");
          }
        }
      }
      if( azArg==0 ) break;
      for(i=0; i<nArg; i++){
        int w;
        if( i<ArraySize(p->actualWidth) ){
           w = p->actualWidth[i];
        }else{
           w = 10;
        }
        fprintf(p->out,"%-*.*s%s",w,w,
            azArg[i] ? azArg[i] : p->nullvalue, i==nArg-1 ? "\n": "  ");
      }
      break;
    }
    case MODE_Semi:
    case MODE_List: {
      if( p->cnt++==0 && p->showHeader ){
        for(i=0; i<nArg; i++){
          fprintf(p->out,"%s%s",azCol[i], i==nArg-1 ? "\n" : p->separator);
        }
      }
      if( azArg==0 ) break;
      for(i=0; i<nArg; i++){
        char *z = azArg[i];
        if( z==0 ) z = p->nullvalue;
        fprintf(p->out, "%s", z);
        if( i<nArg-1 ){
          fprintf(p->out, "%s", p->separator);
        }else if( p->mode==MODE_Semi ){
          fprintf(p->out, ";\n");
        }else{
          fprintf(p->out, "\n");
        }
      }
      break;
    }
    case MODE_Html: {
      if( p->cnt++==0 && p->showHeader ){
        fprintf(p->out,"<TR>");
        for(i=0; i<nArg; i++){
          fprintf(p->out,"<TH>%s</TH>",azCol[i]);
        }
        fprintf(p->out,"</TR>\n");
      }
      if( azArg==0 ) break;
      fprintf(p->out,"<TR>");
      for(i=0; i<nArg; i++){
        fprintf(p->out,"<TD>");
        output_html_string(p->out, azArg[i] ? azArg[i] : p->nullvalue);
        fprintf(p->out,"</TD>\n");
      }
      fprintf(p->out,"</TR>\n");
      break;
    }
    case MODE_Insert: {
      if( azArg==0 ) break;
      fprintf(p->out,"INSERT INTO %s VALUES(",p->zDestTable);
      for(i=0; i<nArg; i++){
        char *zSep = i>0 ? ",": "";
        if( azArg[i]==0 ){
          fprintf(p->out,"%sNULL",zSep);
        }else if( isNumber(azArg[i], 0) ){
          fprintf(p->out,"%s%s",zSep, azArg[i]);
        }else{
          if( zSep[0] ) fprintf(p->out,"%s",zSep);
          output_quoted_string(p->out, azArg[i]);
        }
      }
      fprintf(p->out,");\n");
      break;
    }
  }
  return 0;
}

/*
** Set the destination table field of the callback_data structure to
** the name of the table given.  Escape any quote characters in the
** table name.
*/
static void set_table_name(struct callback_data *p, const char *zName){
  int i, n;
  int needQuote;
  char *z;

  if( p->zDestTable ){
    free(p->zDestTable);
    p->zDestTable = 0;
  }
  if( zName==0 ) return;
  needQuote = !isalpha(*zName) && *zName!='_';
  for(i=n=0; zName[i]; i++, n++){
    if( !isalnum(zName[i]) && zName[i]!='_' ){
      needQuote = 1;
      if( zName[i]=='\'' ) n++;
    }
  }
  if( needQuote ) n += 2;
  z = p->zDestTable = malloc( n+1 );
  if( z==0 ){
    fprintf(stderr,"Out of memory!\n");
    exit(1);
  }
  n = 0;
  if( needQuote ) z[n++] = '\'';
  for(i=0; zName[i]; i++){
    z[n++] = zName[i];
    if( zName[i]=='\'' ) z[n++] = '\'';
  }
  if( needQuote ) z[n++] = '\'';
  z[n] = 0;
}

/*
** This is a different callback routine used for dumping the database.
** Each row received by this callback consists of a table name,
** the table type ("index" or "table") and SQL to create the table.
** This routine should print text sufficient to recreate the table.
*/
static int dump_callback(void *pArg, int nArg, char **azArg, char **azCol){
  struct callback_data *p = (struct callback_data *)pArg;
  if( nArg!=3 ) return 1;
  fprintf(p->out, "%s;\n", azArg[2]);
  if( strcmp(azArg[1],"table")==0 ){
    struct callback_data d2;
    char*   stmt;
    d2 = *p;
    d2.mode = MODE_Insert;
    d2.zDestTable = 0;
    set_table_name(&d2, azArg[0]);
    stmt = sqlite3_mprintf("SELECT * FROM '%q'", azArg[0]);
    sqlite3_exec(p->db,stmt, callback, &d2, 0);
    sqlite3_free(stmt);
    set_table_name(&d2, 0);
  }
  return 0;
}

/*
/Dump database to a file
*/
int dump_database(sqlite3 * db, FILE * outfile){
  int rc = 0;
  char *zErrMsg = 0;
  struct callback_data p;
  memset(&p, 0, sizeof(p));
  p.db = db;
  p.mode = MODE_List;
  strcpy(p.separator,"|");
  p.showHeader = 0;
  p.out = outfile;
    //open_db(p);
  fprintf(p.out, "BEGIN TRANSACTION;\n");
  sqlite3_exec(p.db,
        "SELECT name, type, sql FROM sqlite_master "
        "WHERE type!='meta' AND sql NOT NULL "
        "ORDER BY substr(type,2,1), name",
        dump_callback, &p, &zErrMsg
      );
  if( zErrMsg ){
      /*fprintf(stderr,"Error: %s\n", zErrMsg);*/
      free(zErrMsg);
    }else{
      fprintf(p.out, "COMMIT;\n");
    }
  return rc;
}

/*
/Dump database to a file
*/
int load_database(sqlite3 * db, FILE * infile, int * lineErr){
  int rc = 0;
  process_input(db, infile, lineErr);

  return rc;
}

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


char *sqlbrowser_getline(FILE *in){
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

void process_input(sqlite3 * db, FILE *in, int * lineErr){
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

/* end of shell.c routines*/

