/* Hash score: 151 */
static int keywordCode(const char *z, int n){
  static const char zText[510] =
    "ABORTABLEFTEMPORARYADDATABASELECTHENDEFAULTRANSACTIONATURALTER"
    "AISEACHECKEYAFTEREFERENCESCAPELSEXCEPTRIGGEREINDEXCLUSIVEXISTS"
    "TATEMENTANDEFERRABLEXPLAINITIALLYATTACHAVINGLOBEFOREIGNORENAME"
    "AUTOINCREMENTBEGINNEREPLACEBETWEENOTNULLIKEBYCASCADEFERREDELETE"
    "CASECOLLATECOLUMNCOMMITCONFLICTCONSTRAINTERSECTCREATECROSSCURRENT_DATE"
    "CURRENT_TIMESTAMPRAGMATCHDESCDETACHDISTINCTDROPRIMARYFAILIMIT"
    "FROMFULLGROUPDATEIMMEDIATEINSERTINSTEADINTOFFSETISNULLJOINORDER"
    "ESTRICTOUTERIGHTROLLBACKROWHENUNIONUNIQUEUSINGVACUUMVALUESVIEW"
    "HERE";
  static const unsigned char aHash[127] = {
      89,  79, 101,  88,   0,   4,   0,   0, 108,   0,  75,   0,   0,
      92,  44,   0,  90,   0, 100, 103,  94,   0,   0,  10,   0,   0,
     107,   0, 104,  98,   0,  11,  47,   0,  41,   0,   0,  63,  69,
       0,  62,  19,   0,   0,  33,  81,   0, 102,  72,   0,   0,  30,
       0,  60,  34,   0,   8,   0, 109,  38,  12,   0,  76,  40,  25,
      64,   0,   0,  37,  80,  52,  36,  49,  20,  86,   0,  31,   0,
      73,  26,   0,  70,   0,   0,   0,   0,  46,  65,  22,  85,  35,
      67,  84,   0,   1,   0,   9,  51,  57,  18,   0, 106,  74,  96,
      53,   6,  83,   0,   0,  48,  91,   0,  99,   0,  68,   0,   0,
      15,   0, 110,  50,  55,   0,   2,  54,   0, 105,
  };
  static const unsigned char aNext[110] = {
       0,   0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,
       0,   0,   0,   0,   0,   0,   0,   0,  17,   0,   0,   0,   0,
       0,   0,   0,   5,  13,   0,   7,   0,   0,   0,   0,   0,   0,
       0,   0,   0,   0,   0,   0,  43,   0,   0,   0,   0,   0,   0,
       0,  16,   0,  23,  45,   0,   0,   0,   0,  28,  58,   0,   0,
       0,   0,   0,   0,   0,   0,  71,  42,   0,   0,  24,  59,  21,
       0,  78,   0,  66,   0,   0,  82,  29,   0,   0,   0,   0,   0,
       0,   0,  39,  93,  95,   0,   0,  97,  14,  27,  77,   0,  56,
      87,   0,  32,   0,  61,   0,
  };
  static const unsigned char aLen[110] = {
       5,   5,   4,   4,   9,   2,   3,   8,   2,   6,   4,   3,   7,
      11,   2,   7,   5,   5,   4,   5,   3,   5,  10,   6,   4,   6,
       7,   7,   5,   9,   6,   9,   3,  10,   7,   9,   3,   6,   6,
       4,   6,   3,   7,   6,   6,  13,   2,   2,   5,   5,   7,   7,
       3,   7,   4,   4,   2,   7,   3,   8,   6,   4,   7,   6,   6,
       8,  10,   9,   6,   5,  12,  12,  17,   6,   5,   4,   6,   8,
       2,   4,   7,   4,   5,   4,   4,   5,   6,   9,   6,   7,   4,
       2,   6,   3,   6,   4,   5,   8,   5,   5,   8,   3,   4,   5,
       6,   5,   6,   6,   4,   5,
  };
  static const unsigned short int aOffset[110] = {
       0,   4,   7,  10,  10,  14,  19,  21,  26,  27,  32,  34,  36,
      42,  51,  52,  57,  61,  65,  67,  71,  74,  78,  86,  91,  94,
      99, 105, 107, 110, 118, 123, 132, 134, 143, 148, 153, 157, 162,
     167, 170, 172, 172, 176, 180, 186, 188, 190, 199, 202, 206, 213,
     219, 219, 222, 225, 229, 231, 232, 236, 243, 249, 253, 260, 266,
     272, 280, 287, 296, 302, 307, 319, 319, 335, 339, 344, 348, 354,
     355, 362, 365, 372, 375, 380, 384, 388, 391, 397, 406, 412, 419,
     422, 422, 425, 428, 434, 438, 442, 450, 454, 459, 467, 469, 473,
     478, 484, 489, 495, 501, 504,
  };
  static const unsigned char aCode[110] = {
    TK_ABORT,      TK_TABLE,      TK_JOIN_KW,    TK_TEMP,       TK_TEMP,       
    TK_OR,         TK_ADD,        TK_DATABASE,   TK_AS,         TK_SELECT,     
    TK_THEN,       TK_END,        TK_DEFAULT,    TK_TRANSACTION,TK_ON,         
    TK_JOIN_KW,    TK_ALTER,      TK_RAISE,      TK_EACH,       TK_CHECK,      
    TK_KEY,        TK_AFTER,      TK_REFERENCES, TK_ESCAPE,     TK_ELSE,       
    TK_EXCEPT,     TK_TRIGGER,    TK_REINDEX,    TK_INDEX,      TK_EXCLUSIVE,  
    TK_EXISTS,     TK_STATEMENT,  TK_AND,        TK_DEFERRABLE, TK_EXPLAIN,    
    TK_INITIALLY,  TK_ALL,        TK_ATTACH,     TK_HAVING,     TK_GLOB,       
    TK_BEFORE,     TK_FOR,        TK_FOREIGN,    TK_IGNORE,     TK_RENAME,     
    TK_AUTOINCR,   TK_TO,         TK_IN,         TK_BEGIN,      TK_JOIN_KW,    
    TK_REPLACE,    TK_BETWEEN,    TK_NOT,        TK_NOTNULL,    TK_NULL,       
    TK_LIKE,       TK_BY,         TK_CASCADE,    TK_ASC,        TK_DEFERRED,   
    TK_DELETE,     TK_CASE,       TK_COLLATE,    TK_COLUMNKW,   TK_COMMIT,     
    TK_CONFLICT,   TK_CONSTRAINT, TK_INTERSECT,  TK_CREATE,     TK_JOIN_KW,    
    TK_CDATE,      TK_CTIME,      TK_CTIMESTAMP, TK_PRAGMA,     TK_MATCH,      
    TK_DESC,       TK_DETACH,     TK_DISTINCT,   TK_IS,         TK_DROP,       
    TK_PRIMARY,    TK_FAIL,       TK_LIMIT,      TK_FROM,       TK_JOIN_KW,    
    TK_GROUP,      TK_UPDATE,     TK_IMMEDIATE,  TK_INSERT,     TK_INSTEAD,    
    TK_INTO,       TK_OF,         TK_OFFSET,     TK_SET,        TK_ISNULL,     
    TK_JOIN,       TK_ORDER,      TK_RESTRICT,   TK_JOIN_KW,    TK_JOIN_KW,    
    TK_ROLLBACK,   TK_ROW,        TK_WHEN,       TK_UNION,      TK_UNIQUE,     
    TK_USING,      TK_VACUUM,     TK_VALUES,     TK_VIEW,       TK_WHERE,      
  };
  int h, i;
  if( n<2 ) return TK_ID;
  h = ((sqlite3UpperToLower[((unsigned char*)z)[0]]*4) ^
      (sqlite3UpperToLower[((unsigned char*)z)[n-1]]*3) ^
      n) % 127;
  for(i=((int)aHash[h])-1; i>=0; i=((int)aNext[i])-1){
    if( aLen[i]==n && sqlite3StrNICmp(&zText[aOffset[i]],z,n)==0 ){
      return aCode[i];
    }
  }
  return TK_ID;
}
int sqlite3KeywordCode(const char *z, int n){
  return keywordCode(z, n);
}
