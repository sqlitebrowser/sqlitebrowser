%{
#include <string>
#include "ParserDriver.h"
#include "sqlite3_parser.hpp"
%}

%option noyywrap nounput batch debug case-insensitive 8bit never-interactive nodefault nounistd reentrant warn
%option header-file="sqlite3_lexer.h"
%option outfile="sqlite3_lexer.cpp"

%{
	#define TOKEN(n) sqlb::parser::parser::symbol_type(sqlb::parser::parser::token::TOK_##n, yytext, loc)

	std::string unquote_string(std::string s, char quote_char)
	{
		if(s.size() < 2)
			return s;

		if(quote_char == '[')
		{
			if(s.front() == '[' && s.back() == ']')
				s = s.substr(1, s.size()-2);
		} else {
			if(s.front() == quote_char && s.back() == quote_char)
			{
				s = s.substr(1, s.size()-2);
				auto pos = s.npos;
				while((pos = s.find(std::string(2, quote_char))) != s.npos)
					s = s.replace(pos, 2, std::string(1, quote_char));
			}
		}

		return s;
	}
%}

U	[\x80-\xbf]
U2	[\xc2-\xdf]
U3	[\xe0-\xef]
U4	[\xf0-\xf4]
UNICODE	{U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U}

ID			([a-z_]|{UNICODE})([a-z0-9_]|{UNICODE})*

GRAVEQUOTEDID		`([^\n`]|(``))*`
SQUAREBRACKETID		\[([^\n\]])*\]

QUOTEDLITERAL		\"([^\n\"]|(\"\"))*\"
STRINGLITERAL		\'([^\n\']|(\'\'))*\'
BLOBLITERAL		X\'[0-9a-f]*\'

BINDPARAMETER_NUM	\?([1-9]{DIGIT}*)?
BINDPARAMETER_STR	[:@][a-z]+
	/* TODO Add $ bind parameters */

DIGIT			[0-9]
NUMERIC			(({DIGIT}+(\.{DIGIT}*)?|(\.{DIGIT}+))(e[\+\-]?{DIGIT}+)?)|(0x[0-9a-f]+)

NL			[\r\n]
WS			[ \t\f]

%{
	// Code run each time a pattern is matched.
	#define YY_USER_ACTION loc.columns(yyleng);
%}

%s BETWEEN_MODE

%%

%{
	// Shortcut to the location held by the driver
	sqlb::parser::location& loc = drv.location;

	// Code run each time yylex is called.
	loc.step();
%}

{WS}+		loc.step();
{NL}+		loc.lines(yyleng); loc.step();

"--"		{
			int c;
			while((c = yyinput(yyscanner)) != '\n' && c != EOF)
				;    /* eat up text of comment */
		}

"/*"		{
			int c;

			for(;;)
			{
				while((c = yyinput(yyscanner)) != '*' && c != EOF)
					;    /* eat up text of comment */

				if(c == '*')
				{
					while((c = yyinput(yyscanner)) == '*')
						;
					if(c == '/')
						break;    /* found the end */
				}

				if(c == EOF)
					throw sqlb::parser::parser::syntax_error(loc, "EOF in comment");
			}
		}

 /* For lack of a better idea, we need this hack to avoid reduce/reduce conflicts in the rules for parsing BETWEEN expressions.
  * What we do here is distinguish two types of AND operators: the regular one and the special case when the AND follows a BETWEEN keyword.
  */
<BETWEEN_MODE>"AND"	{ BEGIN INITIAL; return TOKEN(AND_BETWEEN); }
"AND"			return TOKEN(AND);
"BETWEEN"		{ BEGIN BETWEEN_MODE; return TOKEN(BETWEEN); }

"ABORT"			return TOKEN(ABORT);
"ACTION"		return TOKEN(ACTION);
"AS"			return TOKEN(AS);
"ASC"			return TOKEN(ASC);
"AUTOINCREMENT"		return TOKEN(AUTOINCREMENT);
"CASCADE"		return TOKEN(CASCADE);
"CASE"			return TOKEN(CASE);
"CAST"			return TOKEN(CAST);
"CHECK"			return TOKEN(CHECK);
"COLLATE"		return TOKEN(COLLATE);
"CONFLICT"		return TOKEN(CONFLICT);
"CONSTRAINT"		return TOKEN(CONSTRAINT);
"CREATE"		return TOKEN(CREATE);
"CURRENT_DATE"		return TOKEN(CURRENT_DATE);
"CURRENT_TIME"		return TOKEN(CURRENT_TIME);
"CURRENT_TIMESTAMP"	return TOKEN(CURRENT_TIMESTAMP);
"DEFAULT"		return TOKEN(DEFAULT);
"DEFERRABLE"		return TOKEN(DEFERRABLE);
"DEFERRED"		return TOKEN(DEFERRED);
"DELETE"		return TOKEN(DELETE);
"DESC"			return TOKEN(DESC);
"DISTINCT"		return TOKEN(DISTINCT);
"ELSE"			return TOKEN(ELSE);
"END"			return TOKEN(END);
"ESCAPE"		return TOKEN(ESCAPE);
"EXISTS"		return TOKEN(EXISTS);
"FAIL"			return TOKEN(FAIL);
"FALSE"			return TOKEN(FALSE);
"FILTER"		return TOKEN(FILTER);
"FOLLOWING"		return TOKEN(FOLLOWING);
"FOREIGN"		return TOKEN(FOREIGN);
"GLOB"			return TOKEN(GLOB);
"IF"			return TOKEN(IF);
"IGNORE"		return TOKEN(IGNORE);
"IMMEDIATE"		return TOKEN(IMMEDIATE);
"IN"			return TOKEN(IN);
"INDEX"			return TOKEN(INDEX);
"INITIALLY"		return TOKEN(INITIALLY);
"INSERT"		return TOKEN(INSERT);
"IS"			return TOKEN(IS);
"ISNULL"		return TOKEN(ISNULL);
"KEY"			return TOKEN(KEY);
"LIKE"			return TOKEN(LIKE);
"MATCH"			return TOKEN(MATCH);
"NO"			return TOKEN(NO);
"NOT"			return TOKEN(NOT);
"NOTNULL"		return TOKEN(NOTNULL);
"NULL"			return TOKEN(NULL);
"ON"			return TOKEN(ON);
"OR"			return TOKEN(OR);
"OVER"			return TOKEN(OVER);
"PARTITION"		return TOKEN(PARTITION);
"PRECEDING"		return TOKEN(PRECEDING);
"PRIMARY"		return TOKEN(PRIMARY);
"RAISE"			return TOKEN(RAISE);
"RANGE"			return TOKEN(RANGE);
"REFERENCES"		return TOKEN(REFERENCES);
"REGEXP"		return TOKEN(REGEXP);
"REPLACE"		return TOKEN(REPLACE);
"RESTRICT"		return TOKEN(RESTRICT);
"ROLLBACK"		return TOKEN(ROLLBACK);
"ROWID"			return TOKEN(ROWID);
"ROWS"			return TOKEN(ROWS);
"SELECT"		return TOKEN(SELECT);
"SET"			return TOKEN(SET);
"TABLE"			return TOKEN(TABLE);
"TEMP"			return TOKEN(TEMP);
"TEMPORARY"		return TOKEN(TEMPORARY);
"THEN"			return TOKEN(THEN);
"TRUE"			return TOKEN(TRUE);
"UNBOUNDED"		return TOKEN(UNBOUNDED);
"UNIQUE"		return TOKEN(UNIQUE);
"UPDATE"		return TOKEN(UPDATE);
"USING"			return TOKEN(USING);
"VIRTUAL"		return TOKEN(VIRTUAL);
"WHEN"			return TOKEN(WHEN);
"WHERE"			return TOKEN(WHERE);
"WITHOUT"		return TOKEN(WITHOUT);

{ID}			return sqlb::parser::parser::make_IDENTIFIER(yytext, loc);
{GRAVEQUOTEDID}		return sqlb::parser::parser::make_IDENTIFIER(unquote_string(yytext, '`'), loc);
{SQUAREBRACKETID}	return sqlb::parser::parser::make_IDENTIFIER(unquote_string(yytext, '['), loc);
{QUOTEDLITERAL}		return sqlb::parser::parser::make_QUOTEDLITERAL(unquote_string(yytext, '"'), loc);
{STRINGLITERAL}		return sqlb::parser::parser::make_STRINGLITERAL(yytext, loc);
{NUMERIC}		return sqlb::parser::parser::make_NUMERIC(yytext, loc);
{BLOBLITERAL}		return sqlb::parser::parser::make_BLOBLITERAL(yytext, loc);
{BINDPARAMETER_NUM}	return sqlb::parser::parser::make_BINDPARAMETER(yytext, loc);
{BINDPARAMETER_STR}	return sqlb::parser::parser::make_BINDPARAMETER(yytext, loc);

"("		return sqlb::parser::parser::make_LPAREN(loc);
")"		return sqlb::parser::parser::make_RPAREN(loc);
"."		return sqlb::parser::parser::make_DOT(loc);
","		return sqlb::parser::parser::make_COMMA(loc);
";"		return sqlb::parser::parser::make_SEMI(loc);
"+"		return sqlb::parser::parser::make_PLUS(loc);
"-"		return sqlb::parser::parser::make_MINUS(loc);
"*"		return sqlb::parser::parser::make_STAR(loc);
"/"		return sqlb::parser::parser::make_SLASH(loc);
"~"		return sqlb::parser::parser::make_TILDE(loc);
"&"		return sqlb::parser::parser::make_AMPERSAND(loc);
"%"		return sqlb::parser::parser::make_PERCENT(loc);
"|"		return sqlb::parser::parser::make_BITOR(loc);
"||"		return sqlb::parser::parser::make_OROP(loc);
"="		return sqlb::parser::parser::make_EQUAL(loc);
"=="		return sqlb::parser::parser::make_EQUAL2(loc);
">"		return sqlb::parser::parser::make_GREATER(loc);
">="		return sqlb::parser::parser::make_GREATEREQUAL(loc);
"<"		return sqlb::parser::parser::make_LOWER(loc);
"<="		return sqlb::parser::parser::make_LOWEREQUAL(loc);
"!="		return sqlb::parser::parser::make_UNEQUAL(loc);
"<>"		return sqlb::parser::parser::make_UNEQUAL2(loc);
"<<"		return sqlb::parser::parser::make_BITWISELEFT(loc);
">>"		return sqlb::parser::parser::make_BITWISERIGHT(loc);

.		throw sqlb::parser::parser::syntax_error(loc, "Invalid character: " + std::string(yytext));

<<EOF>>		return sqlb::parser::parser::make_EOF(loc);

%%

namespace sqlb
{
namespace parser
{

void ParserDriver::begin_scan()
{
	yylex_init(&scanner);
	location.initialize();
	yyset_debug(trace_scanner, scanner);
	buffer = yy_scan_string(source.c_str(), scanner);
}

void ParserDriver::end_scan()
{
	yy_delete_buffer(buffer, scanner);
	yylex_destroy(scanner);
}

}
}
