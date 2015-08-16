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
  FOREIGN="FOREIGN";
  GLOB="GLOB";
  KEY="KEY";
  LIKE="LIKE";
  TABLE="TABLE";
  IF_T="IF";
  IGNORE="IGNORE";
  IN="IN";
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
  PRIMARY="PRIMARY";
  RAISE="RAISE";
  REFERENCES="REFERENCES";
  REGEXP="REGEXP";
  REPLACE="REPLACE";
  RESTRICT="RESTRICT";
  ROLLBACK="ROLLBACK";
  ROWID="ROWID";
  SET="SET";
  TEMPORARY="TEMPORARY";
  TEMP="TEMP";
  THEN="THEN";
  UNIQUE="UNIQUE";
  UPDATE="UPDATE";
  WHEN="WHEN";
  WITHOUT="WITHOUT";

//ast

  TYPE_NAME;
  COLUMNDEF;
  COLUMNCONSTRAINT;
  TABLECONSTRAINT;
  CREATETABLE;
  KEYWORDASTABLENAME;
  KEYWORDASCOLUMNNAME;
}

protected
DIGIT : '0'..'9' ;

protected DOT:;

ID
  :
  // 00C0 - 02B8 load of good looking unicode chars
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
      | '.' { _ttype=DOT; } (DIGIT)+)
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
  ;

create_statements
  :
  createtable
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
  | END
  | FAIL
  | GLOB
  | KEY
  | LIKE
  | IGNORE
  | INITIALLY
  | IMMEDIATE
  | MATCH
  | RAISE
  | REGEXP
  | REPLACE
  | RESTRICT
  | ROLLBACK
  | TEMPORARY
  | TEMP
  )
  {#keywordastablename = #([KEYWORDASTABLENAME, "KEYWORDASTABLENAME"], #keywordastablename);}
  ;

createtable
  :
  CREATE (TEMP|TEMPORARY)? TABLE (IF_T NOT EXISTS)? (tablename | keywordastablename)
  ( LPAREN columndef (COMMA columndef)* ((COMMA)? tableconstraint)* RPAREN (WITHOUT ROWID)?
  | AS selectstmt
  )
  {#createtable = #([CREATETABLE, "CREATETABLE"], #createtable);}
  ;

keywordascolumnname
  :
  ( ABORT
  | ACTION
  | AUTOINCREMENT
  | AS
  | ASC
  | AND
  | OR
  | CASCADE
  | CASE_T
  | CAST
  | CREATE
  | COLLATE
  | CONFLICT
  | CURRENT_TIME
  | CURRENT_DATE
  | CURRENT_TIMESTAMP
  | DEFAULT
  | DEFERRABLE
  | DEFERRED
  | DELETE
  | DESC
  | ELSE_T
  | END
  | ESCAPE
  | FAIL
  | GLOB
  | KEY
  | LIKE
  | TABLE
  | IF_T
  | IGNORE
  | INITIALLY
  | IMMEDIATE
  | IS
  | NULL_T
  | MATCH
  | EXISTS
  | ON
  | RAISE
  | REFERENCES
  | REGEXP
  | REPLACE
  | RESTRICT
  | ROLLBACK
  | ROWID
  | SET
  | TEMPORARY
  | TEMP
  | THEN
  | UPDATE
  | WHEN
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
  (name)+
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
  | DEFAULT (signednumber | QUOTEDLITERAL | STRINGLITERAL | LPAREN expr RPAREN | literalvalue | ID)
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
  columnname (COLLATE collationname)? (ASC|DESC)? (AUTOINCREMENT)?
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
  subexpr ((binaryoperator | AND | OR) subexpr )*
  ;

subexpr
  :
  ( MINUS | PLUS | TILDE | NOT)?
  ( literalvalue
//  | bindparameter TODO
  | ((databasename DOT)? tablename)? columnname
  | functionname LPAREN (expr (COMMA expr)* )? RPAREN //TODO
  | castexpr
  | (EXISTS)? LPAREN (expr | selectstmt) RPAREN
  | caseexpr
  | raisefunction
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
