/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
#line 35 "parse.y"

#include "sqliteInt.h"
#include "parse.h"

/*
** An instance of this structure holds information about the
** LIMIT clause of a SELECT statement.
*/
struct LimitVal {
  int limit;    /* The LIMIT value.  -1 if there is no limit */
  int offset;   /* The OFFSET.  0 if there is none */
};

/*
** An instance of the following structure describes the event of a
** TRIGGER.  "a" is the event type, one of TK_UPDATE, TK_INSERT,
** TK_DELETE, or TK_INSTEAD.  If the event is of the form
**
**      UPDATE ON (a,b,c)
**
** Then the "b" IdList records the list "a,b,c".
*/
struct TrigEvent { int a; IdList * b; };


#line 34 "parse.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    sqliteParserTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is sqliteParserTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.
**    sqliteParserARG_SDECL     A static variable declaration for the %extra_argument
**    sqliteParserARG_PDECL     A parameter declaration for the %extra_argument
**    sqliteParserARG_STORE     Code to store %extra_argument into yypParser
**    sqliteParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
/*  */
#define YYCODETYPE unsigned char
#define YYNOCODE 219
#define YYACTIONTYPE unsigned short int
#define sqliteParserTOKENTYPE Token
typedef union {
  sqliteParserTOKENTYPE yy0;
  Select* yy11;
  int yy52;
  ExprList* yy62;
  IdList* yy92;
  Token yy210;
  struct TrigEvent yy234;
  Expr * yy270;
  struct {int value; int mask;} yy279;
  struct LimitVal yy280;
  Expr* yy334;
  SrcList* yy335;
  TriggerStep * yy347;
  int yy437;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define sqliteParserARG_SDECL Parse *pParse;
#define sqliteParserARG_PDECL ,Parse *pParse
#define sqliteParserARG_FETCH Parse *pParse = yypParser->pParse
#define sqliteParserARG_STORE yypParser->pParse = pParse
#define YYNSTATE 555
#define YYNRULE 287
#define YYERRORSYMBOL 155
#define YYERRSYMDT yy437
#define YYFALLBACK 1
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)
/* Next is the action table.  Each entry in this table contains
**
**  +  An integer which is the number representing the look-ahead
**     token
**
**  +  An integer indicating what action to take.  Number (N) between
**     0 and YYNSTATE-1 mean shift the look-ahead and go to state N.
**     Numbers between YYNSTATE and YYNSTATE+YYNRULE-1 mean reduce by
**     rule N-YYNSTATE.  Number YYNSTATE+YYNRULE means that a syntax
**     error has occurred.  Number YYNSTATE+YYNRULE+1 means the parser
**     accepts its input.
**
**  +  A pointer to the next entry with the same hash value.
**
** The action table is really a series of hash tables.  Each hash
** table contains a number of entries which is a power of two.  The
** "state" table (which follows) contains information about the starting
** point and size of each hash table.
*/
struct yyActionEntry {
  YYCODETYPE   lookahead;   /* The value of the look-ahead token */
  YYCODETYPE   next;        /* Next entry + 1. Zero at end of collision chain */
  YYACTIONTYPE action;      /* Action to take for this look-ahead */
};
typedef struct yyActionEntry yyActionEntry;
static const yyActionEntry yyActionTable[] = {
/* State 0 */
  { 156,   0,   3}, /*  1:                explain shift  3 */
  { 139,   0,   1}, /*  2:                cmdlist shift  1 */
  { 170,   0, 843}, /*  3:                  input accept */
  {  47,   0, 553}, /*  4:                EXPLAIN shift  553 */
  { 154,   0, 554}, /*  5:                   ecmd shift  554 */
  { 107,   4, 552}, /*  6:                   SEMI shift  552 */
/* State 1 */
  {   0,   0, 555}, /*  1:                      $ reduce 0 */
  { 156,   0,   3}, /*  2:                explain shift  3 */
  { 107,   4, 552}, /*  3:                   SEMI shift  552 */
  {  47,   0, 553}, /*  4:                EXPLAIN shift  553 */
  { 154,   0,   2}, /*  5:                   ecmd shift  2 */
/* State 3 */
  {  66,   0, 512}, /*  1:                 INSERT shift  512 */
  {  28,   0, 515}, /*  2:                   COPY shift  515 */
  {  24,   0,  23}, /*  3:                 COMMIT shift  23 */
  {  29,   0, 388}, /*  4:                 CREATE shift  388 */
  {   8,   0, 543}, /*  5:                 ATTACH shift  543 */
  {  34,   0, 490}, /*  6:                 DELETE shift  490 */
  { 138,   2,   6}, /*  7:                    cmd shift  6 */
  {  95,   4, 526}, /*  8:                 PRAGMA shift  526 */
  { 140,   5,   4}, /*  9:                   cmdx shift  4 */
  { 185,   0,  69}, /* 10:              oneselect shift  69 */
  {  10,   0,   7}, /* 11:                  BEGIN shift  7 */
  {  37,   0, 549}, /* 12:                 DETACH shift  549 */
  { 100,   6, 514}, /* 13:                REPLACE shift  514 */
  { 123,   0, 495}, /* 14:                 UPDATE shift  495 */
  { 102,   0,  27}, /* 15:               ROLLBACK shift  27 */
  { 147,  12,  29}, /* 16:           create_table shift  29 */
  { 126,   0, 524}, /* 17:                 VACUUM shift  524 */
  {  40,   0, 478}, /* 18:                   DROP shift  478 */
  { 106,  18,  73}, /* 19:                 SELECT shift  73 */
  { 173,   0, 502}, /* 20:             insert_cmd shift  502 */
  { 196,   0, 489}, /* 21:                 select shift  489 */
  {  43,   0,  25}, /* 22:                    END shift  25 */
/* State 4 */
  { 107,   0,   5}, /*  1:                   SEMI shift  5 */
/* State 6 */
  { 107,   0, 560}, /*  1:                   SEMI reduce 5 */
/* State 7 */
  { 206,   0,   8}, /*  1:              trans_opt shift  8 */
  { 117,   0,  18}, /*  2:            TRANSACTION shift  18 */
/* State 8 */
  {  90,   0,  10}, /*  1:                     ON shift  10 */
  { 184,   0,   9}, /*  2:                 onconf shift  9 */
  { 107,   0, 643}, /*  3:                   SEMI reduce 88 */
/* State 9 */
  { 107,   0, 563}, /*  1:                   SEMI reduce 8 */
/* State 10 */
  {  26,   0,  11}, /*  1:               CONFLICT shift  11 */
/* State 11 */
  { 102,   3,  13}, /*  1:               ROLLBACK shift  13 */
  { 193,   4,  12}, /*  2:            resolvetype shift  12 */
  {  60,   6,  16}, /*  3:                 IGNORE shift  16 */
  {   1,   0,  14}, /*  4:                  ABORT shift  14 */
  { 100,   0,  17}, /*  5:                REPLACE shift  17 */
  {  48,   0,  15}, /*  6:                   FAIL shift  15 */
/* State 18 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1,  19}, /*  2:                     nm shift  19 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 23 */
  { 117,   0,  18}, /*  1:            TRANSACTION shift  18 */
  { 107,   0, 564}, /*  2:                   SEMI reduce 9 */
  { 206,   2,  24}, /*  3:              trans_opt shift  24 */
/* State 24 */
  { 107,   0, 567}, /*  1:                   SEMI reduce 12 */
/* State 25 */
  { 117,   0,  18}, /*  1:            TRANSACTION shift  18 */
  { 107,   0, 564}, /*  2:                   SEMI reduce 9 */
  { 206,   2,  26}, /*  3:              trans_opt shift  26 */
/* State 26 */
  { 107,   0, 568}, /*  1:                   SEMI reduce 13 */
/* State 27 */
  { 117,   0,  18}, /*  1:            TRANSACTION shift  18 */
  { 107,   0, 564}, /*  2:                   SEMI reduce 9 */
  { 206,   2,  28}, /*  3:              trans_opt shift  28 */
/* State 28 */
  { 107,   0, 569}, /*  1:                   SEMI reduce 14 */
/* State 29 */
  {   6,   0, 386}, /*  1:                     AS shift  386 */
  { 148,   3,  30}, /*  2:      create_table_args shift  30 */
  {  79,   0,  31}, /*  3:                     LP shift  31 */
/* State 30 */
  { 107,   0, 570}, /*  1:                   SEMI reduce 15 */
/* State 31 */
  {  59,   0,  20}, /*  1:                     ID shift  20 */
  { 113,   0,  21}, /*  2:                 STRING shift  21 */
  { 142,   0, 385}, /*  3:                 column shift  385 */
  { 143,   1,  37}, /*  4:               columnid shift  37 */
  { 144,   6,  32}, /*  5:             columnlist shift  32 */
  {  74,   0,  22}, /*  6:                JOIN_KW shift  22 */
  { 181,   0, 351}, /*  7:                     nm shift  351 */
/* State 32 */
  { 104,   0, 631}, /*  1:                     RP reduce 76 */
  {  22,   0,  35}, /*  2:                  COMMA shift  35 */
  { 146,   1,  33}, /*  3:           conslist_opt shift  33 */
/* State 33 */
  { 104,   0,  34}, /*  1:                     RP shift  34 */
/* State 34 */
  { 107,   0, 574}, /*  1:                   SEMI reduce 19 */
/* State 35 */
  { 143,   0,  37}, /*  1:               columnid shift  37 */
  {  27,   0, 355}, /*  2:             CONSTRAINT shift  355 */
  { 145,   0, 352}, /*  3:               conslist shift  352 */
  {  96,   5, 357}, /*  4:                PRIMARY shift  357 */
  {  18,   0, 368}, /*  5:                  CHECK shift  368 */
  { 122,   4, 363}, /*  6:                 UNIQUE shift  363 */
  { 113,   9,  21}, /*  7:                 STRING shift  21 */
  {  59,   0,  20}, /*  8:                     ID shift  20 */
  {  74,   0,  22}, /*  9:                JOIN_KW shift  22 */
  { 204,   7, 384}, /* 10:                  tcons shift  384 */
  { 142,  12,  36}, /* 11:                 column shift  36 */
  {  51,   0, 371}, /* 12:                FOREIGN shift  371 */
  { 181,  11, 351}, /* 13:                     nm shift  351 */
/* State 37 */
  { 165,   0, 350}, /*  1:                    ids shift  350 */
  { 113,   0, 257}, /*  2:                 STRING shift  257 */
  { 212,   0,  38}, /*  3:                   type shift  38 */
  { 213,   2, 342}, /*  4:               typename shift  342 */
  {  59,   0, 256}, /*  5:                     ID shift  256 */
/* State 38 */
  { 133,   0,  39}, /*  1:               carglist shift  39 */
/* State 39 */
  {  96,   4,  54}, /*  1:                PRIMARY shift  54 */
  {  27,   0,  41}, /*  2:             CONSTRAINT shift  41 */
  { 132,   0,  40}, /*  3:                   carg shift  40 */
  {  31,   5, 330}, /*  4:                DEFAULT shift  330 */
  {  18,   0,  62}, /*  5:                  CHECK shift  62 */
  { 122,   1,  60}, /*  6:                 UNIQUE shift  60 */
  {  32,   0, 327}, /*  7:             DEFERRABLE shift  327 */
  { 137,  11, 329}, /*  8:                  ccons shift  329 */
  { 151,   0, 324}, /*  9:        defer_subclause shift  324 */
  {  87,   0,  44}, /* 10:                   NULL shift  44 */
  {  98,  12, 302}, /* 11:             REFERENCES shift  302 */
  {  85,  13,  46}, /* 12:                    NOT shift  46 */
  {  20,   0, 325}, /* 13:                COLLATE shift  325 */
/* State 41 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1,  42}, /*  2:                     nm shift  42 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 42 */
  {  20,   0, 325}, /*  1:                COLLATE shift  325 */
  { 151,   0, 324}, /*  2:        defer_subclause shift  324 */
  { 122,   4,  60}, /*  3:                 UNIQUE shift  60 */
  {  32,   0, 327}, /*  4:             DEFERRABLE shift  327 */
  {  87,   0,  44}, /*  5:                   NULL shift  44 */
  {  85,   0,  46}, /*  6:                    NOT shift  46 */
  {  96,   0,  54}, /*  7:                PRIMARY shift  54 */
  { 137,   5,  43}, /*  8:                  ccons shift  43 */
  {  98,  10, 302}, /*  9:             REFERENCES shift  302 */
  {  18,   0,  62}, /* 10:                  CHECK shift  62 */
/* State 44 */
  { 184,   2,  45}, /*  1:                 onconf shift  45 */
  {  90,   0,  10}, /*  2:                     ON shift  10 */
/* State 46 */
  {  32,   0,  49}, /*  1:             DEFERRABLE shift  49 */
  {  87,   0,  47}, /*  2:                   NULL shift  47 */
/* State 47 */
  { 184,   2,  48}, /*  1:                 onconf shift  48 */
  {  90,   0,  10}, /*  2:                     ON shift  10 */
/* State 49 */
  {  65,   0,  51}, /*  1:              INITIALLY shift  51 */
  { 169,   1,  50}, /*  2: init_deferred_pred_opt shift  50 */
/* State 51 */
  {  62,   0,  53}, /*  1:              IMMEDIATE shift  53 */
  {  33,   0,  52}, /*  2:               DEFERRED shift  52 */
/* State 54 */
  {  75,   0,  55}, /*  1:                    KEY shift  55 */
/* State 55 */
  {  36,   0,  59}, /*  1:                   DESC shift  59 */
  { 202,   3,  56}, /*  2:              sortorder shift  56 */
  {   7,   0,  58}, /*  3:                    ASC shift  58 */
/* State 56 */
  { 184,   2,  57}, /*  1:                 onconf shift  57 */
  {  90,   0,  10}, /*  2:                     ON shift  10 */
/* State 60 */
  { 184,   2,  61}, /*  1:                 onconf shift  61 */
  {  90,   0,  10}, /*  2:                     ON shift  10 */
/* State 62 */
  {  79,   0,  63}, /*  1:                     LP shift  63 */
/* State 63 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 299}, /*  8:                   expr shift  299 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 64 */
  {  39,   0, 583}, /*  1:                    DOT reduce 28 */
  {  79,   1,  65}, /*  2:                     LP shift  65 */
/* State 65 */
  {  59,   0,  64}, /*  1:                     ID shift  64 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  74,   0,  67}, /*  3:                JOIN_KW shift  67 */
  { 111,   0, 297}, /*  4:                   STAR shift  297 */
  {  94,   0, 181}, /*  5:                   PLUS shift  181 */
  { 113,   1,  66}, /*  6:                 STRING shift  66 */
  {  79,   0,  68}, /*  7:                     LP shift  68 */
  {  97,   7, 193}, /*  8:                  RAISE shift  193 */
  {  85,  10, 175}, /*  9:                    NOT shift  175 */
  {  49,  11, 174}, /* 10:                  FLOAT shift  174 */
  {  13,   0, 177}, /* 11:                 BITNOT shift  177 */
  {  83,   0, 179}, /* 12:                  MINUS shift  179 */
  {  68,   0, 173}, /* 13:                INTEGER shift  173 */
  { 157,   9, 172}, /* 14:                   expr shift  172 */
  { 158,  13, 219}, /* 15:               expritem shift  219 */
  { 159,  17, 295}, /* 16:               exprlist shift  295 */
  {  87,   0, 106}, /* 17:                   NULL shift  106 */
  {  17,   0, 183}, /* 18:                   CASE shift  183 */
/* State 66 */
  {  39,   0, 584}, /*  1:                    DOT reduce 29 */
/* State 67 */
  {  39,   0, 585}, /*  1:                    DOT reduce 30 */
/* State 68 */
  { 113,   4,  66}, /*  1:                 STRING shift  66 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  74,   0,  67}, /*  3:                JOIN_KW shift  67 */
  {  59,   0,  64}, /*  4:                     ID shift  64 */
  {  94,   0, 181}, /*  5:                   PLUS shift  181 */
  { 185,   1,  69}, /*  6:              oneselect shift  69 */
  {  79,   0,  68}, /*  7:                     LP shift  68 */
  {  97,   7, 193}, /*  8:                  RAISE shift  193 */
  {  85,  10, 175}, /*  9:                    NOT shift  175 */
  {  49,  11, 174}, /* 10:                  FLOAT shift  174 */
  {  13,   0, 177}, /* 11:                 BITNOT shift  177 */
  {  83,   0, 179}, /* 12:                  MINUS shift  179 */
  { 106,   0,  73}, /* 13:                 SELECT shift  73 */
  { 157,   9, 293}, /* 14:                   expr shift  293 */
  {  68,   0, 173}, /* 15:                INTEGER shift  173 */
  {  87,   0, 106}, /* 16:                   NULL shift  106 */
  { 196,  13,  70}, /* 17:                 select shift  70 */
  {  17,   0, 183}, /* 18:                   CASE shift  183 */
/* State 70 */
  { 180,   0,  71}, /*  1:         multiselect_op shift  71 */
  { 121,   3, 167}, /*  2:                  UNION shift  167 */
  {  46,   0, 170}, /*  3:                 EXCEPT shift  170 */
  {  69,   0, 169}, /*  4:              INTERSECT shift  169 */
  { 104,   4, 292}, /*  5:                     RP shift  292 */
/* State 71 */
  { 106,   0,  73}, /*  1:                 SELECT shift  73 */
  { 185,   0,  72}, /*  2:              oneselect shift  72 */
/* State 73 */
  { 153,   0,  74}, /*  1:               distinct shift  74 */
  {   4,   0, 291}, /*  2:                    ALL shift  291 */
  {  38,   0, 290}, /*  3:               DISTINCT shift  290 */
/* State 74 */
  { 194,   0, 283}, /*  1:                   sclp shift  283 */
  { 195,   0,  75}, /*  2:             selcollist shift  75 */
/* State 75 */
  {  22,   0, 241}, /*  1:                  COMMA shift  241 */
  {  52,   1, 242}, /*  2:                   FROM shift  242 */
  { 161,   0,  76}, /*  3:                   from shift  76 */
/* State 76 */
  { 130,   0, 239}, /*  1:                  WHERE shift  239 */
  { 217,   0,  77}, /*  2:              where_opt shift  77 */
/* State 77 */
  { 162,   2,  78}, /*  1:            groupby_opt shift  78 */
  {  56,   0, 236}, /*  2:                  GROUP shift  236 */
/* State 78 */
  {  58,   0, 234}, /*  1:                 HAVING shift  234 */
  { 163,   0,  79}, /*  2:             having_opt shift  79 */
/* State 79 */
  {  93,   0,  93}, /*  1:                  ORDER shift  93 */
  { 187,   1,  80}, /*  2:            orderby_opt shift  80 */
/* State 80 */
  { 178,   2,  81}, /*  1:              limit_opt shift  81 */
  {  78,   0,  82}, /*  2:                  LIMIT shift  82 */
/* State 82 */
  {  68,   0,  83}, /*  1:                INTEGER shift  83 */
  {  83,   0,  86}, /*  2:                  MINUS shift  86 */
  {  94,   0,  84}, /*  3:                   PLUS shift  84 */
  { 199,   2,  88}, /*  4:                 signed shift  88 */
/* State 84 */
  {  68,   0,  85}, /*  1:                INTEGER shift  85 */
/* State 86 */
  {  68,   0,  87}, /*  1:                INTEGER shift  87 */
/* State 88 */
  {  22,   0,  91}, /*  1:                  COMMA shift  91 */
  {  89,   0,  89}, /*  2:                 OFFSET shift  89 */
/* State 89 */
  {  68,   0,  83}, /*  1:                INTEGER shift  83 */
  {  83,   0,  86}, /*  2:                  MINUS shift  86 */
  {  94,   0,  84}, /*  3:                   PLUS shift  84 */
  { 199,   2,  90}, /*  4:                 signed shift  90 */
/* State 91 */
  {  68,   0,  83}, /*  1:                INTEGER shift  83 */
  {  83,   0,  86}, /*  2:                  MINUS shift  86 */
  {  94,   0,  84}, /*  3:                   PLUS shift  84 */
  { 199,   2,  92}, /*  4:                 signed shift  92 */
/* State 93 */
  {  15,   0,  94}, /*  1:                     BY shift  94 */
/* State 94 */
  {  85,   2, 175}, /*  1:                    NOT shift  175 */
  {  68,   4, 173}, /*  2:                INTEGER shift  173 */
  {  87,   0, 106}, /*  3:                   NULL shift  106 */
  {  17,   0, 183}, /*  4:                   CASE shift  183 */
  { 157,   0, 103}, /*  5:                   expr shift  103 */
  { 113,   8,  66}, /*  6:                 STRING shift  66 */
  {  74,   0,  67}, /*  7:                JOIN_KW shift  67 */
  {  79,   0,  68}, /*  8:                     LP shift  68 */
  {  59,   0,  64}, /*  9:                     ID shift  64 */
  {  94,   0, 181}, /* 10:                   PLUS shift  181 */
  {  13,   0, 177}, /* 11:                 BITNOT shift  177 */
  { 181,   6, 107}, /* 12:                     nm shift  107 */
  {  97,   0, 193}, /* 13:                  RAISE shift  193 */
  { 200,  11, 231}, /* 14:               sortitem shift  231 */
  { 201,   0,  95}, /* 15:               sortlist shift  95 */
  {  83,  17, 179}, /* 16:                  MINUS shift  179 */
  {  49,   0, 174}, /* 17:                  FLOAT shift  174 */
/* State 95 */
  {  22,   0,  96}, /*  1:                  COMMA shift  96 */
/* State 96 */
  {  97,   3, 193}, /*  1:                  RAISE shift  193 */
  { 113,   1,  66}, /*  2:                 STRING shift  66 */
  {  49,   7, 174}, /*  3:                  FLOAT shift  174 */
  {  83,   0, 179}, /*  4:                  MINUS shift  179 */
  {  68,   0, 173}, /*  5:                INTEGER shift  173 */
  { 181,  10, 107}, /*  6:                     nm shift  107 */
  {  17,   0, 183}, /*  7:                   CASE shift  183 */
  {  87,   0, 106}, /*  8:                   NULL shift  106 */
  { 200,   0,  97}, /*  9:               sortitem shift  97 */
  {  85,   0, 175}, /* 10:                    NOT shift  175 */
  {  74,   0,  67}, /* 11:                JOIN_KW shift  67 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  13,   0, 177}, /* 13:                 BITNOT shift  177 */
  { 157,  13, 103}, /* 14:                   expr shift  103 */
  {  94,   0, 181}, /* 15:                   PLUS shift  181 */
  {  79,   0,  68}, /* 16:                     LP shift  68 */
/* State 97 */
  {  20,   0, 100}, /*  1:                COLLATE shift  100 */
  { 141,   0,  98}, /*  2:                collate shift  98 */
/* State 98 */
  {  36,   0,  59}, /*  1:                   DESC shift  59 */
  { 202,   3,  99}, /*  2:              sortorder shift  99 */
  {   7,   0,  58}, /*  3:                    ASC shift  58 */
/* State 100 */
  { 164,   0, 102}, /*  1:                     id shift  102 */
  {  59,   0, 101}, /*  2:                     ID shift  101 */
/* State 103 */
  {  84,   0, 123}, /*  1:                     NE shift  123 */
  {  85,   4, 137}, /*  2:                    NOT shift  137 */
  {  86,   0, 159}, /*  3:                NOTNULL shift  159 */
  {  57,   0, 117}, /*  4:                     GT shift  117 */
  {  63,   0, 164}, /*  5:                     IN shift  164 */
  {   5,   0, 105}, /*  6:                    AND shift  105 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  91,   5, 113}, /*  8:                     OR shift  113 */
  {  92,   0, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  77,   0, 140}, /* 14:                   LIKE shift  140 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  {  99,   7, 150}, /* 16:                    REM shift  150 */
  {  72,   0, 154}, /* 17:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 18:                     EQ shift  125 */
  {  81,  20, 115}, /* 19:                     LT shift  115 */
  {  25,   0, 152}, /* 20:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 21:                     LE shift  119 */
  { 105,  14, 133}, /* 22:                 RSHIFT shift  133 */
  {  83,  24, 144}, /* 23:                  MINUS shift  144 */
  {  55,   0, 141}, /* 24:                   GLOB shift  141 */
  {  80,   0, 131}, /* 25:                 LSHIFT shift  131 */
  { 109,  19, 148}, /* 26:                  SLASH shift  148 */
  {  54,   0, 121}, /* 27:                     GE shift  121 */
  { 111,  23, 146}, /* 28:                   STAR shift  146 */
/* State 105 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 112}, /*  8:                   expr shift  112 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 107 */
  {  39,   0, 108}, /*  1:                    DOT shift  108 */
/* State 108 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 109}, /*  2:                     nm shift  109 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 109 */
  {  39,   0, 110}, /*  1:                    DOT shift  110 */
/* State 110 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 111}, /*  2:                     nm shift  111 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 112 */
  {  54,   0, 121}, /*  1:                     GE shift  121 */
  { 105,   0, 133}, /*  2:                 RSHIFT shift  133 */
  {  80,   1, 131}, /*  3:                 LSHIFT shift  131 */
  {  81,   5, 115}, /*  4:                     LT shift  115 */
  {  55,   0, 141}, /*  5:                   GLOB shift  141 */
  { 109,  10, 148}, /*  6:                  SLASH shift  148 */
  {  84,   0, 123}, /*  7:                     NE shift  123 */
  { 111,  11, 146}, /*  8:                   STAR shift  146 */
  {  86,   0, 159}, /*  9:                NOTNULL shift  159 */
  {  83,  14, 144}, /* 10:                  MINUS shift  144 */
  {  85,   0, 137}, /* 11:                    NOT shift  137 */
  {  63,  16, 164}, /* 12:                     IN shift  164 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  57,   0, 117}, /* 14:                     GT shift  117 */
  {  92,  18, 104}, /* 15:      ORACLE_OUTER_JOIN shift  104 */
  {  11,   0, 160}, /* 16:                BETWEEN shift  160 */
  {  94,   0, 142}, /* 17:                   PLUS shift  142 */
  {  14,   0, 129}, /* 18:                  BITOR shift  129 */
  {  45,   0, 125}, /* 19:                     EQ shift  125 */
  {  71,  19, 155}, /* 20:                     IS shift  155 */
  {  72,   0, 154}, /* 21:                 ISNULL shift  154 */
  { 177,  23, 135}, /* 22:                 likeop shift  135 */
  {  99,   0, 150}, /* 23:                    REM shift  150 */
  {  25,   0, 152}, /* 24:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 25:                     LE shift  119 */
  {  77,  24, 140}, /* 26:                   LIKE shift  140 */
/* State 113 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 114}, /*  8:                   expr shift  114 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 114 */
  {  81,   7, 115}, /*  1:                     LT shift  115 */
  { 109,   8, 148}, /*  2:                  SLASH shift  148 */
  {  83,   0, 144}, /*  3:                  MINUS shift  144 */
  { 111,   9, 146}, /*  4:                   STAR shift  146 */
  {  85,   0, 137}, /*  5:                    NOT shift  137 */
  {  86,  11, 159}, /*  6:                NOTNULL shift  159 */
  {  54,   0, 121}, /*  7:                     GE shift  121 */
  {  55,   0, 141}, /*  8:                   GLOB shift  141 */
  {  84,  17, 123}, /*  9:                     NE shift  123 */
  {  63,   0, 164}, /* 10:                     IN shift  164 */
  {   5,   0, 105}, /* 11:                    AND shift  105 */
  {  92,  20, 104}, /* 12:      ORACLE_OUTER_JOIN shift  104 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  94,   0, 142}, /* 14:                   PLUS shift  142 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  { 177,   0, 135}, /* 16:                 likeop shift  135 */
  {  57,   0, 117}, /* 17:                     GT shift  117 */
  {  71,   0, 155}, /* 18:                     IS shift  155 */
  {  99,  21, 150}, /* 19:                    REM shift  150 */
  {  11,   0, 160}, /* 20:                BETWEEN shift  160 */
  {  72,  22, 154}, /* 21:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 22:                     EQ shift  125 */
  {  76,   0, 119}, /* 23:                     LE shift  119 */
  {  77,   0, 140}, /* 24:                   LIKE shift  140 */
  { 105,   0, 133}, /* 25:                 RSHIFT shift  133 */
  {  25,   0, 152}, /* 26:                 CONCAT shift  152 */
  {  80,   0, 131}, /* 27:                 LSHIFT shift  131 */
/* State 115 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 116}, /*  8:                   expr shift  116 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 116 */
  {  12,   0, 127}, /*  1:                 BITAND shift  127 */
  { 109,   5, 148}, /*  2:                  SLASH shift  148 */
  {  14,   0, 129}, /*  3:                  BITOR shift  129 */
  { 111,   6, 146}, /*  4:                   STAR shift  146 */
  {  25,   0, 152}, /*  5:                 CONCAT shift  152 */
  {  99,   0, 150}, /*  6:                    REM shift  150 */
  {  80,   0, 131}, /*  7:                 LSHIFT shift  131 */
  { 105,   0, 133}, /*  8:                 RSHIFT shift  133 */
  {  92,   7, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   8, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  83,   0, 144}, /* 12:                  MINUS shift  144 */
/* State 117 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 118}, /*  8:                   expr shift  118 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 118 */
  {  12,   0, 127}, /*  1:                 BITAND shift  127 */
  { 109,   5, 148}, /*  2:                  SLASH shift  148 */
  {  14,   0, 129}, /*  3:                  BITOR shift  129 */
  { 111,   6, 146}, /*  4:                   STAR shift  146 */
  {  25,   0, 152}, /*  5:                 CONCAT shift  152 */
  {  99,   0, 150}, /*  6:                    REM shift  150 */
  {  80,   0, 131}, /*  7:                 LSHIFT shift  131 */
  { 105,   0, 133}, /*  8:                 RSHIFT shift  133 */
  {  92,   7, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   8, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  83,   0, 144}, /* 12:                  MINUS shift  144 */
/* State 119 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 120}, /*  8:                   expr shift  120 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 120 */
  {  12,   0, 127}, /*  1:                 BITAND shift  127 */
  { 109,   5, 148}, /*  2:                  SLASH shift  148 */
  {  14,   0, 129}, /*  3:                  BITOR shift  129 */
  { 111,   6, 146}, /*  4:                   STAR shift  146 */
  {  25,   0, 152}, /*  5:                 CONCAT shift  152 */
  {  99,   0, 150}, /*  6:                    REM shift  150 */
  {  80,   0, 131}, /*  7:                 LSHIFT shift  131 */
  { 105,   0, 133}, /*  8:                 RSHIFT shift  133 */
  {  92,   7, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   8, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  83,   0, 144}, /* 12:                  MINUS shift  144 */
/* State 121 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 122}, /*  8:                   expr shift  122 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 122 */
  {  12,   0, 127}, /*  1:                 BITAND shift  127 */
  { 109,   5, 148}, /*  2:                  SLASH shift  148 */
  {  14,   0, 129}, /*  3:                  BITOR shift  129 */
  { 111,   6, 146}, /*  4:                   STAR shift  146 */
  {  25,   0, 152}, /*  5:                 CONCAT shift  152 */
  {  99,   0, 150}, /*  6:                    REM shift  150 */
  {  80,   0, 131}, /*  7:                 LSHIFT shift  131 */
  { 105,   0, 133}, /*  8:                 RSHIFT shift  133 */
  {  92,   7, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   8, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  83,   0, 144}, /* 12:                  MINUS shift  144 */
/* State 123 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 124}, /*  8:                   expr shift  124 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 124 */
  {  80,   0, 131}, /*  1:                 LSHIFT shift  131 */
  { 177,   3, 135}, /*  2:                 likeop shift  135 */
  {  81,   0, 115}, /*  3:                     LT shift  115 */
  {  99,   5, 150}, /*  4:                    REM shift  150 */
  {  83,   0, 144}, /*  5:                  MINUS shift  144 */
  {  57,   8, 117}, /*  6:                     GT shift  117 */
  {  54,   0, 121}, /*  7:                     GE shift  121 */
  {  25,   0, 152}, /*  8:                 CONCAT shift  152 */
  {  76,  11, 119}, /*  9:                     LE shift  119 */
  { 105,   6, 133}, /* 10:                 RSHIFT shift  133 */
  {  12,   0, 127}, /* 11:                 BITAND shift  127 */
  {  14,   0, 129}, /* 12:                  BITOR shift  129 */
  {  92,   9, 104}, /* 13:      ORACLE_OUTER_JOIN shift  104 */
  { 109,   0, 148}, /* 14:                  SLASH shift  148 */
  {  94,  12, 142}, /* 15:                   PLUS shift  142 */
  { 111,   0, 146}, /* 16:                   STAR shift  146 */
/* State 125 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 126}, /*  8:                   expr shift  126 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 126 */
  {  80,   0, 131}, /*  1:                 LSHIFT shift  131 */
  { 177,   3, 135}, /*  2:                 likeop shift  135 */
  {  81,   0, 115}, /*  3:                     LT shift  115 */
  {  99,   5, 150}, /*  4:                    REM shift  150 */
  {  83,   0, 144}, /*  5:                  MINUS shift  144 */
  {  57,   8, 117}, /*  6:                     GT shift  117 */
  {  54,   0, 121}, /*  7:                     GE shift  121 */
  {  25,   0, 152}, /*  8:                 CONCAT shift  152 */
  {  76,  11, 119}, /*  9:                     LE shift  119 */
  { 105,   6, 133}, /* 10:                 RSHIFT shift  133 */
  {  12,   0, 127}, /* 11:                 BITAND shift  127 */
  {  14,   0, 129}, /* 12:                  BITOR shift  129 */
  {  92,   9, 104}, /* 13:      ORACLE_OUTER_JOIN shift  104 */
  { 109,   0, 148}, /* 14:                  SLASH shift  148 */
  {  94,  12, 142}, /* 15:                   PLUS shift  142 */
  { 111,   0, 146}, /* 16:                   STAR shift  146 */
/* State 127 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 128}, /*  8:                   expr shift  128 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 128 */
  {  25,   0, 152}, /*  1:                 CONCAT shift  152 */
  { 177,   1, 135}, /*  2:                 likeop shift  135 */
  {  83,   0, 144}, /*  3:                  MINUS shift  144 */
  {  99,   3, 150}, /*  4:                    REM shift  150 */
  {  92,   0, 104}, /*  5:      ORACLE_OUTER_JOIN shift  104 */
  { 109,   0, 148}, /*  6:                  SLASH shift  148 */
  {  94,   0, 142}, /*  7:                   PLUS shift  142 */
  { 111,   0, 146}, /*  8:                   STAR shift  146 */
/* State 129 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 130}, /*  8:                   expr shift  130 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 130 */
  {  25,   0, 152}, /*  1:                 CONCAT shift  152 */
  { 177,   1, 135}, /*  2:                 likeop shift  135 */
  {  83,   0, 144}, /*  3:                  MINUS shift  144 */
  {  99,   3, 150}, /*  4:                    REM shift  150 */
  {  92,   0, 104}, /*  5:      ORACLE_OUTER_JOIN shift  104 */
  { 109,   0, 148}, /*  6:                  SLASH shift  148 */
  {  94,   0, 142}, /*  7:                   PLUS shift  142 */
  { 111,   0, 146}, /*  8:                   STAR shift  146 */
/* State 131 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 132}, /*  8:                   expr shift  132 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 132 */
  {  25,   0, 152}, /*  1:                 CONCAT shift  152 */
  { 177,   1, 135}, /*  2:                 likeop shift  135 */
  {  83,   0, 144}, /*  3:                  MINUS shift  144 */
  {  99,   3, 150}, /*  4:                    REM shift  150 */
  {  92,   0, 104}, /*  5:      ORACLE_OUTER_JOIN shift  104 */
  { 109,   0, 148}, /*  6:                  SLASH shift  148 */
  {  94,   0, 142}, /*  7:                   PLUS shift  142 */
  { 111,   0, 146}, /*  8:                   STAR shift  146 */
/* State 133 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 134}, /*  8:                   expr shift  134 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 134 */
  {  25,   0, 152}, /*  1:                 CONCAT shift  152 */
  { 177,   1, 135}, /*  2:                 likeop shift  135 */
  {  83,   0, 144}, /*  3:                  MINUS shift  144 */
  {  99,   3, 150}, /*  4:                    REM shift  150 */
  {  92,   0, 104}, /*  5:      ORACLE_OUTER_JOIN shift  104 */
  { 109,   0, 148}, /*  6:                  SLASH shift  148 */
  {  94,   0, 142}, /*  7:                   PLUS shift  142 */
  { 111,   0, 146}, /*  8:                   STAR shift  146 */
/* State 135 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 136}, /*  8:                   expr shift  136 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 136 */
  {  80,   0, 131}, /*  1:                 LSHIFT shift  131 */
  { 177,   3, 135}, /*  2:                 likeop shift  135 */
  {  81,   0, 115}, /*  3:                     LT shift  115 */
  {  99,   5, 150}, /*  4:                    REM shift  150 */
  {  83,   0, 144}, /*  5:                  MINUS shift  144 */
  {  57,   8, 117}, /*  6:                     GT shift  117 */
  {  54,   0, 121}, /*  7:                     GE shift  121 */
  {  25,   0, 152}, /*  8:                 CONCAT shift  152 */
  {  76,  11, 119}, /*  9:                     LE shift  119 */
  { 105,   6, 133}, /* 10:                 RSHIFT shift  133 */
  {  12,   0, 127}, /* 11:                 BITAND shift  127 */
  {  14,   0, 129}, /* 12:                  BITOR shift  129 */
  {  92,   9, 104}, /* 13:      ORACLE_OUTER_JOIN shift  104 */
  { 109,   0, 148}, /* 14:                  SLASH shift  148 */
  {  94,  12, 142}, /* 15:                   PLUS shift  142 */
  { 111,   0, 146}, /* 16:                   STAR shift  146 */
/* State 137 */
  {  87,   3, 220}, /*  1:                   NULL shift  220 */
  {  55,   0, 141}, /*  2:                   GLOB shift  141 */
  {  63,   0, 225}, /*  3:                     IN shift  225 */
  { 177,   1, 138}, /*  4:                 likeop shift  138 */
  {  11,   0, 221}, /*  5:                BETWEEN shift  221 */
  {  77,   5, 140}, /*  6:                   LIKE shift  140 */
/* State 138 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 139}, /*  8:                   expr shift  139 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 139 */
  {  80,   0, 131}, /*  1:                 LSHIFT shift  131 */
  { 177,   3, 135}, /*  2:                 likeop shift  135 */
  {  81,   0, 115}, /*  3:                     LT shift  115 */
  {  99,   5, 150}, /*  4:                    REM shift  150 */
  {  83,   0, 144}, /*  5:                  MINUS shift  144 */
  {  57,   8, 117}, /*  6:                     GT shift  117 */
  {  54,   0, 121}, /*  7:                     GE shift  121 */
  {  25,   0, 152}, /*  8:                 CONCAT shift  152 */
  {  76,  11, 119}, /*  9:                     LE shift  119 */
  { 105,   6, 133}, /* 10:                 RSHIFT shift  133 */
  {  12,   0, 127}, /* 11:                 BITAND shift  127 */
  {  14,   0, 129}, /* 12:                  BITOR shift  129 */
  {  92,   9, 104}, /* 13:      ORACLE_OUTER_JOIN shift  104 */
  { 109,   0, 148}, /* 14:                  SLASH shift  148 */
  {  94,  12, 142}, /* 15:                   PLUS shift  142 */
  { 111,   0, 146}, /* 16:                   STAR shift  146 */
/* State 142 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 143}, /*  8:                   expr shift  143 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 143 */
  {  25,   0, 152}, /*  1:                 CONCAT shift  152 */
  { 109,   1, 148}, /*  2:                  SLASH shift  148 */
  {  92,   0, 104}, /*  3:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   5, 135}, /*  4:                 likeop shift  135 */
  { 111,   6, 146}, /*  5:                   STAR shift  146 */
  {  99,   0, 150}, /*  6:                    REM shift  150 */
/* State 144 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 145}, /*  8:                   expr shift  145 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 145 */
  {  25,   0, 152}, /*  1:                 CONCAT shift  152 */
  { 109,   1, 148}, /*  2:                  SLASH shift  148 */
  {  92,   0, 104}, /*  3:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   5, 135}, /*  4:                 likeop shift  135 */
  { 111,   6, 146}, /*  5:                   STAR shift  146 */
  {  99,   0, 150}, /*  6:                    REM shift  150 */
/* State 146 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 147}, /*  8:                   expr shift  147 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 147 */
  { 177,   0, 135}, /*  1:                 likeop shift  135 */
  {  25,   0, 152}, /*  2:                 CONCAT shift  152 */
  {  92,   0, 104}, /*  3:      ORACLE_OUTER_JOIN shift  104 */
/* State 148 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 149}, /*  8:                   expr shift  149 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 149 */
  { 177,   0, 135}, /*  1:                 likeop shift  135 */
  {  25,   0, 152}, /*  2:                 CONCAT shift  152 */
  {  92,   0, 104}, /*  3:      ORACLE_OUTER_JOIN shift  104 */
/* State 150 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 151}, /*  8:                   expr shift  151 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 151 */
  { 177,   0, 135}, /*  1:                 likeop shift  135 */
  {  25,   0, 152}, /*  2:                 CONCAT shift  152 */
  {  92,   0, 104}, /*  3:      ORACLE_OUTER_JOIN shift  104 */
/* State 152 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 153}, /*  8:                   expr shift  153 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 153 */
  {  92,   0, 104}, /*  1:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /*  2:                 likeop shift  135 */
/* State 155 */
  {  85,   0, 157}, /*  1:                    NOT shift  157 */
  {  87,   1, 156}, /*  2:                   NULL shift  156 */
/* State 157 */
  {  87,   0, 158}, /*  1:                   NULL shift  158 */
/* State 160 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 161}, /*  8:                   expr shift  161 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 161 */
  {  84,   0, 123}, /*  1:                     NE shift  123 */
  {  85,   4, 137}, /*  2:                    NOT shift  137 */
  {  86,   0, 159}, /*  3:                NOTNULL shift  159 */
  {  57,   0, 117}, /*  4:                     GT shift  117 */
  {  63,   0, 164}, /*  5:                     IN shift  164 */
  {   5,   0, 162}, /*  6:                    AND shift  162 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  91,   5, 113}, /*  8:                     OR shift  113 */
  {  92,   0, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  77,   0, 140}, /* 14:                   LIKE shift  140 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  {  99,   7, 150}, /* 16:                    REM shift  150 */
  {  72,   0, 154}, /* 17:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 18:                     EQ shift  125 */
  {  81,  20, 115}, /* 19:                     LT shift  115 */
  {  25,   0, 152}, /* 20:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 21:                     LE shift  119 */
  { 105,  14, 133}, /* 22:                 RSHIFT shift  133 */
  {  83,  24, 144}, /* 23:                  MINUS shift  144 */
  {  55,   0, 141}, /* 24:                   GLOB shift  141 */
  {  80,   0, 131}, /* 25:                 LSHIFT shift  131 */
  { 109,  19, 148}, /* 26:                  SLASH shift  148 */
  {  54,   0, 121}, /* 27:                     GE shift  121 */
  { 111,  23, 146}, /* 28:                   STAR shift  146 */
/* State 162 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 163}, /*  8:                   expr shift  163 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 163 */
  {  80,   0, 131}, /*  1:                 LSHIFT shift  131 */
  { 177,   3, 135}, /*  2:                 likeop shift  135 */
  {  81,   0, 115}, /*  3:                     LT shift  115 */
  {  99,   5, 150}, /*  4:                    REM shift  150 */
  {  83,   0, 144}, /*  5:                  MINUS shift  144 */
  {  57,   8, 117}, /*  6:                     GT shift  117 */
  {  54,   0, 121}, /*  7:                     GE shift  121 */
  {  25,   0, 152}, /*  8:                 CONCAT shift  152 */
  {  76,  11, 119}, /*  9:                     LE shift  119 */
  { 105,   6, 133}, /* 10:                 RSHIFT shift  133 */
  {  12,   0, 127}, /* 11:                 BITAND shift  127 */
  {  14,   0, 129}, /* 12:                  BITOR shift  129 */
  {  92,   9, 104}, /* 13:      ORACLE_OUTER_JOIN shift  104 */
  { 109,   0, 148}, /* 14:                  SLASH shift  148 */
  {  94,  12, 142}, /* 15:                   PLUS shift  142 */
  { 111,   0, 146}, /* 16:                   STAR shift  146 */
/* State 164 */
  {  79,   0, 165}, /*  1:                     LP shift  165 */
/* State 165 */
  {  85,   0, 175}, /*  1:                    NOT shift  175 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  13,   0, 177}, /*  3:                 BITNOT shift  177 */
  {  83,   0, 179}, /*  4:                  MINUS shift  179 */
  {  74,   0,  67}, /*  5:                JOIN_KW shift  67 */
  { 185,   1,  69}, /*  6:              oneselect shift  69 */
  { 106,   0,  73}, /*  7:                 SELECT shift  73 */
  {  87,   0, 106}, /*  8:                   NULL shift  106 */
  {  68,   0, 173}, /*  9:                INTEGER shift  173 */
  {  49,   0, 174}, /* 10:                  FLOAT shift  174 */
  {  97,  12, 193}, /* 11:                  RAISE shift  193 */
  {  17,   0, 183}, /* 12:                   CASE shift  183 */
  {  79,  16,  68}, /* 13:                     LP shift  68 */
  { 113,   3,  66}, /* 14:                 STRING shift  66 */
  {  94,   5, 181}, /* 15:                   PLUS shift  181 */
  {  59,   0,  64}, /* 16:                     ID shift  64 */
  { 196,   0, 166}, /* 17:                 select shift  166 */
  { 157,  11, 172}, /* 18:                   expr shift  172 */
  { 158,   0, 219}, /* 19:               expritem shift  219 */
  { 159,  13, 215}, /* 20:               exprlist shift  215 */
/* State 166 */
  { 180,   0,  71}, /*  1:         multiselect_op shift  71 */
  { 121,   3, 167}, /*  2:                  UNION shift  167 */
  {  46,   0, 170}, /*  3:                 EXCEPT shift  170 */
  {  69,   0, 169}, /*  4:              INTERSECT shift  169 */
  { 104,   4, 171}, /*  5:                     RP shift  171 */
/* State 167 */
  { 106,   2, 658}, /*  1:                 SELECT reduce 103 */
  {   4,   0, 168}, /*  2:                    ALL shift  168 */
/* State 168 */
  { 106,   0, 659}, /*  1:                 SELECT reduce 104 */
/* State 169 */
  { 106,   0, 660}, /*  1:                 SELECT reduce 105 */
/* State 170 */
  { 106,   0, 661}, /*  1:                 SELECT reduce 106 */
/* State 172 */
  {  84,   0, 123}, /*  1:                     NE shift  123 */
  {  85,   4, 137}, /*  2:                    NOT shift  137 */
  {  86,   0, 159}, /*  3:                NOTNULL shift  159 */
  {  57,   0, 117}, /*  4:                     GT shift  117 */
  {  63,   0, 164}, /*  5:                     IN shift  164 */
  {   5,   0, 105}, /*  6:                    AND shift  105 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  91,   5, 113}, /*  8:                     OR shift  113 */
  {  92,   0, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  77,   0, 140}, /* 14:                   LIKE shift  140 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  {  99,   7, 150}, /* 16:                    REM shift  150 */
  {  72,   0, 154}, /* 17:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 18:                     EQ shift  125 */
  {  81,  20, 115}, /* 19:                     LT shift  115 */
  {  25,   0, 152}, /* 20:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 21:                     LE shift  119 */
  { 105,  14, 133}, /* 22:                 RSHIFT shift  133 */
  {  83,  24, 144}, /* 23:                  MINUS shift  144 */
  {  55,   0, 141}, /* 24:                   GLOB shift  141 */
  {  80,   0, 131}, /* 25:                 LSHIFT shift  131 */
  { 109,  19, 148}, /* 26:                  SLASH shift  148 */
  {  54,   0, 121}, /* 27:                     GE shift  121 */
  { 111,  23, 146}, /* 28:                   STAR shift  146 */
/* State 175 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 176}, /*  8:                   expr shift  176 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 176 */
  {  54,   0, 121}, /*  1:                     GE shift  121 */
  { 105,   0, 133}, /*  2:                 RSHIFT shift  133 */
  {  80,   1, 131}, /*  3:                 LSHIFT shift  131 */
  {  81,   5, 115}, /*  4:                     LT shift  115 */
  {  55,   0, 141}, /*  5:                   GLOB shift  141 */
  { 109,  10, 148}, /*  6:                  SLASH shift  148 */
  {  84,   0, 123}, /*  7:                     NE shift  123 */
  { 111,  11, 146}, /*  8:                   STAR shift  146 */
  {  86,   0, 159}, /*  9:                NOTNULL shift  159 */
  {  83,  14, 144}, /* 10:                  MINUS shift  144 */
  {  85,   0, 137}, /* 11:                    NOT shift  137 */
  {  63,  16, 164}, /* 12:                     IN shift  164 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  57,   0, 117}, /* 14:                     GT shift  117 */
  {  92,  18, 104}, /* 15:      ORACLE_OUTER_JOIN shift  104 */
  {  11,   0, 160}, /* 16:                BETWEEN shift  160 */
  {  94,   0, 142}, /* 17:                   PLUS shift  142 */
  {  14,   0, 129}, /* 18:                  BITOR shift  129 */
  {  45,   0, 125}, /* 19:                     EQ shift  125 */
  {  71,  19, 155}, /* 20:                     IS shift  155 */
  {  72,   0, 154}, /* 21:                 ISNULL shift  154 */
  { 177,  23, 135}, /* 22:                 likeop shift  135 */
  {  99,   0, 150}, /* 23:                    REM shift  150 */
  {  25,   0, 152}, /* 24:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 25:                     LE shift  119 */
  {  77,  24, 140}, /* 26:                   LIKE shift  140 */
/* State 177 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 178}, /*  8:                   expr shift  178 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 178 */
  {  92,   0, 104}, /*  1:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /*  2:                 likeop shift  135 */
/* State 179 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 180}, /*  8:                   expr shift  180 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 180 */
  {  92,   0, 104}, /*  1:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /*  2:                 likeop shift  135 */
/* State 181 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 182}, /*  8:                   expr shift  182 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 182 */
  {  92,   0, 104}, /*  1:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /*  2:                 likeop shift  135 */
/* State 183 */
  { 136,   2, 185}, /*  1:           case_operand shift  185 */
  {  85,   4, 175}, /*  2:                    NOT shift  175 */
  {  87,   0, 106}, /*  3:                   NULL shift  106 */
  {  68,   6, 173}, /*  4:                INTEGER shift  173 */
  { 157,   0, 184}, /*  5:                   expr shift  184 */
  {  17,   0, 183}, /*  6:                   CASE shift  183 */
  {  74,   0,  67}, /*  7:                JOIN_KW shift  67 */
  { 113,  15,  66}, /*  8:                 STRING shift  66 */
  {  59,   0,  64}, /*  9:                     ID shift  64 */
  {  94,   0, 181}, /* 10:                   PLUS shift  181 */
  { 129,   0, 781}, /* 11:                   WHEN reduce 226 */
  { 181,   8, 107}, /* 12:                     nm shift  107 */
  {  97,   0, 193}, /* 13:                  RAISE shift  193 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  79,   0,  68}, /* 15:                     LP shift  68 */
  {  83,  17, 179}, /* 16:                  MINUS shift  179 */
  {  49,   0, 174}, /* 17:                  FLOAT shift  174 */
/* State 184 */
  {  63,   2, 164}, /*  1:                     IN shift  164 */
  {   5,   0, 105}, /*  2:                    AND shift  105 */
  {  12,   0, 127}, /*  3:                 BITAND shift  127 */
  { 177,   0, 135}, /*  4:                 likeop shift  135 */
  {  91,   0, 113}, /*  5:                     OR shift  113 */
  {  92,   1, 104}, /*  6:      ORACLE_OUTER_JOIN shift  104 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  94,   0, 142}, /*  8:                   PLUS shift  142 */
  {  14,   0, 129}, /*  9:                  BITOR shift  129 */
  {  76,   0, 119}, /* 10:                     LE shift  119 */
  {  80,   0, 131}, /* 11:                 LSHIFT shift  131 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  99,   3, 150}, /* 13:                    REM shift  150 */
  { 129,   7, 780}, /* 14:                   WHEN reduce 225 */
  {  72,   9, 154}, /* 15:                 ISNULL shift  154 */
  {  54,  18, 121}, /* 16:                     GE shift  121 */
  {  45,   0, 125}, /* 17:                     EQ shift  125 */
  {  25,   0, 152}, /* 18:                 CONCAT shift  152 */
  { 105,  10, 133}, /* 19:                 RSHIFT shift  133 */
  {  77,   0, 140}, /* 20:                   LIKE shift  140 */
  {  55,   0, 141}, /* 21:                   GLOB shift  141 */
  {  57,   0, 117}, /* 22:                     GT shift  117 */
  { 109,  11, 148}, /* 23:                  SLASH shift  148 */
  {  81,   0, 115}, /* 24:                     LT shift  115 */
  { 111,   0, 146}, /* 25:                   STAR shift  146 */
  {  83,  16, 144}, /* 26:                  MINUS shift  144 */
  {  84,  21, 123}, /* 27:                     NE shift  123 */
  {  85,   0, 137}, /* 28:                    NOT shift  137 */
  {  86,  22, 159}, /* 29:                NOTNULL shift  159 */
/* State 185 */
  { 129,   0, 211}, /*  1:                   WHEN shift  211 */
  { 135,   1, 186}, /*  2:          case_exprlist shift  186 */
/* State 186 */
  {  42,   0, 209}, /*  1:                   ELSE shift  209 */
  { 129,   0, 189}, /*  2:                   WHEN shift  189 */
  { 134,   1, 187}, /*  3:              case_else shift  187 */
  {  43,   0, 779}, /*  4:                    END reduce 224 */
/* State 187 */
  {  43,   0, 188}, /*  1:                    END shift  188 */
/* State 189 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 190}, /*  8:                   expr shift  190 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 190 */
  { 116,   0, 191}, /*  1:                   THEN shift  191 */
  {  63,   3, 164}, /*  2:                     IN shift  164 */
  {   5,   0, 105}, /*  3:                    AND shift  105 */
  { 177,   0, 135}, /*  4:                 likeop shift  135 */
  {  91,   0, 113}, /*  5:                     OR shift  113 */
  {  92,   2, 104}, /*  6:      ORACLE_OUTER_JOIN shift  104 */
  {  12,   0, 127}, /*  7:                 BITAND shift  127 */
  {  94,   0, 142}, /*  8:                   PLUS shift  142 */
  {  14,   0, 129}, /*  9:                  BITOR shift  129 */
  {  76,   0, 119}, /* 10:                     LE shift  119 */
  {  80,   0, 131}, /* 11:                 LSHIFT shift  131 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  99,   7, 150}, /* 13:                    REM shift  150 */
  {  71,   0, 155}, /* 14:                     IS shift  155 */
  {  72,   9, 154}, /* 15:                 ISNULL shift  154 */
  {  54,  18, 121}, /* 16:                     GE shift  121 */
  {  45,   0, 125}, /* 17:                     EQ shift  125 */
  {  25,   0, 152}, /* 18:                 CONCAT shift  152 */
  { 105,  10, 133}, /* 19:                 RSHIFT shift  133 */
  {  77,   0, 140}, /* 20:                   LIKE shift  140 */
  {  55,   0, 141}, /* 21:                   GLOB shift  141 */
  {  57,   0, 117}, /* 22:                     GT shift  117 */
  { 109,  11, 148}, /* 23:                  SLASH shift  148 */
  {  81,   0, 115}, /* 24:                     LT shift  115 */
  { 111,   0, 146}, /* 25:                   STAR shift  146 */
  {  83,  16, 144}, /* 26:                  MINUS shift  144 */
  {  84,  21, 123}, /* 27:                     NE shift  123 */
  {  85,   0, 137}, /* 28:                    NOT shift  137 */
  {  86,  22, 159}, /* 29:                NOTNULL shift  159 */
/* State 191 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 192}, /*  8:                   expr shift  192 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 192 */
  {  84,   0, 123}, /*  1:                     NE shift  123 */
  {  85,   4, 137}, /*  2:                    NOT shift  137 */
  {  86,   0, 159}, /*  3:                NOTNULL shift  159 */
  {  57,   0, 117}, /*  4:                     GT shift  117 */
  {  63,   0, 164}, /*  5:                     IN shift  164 */
  {   5,   0, 105}, /*  6:                    AND shift  105 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  91,   5, 113}, /*  8:                     OR shift  113 */
  {  92,   0, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  77,   0, 140}, /* 14:                   LIKE shift  140 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  {  99,   7, 150}, /* 16:                    REM shift  150 */
  {  72,   0, 154}, /* 17:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 18:                     EQ shift  125 */
  {  81,  20, 115}, /* 19:                     LT shift  115 */
  {  25,   0, 152}, /* 20:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 21:                     LE shift  119 */
  { 105,  14, 133}, /* 22:                 RSHIFT shift  133 */
  {  83,  24, 144}, /* 23:                  MINUS shift  144 */
  {  55,   0, 141}, /* 24:                   GLOB shift  141 */
  {  80,   0, 131}, /* 25:                 LSHIFT shift  131 */
  { 109,  19, 148}, /* 26:                  SLASH shift  148 */
  {  54,   0, 121}, /* 27:                     GE shift  121 */
  { 111,  23, 146}, /* 28:                   STAR shift  146 */
/* State 193 */
  {  79,   0, 194}, /*  1:                     LP shift  194 */
/* State 194 */
  {  60,   4, 195}, /*  1:                 IGNORE shift  195 */
  {   1,   0, 201}, /*  2:                  ABORT shift  201 */
  { 102,   0, 197}, /*  3:               ROLLBACK shift  197 */
  {  48,   0, 205}, /*  4:                   FAIL shift  205 */
/* State 195 */
  { 104,   0, 196}, /*  1:                     RP shift  196 */
/* State 197 */
  {  22,   0, 198}, /*  1:                  COMMA shift  198 */
/* State 198 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 199}, /*  2:                     nm shift  199 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 199 */
  { 104,   0, 200}, /*  1:                     RP shift  200 */
/* State 201 */
  {  22,   0, 202}, /*  1:                  COMMA shift  202 */
/* State 202 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 203}, /*  2:                     nm shift  203 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 203 */
  { 104,   0, 204}, /*  1:                     RP shift  204 */
/* State 205 */
  {  22,   0, 206}, /*  1:                  COMMA shift  206 */
/* State 206 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 207}, /*  2:                     nm shift  207 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 207 */
  { 104,   0, 208}, /*  1:                     RP shift  208 */
/* State 209 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 210}, /*  8:                   expr shift  210 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 210 */
  {  63,   2, 164}, /*  1:                     IN shift  164 */
  {   5,   0, 105}, /*  2:                    AND shift  105 */
  {  12,   0, 127}, /*  3:                 BITAND shift  127 */
  { 177,   0, 135}, /*  4:                 likeop shift  135 */
  {  91,   0, 113}, /*  5:                     OR shift  113 */
  {  92,   1, 104}, /*  6:      ORACLE_OUTER_JOIN shift  104 */
  {  43,   9, 778}, /*  7:                    END reduce 223 */
  {  94,   0, 142}, /*  8:                   PLUS shift  142 */
  {  14,   0, 129}, /*  9:                  BITOR shift  129 */
  {  76,   0, 119}, /* 10:                     LE shift  119 */
  {  80,   0, 131}, /* 11:                 LSHIFT shift  131 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  99,   3, 150}, /* 13:                    REM shift  150 */
  {  71,   0, 155}, /* 14:                     IS shift  155 */
  {  72,   7, 154}, /* 15:                 ISNULL shift  154 */
  {  54,  18, 121}, /* 16:                     GE shift  121 */
  {  45,   0, 125}, /* 17:                     EQ shift  125 */
  {  25,   0, 152}, /* 18:                 CONCAT shift  152 */
  { 105,  10, 133}, /* 19:                 RSHIFT shift  133 */
  {  77,   0, 140}, /* 20:                   LIKE shift  140 */
  {  55,   0, 141}, /* 21:                   GLOB shift  141 */
  {  57,   0, 117}, /* 22:                     GT shift  117 */
  { 109,  11, 148}, /* 23:                  SLASH shift  148 */
  {  81,   0, 115}, /* 24:                     LT shift  115 */
  { 111,   0, 146}, /* 25:                   STAR shift  146 */
  {  83,  16, 144}, /* 26:                  MINUS shift  144 */
  {  84,  21, 123}, /* 27:                     NE shift  123 */
  {  85,   0, 137}, /* 28:                    NOT shift  137 */
  {  86,  22, 159}, /* 29:                NOTNULL shift  159 */
/* State 211 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 212}, /*  8:                   expr shift  212 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 212 */
  { 116,   0, 213}, /*  1:                   THEN shift  213 */
  {  63,   3, 164}, /*  2:                     IN shift  164 */
  {   5,   0, 105}, /*  3:                    AND shift  105 */
  { 177,   0, 135}, /*  4:                 likeop shift  135 */
  {  91,   0, 113}, /*  5:                     OR shift  113 */
  {  92,   2, 104}, /*  6:      ORACLE_OUTER_JOIN shift  104 */
  {  12,   0, 127}, /*  7:                 BITAND shift  127 */
  {  94,   0, 142}, /*  8:                   PLUS shift  142 */
  {  14,   0, 129}, /*  9:                  BITOR shift  129 */
  {  76,   0, 119}, /* 10:                     LE shift  119 */
  {  80,   0, 131}, /* 11:                 LSHIFT shift  131 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  99,   7, 150}, /* 13:                    REM shift  150 */
  {  71,   0, 155}, /* 14:                     IS shift  155 */
  {  72,   9, 154}, /* 15:                 ISNULL shift  154 */
  {  54,  18, 121}, /* 16:                     GE shift  121 */
  {  45,   0, 125}, /* 17:                     EQ shift  125 */
  {  25,   0, 152}, /* 18:                 CONCAT shift  152 */
  { 105,  10, 133}, /* 19:                 RSHIFT shift  133 */
  {  77,   0, 140}, /* 20:                   LIKE shift  140 */
  {  55,   0, 141}, /* 21:                   GLOB shift  141 */
  {  57,   0, 117}, /* 22:                     GT shift  117 */
  { 109,  11, 148}, /* 23:                  SLASH shift  148 */
  {  81,   0, 115}, /* 24:                     LT shift  115 */
  { 111,   0, 146}, /* 25:                   STAR shift  146 */
  {  83,  16, 144}, /* 26:                  MINUS shift  144 */
  {  84,  21, 123}, /* 27:                     NE shift  123 */
  {  85,   0, 137}, /* 28:                    NOT shift  137 */
  {  86,  22, 159}, /* 29:                NOTNULL shift  159 */
/* State 213 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 214}, /*  8:                   expr shift  214 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 214 */
  {  84,   0, 123}, /*  1:                     NE shift  123 */
  {  85,   4, 137}, /*  2:                    NOT shift  137 */
  {  86,   0, 159}, /*  3:                NOTNULL shift  159 */
  {  57,   0, 117}, /*  4:                     GT shift  117 */
  {  63,   0, 164}, /*  5:                     IN shift  164 */
  {   5,   0, 105}, /*  6:                    AND shift  105 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  91,   5, 113}, /*  8:                     OR shift  113 */
  {  92,   0, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  77,   0, 140}, /* 14:                   LIKE shift  140 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  {  99,   7, 150}, /* 16:                    REM shift  150 */
  {  72,   0, 154}, /* 17:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 18:                     EQ shift  125 */
  {  81,  20, 115}, /* 19:                     LT shift  115 */
  {  25,   0, 152}, /* 20:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 21:                     LE shift  119 */
  { 105,  14, 133}, /* 22:                 RSHIFT shift  133 */
  {  83,  24, 144}, /* 23:                  MINUS shift  144 */
  {  55,   0, 141}, /* 24:                   GLOB shift  141 */
  {  80,   0, 131}, /* 25:                 LSHIFT shift  131 */
  { 109,  19, 148}, /* 26:                  SLASH shift  148 */
  {  54,   0, 121}, /* 27:                     GE shift  121 */
  { 111,  23, 146}, /* 28:                   STAR shift  146 */
/* State 215 */
  { 104,   2, 216}, /*  1:                     RP shift  216 */
  {  22,   0, 217}, /*  2:                  COMMA shift  217 */
/* State 217 */
  {  97,   3, 193}, /*  1:                  RAISE shift  193 */
  { 113,   1,  66}, /*  2:                 STRING shift  66 */
  {  49,   7, 174}, /*  3:                  FLOAT shift  174 */
  {  83,   0, 179}, /*  4:                  MINUS shift  179 */
  {  68,   0, 173}, /*  5:                INTEGER shift  173 */
  { 181,   9, 107}, /*  6:                     nm shift  107 */
  {  17,   0, 183}, /*  7:                   CASE shift  183 */
  {  87,   0, 106}, /*  8:                   NULL shift  106 */
  {  85,   0, 175}, /*  9:                    NOT shift  175 */
  {  13,   0, 177}, /* 10:                 BITNOT shift  177 */
  {  74,   0,  67}, /* 11:                JOIN_KW shift  67 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  94,   0, 181}, /* 13:                   PLUS shift  181 */
  { 157,  10, 172}, /* 14:                   expr shift  172 */
  { 158,  13, 218}, /* 15:               expritem shift  218 */
  {  79,   0,  68}, /* 16:                     LP shift  68 */
/* State 221 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 222}, /*  8:                   expr shift  222 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 222 */
  {  84,   0, 123}, /*  1:                     NE shift  123 */
  {  85,   4, 137}, /*  2:                    NOT shift  137 */
  {  86,   0, 159}, /*  3:                NOTNULL shift  159 */
  {  57,   0, 117}, /*  4:                     GT shift  117 */
  {  63,   0, 164}, /*  5:                     IN shift  164 */
  {   5,   0, 223}, /*  6:                    AND shift  223 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  91,   5, 113}, /*  8:                     OR shift  113 */
  {  92,   0, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  77,   0, 140}, /* 14:                   LIKE shift  140 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  {  99,   7, 150}, /* 16:                    REM shift  150 */
  {  72,   0, 154}, /* 17:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 18:                     EQ shift  125 */
  {  81,  20, 115}, /* 19:                     LT shift  115 */
  {  25,   0, 152}, /* 20:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 21:                     LE shift  119 */
  { 105,  14, 133}, /* 22:                 RSHIFT shift  133 */
  {  83,  24, 144}, /* 23:                  MINUS shift  144 */
  {  55,   0, 141}, /* 24:                   GLOB shift  141 */
  {  80,   0, 131}, /* 25:                 LSHIFT shift  131 */
  { 109,  19, 148}, /* 26:                  SLASH shift  148 */
  {  54,   0, 121}, /* 27:                     GE shift  121 */
  { 111,  23, 146}, /* 28:                   STAR shift  146 */
/* State 223 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 224}, /*  8:                   expr shift  224 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 224 */
  {  54,   0, 121}, /*  1:                     GE shift  121 */
  { 105,   0, 133}, /*  2:                 RSHIFT shift  133 */
  {  80,   1, 131}, /*  3:                 LSHIFT shift  131 */
  {  81,   5, 115}, /*  4:                     LT shift  115 */
  {  55,   0, 141}, /*  5:                   GLOB shift  141 */
  { 109,  10, 148}, /*  6:                  SLASH shift  148 */
  {  84,   0, 123}, /*  7:                     NE shift  123 */
  { 111,  11, 146}, /*  8:                   STAR shift  146 */
  {  86,   0, 159}, /*  9:                NOTNULL shift  159 */
  {  83,  14, 144}, /* 10:                  MINUS shift  144 */
  {  85,   0, 137}, /* 11:                    NOT shift  137 */
  {  63,  16, 164}, /* 12:                     IN shift  164 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  57,   0, 117}, /* 14:                     GT shift  117 */
  {  92,  18, 104}, /* 15:      ORACLE_OUTER_JOIN shift  104 */
  {  11,   0, 160}, /* 16:                BETWEEN shift  160 */
  {  94,   0, 142}, /* 17:                   PLUS shift  142 */
  {  14,   0, 129}, /* 18:                  BITOR shift  129 */
  {  45,   0, 125}, /* 19:                     EQ shift  125 */
  {  71,  19, 155}, /* 20:                     IS shift  155 */
  {  72,   0, 154}, /* 21:                 ISNULL shift  154 */
  { 177,  23, 135}, /* 22:                 likeop shift  135 */
  {  99,   0, 150}, /* 23:                    REM shift  150 */
  {  25,   0, 152}, /* 24:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 25:                     LE shift  119 */
  {  77,  24, 140}, /* 26:                   LIKE shift  140 */
/* State 225 */
  {  79,   0, 226}, /*  1:                     LP shift  226 */
/* State 226 */
  {  85,   0, 175}, /*  1:                    NOT shift  175 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  13,   0, 177}, /*  3:                 BITNOT shift  177 */
  {  83,   0, 179}, /*  4:                  MINUS shift  179 */
  {  74,   0,  67}, /*  5:                JOIN_KW shift  67 */
  { 185,   1,  69}, /*  6:              oneselect shift  69 */
  { 106,   0,  73}, /*  7:                 SELECT shift  73 */
  {  87,   0, 106}, /*  8:                   NULL shift  106 */
  {  68,   0, 173}, /*  9:                INTEGER shift  173 */
  {  49,   0, 174}, /* 10:                  FLOAT shift  174 */
  {  97,  12, 193}, /* 11:                  RAISE shift  193 */
  {  17,   0, 183}, /* 12:                   CASE shift  183 */
  {  79,  16,  68}, /* 13:                     LP shift  68 */
  { 113,   3,  66}, /* 14:                 STRING shift  66 */
  {  94,   5, 181}, /* 15:                   PLUS shift  181 */
  {  59,   0,  64}, /* 16:                     ID shift  64 */
  { 196,   0, 227}, /* 17:                 select shift  227 */
  { 157,  11, 172}, /* 18:                   expr shift  172 */
  { 158,   0, 219}, /* 19:               expritem shift  219 */
  { 159,  13, 229}, /* 20:               exprlist shift  229 */
/* State 227 */
  { 180,   0,  71}, /*  1:         multiselect_op shift  71 */
  { 121,   3, 167}, /*  2:                  UNION shift  167 */
  {  46,   0, 170}, /*  3:                 EXCEPT shift  170 */
  {  69,   0, 169}, /*  4:              INTERSECT shift  169 */
  { 104,   4, 228}, /*  5:                     RP shift  228 */
/* State 229 */
  { 104,   2, 230}, /*  1:                     RP shift  230 */
  {  22,   0, 217}, /*  2:                  COMMA shift  217 */
/* State 231 */
  {  20,   0, 100}, /*  1:                COLLATE shift  100 */
  { 141,   0, 232}, /*  2:                collate shift  232 */
/* State 232 */
  {  36,   0,  59}, /*  1:                   DESC shift  59 */
  { 202,   3, 233}, /*  2:              sortorder shift  233 */
  {   7,   0,  58}, /*  3:                    ASC shift  58 */
/* State 234 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 235}, /*  8:                   expr shift  235 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 235 */
  {  84,   0, 123}, /*  1:                     NE shift  123 */
  {  85,   4, 137}, /*  2:                    NOT shift  137 */
  {  86,   0, 159}, /*  3:                NOTNULL shift  159 */
  {  57,   0, 117}, /*  4:                     GT shift  117 */
  {  63,   0, 164}, /*  5:                     IN shift  164 */
  {   5,   0, 105}, /*  6:                    AND shift  105 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  91,   5, 113}, /*  8:                     OR shift  113 */
  {  92,   0, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  77,   0, 140}, /* 14:                   LIKE shift  140 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  {  99,   7, 150}, /* 16:                    REM shift  150 */
  {  72,   0, 154}, /* 17:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 18:                     EQ shift  125 */
  {  81,  20, 115}, /* 19:                     LT shift  115 */
  {  25,   0, 152}, /* 20:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 21:                     LE shift  119 */
  { 105,  14, 133}, /* 22:                 RSHIFT shift  133 */
  {  83,  24, 144}, /* 23:                  MINUS shift  144 */
  {  55,   0, 141}, /* 24:                   GLOB shift  141 */
  {  80,   0, 131}, /* 25:                 LSHIFT shift  131 */
  { 109,  19, 148}, /* 26:                  SLASH shift  148 */
  {  54,   0, 121}, /* 27:                     GE shift  121 */
  { 111,  23, 146}, /* 28:                   STAR shift  146 */
/* State 236 */
  {  15,   0, 237}, /*  1:                     BY shift  237 */
/* State 237 */
  {  85,   2, 175}, /*  1:                    NOT shift  175 */
  {  68,   4, 173}, /*  2:                INTEGER shift  173 */
  {  87,   0, 106}, /*  3:                   NULL shift  106 */
  {  17,   0, 183}, /*  4:                   CASE shift  183 */
  { 157,   0, 172}, /*  5:                   expr shift  172 */
  { 158,   0, 219}, /*  6:               expritem shift  219 */
  { 159,   8, 238}, /*  7:               exprlist shift  238 */
  {  74,   0,  67}, /*  8:                JOIN_KW shift  67 */
  {  59,   0,  64}, /*  9:                     ID shift  64 */
  {  94,   0, 181}, /* 10:                   PLUS shift  181 */
  { 113,  15,  66}, /* 11:                 STRING shift  66 */
  { 181,  11, 107}, /* 12:                     nm shift  107 */
  {  97,   0, 193}, /* 13:                  RAISE shift  193 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  79,   0,  68}, /* 15:                     LP shift  68 */
  {  83,  17, 179}, /* 16:                  MINUS shift  179 */
  {  49,   0, 174}, /* 17:                  FLOAT shift  174 */
/* State 238 */
  {  22,   0, 217}, /*  1:                  COMMA shift  217 */
/* State 239 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 240}, /*  8:                   expr shift  240 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 240 */
  {  84,   0, 123}, /*  1:                     NE shift  123 */
  {  85,   4, 137}, /*  2:                    NOT shift  137 */
  {  86,   0, 159}, /*  3:                NOTNULL shift  159 */
  {  57,   0, 117}, /*  4:                     GT shift  117 */
  {  63,   0, 164}, /*  5:                     IN shift  164 */
  {   5,   0, 105}, /*  6:                    AND shift  105 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  91,   5, 113}, /*  8:                     OR shift  113 */
  {  92,   0, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  77,   0, 140}, /* 14:                   LIKE shift  140 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  {  99,   7, 150}, /* 16:                    REM shift  150 */
  {  72,   0, 154}, /* 17:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 18:                     EQ shift  125 */
  {  81,  20, 115}, /* 19:                     LT shift  115 */
  {  25,   0, 152}, /* 20:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 21:                     LE shift  119 */
  { 105,  14, 133}, /* 22:                 RSHIFT shift  133 */
  {  83,  24, 144}, /* 23:                  MINUS shift  144 */
  {  55,   0, 141}, /* 24:                   GLOB shift  141 */
  {  80,   0, 131}, /* 25:                 LSHIFT shift  131 */
  { 109,  19, 148}, /* 26:                  SLASH shift  148 */
  {  54,   0, 121}, /* 27:                     GE shift  121 */
  { 111,  23, 146}, /* 28:                   STAR shift  146 */
/* State 242 */
  { 197,   0, 243}, /*  1:             seltablist shift  243 */
  { 203,   1, 253}, /*  2:             stl_prefix shift  253 */
/* State 243 */
  {  22,   0, 245}, /*  1:                  COMMA shift  245 */
  {  73,   0, 246}, /*  2:                   JOIN shift  246 */
  {  74,   1, 247}, /*  3:                JOIN_KW shift  247 */
  { 175,   0, 244}, /*  4:                 joinop shift  244 */
/* State 247 */
  {  73,   0, 248}, /*  1:                   JOIN shift  248 */
  { 181,   0, 249}, /*  2:                     nm shift  249 */
  {  59,   0,  20}, /*  3:                     ID shift  20 */
  { 113,   1,  21}, /*  4:                 STRING shift  21 */
  {  74,   3,  22}, /*  5:                JOIN_KW shift  22 */
/* State 249 */
  {  73,   0, 250}, /*  1:                   JOIN shift  250 */
  { 181,   0, 251}, /*  2:                     nm shift  251 */
  {  59,   0,  20}, /*  3:                     ID shift  20 */
  { 113,   1,  21}, /*  4:                 STRING shift  21 */
  {  74,   3,  22}, /*  5:                JOIN_KW shift  22 */
/* State 251 */
  {  73,   0, 252}, /*  1:                   JOIN shift  252 */
/* State 253 */
  {  74,   3,  22}, /*  1:                JOIN_KW shift  22 */
  { 181,   0, 254}, /*  2:                     nm shift  254 */
  {  59,   0,  20}, /*  3:                     ID shift  20 */
  { 113,   0,  21}, /*  4:                 STRING shift  21 */
  {  79,   1, 277}, /*  5:                     LP shift  277 */
/* State 254 */
  { 150,   0, 255}, /*  1:                   dbnm shift  255 */
  {  39,   0, 275}, /*  2:                    DOT shift  275 */
/* State 255 */
  { 165,   0, 260}, /*  1:                    ids shift  260 */
  { 131,   3, 261}, /*  2:                     as shift  261 */
  {   6,   0, 258}, /*  3:                     AS shift  258 */
  { 113,   0, 257}, /*  4:                 STRING shift  257 */
  {  59,   0, 256}, /*  5:                     ID shift  256 */
/* State 258 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 259}, /*  2:                     nm shift  259 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 261 */
  {  90,   0, 273}, /*  1:                     ON shift  273 */
  { 183,   0, 262}, /*  2:                 on_opt shift  262 */
/* State 262 */
  { 125,   0, 264}, /*  1:                  USING shift  264 */
  { 215,   1, 263}, /*  2:              using_opt shift  263 */
/* State 264 */
  {  79,   0, 265}, /*  1:                     LP shift  265 */
/* State 265 */
  { 113,   4,  21}, /*  1:                 STRING shift  21 */
  { 181,   0, 270}, /*  2:                     nm shift  270 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
  { 166,   0, 272}, /*  5:                idxitem shift  272 */
  { 167,   1, 266}, /*  6:                idxlist shift  266 */
/* State 266 */
  { 104,   2, 267}, /*  1:                     RP shift  267 */
  {  22,   0, 268}, /*  2:                  COMMA shift  268 */
/* State 268 */
  { 166,   0, 269}, /*  1:                idxitem shift  269 */
  { 181,   1, 270}, /*  2:                     nm shift  270 */
  {  59,   0,  20}, /*  3:                     ID shift  20 */
  { 113,   0,  21}, /*  4:                 STRING shift  21 */
  {  74,   3,  22}, /*  5:                JOIN_KW shift  22 */
/* State 270 */
  {  36,   0,  59}, /*  1:                   DESC shift  59 */
  { 202,   3, 271}, /*  2:              sortorder shift  271 */
  {   7,   0,  58}, /*  3:                    ASC shift  58 */
/* State 273 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 274}, /*  8:                   expr shift  274 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 274 */
  {  84,   0, 123}, /*  1:                     NE shift  123 */
  {  85,   4, 137}, /*  2:                    NOT shift  137 */
  {  86,   0, 159}, /*  3:                NOTNULL shift  159 */
  {  57,   0, 117}, /*  4:                     GT shift  117 */
  {  63,   0, 164}, /*  5:                     IN shift  164 */
  {   5,   0, 105}, /*  6:                    AND shift  105 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  91,   5, 113}, /*  8:                     OR shift  113 */
  {  92,   0, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  77,   0, 140}, /* 14:                   LIKE shift  140 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  {  99,   7, 150}, /* 16:                    REM shift  150 */
  {  72,   0, 154}, /* 17:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 18:                     EQ shift  125 */
  {  81,  20, 115}, /* 19:                     LT shift  115 */
  {  25,   0, 152}, /* 20:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 21:                     LE shift  119 */
  { 105,  14, 133}, /* 22:                 RSHIFT shift  133 */
  {  83,  24, 144}, /* 23:                  MINUS shift  144 */
  {  55,   0, 141}, /* 24:                   GLOB shift  141 */
  {  80,   0, 131}, /* 25:                 LSHIFT shift  131 */
  { 109,  19, 148}, /* 26:                  SLASH shift  148 */
  {  54,   0, 121}, /* 27:                     GE shift  121 */
  { 111,  23, 146}, /* 28:                   STAR shift  146 */
/* State 275 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 276}, /*  2:                     nm shift  276 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 277 */
  { 106,   0,  73}, /*  1:                 SELECT shift  73 */
  { 196,   1, 278}, /*  2:                 select shift  278 */
  { 185,   0,  69}, /*  3:              oneselect shift  69 */
/* State 278 */
  { 180,   0,  71}, /*  1:         multiselect_op shift  71 */
  { 121,   3, 167}, /*  2:                  UNION shift  167 */
  {  46,   0, 170}, /*  3:                 EXCEPT shift  170 */
  {  69,   0, 169}, /*  4:              INTERSECT shift  169 */
  { 104,   4, 279}, /*  5:                     RP shift  279 */
/* State 279 */
  { 165,   0, 260}, /*  1:                    ids shift  260 */
  { 131,   3, 280}, /*  2:                     as shift  280 */
  {   6,   0, 258}, /*  3:                     AS shift  258 */
  { 113,   0, 257}, /*  4:                 STRING shift  257 */
  {  59,   0, 256}, /*  5:                     ID shift  256 */
/* State 280 */
  {  90,   0, 273}, /*  1:                     ON shift  273 */
  { 183,   0, 281}, /*  2:                 on_opt shift  281 */
/* State 281 */
  { 125,   0, 264}, /*  1:                  USING shift  264 */
  { 215,   1, 282}, /*  2:              using_opt shift  282 */
/* State 283 */
  {  97,   3, 193}, /*  1:                  RAISE shift  193 */
  { 113,   1,  66}, /*  2:                 STRING shift  66 */
  {  49,   7, 174}, /*  3:                  FLOAT shift  174 */
  {  83,   0, 179}, /*  4:                  MINUS shift  179 */
  {  68,   0, 173}, /*  5:                INTEGER shift  173 */
  { 181,   9, 287}, /*  6:                     nm shift  287 */
  {  17,   0, 183}, /*  7:                   CASE shift  183 */
  {  87,   0, 106}, /*  8:                   NULL shift  106 */
  {  85,   0, 175}, /*  9:                    NOT shift  175 */
  {  13,   0, 177}, /* 10:                 BITNOT shift  177 */
  {  74,   0,  67}, /* 11:                JOIN_KW shift  67 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  79,   0,  68}, /* 13:                     LP shift  68 */
  { 157,  10, 284}, /* 14:                   expr shift  284 */
  {  94,   0, 181}, /* 15:                   PLUS shift  181 */
  { 111,  13, 286}, /* 16:                   STAR shift  286 */
/* State 284 */
  { 165,   2, 260}, /*  1:                    ids shift  260 */
  {  99,   0, 150}, /*  2:                    REM shift  150 */
  {   5,   0, 105}, /*  3:                    AND shift  105 */
  {  72,   5, 154}, /*  4:                 ISNULL shift  154 */
  {   6,   0, 258}, /*  5:                     AS shift  258 */
  {  71,   3, 155}, /*  6:                     IS shift  155 */
  { 105,   4, 133}, /*  7:                 RSHIFT shift  133 */
  {  76,   0, 119}, /*  8:                     LE shift  119 */
  {  11,   0, 160}, /*  9:                BETWEEN shift  160 */
  { 111,  14, 146}, /* 10:                   STAR shift  146 */
  { 109,   8, 148}, /* 11:                  SLASH shift  148 */
  {  77,   9, 140}, /* 12:                   LIKE shift  140 */
  { 177,  10, 135}, /* 13:                 likeop shift  135 */
  {  45,  17, 125}, /* 14:                     EQ shift  125 */
  { 113,  24, 257}, /* 15:                 STRING shift  257 */
  {  81,   0, 115}, /* 16:                     LT shift  115 */
  {  12,   0, 127}, /* 17:                 BITAND shift  127 */
  {  83,   0, 144}, /* 18:                  MINUS shift  144 */
  {  84,   0, 123}, /* 19:                     NE shift  123 */
  {  85,   0, 137}, /* 20:                    NOT shift  137 */
  {  86,   0, 159}, /* 21:                NOTNULL shift  159 */
  {  54,   0, 121}, /* 22:                     GE shift  121 */
  {  55,   0, 141}, /* 23:                   GLOB shift  141 */
  {  80,  28, 131}, /* 24:                 LSHIFT shift  131 */
  {  57,   0, 117}, /* 25:                     GT shift  117 */
  {  91,  30, 113}, /* 26:                     OR shift  113 */
  {  92,  32, 104}, /* 27:      ORACLE_OUTER_JOIN shift  104 */
  {  14,   0, 129}, /* 28:                  BITOR shift  129 */
  {  94,   0, 142}, /* 29:                   PLUS shift  142 */
  {  25,   0, 152}, /* 30:                 CONCAT shift  152 */
  {  63,   0, 164}, /* 31:                     IN shift  164 */
  {  59,   0, 256}, /* 32:                     ID shift  256 */
  { 131,   0, 285}, /* 33:                     as shift  285 */
/* State 287 */
  {  39,   0, 288}, /*  1:                    DOT shift  288 */
/* State 288 */
  { 111,   0, 289}, /*  1:                   STAR shift  289 */
  { 181,   1, 109}, /*  2:                     nm shift  109 */
  {  59,   0,  20}, /*  3:                     ID shift  20 */
  { 113,   0,  21}, /*  4:                 STRING shift  21 */
  {  74,   3,  22}, /*  5:                JOIN_KW shift  22 */
/* State 293 */
  {  63,   2, 164}, /*  1:                     IN shift  164 */
  {   5,   0, 105}, /*  2:                    AND shift  105 */
  {  12,   0, 127}, /*  3:                 BITAND shift  127 */
  { 177,   0, 135}, /*  4:                 likeop shift  135 */
  {  91,   0, 113}, /*  5:                     OR shift  113 */
  {  92,   1, 104}, /*  6:      ORACLE_OUTER_JOIN shift  104 */
  {  14,   0, 129}, /*  7:                  BITOR shift  129 */
  {  94,   0, 142}, /*  8:                   PLUS shift  142 */
  {  76,   0, 119}, /*  9:                     LE shift  119 */
  {  80,   0, 131}, /* 10:                 LSHIFT shift  131 */
  {  54,  16, 121}, /* 11:                     GE shift  121 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  99,   3, 150}, /* 13:                    REM shift  150 */
  {  71,   0, 155}, /* 14:                     IS shift  155 */
  {  72,   7, 154}, /* 15:                 ISNULL shift  154 */
  {  25,   0, 152}, /* 16:                 CONCAT shift  152 */
  {  45,   0, 125}, /* 17:                     EQ shift  125 */
  { 104,   0, 294}, /* 18:                     RP shift  294 */
  { 105,   9, 133}, /* 19:                 RSHIFT shift  133 */
  {  77,   0, 140}, /* 20:                   LIKE shift  140 */
  {  55,   0, 141}, /* 21:                   GLOB shift  141 */
  {  57,   0, 117}, /* 22:                     GT shift  117 */
  { 109,  10, 148}, /* 23:                  SLASH shift  148 */
  {  81,   0, 115}, /* 24:                     LT shift  115 */
  { 111,   0, 146}, /* 25:                   STAR shift  146 */
  {  83,  11, 144}, /* 26:                  MINUS shift  144 */
  {  84,  21, 123}, /* 27:                     NE shift  123 */
  {  85,   0, 137}, /* 28:                    NOT shift  137 */
  {  86,  22, 159}, /* 29:                NOTNULL shift  159 */
/* State 295 */
  { 104,   2, 296}, /*  1:                     RP shift  296 */
  {  22,   0, 217}, /*  2:                  COMMA shift  217 */
/* State 297 */
  { 104,   0, 298}, /*  1:                     RP shift  298 */
/* State 299 */
  {  63,   2, 164}, /*  1:                     IN shift  164 */
  {   5,   0, 105}, /*  2:                    AND shift  105 */
  {  12,   0, 127}, /*  3:                 BITAND shift  127 */
  { 177,   0, 135}, /*  4:                 likeop shift  135 */
  {  91,   0, 113}, /*  5:                     OR shift  113 */
  {  92,   1, 104}, /*  6:      ORACLE_OUTER_JOIN shift  104 */
  {  14,   0, 129}, /*  7:                  BITOR shift  129 */
  {  94,   0, 142}, /*  8:                   PLUS shift  142 */
  {  76,   0, 119}, /*  9:                     LE shift  119 */
  {  80,   0, 131}, /* 10:                 LSHIFT shift  131 */
  {  54,  16, 121}, /* 11:                     GE shift  121 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  99,   3, 150}, /* 13:                    REM shift  150 */
  {  71,   0, 155}, /* 14:                     IS shift  155 */
  {  72,   7, 154}, /* 15:                 ISNULL shift  154 */
  {  25,   0, 152}, /* 16:                 CONCAT shift  152 */
  {  45,   0, 125}, /* 17:                     EQ shift  125 */
  { 104,   0, 300}, /* 18:                     RP shift  300 */
  { 105,   9, 133}, /* 19:                 RSHIFT shift  133 */
  {  77,   0, 140}, /* 20:                   LIKE shift  140 */
  {  55,   0, 141}, /* 21:                   GLOB shift  141 */
  {  57,   0, 117}, /* 22:                     GT shift  117 */
  { 109,  10, 148}, /* 23:                  SLASH shift  148 */
  {  81,   0, 115}, /* 24:                     LT shift  115 */
  { 111,   0, 146}, /* 25:                   STAR shift  146 */
  {  83,  11, 144}, /* 26:                  MINUS shift  144 */
  {  84,  21, 123}, /* 27:                     NE shift  123 */
  {  85,   0, 137}, /* 28:                    NOT shift  137 */
  {  86,  22, 159}, /* 29:                NOTNULL shift  159 */
/* State 300 */
  { 184,   2, 301}, /*  1:                 onconf shift  301 */
  {  90,   0,  10}, /*  2:                     ON shift  10 */
/* State 302 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 303}, /*  2:                     nm shift  303 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 303 */
  { 168,   0, 304}, /*  1:            idxlist_opt shift  304 */
  {  79,   0, 321}, /*  2:                     LP shift  321 */
/* State 304 */
  { 192,   0, 305}, /*  1:                refargs shift  305 */
/* State 305 */
  {  90,   0, 309}, /*  1:                     ON shift  309 */
  {  82,   0, 307}, /*  2:                  MATCH shift  307 */
  { 191,   0, 306}, /*  3:                 refarg shift  306 */
/* State 307 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 308}, /*  2:                     nm shift  308 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 309 */
  { 123,   3, 317}, /*  1:                 UPDATE shift  317 */
  {  34,   0, 310}, /*  2:                 DELETE shift  310 */
  {  66,   0, 319}, /*  3:                 INSERT shift  319 */
/* State 310 */
  { 108,   4, 312}, /*  1:                    SET shift  312 */
  { 101,   0, 316}, /*  2:               RESTRICT shift  316 */
  { 190,   0, 311}, /*  3:                 refact shift  311 */
  {  16,   0, 315}, /*  4:                CASCADE shift  315 */
/* State 312 */
  {  31,   0, 314}, /*  1:                DEFAULT shift  314 */
  {  87,   1, 313}, /*  2:                   NULL shift  313 */
/* State 317 */
  { 108,   4, 312}, /*  1:                    SET shift  312 */
  { 101,   0, 316}, /*  2:               RESTRICT shift  316 */
  { 190,   0, 318}, /*  3:                 refact shift  318 */
  {  16,   0, 315}, /*  4:                CASCADE shift  315 */
/* State 319 */
  { 108,   4, 312}, /*  1:                    SET shift  312 */
  { 101,   0, 316}, /*  2:               RESTRICT shift  316 */
  { 190,   0, 320}, /*  3:                 refact shift  320 */
  {  16,   0, 315}, /*  4:                CASCADE shift  315 */
/* State 321 */
  { 113,   4,  21}, /*  1:                 STRING shift  21 */
  { 181,   0, 270}, /*  2:                     nm shift  270 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
  { 166,   0, 272}, /*  5:                idxitem shift  272 */
  { 167,   1, 322}, /*  6:                idxlist shift  322 */
/* State 322 */
  { 104,   2, 323}, /*  1:                     RP shift  323 */
  {  22,   0, 268}, /*  2:                  COMMA shift  268 */
/* State 325 */
  { 164,   0, 326}, /*  1:                     id shift  326 */
  {  59,   0, 101}, /*  2:                     ID shift  101 */
/* State 327 */
  {  65,   0,  51}, /*  1:              INITIALLY shift  51 */
  { 169,   1, 328}, /*  2: init_deferred_pred_opt shift  328 */
/* State 330 */
  {  49,   0, 340}, /*  1:                  FLOAT shift  340 */
  { 113,   0, 331}, /*  2:                 STRING shift  331 */
  {  87,   5, 341}, /*  3:                   NULL shift  341 */
  {  94,   3, 334}, /*  4:                   PLUS shift  334 */
  {  59,   0, 332}, /*  5:                     ID shift  332 */
  {  68,   0, 333}, /*  6:                INTEGER shift  333 */
  {  83,   0, 337}, /*  7:                  MINUS shift  337 */
/* State 334 */
  {  68,   0, 335}, /*  1:                INTEGER shift  335 */
  {  49,   0, 336}, /*  2:                  FLOAT shift  336 */
/* State 337 */
  {  68,   0, 338}, /*  1:                INTEGER shift  338 */
  {  49,   0, 339}, /*  2:                  FLOAT shift  339 */
/* State 342 */
  { 113,   0, 257}, /*  1:                 STRING shift  257 */
  { 165,   1, 349}, /*  2:                    ids shift  349 */
  {  59,   0, 256}, /*  3:                     ID shift  256 */
  {  79,   3, 343}, /*  4:                     LP shift  343 */
/* State 343 */
  {  68,   0,  83}, /*  1:                INTEGER shift  83 */
  {  83,   0,  86}, /*  2:                  MINUS shift  86 */
  {  94,   0,  84}, /*  3:                   PLUS shift  84 */
  { 199,   2, 344}, /*  4:                 signed shift  344 */
/* State 344 */
  { 104,   2, 345}, /*  1:                     RP shift  345 */
  {  22,   0, 346}, /*  2:                  COMMA shift  346 */
/* State 346 */
  {  68,   0,  83}, /*  1:                INTEGER shift  83 */
  {  83,   0,  86}, /*  2:                  MINUS shift  86 */
  {  94,   0,  84}, /*  3:                   PLUS shift  84 */
  { 199,   2, 347}, /*  4:                 signed shift  347 */
/* State 347 */
  { 104,   0, 348}, /*  1:                     RP shift  348 */
/* State 352 */
  { 104,   2, 632}, /*  1:                     RP reduce 77 */
  {  96,   0, 357}, /*  2:                PRIMARY shift  357 */
  { 122,   6, 363}, /*  3:                 UNIQUE shift  363 */
  {  51,   8, 371}, /*  4:                FOREIGN shift  371 */
  { 204,   0, 383}, /*  5:                  tcons shift  383 */
  {  18,   0, 368}, /*  6:                  CHECK shift  368 */
  {  22,   0, 353}, /*  7:                  COMMA shift  353 */
  {  27,   0, 355}, /*  8:             CONSTRAINT shift  355 */
/* State 353 */
  { 204,   2, 354}, /*  1:                  tcons shift  354 */
  {  96,   5, 357}, /*  2:                PRIMARY shift  357 */
  { 122,   0, 363}, /*  3:                 UNIQUE shift  363 */
  {  51,   6, 371}, /*  4:                FOREIGN shift  371 */
  {  18,   0, 368}, /*  5:                  CHECK shift  368 */
  {  27,   0, 355}, /*  6:             CONSTRAINT shift  355 */
/* State 355 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 356}, /*  2:                     nm shift  356 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 357 */
  {  75,   0, 358}, /*  1:                    KEY shift  358 */
/* State 358 */
  {  79,   0, 359}, /*  1:                     LP shift  359 */
/* State 359 */
  { 113,   4,  21}, /*  1:                 STRING shift  21 */
  { 181,   0, 270}, /*  2:                     nm shift  270 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
  { 166,   0, 272}, /*  5:                idxitem shift  272 */
  { 167,   1, 360}, /*  6:                idxlist shift  360 */
/* State 360 */
  { 104,   2, 361}, /*  1:                     RP shift  361 */
  {  22,   0, 268}, /*  2:                  COMMA shift  268 */
/* State 361 */
  { 184,   2, 362}, /*  1:                 onconf shift  362 */
  {  90,   0,  10}, /*  2:                     ON shift  10 */
/* State 363 */
  {  79,   0, 364}, /*  1:                     LP shift  364 */
/* State 364 */
  { 113,   4,  21}, /*  1:                 STRING shift  21 */
  { 181,   0, 270}, /*  2:                     nm shift  270 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
  { 166,   0, 272}, /*  5:                idxitem shift  272 */
  { 167,   1, 365}, /*  6:                idxlist shift  365 */
/* State 365 */
  { 104,   2, 366}, /*  1:                     RP shift  366 */
  {  22,   0, 268}, /*  2:                  COMMA shift  268 */
/* State 366 */
  { 184,   2, 367}, /*  1:                 onconf shift  367 */
  {  90,   0,  10}, /*  2:                     ON shift  10 */
/* State 368 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 369}, /*  8:                   expr shift  369 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 369 */
  {  90,   0,  10}, /*  1:                     ON shift  10 */
  {  91,   0, 113}, /*  2:                     OR shift  113 */
  {  92,   0, 104}, /*  3:      ORACLE_OUTER_JOIN shift  104 */
  {  63,   0, 164}, /*  4:                     IN shift  164 */
  { 184,   7, 370}, /*  5:                 onconf shift  370 */
  {   5,   0, 105}, /*  6:                    AND shift  105 */
  {  94,   0, 142}, /*  7:                   PLUS shift  142 */
  {  11,   0, 160}, /*  8:                BETWEEN shift  160 */
  {  12,   0, 127}, /*  9:                 BITAND shift  127 */
  {  99,   0, 150}, /* 10:                    REM shift  150 */
  {  45,   0, 125}, /* 11:                     EQ shift  125 */
  {  71,   8, 155}, /* 12:                     IS shift  155 */
  {  72,   9, 154}, /* 13:                 ISNULL shift  154 */
  {  81,   0, 115}, /* 14:                     LT shift  115 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  { 105,  11, 133}, /* 16:                 RSHIFT shift  133 */
  {  76,   0, 119}, /* 17:                     LE shift  119 */
  {  77,   0, 140}, /* 18:                   LIKE shift  140 */
  {  54,   0, 121}, /* 19:                     GE shift  121 */
  { 109,   0, 148}, /* 20:                  SLASH shift  148 */
  {  80,   0, 131}, /* 21:                 LSHIFT shift  131 */
  { 111,  14, 146}, /* 22:                   STAR shift  146 */
  {  55,  29, 141}, /* 23:                   GLOB shift  141 */
  {  83,   0, 144}, /* 24:                  MINUS shift  144 */
  {  84,  19, 123}, /* 25:                     NE shift  123 */
  {  85,  23, 137}, /* 26:                    NOT shift  137 */
  {  86,   0, 159}, /* 27:                NOTNULL shift  159 */
  { 177,  30, 135}, /* 28:                 likeop shift  135 */
  {  25,   0, 152}, /* 29:                 CONCAT shift  152 */
  {  57,   0, 117}, /* 30:                     GT shift  117 */
/* State 371 */
  {  75,   0, 372}, /*  1:                    KEY shift  372 */
/* State 372 */
  {  79,   0, 373}, /*  1:                     LP shift  373 */
/* State 373 */
  { 113,   4,  21}, /*  1:                 STRING shift  21 */
  { 181,   0, 270}, /*  2:                     nm shift  270 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
  { 166,   0, 272}, /*  5:                idxitem shift  272 */
  { 167,   1, 374}, /*  6:                idxlist shift  374 */
/* State 374 */
  { 104,   2, 375}, /*  1:                     RP shift  375 */
  {  22,   0, 268}, /*  2:                  COMMA shift  268 */
/* State 375 */
  {  98,   0, 376}, /*  1:             REFERENCES shift  376 */
/* State 376 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 377}, /*  2:                     nm shift  377 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 377 */
  { 168,   0, 378}, /*  1:            idxlist_opt shift  378 */
  {  79,   0, 321}, /*  2:                     LP shift  321 */
/* State 378 */
  { 192,   0, 379}, /*  1:                refargs shift  379 */
/* State 379 */
  {  32,   0, 327}, /*  1:             DEFERRABLE shift  327 */
  {  85,   0, 380}, /*  2:                    NOT shift  380 */
  { 191,   0, 306}, /*  3:                 refarg shift  306 */
  {  82,   0, 307}, /*  4:                  MATCH shift  307 */
  { 151,   1, 382}, /*  5:        defer_subclause shift  382 */
  { 152,   4, 381}, /*  6:    defer_subclause_opt shift  381 */
  {  90,   0, 309}, /*  7:                     ON shift  309 */
/* State 380 */
  {  32,   0,  49}, /*  1:             DEFERRABLE shift  49 */
/* State 386 */
  { 106,   0,  73}, /*  1:                 SELECT shift  73 */
  { 196,   1, 387}, /*  2:                 select shift  387 */
  { 185,   0,  69}, /*  3:              oneselect shift  69 */
/* State 387 */
  { 180,   0,  71}, /*  1:         multiselect_op shift  71 */
  { 121,   4, 167}, /*  2:                  UNION shift  167 */
  { 107,   0, 575}, /*  3:                   SEMI reduce 20 */
  {  46,   0, 170}, /*  4:                 EXCEPT shift  170 */
  {  69,   0, 169}, /*  5:              INTERSECT shift  169 */
/* State 388 */
  { 115,   0, 434}, /*  1:                   TEMP shift  434 */
  { 205,   1, 389}, /*  2:                   temp shift  389 */
  { 209,   0, 435}, /*  3:           trigger_decl shift  435 */
/* State 389 */
  { 114,   0, 390}, /*  1:                  TABLE shift  390 */
  { 122,   0, 406}, /*  2:                 UNIQUE shift  406 */
  { 128,   2, 392}, /*  3:                   VIEW shift  392 */
  { 118,   6, 407}, /*  4:                TRIGGER shift  407 */
  { 214,   4, 396}, /*  5:             uniqueflag shift  396 */
  {  64,   0, 788}, /*  6:                  INDEX reduce 233 */
/* State 390 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 391}, /*  2:                     nm shift  391 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 392 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 393}, /*  2:                     nm shift  393 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 393 */
  {   6,   0, 394}, /*  1:                     AS shift  394 */
/* State 394 */
  { 106,   0,  73}, /*  1:                 SELECT shift  73 */
  { 196,   1, 395}, /*  2:                 select shift  395 */
  { 185,   0,  69}, /*  3:              oneselect shift  69 */
/* State 395 */
  { 180,   0,  71}, /*  1:         multiselect_op shift  71 */
  { 121,   4, 167}, /*  2:                  UNION shift  167 */
  { 107,   0, 653}, /*  3:                   SEMI reduce 98 */
  {  46,   0, 170}, /*  4:                 EXCEPT shift  170 */
  {  69,   0, 169}, /*  5:              INTERSECT shift  169 */
/* State 396 */
  {  64,   0, 397}, /*  1:                  INDEX shift  397 */
/* State 397 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 398}, /*  2:                     nm shift  398 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 398 */
  {  90,   0, 399}, /*  1:                     ON shift  399 */
/* State 399 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 400}, /*  2:                     nm shift  400 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 400 */
  { 150,   3, 401}, /*  1:                   dbnm shift  401 */
  {  79,   0, 680}, /*  2:                     LP reduce 125 */
  {  39,   0, 275}, /*  3:                    DOT shift  275 */
/* State 401 */
  {  79,   0, 402}, /*  1:                     LP shift  402 */
/* State 402 */
  { 113,   4,  21}, /*  1:                 STRING shift  21 */
  { 181,   0, 270}, /*  2:                     nm shift  270 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
  { 166,   0, 272}, /*  5:                idxitem shift  272 */
  { 167,   1, 403}, /*  6:                idxlist shift  403 */
/* State 403 */
  { 104,   2, 404}, /*  1:                     RP shift  404 */
  {  22,   0, 268}, /*  2:                  COMMA shift  268 */
/* State 404 */
  {  90,   0,  10}, /*  1:                     ON shift  10 */
  { 184,   0, 405}, /*  2:                 onconf shift  405 */
  { 107,   0, 643}, /*  3:                   SEMI reduce 88 */
/* State 405 */
  { 107,   0, 786}, /*  1:                   SEMI reduce 231 */
/* State 406 */
  {  64,   0, 787}, /*  1:                  INDEX reduce 232 */
/* State 407 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 408}, /*  2:                     nm shift  408 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 408 */
  {  67,   0, 432}, /*  1:                INSTEAD shift  432 */
  {   9,   0, 430}, /*  2:                 BEFORE shift  430 */
  {   2,   0, 431}, /*  3:                  AFTER shift  431 */
  { 211,   1, 409}, /*  4:           trigger_time shift  409 */
/* State 409 */
  {  66,   2, 423}, /*  1:                 INSERT shift  423 */
  {  34,   0, 422}, /*  2:                 DELETE shift  422 */
  { 210,   1, 410}, /*  3:          trigger_event shift  410 */
  { 123,   0, 424}, /*  4:                 UPDATE shift  424 */
/* State 410 */
  {  90,   0, 411}, /*  1:                     ON shift  411 */
/* State 411 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 412}, /*  2:                     nm shift  412 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 412 */
  { 150,   0, 413}, /*  1:                   dbnm shift  413 */
  {  39,   0, 275}, /*  2:                    DOT shift  275 */
/* State 413 */
  { 160,   2, 414}, /*  1:         foreach_clause shift  414 */
  {  50,   0, 418}, /*  2:                    FOR shift  418 */
/* State 414 */
  { 216,   3, 415}, /*  1:            when_clause shift  415 */
  {  10,   0, 824}, /*  2:                  BEGIN reduce 269 */
  { 129,   0, 416}, /*  3:                   WHEN shift  416 */
/* State 415 */
  {  10,   0, 812}, /*  1:                  BEGIN reduce 257 */
/* State 416 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 417}, /*  8:                   expr shift  417 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 417 */
  {  63,   2, 164}, /*  1:                     IN shift  164 */
  {   5,   0, 105}, /*  2:                    AND shift  105 */
  {  12,   0, 127}, /*  3:                 BITAND shift  127 */
  { 177,   0, 135}, /*  4:                 likeop shift  135 */
  {  91,   0, 113}, /*  5:                     OR shift  113 */
  {  92,   1, 104}, /*  6:      ORACLE_OUTER_JOIN shift  104 */
  {  14,   0, 129}, /*  7:                  BITOR shift  129 */
  {  94,   0, 142}, /*  8:                   PLUS shift  142 */
  {  76,   0, 119}, /*  9:                     LE shift  119 */
  {  80,   0, 131}, /* 10:                 LSHIFT shift  131 */
  {  10,   0, 825}, /* 11:                  BEGIN reduce 270 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  99,   3, 150}, /* 13:                    REM shift  150 */
  {  71,   0, 155}, /* 14:                     IS shift  155 */
  {  72,   7, 154}, /* 15:                 ISNULL shift  154 */
  {  54,  18, 121}, /* 16:                     GE shift  121 */
  {  45,   0, 125}, /* 17:                     EQ shift  125 */
  {  25,   0, 152}, /* 18:                 CONCAT shift  152 */
  { 105,   9, 133}, /* 19:                 RSHIFT shift  133 */
  {  77,   0, 140}, /* 20:                   LIKE shift  140 */
  {  55,   0, 141}, /* 21:                   GLOB shift  141 */
  {  57,   0, 117}, /* 22:                     GT shift  117 */
  { 109,  10, 148}, /* 23:                  SLASH shift  148 */
  {  81,   0, 115}, /* 24:                     LT shift  115 */
  { 111,   0, 146}, /* 25:                   STAR shift  146 */
  {  83,  16, 144}, /* 26:                  MINUS shift  144 */
  {  84,  21, 123}, /* 27:                     NE shift  123 */
  {  85,   0, 137}, /* 28:                    NOT shift  137 */
  {  86,  22, 159}, /* 29:                NOTNULL shift  159 */
/* State 418 */
  {  41,   0, 419}, /*  1:                   EACH shift  419 */
/* State 419 */
  { 112,   0, 421}, /*  1:              STATEMENT shift  421 */
  { 103,   0, 420}, /*  2:                    ROW shift  420 */
/* State 422 */
  {  90,   0, 817}, /*  1:                     ON reduce 262 */
/* State 423 */
  {  90,   0, 818}, /*  1:                     ON reduce 263 */
/* State 424 */
  {  90,   2, 819}, /*  1:                     ON reduce 264 */
  {  88,   0, 425}, /*  2:                     OF shift  425 */
/* State 425 */
  { 171,   0, 426}, /*  1:             inscollist shift  426 */
  { 181,   1, 429}, /*  2:                     nm shift  429 */
  {  59,   0,  20}, /*  3:                     ID shift  20 */
  { 113,   0,  21}, /*  4:                 STRING shift  21 */
  {  74,   3,  22}, /*  5:                JOIN_KW shift  22 */
/* State 426 */
  {  90,   2, 820}, /*  1:                     ON reduce 265 */
  {  22,   0, 427}, /*  2:                  COMMA shift  427 */
/* State 427 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 428}, /*  2:                     nm shift  428 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 432 */
  {  88,   0, 433}, /*  1:                     OF shift  433 */
/* State 435 */
  {  10,   0, 436}, /*  1:                  BEGIN shift  436 */
/* State 436 */
  { 207,   0, 440}, /*  1:            trigger_cmd shift  440 */
  { 208,   0, 438}, /*  2:       trigger_cmd_list shift  438 */
  { 106,   5,  73}, /*  3:                 SELECT shift  73 */
  {  66,   0, 458}, /*  4:                 INSERT shift  458 */
  {  43,   9, 827}, /*  5:                    END reduce 272 */
  { 185,   0,  69}, /*  6:              oneselect shift  69 */
  { 123,   0, 443}, /*  7:                 UPDATE shift  443 */
  { 196,   3, 437}, /*  8:                 select shift  437 */
  {  34,   0, 474}, /*  9:                 DELETE shift  474 */
/* State 437 */
  { 180,   0,  71}, /*  1:         multiselect_op shift  71 */
  { 121,   4, 167}, /*  2:                  UNION shift  167 */
  { 107,   0, 832}, /*  3:                   SEMI reduce 277 */
  {  46,   0, 170}, /*  4:                 EXCEPT shift  170 */
  {  69,   0, 169}, /*  5:              INTERSECT shift  169 */
/* State 438 */
  {  43,   0, 439}, /*  1:                    END shift  439 */
/* State 439 */
  { 107,   0, 811}, /*  1:                   SEMI reduce 256 */
/* State 440 */
  { 107,   0, 441}, /*  1:                   SEMI shift  441 */
/* State 441 */
  { 207,   0, 440}, /*  1:            trigger_cmd shift  440 */
  { 208,   0, 442}, /*  2:       trigger_cmd_list shift  442 */
  { 106,   5,  73}, /*  3:                 SELECT shift  73 */
  {  66,   0, 458}, /*  4:                 INSERT shift  458 */
  {  43,   9, 827}, /*  5:                    END reduce 272 */
  { 185,   0,  69}, /*  6:              oneselect shift  69 */
  { 123,   0, 443}, /*  7:                 UPDATE shift  443 */
  { 196,   3, 437}, /*  8:                 select shift  437 */
  {  34,   0, 474}, /*  9:                 DELETE shift  474 */
/* State 442 */
  {  43,   0, 826}, /*  1:                    END reduce 271 */
/* State 443 */
  { 186,   0, 446}, /*  1:                 orconf shift  446 */
  {  91,   0, 444}, /*  2:                     OR shift  444 */
/* State 444 */
  { 102,   3,  13}, /*  1:               ROLLBACK shift  13 */
  { 193,   4, 445}, /*  2:            resolvetype shift  445 */
  {  60,   6,  16}, /*  3:                 IGNORE shift  16 */
  {   1,   0,  14}, /*  4:                  ABORT shift  14 */
  { 100,   0,  17}, /*  5:                REPLACE shift  17 */
  {  48,   0,  15}, /*  6:                   FAIL shift  15 */
/* State 446 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 447}, /*  2:                     nm shift  447 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 447 */
  { 108,   0, 448}, /*  1:                    SET shift  448 */
/* State 448 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   0, 455}, /*  2:                     nm shift  455 */
  {  59,   0,  20}, /*  3:                     ID shift  20 */
  { 198,   1, 449}, /*  4:                setlist shift  449 */
  {  74,   3,  22}, /*  5:                JOIN_KW shift  22 */
/* State 449 */
  {  22,   0, 450}, /*  1:                  COMMA shift  450 */
  { 217,   0, 454}, /*  2:              where_opt shift  454 */
  { 130,   1, 239}, /*  3:                  WHERE shift  239 */
  { 107,   0, 710}, /*  4:                   SEMI reduce 155 */
/* State 450 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 451}, /*  2:                     nm shift  451 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 451 */
  {  45,   0, 452}, /*  1:                     EQ shift  452 */
/* State 452 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 453}, /*  8:                   expr shift  453 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 453 */
  {  84,   0, 123}, /*  1:                     NE shift  123 */
  {  85,   4, 137}, /*  2:                    NOT shift  137 */
  {  86,   0, 159}, /*  3:                NOTNULL shift  159 */
  {  57,   0, 117}, /*  4:                     GT shift  117 */
  {  63,   0, 164}, /*  5:                     IN shift  164 */
  {   5,   0, 105}, /*  6:                    AND shift  105 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  91,   5, 113}, /*  8:                     OR shift  113 */
  {  92,   0, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  77,   0, 140}, /* 14:                   LIKE shift  140 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  {  99,   7, 150}, /* 16:                    REM shift  150 */
  {  72,   0, 154}, /* 17:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 18:                     EQ shift  125 */
  {  81,  20, 115}, /* 19:                     LT shift  115 */
  {  25,   0, 152}, /* 20:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 21:                     LE shift  119 */
  { 105,  14, 133}, /* 22:                 RSHIFT shift  133 */
  {  83,  24, 144}, /* 23:                  MINUS shift  144 */
  {  55,   0, 141}, /* 24:                   GLOB shift  141 */
  {  80,   0, 131}, /* 25:                 LSHIFT shift  131 */
  { 109,  19, 148}, /* 26:                  SLASH shift  148 */
  {  54,   0, 121}, /* 27:                     GE shift  121 */
  { 111,  23, 146}, /* 28:                   STAR shift  146 */
/* State 454 */
  { 107,   0, 828}, /*  1:                   SEMI reduce 273 */
/* State 455 */
  {  45,   0, 456}, /*  1:                     EQ shift  456 */
/* State 456 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 457}, /*  8:                   expr shift  457 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 457 */
  {  84,   0, 123}, /*  1:                     NE shift  123 */
  {  85,   4, 137}, /*  2:                    NOT shift  137 */
  {  86,   0, 159}, /*  3:                NOTNULL shift  159 */
  {  57,   0, 117}, /*  4:                     GT shift  117 */
  {  63,   0, 164}, /*  5:                     IN shift  164 */
  {   5,   0, 105}, /*  6:                    AND shift  105 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  91,   5, 113}, /*  8:                     OR shift  113 */
  {  92,   0, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  77,   0, 140}, /* 14:                   LIKE shift  140 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  {  99,   7, 150}, /* 16:                    REM shift  150 */
  {  72,   0, 154}, /* 17:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 18:                     EQ shift  125 */
  {  81,  20, 115}, /* 19:                     LT shift  115 */
  {  25,   0, 152}, /* 20:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 21:                     LE shift  119 */
  { 105,  14, 133}, /* 22:                 RSHIFT shift  133 */
  {  83,  24, 144}, /* 23:                  MINUS shift  144 */
  {  55,   0, 141}, /* 24:                   GLOB shift  141 */
  {  80,   0, 131}, /* 25:                 LSHIFT shift  131 */
  { 109,  19, 148}, /* 26:                  SLASH shift  148 */
  {  54,   0, 121}, /* 27:                     GE shift  121 */
  { 111,  23, 146}, /* 28:                   STAR shift  146 */
/* State 458 */
  { 186,   0, 459}, /*  1:                 orconf shift  459 */
  {  91,   3, 444}, /*  2:                     OR shift  444 */
  {  70,   0, 645}, /*  3:                   INTO reduce 90 */
/* State 459 */
  {  70,   0, 460}, /*  1:                   INTO shift  460 */
/* State 460 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 461}, /*  2:                     nm shift  461 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 461 */
  { 172,   0, 465}, /*  1:         inscollist_opt shift  465 */
  {  79,   0, 462}, /*  2:                     LP shift  462 */
/* State 462 */
  { 171,   0, 463}, /*  1:             inscollist shift  463 */
  { 181,   1, 429}, /*  2:                     nm shift  429 */
  {  59,   0,  20}, /*  3:                     ID shift  20 */
  { 113,   0,  21}, /*  4:                 STRING shift  21 */
  {  74,   3,  22}, /*  5:                JOIN_KW shift  22 */
/* State 463 */
  { 104,   2, 464}, /*  1:                     RP shift  464 */
  {  22,   0, 427}, /*  2:                  COMMA shift  427 */
/* State 465 */
  { 196,   0, 466}, /*  1:                 select shift  466 */
  { 185,   0,  69}, /*  2:              oneselect shift  69 */
  { 106,   0,  73}, /*  3:                 SELECT shift  73 */
  { 127,   0, 467}, /*  4:                 VALUES shift  467 */
/* State 466 */
  { 180,   0,  71}, /*  1:         multiselect_op shift  71 */
  { 121,   4, 167}, /*  2:                  UNION shift  167 */
  { 107,   0, 830}, /*  3:                   SEMI reduce 275 */
  {  46,   0, 170}, /*  4:                 EXCEPT shift  170 */
  {  69,   0, 169}, /*  5:              INTERSECT shift  169 */
/* State 467 */
  {  79,   0, 468}, /*  1:                     LP shift  468 */
/* State 468 */
  {  97,   3, 193}, /*  1:                  RAISE shift  193 */
  { 113,   1,  66}, /*  2:                 STRING shift  66 */
  {  49,   7, 174}, /*  3:                  FLOAT shift  174 */
  {  83,   0, 179}, /*  4:                  MINUS shift  179 */
  {  68,   0, 173}, /*  5:                INTEGER shift  173 */
  { 181,   9, 107}, /*  6:                     nm shift  107 */
  {  17,   0, 183}, /*  7:                   CASE shift  183 */
  {  87,   0, 106}, /*  8:                   NULL shift  106 */
  {  85,   0, 175}, /*  9:                    NOT shift  175 */
  {  13,   0, 177}, /* 10:                 BITNOT shift  177 */
  {  74,   0,  67}, /* 11:                JOIN_KW shift  67 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  94,   0, 181}, /* 13:                   PLUS shift  181 */
  { 157,  10, 473}, /* 14:                   expr shift  473 */
  { 174,  13, 469}, /* 15:               itemlist shift  469 */
  {  79,   0,  68}, /* 16:                     LP shift  68 */
/* State 469 */
  { 104,   2, 472}, /*  1:                     RP shift  472 */
  {  22,   0, 470}, /*  2:                  COMMA shift  470 */
/* State 470 */
  {  79,   4,  68}, /*  1:                     LP shift  68 */
  { 181,   0, 107}, /*  2:                     nm shift  107 */
  {  17,   0, 183}, /*  3:                   CASE shift  183 */
  {  49,   0, 174}, /*  4:                  FLOAT shift  174 */
  {  94,   1, 181}, /*  5:                   PLUS shift  181 */
  {  97,   0, 193}, /*  6:                  RAISE shift  193 */
  {  83,  10, 179}, /*  7:                  MINUS shift  179 */
  { 157,   6, 471}, /*  8:                   expr shift  471 */
  { 113,   7,  66}, /*  9:                 STRING shift  66 */
  {  68,   0, 173}, /* 10:                INTEGER shift  173 */
  {  85,   0, 175}, /* 11:                    NOT shift  175 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  87,   0, 106}, /* 13:                   NULL shift  106 */
  {  13,   0, 177}, /* 14:                 BITNOT shift  177 */
  {  74,  12,  67}, /* 15:                JOIN_KW shift  67 */
/* State 471 */
  {  84,   0, 123}, /*  1:                     NE shift  123 */
  {  85,   4, 137}, /*  2:                    NOT shift  137 */
  {  86,   0, 159}, /*  3:                NOTNULL shift  159 */
  {  57,   0, 117}, /*  4:                     GT shift  117 */
  {  63,   0, 164}, /*  5:                     IN shift  164 */
  {   5,   0, 105}, /*  6:                    AND shift  105 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  91,   5, 113}, /*  8:                     OR shift  113 */
  {  92,   0, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  77,   0, 140}, /* 14:                   LIKE shift  140 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  {  99,   7, 150}, /* 16:                    REM shift  150 */
  {  72,   0, 154}, /* 17:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 18:                     EQ shift  125 */
  {  81,  20, 115}, /* 19:                     LT shift  115 */
  {  25,   0, 152}, /* 20:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 21:                     LE shift  119 */
  { 105,  14, 133}, /* 22:                 RSHIFT shift  133 */
  {  83,  24, 144}, /* 23:                  MINUS shift  144 */
  {  55,   0, 141}, /* 24:                   GLOB shift  141 */
  {  80,   0, 131}, /* 25:                 LSHIFT shift  131 */
  { 109,  19, 148}, /* 26:                  SLASH shift  148 */
  {  54,   0, 121}, /* 27:                     GE shift  121 */
  { 111,  23, 146}, /* 28:                   STAR shift  146 */
/* State 472 */
  { 107,   0, 829}, /*  1:                   SEMI reduce 274 */
/* State 473 */
  {  84,   0, 123}, /*  1:                     NE shift  123 */
  {  85,   4, 137}, /*  2:                    NOT shift  137 */
  {  86,   0, 159}, /*  3:                NOTNULL shift  159 */
  {  57,   0, 117}, /*  4:                     GT shift  117 */
  {  63,   0, 164}, /*  5:                     IN shift  164 */
  {   5,   0, 105}, /*  6:                    AND shift  105 */
  {  71,   0, 155}, /*  7:                     IS shift  155 */
  {  91,   5, 113}, /*  8:                     OR shift  113 */
  {  92,   0, 104}, /*  9:      ORACLE_OUTER_JOIN shift  104 */
  { 177,   0, 135}, /* 10:                 likeop shift  135 */
  {  94,   0, 142}, /* 11:                   PLUS shift  142 */
  {  11,   0, 160}, /* 12:                BETWEEN shift  160 */
  {  12,   0, 127}, /* 13:                 BITAND shift  127 */
  {  77,   0, 140}, /* 14:                   LIKE shift  140 */
  {  14,   0, 129}, /* 15:                  BITOR shift  129 */
  {  99,   7, 150}, /* 16:                    REM shift  150 */
  {  72,   0, 154}, /* 17:                 ISNULL shift  154 */
  {  45,   0, 125}, /* 18:                     EQ shift  125 */
  {  81,  20, 115}, /* 19:                     LT shift  115 */
  {  25,   0, 152}, /* 20:                 CONCAT shift  152 */
  {  76,   0, 119}, /* 21:                     LE shift  119 */
  { 105,  14, 133}, /* 22:                 RSHIFT shift  133 */
  {  83,  24, 144}, /* 23:                  MINUS shift  144 */
  {  55,   0, 141}, /* 24:                   GLOB shift  141 */
  {  80,   0, 131}, /* 25:                 LSHIFT shift  131 */
  { 109,  19, 148}, /* 26:                  SLASH shift  148 */
  {  54,   0, 121}, /* 27:                     GE shift  121 */
  { 111,  23, 146}, /* 28:                   STAR shift  146 */
/* State 474 */
  {  52,   0, 475}, /*  1:                   FROM shift  475 */
/* State 475 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 476}, /*  2:                     nm shift  476 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 476 */
  { 130,   0, 239}, /*  1:                  WHERE shift  239 */
  { 217,   1, 477}, /*  2:              where_opt shift  477 */
  { 107,   0, 710}, /*  3:                   SEMI reduce 155 */
/* State 477 */
  { 107,   0, 831}, /*  1:                   SEMI reduce 276 */
/* State 478 */
  { 128,   2, 481}, /*  1:                   VIEW shift  481 */
  {  64,   0, 483}, /*  2:                  INDEX shift  483 */
  { 118,   4, 486}, /*  3:                TRIGGER shift  486 */
  { 114,   0, 479}, /*  4:                  TABLE shift  479 */
/* State 479 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 480}, /*  2:                     nm shift  480 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 480 */
  { 107,   0, 652}, /*  1:                   SEMI reduce 97 */
/* State 481 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 482}, /*  2:                     nm shift  482 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 482 */
  { 107,   0, 654}, /*  1:                   SEMI reduce 99 */
/* State 483 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 484}, /*  2:                     nm shift  484 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 484 */
  { 150,   2, 485}, /*  1:                   dbnm shift  485 */
  {  39,   0, 275}, /*  2:                    DOT shift  275 */
  { 107,   0, 680}, /*  3:                   SEMI reduce 125 */
/* State 485 */
  { 107,   0, 794}, /*  1:                   SEMI reduce 239 */
/* State 486 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 487}, /*  2:                     nm shift  487 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 487 */
  { 150,   2, 488}, /*  1:                   dbnm shift  488 */
  {  39,   0, 275}, /*  2:                    DOT shift  275 */
  { 107,   0, 680}, /*  3:                   SEMI reduce 125 */
/* State 488 */
  { 107,   0, 837}, /*  1:                   SEMI reduce 282 */
/* State 489 */
  { 180,   0,  71}, /*  1:         multiselect_op shift  71 */
  { 121,   4, 167}, /*  2:                  UNION shift  167 */
  { 107,   0, 655}, /*  3:                   SEMI reduce 100 */
  {  46,   0, 170}, /*  4:                 EXCEPT shift  170 */
  {  69,   0, 169}, /*  5:              INTERSECT shift  169 */
/* State 490 */
  {  52,   0, 491}, /*  1:                   FROM shift  491 */
/* State 491 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 492}, /*  2:                     nm shift  492 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 492 */
  { 150,   0, 493}, /*  1:                   dbnm shift  493 */
  {  39,   0, 275}, /*  2:                    DOT shift  275 */
/* State 493 */
  { 130,   0, 239}, /*  1:                  WHERE shift  239 */
  { 217,   1, 494}, /*  2:              where_opt shift  494 */
  { 107,   0, 710}, /*  3:                   SEMI reduce 155 */
/* State 494 */
  { 107,   0, 709}, /*  1:                   SEMI reduce 154 */
/* State 495 */
  { 186,   0, 496}, /*  1:                 orconf shift  496 */
  {  91,   0, 444}, /*  2:                     OR shift  444 */
/* State 496 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 497}, /*  2:                     nm shift  497 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 497 */
  { 150,   2, 498}, /*  1:                   dbnm shift  498 */
  { 108,   3, 680}, /*  2:                    SET reduce 125 */
  {  39,   0, 275}, /*  3:                    DOT shift  275 */
/* State 498 */
  { 108,   0, 499}, /*  1:                    SET shift  499 */
/* State 499 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   0, 455}, /*  2:                     nm shift  455 */
  {  59,   0,  20}, /*  3:                     ID shift  20 */
  { 198,   1, 500}, /*  4:                setlist shift  500 */
  {  74,   3,  22}, /*  5:                JOIN_KW shift  22 */
/* State 500 */
  {  22,   0, 450}, /*  1:                  COMMA shift  450 */
  { 217,   0, 501}, /*  2:              where_opt shift  501 */
  { 130,   1, 239}, /*  3:                  WHERE shift  239 */
  { 107,   0, 710}, /*  4:                   SEMI reduce 155 */
/* State 501 */
  { 107,   0, 712}, /*  1:                   SEMI reduce 157 */
/* State 502 */
  {  70,   0, 503}, /*  1:                   INTO shift  503 */
/* State 503 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 504}, /*  2:                     nm shift  504 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 504 */
  { 150,   0, 505}, /*  1:                   dbnm shift  505 */
  {  39,   0, 275}, /*  2:                    DOT shift  275 */
/* State 505 */
  { 172,   0, 506}, /*  1:         inscollist_opt shift  506 */
  {  79,   0, 462}, /*  2:                     LP shift  462 */
/* State 506 */
  { 196,   0, 507}, /*  1:                 select shift  507 */
  { 185,   0,  69}, /*  2:              oneselect shift  69 */
  { 106,   0,  73}, /*  3:                 SELECT shift  73 */
  { 127,   0, 508}, /*  4:                 VALUES shift  508 */
/* State 507 */
  { 180,   0,  71}, /*  1:         multiselect_op shift  71 */
  { 121,   4, 167}, /*  2:                  UNION shift  167 */
  { 107,   0, 716}, /*  3:                   SEMI reduce 161 */
  {  46,   0, 170}, /*  4:                 EXCEPT shift  170 */
  {  69,   0, 169}, /*  5:              INTERSECT shift  169 */
/* State 508 */
  {  79,   0, 509}, /*  1:                     LP shift  509 */
/* State 509 */
  {  97,   3, 193}, /*  1:                  RAISE shift  193 */
  { 113,   1,  66}, /*  2:                 STRING shift  66 */
  {  49,   7, 174}, /*  3:                  FLOAT shift  174 */
  {  83,   0, 179}, /*  4:                  MINUS shift  179 */
  {  68,   0, 173}, /*  5:                INTEGER shift  173 */
  { 181,   9, 107}, /*  6:                     nm shift  107 */
  {  17,   0, 183}, /*  7:                   CASE shift  183 */
  {  87,   0, 106}, /*  8:                   NULL shift  106 */
  {  85,   0, 175}, /*  9:                    NOT shift  175 */
  {  13,   0, 177}, /* 10:                 BITNOT shift  177 */
  {  74,   0,  67}, /* 11:                JOIN_KW shift  67 */
  {  59,   0,  64}, /* 12:                     ID shift  64 */
  {  94,   0, 181}, /* 13:                   PLUS shift  181 */
  { 157,  10, 473}, /* 14:                   expr shift  473 */
  { 174,  13, 510}, /* 15:               itemlist shift  510 */
  {  79,   0,  68}, /* 16:                     LP shift  68 */
/* State 510 */
  { 104,   2, 511}, /*  1:                     RP shift  511 */
  {  22,   0, 470}, /*  2:                  COMMA shift  470 */
/* State 511 */
  { 107,   0, 715}, /*  1:                   SEMI reduce 160 */
/* State 512 */
  { 186,   0, 513}, /*  1:                 orconf shift  513 */
  {  91,   3, 444}, /*  2:                     OR shift  444 */
  {  70,   0, 645}, /*  3:                   INTO reduce 90 */
/* State 513 */
  {  70,   0, 717}, /*  1:                   INTO reduce 162 */
/* State 514 */
  {  70,   0, 718}, /*  1:                   INTO reduce 163 */
/* State 515 */
  { 186,   0, 516}, /*  1:                 orconf shift  516 */
  {  91,   0, 444}, /*  2:                     OR shift  444 */
/* State 516 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 517}, /*  2:                     nm shift  517 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 517 */
  { 150,   3, 518}, /*  1:                   dbnm shift  518 */
  {  52,   0, 680}, /*  2:                   FROM reduce 125 */
  {  39,   0, 275}, /*  3:                    DOT shift  275 */
/* State 518 */
  {  52,   0, 519}, /*  1:                   FROM shift  519 */
/* State 519 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 520}, /*  2:                     nm shift  520 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 520 */
  { 107,   0, 796}, /*  1:                   SEMI reduce 241 */
  { 125,   1, 521}, /*  2:                  USING shift  521 */
/* State 521 */
  {  35,   0, 522}, /*  1:             DELIMITERS shift  522 */
/* State 522 */
  { 113,   0, 523}, /*  1:                 STRING shift  523 */
/* State 523 */
  { 107,   0, 795}, /*  1:                   SEMI reduce 240 */
/* State 524 */
  {  59,   0,  20}, /*  1:                     ID shift  20 */
  { 181,   0, 525}, /*  2:                     nm shift  525 */
  { 107,   0, 797}, /*  3:                   SEMI reduce 242 */
  { 113,   0,  21}, /*  4:                 STRING shift  21 */
  {  74,   1,  22}, /*  5:                JOIN_KW shift  22 */
/* State 525 */
  { 107,   0, 798}, /*  1:                   SEMI reduce 243 */
/* State 526 */
  { 165,   0, 527}, /*  1:                    ids shift  527 */
  {  59,   0, 256}, /*  2:                     ID shift  256 */
  { 113,   2, 257}, /*  3:                 STRING shift  257 */
/* State 527 */
  {  45,   0, 528}, /*  1:                     EQ shift  528 */
  {  79,   0, 540}, /*  2:                     LP shift  540 */
  { 107,   0, 804}, /*  3:                   SEMI reduce 249 */
/* State 528 */
  {  90,   0, 530}, /*  1:                     ON shift  530 */
  { 181,   0, 529}, /*  2:                     nm shift  529 */
  {  83,   0, 537}, /*  3:                  MINUS shift  537 */
  { 113,   3,  21}, /*  4:                 STRING shift  21 */
  {  94,   6, 539}, /*  5:                   PLUS shift  539 */
  {  74,   0,  22}, /*  6:                JOIN_KW shift  22 */
  { 179,   8, 532}, /*  7:              minus_num shift  532 */
  {  59,   0,  20}, /*  8:                     ID shift  20 */
  { 188,   0, 531}, /*  9:               plus_num shift  531 */
  { 189,   7, 533}, /* 10:               plus_opt shift  533 */
/* State 529 */
  { 107,   0, 799}, /*  1:                   SEMI reduce 244 */
/* State 530 */
  { 107,   0, 800}, /*  1:                   SEMI reduce 245 */
/* State 531 */
  { 107,   0, 801}, /*  1:                   SEMI reduce 246 */
/* State 532 */
  { 107,   0, 802}, /*  1:                   SEMI reduce 247 */
/* State 533 */
  {  68,   0, 535}, /*  1:                INTEGER shift  535 */
  {  49,   0, 536}, /*  2:                  FLOAT shift  536 */
  { 182,   1, 534}, /*  3:                 number shift  534 */
/* State 534 */
  { 107,   0, 805}, /*  1:                   SEMI reduce 250 */
/* State 535 */
  { 107,   0, 807}, /*  1:                   SEMI reduce 252 */
/* State 536 */
  { 107,   0, 808}, /*  1:                   SEMI reduce 253 */
/* State 537 */
  {  68,   0, 535}, /*  1:                INTEGER shift  535 */
  {  49,   0, 536}, /*  2:                  FLOAT shift  536 */
  { 182,   1, 538}, /*  3:                 number shift  538 */
/* State 538 */
  { 107,   0, 806}, /*  1:                   SEMI reduce 251 */
/* State 540 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 541}, /*  2:                     nm shift  541 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 541 */
  { 104,   0, 542}, /*  1:                     RP shift  542 */
/* State 542 */
  { 107,   0, 803}, /*  1:                   SEMI reduce 248 */
/* State 543 */
  {  30,   0, 548}, /*  1:               DATABASE shift  548 */
  { 149,   0, 544}, /*  2:        database_kw_opt shift  544 */
/* State 544 */
  { 165,   0, 545}, /*  1:                    ids shift  545 */
  {  59,   0, 256}, /*  2:                     ID shift  256 */
  { 113,   2, 257}, /*  3:                 STRING shift  257 */
/* State 545 */
  {   6,   0, 546}, /*  1:                     AS shift  546 */
/* State 546 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 547}, /*  2:                     nm shift  547 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 547 */
  { 107,   0, 838}, /*  1:                   SEMI reduce 283 */
/* State 549 */
  {  30,   0, 548}, /*  1:               DATABASE shift  548 */
  { 149,   0, 550}, /*  2:        database_kw_opt shift  550 */
/* State 550 */
  { 113,   0,  21}, /*  1:                 STRING shift  21 */
  { 181,   1, 551}, /*  2:                     nm shift  551 */
  {  74,   0,  22}, /*  3:                JOIN_KW shift  22 */
  {  59,   0,  20}, /*  4:                     ID shift  20 */
/* State 551 */
  { 107,   0, 841}, /*  1:                   SEMI reduce 286 */
};

/* The state table contains information needed to look up the correct
** action in the action table, given the current state of the parser.
** Information needed includes:
**
**  +  A pointer to the start of the action hash table in yyActionTable.
**
**  +  The number of entries in the action hash table.
**
**  +  The default action.  This is the action to take if no entry for
**     the given look-ahead is found in the action hash table.
*/
struct yyStateEntry {
  const yyActionEntry *hashtbl;  /* Start of the hash table in yyActionTable */
  YYCODETYPE nEntry;             /* Number of entries in action hash table */
  YYACTIONTYPE actionDefault;    /* Default action if look-ahead not found */
};
typedef struct yyStateEntry yyStateEntry;
static const yyStateEntry yyStateTable[] = {
  { &yyActionTable[0],   6, 562 },
  { &yyActionTable[6],   5, 562 },
  { &yyActionTable[11],   0, 557 },
  { &yyActionTable[11],  22, 842 },
  { &yyActionTable[33],   1, 842 },
  { &yyActionTable[34],   0, 558 },
  { &yyActionTable[34],   1, 842 },
  { &yyActionTable[35],   2, 564 },
  { &yyActionTable[37],   3, 842 },
  { &yyActionTable[40],   1, 842 },
  { &yyActionTable[41],   1, 842 },
  { &yyActionTable[42],   6, 842 },
  { &yyActionTable[48],   0, 644 },
  { &yyActionTable[48],   0, 647 },
  { &yyActionTable[48],   0, 648 },
  { &yyActionTable[48],   0, 649 },
  { &yyActionTable[48],   0, 650 },
  { &yyActionTable[48],   0, 651 },
  { &yyActionTable[48],   4, 565 },
  { &yyActionTable[52],   0, 566 },
  { &yyActionTable[52],   0, 583 },
  { &yyActionTable[52],   0, 584 },
  { &yyActionTable[52],   0, 585 },
  { &yyActionTable[52],   3, 842 },
  { &yyActionTable[55],   1, 842 },
  { &yyActionTable[56],   3, 842 },
  { &yyActionTable[59],   1, 842 },
  { &yyActionTable[60],   3, 842 },
  { &yyActionTable[63],   1, 842 },
  { &yyActionTable[64],   3, 842 },
  { &yyActionTable[67],   1, 842 },
  { &yyActionTable[68],   7, 842 },
  { &yyActionTable[75],   3, 842 },
  { &yyActionTable[78],   1, 842 },
  { &yyActionTable[79],   1, 842 },
  { &yyActionTable[80],  13, 842 },
  { &yyActionTable[93],   0, 576 },
  { &yyActionTable[93],   5, 586 },
  { &yyActionTable[98],   1, 596 },
  { &yyActionTable[99],  13, 578 },
  { &yyActionTable[112],   0, 595 },
  { &yyActionTable[112],   4, 842 },
  { &yyActionTable[116],  10, 842 },
  { &yyActionTable[126],   0, 597 },
  { &yyActionTable[126],   2, 643 },
  { &yyActionTable[128],   0, 608 },
  { &yyActionTable[128],   2, 842 },
  { &yyActionTable[130],   2, 643 },
  { &yyActionTable[132],   0, 609 },
  { &yyActionTable[132],   2, 628 },
  { &yyActionTable[134],   0, 626 },
  { &yyActionTable[134],   2, 842 },
  { &yyActionTable[136],   0, 629 },
  { &yyActionTable[136],   0, 630 },
  { &yyActionTable[136],   1, 842 },
  { &yyActionTable[137],   3, 698 },
  { &yyActionTable[140],   2, 643 },
  { &yyActionTable[142],   0, 610 },
  { &yyActionTable[142],   0, 696 },
  { &yyActionTable[142],   0, 697 },
  { &yyActionTable[142],   2, 643 },
  { &yyActionTable[144],   0, 611 },
  { &yyActionTable[144],   1, 842 },
  { &yyActionTable[145],  15, 842 },
  { &yyActionTable[160],   2, 727 },
  { &yyActionTable[162],  18, 785 },
  { &yyActionTable[180],   1, 734 },
  { &yyActionTable[181],   1, 728 },
  { &yyActionTable[182],  18, 842 },
  { &yyActionTable[200],   0, 656 },
  { &yyActionTable[200],   5, 842 },
  { &yyActionTable[205],   2, 842 },
  { &yyActionTable[207],   0, 657 },
  { &yyActionTable[207],   3, 665 },
  { &yyActionTable[210],   2, 667 },
  { &yyActionTable[212],   3, 674 },
  { &yyActionTable[215],   2, 710 },
  { &yyActionTable[217],   2, 701 },
  { &yyActionTable[219],   2, 703 },
  { &yyActionTable[221],   2, 691 },
  { &yyActionTable[223],   2, 705 },
  { &yyActionTable[225],   0, 662 },
  { &yyActionTable[225],   4, 842 },
  { &yyActionTable[229],   0, 592 },
  { &yyActionTable[229],   1, 842 },
  { &yyActionTable[230],   0, 593 },
  { &yyActionTable[230],   1, 842 },
  { &yyActionTable[231],   0, 594 },
  { &yyActionTable[231],   2, 706 },
  { &yyActionTable[233],   4, 842 },
  { &yyActionTable[237],   0, 707 },
  { &yyActionTable[237],   4, 842 },
  { &yyActionTable[241],   0, 708 },
  { &yyActionTable[241],   1, 842 },
  { &yyActionTable[242],  17, 842 },
  { &yyActionTable[259],   1, 692 },
  { &yyActionTable[260],  16, 842 },
  { &yyActionTable[276],   2, 699 },
  { &yyActionTable[278],   3, 698 },
  { &yyActionTable[281],   0, 693 },
  { &yyActionTable[281],   2, 842 },
  { &yyActionTable[283],   0, 580 },
  { &yyActionTable[283],   0, 700 },
  { &yyActionTable[283],  28, 695 },
  { &yyActionTable[311],   0, 731 },
  { &yyActionTable[311],  15, 842 },
  { &yyActionTable[326],   0, 726 },
  { &yyActionTable[326],   1, 842 },
  { &yyActionTable[327],   4, 842 },
  { &yyActionTable[331],   1, 729 },
  { &yyActionTable[332],   4, 842 },
  { &yyActionTable[336],   0, 730 },
  { &yyActionTable[336],  26, 737 },
  { &yyActionTable[362],  15, 842 },
  { &yyActionTable[377],  27, 738 },
  { &yyActionTable[404],  15, 842 },
  { &yyActionTable[419],  12, 739 },
  { &yyActionTable[431],  15, 842 },
  { &yyActionTable[446],  12, 740 },
  { &yyActionTable[458],  15, 842 },
  { &yyActionTable[473],  12, 741 },
  { &yyActionTable[485],  15, 842 },
  { &yyActionTable[500],  12, 742 },
  { &yyActionTable[512],  15, 842 },
  { &yyActionTable[527],  16, 743 },
  { &yyActionTable[543],  15, 842 },
  { &yyActionTable[558],  16, 744 },
  { &yyActionTable[574],  15, 842 },
  { &yyActionTable[589],   8, 745 },
  { &yyActionTable[597],  15, 842 },
  { &yyActionTable[612],   8, 746 },
  { &yyActionTable[620],  15, 842 },
  { &yyActionTable[635],   8, 747 },
  { &yyActionTable[643],  15, 842 },
  { &yyActionTable[658],   8, 748 },
  { &yyActionTable[666],  15, 842 },
  { &yyActionTable[681],  16, 749 },
  { &yyActionTable[697],   6, 842 },
  { &yyActionTable[703],  15, 842 },
  { &yyActionTable[718],  16, 750 },
  { &yyActionTable[734],   0, 751 },
  { &yyActionTable[734],   0, 752 },
  { &yyActionTable[734],  15, 842 },
  { &yyActionTable[749],   6, 753 },
  { &yyActionTable[755],  15, 842 },
  { &yyActionTable[770],   6, 754 },
  { &yyActionTable[776],  15, 842 },
  { &yyActionTable[791],   3, 755 },
  { &yyActionTable[794],  15, 842 },
  { &yyActionTable[809],   3, 756 },
  { &yyActionTable[812],  15, 842 },
  { &yyActionTable[827],   3, 757 },
  { &yyActionTable[830],  15, 842 },
  { &yyActionTable[845],   2, 758 },
  { &yyActionTable[847],   0, 759 },
  { &yyActionTable[847],   2, 842 },
  { &yyActionTable[849],   0, 760 },
  { &yyActionTable[849],   1, 842 },
  { &yyActionTable[850],   0, 763 },
  { &yyActionTable[850],   0, 761 },
  { &yyActionTable[850],  15, 842 },
  { &yyActionTable[865],  28, 842 },
  { &yyActionTable[893],  15, 842 },
  { &yyActionTable[908],  16, 769 },
  { &yyActionTable[924],   1, 842 },
  { &yyActionTable[925],  20, 785 },
  { &yyActionTable[945],   5, 842 },
  { &yyActionTable[950],   2, 842 },
  { &yyActionTable[952],   1, 842 },
  { &yyActionTable[953],   1, 842 },
  { &yyActionTable[954],   1, 842 },
  { &yyActionTable[955],   0, 772 },
  { &yyActionTable[955],  28, 784 },
  { &yyActionTable[983],   0, 732 },
  { &yyActionTable[983],   0, 733 },
  { &yyActionTable[983],  15, 842 },
  { &yyActionTable[998],  26, 764 },
  { &yyActionTable[1024],  15, 842 },
  { &yyActionTable[1039],   2, 765 },
  { &yyActionTable[1041],  15, 842 },
  { &yyActionTable[1056],   2, 766 },
  { &yyActionTable[1058],  15, 842 },
  { &yyActionTable[1073],   2, 767 },
  { &yyActionTable[1075],  17, 842 },
  { &yyActionTable[1092],  29, 842 },
  { &yyActionTable[1121],   2, 842 },
  { &yyActionTable[1123],   4, 842 },
  { &yyActionTable[1127],   1, 842 },
  { &yyActionTable[1128],   0, 775 },
  { &yyActionTable[1128],  15, 842 },
  { &yyActionTable[1143],  29, 842 },
  { &yyActionTable[1172],  15, 842 },
  { &yyActionTable[1187],  28, 776 },
  { &yyActionTable[1215],   1, 842 },
  { &yyActionTable[1216],   4, 842 },
  { &yyActionTable[1220],   1, 842 },
  { &yyActionTable[1221],   0, 833 },
  { &yyActionTable[1221],   1, 842 },
  { &yyActionTable[1222],   4, 842 },
  { &yyActionTable[1226],   1, 842 },
  { &yyActionTable[1227],   0, 834 },
  { &yyActionTable[1227],   1, 842 },
  { &yyActionTable[1228],   4, 842 },
  { &yyActionTable[1232],   1, 842 },
  { &yyActionTable[1233],   0, 835 },
  { &yyActionTable[1233],   1, 842 },
  { &yyActionTable[1234],   4, 842 },
  { &yyActionTable[1238],   1, 842 },
  { &yyActionTable[1239],   0, 836 },
  { &yyActionTable[1239],  15, 842 },
  { &yyActionTable[1254],  29, 842 },
  { &yyActionTable[1283],  15, 842 },
  { &yyActionTable[1298],  29, 842 },
  { &yyActionTable[1327],  15, 842 },
  { &yyActionTable[1342],  28, 777 },
  { &yyActionTable[1370],   2, 842 },
  { &yyActionTable[1372],   0, 771 },
  { &yyActionTable[1372],  16, 785 },
  { &yyActionTable[1388],   0, 782 },
  { &yyActionTable[1388],   0, 783 },
  { &yyActionTable[1388],   0, 762 },
  { &yyActionTable[1388],  15, 842 },
  { &yyActionTable[1403],  28, 842 },
  { &yyActionTable[1431],  15, 842 },
  { &yyActionTable[1446],  26, 770 },
  { &yyActionTable[1472],   1, 842 },
  { &yyActionTable[1473],  20, 785 },
  { &yyActionTable[1493],   5, 842 },
  { &yyActionTable[1498],   0, 774 },
  { &yyActionTable[1498],   2, 842 },
  { &yyActionTable[1500],   0, 773 },
  { &yyActionTable[1500],   2, 699 },
  { &yyActionTable[1502],   3, 698 },
  { &yyActionTable[1505],   0, 694 },
  { &yyActionTable[1505],  15, 842 },
  { &yyActionTable[1520],  28, 704 },
  { &yyActionTable[1548],   1, 842 },
  { &yyActionTable[1549],  17, 785 },
  { &yyActionTable[1566],   1, 702 },
  { &yyActionTable[1567],  15, 842 },
  { &yyActionTable[1582],  28, 711 },
  { &yyActionTable[1610],   0, 666 },
  { &yyActionTable[1610],   2, 677 },
  { &yyActionTable[1612],   4, 675 },
  { &yyActionTable[1616],   0, 676 },
  { &yyActionTable[1616],   0, 682 },
  { &yyActionTable[1616],   0, 683 },
  { &yyActionTable[1616],   5, 842 },
  { &yyActionTable[1621],   0, 684 },
  { &yyActionTable[1621],   5, 842 },
  { &yyActionTable[1626],   0, 685 },
  { &yyActionTable[1626],   1, 842 },
  { &yyActionTable[1627],   0, 686 },
  { &yyActionTable[1627],   5, 842 },
  { &yyActionTable[1632],   2, 680 },
  { &yyActionTable[1634],   5, 673 },
  { &yyActionTable[1639],   0, 581 },
  { &yyActionTable[1639],   0, 582 },
  { &yyActionTable[1639],   4, 842 },
  { &yyActionTable[1643],   0, 671 },
  { &yyActionTable[1643],   0, 672 },
  { &yyActionTable[1643],   2, 688 },
  { &yyActionTable[1645],   2, 690 },
  { &yyActionTable[1647],   0, 678 },
  { &yyActionTable[1647],   1, 842 },
  { &yyActionTable[1648],   6, 842 },
  { &yyActionTable[1654],   2, 842 },
  { &yyActionTable[1656],   0, 689 },
  { &yyActionTable[1656],   5, 842 },
  { &yyActionTable[1661],   0, 791 },
  { &yyActionTable[1661],   3, 698 },
  { &yyActionTable[1664],   0, 793 },
  { &yyActionTable[1664],   0, 792 },
  { &yyActionTable[1664],  15, 842 },
  { &yyActionTable[1679],  28, 687 },
  { &yyActionTable[1707],   4, 842 },
  { &yyActionTable[1711],   0, 681 },
  { &yyActionTable[1711],   3, 842 },
  { &yyActionTable[1714],   5, 842 },
  { &yyActionTable[1719],   5, 673 },
  { &yyActionTable[1724],   2, 688 },
  { &yyActionTable[1726],   2, 690 },
  { &yyActionTable[1728],   0, 679 },
  { &yyActionTable[1728],  16, 842 },
  { &yyActionTable[1744],  33, 673 },
  { &yyActionTable[1777],   0, 668 },
  { &yyActionTable[1777],   0, 669 },
  { &yyActionTable[1777],   1, 842 },
  { &yyActionTable[1778],   5, 842 },
  { &yyActionTable[1783],   0, 670 },
  { &yyActionTable[1783],   0, 663 },
  { &yyActionTable[1783],   0, 664 },
  { &yyActionTable[1783],   0, 768 },
  { &yyActionTable[1783],  29, 842 },
  { &yyActionTable[1812],   0, 725 },
  { &yyActionTable[1812],   2, 842 },
  { &yyActionTable[1814],   0, 735 },
  { &yyActionTable[1814],   1, 842 },
  { &yyActionTable[1815],   0, 736 },
  { &yyActionTable[1815],  29, 842 },
  { &yyActionTable[1844],   2, 643 },
  { &yyActionTable[1846],   0, 612 },
  { &yyActionTable[1846],   4, 842 },
  { &yyActionTable[1850],   2, 789 },
  { &yyActionTable[1852],   1, 616 },
  { &yyActionTable[1853],   3, 613 },
  { &yyActionTable[1856],   0, 617 },
  { &yyActionTable[1856],   4, 842 },
  { &yyActionTable[1860],   0, 618 },
  { &yyActionTable[1860],   3, 842 },
  { &yyActionTable[1863],   4, 842 },
  { &yyActionTable[1867],   0, 619 },
  { &yyActionTable[1867],   2, 842 },
  { &yyActionTable[1869],   0, 622 },
  { &yyActionTable[1869],   0, 623 },
  { &yyActionTable[1869],   0, 624 },
  { &yyActionTable[1869],   0, 625 },
  { &yyActionTable[1869],   4, 842 },
  { &yyActionTable[1873],   0, 620 },
  { &yyActionTable[1873],   4, 842 },
  { &yyActionTable[1877],   0, 621 },
  { &yyActionTable[1877],   6, 842 },
  { &yyActionTable[1883],   2, 842 },
  { &yyActionTable[1885],   0, 790 },
  { &yyActionTable[1885],   0, 614 },
  { &yyActionTable[1885],   2, 842 },
  { &yyActionTable[1887],   0, 615 },
  { &yyActionTable[1887],   2, 628 },
  { &yyActionTable[1889],   0, 627 },
  { &yyActionTable[1889],   0, 598 },
  { &yyActionTable[1889],   7, 842 },
  { &yyActionTable[1896],   0, 599 },
  { &yyActionTable[1896],   0, 600 },
  { &yyActionTable[1896],   0, 601 },
  { &yyActionTable[1896],   2, 842 },
  { &yyActionTable[1898],   0, 602 },
  { &yyActionTable[1898],   0, 605 },
  { &yyActionTable[1898],   2, 842 },
  { &yyActionTable[1900],   0, 603 },
  { &yyActionTable[1900],   0, 606 },
  { &yyActionTable[1900],   0, 604 },
  { &yyActionTable[1900],   0, 607 },
  { &yyActionTable[1900],   4, 587 },
  { &yyActionTable[1904],   4, 842 },
  { &yyActionTable[1908],   2, 842 },
  { &yyActionTable[1910],   0, 588 },
  { &yyActionTable[1910],   4, 842 },
  { &yyActionTable[1914],   1, 842 },
  { &yyActionTable[1915],   0, 589 },
  { &yyActionTable[1915],   0, 591 },
  { &yyActionTable[1915],   0, 590 },
  { &yyActionTable[1915],   0, 579 },
  { &yyActionTable[1915],   8, 842 },
  { &yyActionTable[1923],   6, 842 },
  { &yyActionTable[1929],   0, 633 },
  { &yyActionTable[1929],   4, 842 },
  { &yyActionTable[1933],   0, 636 },
  { &yyActionTable[1933],   1, 842 },
  { &yyActionTable[1934],   1, 842 },
  { &yyActionTable[1935],   6, 842 },
  { &yyActionTable[1941],   2, 842 },
  { &yyActionTable[1943],   2, 643 },
  { &yyActionTable[1945],   0, 637 },
  { &yyActionTable[1945],   1, 842 },
  { &yyActionTable[1946],   6, 842 },
  { &yyActionTable[1952],   2, 842 },
  { &yyActionTable[1954],   2, 643 },
  { &yyActionTable[1956],   0, 638 },
  { &yyActionTable[1956],  15, 842 },
  { &yyActionTable[1971],  30, 643 },
  { &yyActionTable[2001],   0, 639 },
  { &yyActionTable[2001],   1, 842 },
  { &yyActionTable[2002],   1, 842 },
  { &yyActionTable[2003],   6, 842 },
  { &yyActionTable[2009],   2, 842 },
  { &yyActionTable[2011],   1, 842 },
  { &yyActionTable[2012],   4, 842 },
  { &yyActionTable[2016],   2, 789 },
  { &yyActionTable[2018],   1, 616 },
  { &yyActionTable[2019],   7, 641 },
  { &yyActionTable[2026],   1, 842 },
  { &yyActionTable[2027],   0, 640 },
  { &yyActionTable[2027],   0, 642 },
  { &yyActionTable[2027],   0, 634 },
  { &yyActionTable[2027],   0, 635 },
  { &yyActionTable[2027],   0, 577 },
  { &yyActionTable[2027],   3, 842 },
  { &yyActionTable[2030],   5, 842 },
  { &yyActionTable[2035],   3, 573 },
  { &yyActionTable[2038],   6, 842 },
  { &yyActionTable[2044],   4, 842 },
  { &yyActionTable[2048],   0, 571 },
  { &yyActionTable[2048],   4, 842 },
  { &yyActionTable[2052],   1, 842 },
  { &yyActionTable[2053],   3, 842 },
  { &yyActionTable[2056],   5, 842 },
  { &yyActionTable[2061],   1, 842 },
  { &yyActionTable[2062],   4, 842 },
  { &yyActionTable[2066],   1, 842 },
  { &yyActionTable[2067],   4, 842 },
  { &yyActionTable[2071],   3, 842 },
  { &yyActionTable[2074],   1, 842 },
  { &yyActionTable[2075],   6, 842 },
  { &yyActionTable[2081],   2, 842 },
  { &yyActionTable[2083],   3, 842 },
  { &yyActionTable[2086],   1, 842 },
  { &yyActionTable[2087],   1, 842 },
  { &yyActionTable[2088],   4, 842 },
  { &yyActionTable[2092],   4, 816 },
  { &yyActionTable[2096],   4, 842 },
  { &yyActionTable[2100],   1, 842 },
  { &yyActionTable[2101],   4, 842 },
  { &yyActionTable[2105],   2, 680 },
  { &yyActionTable[2107],   2, 821 },
  { &yyActionTable[2109],   3, 842 },
  { &yyActionTable[2112],   1, 842 },
  { &yyActionTable[2113],  15, 842 },
  { &yyActionTable[2128],  29, 842 },
  { &yyActionTable[2157],   1, 842 },
  { &yyActionTable[2158],   2, 842 },
  { &yyActionTable[2160],   0, 822 },
  { &yyActionTable[2160],   0, 823 },
  { &yyActionTable[2160],   1, 842 },
  { &yyActionTable[2161],   1, 842 },
  { &yyActionTable[2162],   2, 842 },
  { &yyActionTable[2164],   5, 842 },
  { &yyActionTable[2169],   2, 842 },
  { &yyActionTable[2171],   4, 842 },
  { &yyActionTable[2175],   0, 723 },
  { &yyActionTable[2175],   0, 724 },
  { &yyActionTable[2175],   0, 813 },
  { &yyActionTable[2175],   0, 814 },
  { &yyActionTable[2175],   1, 842 },
  { &yyActionTable[2176],   0, 815 },
  { &yyActionTable[2176],   0, 572 },
  { &yyActionTable[2176],   1, 842 },
  { &yyActionTable[2177],   9, 842 },
  { &yyActionTable[2186],   5, 842 },
  { &yyActionTable[2191],   1, 842 },
  { &yyActionTable[2192],   1, 842 },
  { &yyActionTable[2193],   1, 842 },
  { &yyActionTable[2194],   9, 842 },
  { &yyActionTable[2203],   1, 842 },
  { &yyActionTable[2204],   2, 645 },
  { &yyActionTable[2206],   6, 842 },
  { &yyActionTable[2212],   0, 646 },
  { &yyActionTable[2212],   4, 842 },
  { &yyActionTable[2216],   1, 842 },
  { &yyActionTable[2217],   5, 842 },
  { &yyActionTable[2222],   4, 842 },
  { &yyActionTable[2226],   4, 842 },
  { &yyActionTable[2230],   1, 842 },
  { &yyActionTable[2231],  15, 842 },
  { &yyActionTable[2246],  28, 713 },
  { &yyActionTable[2274],   1, 842 },
  { &yyActionTable[2275],   1, 842 },
  { &yyActionTable[2276],  15, 842 },
  { &yyActionTable[2291],  28, 714 },
  { &yyActionTable[2319],   3, 842 },
  { &yyActionTable[2322],   1, 842 },
  { &yyActionTable[2323],   4, 842 },
  { &yyActionTable[2327],   2, 721 },
  { &yyActionTable[2329],   5, 842 },
  { &yyActionTable[2334],   2, 842 },
  { &yyActionTable[2336],   0, 722 },
  { &yyActionTable[2336],   4, 842 },
  { &yyActionTable[2340],   5, 842 },
  { &yyActionTable[2345],   1, 842 },
  { &yyActionTable[2346],  16, 842 },
  { &yyActionTable[2362],   2, 842 },
  { &yyActionTable[2364],  15, 842 },
  { &yyActionTable[2379],  28, 719 },
  { &yyActionTable[2407],   1, 842 },
  { &yyActionTable[2408],  28, 720 },
  { &yyActionTable[2436],   1, 842 },
  { &yyActionTable[2437],   4, 842 },
  { &yyActionTable[2441],   3, 842 },
  { &yyActionTable[2444],   1, 842 },
  { &yyActionTable[2445],   4, 842 },
  { &yyActionTable[2449],   4, 842 },
  { &yyActionTable[2453],   1, 842 },
  { &yyActionTable[2454],   4, 842 },
  { &yyActionTable[2458],   1, 842 },
  { &yyActionTable[2459],   4, 842 },
  { &yyActionTable[2463],   3, 842 },
  { &yyActionTable[2466],   1, 842 },
  { &yyActionTable[2467],   4, 842 },
  { &yyActionTable[2471],   3, 842 },
  { &yyActionTable[2474],   1, 842 },
  { &yyActionTable[2475],   5, 842 },
  { &yyActionTable[2480],   1, 842 },
  { &yyActionTable[2481],   4, 842 },
  { &yyActionTable[2485],   2, 680 },
  { &yyActionTable[2487],   3, 842 },
  { &yyActionTable[2490],   1, 842 },
  { &yyActionTable[2491],   2, 645 },
  { &yyActionTable[2493],   4, 842 },
  { &yyActionTable[2497],   3, 842 },
  { &yyActionTable[2500],   1, 842 },
  { &yyActionTable[2501],   5, 842 },
  { &yyActionTable[2506],   4, 842 },
  { &yyActionTable[2510],   1, 842 },
  { &yyActionTable[2511],   1, 842 },
  { &yyActionTable[2512],   4, 842 },
  { &yyActionTable[2516],   2, 680 },
  { &yyActionTable[2518],   2, 721 },
  { &yyActionTable[2520],   4, 842 },
  { &yyActionTable[2524],   5, 842 },
  { &yyActionTable[2529],   1, 842 },
  { &yyActionTable[2530],  16, 842 },
  { &yyActionTable[2546],   2, 842 },
  { &yyActionTable[2548],   1, 842 },
  { &yyActionTable[2549],   3, 842 },
  { &yyActionTable[2552],   1, 842 },
  { &yyActionTable[2553],   1, 842 },
  { &yyActionTable[2554],   2, 645 },
  { &yyActionTable[2556],   4, 842 },
  { &yyActionTable[2560],   3, 842 },
  { &yyActionTable[2563],   1, 842 },
  { &yyActionTable[2564],   4, 842 },
  { &yyActionTable[2568],   2, 842 },
  { &yyActionTable[2570],   1, 842 },
  { &yyActionTable[2571],   1, 842 },
  { &yyActionTable[2572],   1, 842 },
  { &yyActionTable[2573],   5, 842 },
  { &yyActionTable[2578],   1, 842 },
  { &yyActionTable[2579],   3, 842 },
  { &yyActionTable[2582],   3, 842 },
  { &yyActionTable[2585],  10, 810 },
  { &yyActionTable[2595],   1, 842 },
  { &yyActionTable[2596],   1, 842 },
  { &yyActionTable[2597],   1, 842 },
  { &yyActionTable[2598],   1, 842 },
  { &yyActionTable[2599],   3, 842 },
  { &yyActionTable[2602],   1, 842 },
  { &yyActionTable[2603],   1, 842 },
  { &yyActionTable[2604],   1, 842 },
  { &yyActionTable[2605],   3, 842 },
  { &yyActionTable[2608],   1, 842 },
  { &yyActionTable[2609],   0, 809 },
  { &yyActionTable[2609],   4, 842 },
  { &yyActionTable[2613],   1, 842 },
  { &yyActionTable[2614],   1, 842 },
  { &yyActionTable[2615],   2, 840 },
  { &yyActionTable[2617],   3, 842 },
  { &yyActionTable[2620],   1, 842 },
  { &yyActionTable[2621],   4, 842 },
  { &yyActionTable[2625],   1, 842 },
  { &yyActionTable[2626],   0, 839 },
  { &yyActionTable[2626],   2, 840 },
  { &yyActionTable[2628],   4, 842 },
  { &yyActionTable[2632],   1, 842 },
  { &yyActionTable[2633],   0, 559 },
  { &yyActionTable[2633],   0, 561 },
  { &yyActionTable[2633],   0, 556 },
};

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammer, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
    0,  /*          $ => nothing */
   59,  /*      ABORT => ID */
   59,  /*      AFTER => ID */
    0,  /* AGG_FUNCTION => nothing */
    0,  /*        ALL => nothing */
    0,  /*        AND => nothing */
    0,  /*         AS => nothing */
   59,  /*        ASC => ID */
   59,  /*     ATTACH => ID */
   59,  /*     BEFORE => ID */
   59,  /*      BEGIN => ID */
    0,  /*    BETWEEN => nothing */
    0,  /*     BITAND => nothing */
    0,  /*     BITNOT => nothing */
    0,  /*      BITOR => nothing */
    0,  /*         BY => nothing */
   59,  /*    CASCADE => ID */
    0,  /*       CASE => nothing */
    0,  /*      CHECK => nothing */
   59,  /*    CLUSTER => ID */
    0,  /*    COLLATE => nothing */
    0,  /*     COLUMN => nothing */
    0,  /*      COMMA => nothing */
    0,  /*    COMMENT => nothing */
    0,  /*     COMMIT => nothing */
    0,  /*     CONCAT => nothing */
   59,  /*   CONFLICT => ID */
    0,  /* CONSTRAINT => nothing */
   59,  /*       COPY => ID */
    0,  /*     CREATE => nothing */
   59,  /*   DATABASE => ID */
    0,  /*    DEFAULT => nothing */
    0,  /* DEFERRABLE => nothing */
   59,  /*   DEFERRED => ID */
    0,  /*     DELETE => nothing */
   59,  /* DELIMITERS => ID */
   59,  /*       DESC => ID */
   59,  /*     DETACH => ID */
    0,  /*   DISTINCT => nothing */
    0,  /*        DOT => nothing */
    0,  /*       DROP => nothing */
   59,  /*       EACH => ID */
    0,  /*       ELSE => nothing */
   59,  /*        END => ID */
    0,  /* END_OF_FILE => nothing */
    0,  /*         EQ => nothing */
    0,  /*     EXCEPT => nothing */
   59,  /*    EXPLAIN => ID */
   59,  /*       FAIL => ID */
    0,  /*      FLOAT => nothing */
   59,  /*        FOR => ID */
    0,  /*    FOREIGN => nothing */
    0,  /*       FROM => nothing */
    0,  /*   FUNCTION => nothing */
    0,  /*         GE => nothing */
    0,  /*       GLOB => nothing */
    0,  /*      GROUP => nothing */
    0,  /*         GT => nothing */
    0,  /*     HAVING => nothing */
    0,  /*         ID => nothing */
   59,  /*     IGNORE => ID */
    0,  /*    ILLEGAL => nothing */
   59,  /*  IMMEDIATE => ID */
    0,  /*         IN => nothing */
    0,  /*      INDEX => nothing */
   59,  /*  INITIALLY => ID */
    0,  /*     INSERT => nothing */
   59,  /*    INSTEAD => ID */
    0,  /*    INTEGER => nothing */
    0,  /*  INTERSECT => nothing */
    0,  /*       INTO => nothing */
    0,  /*         IS => nothing */
    0,  /*     ISNULL => nothing */
    0,  /*       JOIN => nothing */
    0,  /*    JOIN_KW => nothing */
   59,  /*        KEY => ID */
    0,  /*         LE => nothing */
    0,  /*       LIKE => nothing */
    0,  /*      LIMIT => nothing */
    0,  /*         LP => nothing */
    0,  /*     LSHIFT => nothing */
    0,  /*         LT => nothing */
   59,  /*      MATCH => ID */
    0,  /*      MINUS => nothing */
    0,  /*         NE => nothing */
    0,  /*        NOT => nothing */
    0,  /*    NOTNULL => nothing */
    0,  /*       NULL => nothing */
   59,  /*         OF => ID */
   59,  /*     OFFSET => ID */
    0,  /*         ON => nothing */
    0,  /*         OR => nothing */
    0,  /* ORACLE_OUTER_JOIN => nothing */
    0,  /*      ORDER => nothing */
    0,  /*       PLUS => nothing */
   59,  /*     PRAGMA => ID */
    0,  /*    PRIMARY => nothing */
   59,  /*      RAISE => ID */
    0,  /* REFERENCES => nothing */
    0,  /*        REM => nothing */
   59,  /*    REPLACE => ID */
   59,  /*   RESTRICT => ID */
    0,  /*   ROLLBACK => nothing */
   59,  /*        ROW => ID */
    0,  /*         RP => nothing */
    0,  /*     RSHIFT => nothing */
    0,  /*     SELECT => nothing */
    0,  /*       SEMI => nothing */
    0,  /*        SET => nothing */
    0,  /*      SLASH => nothing */
    0,  /*      SPACE => nothing */
    0,  /*       STAR => nothing */
   59,  /*  STATEMENT => ID */
    0,  /*     STRING => nothing */
    0,  /*      TABLE => nothing */
   59,  /*       TEMP => ID */
    0,  /*       THEN => nothing */
    0,  /* TRANSACTION => nothing */
   59,  /*    TRIGGER => ID */
    0,  /*     UMINUS => nothing */
    0,  /* UNCLOSED_STRING => nothing */
    0,  /*      UNION => nothing */
    0,  /*     UNIQUE => nothing */
    0,  /*     UPDATE => nothing */
    0,  /*      UPLUS => nothing */
    0,  /*      USING => nothing */
   59,  /*     VACUUM => ID */
    0,  /*     VALUES => nothing */
   59,  /*       VIEW => ID */
    0,  /*       WHEN => nothing */
    0,  /*      WHERE => nothing */
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  int stateno;       /* The state-number */
  int major;         /* The major token value.  This is the code
                     ** number for the token at this stack level */
  YYMINORTYPE minor; /* The user-supplied minor token value.  This
                     ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
  int yyerrcnt;                 /* Shifts left before out of the error */
  yyStackEntry *yytop;          /* Pointer to the top stack element */
  sqliteParserARG_SDECL                /* A place to hold %extra_argument */
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void sqliteParserTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *yyTokenName[] = { 
  "$",             "ABORT",         "AFTER",         "AGG_FUNCTION",
  "ALL",           "AND",           "AS",            "ASC",         
  "ATTACH",        "BEFORE",        "BEGIN",         "BETWEEN",     
  "BITAND",        "BITNOT",        "BITOR",         "BY",          
  "CASCADE",       "CASE",          "CHECK",         "CLUSTER",     
  "COLLATE",       "COLUMN",        "COMMA",         "COMMENT",     
  "COMMIT",        "CONCAT",        "CONFLICT",      "CONSTRAINT",  
  "COPY",          "CREATE",        "DATABASE",      "DEFAULT",     
  "DEFERRABLE",    "DEFERRED",      "DELETE",        "DELIMITERS",  
  "DESC",          "DETACH",        "DISTINCT",      "DOT",         
  "DROP",          "EACH",          "ELSE",          "END",         
  "END_OF_FILE",   "EQ",            "EXCEPT",        "EXPLAIN",     
  "FAIL",          "FLOAT",         "FOR",           "FOREIGN",     
  "FROM",          "FUNCTION",      "GE",            "GLOB",        
  "GROUP",         "GT",            "HAVING",        "ID",          
  "IGNORE",        "ILLEGAL",       "IMMEDIATE",     "IN",          
  "INDEX",         "INITIALLY",     "INSERT",        "INSTEAD",     
  "INTEGER",       "INTERSECT",     "INTO",          "IS",          
  "ISNULL",        "JOIN",          "JOIN_KW",       "KEY",         
  "LE",            "LIKE",          "LIMIT",         "LP",          
  "LSHIFT",        "LT",            "MATCH",         "MINUS",       
  "NE",            "NOT",           "NOTNULL",       "NULL",        
  "OF",            "OFFSET",        "ON",            "OR",          
  "ORACLE_OUTER_JOIN",  "ORDER",         "PLUS",          "PRAGMA",      
  "PRIMARY",       "RAISE",         "REFERENCES",    "REM",         
  "REPLACE",       "RESTRICT",      "ROLLBACK",      "ROW",         
  "RP",            "RSHIFT",        "SELECT",        "SEMI",        
  "SET",           "SLASH",         "SPACE",         "STAR",        
  "STATEMENT",     "STRING",        "TABLE",         "TEMP",        
  "THEN",          "TRANSACTION",   "TRIGGER",       "UMINUS",      
  "UNCLOSED_STRING",  "UNION",         "UNIQUE",        "UPDATE",      
  "UPLUS",         "USING",         "VACUUM",        "VALUES",      
  "VIEW",          "WHEN",          "WHERE",         "as",          
  "carg",          "carglist",      "case_else",     "case_exprlist",
  "case_operand",  "ccons",         "cmd",           "cmdlist",     
  "cmdx",          "collate",       "column",        "columnid",    
  "columnlist",    "conslist",      "conslist_opt",  "create_table",
  "create_table_args",  "database_kw_opt",  "dbnm",          "defer_subclause",
  "defer_subclause_opt",  "distinct",      "ecmd",          "error",       
  "explain",       "expr",          "expritem",      "exprlist",    
  "foreach_clause",  "from",          "groupby_opt",   "having_opt",  
  "id",            "ids",           "idxitem",       "idxlist",     
  "idxlist_opt",   "init_deferred_pred_opt",  "input",         "inscollist",  
  "inscollist_opt",  "insert_cmd",    "itemlist",      "joinop",      
  "joinop2",       "likeop",        "limit_opt",     "minus_num",   
  "multiselect_op",  "nm",            "number",        "on_opt",      
  "onconf",        "oneselect",     "orconf",        "orderby_opt", 
  "plus_num",      "plus_opt",      "refact",        "refarg",      
  "refargs",       "resolvetype",   "sclp",          "selcollist",  
  "select",        "seltablist",    "setlist",       "signed",      
  "sortitem",      "sortlist",      "sortorder",     "stl_prefix",  
  "tcons",         "temp",          "trans_opt",     "trigger_cmd", 
  "trigger_cmd_list",  "trigger_decl",  "trigger_event",  "trigger_time",
  "type",          "typename",      "uniqueflag",    "using_opt",   
  "when_clause",   "where_opt",   
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *yyRuleName[] = {
 /*   0 */ "input ::= cmdlist",
 /*   1 */ "cmdlist ::= ecmd",
 /*   2 */ "cmdlist ::= cmdlist ecmd",
 /*   3 */ "ecmd ::= explain cmdx SEMI",
 /*   4 */ "ecmd ::= SEMI",
 /*   5 */ "cmdx ::= cmd",
 /*   6 */ "explain ::= EXPLAIN",
 /*   7 */ "explain ::=",
 /*   8 */ "cmd ::= BEGIN trans_opt onconf",
 /*   9 */ "trans_opt ::=",
 /*  10 */ "trans_opt ::= TRANSACTION",
 /*  11 */ "trans_opt ::= TRANSACTION nm",
 /*  12 */ "cmd ::= COMMIT trans_opt",
 /*  13 */ "cmd ::= END trans_opt",
 /*  14 */ "cmd ::= ROLLBACK trans_opt",
 /*  15 */ "cmd ::= create_table create_table_args",
 /*  16 */ "create_table ::= CREATE temp TABLE nm",
 /*  17 */ "temp ::= TEMP",
 /*  18 */ "temp ::=",
 /*  19 */ "create_table_args ::= LP columnlist conslist_opt RP",
 /*  20 */ "create_table_args ::= AS select",
 /*  21 */ "columnlist ::= columnlist COMMA column",
 /*  22 */ "columnlist ::= column",
 /*  23 */ "column ::= columnid type carglist",
 /*  24 */ "columnid ::= nm",
 /*  25 */ "id ::= ID",
 /*  26 */ "ids ::= ID",
 /*  27 */ "ids ::= STRING",
 /*  28 */ "nm ::= ID",
 /*  29 */ "nm ::= STRING",
 /*  30 */ "nm ::= JOIN_KW",
 /*  31 */ "type ::=",
 /*  32 */ "type ::= typename",
 /*  33 */ "type ::= typename LP signed RP",
 /*  34 */ "type ::= typename LP signed COMMA signed RP",
 /*  35 */ "typename ::= ids",
 /*  36 */ "typename ::= typename ids",
 /*  37 */ "signed ::= INTEGER",
 /*  38 */ "signed ::= PLUS INTEGER",
 /*  39 */ "signed ::= MINUS INTEGER",
 /*  40 */ "carglist ::= carglist carg",
 /*  41 */ "carglist ::=",
 /*  42 */ "carg ::= CONSTRAINT nm ccons",
 /*  43 */ "carg ::= ccons",
 /*  44 */ "carg ::= DEFAULT STRING",
 /*  45 */ "carg ::= DEFAULT ID",
 /*  46 */ "carg ::= DEFAULT INTEGER",
 /*  47 */ "carg ::= DEFAULT PLUS INTEGER",
 /*  48 */ "carg ::= DEFAULT MINUS INTEGER",
 /*  49 */ "carg ::= DEFAULT FLOAT",
 /*  50 */ "carg ::= DEFAULT PLUS FLOAT",
 /*  51 */ "carg ::= DEFAULT MINUS FLOAT",
 /*  52 */ "carg ::= DEFAULT NULL",
 /*  53 */ "ccons ::= NULL onconf",
 /*  54 */ "ccons ::= NOT NULL onconf",
 /*  55 */ "ccons ::= PRIMARY KEY sortorder onconf",
 /*  56 */ "ccons ::= UNIQUE onconf",
 /*  57 */ "ccons ::= CHECK LP expr RP onconf",
 /*  58 */ "ccons ::= REFERENCES nm idxlist_opt refargs",
 /*  59 */ "ccons ::= defer_subclause",
 /*  60 */ "ccons ::= COLLATE id",
 /*  61 */ "refargs ::=",
 /*  62 */ "refargs ::= refargs refarg",
 /*  63 */ "refarg ::= MATCH nm",
 /*  64 */ "refarg ::= ON DELETE refact",
 /*  65 */ "refarg ::= ON UPDATE refact",
 /*  66 */ "refarg ::= ON INSERT refact",
 /*  67 */ "refact ::= SET NULL",
 /*  68 */ "refact ::= SET DEFAULT",
 /*  69 */ "refact ::= CASCADE",
 /*  70 */ "refact ::= RESTRICT",
 /*  71 */ "defer_subclause ::= NOT DEFERRABLE init_deferred_pred_opt",
 /*  72 */ "defer_subclause ::= DEFERRABLE init_deferred_pred_opt",
 /*  73 */ "init_deferred_pred_opt ::=",
 /*  74 */ "init_deferred_pred_opt ::= INITIALLY DEFERRED",
 /*  75 */ "init_deferred_pred_opt ::= INITIALLY IMMEDIATE",
 /*  76 */ "conslist_opt ::=",
 /*  77 */ "conslist_opt ::= COMMA conslist",
 /*  78 */ "conslist ::= conslist COMMA tcons",
 /*  79 */ "conslist ::= conslist tcons",
 /*  80 */ "conslist ::= tcons",
 /*  81 */ "tcons ::= CONSTRAINT nm",
 /*  82 */ "tcons ::= PRIMARY KEY LP idxlist RP onconf",
 /*  83 */ "tcons ::= UNIQUE LP idxlist RP onconf",
 /*  84 */ "tcons ::= CHECK expr onconf",
 /*  85 */ "tcons ::= FOREIGN KEY LP idxlist RP REFERENCES nm idxlist_opt refargs defer_subclause_opt",
 /*  86 */ "defer_subclause_opt ::=",
 /*  87 */ "defer_subclause_opt ::= defer_subclause",
 /*  88 */ "onconf ::=",
 /*  89 */ "onconf ::= ON CONFLICT resolvetype",
 /*  90 */ "orconf ::=",
 /*  91 */ "orconf ::= OR resolvetype",
 /*  92 */ "resolvetype ::= ROLLBACK",
 /*  93 */ "resolvetype ::= ABORT",
 /*  94 */ "resolvetype ::= FAIL",
 /*  95 */ "resolvetype ::= IGNORE",
 /*  96 */ "resolvetype ::= REPLACE",
 /*  97 */ "cmd ::= DROP TABLE nm",
 /*  98 */ "cmd ::= CREATE temp VIEW nm AS select",
 /*  99 */ "cmd ::= DROP VIEW nm",
 /* 100 */ "cmd ::= select",
 /* 101 */ "select ::= oneselect",
 /* 102 */ "select ::= select multiselect_op oneselect",
 /* 103 */ "multiselect_op ::= UNION",
 /* 104 */ "multiselect_op ::= UNION ALL",
 /* 105 */ "multiselect_op ::= INTERSECT",
 /* 106 */ "multiselect_op ::= EXCEPT",
 /* 107 */ "oneselect ::= SELECT distinct selcollist from where_opt groupby_opt having_opt orderby_opt limit_opt",
 /* 108 */ "distinct ::= DISTINCT",
 /* 109 */ "distinct ::= ALL",
 /* 110 */ "distinct ::=",
 /* 111 */ "sclp ::= selcollist COMMA",
 /* 112 */ "sclp ::=",
 /* 113 */ "selcollist ::= sclp expr as",
 /* 114 */ "selcollist ::= sclp STAR",
 /* 115 */ "selcollist ::= sclp nm DOT STAR",
 /* 116 */ "as ::= AS nm",
 /* 117 */ "as ::= ids",
 /* 118 */ "as ::=",
 /* 119 */ "from ::=",
 /* 120 */ "from ::= FROM seltablist",
 /* 121 */ "stl_prefix ::= seltablist joinop",
 /* 122 */ "stl_prefix ::=",
 /* 123 */ "seltablist ::= stl_prefix nm dbnm as on_opt using_opt",
 /* 124 */ "seltablist ::= stl_prefix LP select RP as on_opt using_opt",
 /* 125 */ "dbnm ::=",
 /* 126 */ "dbnm ::= DOT nm",
 /* 127 */ "joinop ::= COMMA",
 /* 128 */ "joinop ::= JOIN",
 /* 129 */ "joinop ::= JOIN_KW JOIN",
 /* 130 */ "joinop ::= JOIN_KW nm JOIN",
 /* 131 */ "joinop ::= JOIN_KW nm nm JOIN",
 /* 132 */ "on_opt ::= ON expr",
 /* 133 */ "on_opt ::=",
 /* 134 */ "using_opt ::= USING LP idxlist RP",
 /* 135 */ "using_opt ::=",
 /* 136 */ "orderby_opt ::=",
 /* 137 */ "orderby_opt ::= ORDER BY sortlist",
 /* 138 */ "sortlist ::= sortlist COMMA sortitem collate sortorder",
 /* 139 */ "sortlist ::= sortitem collate sortorder",
 /* 140 */ "sortitem ::= expr",
 /* 141 */ "sortorder ::= ASC",
 /* 142 */ "sortorder ::= DESC",
 /* 143 */ "sortorder ::=",
 /* 144 */ "collate ::=",
 /* 145 */ "collate ::= COLLATE id",
 /* 146 */ "groupby_opt ::=",
 /* 147 */ "groupby_opt ::= GROUP BY exprlist",
 /* 148 */ "having_opt ::=",
 /* 149 */ "having_opt ::= HAVING expr",
 /* 150 */ "limit_opt ::=",
 /* 151 */ "limit_opt ::= LIMIT signed",
 /* 152 */ "limit_opt ::= LIMIT signed OFFSET signed",
 /* 153 */ "limit_opt ::= LIMIT signed COMMA signed",
 /* 154 */ "cmd ::= DELETE FROM nm dbnm where_opt",
 /* 155 */ "where_opt ::=",
 /* 156 */ "where_opt ::= WHERE expr",
 /* 157 */ "cmd ::= UPDATE orconf nm dbnm SET setlist where_opt",
 /* 158 */ "setlist ::= setlist COMMA nm EQ expr",
 /* 159 */ "setlist ::= nm EQ expr",
 /* 160 */ "cmd ::= insert_cmd INTO nm dbnm inscollist_opt VALUES LP itemlist RP",
 /* 161 */ "cmd ::= insert_cmd INTO nm dbnm inscollist_opt select",
 /* 162 */ "insert_cmd ::= INSERT orconf",
 /* 163 */ "insert_cmd ::= REPLACE",
 /* 164 */ "itemlist ::= itemlist COMMA expr",
 /* 165 */ "itemlist ::= expr",
 /* 166 */ "inscollist_opt ::=",
 /* 167 */ "inscollist_opt ::= LP inscollist RP",
 /* 168 */ "inscollist ::= inscollist COMMA nm",
 /* 169 */ "inscollist ::= nm",
 /* 170 */ "expr ::= LP expr RP",
 /* 171 */ "expr ::= NULL",
 /* 172 */ "expr ::= ID",
 /* 173 */ "expr ::= JOIN_KW",
 /* 174 */ "expr ::= nm DOT nm",
 /* 175 */ "expr ::= nm DOT nm DOT nm",
 /* 176 */ "expr ::= expr ORACLE_OUTER_JOIN",
 /* 177 */ "expr ::= INTEGER",
 /* 178 */ "expr ::= FLOAT",
 /* 179 */ "expr ::= STRING",
 /* 180 */ "expr ::= ID LP exprlist RP",
 /* 181 */ "expr ::= ID LP STAR RP",
 /* 182 */ "expr ::= expr AND expr",
 /* 183 */ "expr ::= expr OR expr",
 /* 184 */ "expr ::= expr LT expr",
 /* 185 */ "expr ::= expr GT expr",
 /* 186 */ "expr ::= expr LE expr",
 /* 187 */ "expr ::= expr GE expr",
 /* 188 */ "expr ::= expr NE expr",
 /* 189 */ "expr ::= expr EQ expr",
 /* 190 */ "expr ::= expr BITAND expr",
 /* 191 */ "expr ::= expr BITOR expr",
 /* 192 */ "expr ::= expr LSHIFT expr",
 /* 193 */ "expr ::= expr RSHIFT expr",
 /* 194 */ "expr ::= expr likeop expr",
 /* 195 */ "expr ::= expr NOT likeop expr",
 /* 196 */ "likeop ::= LIKE",
 /* 197 */ "likeop ::= GLOB",
 /* 198 */ "expr ::= expr PLUS expr",
 /* 199 */ "expr ::= expr MINUS expr",
 /* 200 */ "expr ::= expr STAR expr",
 /* 201 */ "expr ::= expr SLASH expr",
 /* 202 */ "expr ::= expr REM expr",
 /* 203 */ "expr ::= expr CONCAT expr",
 /* 204 */ "expr ::= expr ISNULL",
 /* 205 */ "expr ::= expr IS NULL",
 /* 206 */ "expr ::= expr NOTNULL",
 /* 207 */ "expr ::= expr NOT NULL",
 /* 208 */ "expr ::= expr IS NOT NULL",
 /* 209 */ "expr ::= NOT expr",
 /* 210 */ "expr ::= BITNOT expr",
 /* 211 */ "expr ::= MINUS expr",
 /* 212 */ "expr ::= PLUS expr",
 /* 213 */ "expr ::= LP select RP",
 /* 214 */ "expr ::= expr BETWEEN expr AND expr",
 /* 215 */ "expr ::= expr NOT BETWEEN expr AND expr",
 /* 216 */ "expr ::= expr IN LP exprlist RP",
 /* 217 */ "expr ::= expr IN LP select RP",
 /* 218 */ "expr ::= expr NOT IN LP exprlist RP",
 /* 219 */ "expr ::= expr NOT IN LP select RP",
 /* 220 */ "expr ::= CASE case_operand case_exprlist case_else END",
 /* 221 */ "case_exprlist ::= case_exprlist WHEN expr THEN expr",
 /* 222 */ "case_exprlist ::= WHEN expr THEN expr",
 /* 223 */ "case_else ::= ELSE expr",
 /* 224 */ "case_else ::=",
 /* 225 */ "case_operand ::= expr",
 /* 226 */ "case_operand ::=",
 /* 227 */ "exprlist ::= exprlist COMMA expritem",
 /* 228 */ "exprlist ::= expritem",
 /* 229 */ "expritem ::= expr",
 /* 230 */ "expritem ::=",
 /* 231 */ "cmd ::= CREATE temp uniqueflag INDEX nm ON nm dbnm LP idxlist RP onconf",
 /* 232 */ "uniqueflag ::= UNIQUE",
 /* 233 */ "uniqueflag ::=",
 /* 234 */ "idxlist_opt ::=",
 /* 235 */ "idxlist_opt ::= LP idxlist RP",
 /* 236 */ "idxlist ::= idxlist COMMA idxitem",
 /* 237 */ "idxlist ::= idxitem",
 /* 238 */ "idxitem ::= nm sortorder",
 /* 239 */ "cmd ::= DROP INDEX nm dbnm",
 /* 240 */ "cmd ::= COPY orconf nm dbnm FROM nm USING DELIMITERS STRING",
 /* 241 */ "cmd ::= COPY orconf nm dbnm FROM nm",
 /* 242 */ "cmd ::= VACUUM",
 /* 243 */ "cmd ::= VACUUM nm",
 /* 244 */ "cmd ::= PRAGMA ids EQ nm",
 /* 245 */ "cmd ::= PRAGMA ids EQ ON",
 /* 246 */ "cmd ::= PRAGMA ids EQ plus_num",
 /* 247 */ "cmd ::= PRAGMA ids EQ minus_num",
 /* 248 */ "cmd ::= PRAGMA ids LP nm RP",
 /* 249 */ "cmd ::= PRAGMA ids",
 /* 250 */ "plus_num ::= plus_opt number",
 /* 251 */ "minus_num ::= MINUS number",
 /* 252 */ "number ::= INTEGER",
 /* 253 */ "number ::= FLOAT",
 /* 254 */ "plus_opt ::= PLUS",
 /* 255 */ "plus_opt ::=",
 /* 256 */ "cmd ::= CREATE trigger_decl BEGIN trigger_cmd_list END",
 /* 257 */ "trigger_decl ::= temp TRIGGER nm trigger_time trigger_event ON nm dbnm foreach_clause when_clause",
 /* 258 */ "trigger_time ::= BEFORE",
 /* 259 */ "trigger_time ::= AFTER",
 /* 260 */ "trigger_time ::= INSTEAD OF",
 /* 261 */ "trigger_time ::=",
 /* 262 */ "trigger_event ::= DELETE",
 /* 263 */ "trigger_event ::= INSERT",
 /* 264 */ "trigger_event ::= UPDATE",
 /* 265 */ "trigger_event ::= UPDATE OF inscollist",
 /* 266 */ "foreach_clause ::=",
 /* 267 */ "foreach_clause ::= FOR EACH ROW",
 /* 268 */ "foreach_clause ::= FOR EACH STATEMENT",
 /* 269 */ "when_clause ::=",
 /* 270 */ "when_clause ::= WHEN expr",
 /* 271 */ "trigger_cmd_list ::= trigger_cmd SEMI trigger_cmd_list",
 /* 272 */ "trigger_cmd_list ::=",
 /* 273 */ "trigger_cmd ::= UPDATE orconf nm SET setlist where_opt",
 /* 274 */ "trigger_cmd ::= INSERT orconf INTO nm inscollist_opt VALUES LP itemlist RP",
 /* 275 */ "trigger_cmd ::= INSERT orconf INTO nm inscollist_opt select",
 /* 276 */ "trigger_cmd ::= DELETE FROM nm where_opt",
 /* 277 */ "trigger_cmd ::= select",
 /* 278 */ "expr ::= RAISE LP IGNORE RP",
 /* 279 */ "expr ::= RAISE LP ROLLBACK COMMA nm RP",
 /* 280 */ "expr ::= RAISE LP ABORT COMMA nm RP",
 /* 281 */ "expr ::= RAISE LP FAIL COMMA nm RP",
 /* 282 */ "cmd ::= DROP TRIGGER nm dbnm",
 /* 283 */ "cmd ::= ATTACH database_kw_opt ids AS nm",
 /* 284 */ "database_kw_opt ::= DATABASE",
 /* 285 */ "database_kw_opt ::=",
 /* 286 */ "cmd ::= DETACH database_kw_opt nm",
};
#endif /* NDEBUG */

/*
** This function returns the symbolic name associated with a token
** value.
*/
const char *sqliteParserTokenName(int tokenType){
#ifndef NDEBUG
  if( tokenType>0 && tokenType<(sizeof(yyTokenName)/sizeof(yyTokenName[0])) ){
    return yyTokenName[tokenType];
  }else{
    return "Unknown";
  }
#else
  return "";
#endif
}

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to sqliteParser and sqliteParserFree.
*/
void *sqliteParserAlloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(YYCODETYPE yymajor, YYMINORTYPE *yypminor){
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
    case 135:
#line 674 "parse.y"
{sqliteExprListDelete((yypminor->yy62));}
#line 4389 "parse.c"
      break;
    case 157:
#line 521 "parse.y"
{sqliteExprDelete((yypminor->yy334));}
#line 4394 "parse.c"
      break;
    case 158:
#line 693 "parse.y"
{sqliteExprDelete((yypminor->yy334));}
#line 4399 "parse.c"
      break;
    case 159:
#line 691 "parse.y"
{sqliteExprListDelete((yypminor->yy62));}
#line 4404 "parse.c"
      break;
    case 161:
#line 342 "parse.y"
{sqliteSrcListDelete((yypminor->yy335));}
#line 4409 "parse.c"
      break;
    case 162:
#line 436 "parse.y"
{sqliteExprListDelete((yypminor->yy62));}
#line 4414 "parse.c"
      break;
    case 163:
#line 441 "parse.y"
{sqliteExprDelete((yypminor->yy334));}
#line 4419 "parse.c"
      break;
    case 167:
#line 716 "parse.y"
{sqliteIdListDelete((yypminor->yy92));}
#line 4424 "parse.c"
      break;
    case 168:
#line 718 "parse.y"
{sqliteIdListDelete((yypminor->yy92));}
#line 4429 "parse.c"
      break;
    case 171:
#line 499 "parse.y"
{sqliteIdListDelete((yypminor->yy92));}
#line 4434 "parse.c"
      break;
    case 172:
#line 497 "parse.y"
{sqliteIdListDelete((yypminor->yy92));}
#line 4439 "parse.c"
      break;
    case 174:
#line 491 "parse.y"
{sqliteExprListDelete((yypminor->yy62));}
#line 4444 "parse.c"
      break;
    case 183:
#line 397 "parse.y"
{sqliteExprDelete((yypminor->yy334));}
#line 4449 "parse.c"
      break;
    case 185:
#line 277 "parse.y"
{sqliteSelectDelete((yypminor->yy11));}
#line 4454 "parse.c"
      break;
    case 187:
#line 408 "parse.y"
{sqliteExprListDelete((yypminor->yy62));}
#line 4459 "parse.c"
      break;
    case 194:
#line 313 "parse.y"
{sqliteExprListDelete((yypminor->yy62));}
#line 4464 "parse.c"
      break;
    case 195:
#line 311 "parse.y"
{sqliteExprListDelete((yypminor->yy62));}
#line 4469 "parse.c"
      break;
    case 196:
#line 275 "parse.y"
{sqliteSelectDelete((yypminor->yy11));}
#line 4474 "parse.c"
      break;
    case 197:
#line 338 "parse.y"
{sqliteSrcListDelete((yypminor->yy335));}
#line 4479 "parse.c"
      break;
    case 198:
#line 466 "parse.y"
{sqliteExprListDelete((yypminor->yy62));}
#line 4484 "parse.c"
      break;
    case 200:
#line 412 "parse.y"
{sqliteExprDelete((yypminor->yy334));}
#line 4489 "parse.c"
      break;
    case 201:
#line 410 "parse.y"
{sqliteExprListDelete((yypminor->yy62));}
#line 4494 "parse.c"
      break;
    case 203:
#line 340 "parse.y"
{sqliteSrcListDelete((yypminor->yy335));}
#line 4499 "parse.c"
      break;
    case 207:
#line 808 "parse.y"
{sqliteDeleteTriggerStep((yypminor->yy347));}
#line 4504 "parse.c"
      break;
    case 208:
#line 800 "parse.y"
{sqliteDeleteTriggerStep((yypminor->yy347));}
#line 4509 "parse.c"
      break;
    case 210:
#line 784 "parse.y"
{sqliteIdListDelete((yypminor->yy234).b);}
#line 4514 "parse.c"
      break;
    case 215:
#line 402 "parse.y"
{sqliteIdListDelete((yypminor->yy92));}
#line 4519 "parse.c"
      break;
    case 217:
#line 460 "parse.y"
{sqliteExprDelete((yypminor->yy334));}
#line 4524 "parse.c"
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[pParser->yytop->major]);
  }
#endif
  yymajor = pParser->yytop->major;
  yy_destructor( yymajor, &pParser->yytop->minor);
  pParser->yyidx--;
  pParser->yytop--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from sqliteParserAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void sqliteParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
  (*freeProc)((void*)pParser);
}

/*
** Find the appropriate action for a parser given the look-ahead token.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_parser_action(
  yyParser *pParser,        /* The parser */
  int iLookAhead             /* The look-ahead token */
){
  const yyStateEntry *pState;   /* Appropriate entry in the state table */
  const yyActionEntry *pAction; /* Action appropriate for the look-ahead */
  int iFallback;                /* Fallback token */
 
  /* if( pParser->yyidx<0 ) return YY_NO_ACTION;  */
  pState = &yyStateTable[pParser->yytop->stateno];
  if( pState->nEntry==0 ){
    return pState->actionDefault;
  }else if( iLookAhead!=YYNOCODE ){
    pAction = &pState->hashtbl[iLookAhead % pState->nEntry];
    while( 1 ){
      if( pAction->lookahead==iLookAhead ) return pAction->action;
      if( pAction->next==0 ) break;
      pAction = &pState->hashtbl[pAction->next-1];
    }
#ifdef YYFALLBACK
    if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
           && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
           yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
      }
#endif
      return yy_find_parser_action(pParser, iFallback);
    }
#endif
  }else if( pState->hashtbl->lookahead!=YYNOCODE ){
    return YY_NO_ACTION;
  }
  return pState->actionDefault;
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer ot the minor token to shift in */
){
  yypParser->yyidx++;
  yypParser->yytop++;
  if( yypParser->yyidx>=YYSTACKDEPTH ){
     sqliteParserARG_FETCH;
     yypParser->yyidx--;
     yypParser->yytop--;
#ifndef NDEBUG
     if( yyTraceFILE ){
       fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
     }
#endif
     while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
     /* Here code is inserted which will execute if the parser
     ** stack every overflows */
     sqliteParserARG_STORE; /* Suppress warning about unused %extra_argument var */
     return;
  }
  yypParser->yytop->stateno = yyNewState;
  yypParser->yytop->major = yyMajor;
  yypParser->yytop->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 170, 1 },
  { 139, 1 },
  { 139, 2 },
  { 154, 3 },
  { 154, 1 },
  { 140, 1 },
  { 156, 1 },
  { 156, 0 },
  { 138, 3 },
  { 206, 0 },
  { 206, 1 },
  { 206, 2 },
  { 138, 2 },
  { 138, 2 },
  { 138, 2 },
  { 138, 2 },
  { 147, 4 },
  { 205, 1 },
  { 205, 0 },
  { 148, 4 },
  { 148, 2 },
  { 144, 3 },
  { 144, 1 },
  { 142, 3 },
  { 143, 1 },
  { 164, 1 },
  { 165, 1 },
  { 165, 1 },
  { 181, 1 },
  { 181, 1 },
  { 181, 1 },
  { 212, 0 },
  { 212, 1 },
  { 212, 4 },
  { 212, 6 },
  { 213, 1 },
  { 213, 2 },
  { 199, 1 },
  { 199, 2 },
  { 199, 2 },
  { 133, 2 },
  { 133, 0 },
  { 132, 3 },
  { 132, 1 },
  { 132, 2 },
  { 132, 2 },
  { 132, 2 },
  { 132, 3 },
  { 132, 3 },
  { 132, 2 },
  { 132, 3 },
  { 132, 3 },
  { 132, 2 },
  { 137, 2 },
  { 137, 3 },
  { 137, 4 },
  { 137, 2 },
  { 137, 5 },
  { 137, 4 },
  { 137, 1 },
  { 137, 2 },
  { 192, 0 },
  { 192, 2 },
  { 191, 2 },
  { 191, 3 },
  { 191, 3 },
  { 191, 3 },
  { 190, 2 },
  { 190, 2 },
  { 190, 1 },
  { 190, 1 },
  { 151, 3 },
  { 151, 2 },
  { 169, 0 },
  { 169, 2 },
  { 169, 2 },
  { 146, 0 },
  { 146, 2 },
  { 145, 3 },
  { 145, 2 },
  { 145, 1 },
  { 204, 2 },
  { 204, 6 },
  { 204, 5 },
  { 204, 3 },
  { 204, 10 },
  { 152, 0 },
  { 152, 1 },
  { 184, 0 },
  { 184, 3 },
  { 186, 0 },
  { 186, 2 },
  { 193, 1 },
  { 193, 1 },
  { 193, 1 },
  { 193, 1 },
  { 193, 1 },
  { 138, 3 },
  { 138, 6 },
  { 138, 3 },
  { 138, 1 },
  { 196, 1 },
  { 196, 3 },
  { 180, 1 },
  { 180, 2 },
  { 180, 1 },
  { 180, 1 },
  { 185, 9 },
  { 153, 1 },
  { 153, 1 },
  { 153, 0 },
  { 194, 2 },
  { 194, 0 },
  { 195, 3 },
  { 195, 2 },
  { 195, 4 },
  { 131, 2 },
  { 131, 1 },
  { 131, 0 },
  { 161, 0 },
  { 161, 2 },
  { 203, 2 },
  { 203, 0 },
  { 197, 6 },
  { 197, 7 },
  { 150, 0 },
  { 150, 2 },
  { 175, 1 },
  { 175, 1 },
  { 175, 2 },
  { 175, 3 },
  { 175, 4 },
  { 183, 2 },
  { 183, 0 },
  { 215, 4 },
  { 215, 0 },
  { 187, 0 },
  { 187, 3 },
  { 201, 5 },
  { 201, 3 },
  { 200, 1 },
  { 202, 1 },
  { 202, 1 },
  { 202, 0 },
  { 141, 0 },
  { 141, 2 },
  { 162, 0 },
  { 162, 3 },
  { 163, 0 },
  { 163, 2 },
  { 178, 0 },
  { 178, 2 },
  { 178, 4 },
  { 178, 4 },
  { 138, 5 },
  { 217, 0 },
  { 217, 2 },
  { 138, 7 },
  { 198, 5 },
  { 198, 3 },
  { 138, 9 },
  { 138, 6 },
  { 173, 2 },
  { 173, 1 },
  { 174, 3 },
  { 174, 1 },
  { 172, 0 },
  { 172, 3 },
  { 171, 3 },
  { 171, 1 },
  { 157, 3 },
  { 157, 1 },
  { 157, 1 },
  { 157, 1 },
  { 157, 3 },
  { 157, 5 },
  { 157, 2 },
  { 157, 1 },
  { 157, 1 },
  { 157, 1 },
  { 157, 4 },
  { 157, 4 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 4 },
  { 177, 1 },
  { 177, 1 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 2 },
  { 157, 3 },
  { 157, 2 },
  { 157, 3 },
  { 157, 4 },
  { 157, 2 },
  { 157, 2 },
  { 157, 2 },
  { 157, 2 },
  { 157, 3 },
  { 157, 5 },
  { 157, 6 },
  { 157, 5 },
  { 157, 5 },
  { 157, 6 },
  { 157, 6 },
  { 157, 5 },
  { 135, 5 },
  { 135, 4 },
  { 134, 2 },
  { 134, 0 },
  { 136, 1 },
  { 136, 0 },
  { 159, 3 },
  { 159, 1 },
  { 158, 1 },
  { 158, 0 },
  { 138, 12 },
  { 214, 1 },
  { 214, 0 },
  { 168, 0 },
  { 168, 3 },
  { 167, 3 },
  { 167, 1 },
  { 166, 2 },
  { 138, 4 },
  { 138, 9 },
  { 138, 6 },
  { 138, 1 },
  { 138, 2 },
  { 138, 4 },
  { 138, 4 },
  { 138, 4 },
  { 138, 4 },
  { 138, 5 },
  { 138, 2 },
  { 188, 2 },
  { 179, 2 },
  { 182, 1 },
  { 182, 1 },
  { 189, 1 },
  { 189, 0 },
  { 138, 5 },
  { 209, 10 },
  { 211, 1 },
  { 211, 1 },
  { 211, 2 },
  { 211, 0 },
  { 210, 1 },
  { 210, 1 },
  { 210, 1 },
  { 210, 3 },
  { 160, 0 },
  { 160, 3 },
  { 160, 3 },
  { 216, 0 },
  { 216, 2 },
  { 208, 3 },
  { 208, 0 },
  { 207, 6 },
  { 207, 9 },
  { 207, 6 },
  { 207, 4 },
  { 207, 1 },
  { 157, 4 },
  { 157, 6 },
  { 157, 6 },
  { 157, 6 },
  { 138, 4 },
  { 138, 5 },
  { 149, 1 },
  { 149, 0 },
  { 138, 3 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  sqliteParserARG_FETCH;
  yymsp = yypParser->yytop;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<sizeof(yyRuleName)/sizeof(yyRuleName[0]) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 0:
        /* No destructor defined for cmdlist */
        break;
      case 1:
        /* No destructor defined for ecmd */
        break;
      case 2:
        /* No destructor defined for cmdlist */
        /* No destructor defined for ecmd */
        break;
      case 3:
        /* No destructor defined for explain */
        /* No destructor defined for cmdx */
        /* No destructor defined for SEMI */
        break;
      case 4:
        /* No destructor defined for SEMI */
        break;
      case 5:
#line 77 "parse.y"
{ sqliteExec(pParse); }
#line 5014 "parse.c"
        /* No destructor defined for cmd */
        break;
      case 6:
#line 78 "parse.y"
{ sqliteBeginParse(pParse, 1); }
#line 5020 "parse.c"
        /* No destructor defined for EXPLAIN */
        break;
      case 7:
#line 79 "parse.y"
{ sqliteBeginParse(pParse, 0); }
#line 5026 "parse.c"
        break;
      case 8:
#line 84 "parse.y"
{sqliteBeginTransaction(pParse,yymsp[0].minor.yy52);}
#line 5031 "parse.c"
        /* No destructor defined for BEGIN */
        /* No destructor defined for trans_opt */
        break;
      case 9:
        break;
      case 10:
        /* No destructor defined for TRANSACTION */
        break;
      case 11:
        /* No destructor defined for TRANSACTION */
        /* No destructor defined for nm */
        break;
      case 12:
#line 88 "parse.y"
{sqliteCommitTransaction(pParse);}
#line 5047 "parse.c"
        /* No destructor defined for COMMIT */
        /* No destructor defined for trans_opt */
        break;
      case 13:
#line 89 "parse.y"
{sqliteCommitTransaction(pParse);}
#line 5054 "parse.c"
        /* No destructor defined for END */
        /* No destructor defined for trans_opt */
        break;
      case 14:
#line 90 "parse.y"
{sqliteRollbackTransaction(pParse);}
#line 5061 "parse.c"
        /* No destructor defined for ROLLBACK */
        /* No destructor defined for trans_opt */
        break;
      case 15:
        /* No destructor defined for create_table */
        /* No destructor defined for create_table_args */
        break;
      case 16:
#line 95 "parse.y"
{
   sqliteStartTable(pParse,&yymsp[-3].minor.yy0,&yymsp[0].minor.yy210,yymsp[-2].minor.yy52,0);
}
#line 5074 "parse.c"
        /* No destructor defined for TABLE */
        break;
      case 17:
#line 99 "parse.y"
{yygotominor.yy52 = 1;}
#line 5080 "parse.c"
        /* No destructor defined for TEMP */
        break;
      case 18:
#line 100 "parse.y"
{yygotominor.yy52 = 0;}
#line 5086 "parse.c"
        break;
      case 19:
#line 101 "parse.y"
{
  sqliteEndTable(pParse,&yymsp[0].minor.yy0,0);
}
#line 5093 "parse.c"
        /* No destructor defined for LP */
        /* No destructor defined for columnlist */
        /* No destructor defined for conslist_opt */
        break;
      case 20:
#line 104 "parse.y"
{
  sqliteEndTable(pParse,0,yymsp[0].minor.yy11);
  sqliteSelectDelete(yymsp[0].minor.yy11);
}
#line 5104 "parse.c"
        /* No destructor defined for AS */
        break;
      case 21:
        /* No destructor defined for columnlist */
        /* No destructor defined for COMMA */
        /* No destructor defined for column */
        break;
      case 22:
        /* No destructor defined for column */
        break;
      case 23:
        /* No destructor defined for columnid */
        /* No destructor defined for type */
        /* No destructor defined for carglist */
        break;
      case 24:
#line 116 "parse.y"
{sqliteAddColumn(pParse,&yymsp[0].minor.yy210);}
#line 5123 "parse.c"
        break;
      case 25:
#line 122 "parse.y"
{yygotominor.yy210 = yymsp[0].minor.yy0;}
#line 5128 "parse.c"
        break;
      case 26:
#line 138 "parse.y"
{yygotominor.yy210 = yymsp[0].minor.yy0;}
#line 5133 "parse.c"
        break;
      case 27:
#line 139 "parse.y"
{yygotominor.yy210 = yymsp[0].minor.yy0;}
#line 5138 "parse.c"
        break;
      case 28:
#line 144 "parse.y"
{yygotominor.yy210 = yymsp[0].minor.yy0;}
#line 5143 "parse.c"
        break;
      case 29:
#line 145 "parse.y"
{yygotominor.yy210 = yymsp[0].minor.yy0;}
#line 5148 "parse.c"
        break;
      case 30:
#line 146 "parse.y"
{yygotominor.yy210 = yymsp[0].minor.yy0;}
#line 5153 "parse.c"
        break;
      case 31:
        break;
      case 32:
#line 149 "parse.y"
{sqliteAddColumnType(pParse,&yymsp[0].minor.yy210,&yymsp[0].minor.yy210);}
#line 5160 "parse.c"
        break;
      case 33:
#line 150 "parse.y"
{sqliteAddColumnType(pParse,&yymsp[-3].minor.yy210,&yymsp[0].minor.yy0);}
#line 5165 "parse.c"
        /* No destructor defined for LP */
        /* No destructor defined for signed */
        break;
      case 34:
#line 152 "parse.y"
{sqliteAddColumnType(pParse,&yymsp[-5].minor.yy210,&yymsp[0].minor.yy0);}
#line 5172 "parse.c"
        /* No destructor defined for LP */
        /* No destructor defined for signed */
        /* No destructor defined for COMMA */
        /* No destructor defined for signed */
        break;
      case 35:
#line 154 "parse.y"
{yygotominor.yy210 = yymsp[0].minor.yy210;}
#line 5181 "parse.c"
        break;
      case 36:
#line 155 "parse.y"
{yygotominor.yy210 = yymsp[-1].minor.yy210;}
#line 5186 "parse.c"
        /* No destructor defined for ids */
        break;
      case 37:
#line 157 "parse.y"
{ yygotominor.yy52 = atoi(yymsp[0].minor.yy0.z); }
#line 5192 "parse.c"
        break;
      case 38:
#line 158 "parse.y"
{ yygotominor.yy52 = atoi(yymsp[0].minor.yy0.z); }
#line 5197 "parse.c"
        /* No destructor defined for PLUS */
        break;
      case 39:
#line 159 "parse.y"
{ yygotominor.yy52 = -atoi(yymsp[0].minor.yy0.z); }
#line 5203 "parse.c"
        /* No destructor defined for MINUS */
        break;
      case 40:
        /* No destructor defined for carglist */
        /* No destructor defined for carg */
        break;
      case 41:
        break;
      case 42:
        /* No destructor defined for CONSTRAINT */
        /* No destructor defined for nm */
        /* No destructor defined for ccons */
        break;
      case 43:
        /* No destructor defined for ccons */
        break;
      case 44:
#line 164 "parse.y"
{sqliteAddDefaultValue(pParse,&yymsp[0].minor.yy0,0);}
#line 5223 "parse.c"
        /* No destructor defined for DEFAULT */
        break;
      case 45:
#line 165 "parse.y"
{sqliteAddDefaultValue(pParse,&yymsp[0].minor.yy0,0);}
#line 5229 "parse.c"
        /* No destructor defined for DEFAULT */
        break;
      case 46:
#line 166 "parse.y"
{sqliteAddDefaultValue(pParse,&yymsp[0].minor.yy0,0);}
#line 5235 "parse.c"
        /* No destructor defined for DEFAULT */
        break;
      case 47:
#line 167 "parse.y"
{sqliteAddDefaultValue(pParse,&yymsp[0].minor.yy0,0);}
#line 5241 "parse.c"
        /* No destructor defined for DEFAULT */
        /* No destructor defined for PLUS */
        break;
      case 48:
#line 168 "parse.y"
{sqliteAddDefaultValue(pParse,&yymsp[0].minor.yy0,1);}
#line 5248 "parse.c"
        /* No destructor defined for DEFAULT */
        /* No destructor defined for MINUS */
        break;
      case 49:
#line 169 "parse.y"
{sqliteAddDefaultValue(pParse,&yymsp[0].minor.yy0,0);}
#line 5255 "parse.c"
        /* No destructor defined for DEFAULT */
        break;
      case 50:
#line 170 "parse.y"
{sqliteAddDefaultValue(pParse,&yymsp[0].minor.yy0,0);}
#line 5261 "parse.c"
        /* No destructor defined for DEFAULT */
        /* No destructor defined for PLUS */
        break;
      case 51:
#line 171 "parse.y"
{sqliteAddDefaultValue(pParse,&yymsp[0].minor.yy0,1);}
#line 5268 "parse.c"
        /* No destructor defined for DEFAULT */
        /* No destructor defined for MINUS */
        break;
      case 52:
        /* No destructor defined for DEFAULT */
        /* No destructor defined for NULL */
        break;
      case 53:
        /* No destructor defined for NULL */
        /* No destructor defined for onconf */
        break;
      case 54:
#line 178 "parse.y"
{sqliteAddNotNull(pParse, yymsp[0].minor.yy52);}
#line 5283 "parse.c"
        /* No destructor defined for NOT */
        /* No destructor defined for NULL */
        break;
      case 55:
#line 179 "parse.y"
{sqliteAddPrimaryKey(pParse,0,yymsp[0].minor.yy52);}
#line 5290 "parse.c"
        /* No destructor defined for PRIMARY */
        /* No destructor defined for KEY */
        /* No destructor defined for sortorder */
        break;
      case 56:
#line 180 "parse.y"
{sqliteCreateIndex(pParse,0,0,0,yymsp[0].minor.yy52,0,0,0);}
#line 5298 "parse.c"
        /* No destructor defined for UNIQUE */
        break;
      case 57:
        /* No destructor defined for CHECK */
        /* No destructor defined for LP */
  yy_destructor(157,&yymsp[-2].minor);
        /* No destructor defined for RP */
        /* No destructor defined for onconf */
        break;
      case 58:
#line 183 "parse.y"
{sqliteCreateForeignKey(pParse,0,&yymsp[-2].minor.yy210,yymsp[-1].minor.yy92,yymsp[0].minor.yy52);}
#line 5311 "parse.c"
        /* No destructor defined for REFERENCES */
        break;
      case 59:
#line 184 "parse.y"
{sqliteDeferForeignKey(pParse,yymsp[0].minor.yy52);}
#line 5317 "parse.c"
        break;
      case 60:
#line 185 "parse.y"
{
   sqliteAddCollateType(pParse, sqliteCollateType(yymsp[0].minor.yy210.z, yymsp[0].minor.yy210.n));
}
#line 5324 "parse.c"
        /* No destructor defined for COLLATE */
        break;
      case 61:
#line 195 "parse.y"
{ yygotominor.yy52 = OE_Restrict * 0x010101; }
#line 5330 "parse.c"
        break;
      case 62:
#line 196 "parse.y"
{ yygotominor.yy52 = (yymsp[-1].minor.yy52 & yymsp[0].minor.yy279.mask) | yymsp[0].minor.yy279.value; }
#line 5335 "parse.c"
        break;
      case 63:
#line 198 "parse.y"
{ yygotominor.yy279.value = 0;     yygotominor.yy279.mask = 0x000000; }
#line 5340 "parse.c"
        /* No destructor defined for MATCH */
        /* No destructor defined for nm */
        break;
      case 64:
#line 199 "parse.y"
{ yygotominor.yy279.value = yymsp[0].minor.yy52;     yygotominor.yy279.mask = 0x0000ff; }
#line 5347 "parse.c"
        /* No destructor defined for ON */
        /* No destructor defined for DELETE */
        break;
      case 65:
#line 200 "parse.y"
{ yygotominor.yy279.value = yymsp[0].minor.yy52<<8;  yygotominor.yy279.mask = 0x00ff00; }
#line 5354 "parse.c"
        /* No destructor defined for ON */
        /* No destructor defined for UPDATE */
        break;
      case 66:
#line 201 "parse.y"
{ yygotominor.yy279.value = yymsp[0].minor.yy52<<16; yygotominor.yy279.mask = 0xff0000; }
#line 5361 "parse.c"
        /* No destructor defined for ON */
        /* No destructor defined for INSERT */
        break;
      case 67:
#line 203 "parse.y"
{ yygotominor.yy52 = OE_SetNull; }
#line 5368 "parse.c"
        /* No destructor defined for SET */
        /* No destructor defined for NULL */
        break;
      case 68:
#line 204 "parse.y"
{ yygotominor.yy52 = OE_SetDflt; }
#line 5375 "parse.c"
        /* No destructor defined for SET */
        /* No destructor defined for DEFAULT */
        break;
      case 69:
#line 205 "parse.y"
{ yygotominor.yy52 = OE_Cascade; }
#line 5382 "parse.c"
        /* No destructor defined for CASCADE */
        break;
      case 70:
#line 206 "parse.y"
{ yygotominor.yy52 = OE_Restrict; }
#line 5388 "parse.c"
        /* No destructor defined for RESTRICT */
        break;
      case 71:
#line 208 "parse.y"
{yygotominor.yy52 = yymsp[0].minor.yy52;}
#line 5394 "parse.c"
        /* No destructor defined for NOT */
        /* No destructor defined for DEFERRABLE */
        break;
      case 72:
#line 209 "parse.y"
{yygotominor.yy52 = yymsp[0].minor.yy52;}
#line 5401 "parse.c"
        /* No destructor defined for DEFERRABLE */
        break;
      case 73:
#line 211 "parse.y"
{yygotominor.yy52 = 0;}
#line 5407 "parse.c"
        break;
      case 74:
#line 212 "parse.y"
{yygotominor.yy52 = 1;}
#line 5412 "parse.c"
        /* No destructor defined for INITIALLY */
        /* No destructor defined for DEFERRED */
        break;
      case 75:
#line 213 "parse.y"
{yygotominor.yy52 = 0;}
#line 5419 "parse.c"
        /* No destructor defined for INITIALLY */
        /* No destructor defined for IMMEDIATE */
        break;
      case 76:
        break;
      case 77:
        /* No destructor defined for COMMA */
        /* No destructor defined for conslist */
        break;
      case 78:
        /* No destructor defined for conslist */
        /* No destructor defined for COMMA */
        /* No destructor defined for tcons */
        break;
      case 79:
        /* No destructor defined for conslist */
        /* No destructor defined for tcons */
        break;
      case 80:
        /* No destructor defined for tcons */
        break;
      case 81:
        /* No destructor defined for CONSTRAINT */
        /* No destructor defined for nm */
        break;
      case 82:
#line 225 "parse.y"
{sqliteAddPrimaryKey(pParse,yymsp[-2].minor.yy92,yymsp[0].minor.yy52);}
#line 5448 "parse.c"
        /* No destructor defined for PRIMARY */
        /* No destructor defined for KEY */
        /* No destructor defined for LP */
        /* No destructor defined for RP */
        break;
      case 83:
#line 227 "parse.y"
{sqliteCreateIndex(pParse,0,0,yymsp[-2].minor.yy92,yymsp[0].minor.yy52,0,0,0);}
#line 5457 "parse.c"
        /* No destructor defined for UNIQUE */
        /* No destructor defined for LP */
        /* No destructor defined for RP */
        break;
      case 84:
        /* No destructor defined for CHECK */
  yy_destructor(157,&yymsp[-1].minor);
        /* No destructor defined for onconf */
        break;
      case 85:
#line 230 "parse.y"
{
    sqliteCreateForeignKey(pParse, yymsp[-6].minor.yy92, &yymsp[-3].minor.yy210, yymsp[-2].minor.yy92, yymsp[-1].minor.yy52);
    sqliteDeferForeignKey(pParse, yymsp[0].minor.yy52);
}
#line 5473 "parse.c"
        /* No destructor defined for FOREIGN */
        /* No destructor defined for KEY */
        /* No destructor defined for LP */
        /* No destructor defined for RP */
        /* No destructor defined for REFERENCES */
        break;
      case 86:
#line 235 "parse.y"
{yygotominor.yy52 = 0;}
#line 5483 "parse.c"
        break;
      case 87:
#line 236 "parse.y"
{yygotominor.yy52 = yymsp[0].minor.yy52;}
#line 5488 "parse.c"
        break;
      case 88:
#line 244 "parse.y"
{ yygotominor.yy52 = OE_Default; }
#line 5493 "parse.c"
        break;
      case 89:
#line 245 "parse.y"
{ yygotominor.yy52 = yymsp[0].minor.yy52; }
#line 5498 "parse.c"
        /* No destructor defined for ON */
        /* No destructor defined for CONFLICT */
        break;
      case 90:
#line 246 "parse.y"
{ yygotominor.yy52 = OE_Default; }
#line 5505 "parse.c"
        break;
      case 91:
#line 247 "parse.y"
{ yygotominor.yy52 = yymsp[0].minor.yy52; }
#line 5510 "parse.c"
        /* No destructor defined for OR */
        break;
      case 92:
#line 248 "parse.y"
{ yygotominor.yy52 = OE_Rollback; }
#line 5516 "parse.c"
        /* No destructor defined for ROLLBACK */
        break;
      case 93:
#line 249 "parse.y"
{ yygotominor.yy52 = OE_Abort; }
#line 5522 "parse.c"
        /* No destructor defined for ABORT */
        break;
      case 94:
#line 250 "parse.y"
{ yygotominor.yy52 = OE_Fail; }
#line 5528 "parse.c"
        /* No destructor defined for FAIL */
        break;
      case 95:
#line 251 "parse.y"
{ yygotominor.yy52 = OE_Ignore; }
#line 5534 "parse.c"
        /* No destructor defined for IGNORE */
        break;
      case 96:
#line 252 "parse.y"
{ yygotominor.yy52 = OE_Replace; }
#line 5540 "parse.c"
        /* No destructor defined for REPLACE */
        break;
      case 97:
#line 256 "parse.y"
{sqliteDropTable(pParse,&yymsp[0].minor.yy210,0);}
#line 5546 "parse.c"
        /* No destructor defined for DROP */
        /* No destructor defined for TABLE */
        break;
      case 98:
#line 260 "parse.y"
{
  sqliteCreateView(pParse, &yymsp[-5].minor.yy0, &yymsp[-2].minor.yy210, yymsp[0].minor.yy11, yymsp[-4].minor.yy52);
}
#line 5555 "parse.c"
        /* No destructor defined for VIEW */
        /* No destructor defined for AS */
        break;
      case 99:
#line 263 "parse.y"
{
  sqliteDropTable(pParse, &yymsp[0].minor.yy210, 1);
}
#line 5564 "parse.c"
        /* No destructor defined for DROP */
        /* No destructor defined for VIEW */
        break;
      case 100:
#line 269 "parse.y"
{
  sqliteSelect(pParse, yymsp[0].minor.yy11, SRT_Callback, 0, 0, 0, 0);
  sqliteSelectDelete(yymsp[0].minor.yy11);
}
#line 5574 "parse.c"
        break;
      case 101:
#line 279 "parse.y"
{yygotominor.yy11 = yymsp[0].minor.yy11;}
#line 5579 "parse.c"
        break;
      case 102:
#line 280 "parse.y"
{
  if( yymsp[0].minor.yy11 ){
    yymsp[0].minor.yy11->op = yymsp[-1].minor.yy52;
    yymsp[0].minor.yy11->pPrior = yymsp[-2].minor.yy11;
  }
  yygotominor.yy11 = yymsp[0].minor.yy11;
}
#line 5590 "parse.c"
        break;
      case 103:
#line 288 "parse.y"
{yygotominor.yy52 = TK_UNION;}
#line 5595 "parse.c"
        /* No destructor defined for UNION */
        break;
      case 104:
#line 289 "parse.y"
{yygotominor.yy52 = TK_ALL;}
#line 5601 "parse.c"
        /* No destructor defined for UNION */
        /* No destructor defined for ALL */
        break;
      case 105:
#line 290 "parse.y"
{yygotominor.yy52 = TK_INTERSECT;}
#line 5608 "parse.c"
        /* No destructor defined for INTERSECT */
        break;
      case 106:
#line 291 "parse.y"
{yygotominor.yy52 = TK_EXCEPT;}
#line 5614 "parse.c"
        /* No destructor defined for EXCEPT */
        break;
      case 107:
#line 293 "parse.y"
{
  yygotominor.yy11 = sqliteSelectNew(yymsp[-6].minor.yy62,yymsp[-5].minor.yy335,yymsp[-4].minor.yy334,yymsp[-3].minor.yy62,yymsp[-2].minor.yy334,yymsp[-1].minor.yy62,yymsp[-7].minor.yy52,yymsp[0].minor.yy280.limit,yymsp[0].minor.yy280.offset);
}
#line 5622 "parse.c"
        /* No destructor defined for SELECT */
        break;
      case 108:
#line 301 "parse.y"
{yygotominor.yy52 = 1;}
#line 5628 "parse.c"
        /* No destructor defined for DISTINCT */
        break;
      case 109:
#line 302 "parse.y"
{yygotominor.yy52 = 0;}
#line 5634 "parse.c"
        /* No destructor defined for ALL */
        break;
      case 110:
#line 303 "parse.y"
{yygotominor.yy52 = 0;}
#line 5640 "parse.c"
        break;
      case 111:
#line 314 "parse.y"
{yygotominor.yy62 = yymsp[-1].minor.yy62;}
#line 5645 "parse.c"
        /* No destructor defined for COMMA */
        break;
      case 112:
#line 315 "parse.y"
{yygotominor.yy62 = 0;}
#line 5651 "parse.c"
        break;
      case 113:
#line 316 "parse.y"
{
   yygotominor.yy62 = sqliteExprListAppend(yymsp[-2].minor.yy62,yymsp[-1].minor.yy334,yymsp[0].minor.yy210.n?&yymsp[0].minor.yy210:0);
}
#line 5658 "parse.c"
        break;
      case 114:
#line 319 "parse.y"
{
  yygotominor.yy62 = sqliteExprListAppend(yymsp[-1].minor.yy62, sqliteExpr(TK_ALL, 0, 0, 0), 0);
}
#line 5665 "parse.c"
        /* No destructor defined for STAR */
        break;
      case 115:
#line 322 "parse.y"
{
  Expr *pRight = sqliteExpr(TK_ALL, 0, 0, 0);
  Expr *pLeft = sqliteExpr(TK_ID, 0, 0, &yymsp[-2].minor.yy210);
  yygotominor.yy62 = sqliteExprListAppend(yymsp[-3].minor.yy62, sqliteExpr(TK_DOT, pLeft, pRight, 0), 0);
}
#line 5675 "parse.c"
        /* No destructor defined for DOT */
        /* No destructor defined for STAR */
        break;
      case 116:
#line 332 "parse.y"
{ yygotominor.yy210 = yymsp[0].minor.yy210; }
#line 5682 "parse.c"
        /* No destructor defined for AS */
        break;
      case 117:
#line 333 "parse.y"
{ yygotominor.yy210 = yymsp[0].minor.yy210; }
#line 5688 "parse.c"
        break;
      case 118:
#line 334 "parse.y"
{ yygotominor.yy210.n = 0; }
#line 5693 "parse.c"
        break;
      case 119:
#line 346 "parse.y"
{yygotominor.yy335 = sqliteMalloc(sizeof(*yygotominor.yy335));}
#line 5698 "parse.c"
        break;
      case 120:
#line 347 "parse.y"
{yygotominor.yy335 = yymsp[0].minor.yy335;}
#line 5703 "parse.c"
        /* No destructor defined for FROM */
        break;
      case 121:
#line 352 "parse.y"
{
   yygotominor.yy335 = yymsp[-1].minor.yy335;
   if( yygotominor.yy335 && yygotominor.yy335->nSrc>0 ) yygotominor.yy335->a[yygotominor.yy335->nSrc-1].jointype = yymsp[0].minor.yy52;
}
#line 5712 "parse.c"
        break;
      case 122:
#line 356 "parse.y"
{yygotominor.yy335 = 0;}
#line 5717 "parse.c"
        break;
      case 123:
#line 357 "parse.y"
{
  yygotominor.yy335 = sqliteSrcListAppend(yymsp[-5].minor.yy335,&yymsp[-4].minor.yy210,&yymsp[-3].minor.yy210);
  if( yymsp[-2].minor.yy210.n ) sqliteSrcListAddAlias(yygotominor.yy335,&yymsp[-2].minor.yy210);
  if( yymsp[-1].minor.yy334 ){
    if( yygotominor.yy335 && yygotominor.yy335->nSrc>1 ){ yygotominor.yy335->a[yygotominor.yy335->nSrc-2].pOn = yymsp[-1].minor.yy334; }
    else { sqliteExprDelete(yymsp[-1].minor.yy334); }
  }
  if( yymsp[0].minor.yy92 ){
    if( yygotominor.yy335 && yygotominor.yy335->nSrc>1 ){ yygotominor.yy335->a[yygotominor.yy335->nSrc-2].pUsing = yymsp[0].minor.yy92; }
    else { sqliteIdListDelete(yymsp[0].minor.yy92); }
  }
}
#line 5733 "parse.c"
        break;
      case 124:
#line 369 "parse.y"
{
  yygotominor.yy335 = sqliteSrcListAppend(yymsp[-6].minor.yy335,0,0);
  yygotominor.yy335->a[yygotominor.yy335->nSrc-1].pSelect = yymsp[-4].minor.yy11;
  if( yymsp[-2].minor.yy210.n ) sqliteSrcListAddAlias(yygotominor.yy335,&yymsp[-2].minor.yy210);
  if( yymsp[-1].minor.yy334 ){
    if( yygotominor.yy335 && yygotominor.yy335->nSrc>1 ){ yygotominor.yy335->a[yygotominor.yy335->nSrc-2].pOn = yymsp[-1].minor.yy334; }
    else { sqliteExprDelete(yymsp[-1].minor.yy334); }
  }
  if( yymsp[0].minor.yy92 ){
    if( yygotominor.yy335 && yygotominor.yy335->nSrc>1 ){ yygotominor.yy335->a[yygotominor.yy335->nSrc-2].pUsing = yymsp[0].minor.yy92; }
    else { sqliteIdListDelete(yymsp[0].minor.yy92); }
  }
}
#line 5750 "parse.c"
        /* No destructor defined for LP */
        /* No destructor defined for RP */
        break;
      case 125:
#line 384 "parse.y"
{yygotominor.yy210.z=0; yygotominor.yy210.n=0;}
#line 5757 "parse.c"
        break;
      case 126:
#line 385 "parse.y"
{yygotominor.yy210 = yymsp[0].minor.yy210;}
#line 5762 "parse.c"
        /* No destructor defined for DOT */
        break;
      case 127:
#line 389 "parse.y"
{ yygotominor.yy52 = JT_INNER; }
#line 5768 "parse.c"
        /* No destructor defined for COMMA */
        break;
      case 128:
#line 390 "parse.y"
{ yygotominor.yy52 = JT_INNER; }
#line 5774 "parse.c"
        /* No destructor defined for JOIN */
        break;
      case 129:
#line 391 "parse.y"
{ yygotominor.yy52 = sqliteJoinType(pParse,&yymsp[-1].minor.yy0,0,0); }
#line 5780 "parse.c"
        /* No destructor defined for JOIN */
        break;
      case 130:
#line 392 "parse.y"
{ yygotominor.yy52 = sqliteJoinType(pParse,&yymsp[-2].minor.yy0,&yymsp[-1].minor.yy210,0); }
#line 5786 "parse.c"
        /* No destructor defined for JOIN */
        break;
      case 131:
#line 394 "parse.y"
{ yygotominor.yy52 = sqliteJoinType(pParse,&yymsp[-3].minor.yy0,&yymsp[-2].minor.yy210,&yymsp[-1].minor.yy210); }
#line 5792 "parse.c"
        /* No destructor defined for JOIN */
        break;
      case 132:
#line 398 "parse.y"
{yygotominor.yy334 = yymsp[0].minor.yy334;}
#line 5798 "parse.c"
        /* No destructor defined for ON */
        break;
      case 133:
#line 399 "parse.y"
{yygotominor.yy334 = 0;}
#line 5804 "parse.c"
        break;
      case 134:
#line 403 "parse.y"
{yygotominor.yy92 = yymsp[-1].minor.yy92;}
#line 5809 "parse.c"
        /* No destructor defined for USING */
        /* No destructor defined for LP */
        /* No destructor defined for RP */
        break;
      case 135:
#line 404 "parse.y"
{yygotominor.yy92 = 0;}
#line 5817 "parse.c"
        break;
      case 136:
#line 414 "parse.y"
{yygotominor.yy62 = 0;}
#line 5822 "parse.c"
        break;
      case 137:
#line 415 "parse.y"
{yygotominor.yy62 = yymsp[0].minor.yy62;}
#line 5827 "parse.c"
        /* No destructor defined for ORDER */
        /* No destructor defined for BY */
        break;
      case 138:
#line 416 "parse.y"
{
  yygotominor.yy62 = sqliteExprListAppend(yymsp[-4].minor.yy62,yymsp[-2].minor.yy334,0);
  if( yygotominor.yy62 ) yygotominor.yy62->a[yygotominor.yy62->nExpr-1].sortOrder = yymsp[-1].minor.yy52+yymsp[0].minor.yy52;
}
#line 5837 "parse.c"
        /* No destructor defined for COMMA */
        break;
      case 139:
#line 420 "parse.y"
{
  yygotominor.yy62 = sqliteExprListAppend(0,yymsp[-2].minor.yy334,0);
  if( yygotominor.yy62 ) yygotominor.yy62->a[0].sortOrder = yymsp[-1].minor.yy52+yymsp[0].minor.yy52;
}
#line 5846 "parse.c"
        break;
      case 140:
#line 424 "parse.y"
{yygotominor.yy334 = yymsp[0].minor.yy334;}
#line 5851 "parse.c"
        break;
      case 141:
#line 429 "parse.y"
{yygotominor.yy52 = SQLITE_SO_ASC;}
#line 5856 "parse.c"
        /* No destructor defined for ASC */
        break;
      case 142:
#line 430 "parse.y"
{yygotominor.yy52 = SQLITE_SO_DESC;}
#line 5862 "parse.c"
        /* No destructor defined for DESC */
        break;
      case 143:
#line 431 "parse.y"
{yygotominor.yy52 = SQLITE_SO_ASC;}
#line 5868 "parse.c"
        break;
      case 144:
#line 432 "parse.y"
{yygotominor.yy52 = SQLITE_SO_UNK;}
#line 5873 "parse.c"
        break;
      case 145:
#line 433 "parse.y"
{yygotominor.yy52 = sqliteCollateType(yymsp[0].minor.yy210.z, yymsp[0].minor.yy210.n);}
#line 5878 "parse.c"
        /* No destructor defined for COLLATE */
        break;
      case 146:
#line 437 "parse.y"
{yygotominor.yy62 = 0;}
#line 5884 "parse.c"
        break;
      case 147:
#line 438 "parse.y"
{yygotominor.yy62 = yymsp[0].minor.yy62;}
#line 5889 "parse.c"
        /* No destructor defined for GROUP */
        /* No destructor defined for BY */
        break;
      case 148:
#line 442 "parse.y"
{yygotominor.yy334 = 0;}
#line 5896 "parse.c"
        break;
      case 149:
#line 443 "parse.y"
{yygotominor.yy334 = yymsp[0].minor.yy334;}
#line 5901 "parse.c"
        /* No destructor defined for HAVING */
        break;
      case 150:
#line 446 "parse.y"
{yygotominor.yy280.limit = -1; yygotominor.yy280.offset = 0;}
#line 5907 "parse.c"
        break;
      case 151:
#line 447 "parse.y"
{yygotominor.yy280.limit = yymsp[0].minor.yy52; yygotominor.yy280.offset = 0;}
#line 5912 "parse.c"
        /* No destructor defined for LIMIT */
        break;
      case 152:
#line 449 "parse.y"
{yygotominor.yy280.limit = yymsp[-2].minor.yy52; yygotominor.yy280.offset = yymsp[0].minor.yy52;}
#line 5918 "parse.c"
        /* No destructor defined for LIMIT */
        /* No destructor defined for OFFSET */
        break;
      case 153:
#line 451 "parse.y"
{yygotominor.yy280.limit = yymsp[0].minor.yy52; yygotominor.yy280.offset = yymsp[-2].minor.yy52;}
#line 5925 "parse.c"
        /* No destructor defined for LIMIT */
        /* No destructor defined for COMMA */
        break;
      case 154:
#line 455 "parse.y"
{
   sqliteDeleteFrom(pParse, sqliteSrcListAppend(0,&yymsp[-2].minor.yy210,&yymsp[-1].minor.yy210), yymsp[0].minor.yy334);
}
#line 5934 "parse.c"
        /* No destructor defined for DELETE */
        /* No destructor defined for FROM */
        break;
      case 155:
#line 462 "parse.y"
{yygotominor.yy334 = 0;}
#line 5941 "parse.c"
        break;
      case 156:
#line 463 "parse.y"
{yygotominor.yy334 = yymsp[0].minor.yy334;}
#line 5946 "parse.c"
        /* No destructor defined for WHERE */
        break;
      case 157:
#line 471 "parse.y"
{sqliteUpdate(pParse,sqliteSrcListAppend(0,&yymsp[-4].minor.yy210,&yymsp[-3].minor.yy210),yymsp[-1].minor.yy62,yymsp[0].minor.yy334,yymsp[-5].minor.yy52);}
#line 5952 "parse.c"
        /* No destructor defined for UPDATE */
        /* No destructor defined for SET */
        break;
      case 158:
#line 474 "parse.y"
{yygotominor.yy62 = sqliteExprListAppend(yymsp[-4].minor.yy62,yymsp[0].minor.yy334,&yymsp[-2].minor.yy210);}
#line 5959 "parse.c"
        /* No destructor defined for COMMA */
        /* No destructor defined for EQ */
        break;
      case 159:
#line 475 "parse.y"
{yygotominor.yy62 = sqliteExprListAppend(0,yymsp[0].minor.yy334,&yymsp[-2].minor.yy210);}
#line 5966 "parse.c"
        /* No destructor defined for EQ */
        break;
      case 160:
#line 481 "parse.y"
{sqliteInsert(pParse, sqliteSrcListAppend(0,&yymsp[-6].minor.yy210,&yymsp[-5].minor.yy210), yymsp[-1].minor.yy62, 0, yymsp[-4].minor.yy92, yymsp[-8].minor.yy52);}
#line 5972 "parse.c"
        /* No destructor defined for INTO */
        /* No destructor defined for VALUES */
        /* No destructor defined for LP */
        /* No destructor defined for RP */
        break;
      case 161:
#line 483 "parse.y"
{sqliteInsert(pParse, sqliteSrcListAppend(0,&yymsp[-3].minor.yy210,&yymsp[-2].minor.yy210), 0, yymsp[0].minor.yy11, yymsp[-1].minor.yy92, yymsp[-5].minor.yy52);}
#line 5981 "parse.c"
        /* No destructor defined for INTO */
        break;
      case 162:
#line 486 "parse.y"
{yygotominor.yy52 = yymsp[0].minor.yy52;}
#line 5987 "parse.c"
        /* No destructor defined for INSERT */
        break;
      case 163:
#line 487 "parse.y"
{yygotominor.yy52 = OE_Replace;}
#line 5993 "parse.c"
        /* No destructor defined for REPLACE */
        break;
      case 164:
#line 493 "parse.y"
{yygotominor.yy62 = sqliteExprListAppend(yymsp[-2].minor.yy62,yymsp[0].minor.yy334,0);}
#line 5999 "parse.c"
        /* No destructor defined for COMMA */
        break;
      case 165:
#line 494 "parse.y"
{yygotominor.yy62 = sqliteExprListAppend(0,yymsp[0].minor.yy334,0);}
#line 6005 "parse.c"
        break;
      case 166:
#line 501 "parse.y"
{yygotominor.yy92 = 0;}
#line 6010 "parse.c"
        break;
      case 167:
#line 502 "parse.y"
{yygotominor.yy92 = yymsp[-1].minor.yy92;}
#line 6015 "parse.c"
        /* No destructor defined for LP */
        /* No destructor defined for RP */
        break;
      case 168:
#line 503 "parse.y"
{yygotominor.yy92 = sqliteIdListAppend(yymsp[-2].minor.yy92,&yymsp[0].minor.yy210);}
#line 6022 "parse.c"
        /* No destructor defined for COMMA */
        break;
      case 169:
#line 504 "parse.y"
{yygotominor.yy92 = sqliteIdListAppend(0,&yymsp[0].minor.yy210);}
#line 6028 "parse.c"
        break;
      case 170:
#line 523 "parse.y"
{yygotominor.yy334 = yymsp[-1].minor.yy334; sqliteExprSpan(yygotominor.yy334,&yymsp[-2].minor.yy0,&yymsp[0].minor.yy0); }
#line 6033 "parse.c"
        break;
      case 171:
#line 524 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_NULL, 0, 0, &yymsp[0].minor.yy0);}
#line 6038 "parse.c"
        break;
      case 172:
#line 525 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_ID, 0, 0, &yymsp[0].minor.yy0);}
#line 6043 "parse.c"
        break;
      case 173:
#line 526 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_ID, 0, 0, &yymsp[0].minor.yy0);}
#line 6048 "parse.c"
        break;
      case 174:
#line 527 "parse.y"
{
  Expr *temp1 = sqliteExpr(TK_ID, 0, 0, &yymsp[-2].minor.yy210);
  Expr *temp2 = sqliteExpr(TK_ID, 0, 0, &yymsp[0].minor.yy210);
  yygotominor.yy334 = sqliteExpr(TK_DOT, temp1, temp2, 0);
}
#line 6057 "parse.c"
        /* No destructor defined for DOT */
        break;
      case 175:
#line 532 "parse.y"
{
  Expr *temp1 = sqliteExpr(TK_ID, 0, 0, &yymsp[-4].minor.yy210);
  Expr *temp2 = sqliteExpr(TK_ID, 0, 0, &yymsp[-2].minor.yy210);
  Expr *temp3 = sqliteExpr(TK_ID, 0, 0, &yymsp[0].minor.yy210);
  Expr *temp4 = sqliteExpr(TK_DOT, temp2, temp3, 0);
  yygotominor.yy334 = sqliteExpr(TK_DOT, temp1, temp4, 0);
}
#line 6069 "parse.c"
        /* No destructor defined for DOT */
        /* No destructor defined for DOT */
        break;
      case 176:
#line 540 "parse.y"
{yygotominor.yy334 = yymsp[-1].minor.yy334; ExprSetProperty(yygotominor.yy334,EP_Oracle8Join);}
#line 6076 "parse.c"
        /* No destructor defined for ORACLE_OUTER_JOIN */
        break;
      case 177:
#line 541 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_INTEGER, 0, 0, &yymsp[0].minor.yy0);}
#line 6082 "parse.c"
        break;
      case 178:
#line 542 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_FLOAT, 0, 0, &yymsp[0].minor.yy0);}
#line 6087 "parse.c"
        break;
      case 179:
#line 543 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_STRING, 0, 0, &yymsp[0].minor.yy0);}
#line 6092 "parse.c"
        break;
      case 180:
#line 544 "parse.y"
{
  yygotominor.yy334 = sqliteExprFunction(yymsp[-1].minor.yy62, &yymsp[-3].minor.yy0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-3].minor.yy0,&yymsp[0].minor.yy0);
}
#line 6100 "parse.c"
        /* No destructor defined for LP */
        break;
      case 181:
#line 548 "parse.y"
{
  yygotominor.yy334 = sqliteExprFunction(0, &yymsp[-3].minor.yy0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-3].minor.yy0,&yymsp[0].minor.yy0);
}
#line 6109 "parse.c"
        /* No destructor defined for LP */
        /* No destructor defined for STAR */
        break;
      case 182:
#line 552 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_AND, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6116 "parse.c"
        /* No destructor defined for AND */
        break;
      case 183:
#line 553 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_OR, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6122 "parse.c"
        /* No destructor defined for OR */
        break;
      case 184:
#line 554 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_LT, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6128 "parse.c"
        /* No destructor defined for LT */
        break;
      case 185:
#line 555 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_GT, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6134 "parse.c"
        /* No destructor defined for GT */
        break;
      case 186:
#line 556 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_LE, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6140 "parse.c"
        /* No destructor defined for LE */
        break;
      case 187:
#line 557 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_GE, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6146 "parse.c"
        /* No destructor defined for GE */
        break;
      case 188:
#line 558 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_NE, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6152 "parse.c"
        /* No destructor defined for NE */
        break;
      case 189:
#line 559 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_EQ, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6158 "parse.c"
        /* No destructor defined for EQ */
        break;
      case 190:
#line 560 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_BITAND, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6164 "parse.c"
        /* No destructor defined for BITAND */
        break;
      case 191:
#line 561 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_BITOR, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6170 "parse.c"
        /* No destructor defined for BITOR */
        break;
      case 192:
#line 562 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_LSHIFT, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6176 "parse.c"
        /* No destructor defined for LSHIFT */
        break;
      case 193:
#line 563 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_RSHIFT, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6182 "parse.c"
        /* No destructor defined for RSHIFT */
        break;
      case 194:
#line 564 "parse.y"
{
  ExprList *pList = sqliteExprListAppend(0, yymsp[0].minor.yy334, 0);
  pList = sqliteExprListAppend(pList, yymsp[-2].minor.yy334, 0);
  yygotominor.yy334 = sqliteExprFunction(pList, 0);
  if( yygotominor.yy334 ) yygotominor.yy334->op = yymsp[-1].minor.yy52;
  sqliteExprSpan(yygotominor.yy334, &yymsp[-2].minor.yy334->span, &yymsp[0].minor.yy334->span);
}
#line 6194 "parse.c"
        break;
      case 195:
#line 571 "parse.y"
{
  ExprList *pList = sqliteExprListAppend(0, yymsp[0].minor.yy334, 0);
  pList = sqliteExprListAppend(pList, yymsp[-3].minor.yy334, 0);
  yygotominor.yy334 = sqliteExprFunction(pList, 0);
  if( yygotominor.yy334 ) yygotominor.yy334->op = yymsp[-1].minor.yy52;
  yygotominor.yy334 = sqliteExpr(TK_NOT, yygotominor.yy334, 0, 0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-3].minor.yy334->span,&yymsp[0].minor.yy334->span);
}
#line 6206 "parse.c"
        /* No destructor defined for NOT */
        break;
      case 196:
#line 580 "parse.y"
{yygotominor.yy52 = TK_LIKE;}
#line 6212 "parse.c"
        /* No destructor defined for LIKE */
        break;
      case 197:
#line 581 "parse.y"
{yygotominor.yy52 = TK_GLOB;}
#line 6218 "parse.c"
        /* No destructor defined for GLOB */
        break;
      case 198:
#line 582 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_PLUS, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6224 "parse.c"
        /* No destructor defined for PLUS */
        break;
      case 199:
#line 583 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_MINUS, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6230 "parse.c"
        /* No destructor defined for MINUS */
        break;
      case 200:
#line 584 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_STAR, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6236 "parse.c"
        /* No destructor defined for STAR */
        break;
      case 201:
#line 585 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_SLASH, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6242 "parse.c"
        /* No destructor defined for SLASH */
        break;
      case 202:
#line 586 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_REM, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6248 "parse.c"
        /* No destructor defined for REM */
        break;
      case 203:
#line 587 "parse.y"
{yygotominor.yy334 = sqliteExpr(TK_CONCAT, yymsp[-2].minor.yy334, yymsp[0].minor.yy334, 0);}
#line 6254 "parse.c"
        /* No destructor defined for CONCAT */
        break;
      case 204:
#line 588 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_ISNULL, yymsp[-1].minor.yy334, 0, 0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-1].minor.yy334->span,&yymsp[0].minor.yy0);
}
#line 6263 "parse.c"
        break;
      case 205:
#line 592 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_ISNULL, yymsp[-2].minor.yy334, 0, 0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-2].minor.yy334->span,&yymsp[0].minor.yy0);
}
#line 6271 "parse.c"
        /* No destructor defined for IS */
        break;
      case 206:
#line 596 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_NOTNULL, yymsp[-1].minor.yy334, 0, 0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-1].minor.yy334->span,&yymsp[0].minor.yy0);
}
#line 6280 "parse.c"
        break;
      case 207:
#line 600 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_NOTNULL, yymsp[-2].minor.yy334, 0, 0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-2].minor.yy334->span,&yymsp[0].minor.yy0);
}
#line 6288 "parse.c"
        /* No destructor defined for NOT */
        break;
      case 208:
#line 604 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_NOTNULL, yymsp[-3].minor.yy334, 0, 0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-3].minor.yy334->span,&yymsp[0].minor.yy0);
}
#line 6297 "parse.c"
        /* No destructor defined for IS */
        /* No destructor defined for NOT */
        break;
      case 209:
#line 608 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_NOT, yymsp[0].minor.yy334, 0, 0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy334->span);
}
#line 6307 "parse.c"
        break;
      case 210:
#line 612 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_BITNOT, yymsp[0].minor.yy334, 0, 0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy334->span);
}
#line 6315 "parse.c"
        break;
      case 211:
#line 616 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_UMINUS, yymsp[0].minor.yy334, 0, 0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy334->span);
}
#line 6323 "parse.c"
        break;
      case 212:
#line 620 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_UPLUS, yymsp[0].minor.yy334, 0, 0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-1].minor.yy0,&yymsp[0].minor.yy334->span);
}
#line 6331 "parse.c"
        break;
      case 213:
#line 624 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_SELECT, 0, 0, 0);
  if( yygotominor.yy334 ) yygotominor.yy334->pSelect = yymsp[-1].minor.yy11;
  sqliteExprSpan(yygotominor.yy334,&yymsp[-2].minor.yy0,&yymsp[0].minor.yy0);
}
#line 6340 "parse.c"
        break;
      case 214:
#line 629 "parse.y"
{
  ExprList *pList = sqliteExprListAppend(0, yymsp[-2].minor.yy334, 0);
  pList = sqliteExprListAppend(pList, yymsp[0].minor.yy334, 0);
  yygotominor.yy334 = sqliteExpr(TK_BETWEEN, yymsp[-4].minor.yy334, 0, 0);
  if( yygotominor.yy334 ) yygotominor.yy334->pList = pList;
  sqliteExprSpan(yygotominor.yy334,&yymsp[-4].minor.yy334->span,&yymsp[0].minor.yy334->span);
}
#line 6351 "parse.c"
        /* No destructor defined for BETWEEN */
        /* No destructor defined for AND */
        break;
      case 215:
#line 636 "parse.y"
{
  ExprList *pList = sqliteExprListAppend(0, yymsp[-2].minor.yy334, 0);
  pList = sqliteExprListAppend(pList, yymsp[0].minor.yy334, 0);
  yygotominor.yy334 = sqliteExpr(TK_BETWEEN, yymsp[-5].minor.yy334, 0, 0);
  if( yygotominor.yy334 ) yygotominor.yy334->pList = pList;
  yygotominor.yy334 = sqliteExpr(TK_NOT, yygotominor.yy334, 0, 0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-5].minor.yy334->span,&yymsp[0].minor.yy334->span);
}
#line 6365 "parse.c"
        /* No destructor defined for NOT */
        /* No destructor defined for BETWEEN */
        /* No destructor defined for AND */
        break;
      case 216:
#line 644 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_IN, yymsp[-4].minor.yy334, 0, 0);
  if( yygotominor.yy334 ) yygotominor.yy334->pList = yymsp[-1].minor.yy62;
  sqliteExprSpan(yygotominor.yy334,&yymsp[-4].minor.yy334->span,&yymsp[0].minor.yy0);
}
#line 6377 "parse.c"
        /* No destructor defined for IN */
        /* No destructor defined for LP */
        break;
      case 217:
#line 649 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_IN, yymsp[-4].minor.yy334, 0, 0);
  if( yygotominor.yy334 ) yygotominor.yy334->pSelect = yymsp[-1].minor.yy11;
  sqliteExprSpan(yygotominor.yy334,&yymsp[-4].minor.yy334->span,&yymsp[0].minor.yy0);
}
#line 6388 "parse.c"
        /* No destructor defined for IN */
        /* No destructor defined for LP */
        break;
      case 218:
#line 654 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_IN, yymsp[-5].minor.yy334, 0, 0);
  if( yygotominor.yy334 ) yygotominor.yy334->pList = yymsp[-1].minor.yy62;
  yygotominor.yy334 = sqliteExpr(TK_NOT, yygotominor.yy334, 0, 0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-5].minor.yy334->span,&yymsp[0].minor.yy0);
}
#line 6400 "parse.c"
        /* No destructor defined for NOT */
        /* No destructor defined for IN */
        /* No destructor defined for LP */
        break;
      case 219:
#line 660 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_IN, yymsp[-5].minor.yy334, 0, 0);
  if( yygotominor.yy334 ) yygotominor.yy334->pSelect = yymsp[-1].minor.yy11;
  yygotominor.yy334 = sqliteExpr(TK_NOT, yygotominor.yy334, 0, 0);
  sqliteExprSpan(yygotominor.yy334,&yymsp[-5].minor.yy334->span,&yymsp[0].minor.yy0);
}
#line 6413 "parse.c"
        /* No destructor defined for NOT */
        /* No destructor defined for IN */
        /* No destructor defined for LP */
        break;
      case 220:
#line 668 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_CASE, yymsp[-3].minor.yy334, yymsp[-1].minor.yy334, 0);
  if( yygotominor.yy334 ) yygotominor.yy334->pList = yymsp[-2].minor.yy62;
  sqliteExprSpan(yygotominor.yy334, &yymsp[-4].minor.yy0, &yymsp[0].minor.yy0);
}
#line 6425 "parse.c"
        break;
      case 221:
#line 675 "parse.y"
{
  yygotominor.yy62 = sqliteExprListAppend(yymsp[-4].minor.yy62, yymsp[-2].minor.yy334, 0);
  yygotominor.yy62 = sqliteExprListAppend(yygotominor.yy62, yymsp[0].minor.yy334, 0);
}
#line 6433 "parse.c"
        /* No destructor defined for WHEN */
        /* No destructor defined for THEN */
        break;
      case 222:
#line 679 "parse.y"
{
  yygotominor.yy62 = sqliteExprListAppend(0, yymsp[-2].minor.yy334, 0);
  yygotominor.yy62 = sqliteExprListAppend(yygotominor.yy62, yymsp[0].minor.yy334, 0);
}
#line 6443 "parse.c"
        /* No destructor defined for WHEN */
        /* No destructor defined for THEN */
        break;
      case 223:
#line 684 "parse.y"
{yygotominor.yy334 = yymsp[0].minor.yy334;}
#line 6450 "parse.c"
        /* No destructor defined for ELSE */
        break;
      case 224:
#line 685 "parse.y"
{yygotominor.yy334 = 0;}
#line 6456 "parse.c"
        break;
      case 225:
#line 687 "parse.y"
{yygotominor.yy334 = yymsp[0].minor.yy334;}
#line 6461 "parse.c"
        break;
      case 226:
#line 688 "parse.y"
{yygotominor.yy334 = 0;}
#line 6466 "parse.c"
        break;
      case 227:
#line 696 "parse.y"
{yygotominor.yy62 = sqliteExprListAppend(yymsp[-2].minor.yy62,yymsp[0].minor.yy334,0);}
#line 6471 "parse.c"
        /* No destructor defined for COMMA */
        break;
      case 228:
#line 697 "parse.y"
{yygotominor.yy62 = sqliteExprListAppend(0,yymsp[0].minor.yy334,0);}
#line 6477 "parse.c"
        break;
      case 229:
#line 698 "parse.y"
{yygotominor.yy334 = yymsp[0].minor.yy334;}
#line 6482 "parse.c"
        break;
      case 230:
#line 699 "parse.y"
{yygotominor.yy334 = 0;}
#line 6487 "parse.c"
        break;
      case 231:
#line 704 "parse.y"
{
  SrcList *pSrc = sqliteSrcListAppend(0, &yymsp[-5].minor.yy210, &yymsp[-4].minor.yy210);
  if( yymsp[-9].minor.yy52!=OE_None ) yymsp[-9].minor.yy52 = yymsp[0].minor.yy52;
  if( yymsp[-9].minor.yy52==OE_Default) yymsp[-9].minor.yy52 = OE_Abort;
  sqliteCreateIndex(pParse, &yymsp[-7].minor.yy210, pSrc, yymsp[-2].minor.yy92, yymsp[-9].minor.yy52, yymsp[-10].minor.yy52, &yymsp[-11].minor.yy0, &yymsp[-1].minor.yy0);
}
#line 6497 "parse.c"
        /* No destructor defined for INDEX */
        /* No destructor defined for ON */
        /* No destructor defined for LP */
        break;
      case 232:
#line 712 "parse.y"
{ yygotominor.yy52 = OE_Abort; }
#line 6505 "parse.c"
        /* No destructor defined for UNIQUE */
        break;
      case 233:
#line 713 "parse.y"
{ yygotominor.yy52 = OE_None; }
#line 6511 "parse.c"
        break;
      case 234:
#line 721 "parse.y"
{yygotominor.yy92 = 0;}
#line 6516 "parse.c"
        break;
      case 235:
#line 722 "parse.y"
{yygotominor.yy92 = yymsp[-1].minor.yy92;}
#line 6521 "parse.c"
        /* No destructor defined for LP */
        /* No destructor defined for RP */
        break;
      case 236:
#line 723 "parse.y"
{yygotominor.yy92 = sqliteIdListAppend(yymsp[-2].minor.yy92,&yymsp[0].minor.yy210);}
#line 6528 "parse.c"
        /* No destructor defined for COMMA */
        break;
      case 237:
#line 724 "parse.y"
{yygotominor.yy92 = sqliteIdListAppend(0,&yymsp[0].minor.yy210);}
#line 6534 "parse.c"
        break;
      case 238:
#line 725 "parse.y"
{yygotominor.yy210 = yymsp[-1].minor.yy210;}
#line 6539 "parse.c"
        /* No destructor defined for sortorder */
        break;
      case 239:
#line 730 "parse.y"
{
  sqliteDropIndex(pParse, sqliteSrcListAppend(0,&yymsp[-1].minor.yy210,&yymsp[0].minor.yy210));
}
#line 6547 "parse.c"
        /* No destructor defined for DROP */
        /* No destructor defined for INDEX */
        break;
      case 240:
#line 738 "parse.y"
{sqliteCopy(pParse,sqliteSrcListAppend(0,&yymsp[-6].minor.yy210,&yymsp[-5].minor.yy210),&yymsp[-3].minor.yy210,&yymsp[0].minor.yy0,yymsp[-7].minor.yy52);}
#line 6554 "parse.c"
        /* No destructor defined for COPY */
        /* No destructor defined for FROM */
        /* No destructor defined for USING */
        /* No destructor defined for DELIMITERS */
        break;
      case 241:
#line 740 "parse.y"
{sqliteCopy(pParse,sqliteSrcListAppend(0,&yymsp[-3].minor.yy210,&yymsp[-2].minor.yy210),&yymsp[0].minor.yy210,0,yymsp[-4].minor.yy52);}
#line 6563 "parse.c"
        /* No destructor defined for COPY */
        /* No destructor defined for FROM */
        break;
      case 242:
#line 744 "parse.y"
{sqliteVacuum(pParse,0);}
#line 6570 "parse.c"
        /* No destructor defined for VACUUM */
        break;
      case 243:
#line 745 "parse.y"
{sqliteVacuum(pParse,&yymsp[0].minor.yy210);}
#line 6576 "parse.c"
        /* No destructor defined for VACUUM */
        break;
      case 244:
#line 749 "parse.y"
{sqlitePragma(pParse,&yymsp[-2].minor.yy210,&yymsp[0].minor.yy210,0);}
#line 6582 "parse.c"
        /* No destructor defined for PRAGMA */
        /* No destructor defined for EQ */
        break;
      case 245:
#line 750 "parse.y"
{sqlitePragma(pParse,&yymsp[-2].minor.yy210,&yymsp[0].minor.yy0,0);}
#line 6589 "parse.c"
        /* No destructor defined for PRAGMA */
        /* No destructor defined for EQ */
        break;
      case 246:
#line 751 "parse.y"
{sqlitePragma(pParse,&yymsp[-2].minor.yy210,&yymsp[0].minor.yy210,0);}
#line 6596 "parse.c"
        /* No destructor defined for PRAGMA */
        /* No destructor defined for EQ */
        break;
      case 247:
#line 752 "parse.y"
{sqlitePragma(pParse,&yymsp[-2].minor.yy210,&yymsp[0].minor.yy210,1);}
#line 6603 "parse.c"
        /* No destructor defined for PRAGMA */
        /* No destructor defined for EQ */
        break;
      case 248:
#line 753 "parse.y"
{sqlitePragma(pParse,&yymsp[-3].minor.yy210,&yymsp[-1].minor.yy210,0);}
#line 6610 "parse.c"
        /* No destructor defined for PRAGMA */
        /* No destructor defined for LP */
        /* No destructor defined for RP */
        break;
      case 249:
#line 754 "parse.y"
{sqlitePragma(pParse,&yymsp[0].minor.yy210,&yymsp[0].minor.yy210,0);}
#line 6618 "parse.c"
        /* No destructor defined for PRAGMA */
        break;
      case 250:
#line 755 "parse.y"
{yygotominor.yy210 = yymsp[0].minor.yy210;}
#line 6624 "parse.c"
        /* No destructor defined for plus_opt */
        break;
      case 251:
#line 756 "parse.y"
{yygotominor.yy210 = yymsp[0].minor.yy210;}
#line 6630 "parse.c"
        /* No destructor defined for MINUS */
        break;
      case 252:
#line 757 "parse.y"
{yygotominor.yy210 = yymsp[0].minor.yy0;}
#line 6636 "parse.c"
        break;
      case 253:
#line 758 "parse.y"
{yygotominor.yy210 = yymsp[0].minor.yy0;}
#line 6641 "parse.c"
        break;
      case 254:
        /* No destructor defined for PLUS */
        break;
      case 255:
        break;
      case 256:
#line 764 "parse.y"
{
  Token all;
  all.z = yymsp[-4].minor.yy0.z;
  all.n = (yymsp[0].minor.yy0.z - yymsp[-4].minor.yy0.z) + yymsp[0].minor.yy0.n;
  sqliteFinishTrigger(pParse, yymsp[-1].minor.yy347, &all);
}
#line 6656 "parse.c"
        /* No destructor defined for trigger_decl */
        /* No destructor defined for BEGIN */
        break;
      case 257:
#line 772 "parse.y"
{
  SrcList *pTab = sqliteSrcListAppend(0, &yymsp[-3].minor.yy210, &yymsp[-2].minor.yy210);
  sqliteBeginTrigger(pParse, &yymsp[-7].minor.yy210, yymsp[-6].minor.yy52, yymsp[-5].minor.yy234.a, yymsp[-5].minor.yy234.b, pTab, yymsp[-1].minor.yy52, yymsp[0].minor.yy270, yymsp[-9].minor.yy52);
}
#line 6666 "parse.c"
        /* No destructor defined for TRIGGER */
        /* No destructor defined for ON */
        break;
      case 258:
#line 778 "parse.y"
{ yygotominor.yy52 = TK_BEFORE; }
#line 6673 "parse.c"
        /* No destructor defined for BEFORE */
        break;
      case 259:
#line 779 "parse.y"
{ yygotominor.yy52 = TK_AFTER;  }
#line 6679 "parse.c"
        /* No destructor defined for AFTER */
        break;
      case 260:
#line 780 "parse.y"
{ yygotominor.yy52 = TK_INSTEAD;}
#line 6685 "parse.c"
        /* No destructor defined for INSTEAD */
        /* No destructor defined for OF */
        break;
      case 261:
#line 781 "parse.y"
{ yygotominor.yy52 = TK_BEFORE; }
#line 6692 "parse.c"
        break;
      case 262:
#line 785 "parse.y"
{ yygotominor.yy234.a = TK_DELETE; yygotominor.yy234.b = 0; }
#line 6697 "parse.c"
        /* No destructor defined for DELETE */
        break;
      case 263:
#line 786 "parse.y"
{ yygotominor.yy234.a = TK_INSERT; yygotominor.yy234.b = 0; }
#line 6703 "parse.c"
        /* No destructor defined for INSERT */
        break;
      case 264:
#line 787 "parse.y"
{ yygotominor.yy234.a = TK_UPDATE; yygotominor.yy234.b = 0;}
#line 6709 "parse.c"
        /* No destructor defined for UPDATE */
        break;
      case 265:
#line 788 "parse.y"
{yygotominor.yy234.a = TK_UPDATE; yygotominor.yy234.b = yymsp[0].minor.yy92; }
#line 6715 "parse.c"
        /* No destructor defined for UPDATE */
        /* No destructor defined for OF */
        break;
      case 266:
#line 791 "parse.y"
{ yygotominor.yy52 = TK_ROW; }
#line 6722 "parse.c"
        break;
      case 267:
#line 792 "parse.y"
{ yygotominor.yy52 = TK_ROW; }
#line 6727 "parse.c"
        /* No destructor defined for FOR */
        /* No destructor defined for EACH */
        /* No destructor defined for ROW */
        break;
      case 268:
#line 793 "parse.y"
{ yygotominor.yy52 = TK_STATEMENT; }
#line 6735 "parse.c"
        /* No destructor defined for FOR */
        /* No destructor defined for EACH */
        /* No destructor defined for STATEMENT */
        break;
      case 269:
#line 796 "parse.y"
{ yygotominor.yy270 = 0; }
#line 6743 "parse.c"
        break;
      case 270:
#line 797 "parse.y"
{ yygotominor.yy270 = yymsp[0].minor.yy334; }
#line 6748 "parse.c"
        /* No destructor defined for WHEN */
        break;
      case 271:
#line 801 "parse.y"
{
  yymsp[-2].minor.yy347->pNext = yymsp[0].minor.yy347;
  yygotominor.yy347 = yymsp[-2].minor.yy347;
}
#line 6757 "parse.c"
        /* No destructor defined for SEMI */
        break;
      case 272:
#line 805 "parse.y"
{ yygotominor.yy347 = 0; }
#line 6763 "parse.c"
        break;
      case 273:
#line 811 "parse.y"
{ yygotominor.yy347 = sqliteTriggerUpdateStep(&yymsp[-3].minor.yy210, yymsp[-1].minor.yy62, yymsp[0].minor.yy334, yymsp[-4].minor.yy52); }
#line 6768 "parse.c"
        /* No destructor defined for UPDATE */
        /* No destructor defined for SET */
        break;
      case 274:
#line 816 "parse.y"
{yygotominor.yy347 = sqliteTriggerInsertStep(&yymsp[-5].minor.yy210, yymsp[-4].minor.yy92, yymsp[-1].minor.yy62, 0, yymsp[-7].minor.yy52);}
#line 6775 "parse.c"
        /* No destructor defined for INSERT */
        /* No destructor defined for INTO */
        /* No destructor defined for VALUES */
        /* No destructor defined for LP */
        /* No destructor defined for RP */
        break;
      case 275:
#line 819 "parse.y"
{yygotominor.yy347 = sqliteTriggerInsertStep(&yymsp[-2].minor.yy210, yymsp[-1].minor.yy92, 0, yymsp[0].minor.yy11, yymsp[-4].minor.yy52);}
#line 6785 "parse.c"
        /* No destructor defined for INSERT */
        /* No destructor defined for INTO */
        break;
      case 276:
#line 823 "parse.y"
{yygotominor.yy347 = sqliteTriggerDeleteStep(&yymsp[-1].minor.yy210, yymsp[0].minor.yy334);}
#line 6792 "parse.c"
        /* No destructor defined for DELETE */
        /* No destructor defined for FROM */
        break;
      case 277:
#line 826 "parse.y"
{yygotominor.yy347 = sqliteTriggerSelectStep(yymsp[0].minor.yy11); }
#line 6799 "parse.c"
        break;
      case 278:
#line 829 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_RAISE, 0, 0, 0); 
  yygotominor.yy334->iColumn = OE_Ignore;
  sqliteExprSpan(yygotominor.yy334, &yymsp[-3].minor.yy0, &yymsp[0].minor.yy0);
}
#line 6808 "parse.c"
        /* No destructor defined for LP */
        /* No destructor defined for IGNORE */
        break;
      case 279:
#line 834 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_RAISE, 0, 0, &yymsp[-1].minor.yy210); 
  yygotominor.yy334->iColumn = OE_Rollback;
  sqliteExprSpan(yygotominor.yy334, &yymsp[-5].minor.yy0, &yymsp[0].minor.yy0);
}
#line 6819 "parse.c"
        /* No destructor defined for LP */
        /* No destructor defined for ROLLBACK */
        /* No destructor defined for COMMA */
        break;
      case 280:
#line 839 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_RAISE, 0, 0, &yymsp[-1].minor.yy210); 
  yygotominor.yy334->iColumn = OE_Abort;
  sqliteExprSpan(yygotominor.yy334, &yymsp[-5].minor.yy0, &yymsp[0].minor.yy0);
}
#line 6831 "parse.c"
        /* No destructor defined for LP */
        /* No destructor defined for ABORT */
        /* No destructor defined for COMMA */
        break;
      case 281:
#line 844 "parse.y"
{
  yygotominor.yy334 = sqliteExpr(TK_RAISE, 0, 0, &yymsp[-1].minor.yy210); 
  yygotominor.yy334->iColumn = OE_Fail;
  sqliteExprSpan(yygotominor.yy334, &yymsp[-5].minor.yy0, &yymsp[0].minor.yy0);
}
#line 6843 "parse.c"
        /* No destructor defined for LP */
        /* No destructor defined for FAIL */
        /* No destructor defined for COMMA */
        break;
      case 282:
#line 851 "parse.y"
{
  sqliteDropTrigger(pParse,sqliteSrcListAppend(0,&yymsp[-1].minor.yy210,&yymsp[0].minor.yy210));
}
#line 6853 "parse.c"
        /* No destructor defined for DROP */
        /* No destructor defined for TRIGGER */
        break;
      case 283:
#line 856 "parse.y"
{
  sqliteAttach(pParse, &yymsp[-2].minor.yy210, &yymsp[0].minor.yy210);
}
#line 6862 "parse.c"
        /* No destructor defined for ATTACH */
        /* No destructor defined for database_kw_opt */
        /* No destructor defined for AS */
        break;
      case 284:
        /* No destructor defined for DATABASE */
        break;
      case 285:
        break;
      case 286:
#line 864 "parse.y"
{
  sqliteDetach(pParse, &yymsp[0].minor.yy210);
}
#line 6877 "parse.c"
        /* No destructor defined for DETACH */
        /* No destructor defined for database_kw_opt */
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yypParser->yytop -= yysize;
  yyact = yy_find_parser_action(yypParser,yygoto);
  if( yyact < YYNSTATE ){
    yy_shift(yypParser,yyact,yygoto,&yygotominor);
  }else if( yyact == YYNSTATE + YYNRULE + 1 ){
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  sqliteParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  sqliteParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  sqliteParserARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 23 "parse.y"

  if( pParse->zErrMsg==0 ){
    if( TOKEN.z[0] ){
      sqliteSetNString(&pParse->zErrMsg, 
          "near \"", -1, TOKEN.z, TOKEN.n, "\": syntax error", -1, 0);
    }else{
      sqliteSetString(&pParse->zErrMsg, "incomplete SQL statement", 0);
    }
  }
  pParse->nErr++;

#line 6934 "parse.c"
  sqliteParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  sqliteParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  sqliteParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "sqliteParserAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void sqliteParser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  sqliteParserTOKENTYPE yyminor       /* The value for the token */
  sqliteParserARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
    if( yymajor==0 ) return;
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yytop = &yypParser->yystack[0];
    yypParser->yytop->stateno = 0;
    yypParser->yytop->major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  sqliteParserARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_parser_action(yypParser,yymajor);
    if( yyact<YYNSTATE ){
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      if( yyendofinput && yypParser->yyidx>=0 ){
        yymajor = 0;
      }else{
        yymajor = YYNOCODE;
      }
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else if( yyact == YY_ERROR_ACTION ){
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      if( yypParser->yytop->major==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yypParser->yytop->major != YYERRORSYMBOL &&
          (yyact = yy_find_parser_action(yypParser,YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yypParser->yytop->major!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }else{
      yy_accept(yypParser);
      yymajor = YYNOCODE;
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}
