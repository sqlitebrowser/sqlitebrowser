%skeleton "lalr1.cc"
%require "3.4.1"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%output "sqlite3_parser.cpp"
%define api.location.file "sqlite3_location.h"
%define api.namespace { sqlb::parser }

%code requires {
	#include <string>
	#include "../sqlitetypes.h"
	#include "../ObjectIdentifier.h"
	namespace sqlb { namespace parser { class ParserDriver; } }
	typedef void* yyscan_t;
}

// The parsing context
%param { yyscan_t yyscanner }
%param { ParserDriver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
	#include "ParserDriver.h"
	
	static std::string unquote_text(std::string str, char quote_char)
	{
		if(str.front() != quote_char || str.back() != quote_char)
			return str;

		str = str.substr(1, str.size()-2);

		std::string quote(2, quote_char);

		size_t pos = 0;
		while((pos = str.find(quote, pos)) != std::string::npos)
		{
			str.erase(pos, 1);
			pos += 1;               // Don't remove the other quote char too
		}
		return str;
	}
}

%define api.token.prefix {TOK_}
%token
	EOF		0  "end of file"
	LPAREN		"("
	RPAREN		")"
	DOT		"."
	COMMA		","
	SEMI		";"
	PLUS		"+"
	MINUS		"-"
	STAR		"*"
	SLASH		"/"
	TILDE		"~"
	AMPERSAND	"&"
	PERCENT		"%"
	BITOR		"|"
	OROP		"||"
	EQUAL		"="
	EQUAL2		"=="
	GREATER		">"
	GREATEREQUAL	">="
	LOWER		"<"
	LOWEREQUAL	"<="
	UNEQUAL		"!="
	UNEQUAL2	"<>"
	BITWISELEFT	"<<"
	BITWISERIGHT	">>"
;

%token <std::string> ABORT "ABORT"
%token <std::string> ACTION "ACTION"
%token <std::string> AND "AND"
%token <std::string> AND_BETWEEN "AND BETWEEN"
%token <std::string> AS "AS"
%token <std::string> ASC "ASC"
%token <std::string> AUTOINCREMENT "AUTOINCREMENT"
%token <std::string> BETWEEN "BETWEEN"
%token <std::string> CASCADE "CASCADE"
%token <std::string> CASE "CASE"
%token <std::string> CAST "CAST"
%token <std::string> CHECK "CHECK"
%token <std::string> COLLATE "COLLATE"
%token <std::string> CONFLICT "CONFLICT"
%token <std::string> CONSTRAINT "CONSTRAINT"
%token <std::string> CREATE "CREATE"
%token <std::string> CURRENT_DATE "CURRENT_DATE"
%token <std::string> CURRENT_TIME "CURRENT_TIME"
%token <std::string> CURRENT_TIMESTAMP "CURRENT_TIMESTAMP"
%token <std::string> DEFAULT "DEFAULT"
%token <std::string> DEFERRABLE "DEFERRABLE"
%token <std::string> DEFERRED "DEFERRED"
%token <std::string> DELETE "DELETE"
%token <std::string> DESC "DESC"
%token <std::string> DISTINCT "DISTINCT"
%token <std::string> ELSE "ELSE"
%token <std::string> END "END"
%token <std::string> ESCAPE "ESCAPE"
%token <std::string> EXISTS "EXISTS"
%token <std::string> FAIL "FAIL"
%token <std::string> FALSE "FALSE"
%token <std::string> FILTER "FILTER"
%token <std::string> FOLLOWING "FOLLOWING"
%token <std::string> FOREIGN "FOREIGN"
%token <std::string> GLOB "GLOB"
%token <std::string> IF "IF"
%token <std::string> IGNORE "IGNORE"
%token <std::string> IMMEDIATE "IMMEDIATE"
%token <std::string> IN "IN"
%token <std::string> INDEX "INDEX"
%token <std::string> INITIALLY "INITIALLY"
%token <std::string> INSERT "INSERT"
%token <std::string> IS "IS"
%token <std::string> ISNULL "ISNULL"
%token <std::string> KEY "KEY"
%token <std::string> LIKE "LIKE"
%token <std::string> MATCH "MATCH"
%token <std::string> NO "NO"
%token <std::string> NOT "NOT"
%token <std::string> NOTNULL "NOTNULL"
%token <std::string> NULL "NULL"
%token <std::string> ON "ON"
%token <std::string> OR "OR"
%token <std::string> OVER "OVER"
%token <std::string> PARTITION "PARTITION"
%token <std::string> PRECEDING "PRECEDING"
%token <std::string> PRIMARY "PRIMARY"
%token <std::string> RAISE "RAISE"
%token <std::string> RANGE "RANGE"
%token <std::string> REFERENCES "REFERENCES"
%token <std::string> REGEXP "REGEXP"
%token <std::string> REPLACE "REPLACE"
%token <std::string> RESTRICT "RESTRICT"
%token <std::string> ROLLBACK "ROLLBACK"
%token <std::string> ROWID "ROWID"
%token <std::string> ROWS "ROWS"
%token <std::string> SELECT "SELECT"
%token <std::string> SET "SET"
%token <std::string> TABLE "TABLE"
%token <std::string> TEMP "TEMP"
%token <std::string> TEMPORARY "TEMPORARY"
%token <std::string> THEN "THEN"
%token <std::string> TRUE "TRUE"
%token <std::string> UNBOUNDED "UNBOUNDED"
%token <std::string> UNIQUE "UNIQUE"
%token <std::string> UPDATE "UPDATE"
%token <std::string> USING "USING"
%token <std::string> VIRTUAL "VIRTUAL"
%token <std::string> WHEN "WHEN"
%token <std::string> WHERE "WHERE"
%token <std::string> WITHOUT "WITHOUT"

%token <std::string> IDENTIFIER "identifier"
%token <std::string> NUMERIC "numeric"
%token <std::string> STRINGLITERAL "string literal"
%token <std::string> QUOTEDLITERAL "quoted literal"
%token <std::string> BLOBLITERAL "blob literal"
%token <std::string> BINDPARAMETER "bind parameter"

%type <std::string> literalvalue
%type <std::string> signednumber
%type <std::string> signednumber_or_numeric
%type <std::string> id
%type <std::string> allowed_keywords_as_identifier
%type <std::string> tableid
%type <std::string> columnid
%type <std::string> typename_namelist
%type <std::string> type_name
%type <std::string> unary_expr
%type <std::string> binary_expr
%type <std::string> like_expr
%type <std::string> exprlist_expr
%type <std::string> function_expr
%type <std::string> isnull_expr
%type <std::string> between_expr
%type <std::string> in_expr
%type <std::string> whenthenlist_expr
%type <std::string> case_expr
%type <std::string> raise_expr
%type <std::string> expr
%type <bool> optional_if_not_exists
%type <bool> optional_unique
%type <std::string> optional_sort_order
%type <std::string> optional_where
%type <std::string> tableid_with_uninteresting_schema
%type <std::string> select_stmt
%type <sqlb::IndexedColumn> indexed_column
%type <sqlb::IndexedColumnVector> indexed_column_list
%type <sqlb::IndexPtr> createindex_stmt

%%

%left OR;
%left AND;
%right NOT;
%left IS MATCH LIKE BETWEEN IN UNEQUAL UNEQUAL2 EQUAL EQUAL2 GLOB REGEXP ISNULL NOTNULL;
%left GREATER LOWEREQUAL LOWER GREATEREQUAL;
%right ESCAPE;
%left AMPERSAND BITOR BITWISELEFT BITWISERIGHT;
%left PLUS MINUS;
%left STAR SLASH PERCENT;
%left OROP;
%left COLLATE;
%right TILDE;
%nonassoc ON;

/*
 * Statements
 */

%start sql;

sql:
	statement
	| statement ";"
	;

statement:
	createindex_stmt { drv.result = $1; }
	;

/*
 * Expressions
 */

literalvalue:
	NUMERIC
	| STRINGLITERAL
	| BLOBLITERAL
	| NULL
	| TRUE
	| FALSE
	| CURRENT_TIME
	| CURRENT_DATE
	| CURRENT_TIMESTAMP
	;

id:
	IDENTIFIER
	| QUOTEDLITERAL
	//| STRINGLITERAL
	;

allowed_keywords_as_identifier:
	ABORT
	| ACTION
	| ASC
	| CASCADE
	| CAST
	| CONFLICT
	| DEFERRED
	| DESC
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
	;

tableid:
	allowed_keywords_as_identifier
	| CURRENT_TIME
	| CURRENT_DATE
	| CURRENT_TIMESTAMP
	| id
	| STRINGLITERAL				{ $$ = unquote_text($1, '\''); }
	;

columnid:
	allowed_keywords_as_identifier
	| CURRENT_TIME
	| CURRENT_DATE
	| CURRENT_TIMESTAMP
	| IF
	| id
	| STRINGLITERAL				{ $$ = unquote_text($1, '\''); }
	;

signednumber:
	"+" NUMERIC				{ $$ = "+" + $2; }	// No NUMERIC without "+" or "-" here because that is just a literalvalue
	| "-" NUMERIC				{ $$ = "-" + $2; }
	;

signednumber_or_numeric:
	signednumber
	| NUMERIC
	;

typename_namelist:
	tableid					{ $$ = $1; }
	| typename_namelist tableid		{ $$ = $1 + " " + $2; }
	;

type_name:
	typename_namelist								{ $$ = $1; }
	| typename_namelist "(" signednumber_or_numeric ")"				{ $$ = $1 + "(" + $3 + ")"; }
	| typename_namelist "(" signednumber_or_numeric "," signednumber_or_numeric ")"	{ $$ = $1 + "(" + $3 + ", " + $5 + ")"; }
	;

unary_expr:
	"-" expr %prec TILDE	{ $$ = "-" + $2; }
	| "+" expr %prec TILDE	{ $$ = "+" + $2; }
	| "~" expr		{ $$ = "~" + $2; }
	| NOT expr		{ $$ = "NOT " + $2; }
	;

binary_expr:
	expr "||" expr		{ $$ = $1 + " || " + $3; }
	| expr "*" expr		{ $$ = $1 + " * " + $3; }
	| expr "/" expr		{ $$ = $1 + " / " + $3; }
	| expr "%" expr		{ $$ = $1 + " % " + $3; }
	| expr "+" expr		{ $$ = $1 + " + " + $3; }
	| expr "-" expr		{ $$ = $1 + " - " + $3; }
	| expr "<<" expr	{ $$ = $1 + " << " + $3; }
	| expr ">>" expr	{ $$ = $1 + " >> " + $3; }
	| expr "&" expr		{ $$ = $1 + " & " + $3; }
	| expr "|" expr		{ $$ = $1 + " | " + $3; }
	| expr "<" expr		{ $$ = $1 + " < " + $3; }
	| expr "<=" expr	{ $$ = $1 + " <= " + $3; }
	| expr ">" expr		{ $$ = $1 + " > " + $3; }
	| expr ">=" expr	{ $$ = $1 + " >= " + $3; }
	| expr "=" expr		{ $$ = $1 + " = " + $3; }
	| expr "==" expr	{ $$ = $1 + " == " + $3; }
	| expr "!=" expr	{ $$ = $1 + " != " + $3; }
	| expr "<>" expr	{ $$ = $1 + " <> " + $3; }
	| expr IS expr		{ $$ = $1 + " IS " + $3; }
	| expr AND expr		{ $$ = $1 + " AND " + $3; }
	| expr OR expr		{ $$ = $1 + " OR " + $3; }
	;

like_expr:
	expr LIKE expr					{ $$ = $1 + " LIKE " + $3; }
	| expr GLOB expr				{ $$ = $1 + " GLOB " + $3; }
	| expr MATCH expr				{ $$ = $1 + " MATCH " + $3; }
	| expr REGEXP expr				{ $$ = $1 + " REGEXP " + $3; }
	| expr NOT LIKE expr				{ $$ = $1 + " NOT LIKE " + $4; }
	| expr NOT GLOB expr				{ $$ = $1 + " NOT GLOB " + $4; }
	| expr NOT MATCH expr				{ $$ = $1 + " NOT MATCH " + $4; }
	| expr NOT REGEXP expr				{ $$ = $1 + " NOT REGEXP " + $4; }
	| expr LIKE expr ESCAPE expr %prec LIKE		{ $$ = $1 + " LIKE " + $3 + " ESCAPE " + $5; }
	| expr GLOB expr ESCAPE expr %prec GLOB		{ $$ = $1 + " GLOB " + $3 + " ESCAPE " + $5; }
	| expr MATCH expr ESCAPE expr %prec MATCH	{ $$ = $1 + " MATCH " + $3 + " ESCAPE " + $5; }
	| expr REGEXP expr ESCAPE expr %prec REGEXP	{ $$ = $1 + " REGEXP " + $3 + " ESCAPE " + $5; }
	| expr NOT LIKE expr ESCAPE expr %prec LIKE	{ $$ = $1 + " NOT LIKE " + $3 + " ESCAPE " + $6; }
	| expr NOT GLOB expr ESCAPE expr %prec GLOB	{ $$ = $1 + " NOT GLOB " + $3 + " ESCAPE " + $6; }
	| expr NOT MATCH expr ESCAPE expr %prec MATCH	{ $$ = $1 + " NOT MATCH " + $3 + " ESCAPE " + $6; }
	| expr NOT REGEXP expr ESCAPE expr %prec REGEXP	{ $$ = $1 + " NOT REGEXP " + $3 + " ESCAPE " + $6; }
	;

exprlist_expr:
	expr					{ $$ = $1; }
	| exprlist_expr "," expr		{ $$ = $1 + ", " + $3; }
	;

function_expr:
	id "(" exprlist_expr ")"		{ $$ = $1 + "(" + $3 + ")"; }
	| id "(" DISTINCT exprlist_expr ")"	{ $$ = $1 + "(DISTINCT " + $4 + ")"; }
	| id "(" ")"				{ $$ = $1 + "()"; }
	| id "(" "*" ")"			{ $$ = $1 + "(*)"; }
	;

isnull_expr:
	expr ISNULL				{ $$ = $1 + " ISNULL"; }
	| expr NOTNULL				{ $$ = $1 + " NOTNULL"; }
	| expr NOT NULL %prec NOTNULL		{ $$ = $1 + " NOT NULL"; }
	;

between_expr:
	expr BETWEEN expr AND_BETWEEN expr %prec BETWEEN	{ $$ = $1 + " BETWEEN " + $3 + " AND " + $5; }
	| expr NOT BETWEEN expr AND_BETWEEN expr %prec BETWEEN	{ $$ = $1 + " NOT BETWEEN " + $4 + " AND " + $6; }
	;

in_expr:
	expr IN "(" ")"					{ $$ = $1 + " IN ()"; }
	| expr IN "(" select_stmt ")"			{ $$ = $1 + " IN (" + $4 + ")"; }
	| expr IN "(" exprlist_expr ")"			{ $$ = $1 + " IN (" + $4 + ")"; }
	| expr IN id "." tableid			{ $$ = $1 + " IN " + sqlb::escapeIdentifier($3) + "." + sqlb::escapeIdentifier($5); }
	| expr IN tableid				{ $$ = $1 + " IN " + sqlb::escapeIdentifier($3); }
	| expr IN id "." id "(" ")"			{ $$ = $1 + " IN " + sqlb::escapeIdentifier($3) + "." + $5 + "()"; }
	| expr IN id "." id "(" exprlist_expr ")"	{ $$ = $1 + " IN " + sqlb::escapeIdentifier($3) + "." + $5 + "(" + $7 + ")"; }
	| expr IN id "(" exprlist_expr ")"		{ $$ = $1 + " IN " + $3 + "(" + $5 + ")"; }
	| expr NOT IN "(" ")"				{ $$ = $1 + " NOT IN ()"; }
	| expr NOT IN "(" select_stmt ")"		{ $$ = $1 + " NOT IN (" + $5 + ")"; }
	| expr NOT IN "(" exprlist_expr ")"		{ $$ = $1 + " NOT IN (" + $5 + ")"; }
	| expr NOT IN id "." tableid			{ $$ = $1 + " NOT IN " + sqlb::escapeIdentifier($4) + "." + sqlb::escapeIdentifier($6); }
	| expr NOT IN tableid				{ $$ = $1 + " NOT IN " + sqlb::escapeIdentifier($4); }
	| expr NOT IN id "." id "(" ")"			{ $$ = $1 + " NOT IN " + sqlb::escapeIdentifier($4) + "." + $6 + "()"; }
	| expr NOT IN id "." id "(" exprlist_expr ")"	{ $$ = $1 + " NOT IN " + sqlb::escapeIdentifier($4) + "." + $6 + "(" + $8 + ")"; }
	| expr NOT IN id "(" exprlist_expr ")"		{ $$ = $1 + " NOT IN " + $4 + "(" + $6 + ")"; }
	;

whenthenlist_expr:
	WHEN expr THEN expr				{ $$ = "WHEN " + $2 + " THEN " + $4; }
	| whenthenlist_expr WHEN expr THEN expr		{ $$ = $1 + " WHEN" + $3 + " THEN " + $5; }
	;

case_expr:
	CASE expr whenthenlist_expr ELSE expr END	{ $$ = "CASE " + $2 + " " + $3 + " ELSE " + $5 + " END"; }
	| CASE expr whenthenlist_expr END		{ $$ = "CASE " + $2 + " " + $3 + " END"; }
	| CASE whenthenlist_expr ELSE expr END		{ $$ = "CASE " + $2 + " ELSE " + $4 + " END"; }
	| CASE whenthenlist_expr END			{ $$ = "CASE " + $2 + " END"; }
	;

raise_expr:
	RAISE "(" IGNORE ")"				{ $$ = "RAISE(IGNORE)"; }
	| RAISE "(" ROLLBACK "," STRINGLITERAL ")"	{ $$ = "RAISE(ROLLBACK, " + $5 + ")"; }
	| RAISE "(" ABORT "," STRINGLITERAL ")"		{ $$ = "RAISE(ABORT, " + $5 + ")"; }
	| RAISE "(" FAIL "," STRINGLITERAL ")"		{ $$ = "RAISE(FAIL, " + $5 + ")"; }
	;

expr:
	literalvalue
	| allowed_keywords_as_identifier	{ $$ = sqlb::escapeIdentifier($1); }
	| BINDPARAMETER
	| id "." id "." id			{ $$ = sqlb::escapeIdentifier($1) + "." + sqlb::escapeIdentifier($3) + "." + sqlb::escapeIdentifier($5); }
	| id "." id				{ $$ = sqlb::escapeIdentifier($1) + "." + sqlb::escapeIdentifier($3); }
	| id					{ $$ = sqlb::escapeIdentifier($1); }
	| unary_expr
	| binary_expr
	| function_expr
	| "(" exprlist_expr ")"			{ $$ = "(" + $2 + ")"; }
	| CAST "(" expr AS type_name ")"	{ $$ = "CAST(" + $3 + " AS " + $5 + ")"; }
	| expr COLLATE id			{ $$ = $1 + " COLLATE " + $3; }
	| like_expr
	| isnull_expr
	| between_expr
	| in_expr
	| case_expr
	| raise_expr
	// TODO Window functions
	;

/*
 * SELECT
 */

select_stmt:
	SELECT					{ $$ = "SELECT"; }		// TODO
	;

/*
 * Helper rules for CREATE statements
 */

optional_if_not_exists:
	%empty					{ $$ = false; }
	| IF NOT EXISTS				{ $$ = true; }
	;

optional_sort_order:
	%empty					{ $$ = ""; }
	| ASC					{ $$ = "ASC"; }
	| DESC					{ $$ = "DESC"; }
	;

/*
 * CREATE INDEX
 */

optional_unique:
	%empty					{ $$ = false; }
	| UNIQUE				{ $$ = true; }
	;

optional_where:
	%empty					{ $$ = ""; }
	| WHERE expr				{ $$ = $2; }
	;

tableid_with_uninteresting_schema:
	id "." tableid				{ $$ = $3; }
	| TEMP "." tableid			{ $$ = $3; }
	| tableid				{ $$ = $1; }
	;

indexed_column:
	expr optional_sort_order		{
							// If the expression is only one column name and nothing else, treat it as a column name; otherwise as an expression.
							char quote = getIdentifierQuoteChar();
							if((quote == '[' && std::count($1.begin(), $1.end(), quote) == 1 && $1.front() == '[' && $1.back() == ']') ||
								(quote != '[' && std::count($1.begin(), $1.end(), quote) == 2 && $1.front() == quote && $1.back() == quote))
							{
								$$ = sqlb::IndexedColumn(unquote_text($1, quote), false, $2);
							} else {
								$$ = sqlb::IndexedColumn($1, true, $2);
							}
						}
	;

indexed_column_list:
	indexed_column					{ $$ = sqlb::IndexedColumnVector(1, $1); }
	| indexed_column_list "," indexed_column	{ $$ = $1; $$.push_back($3); }
	;

createindex_stmt:
	CREATE optional_unique INDEX optional_if_not_exists tableid_with_uninteresting_schema ON tableid "(" indexed_column_list ")" optional_where		{
													$$ = sqlb::IndexPtr(new sqlb::Index($5));
													$$->setTable($7);
													$$->setUnique($2);
													$$->setWhereExpr($11);
													$$->fields = $9;
													$$->setFullyParsed(true);
												}
	;

%%

void sqlb::parser::parser::error(const location_type& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}
