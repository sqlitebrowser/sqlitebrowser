options {
  language="Cpp";
  genHashLines=false;
}

class Sqlite3Lexer extends Lexer;
options {
  k=2;
  exportVocab=sqlite3;
  caseSensitive=false;
  caseSensitiveLiterals=false;
  charVocabulary='\u0000'..'\uFFFE';
}

tokens {
  ABORT="ABORT";
  ACTION="ACTION";
  AUTOINCREMENT="AUTOINCREMENT";
  AS="AS";
  ASC="ASC";
  AND="AND";
  OR="OR";
  BETWEEN="BETWEEN";
  CASCADE="CASCADE";
  CASE_T="CASE";
  CAST="CAST";
  CHECK="CHECK";
  CREATE="CREATE";
  COLLATE="COLLATE";
  CONFLICT="CONFLICT";
  CONSTRAINT="CONSTRAINT";
  CURRENT_TIME="CURRENT_TIME";
  CURRENT_DATE="CURRENT_DATE";
  CURRENT_TIMESTAMP="CURRENT_TIMESTAMP";
  DEFAULT="DEFAULT";
  DEFERRABLE="DEFERRABLE";
  DEFERRED="DEFERRED";
  DELETE="DELETE";
  DESC="DESC";
  ELSE_T="ELSE";
  END="END";
  ESCAPE="ESCAPE";
  FAIL="FAIL";
  FILTER="FILTER";
  FOLLOWING="FOLLOWING";
  FOREIGN="FOREIGN";
  GLOB="GLOB";
  KEY="KEY";
  LIKE="LIKE";
  TABLE="TABLE";
  IF_T="IF";
  IGNORE="IGNORE";
  IN="IN";
  INDEX="INDEX";
  INITIALLY="INITIALLY";
  INSERT="INSERT";
  IMMEDIATE="IMMEDIATE";
  IS="IS";
  NO="NO";
  NOT="NOT";
  NULL_T="NULL";
  MATCH="MATCH";
  EXISTS="EXISTS";
  ON="ON";
  OVER="OVER";
  PARTITION="PARTITION";
  PRECEDING="PRECEDING";
  PRIMARY="PRIMARY";
  RAISE="RAISE";
  RANGE="RANGE";
  REFERENCES="REFERENCES";
  REGEXP="REGEXP";
  REPLACE="REPLACE";
  RESTRICT="RESTRICT";
  ROLLBACK="ROLLBACK";
  ROWID="ROWID";
  ROWS="ROWS";
  SET="SET";
  TEMPORARY="TEMPORARY";
  TEMP="TEMP";
  THEN="THEN";
  UNBOUNDED="UNBOUNDED";
  UNIQUE="UNIQUE";
  UPDATE="UPDATE";
  USING="USING";
  VIRTUAL="VIRTUAL";
  WHEN="WHEN";
  WHERE="WHERE";
  WITHOUT="WITHOUT";

//ast

  TYPE_NAME;
  COLUMNDEF;
  COLUMNCONSTRAINT;
  TABLECONSTRAINT;
  CREATETABLE;
  CREATEINDEX;
  INDEXEDCOLUMN;
  KEYWORDASTABLENAME;
  KEYWORDASCOLUMNNAME;
}

protected
DIGIT : '0'..'9' ;

protected DOT:;

ID
  :
  // 0080 - 02B8 load of good looking unicode chars
  // there might be more allowed characters
  ('a'..'z'|'_') ('a'..'z'|'0'..'9'|'_'|'\u0080'..'\u02B8')*
  ;

QUOTEDID
	: '`' ( ~('`') | ('`' '`') )* '`'
	| '[' ( ~(']') )*  ']'
	;

QUOTEDLITERAL
  : '"' ( ~'"' | ('"' '"' ) )* '"'
  ;

NUMERIC
    : ( (DIGIT)+ ( '.' (DIGIT)* )?
      | '.' (DIGIT)+)
      ( 'e' (PLUS|MINUS)? (DIGIT)+ )?
	;

protected
NL :
    ( '\r'
    | '\n' {newline();}
    );

COMMENT :
    ( '-''-' (~('\n'|'\r'))* NL {newline();} // single line comment
    | '/''*' ( options{greedy=false;} : NL {newline();} | ~('\n'|'\r') )* '*''/' // multi-line comment
    ) { $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP); };

WS :
    ( ' '
    | '\t'
    | '\f'
    | NL
    ) { $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP); };

STRINGLITERAL
  :
//  '"' ( ESC_SEQ | ~('\\'|'"') )* '"'
  '\'' ( ~'\'' | ('\'' '\'') )* '\''
  ;

//protected
//ESC_SEQ
//    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
//    ;

LPAREN	:	'(';
RPAREN	:	')';
COMMA	:	',';
SEMI 	:	';';
PLUS    :   '+';
MINUS   :   '-';
STAR    :   '*';
TILDE : '~';
AMPERSAND: '&';
BITOR: '|';
OROP: BITOR BITOR;
EQUAL: '=';
EQUAL2: EQUAL EQUAL;
GREATER: '>';
GREATEREQUAL: GREATER EQUAL;
LOWER: '<';
LOWEREQUAL: LOWER EQUAL;
UNEQUAL: '!' EQUAL;
UNEQUAL2: LOWER GREATER;
BITWISELEFT: LOWER LOWER;
BITWISERIGHT: GREATER GREATER;

// parser

class Sqlite3Parser extends Parser;
options {
  k=2;
  buildAST = true;
  defaultErrorHandler=false;
}

id : ID | QUOTEDID | QUOTEDLITERAL | STRINGLITERAL ;

databasename
  :
  id
  ;

tablename
  :
  id
  ;

nonkeyword_columnname
  :
  id
  ;

identifier
  :
  ((databasename)? DOT)? tablename
  ;

collationname
  :
  ID
  ;

signednumber
  : (PLUS | MINUS)? NUMERIC
//	{#signednumber = #([SIGNEDNUMBER, "SIGNEDNUMBER"], #signednumber);}
  ;

// parser part
statementlist
  :
  (statement)?
  (SEMI statement)*
  ;

statement
  :
  createtable
  | createindex
  ;

create_statements
  :
  createtable
  | createindex
  ;

keywordastablename
  :
  ( ABORT
  | ACTION
  | ASC
  | CASCADE
  | CAST
  | CONFLICT
  | CURRENT_TIME
  | CURRENT_DATE
  | CURRENT_TIMESTAMP
  | DEFERRED
  | DESC
  | ELSE_T
  | END
  | FAIL
  | FILTER
  | FOLLOWING
  | GLOB
  | KEY
  | LIKE
  | IGNORE
  | INITIALLY
  | IMMEDIATE
  | MATCH
  | NO
  | OVER
  | PARTITION
  | PRECEDING
  | RAISE
  | RANGE
  | REGEXP
  | REPLACE
  | RESTRICT
  | ROLLBACK
  | ROWID
  | ROWS
  | TEMPORARY
  | TEMP
  | UNBOUNDED
  | VIRTUAL
  | WITHOUT
  )
  {#keywordastablename = #([KEYWORDASTABLENAME, "KEYWORDASTABLENAME"], #keywordastablename);}
  ;

createtable
  :
  (CREATE (TEMP|TEMPORARY)? TABLE (IF_T NOT EXISTS)? (tablename | keywordastablename)
    ( LPAREN columndef (COMMA columndef)* ((COMMA)? tableconstraint)* RPAREN (WITHOUT ROWID)?
    | AS selectstmt
    )
    {#createtable = #([CREATETABLE, "CREATETABLE"], #createtable);}
  )
  |(CREATE VIRTUAL TABLE (IF_T NOT EXISTS)? (tablename | keywordastablename)
    USING name (LPAREN (expr (COMMA expr)*)? RPAREN)?		// TODO: Not sure about using "expr" here
   )
  ;

createindex
  :
  CREATE (UNIQUE)? INDEX (IF_T NOT EXISTS)? (tablename | keywordastablename) ON (tablename | keywordastablename)
    ( LPAREN indexedcolumn (COMMA indexedcolumn)* RPAREN (WHERE expr)? )
    {#createindex = #([CREATEINDEX, "CREATEINDEX"], #createindex);}
  ;

keywordascolumnname
  :
  ( ABORT
  | ACTION
  | ASC
  | CASCADE
  | CAST
  | CONFLICT
  | CURRENT_TIME
  | CURRENT_DATE
  | CURRENT_TIMESTAMP
  | DEFERRED
  | DESC
  | END
  | FAIL
  | FILTER
  | FOLLOWING
  | GLOB
  | KEY
  | LIKE
  | IF_T
  | IGNORE
  | INITIALLY
  | IMMEDIATE
  | MATCH
  | NO
  | OVER
  | PARTITION
  | PRECEDING
  | RAISE
  | RANGE
  | REGEXP
  | REPLACE
  | RESTRICT
  | ROLLBACK
  | ROWID
  | ROWS
  | TEMPORARY
  | TEMP
  | UNBOUNDED
  | VIRTUAL
  | WITHOUT
  )
  {#keywordascolumnname = #([KEYWORDASCOLUMNNAME, "KEYWORDASCOLUMNNAME"], #keywordascolumnname);}
  ;

columnname
  :
  ( nonkeyword_columnname
  | keywordascolumnname
  )
  {}
  ;

columndef
  :
  columnname (type_name)? (columnconstraint)*
  {#columndef = #([COLUMNDEF, "COLUMNDEF"], #columndef);}
  ;

name : ID | QUOTEDID | QUOTEDLITERAL | STRINGLITERAL;

type_name
  :
  (name | keywordastablename)+
  (LPAREN signednumber (COMMA signednumber)? RPAREN)?
  {#type_name = #([TYPE_NAME, "TYPE_NAME"], #type_name);}
  ;

columnconstraint
  :
  (CONSTRAINT name)?
  ( PRIMARY KEY (ASC|DESC)? (conflictclause)? (AUTOINCREMENT)?
  | (NOT)? NULL_T (conflictclause)?
  | UNIQUE (conflictclause)?
  | CHECK LPAREN expr RPAREN
  | DEFAULT (QUOTEDLITERAL | LPAREN expr RPAREN | literalvalue | ID | keywordastablename | signednumber)
  | COLLATE collationname
  | foreignkeyclause)
  {#columnconstraint = #([COLUMNCONSTRAINT, "COLUMNCONSTRAINT"], #columnconstraint);}
  ;

tableconstraint
  :
  (CONSTRAINT name)?
  ( PRIMARY KEY LPAREN indexedcolumn (COMMA indexedcolumn)* RPAREN (conflictclause)?
  | UNIQUE LPAREN indexedcolumn (COMMA indexedcolumn)* RPAREN (conflictclause)?
  | CHECK LPAREN expr RPAREN
  | FOREIGN KEY LPAREN columnname (COMMA columnname)* RPAREN foreignkeyclause
  )
  {#tableconstraint = #([TABLECONSTRAINT, "TABLECONSTRAINT"], #tableconstraint);}
  ;

indexedcolumn
  :
  expr (COLLATE collationname)? (ASC|DESC)? (AUTOINCREMENT)?
  {#indexedcolumn = #([INDEXEDCOLUMN, "INDEXEDCOLUMN"], #indexedcolumn);}
  ;

conflictclause
  :
  ON CONFLICT
  ( ROLLBACK
  | ABORT
  | FAIL
  | IGNORE
  | REPLACE
  )
  ;

foreignkeyclause
  :
  REFERENCES tablename (LPAREN columnname (COMMA columnname)* RPAREN)?
  ( ON (DELETE | UPDATE | INSERT)
    ( SET (NULL_T | DEFAULT)
    | CASCADE
    | RESTRICT
    | NO ACTION
    )
  | MATCH name
  )*
  ( (NOT DEFERRABLE) => NOT DEFERRABLE (INITIALLY (DEFERRED | IMMEDIATE))?
  | DEFERRABLE (INITIALLY (DEFERRED | IMMEDIATE) ) )?
  ;

selectstmt
  : SELECT
  ;

functionname
  : id ;

expr
  :
    ( LPAREN (((subexpr (COMMA subexpr)+ RPAREN binaryoperator LPAREN subexpr (COMMA subexpr)+) | (expr)) RPAREN) ((AND | OR) expr)* )
  | ( subexpr ((binaryoperator | AND | OR) subexpr )* )
  ;

subexpr
  :
  ( MINUS | PLUS | TILDE | NOT)?
  ( literalvalue
//  | bindparameter TODO
  | ((databasename DOT)? tablename DOT)? columnname
  | functionname LPAREN (expr (COMMA expr)* )? RPAREN //TODO
  | castexpr
  | EXISTS LPAREN (expr | selectstmt) RPAREN
  | caseexpr
  | raisefunction
//  | windowfunc
  )
  (suffixexpr)?
  ;

castexpr
  :
  CAST LPAREN expr AS type_name RPAREN
  ;

caseexpr
  :
  CASE_T (expr)? (WHEN expr THEN expr)+ (ELSE_T expr)? END
  ;

/*
windowfunc
  :
  functionname LPAREN (STAR | (expr (COMMA expr)*))? RPAREN (FILTER LPAREN WHERE expr RPAREN)? OVER
  (
    (LPAREN windowdefn RPAREN)
    | id
  )
  ;

windowdefn
  :
  (PARTITION BY expr (COMMA expr)*)?
  (ORDER BY orderingterm (COMMA orderingterm)*)?
  (framespec)?
  ;

orderingterm
  :
  expr (COLLATE collationname)? (ASC | DESC)?
  ;

framespec
  :
  (RANGE | ROWS)
  (BETWEEN
    (
    (UNBOUNDED PRECEDING)
    | (expr PRECEDING)
    | (CURRENT ROW)
    | (expr FOLLOWING)
    )
    AND
    (
    (expr PRECEDING)
    | (CURRENT ROW)
    | (expr FOLLOWING)
    | (UNBOUNDED FOLLOWING)
    )
  )
  |
  (
    (UNBOUNDED PRECEDING)
    | (expr PRECEDING)
    | (CURRENT ROW)
    | (expr FOLLOWING)
  )
  ;
*/

like_operator
  :
  LIKE
  | GLOB
  | REGEXP
  | MATCH
  ;

between_subexpr
  :
  subexpr (AND subexpr)+
  ;

suffixexpr
  :
  COLLATE collationname
  | (NOT)?
	( BETWEEN subexpr ((binaryoperator | OR) subexpr )* AND expr
	| IN ( LPAREN (selectstmt | expr (COMMA expr)* )? RPAREN | tablename)
    | like_operator subexpr (ESCAPE subexpr)?
    )
  ;

literalvalue
  :
    NUMERIC
  | STRINGLITERAL
//  | blob-literal
  | NULL_T
  | CURRENT_TIME
  | CURRENT_DATE
  | CURRENT_TIMESTAMP
  ;

raisefunction
  : RAISE LPAREN ( IGNORE | (ROLLBACK | ABORT | FAIL) COMMA STRINGLITERAL ) RPAREN ;

binaryoperator
  :
    OROP
  | STAR | SLASH | PERCENT
  | PLUS | MINUS
  | BITWISELEFT | BITWISERIGHT | AMPERSAND | BITOR
  | LOWER | LOWEREQUAL | GREATER | GREATEREQUAL
  | EQUAL | EQUAL2 | UNEQUAL | UNEQUAL2
  | IS | like_operator
  ;
