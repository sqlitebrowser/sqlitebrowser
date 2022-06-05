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
	#include <tuple>
	#include "../sqlitetypes.h"
	#include "../ObjectIdentifier.h"
	namespace sqlb { namespace parser { class ParserDriver; } }
	typedef void* yyscan_t;

	struct Constraints
	{
		std::multimap<sqlb::IndexedColumnVector, std::shared_ptr<sqlb::UniqueConstraint>> index;
		std::multimap<sqlb::StringVector, std::shared_ptr<sqlb::ForeignKeyClause>> fk;
		std::vector<std::shared_ptr<sqlb::CheckConstraint>> check;
	};

	// Colum definitions are a tuple of two elements: the Field object and a set of table constraints
	using ColumndefData = std::tuple<sqlb::Field, Constraints>;
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
		if(quote_char != '[')
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
		} else {
			if(str.front() != '[' || str.back() != ']')
				return str;

			return str.substr(1, str.size()-2);
		}
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
%token <std::string> ALWAYS "ALWAYS"
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
%token <std::string> GENERATED "GENERATED"
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
%token <std::string> RETURNING "RETURNING"
%token <std::string> ROLLBACK "ROLLBACK"
%token <std::string> ROWID "ROWID"
%token <std::string> ROWS "ROWS"
%token <std::string> SELECT "SELECT"
%token <std::string> SET "SET"
%token <std::string> STORED "STORED"
%token <std::string> STRICT "STRICT"
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
%type <bool> optional_temporary
%type <std::string> optional_sort_order
%type <std::string> optional_where
%type <std::string> optional_constraintname
%type <std::string> optional_conflictclause
%type <std::string> tableid_with_uninteresting_schema
%type <std::string> optional_exprlist_with_paren
%type <std::string> select_stmt

%type <sqlb::IndexedColumn> indexed_column
%type <sqlb::IndexedColumnVector> indexed_column_list
%type <sqlb::IndexPtr> createindex_stmt
%type <sqlb::TablePtr> createvirtualtable_stmt

%type <std::string> optional_typename
%type <std::string> optional_storage_identifier
%type <bool> optional_always_generated
%type <std::vector<sqlb::ConstraintPtr>> columnconstraint_list
%type <sqlb::ConstraintPtr> columnconstraint
%type <ColumndefData> columndef
%type <std::vector<ColumndefData>> columndef_list
%type <sqlb::StringVector> columnid_list
%type <sqlb::StringVector> optional_columnid_with_paren_list
%type <std::string> fk_clause_part
%type <std::string> fk_clause_part_list
%type <std::string> optional_fk_clause
%type <Constraints> tableconstraint
%type <Constraints> tableconstraint_list
%type <Constraints> optional_tableconstraint_list
%type <std::bitset<sqlb::Table::NumOptions>> tableoption
%type <std::bitset<sqlb::Table::NumOptions>> tableoptions_list
%type <std::bitset<sqlb::Table::NumOptions>> optional_tableoptions_list
%type <sqlb::TablePtr> createtable_stmt

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
	createindex_stmt		{ drv.result = $1; }
	| createvirtualtable_stmt	{ drv.result = $1; }
	| createtable_stmt		{ drv.result = $1; }
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
	| ALWAYS
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
	| GENERATED
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
	| RETURNING
	| ROLLBACK
	| ROWID
	| ROWS
	| STORED
	| STRICT
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
	%empty					{ }
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
	%empty					{ }
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
							} else if(std::count($1.begin(), $1.end(), '\'') == 2 && $1.front() == '\'' && $1.back() == '\'') {
								// Also remove single quotes when this actually is a string literal but looks like a columnid
								$$ = sqlb::IndexedColumn(unquote_text($1, '\''), false, $2);
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
													$$ = std::make_shared<sqlb::Index>($5);
													$$->setTable($7);
													$$->setUnique($2);
													$$->setWhereExpr($11);
													$$->fields = $9;
													$$->setFullyParsed(true);
												}
	;

/*
 * CREATE VIRTUAL TABLE
 */

optional_exprlist_with_paren:
	%empty					{ }
	| "(" ")"				{ }
	| "(" exprlist_expr ")"			{ $$ = $2; }
	;

createvirtualtable_stmt:
	CREATE VIRTUAL TABLE optional_if_not_exists tableid_with_uninteresting_schema USING id optional_exprlist_with_paren	{
													$$ = std::make_shared<sqlb::Table>($5);
													$$->setVirtualUsing($7);
													$$->setFullyParsed(false);
												}
	;

/*
 * CREATE TABLE
 */

optional_temporary:
	%empty						{ $$ = false; }
	| TEMP						{ $$ = true; }
	| TEMPORARY					{ $$ = true; }
	;

tableoption:
	WITHOUT ROWID					{ $$.set(sqlb::Table::WithoutRowid, true); }
	| STRICT					{ $$.set(sqlb::Table::Strict, true); }
	;

tableoptions_list:
	tableoption					{ $$ = $1; }
	| tableoptions_list "," tableoption		{ $$ = $1 | $3; }
	| tableoptions_list tableoption			{ $$ = $1 | $2; }
	;

optional_tableoptions_list:
	%empty						{ }
	| tableoptions_list				{ $$ = $1; }
	;

optional_conflictclause:
	%empty						{ }
	| ON CONFLICT ROLLBACK				{ $$ = $3; }
	| ON CONFLICT ABORT				{ $$ = $3; }
	| ON CONFLICT FAIL				{ $$ = $3; }
	| ON CONFLICT IGNORE				{ $$ = $3; }
	| ON CONFLICT REPLACE				{ $$ = $3; }
	;

optional_typename:
	%empty					{ }
	| type_name				{ $$ = $1; }
	;

optional_storage_identifier:
	%empty						{ $$ = "VIRTUAL"; }
	| STORED					{ $$ = "STORED"; }
	| VIRTUAL					{ $$ = "VIRTUAL"; }
	;

optional_always_generated:
	%empty						{ $$ = false; }
	| GENERATED ALWAYS				{ $$ = true; }
	;

columnconstraint:
	optional_constraintname PRIMARY KEY optional_sort_order optional_conflictclause	{
												auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>();
												pk->setName($1);
												pk->setConflictAction($5);
												$$ = pk;
											}
	| optional_constraintname PRIMARY KEY optional_sort_order optional_conflictclause AUTOINCREMENT	{
												auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>();
												pk->setName($1);
												pk->setConflictAction($5);
												pk->setAutoIncrement(true);
												$$ = pk;
											}
	| optional_constraintname NOT NULL optional_conflictclause			{
												auto nn = std::make_shared<sqlb::NotNullConstraint>();
												nn->setName($1);
												nn->setConflictAction($4);
												$$ = nn;
											}
	| optional_constraintname NULL 							{
											}
	| optional_constraintname UNIQUE optional_conflictclause			{
												auto u = std::make_shared<sqlb::UniqueConstraint>();
												u->setName($1);
												u->setConflictAction($3);
												$$ = u;
											}
	| optional_constraintname CHECK "(" expr ")"					{
												$$ = std::make_shared<sqlb::CheckConstraint>($4);
												$$->setName($1);
											}
	| optional_constraintname DEFAULT signednumber					{
												$$ = std::make_shared<sqlb::DefaultConstraint>($3);
												$$->setName($1);
											}
	| optional_constraintname DEFAULT literalvalue					{
												$$ = std::make_shared<sqlb::DefaultConstraint>($3);
												$$->setName($1);
											}
	| optional_constraintname DEFAULT id						{
												$$ = std::make_shared<sqlb::DefaultConstraint>($3);
												$$->setName($1);
											}
	| optional_constraintname DEFAULT allowed_keywords_as_identifier		{	// We must allow the same keywords as unquoted default values as in the columnid context.
												// But we do not use columnid here in order to avoid reduce/reduce conflicts.
												$$ = std::make_shared<sqlb::DefaultConstraint>($3);
												$$->setName($1);
											}
	| optional_constraintname DEFAULT IF						{	// Same as above.
												$$ = std::make_shared<sqlb::DefaultConstraint>($3);
												$$->setName($1);
											}
	| optional_constraintname DEFAULT "(" expr ")"					{
												$$ = std::make_shared<sqlb::DefaultConstraint>("(" + $4 + ")");
												$$->setName($1);
											}
	| optional_constraintname COLLATE id						{
												$$ = std::make_shared<sqlb::CollateConstraint>($3);
												$$->setName($1);
											}
	| optional_constraintname REFERENCES tableid optional_columnid_with_paren_list optional_fk_clause	{	// TODO Solve shift/reduce conflict. It is not super important though as shifting seems to be right here.
												auto fk = std::make_shared<sqlb::ForeignKeyClause>();
												fk->setName($1);
												fk->setTable($3);
												fk->setColumns($4);
												fk->setConstraint($5);
												$$ = fk;
											}
	| optional_constraintname optional_always_generated AS "(" expr ")" optional_storage_identifier	{		// TODO Solve shift/reduce conflict.
												$$ = std::make_shared<sqlb::GeneratedColumnConstraint>($5, $7);
												$$->setName($1);
											}
	;

columnconstraint_list:
	columnconstraint				{ $$ = { $1 }; }
	| columnconstraint_list columnconstraint	{ $$ = $1; $$.push_back($2); }
	;

columndef:
	columnid optional_typename columnconstraint_list	{
								sqlb::Field f($1, $2);
								Constraints table_constraints;
								for(const auto& c : $3)
								{
									if(!c)
										continue;

									switch(c->type())
									{
									// Primary key and foreign key constraints are converted to table constraints
									// because we cannot store them as column constraints at the moment.
									case sqlb::Constraint::PrimaryKeyConstraintType:
										table_constraints.index.insert(std::make_pair(sqlb::IndexedColumnVector{sqlb::IndexedColumn($1, false)}, std::dynamic_pointer_cast<sqlb::UniqueConstraint>(c)));
										break;
									case sqlb::Constraint::ForeignKeyConstraintType:
										table_constraints.fk.insert(std::make_pair(sqlb::StringVector{$1}, std::dynamic_pointer_cast<sqlb::ForeignKeyClause>(c)));
										break;
									case sqlb::Constraint::NotNullConstraintType:
										f.setNotNull(std::dynamic_pointer_cast<sqlb::NotNullConstraint>(c));
										break;
									case sqlb::Constraint::UniqueConstraintType:
										f.setUnique(std::dynamic_pointer_cast<sqlb::UniqueConstraint>(c));
										break;
									case sqlb::Constraint::CheckConstraintType:
										f.setCheck(std::dynamic_pointer_cast<sqlb::CheckConstraint>(c));
										break;
									case sqlb::Constraint::DefaultConstraintType:
										f.setDefaultValue(std::dynamic_pointer_cast<sqlb::DefaultConstraint>(c));
										break;
									case sqlb::Constraint::CollateConstraintType:
										f.setCollation(std::dynamic_pointer_cast<sqlb::CollateConstraint>(c));
										break;
									case sqlb::Constraint::GeneratedColumnConstraintType:
									{
										f.setGenerated(std::dynamic_pointer_cast<sqlb::GeneratedColumnConstraint>(c));

										// This is a hack which removes any "GENERATED ALWAYS" from the end of the type name.
										// As of now these are shifted to the type instead of reducing the type when seeing the GENERATED identifier.
										// TODO Remove this once the grammar is conflict free
										const std::string generated_always = "GENERATED ALWAYS";
										if(f.type().size() >= generated_always.size() && f.type().compare(f.type().size() - generated_always.size(), generated_always.size(), generated_always) == 0)
										{
											std::string type = f.type().substr(0, f.type().size()-generated_always.size());
											if(type.back() == ' ')
												type.pop_back();
											f.setType(type);
										}
										break;
									}
									}
								}

								$$ = std::make_tuple(f, table_constraints);
							}
	| columnid optional_typename			{ $$ = std::make_tuple(sqlb::Field($1, $2), Constraints{}); }
	;

columndef_list:
	columndef					{ $$ = {$1}; }
	| columndef_list "," columndef			{ $$ = $1; $$.push_back($3); }
	;

optional_constraintname:
	%empty						{ }
	| CONSTRAINT id					{ $$ = $2; }
	| CONSTRAINT STRINGLITERAL			{ $$ = $2; }
	;

columnid_list:
	columnid					{ $$ = sqlb::StringVector(1, $1); }
	| columnid_list "," columnid			{ $$ = $1; $$.push_back($3); }
	;

optional_columnid_with_paren_list:
	%empty						{ }
	| "(" columnid_list ")"				{ $$ = $2; }
	;

fk_clause_part:
	ON DELETE SET NULL				{ $$ = $1 + " " + $2 + " " + $3 + " " + $4; }
	| ON DELETE SET DEFAULT				{ $$ = $1 + " " + $2 + " " + $3 + " " + $4; }
	| ON DELETE CASCADE				{ $$ = $1 + " " + $2 + " " + $3; }
	| ON DELETE RESTRICT				{ $$ = $1 + " " + $2 + " " + $3; }
	| ON DELETE NO ACTION				{ $$ = $1 + " " + $2 + " " + $3 + " " + $4; }
	| ON UPDATE SET NULL				{ $$ = $1 + " " + $2 + " " + $3 + " " + $4; }
	| ON UPDATE SET DEFAULT				{ $$ = $1 + " " + $2 + " " + $3 + " " + $4; }
	| ON UPDATE CASCADE				{ $$ = $1 + " " + $2 + " " + $3; }
	| ON UPDATE RESTRICT				{ $$ = $1 + " " + $2 + " " + $3; }
	| ON UPDATE NO ACTION				{ $$ = $1 + " " + $2 + " " + $3 + " " + $4; }
	| ON INSERT SET NULL				{ $$ = $1 + " " + $2 + " " + $3 + " " + $4; }
	| ON INSERT SET DEFAULT				{ $$ = $1 + " " + $2 + " " + $3 + " " + $4; }
	| ON INSERT CASCADE				{ $$ = $1 + " " + $2 + " " + $3; }
	| ON INSERT RESTRICT				{ $$ = $1 + " " + $2 + " " + $3; }
	| ON INSERT NO ACTION				{ $$ = $1 + " " + $2 + " " + $3 + " " + $4; }
	| MATCH id					{ $$ = $1 + " " + $2; }
	;

fk_clause_part_list:
	fk_clause_part					{ $$ = $1; }
	| fk_clause_part_list fk_clause_part		{ $$ = $1 + " " + $2; }
	;

optional_fk_clause:
	%empty								{ }
	| fk_clause_part_list						{ $$ = $1; }
	| fk_clause_part_list DEFERRABLE INITIALLY DEFERRED		{ $$ = $1 + " " + $2 + " " + $3 + " " + $4; }
	| fk_clause_part_list DEFERRABLE INITIALLY IMMEDIATE		{ $$ = $1 + " " + $2 + " " + $3 + " " + $4; }
	| fk_clause_part_list DEFERRABLE				{ $$ = $1 + " " + $2; }
	| fk_clause_part_list NOT DEFERRABLE INITIALLY DEFERRED		{ $$ = $1 + " " + $2 + " " + $3 + " " + $4 + " " + $5; }
	| fk_clause_part_list NOT DEFERRABLE INITIALLY IMMEDIATE	{ $$ = $1 + " " + $2 + " " + $3 + " " + $4 + " " + $5; }
	| fk_clause_part_list NOT DEFERRABLE				{ $$ = $1 + " " + $2 + " " + $3; }
	| DEFERRABLE INITIALLY DEFERRED					{ $$ = $1 + " " + $2 + " " + $3; }
	| DEFERRABLE INITIALLY IMMEDIATE				{ $$ = $1 + " " + $2 + " " + $3; }
	| DEFERRABLE							{ $$ = $1; }
	| NOT DEFERRABLE INITIALLY DEFERRED				{ $$ = $1 + " " + $2 + " " + $3 + " " + $4; }
	| NOT DEFERRABLE INITIALLY IMMEDIATE				{ $$ = $1 + " " + $2 + " " + $3 + " " + $4; }
	| NOT DEFERRABLE						{ $$ = $1 + " " + $2; }
	;

tableconstraint:
	optional_constraintname PRIMARY KEY "(" indexed_column_list ")" optional_conflictclause		{
														auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>();
														pk->setName($1);
														pk->setConflictAction($7);
														$$.index.insert(std::make_pair($5, pk));
													}
	| optional_constraintname PRIMARY KEY "(" indexed_column_list AUTOINCREMENT ")" optional_conflictclause	{
														auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>();
														pk->setName($1);
														pk->setConflictAction($8);
														pk->setAutoIncrement(true);
														$$.index.insert(std::make_pair($5, pk));
													}
	| optional_constraintname UNIQUE "(" indexed_column_list ")" optional_conflictclause		{
														auto u = std::make_shared<sqlb::UniqueConstraint>();
														u->setName($1);
														u->setConflictAction($6);
														sqlb::StringVector columns;
														$$.index.insert(std::make_pair($4, u));
													}
	| optional_constraintname CHECK "(" expr ")"							{
														auto c = std::make_shared<sqlb::CheckConstraint>($4);
														c->setName($1);
														$$.check.push_back(c);
													}
	| optional_constraintname FOREIGN KEY "(" columnid_list ")" REFERENCES tableid optional_columnid_with_paren_list optional_fk_clause	{
														auto f = std::make_shared<sqlb::ForeignKeyClause>($8, $9, $10);
														f->setName($1);
														$$.fk.insert(std::make_pair($5, f));
													}
	;

tableconstraint_list:
	tableconstraint					{ $$ = $1; }
	| tableconstraint_list "," tableconstraint	{ $$ = $1; $$.index.insert($3.index.begin(), $3.index.end()); $$.fk.insert($3.fk.begin(), $3.fk.end()); std::copy($3.check.begin(), $3.check.end(), std::back_inserter($$.check)); }
	| tableconstraint_list tableconstraint		{ $$ = $1; $$.index.insert($2.index.begin(), $2.index.end()); $$.fk.insert($2.fk.begin(), $2.fk.end()); std::copy($2.check.begin(), $2.check.end(), std::back_inserter($$.check)); }
	;

optional_tableconstraint_list:
	%empty						{ }
	| "," tableconstraint_list			{ $$ = $2; }
	;

createtable_stmt:
	CREATE optional_temporary TABLE optional_if_not_exists tableid_with_uninteresting_schema AS select_stmt		{
										$$ = std::make_shared<sqlb::Table>($5);
										$$->setFullyParsed(false);
									}
	| CREATE optional_temporary TABLE optional_if_not_exists tableid_with_uninteresting_schema "(" columndef_list optional_tableconstraint_list ")" optional_tableoptions_list		{
										$$ = std::make_shared<sqlb::Table>($5);
										$$->setWithoutRowidTable($10.test(sqlb::Table::WithoutRowid));
										$$->setStrict($10.test(sqlb::Table::Strict));
										for(const auto& i : $8.index)
											$$->addConstraint(i.first, i.second);
										for(const auto& i : $8.fk)
											$$->addConstraint(i.first, i.second);
										for(const auto& i : $8.check)
											$$->addConstraint(i);
										$$->setFullyParsed(true);

										for(const auto& column : $7)
										{
											sqlb::Field f;
											Constraints c;
											std::tie(f, c) = column;

											$$->fields.push_back(f);
											for(const auto& i : c.index)
												$$->addConstraint(i.first, i.second);
											for(const auto& i : c.fk)
												$$->addConstraint(i.first, i.second);
											for(const auto& i : c.check)
												$$->addConstraint(i);
										}
									}
	;

%%

void sqlb::parser::parser::error(const location_type& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}
