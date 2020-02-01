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

	// Unfortunately we do not store column constraints in a systematic manner yet.
	// Instead there is a variable for most column constraints directly inside the
	// sqlb::Field class. This means that when parsing a column constraint we cannot
	// just build a column constraint object with all the information and insert that
	// into the Field object. Instead, the information needs to be passed upwards in
	// some other way. This is what these declarations are for. We need to be able
	// to pass information to the Field object as well as to the Table object too
	// because some column constraints need to be transformed into Table constraints
	// with our current layout.
	class ColumnConstraintInfo
	{
	public:
		ColumnConstraintInfo() : is_table_constraint(false), fully_parsed(false) {}
		~ColumnConstraintInfo() {}
		ColumnConstraintInfo& operator=(const ColumnConstraintInfo& other)
		{
			type = other.type;
			is_table_constraint = other.is_table_constraint;
			fully_parsed = other.fully_parsed;
			if(is_table_constraint)
				table_constraint = other.table_constraint;
			text = other.text;

			return *this;
		}
		ColumnConstraintInfo(const ColumnConstraintInfo& other)
		{
			*this = other;
		}

		enum ConstraintType
		{
			None,
			AutoIncrement,
			PrimaryKey,
			NotNull,
			Unique,
			Check,
			Default,
			Collate,
			ForeignKey,
		};

		ConstraintType type;
		bool is_table_constraint;
		bool fully_parsed;

		sqlb::ConstraintPtr table_constraint;
		std::string text;
	};
	using ColumnConstraintInfoVector = std::vector<ColumnConstraintInfo>;

	// Colum definitions are a tuple of three elements: the Field object, a set of table constraints, and a bool to indicate whether parsing was complete
	using ColumndefData = std::tuple<sqlb::Field, sqlb::ConstraintSet, bool>;
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
%type <bool> optional_temporary
%type <bool> optional_withoutrowid
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
%type <ColumnConstraintInfoVector> columnconstraint_list
%type <ColumnConstraintInfo> columnconstraint
%type <ColumndefData> columndef
%type <std::vector<ColumndefData>> columndef_list
%type <sqlb::StringVector> columnid_list
%type <sqlb::StringVector> optional_columnid_with_paren_list
%type <std::string> fk_clause_part
%type <std::string> fk_clause_part_list
%type <std::string> optional_fk_clause
%type <sqlb::ConstraintPtr> tableconstraint
%type <sqlb::ConstraintSet> tableconstraint_list
%type <sqlb::ConstraintSet> optional_tableconstraint_list
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

/*
 * CREATE VIRTUAL TABLE
 */

optional_exprlist_with_paren:
	%empty					{ $$ = {}; }
	| "(" ")"				{ $$ = {}; }
	| "(" exprlist_expr ")"			{ $$ = $2; }
	;

createvirtualtable_stmt:
	CREATE VIRTUAL TABLE optional_if_not_exists tableid_with_uninteresting_schema USING id optional_exprlist_with_paren	{
													$$ = sqlb::TablePtr(new sqlb::Table($5));
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

optional_withoutrowid:
	%empty						{ $$ = false; }
	| WITHOUT ROWID					{ $$ = true; }
	;

optional_conflictclause:
	%empty						{ $$ = ""; }
	| ON CONFLICT ROLLBACK				{ $$ = $3; }
	| ON CONFLICT ABORT				{ $$ = $3; }
	| ON CONFLICT FAIL				{ $$ = $3; }
	| ON CONFLICT IGNORE				{ $$ = $3; }
	| ON CONFLICT REPLACE				{ $$ = $3; }
	;

optional_typename:
	%empty					{ $$ = ""; }
	| type_name				{ $$ = $1; }
	;

columnconstraint:
	optional_constraintname PRIMARY KEY optional_sort_order optional_conflictclause	{
												$$.type = ColumnConstraintInfo::PrimaryKey;
												$$.is_table_constraint = true;
												sqlb::PrimaryKeyConstraint* pk = new sqlb::PrimaryKeyConstraint({sqlb::IndexedColumn("", false, $4)});
												pk->setName($1);
												pk->setConflictAction($5);
												$$.table_constraint = sqlb::ConstraintPtr(pk);
												$$.fully_parsed = true;
											}
	| optional_constraintname PRIMARY KEY optional_sort_order optional_conflictclause AUTOINCREMENT	{
												$$.type = ColumnConstraintInfo::PrimaryKey;
												$$.is_table_constraint = true;
												sqlb::PrimaryKeyConstraint* pk = new sqlb::PrimaryKeyConstraint({sqlb::IndexedColumn("", false, $4)});
												pk->setName($1);
												pk->setConflictAction($5);
												pk->setAutoIncrement(true);
												$$.table_constraint = sqlb::ConstraintPtr(pk);
												$$.fully_parsed = true;
											}
	| optional_constraintname NOT NULL optional_conflictclause			{
												$$.type = ColumnConstraintInfo::NotNull;
												$$.is_table_constraint = false;
												$$.fully_parsed = ($1 == "" && $4 == "");
											}
	| optional_constraintname NULL 							{
												$$.type = ColumnConstraintInfo::None;
												$$.is_table_constraint = false;
												$$.fully_parsed = true;
											}
	| optional_constraintname UNIQUE optional_conflictclause			{
												$$.type = ColumnConstraintInfo::Unique;
												$$.is_table_constraint = false;
												$$.fully_parsed = ($1 == "" && $3 == "");
											}
	| optional_constraintname CHECK "(" expr ")"					{
												$$.type = ColumnConstraintInfo::Check;
												$$.is_table_constraint = false;
												$$.text = $4;
												$$.fully_parsed = ($1 == "");
											}
	| optional_constraintname DEFAULT signednumber					{
												$$.type = ColumnConstraintInfo::Default;
												$$.is_table_constraint = false;
												$$.text = $3;
												$$.fully_parsed = ($1 == "");
											}
	| optional_constraintname DEFAULT literalvalue					{
												$$.type = ColumnConstraintInfo::Default;
												$$.is_table_constraint = false;
												$$.text = $3;
												$$.fully_parsed = ($1 == "");
											}
	| optional_constraintname DEFAULT id						{
												$$.type = ColumnConstraintInfo::Default;
												$$.is_table_constraint = false;
												$$.text = $3;
												$$.fully_parsed = ($1 == "");
											}
	| optional_constraintname DEFAULT allowed_keywords_as_identifier		{	// We must allow the same keywords as unquoted default values as in the columnid context.
												// But we do not use columnid here in order to avoid reduce/reduce conflicts.
												$$.type = ColumnConstraintInfo::Default;
												$$.is_table_constraint = false;
												$$.text = $3;
												$$.fully_parsed = ($1 == "");
											}
	| optional_constraintname DEFAULT IF						{	// Same as above.
												$$.type = ColumnConstraintInfo::Default;
												$$.is_table_constraint = false;
												$$.text = $3;
												$$.fully_parsed = ($1 == "");
											}
	| optional_constraintname DEFAULT "(" expr ")"					{
												$$.type = ColumnConstraintInfo::Default;
												$$.is_table_constraint = false;
												$$.text = "(" + $4 + ")";
												$$.fully_parsed = ($1 == "");
											}
	| optional_constraintname COLLATE id						{
												$$.type = ColumnConstraintInfo::Collate;
												$$.is_table_constraint = false;
												$$.text = $3;
												$$.fully_parsed = ($1 == "");
											}
	| optional_constraintname REFERENCES tableid optional_columnid_with_paren_list optional_fk_clause	{	// TODO Solve shift/reduce conflict. It is not super important though as shifting seems to be right here.
												$$.type = ColumnConstraintInfo::ForeignKey;
												$$.is_table_constraint = true;
												sqlb::ForeignKeyClause* fk = new sqlb::ForeignKeyClause();
												fk->setName($1);
												fk->setTable($3);
												fk->setColumns($4);
												fk->setConstraint($5);
												$$.table_constraint = sqlb::ConstraintPtr(fk);
												$$.fully_parsed = true;
											}
	;

columnconstraint_list:
	columnconstraint				{ $$ = { $1 }; }
	| columnconstraint_list columnconstraint	{ $$ = $1; $$.push_back($2); }
	;

columndef:
	columnid optional_typename columnconstraint_list	{
								sqlb::Field f($1, $2);
								bool fully_parsed = true;
								sqlb::ConstraintSet table_constraints{};
								for(auto c : $3)
								{
									if(c.fully_parsed == false)
										fully_parsed = false;

									if(c.type == ColumnConstraintInfo::None)
										continue;

									if(c.is_table_constraint)
									{
										if(c.table_constraint->columnList().empty())
											c.table_constraint->setColumnList({$1});
										else
											c.table_constraint->replaceInColumnList("", $1);
										table_constraints.insert(c.table_constraint);
									} else {
										if(c.type == ColumnConstraintInfo::NotNull) {
											f.setNotNull(true);
										} else if(c.type == ColumnConstraintInfo::Unique) {
											f.setUnique(true);
										} else if(c.type == ColumnConstraintInfo::Check) {
											f.setCheck(c.text);
										} else if(c.type == ColumnConstraintInfo::Default) {
											f.setDefaultValue(c.text);
										} else if(c.type == ColumnConstraintInfo::Collate) {
											f.setCollation(c.text);
										} else {
											fully_parsed = false;
										}
									}
								}

								$$ = std::make_tuple(f, table_constraints, fully_parsed);
							}
	| columnid optional_typename			{ $$ = std::make_tuple(sqlb::Field($1, $2), sqlb::ConstraintSet{}, true); }
	;

columndef_list:
	columndef					{ $$ = {$1}; }
	| columndef_list "," columndef			{ $$ = $1; $$.push_back($3); }
	;

optional_constraintname:
	%empty						{ $$ = ""; }
	| CONSTRAINT id					{ $$ = $2; }
	;

columnid_list:
	columnid					{ $$ = sqlb::StringVector(1, $1); }
	| columnid_list "," columnid			{ $$ = $1; $$.push_back($3); }
	;

optional_columnid_with_paren_list:
	%empty						{ $$ = sqlb::StringVector(); }
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
	%empty								{ $$ = ""; }
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
														sqlb::PrimaryKeyConstraint* pk = new sqlb::PrimaryKeyConstraint($5);
														pk->setName($1);
														pk->setConflictAction($7);
														$$ = sqlb::ConstraintPtr(pk);
													}
	| optional_constraintname PRIMARY KEY "(" indexed_column_list AUTOINCREMENT ")" optional_conflictclause	{
														sqlb::PrimaryKeyConstraint* pk = new sqlb::PrimaryKeyConstraint($5);
														pk->setName($1);
														pk->setConflictAction($8);
														pk->setAutoIncrement(true);
														$$ = sqlb::ConstraintPtr(pk);
													}
	| optional_constraintname UNIQUE "(" indexed_column_list ")" optional_conflictclause		{
														sqlb::UniqueConstraint* u = new sqlb::UniqueConstraint($4);
														u->setName($1);
														u->setConflictAction($6);
														$$ = sqlb::ConstraintPtr(u);
													}
	| optional_constraintname CHECK "(" expr ")"							{
														$$ = sqlb::ConstraintPtr(new sqlb::CheckConstraint($4));
														$$->setName($1);
													}
	| optional_constraintname FOREIGN KEY "(" columnid_list ")" REFERENCES tableid optional_columnid_with_paren_list optional_fk_clause	{
														$$ = sqlb::ConstraintPtr(new sqlb::ForeignKeyClause($8, $9, $10));
														$$->setColumnList($5);
														$$->setName($1);
													}
	;

tableconstraint_list:
	tableconstraint					{ $$ = {$1}; }
	| tableconstraint_list "," tableconstraint	{ $$ = $1; $$.insert($3); }
	| tableconstraint_list tableconstraint		{ $$ = $1; $$.insert($2); }
	;

optional_tableconstraint_list:
	%empty						{ $$ = {}; }
	| "," tableconstraint_list			{ $$ = $2; }
	;

createtable_stmt:
	CREATE optional_temporary TABLE optional_if_not_exists tableid_with_uninteresting_schema AS select_stmt		{
										$$ = sqlb::TablePtr(new sqlb::Table($5));
										$$->setFullyParsed(false);
									}
	| CREATE optional_temporary TABLE optional_if_not_exists tableid_with_uninteresting_schema "(" columndef_list optional_tableconstraint_list ")" optional_withoutrowid		{
										$$ = sqlb::TablePtr(new sqlb::Table($5));
										$$->setWithoutRowidTable($10);
										$$->setConstraints($8);
										$$->setFullyParsed(true);

										for(const auto& column : $7)
										{
											sqlb::Field f;
											sqlb::ConstraintSet c;
											bool fully_parsed;
											std::tie(f, c, fully_parsed) = column;

											if(fully_parsed == false)
												$$->setFullyParsed(false);
											$$->fields.push_back(f);
											for(const auto& i : c)
												$$->addConstraint(i);
										}
									}
	;

%%

void sqlb::parser::parser::error(const location_type& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}
