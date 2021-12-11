// A Bison parser, made by GNU Bison 3.7.5.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "sqlite3_parser.hpp"


// Unqualified %code blocks.
#line 33 "sqlite3_parser.yy"

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

#line 76 "sqlite3_parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 10 "sqlite3_parser.yy"
namespace  sqlb { namespace parser  {
#line 169 "sqlite3_parser.cpp"

  /// Build a parser object.
  parser::parser (yyscan_t yyscanner_yyarg, ParserDriver& drv_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      yyscanner (yyscanner_yyarg),
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_columndef: // columndef
        value.YY_MOVE_OR_COPY< ColumndefData > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_withoutrowid: // optional_withoutrowid
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
      case symbol_kind::S_tableconstraint: // tableconstraint
        value.YY_MOVE_OR_COPY< sqlb::ConstraintPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.YY_MOVE_OR_COPY< sqlb::ConstraintVector > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_createindex_stmt: // createindex_stmt
        value.YY_MOVE_OR_COPY< sqlb::IndexPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_indexed_column: // indexed_column
        value.YY_MOVE_OR_COPY< sqlb::IndexedColumn > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_indexed_column_list: // indexed_column_list
        value.YY_MOVE_OR_COPY< sqlb::IndexedColumnVector > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columnid_list: // columnid_list
      case symbol_kind::S_optional_columnid_with_paren_list: // optional_columnid_with_paren_list
        value.YY_MOVE_OR_COPY< sqlb::StringVector > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_createvirtualtable_stmt: // createvirtualtable_stmt
      case symbol_kind::S_createtable_stmt: // createtable_stmt
        value.YY_MOVE_OR_COPY< sqlb::TablePtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ABORT: // "ABORT"
      case symbol_kind::S_ACTION: // "ACTION"
      case symbol_kind::S_ALWAYS: // "ALWAYS"
      case symbol_kind::S_AND: // "AND"
      case symbol_kind::S_AND_BETWEEN: // "AND BETWEEN"
      case symbol_kind::S_AS: // "AS"
      case symbol_kind::S_ASC: // "ASC"
      case symbol_kind::S_AUTOINCREMENT: // "AUTOINCREMENT"
      case symbol_kind::S_BETWEEN: // "BETWEEN"
      case symbol_kind::S_CASCADE: // "CASCADE"
      case symbol_kind::S_CASE: // "CASE"
      case symbol_kind::S_CAST: // "CAST"
      case symbol_kind::S_CHECK: // "CHECK"
      case symbol_kind::S_COLLATE: // "COLLATE"
      case symbol_kind::S_CONFLICT: // "CONFLICT"
      case symbol_kind::S_CONSTRAINT: // "CONSTRAINT"
      case symbol_kind::S_CREATE: // "CREATE"
      case symbol_kind::S_CURRENT_DATE: // "CURRENT_DATE"
      case symbol_kind::S_CURRENT_TIME: // "CURRENT_TIME"
      case symbol_kind::S_CURRENT_TIMESTAMP: // "CURRENT_TIMESTAMP"
      case symbol_kind::S_DEFAULT: // "DEFAULT"
      case symbol_kind::S_DEFERRABLE: // "DEFERRABLE"
      case symbol_kind::S_DEFERRED: // "DEFERRED"
      case symbol_kind::S_DELETE: // "DELETE"
      case symbol_kind::S_DESC: // "DESC"
      case symbol_kind::S_DISTINCT: // "DISTINCT"
      case symbol_kind::S_ELSE: // "ELSE"
      case symbol_kind::S_END: // "END"
      case symbol_kind::S_ESCAPE: // "ESCAPE"
      case symbol_kind::S_EXISTS: // "EXISTS"
      case symbol_kind::S_FAIL: // "FAIL"
      case symbol_kind::S_FALSE: // "FALSE"
      case symbol_kind::S_FILTER: // "FILTER"
      case symbol_kind::S_FOLLOWING: // "FOLLOWING"
      case symbol_kind::S_FOREIGN: // "FOREIGN"
      case symbol_kind::S_GENERATED: // "GENERATED"
      case symbol_kind::S_GLOB: // "GLOB"
      case symbol_kind::S_IF: // "IF"
      case symbol_kind::S_IGNORE: // "IGNORE"
      case symbol_kind::S_IMMEDIATE: // "IMMEDIATE"
      case symbol_kind::S_IN: // "IN"
      case symbol_kind::S_INDEX: // "INDEX"
      case symbol_kind::S_INITIALLY: // "INITIALLY"
      case symbol_kind::S_INSERT: // "INSERT"
      case symbol_kind::S_IS: // "IS"
      case symbol_kind::S_ISNULL: // "ISNULL"
      case symbol_kind::S_KEY: // "KEY"
      case symbol_kind::S_LIKE: // "LIKE"
      case symbol_kind::S_MATCH: // "MATCH"
      case symbol_kind::S_NO: // "NO"
      case symbol_kind::S_NOT: // "NOT"
      case symbol_kind::S_NOTNULL: // "NOTNULL"
      case symbol_kind::S_NULL: // "NULL"
      case symbol_kind::S_ON: // "ON"
      case symbol_kind::S_OR: // "OR"
      case symbol_kind::S_OVER: // "OVER"
      case symbol_kind::S_PARTITION: // "PARTITION"
      case symbol_kind::S_PRECEDING: // "PRECEDING"
      case symbol_kind::S_PRIMARY: // "PRIMARY"
      case symbol_kind::S_RAISE: // "RAISE"
      case symbol_kind::S_RANGE: // "RANGE"
      case symbol_kind::S_REFERENCES: // "REFERENCES"
      case symbol_kind::S_REGEXP: // "REGEXP"
      case symbol_kind::S_REPLACE: // "REPLACE"
      case symbol_kind::S_RESTRICT: // "RESTRICT"
      case symbol_kind::S_RETURNING: // "RETURNING"
      case symbol_kind::S_ROLLBACK: // "ROLLBACK"
      case symbol_kind::S_ROWID: // "ROWID"
      case symbol_kind::S_ROWS: // "ROWS"
      case symbol_kind::S_SELECT: // "SELECT"
      case symbol_kind::S_SET: // "SET"
      case symbol_kind::S_STORED: // "STORED"
      case symbol_kind::S_TABLE: // "TABLE"
      case symbol_kind::S_TEMP: // "TEMP"
      case symbol_kind::S_TEMPORARY: // "TEMPORARY"
      case symbol_kind::S_THEN: // "THEN"
      case symbol_kind::S_TRUE: // "TRUE"
      case symbol_kind::S_UNBOUNDED: // "UNBOUNDED"
      case symbol_kind::S_UNIQUE: // "UNIQUE"
      case symbol_kind::S_UPDATE: // "UPDATE"
      case symbol_kind::S_USING: // "USING"
      case symbol_kind::S_VIRTUAL: // "VIRTUAL"
      case symbol_kind::S_WHEN: // "WHEN"
      case symbol_kind::S_WHERE: // "WHERE"
      case symbol_kind::S_WITHOUT: // "WITHOUT"
      case symbol_kind::S_IDENTIFIER: // "identifier"
      case symbol_kind::S_NUMERIC: // "numeric"
      case symbol_kind::S_STRINGLITERAL: // "string literal"
      case symbol_kind::S_QUOTEDLITERAL: // "quoted literal"
      case symbol_kind::S_BLOBLITERAL: // "blob literal"
      case symbol_kind::S_BINDPARAMETER: // "bind parameter"
      case symbol_kind::S_literalvalue: // literalvalue
      case symbol_kind::S_id: // id
      case symbol_kind::S_allowed_keywords_as_identifier: // allowed_keywords_as_identifier
      case symbol_kind::S_tableid: // tableid
      case symbol_kind::S_columnid: // columnid
      case symbol_kind::S_signednumber: // signednumber
      case symbol_kind::S_signednumber_or_numeric: // signednumber_or_numeric
      case symbol_kind::S_typename_namelist: // typename_namelist
      case symbol_kind::S_type_name: // type_name
      case symbol_kind::S_unary_expr: // unary_expr
      case symbol_kind::S_binary_expr: // binary_expr
      case symbol_kind::S_like_expr: // like_expr
      case symbol_kind::S_exprlist_expr: // exprlist_expr
      case symbol_kind::S_function_expr: // function_expr
      case symbol_kind::S_isnull_expr: // isnull_expr
      case symbol_kind::S_between_expr: // between_expr
      case symbol_kind::S_in_expr: // in_expr
      case symbol_kind::S_whenthenlist_expr: // whenthenlist_expr
      case symbol_kind::S_case_expr: // case_expr
      case symbol_kind::S_raise_expr: // raise_expr
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_select_stmt: // select_stmt
      case symbol_kind::S_optional_sort_order: // optional_sort_order
      case symbol_kind::S_optional_where: // optional_where
      case symbol_kind::S_tableid_with_uninteresting_schema: // tableid_with_uninteresting_schema
      case symbol_kind::S_optional_exprlist_with_paren: // optional_exprlist_with_paren
      case symbol_kind::S_optional_conflictclause: // optional_conflictclause
      case symbol_kind::S_optional_typename: // optional_typename
      case symbol_kind::S_optional_storage_identifier: // optional_storage_identifier
      case symbol_kind::S_optional_constraintname: // optional_constraintname
      case symbol_kind::S_fk_clause_part: // fk_clause_part
      case symbol_kind::S_fk_clause_part_list: // fk_clause_part_list
      case symbol_kind::S_optional_fk_clause: // optional_fk_clause
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columndef_list: // columndef_list
        value.YY_MOVE_OR_COPY< std::vector<ColumndefData> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_columndef: // columndef
        value.move< ColumndefData > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_withoutrowid: // optional_withoutrowid
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.move< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
      case symbol_kind::S_tableconstraint: // tableconstraint
        value.move< sqlb::ConstraintPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.move< sqlb::ConstraintVector > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_createindex_stmt: // createindex_stmt
        value.move< sqlb::IndexPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_indexed_column: // indexed_column
        value.move< sqlb::IndexedColumn > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_indexed_column_list: // indexed_column_list
        value.move< sqlb::IndexedColumnVector > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columnid_list: // columnid_list
      case symbol_kind::S_optional_columnid_with_paren_list: // optional_columnid_with_paren_list
        value.move< sqlb::StringVector > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_createvirtualtable_stmt: // createvirtualtable_stmt
      case symbol_kind::S_createtable_stmt: // createtable_stmt
        value.move< sqlb::TablePtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ABORT: // "ABORT"
      case symbol_kind::S_ACTION: // "ACTION"
      case symbol_kind::S_ALWAYS: // "ALWAYS"
      case symbol_kind::S_AND: // "AND"
      case symbol_kind::S_AND_BETWEEN: // "AND BETWEEN"
      case symbol_kind::S_AS: // "AS"
      case symbol_kind::S_ASC: // "ASC"
      case symbol_kind::S_AUTOINCREMENT: // "AUTOINCREMENT"
      case symbol_kind::S_BETWEEN: // "BETWEEN"
      case symbol_kind::S_CASCADE: // "CASCADE"
      case symbol_kind::S_CASE: // "CASE"
      case symbol_kind::S_CAST: // "CAST"
      case symbol_kind::S_CHECK: // "CHECK"
      case symbol_kind::S_COLLATE: // "COLLATE"
      case symbol_kind::S_CONFLICT: // "CONFLICT"
      case symbol_kind::S_CONSTRAINT: // "CONSTRAINT"
      case symbol_kind::S_CREATE: // "CREATE"
      case symbol_kind::S_CURRENT_DATE: // "CURRENT_DATE"
      case symbol_kind::S_CURRENT_TIME: // "CURRENT_TIME"
      case symbol_kind::S_CURRENT_TIMESTAMP: // "CURRENT_TIMESTAMP"
      case symbol_kind::S_DEFAULT: // "DEFAULT"
      case symbol_kind::S_DEFERRABLE: // "DEFERRABLE"
      case symbol_kind::S_DEFERRED: // "DEFERRED"
      case symbol_kind::S_DELETE: // "DELETE"
      case symbol_kind::S_DESC: // "DESC"
      case symbol_kind::S_DISTINCT: // "DISTINCT"
      case symbol_kind::S_ELSE: // "ELSE"
      case symbol_kind::S_END: // "END"
      case symbol_kind::S_ESCAPE: // "ESCAPE"
      case symbol_kind::S_EXISTS: // "EXISTS"
      case symbol_kind::S_FAIL: // "FAIL"
      case symbol_kind::S_FALSE: // "FALSE"
      case symbol_kind::S_FILTER: // "FILTER"
      case symbol_kind::S_FOLLOWING: // "FOLLOWING"
      case symbol_kind::S_FOREIGN: // "FOREIGN"
      case symbol_kind::S_GENERATED: // "GENERATED"
      case symbol_kind::S_GLOB: // "GLOB"
      case symbol_kind::S_IF: // "IF"
      case symbol_kind::S_IGNORE: // "IGNORE"
      case symbol_kind::S_IMMEDIATE: // "IMMEDIATE"
      case symbol_kind::S_IN: // "IN"
      case symbol_kind::S_INDEX: // "INDEX"
      case symbol_kind::S_INITIALLY: // "INITIALLY"
      case symbol_kind::S_INSERT: // "INSERT"
      case symbol_kind::S_IS: // "IS"
      case symbol_kind::S_ISNULL: // "ISNULL"
      case symbol_kind::S_KEY: // "KEY"
      case symbol_kind::S_LIKE: // "LIKE"
      case symbol_kind::S_MATCH: // "MATCH"
      case symbol_kind::S_NO: // "NO"
      case symbol_kind::S_NOT: // "NOT"
      case symbol_kind::S_NOTNULL: // "NOTNULL"
      case symbol_kind::S_NULL: // "NULL"
      case symbol_kind::S_ON: // "ON"
      case symbol_kind::S_OR: // "OR"
      case symbol_kind::S_OVER: // "OVER"
      case symbol_kind::S_PARTITION: // "PARTITION"
      case symbol_kind::S_PRECEDING: // "PRECEDING"
      case symbol_kind::S_PRIMARY: // "PRIMARY"
      case symbol_kind::S_RAISE: // "RAISE"
      case symbol_kind::S_RANGE: // "RANGE"
      case symbol_kind::S_REFERENCES: // "REFERENCES"
      case symbol_kind::S_REGEXP: // "REGEXP"
      case symbol_kind::S_REPLACE: // "REPLACE"
      case symbol_kind::S_RESTRICT: // "RESTRICT"
      case symbol_kind::S_RETURNING: // "RETURNING"
      case symbol_kind::S_ROLLBACK: // "ROLLBACK"
      case symbol_kind::S_ROWID: // "ROWID"
      case symbol_kind::S_ROWS: // "ROWS"
      case symbol_kind::S_SELECT: // "SELECT"
      case symbol_kind::S_SET: // "SET"
      case symbol_kind::S_STORED: // "STORED"
      case symbol_kind::S_TABLE: // "TABLE"
      case symbol_kind::S_TEMP: // "TEMP"
      case symbol_kind::S_TEMPORARY: // "TEMPORARY"
      case symbol_kind::S_THEN: // "THEN"
      case symbol_kind::S_TRUE: // "TRUE"
      case symbol_kind::S_UNBOUNDED: // "UNBOUNDED"
      case symbol_kind::S_UNIQUE: // "UNIQUE"
      case symbol_kind::S_UPDATE: // "UPDATE"
      case symbol_kind::S_USING: // "USING"
      case symbol_kind::S_VIRTUAL: // "VIRTUAL"
      case symbol_kind::S_WHEN: // "WHEN"
      case symbol_kind::S_WHERE: // "WHERE"
      case symbol_kind::S_WITHOUT: // "WITHOUT"
      case symbol_kind::S_IDENTIFIER: // "identifier"
      case symbol_kind::S_NUMERIC: // "numeric"
      case symbol_kind::S_STRINGLITERAL: // "string literal"
      case symbol_kind::S_QUOTEDLITERAL: // "quoted literal"
      case symbol_kind::S_BLOBLITERAL: // "blob literal"
      case symbol_kind::S_BINDPARAMETER: // "bind parameter"
      case symbol_kind::S_literalvalue: // literalvalue
      case symbol_kind::S_id: // id
      case symbol_kind::S_allowed_keywords_as_identifier: // allowed_keywords_as_identifier
      case symbol_kind::S_tableid: // tableid
      case symbol_kind::S_columnid: // columnid
      case symbol_kind::S_signednumber: // signednumber
      case symbol_kind::S_signednumber_or_numeric: // signednumber_or_numeric
      case symbol_kind::S_typename_namelist: // typename_namelist
      case symbol_kind::S_type_name: // type_name
      case symbol_kind::S_unary_expr: // unary_expr
      case symbol_kind::S_binary_expr: // binary_expr
      case symbol_kind::S_like_expr: // like_expr
      case symbol_kind::S_exprlist_expr: // exprlist_expr
      case symbol_kind::S_function_expr: // function_expr
      case symbol_kind::S_isnull_expr: // isnull_expr
      case symbol_kind::S_between_expr: // between_expr
      case symbol_kind::S_in_expr: // in_expr
      case symbol_kind::S_whenthenlist_expr: // whenthenlist_expr
      case symbol_kind::S_case_expr: // case_expr
      case symbol_kind::S_raise_expr: // raise_expr
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_select_stmt: // select_stmt
      case symbol_kind::S_optional_sort_order: // optional_sort_order
      case symbol_kind::S_optional_where: // optional_where
      case symbol_kind::S_tableid_with_uninteresting_schema: // tableid_with_uninteresting_schema
      case symbol_kind::S_optional_exprlist_with_paren: // optional_exprlist_with_paren
      case symbol_kind::S_optional_conflictclause: // optional_conflictclause
      case symbol_kind::S_optional_typename: // optional_typename
      case symbol_kind::S_optional_storage_identifier: // optional_storage_identifier
      case symbol_kind::S_optional_constraintname: // optional_constraintname
      case symbol_kind::S_fk_clause_part: // fk_clause_part
      case symbol_kind::S_fk_clause_part_list: // fk_clause_part_list
      case symbol_kind::S_optional_fk_clause: // optional_fk_clause
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columndef_list: // columndef_list
        value.move< std::vector<ColumndefData> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_columndef: // columndef
        value.copy< ColumndefData > (that.value);
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_withoutrowid: // optional_withoutrowid
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.copy< bool > (that.value);
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
      case symbol_kind::S_tableconstraint: // tableconstraint
        value.copy< sqlb::ConstraintPtr > (that.value);
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.copy< sqlb::ConstraintVector > (that.value);
        break;

      case symbol_kind::S_createindex_stmt: // createindex_stmt
        value.copy< sqlb::IndexPtr > (that.value);
        break;

      case symbol_kind::S_indexed_column: // indexed_column
        value.copy< sqlb::IndexedColumn > (that.value);
        break;

      case symbol_kind::S_indexed_column_list: // indexed_column_list
        value.copy< sqlb::IndexedColumnVector > (that.value);
        break;

      case symbol_kind::S_columnid_list: // columnid_list
      case symbol_kind::S_optional_columnid_with_paren_list: // optional_columnid_with_paren_list
        value.copy< sqlb::StringVector > (that.value);
        break;

      case symbol_kind::S_createvirtualtable_stmt: // createvirtualtable_stmt
      case symbol_kind::S_createtable_stmt: // createtable_stmt
        value.copy< sqlb::TablePtr > (that.value);
        break;

      case symbol_kind::S_ABORT: // "ABORT"
      case symbol_kind::S_ACTION: // "ACTION"
      case symbol_kind::S_ALWAYS: // "ALWAYS"
      case symbol_kind::S_AND: // "AND"
      case symbol_kind::S_AND_BETWEEN: // "AND BETWEEN"
      case symbol_kind::S_AS: // "AS"
      case symbol_kind::S_ASC: // "ASC"
      case symbol_kind::S_AUTOINCREMENT: // "AUTOINCREMENT"
      case symbol_kind::S_BETWEEN: // "BETWEEN"
      case symbol_kind::S_CASCADE: // "CASCADE"
      case symbol_kind::S_CASE: // "CASE"
      case symbol_kind::S_CAST: // "CAST"
      case symbol_kind::S_CHECK: // "CHECK"
      case symbol_kind::S_COLLATE: // "COLLATE"
      case symbol_kind::S_CONFLICT: // "CONFLICT"
      case symbol_kind::S_CONSTRAINT: // "CONSTRAINT"
      case symbol_kind::S_CREATE: // "CREATE"
      case symbol_kind::S_CURRENT_DATE: // "CURRENT_DATE"
      case symbol_kind::S_CURRENT_TIME: // "CURRENT_TIME"
      case symbol_kind::S_CURRENT_TIMESTAMP: // "CURRENT_TIMESTAMP"
      case symbol_kind::S_DEFAULT: // "DEFAULT"
      case symbol_kind::S_DEFERRABLE: // "DEFERRABLE"
      case symbol_kind::S_DEFERRED: // "DEFERRED"
      case symbol_kind::S_DELETE: // "DELETE"
      case symbol_kind::S_DESC: // "DESC"
      case symbol_kind::S_DISTINCT: // "DISTINCT"
      case symbol_kind::S_ELSE: // "ELSE"
      case symbol_kind::S_END: // "END"
      case symbol_kind::S_ESCAPE: // "ESCAPE"
      case symbol_kind::S_EXISTS: // "EXISTS"
      case symbol_kind::S_FAIL: // "FAIL"
      case symbol_kind::S_FALSE: // "FALSE"
      case symbol_kind::S_FILTER: // "FILTER"
      case symbol_kind::S_FOLLOWING: // "FOLLOWING"
      case symbol_kind::S_FOREIGN: // "FOREIGN"
      case symbol_kind::S_GENERATED: // "GENERATED"
      case symbol_kind::S_GLOB: // "GLOB"
      case symbol_kind::S_IF: // "IF"
      case symbol_kind::S_IGNORE: // "IGNORE"
      case symbol_kind::S_IMMEDIATE: // "IMMEDIATE"
      case symbol_kind::S_IN: // "IN"
      case symbol_kind::S_INDEX: // "INDEX"
      case symbol_kind::S_INITIALLY: // "INITIALLY"
      case symbol_kind::S_INSERT: // "INSERT"
      case symbol_kind::S_IS: // "IS"
      case symbol_kind::S_ISNULL: // "ISNULL"
      case symbol_kind::S_KEY: // "KEY"
      case symbol_kind::S_LIKE: // "LIKE"
      case symbol_kind::S_MATCH: // "MATCH"
      case symbol_kind::S_NO: // "NO"
      case symbol_kind::S_NOT: // "NOT"
      case symbol_kind::S_NOTNULL: // "NOTNULL"
      case symbol_kind::S_NULL: // "NULL"
      case symbol_kind::S_ON: // "ON"
      case symbol_kind::S_OR: // "OR"
      case symbol_kind::S_OVER: // "OVER"
      case symbol_kind::S_PARTITION: // "PARTITION"
      case symbol_kind::S_PRECEDING: // "PRECEDING"
      case symbol_kind::S_PRIMARY: // "PRIMARY"
      case symbol_kind::S_RAISE: // "RAISE"
      case symbol_kind::S_RANGE: // "RANGE"
      case symbol_kind::S_REFERENCES: // "REFERENCES"
      case symbol_kind::S_REGEXP: // "REGEXP"
      case symbol_kind::S_REPLACE: // "REPLACE"
      case symbol_kind::S_RESTRICT: // "RESTRICT"
      case symbol_kind::S_RETURNING: // "RETURNING"
      case symbol_kind::S_ROLLBACK: // "ROLLBACK"
      case symbol_kind::S_ROWID: // "ROWID"
      case symbol_kind::S_ROWS: // "ROWS"
      case symbol_kind::S_SELECT: // "SELECT"
      case symbol_kind::S_SET: // "SET"
      case symbol_kind::S_STORED: // "STORED"
      case symbol_kind::S_TABLE: // "TABLE"
      case symbol_kind::S_TEMP: // "TEMP"
      case symbol_kind::S_TEMPORARY: // "TEMPORARY"
      case symbol_kind::S_THEN: // "THEN"
      case symbol_kind::S_TRUE: // "TRUE"
      case symbol_kind::S_UNBOUNDED: // "UNBOUNDED"
      case symbol_kind::S_UNIQUE: // "UNIQUE"
      case symbol_kind::S_UPDATE: // "UPDATE"
      case symbol_kind::S_USING: // "USING"
      case symbol_kind::S_VIRTUAL: // "VIRTUAL"
      case symbol_kind::S_WHEN: // "WHEN"
      case symbol_kind::S_WHERE: // "WHERE"
      case symbol_kind::S_WITHOUT: // "WITHOUT"
      case symbol_kind::S_IDENTIFIER: // "identifier"
      case symbol_kind::S_NUMERIC: // "numeric"
      case symbol_kind::S_STRINGLITERAL: // "string literal"
      case symbol_kind::S_QUOTEDLITERAL: // "quoted literal"
      case symbol_kind::S_BLOBLITERAL: // "blob literal"
      case symbol_kind::S_BINDPARAMETER: // "bind parameter"
      case symbol_kind::S_literalvalue: // literalvalue
      case symbol_kind::S_id: // id
      case symbol_kind::S_allowed_keywords_as_identifier: // allowed_keywords_as_identifier
      case symbol_kind::S_tableid: // tableid
      case symbol_kind::S_columnid: // columnid
      case symbol_kind::S_signednumber: // signednumber
      case symbol_kind::S_signednumber_or_numeric: // signednumber_or_numeric
      case symbol_kind::S_typename_namelist: // typename_namelist
      case symbol_kind::S_type_name: // type_name
      case symbol_kind::S_unary_expr: // unary_expr
      case symbol_kind::S_binary_expr: // binary_expr
      case symbol_kind::S_like_expr: // like_expr
      case symbol_kind::S_exprlist_expr: // exprlist_expr
      case symbol_kind::S_function_expr: // function_expr
      case symbol_kind::S_isnull_expr: // isnull_expr
      case symbol_kind::S_between_expr: // between_expr
      case symbol_kind::S_in_expr: // in_expr
      case symbol_kind::S_whenthenlist_expr: // whenthenlist_expr
      case symbol_kind::S_case_expr: // case_expr
      case symbol_kind::S_raise_expr: // raise_expr
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_select_stmt: // select_stmt
      case symbol_kind::S_optional_sort_order: // optional_sort_order
      case symbol_kind::S_optional_where: // optional_where
      case symbol_kind::S_tableid_with_uninteresting_schema: // tableid_with_uninteresting_schema
      case symbol_kind::S_optional_exprlist_with_paren: // optional_exprlist_with_paren
      case symbol_kind::S_optional_conflictclause: // optional_conflictclause
      case symbol_kind::S_optional_typename: // optional_typename
      case symbol_kind::S_optional_storage_identifier: // optional_storage_identifier
      case symbol_kind::S_optional_constraintname: // optional_constraintname
      case symbol_kind::S_fk_clause_part: // fk_clause_part
      case symbol_kind::S_fk_clause_part_list: // fk_clause_part_list
      case symbol_kind::S_optional_fk_clause: // optional_fk_clause
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_columndef_list: // columndef_list
        value.copy< std::vector<ColumndefData> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_columndef: // columndef
        value.move< ColumndefData > (that.value);
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_withoutrowid: // optional_withoutrowid
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.move< bool > (that.value);
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
      case symbol_kind::S_tableconstraint: // tableconstraint
        value.move< sqlb::ConstraintPtr > (that.value);
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.move< sqlb::ConstraintVector > (that.value);
        break;

      case symbol_kind::S_createindex_stmt: // createindex_stmt
        value.move< sqlb::IndexPtr > (that.value);
        break;

      case symbol_kind::S_indexed_column: // indexed_column
        value.move< sqlb::IndexedColumn > (that.value);
        break;

      case symbol_kind::S_indexed_column_list: // indexed_column_list
        value.move< sqlb::IndexedColumnVector > (that.value);
        break;

      case symbol_kind::S_columnid_list: // columnid_list
      case symbol_kind::S_optional_columnid_with_paren_list: // optional_columnid_with_paren_list
        value.move< sqlb::StringVector > (that.value);
        break;

      case symbol_kind::S_createvirtualtable_stmt: // createvirtualtable_stmt
      case symbol_kind::S_createtable_stmt: // createtable_stmt
        value.move< sqlb::TablePtr > (that.value);
        break;

      case symbol_kind::S_ABORT: // "ABORT"
      case symbol_kind::S_ACTION: // "ACTION"
      case symbol_kind::S_ALWAYS: // "ALWAYS"
      case symbol_kind::S_AND: // "AND"
      case symbol_kind::S_AND_BETWEEN: // "AND BETWEEN"
      case symbol_kind::S_AS: // "AS"
      case symbol_kind::S_ASC: // "ASC"
      case symbol_kind::S_AUTOINCREMENT: // "AUTOINCREMENT"
      case symbol_kind::S_BETWEEN: // "BETWEEN"
      case symbol_kind::S_CASCADE: // "CASCADE"
      case symbol_kind::S_CASE: // "CASE"
      case symbol_kind::S_CAST: // "CAST"
      case symbol_kind::S_CHECK: // "CHECK"
      case symbol_kind::S_COLLATE: // "COLLATE"
      case symbol_kind::S_CONFLICT: // "CONFLICT"
      case symbol_kind::S_CONSTRAINT: // "CONSTRAINT"
      case symbol_kind::S_CREATE: // "CREATE"
      case symbol_kind::S_CURRENT_DATE: // "CURRENT_DATE"
      case symbol_kind::S_CURRENT_TIME: // "CURRENT_TIME"
      case symbol_kind::S_CURRENT_TIMESTAMP: // "CURRENT_TIMESTAMP"
      case symbol_kind::S_DEFAULT: // "DEFAULT"
      case symbol_kind::S_DEFERRABLE: // "DEFERRABLE"
      case symbol_kind::S_DEFERRED: // "DEFERRED"
      case symbol_kind::S_DELETE: // "DELETE"
      case symbol_kind::S_DESC: // "DESC"
      case symbol_kind::S_DISTINCT: // "DISTINCT"
      case symbol_kind::S_ELSE: // "ELSE"
      case symbol_kind::S_END: // "END"
      case symbol_kind::S_ESCAPE: // "ESCAPE"
      case symbol_kind::S_EXISTS: // "EXISTS"
      case symbol_kind::S_FAIL: // "FAIL"
      case symbol_kind::S_FALSE: // "FALSE"
      case symbol_kind::S_FILTER: // "FILTER"
      case symbol_kind::S_FOLLOWING: // "FOLLOWING"
      case symbol_kind::S_FOREIGN: // "FOREIGN"
      case symbol_kind::S_GENERATED: // "GENERATED"
      case symbol_kind::S_GLOB: // "GLOB"
      case symbol_kind::S_IF: // "IF"
      case symbol_kind::S_IGNORE: // "IGNORE"
      case symbol_kind::S_IMMEDIATE: // "IMMEDIATE"
      case symbol_kind::S_IN: // "IN"
      case symbol_kind::S_INDEX: // "INDEX"
      case symbol_kind::S_INITIALLY: // "INITIALLY"
      case symbol_kind::S_INSERT: // "INSERT"
      case symbol_kind::S_IS: // "IS"
      case symbol_kind::S_ISNULL: // "ISNULL"
      case symbol_kind::S_KEY: // "KEY"
      case symbol_kind::S_LIKE: // "LIKE"
      case symbol_kind::S_MATCH: // "MATCH"
      case symbol_kind::S_NO: // "NO"
      case symbol_kind::S_NOT: // "NOT"
      case symbol_kind::S_NOTNULL: // "NOTNULL"
      case symbol_kind::S_NULL: // "NULL"
      case symbol_kind::S_ON: // "ON"
      case symbol_kind::S_OR: // "OR"
      case symbol_kind::S_OVER: // "OVER"
      case symbol_kind::S_PARTITION: // "PARTITION"
      case symbol_kind::S_PRECEDING: // "PRECEDING"
      case symbol_kind::S_PRIMARY: // "PRIMARY"
      case symbol_kind::S_RAISE: // "RAISE"
      case symbol_kind::S_RANGE: // "RANGE"
      case symbol_kind::S_REFERENCES: // "REFERENCES"
      case symbol_kind::S_REGEXP: // "REGEXP"
      case symbol_kind::S_REPLACE: // "REPLACE"
      case symbol_kind::S_RESTRICT: // "RESTRICT"
      case symbol_kind::S_RETURNING: // "RETURNING"
      case symbol_kind::S_ROLLBACK: // "ROLLBACK"
      case symbol_kind::S_ROWID: // "ROWID"
      case symbol_kind::S_ROWS: // "ROWS"
      case symbol_kind::S_SELECT: // "SELECT"
      case symbol_kind::S_SET: // "SET"
      case symbol_kind::S_STORED: // "STORED"
      case symbol_kind::S_TABLE: // "TABLE"
      case symbol_kind::S_TEMP: // "TEMP"
      case symbol_kind::S_TEMPORARY: // "TEMPORARY"
      case symbol_kind::S_THEN: // "THEN"
      case symbol_kind::S_TRUE: // "TRUE"
      case symbol_kind::S_UNBOUNDED: // "UNBOUNDED"
      case symbol_kind::S_UNIQUE: // "UNIQUE"
      case symbol_kind::S_UPDATE: // "UPDATE"
      case symbol_kind::S_USING: // "USING"
      case symbol_kind::S_VIRTUAL: // "VIRTUAL"
      case symbol_kind::S_WHEN: // "WHEN"
      case symbol_kind::S_WHERE: // "WHERE"
      case symbol_kind::S_WITHOUT: // "WITHOUT"
      case symbol_kind::S_IDENTIFIER: // "identifier"
      case symbol_kind::S_NUMERIC: // "numeric"
      case symbol_kind::S_STRINGLITERAL: // "string literal"
      case symbol_kind::S_QUOTEDLITERAL: // "quoted literal"
      case symbol_kind::S_BLOBLITERAL: // "blob literal"
      case symbol_kind::S_BINDPARAMETER: // "bind parameter"
      case symbol_kind::S_literalvalue: // literalvalue
      case symbol_kind::S_id: // id
      case symbol_kind::S_allowed_keywords_as_identifier: // allowed_keywords_as_identifier
      case symbol_kind::S_tableid: // tableid
      case symbol_kind::S_columnid: // columnid
      case symbol_kind::S_signednumber: // signednumber
      case symbol_kind::S_signednumber_or_numeric: // signednumber_or_numeric
      case symbol_kind::S_typename_namelist: // typename_namelist
      case symbol_kind::S_type_name: // type_name
      case symbol_kind::S_unary_expr: // unary_expr
      case symbol_kind::S_binary_expr: // binary_expr
      case symbol_kind::S_like_expr: // like_expr
      case symbol_kind::S_exprlist_expr: // exprlist_expr
      case symbol_kind::S_function_expr: // function_expr
      case symbol_kind::S_isnull_expr: // isnull_expr
      case symbol_kind::S_between_expr: // between_expr
      case symbol_kind::S_in_expr: // in_expr
      case symbol_kind::S_whenthenlist_expr: // whenthenlist_expr
      case symbol_kind::S_case_expr: // case_expr
      case symbol_kind::S_raise_expr: // raise_expr
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_select_stmt: // select_stmt
      case symbol_kind::S_optional_sort_order: // optional_sort_order
      case symbol_kind::S_optional_where: // optional_where
      case symbol_kind::S_tableid_with_uninteresting_schema: // tableid_with_uninteresting_schema
      case symbol_kind::S_optional_exprlist_with_paren: // optional_exprlist_with_paren
      case symbol_kind::S_optional_conflictclause: // optional_conflictclause
      case symbol_kind::S_optional_typename: // optional_typename
      case symbol_kind::S_optional_storage_identifier: // optional_storage_identifier
      case symbol_kind::S_optional_constraintname: // optional_constraintname
      case symbol_kind::S_fk_clause_part: // fk_clause_part
      case symbol_kind::S_fk_clause_part_list: // fk_clause_part_list
      case symbol_kind::S_optional_fk_clause: // optional_fk_clause
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_columndef_list: // columndef_list
        value.move< std::vector<ColumndefData> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (yyscanner, drv));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_columndef: // columndef
        yylhs.value.emplace< ColumndefData > ();
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_withoutrowid: // optional_withoutrowid
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        yylhs.value.emplace< bool > ();
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
      case symbol_kind::S_tableconstraint: // tableconstraint
        yylhs.value.emplace< sqlb::ConstraintPtr > ();
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        yylhs.value.emplace< sqlb::ConstraintVector > ();
        break;

      case symbol_kind::S_createindex_stmt: // createindex_stmt
        yylhs.value.emplace< sqlb::IndexPtr > ();
        break;

      case symbol_kind::S_indexed_column: // indexed_column
        yylhs.value.emplace< sqlb::IndexedColumn > ();
        break;

      case symbol_kind::S_indexed_column_list: // indexed_column_list
        yylhs.value.emplace< sqlb::IndexedColumnVector > ();
        break;

      case symbol_kind::S_columnid_list: // columnid_list
      case symbol_kind::S_optional_columnid_with_paren_list: // optional_columnid_with_paren_list
        yylhs.value.emplace< sqlb::StringVector > ();
        break;

      case symbol_kind::S_createvirtualtable_stmt: // createvirtualtable_stmt
      case symbol_kind::S_createtable_stmt: // createtable_stmt
        yylhs.value.emplace< sqlb::TablePtr > ();
        break;

      case symbol_kind::S_ABORT: // "ABORT"
      case symbol_kind::S_ACTION: // "ACTION"
      case symbol_kind::S_ALWAYS: // "ALWAYS"
      case symbol_kind::S_AND: // "AND"
      case symbol_kind::S_AND_BETWEEN: // "AND BETWEEN"
      case symbol_kind::S_AS: // "AS"
      case symbol_kind::S_ASC: // "ASC"
      case symbol_kind::S_AUTOINCREMENT: // "AUTOINCREMENT"
      case symbol_kind::S_BETWEEN: // "BETWEEN"
      case symbol_kind::S_CASCADE: // "CASCADE"
      case symbol_kind::S_CASE: // "CASE"
      case symbol_kind::S_CAST: // "CAST"
      case symbol_kind::S_CHECK: // "CHECK"
      case symbol_kind::S_COLLATE: // "COLLATE"
      case symbol_kind::S_CONFLICT: // "CONFLICT"
      case symbol_kind::S_CONSTRAINT: // "CONSTRAINT"
      case symbol_kind::S_CREATE: // "CREATE"
      case symbol_kind::S_CURRENT_DATE: // "CURRENT_DATE"
      case symbol_kind::S_CURRENT_TIME: // "CURRENT_TIME"
      case symbol_kind::S_CURRENT_TIMESTAMP: // "CURRENT_TIMESTAMP"
      case symbol_kind::S_DEFAULT: // "DEFAULT"
      case symbol_kind::S_DEFERRABLE: // "DEFERRABLE"
      case symbol_kind::S_DEFERRED: // "DEFERRED"
      case symbol_kind::S_DELETE: // "DELETE"
      case symbol_kind::S_DESC: // "DESC"
      case symbol_kind::S_DISTINCT: // "DISTINCT"
      case symbol_kind::S_ELSE: // "ELSE"
      case symbol_kind::S_END: // "END"
      case symbol_kind::S_ESCAPE: // "ESCAPE"
      case symbol_kind::S_EXISTS: // "EXISTS"
      case symbol_kind::S_FAIL: // "FAIL"
      case symbol_kind::S_FALSE: // "FALSE"
      case symbol_kind::S_FILTER: // "FILTER"
      case symbol_kind::S_FOLLOWING: // "FOLLOWING"
      case symbol_kind::S_FOREIGN: // "FOREIGN"
      case symbol_kind::S_GENERATED: // "GENERATED"
      case symbol_kind::S_GLOB: // "GLOB"
      case symbol_kind::S_IF: // "IF"
      case symbol_kind::S_IGNORE: // "IGNORE"
      case symbol_kind::S_IMMEDIATE: // "IMMEDIATE"
      case symbol_kind::S_IN: // "IN"
      case symbol_kind::S_INDEX: // "INDEX"
      case symbol_kind::S_INITIALLY: // "INITIALLY"
      case symbol_kind::S_INSERT: // "INSERT"
      case symbol_kind::S_IS: // "IS"
      case symbol_kind::S_ISNULL: // "ISNULL"
      case symbol_kind::S_KEY: // "KEY"
      case symbol_kind::S_LIKE: // "LIKE"
      case symbol_kind::S_MATCH: // "MATCH"
      case symbol_kind::S_NO: // "NO"
      case symbol_kind::S_NOT: // "NOT"
      case symbol_kind::S_NOTNULL: // "NOTNULL"
      case symbol_kind::S_NULL: // "NULL"
      case symbol_kind::S_ON: // "ON"
      case symbol_kind::S_OR: // "OR"
      case symbol_kind::S_OVER: // "OVER"
      case symbol_kind::S_PARTITION: // "PARTITION"
      case symbol_kind::S_PRECEDING: // "PRECEDING"
      case symbol_kind::S_PRIMARY: // "PRIMARY"
      case symbol_kind::S_RAISE: // "RAISE"
      case symbol_kind::S_RANGE: // "RANGE"
      case symbol_kind::S_REFERENCES: // "REFERENCES"
      case symbol_kind::S_REGEXP: // "REGEXP"
      case symbol_kind::S_REPLACE: // "REPLACE"
      case symbol_kind::S_RESTRICT: // "RESTRICT"
      case symbol_kind::S_RETURNING: // "RETURNING"
      case symbol_kind::S_ROLLBACK: // "ROLLBACK"
      case symbol_kind::S_ROWID: // "ROWID"
      case symbol_kind::S_ROWS: // "ROWS"
      case symbol_kind::S_SELECT: // "SELECT"
      case symbol_kind::S_SET: // "SET"
      case symbol_kind::S_STORED: // "STORED"
      case symbol_kind::S_TABLE: // "TABLE"
      case symbol_kind::S_TEMP: // "TEMP"
      case symbol_kind::S_TEMPORARY: // "TEMPORARY"
      case symbol_kind::S_THEN: // "THEN"
      case symbol_kind::S_TRUE: // "TRUE"
      case symbol_kind::S_UNBOUNDED: // "UNBOUNDED"
      case symbol_kind::S_UNIQUE: // "UNIQUE"
      case symbol_kind::S_UPDATE: // "UPDATE"
      case symbol_kind::S_USING: // "USING"
      case symbol_kind::S_VIRTUAL: // "VIRTUAL"
      case symbol_kind::S_WHEN: // "WHEN"
      case symbol_kind::S_WHERE: // "WHERE"
      case symbol_kind::S_WITHOUT: // "WITHOUT"
      case symbol_kind::S_IDENTIFIER: // "identifier"
      case symbol_kind::S_NUMERIC: // "numeric"
      case symbol_kind::S_STRINGLITERAL: // "string literal"
      case symbol_kind::S_QUOTEDLITERAL: // "quoted literal"
      case symbol_kind::S_BLOBLITERAL: // "blob literal"
      case symbol_kind::S_BINDPARAMETER: // "bind parameter"
      case symbol_kind::S_literalvalue: // literalvalue
      case symbol_kind::S_id: // id
      case symbol_kind::S_allowed_keywords_as_identifier: // allowed_keywords_as_identifier
      case symbol_kind::S_tableid: // tableid
      case symbol_kind::S_columnid: // columnid
      case symbol_kind::S_signednumber: // signednumber
      case symbol_kind::S_signednumber_or_numeric: // signednumber_or_numeric
      case symbol_kind::S_typename_namelist: // typename_namelist
      case symbol_kind::S_type_name: // type_name
      case symbol_kind::S_unary_expr: // unary_expr
      case symbol_kind::S_binary_expr: // binary_expr
      case symbol_kind::S_like_expr: // like_expr
      case symbol_kind::S_exprlist_expr: // exprlist_expr
      case symbol_kind::S_function_expr: // function_expr
      case symbol_kind::S_isnull_expr: // isnull_expr
      case symbol_kind::S_between_expr: // between_expr
      case symbol_kind::S_in_expr: // in_expr
      case symbol_kind::S_whenthenlist_expr: // whenthenlist_expr
      case symbol_kind::S_case_expr: // case_expr
      case symbol_kind::S_raise_expr: // raise_expr
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_select_stmt: // select_stmt
      case symbol_kind::S_optional_sort_order: // optional_sort_order
      case symbol_kind::S_optional_where: // optional_where
      case symbol_kind::S_tableid_with_uninteresting_schema: // tableid_with_uninteresting_schema
      case symbol_kind::S_optional_exprlist_with_paren: // optional_exprlist_with_paren
      case symbol_kind::S_optional_conflictclause: // optional_conflictclause
      case symbol_kind::S_optional_typename: // optional_typename
      case symbol_kind::S_optional_storage_identifier: // optional_storage_identifier
      case symbol_kind::S_optional_constraintname: // optional_constraintname
      case symbol_kind::S_fk_clause_part: // fk_clause_part
      case symbol_kind::S_fk_clause_part_list: // fk_clause_part_list
      case symbol_kind::S_optional_fk_clause: // optional_fk_clause
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_columndef_list: // columndef_list
        yylhs.value.emplace< std::vector<ColumndefData> > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 4: // statement: createindex_stmt
#line 268 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::IndexPtr > (); }
#line 1444 "sqlite3_parser.cpp"
    break;

  case 5: // statement: createvirtualtable_stmt
#line 269 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::TablePtr > (); }
#line 1450 "sqlite3_parser.cpp"
    break;

  case 6: // statement: createtable_stmt
#line 270 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::TablePtr > (); }
#line 1456 "sqlite3_parser.cpp"
    break;

  case 7: // literalvalue: "numeric"
#line 278 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1462 "sqlite3_parser.cpp"
    break;

  case 8: // literalvalue: "string literal"
#line 279 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1468 "sqlite3_parser.cpp"
    break;

  case 9: // literalvalue: "blob literal"
#line 280 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1474 "sqlite3_parser.cpp"
    break;

  case 10: // literalvalue: "NULL"
#line 281 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1480 "sqlite3_parser.cpp"
    break;

  case 11: // literalvalue: "TRUE"
#line 282 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1486 "sqlite3_parser.cpp"
    break;

  case 12: // literalvalue: "FALSE"
#line 283 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1492 "sqlite3_parser.cpp"
    break;

  case 13: // literalvalue: "CURRENT_TIME"
#line 284 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1498 "sqlite3_parser.cpp"
    break;

  case 14: // literalvalue: "CURRENT_DATE"
#line 285 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1504 "sqlite3_parser.cpp"
    break;

  case 15: // literalvalue: "CURRENT_TIMESTAMP"
#line 286 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1510 "sqlite3_parser.cpp"
    break;

  case 16: // id: "identifier"
#line 290 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1516 "sqlite3_parser.cpp"
    break;

  case 17: // id: "quoted literal"
#line 291 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1522 "sqlite3_parser.cpp"
    break;

  case 18: // allowed_keywords_as_identifier: "ABORT"
#line 296 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1528 "sqlite3_parser.cpp"
    break;

  case 19: // allowed_keywords_as_identifier: "ACTION"
#line 297 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1534 "sqlite3_parser.cpp"
    break;

  case 20: // allowed_keywords_as_identifier: "ALWAYS"
#line 298 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1540 "sqlite3_parser.cpp"
    break;

  case 21: // allowed_keywords_as_identifier: "ASC"
#line 299 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1546 "sqlite3_parser.cpp"
    break;

  case 22: // allowed_keywords_as_identifier: "CASCADE"
#line 300 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1552 "sqlite3_parser.cpp"
    break;

  case 23: // allowed_keywords_as_identifier: "CAST"
#line 301 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1558 "sqlite3_parser.cpp"
    break;

  case 24: // allowed_keywords_as_identifier: "CONFLICT"
#line 302 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1564 "sqlite3_parser.cpp"
    break;

  case 25: // allowed_keywords_as_identifier: "DEFERRED"
#line 303 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1570 "sqlite3_parser.cpp"
    break;

  case 26: // allowed_keywords_as_identifier: "DESC"
#line 304 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1576 "sqlite3_parser.cpp"
    break;

  case 27: // allowed_keywords_as_identifier: "END"
#line 305 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1582 "sqlite3_parser.cpp"
    break;

  case 28: // allowed_keywords_as_identifier: "FAIL"
#line 306 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1588 "sqlite3_parser.cpp"
    break;

  case 29: // allowed_keywords_as_identifier: "FILTER"
#line 307 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1594 "sqlite3_parser.cpp"
    break;

  case 30: // allowed_keywords_as_identifier: "FOLLOWING"
#line 308 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1600 "sqlite3_parser.cpp"
    break;

  case 31: // allowed_keywords_as_identifier: "GENERATED"
#line 309 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1606 "sqlite3_parser.cpp"
    break;

  case 32: // allowed_keywords_as_identifier: "GLOB"
#line 310 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1612 "sqlite3_parser.cpp"
    break;

  case 33: // allowed_keywords_as_identifier: "KEY"
#line 311 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1618 "sqlite3_parser.cpp"
    break;

  case 34: // allowed_keywords_as_identifier: "LIKE"
#line 312 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1624 "sqlite3_parser.cpp"
    break;

  case 35: // allowed_keywords_as_identifier: "IGNORE"
#line 313 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1630 "sqlite3_parser.cpp"
    break;

  case 36: // allowed_keywords_as_identifier: "INITIALLY"
#line 314 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1636 "sqlite3_parser.cpp"
    break;

  case 37: // allowed_keywords_as_identifier: "IMMEDIATE"
#line 315 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1642 "sqlite3_parser.cpp"
    break;

  case 38: // allowed_keywords_as_identifier: "MATCH"
#line 316 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1648 "sqlite3_parser.cpp"
    break;

  case 39: // allowed_keywords_as_identifier: "NO"
#line 317 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1654 "sqlite3_parser.cpp"
    break;

  case 40: // allowed_keywords_as_identifier: "OVER"
#line 318 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1660 "sqlite3_parser.cpp"
    break;

  case 41: // allowed_keywords_as_identifier: "PARTITION"
#line 319 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1666 "sqlite3_parser.cpp"
    break;

  case 42: // allowed_keywords_as_identifier: "PRECEDING"
#line 320 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1672 "sqlite3_parser.cpp"
    break;

  case 43: // allowed_keywords_as_identifier: "RAISE"
#line 321 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1678 "sqlite3_parser.cpp"
    break;

  case 44: // allowed_keywords_as_identifier: "RANGE"
#line 322 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1684 "sqlite3_parser.cpp"
    break;

  case 45: // allowed_keywords_as_identifier: "REGEXP"
#line 323 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1690 "sqlite3_parser.cpp"
    break;

  case 46: // allowed_keywords_as_identifier: "REPLACE"
#line 324 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1696 "sqlite3_parser.cpp"
    break;

  case 47: // allowed_keywords_as_identifier: "RESTRICT"
#line 325 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1702 "sqlite3_parser.cpp"
    break;

  case 48: // allowed_keywords_as_identifier: "RETURNING"
#line 326 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1708 "sqlite3_parser.cpp"
    break;

  case 49: // allowed_keywords_as_identifier: "ROLLBACK"
#line 327 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1714 "sqlite3_parser.cpp"
    break;

  case 50: // allowed_keywords_as_identifier: "ROWID"
#line 328 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1720 "sqlite3_parser.cpp"
    break;

  case 51: // allowed_keywords_as_identifier: "ROWS"
#line 329 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1726 "sqlite3_parser.cpp"
    break;

  case 52: // allowed_keywords_as_identifier: "STORED"
#line 330 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1732 "sqlite3_parser.cpp"
    break;

  case 53: // allowed_keywords_as_identifier: "TEMPORARY"
#line 331 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1738 "sqlite3_parser.cpp"
    break;

  case 54: // allowed_keywords_as_identifier: "TEMP"
#line 332 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1744 "sqlite3_parser.cpp"
    break;

  case 55: // allowed_keywords_as_identifier: "UNBOUNDED"
#line 333 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1750 "sqlite3_parser.cpp"
    break;

  case 56: // allowed_keywords_as_identifier: "VIRTUAL"
#line 334 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1756 "sqlite3_parser.cpp"
    break;

  case 57: // allowed_keywords_as_identifier: "WITHOUT"
#line 335 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1762 "sqlite3_parser.cpp"
    break;

  case 58: // tableid: allowed_keywords_as_identifier
#line 339 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1768 "sqlite3_parser.cpp"
    break;

  case 59: // tableid: "CURRENT_TIME"
#line 340 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1774 "sqlite3_parser.cpp"
    break;

  case 60: // tableid: "CURRENT_DATE"
#line 341 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1780 "sqlite3_parser.cpp"
    break;

  case 61: // tableid: "CURRENT_TIMESTAMP"
#line 342 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1786 "sqlite3_parser.cpp"
    break;

  case 62: // tableid: id
#line 343 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1792 "sqlite3_parser.cpp"
    break;

  case 63: // tableid: "string literal"
#line 344 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1798 "sqlite3_parser.cpp"
    break;

  case 64: // columnid: allowed_keywords_as_identifier
#line 348 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1804 "sqlite3_parser.cpp"
    break;

  case 65: // columnid: "CURRENT_TIME"
#line 349 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1810 "sqlite3_parser.cpp"
    break;

  case 66: // columnid: "CURRENT_DATE"
#line 350 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1816 "sqlite3_parser.cpp"
    break;

  case 67: // columnid: "CURRENT_TIMESTAMP"
#line 351 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1822 "sqlite3_parser.cpp"
    break;

  case 68: // columnid: "IF"
#line 352 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1828 "sqlite3_parser.cpp"
    break;

  case 69: // columnid: id
#line 353 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1834 "sqlite3_parser.cpp"
    break;

  case 70: // columnid: "string literal"
#line 354 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1840 "sqlite3_parser.cpp"
    break;

  case 71: // signednumber: "+" "numeric"
#line 358 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1846 "sqlite3_parser.cpp"
    break;

  case 72: // signednumber: "-" "numeric"
#line 359 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1852 "sqlite3_parser.cpp"
    break;

  case 73: // signednumber_or_numeric: signednumber
#line 363 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1858 "sqlite3_parser.cpp"
    break;

  case 74: // signednumber_or_numeric: "numeric"
#line 364 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1864 "sqlite3_parser.cpp"
    break;

  case 75: // typename_namelist: tableid
#line 368 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1870 "sqlite3_parser.cpp"
    break;

  case 76: // typename_namelist: typename_namelist tableid
#line 369 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 1876 "sqlite3_parser.cpp"
    break;

  case 77: // type_name: typename_namelist
#line 373 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1882 "sqlite3_parser.cpp"
    break;

  case 78: // type_name: typename_namelist "(" signednumber_or_numeric ")"
#line 374 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1888 "sqlite3_parser.cpp"
    break;

  case 79: // type_name: typename_namelist "(" signednumber_or_numeric "," signednumber_or_numeric ")"
#line 375 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + "(" + yystack_[3].value.as < std::string > () + ", " + yystack_[1].value.as < std::string > () + ")"; }
#line 1894 "sqlite3_parser.cpp"
    break;

  case 80: // unary_expr: "-" expr
#line 379 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1900 "sqlite3_parser.cpp"
    break;

  case 81: // unary_expr: "+" expr
#line 380 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1906 "sqlite3_parser.cpp"
    break;

  case 82: // unary_expr: "~" expr
#line 381 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "~" + yystack_[0].value.as < std::string > (); }
#line 1912 "sqlite3_parser.cpp"
    break;

  case 83: // unary_expr: "NOT" expr
#line 382 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "NOT " + yystack_[0].value.as < std::string > (); }
#line 1918 "sqlite3_parser.cpp"
    break;

  case 84: // binary_expr: expr "||" expr
#line 386 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " || " + yystack_[0].value.as < std::string > (); }
#line 1924 "sqlite3_parser.cpp"
    break;

  case 85: // binary_expr: expr "*" expr
#line 387 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " * " + yystack_[0].value.as < std::string > (); }
#line 1930 "sqlite3_parser.cpp"
    break;

  case 86: // binary_expr: expr "/" expr
#line 388 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " / " + yystack_[0].value.as < std::string > (); }
#line 1936 "sqlite3_parser.cpp"
    break;

  case 87: // binary_expr: expr "%" expr
#line 389 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " % " + yystack_[0].value.as < std::string > (); }
#line 1942 "sqlite3_parser.cpp"
    break;

  case 88: // binary_expr: expr "+" expr
#line 390 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " + " + yystack_[0].value.as < std::string > (); }
#line 1948 "sqlite3_parser.cpp"
    break;

  case 89: // binary_expr: expr "-" expr
#line 391 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " - " + yystack_[0].value.as < std::string > (); }
#line 1954 "sqlite3_parser.cpp"
    break;

  case 90: // binary_expr: expr "<<" expr
#line 392 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " << " + yystack_[0].value.as < std::string > (); }
#line 1960 "sqlite3_parser.cpp"
    break;

  case 91: // binary_expr: expr ">>" expr
#line 393 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >> " + yystack_[0].value.as < std::string > (); }
#line 1966 "sqlite3_parser.cpp"
    break;

  case 92: // binary_expr: expr "&" expr
#line 394 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " & " + yystack_[0].value.as < std::string > (); }
#line 1972 "sqlite3_parser.cpp"
    break;

  case 93: // binary_expr: expr "|" expr
#line 395 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " | " + yystack_[0].value.as < std::string > (); }
#line 1978 "sqlite3_parser.cpp"
    break;

  case 94: // binary_expr: expr "<" expr
#line 396 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " < " + yystack_[0].value.as < std::string > (); }
#line 1984 "sqlite3_parser.cpp"
    break;

  case 95: // binary_expr: expr "<=" expr
#line 397 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <= " + yystack_[0].value.as < std::string > (); }
#line 1990 "sqlite3_parser.cpp"
    break;

  case 96: // binary_expr: expr ">" expr
#line 398 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " > " + yystack_[0].value.as < std::string > (); }
#line 1996 "sqlite3_parser.cpp"
    break;

  case 97: // binary_expr: expr ">=" expr
#line 399 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >= " + yystack_[0].value.as < std::string > (); }
#line 2002 "sqlite3_parser.cpp"
    break;

  case 98: // binary_expr: expr "=" expr
#line 400 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " = " + yystack_[0].value.as < std::string > (); }
#line 2008 "sqlite3_parser.cpp"
    break;

  case 99: // binary_expr: expr "==" expr
#line 401 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " == " + yystack_[0].value.as < std::string > (); }
#line 2014 "sqlite3_parser.cpp"
    break;

  case 100: // binary_expr: expr "!=" expr
#line 402 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " != " + yystack_[0].value.as < std::string > (); }
#line 2020 "sqlite3_parser.cpp"
    break;

  case 101: // binary_expr: expr "<>" expr
#line 403 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <> " + yystack_[0].value.as < std::string > (); }
#line 2026 "sqlite3_parser.cpp"
    break;

  case 102: // binary_expr: expr "IS" expr
#line 404 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IS " + yystack_[0].value.as < std::string > (); }
#line 2032 "sqlite3_parser.cpp"
    break;

  case 103: // binary_expr: expr "AND" expr
#line 405 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2038 "sqlite3_parser.cpp"
    break;

  case 104: // binary_expr: expr "OR" expr
#line 406 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " OR " + yystack_[0].value.as < std::string > (); }
#line 2044 "sqlite3_parser.cpp"
    break;

  case 105: // like_expr: expr "LIKE" expr
#line 410 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " LIKE " + yystack_[0].value.as < std::string > (); }
#line 2050 "sqlite3_parser.cpp"
    break;

  case 106: // like_expr: expr "GLOB" expr
#line 411 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " GLOB " + yystack_[0].value.as < std::string > (); }
#line 2056 "sqlite3_parser.cpp"
    break;

  case 107: // like_expr: expr "MATCH" expr
#line 412 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " MATCH " + yystack_[0].value.as < std::string > (); }
#line 2062 "sqlite3_parser.cpp"
    break;

  case 108: // like_expr: expr "REGEXP" expr
#line 413 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " REGEXP " + yystack_[0].value.as < std::string > (); }
#line 2068 "sqlite3_parser.cpp"
    break;

  case 109: // like_expr: expr "NOT" "LIKE" expr
#line 414 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT LIKE " + yystack_[0].value.as < std::string > (); }
#line 2074 "sqlite3_parser.cpp"
    break;

  case 110: // like_expr: expr "NOT" "GLOB" expr
#line 415 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT GLOB " + yystack_[0].value.as < std::string > (); }
#line 2080 "sqlite3_parser.cpp"
    break;

  case 111: // like_expr: expr "NOT" "MATCH" expr
#line 416 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT MATCH " + yystack_[0].value.as < std::string > (); }
#line 2086 "sqlite3_parser.cpp"
    break;

  case 112: // like_expr: expr "NOT" "REGEXP" expr
#line 417 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT REGEXP " + yystack_[0].value.as < std::string > (); }
#line 2092 "sqlite3_parser.cpp"
    break;

  case 113: // like_expr: expr "LIKE" expr "ESCAPE" expr
#line 418 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " LIKE " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2098 "sqlite3_parser.cpp"
    break;

  case 114: // like_expr: expr "GLOB" expr "ESCAPE" expr
#line 419 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " GLOB " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2104 "sqlite3_parser.cpp"
    break;

  case 115: // like_expr: expr "MATCH" expr "ESCAPE" expr
#line 420 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " MATCH " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2110 "sqlite3_parser.cpp"
    break;

  case 116: // like_expr: expr "REGEXP" expr "ESCAPE" expr
#line 421 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " REGEXP " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2116 "sqlite3_parser.cpp"
    break;

  case 117: // like_expr: expr "NOT" "LIKE" expr "ESCAPE" expr
#line 422 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT LIKE " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2122 "sqlite3_parser.cpp"
    break;

  case 118: // like_expr: expr "NOT" "GLOB" expr "ESCAPE" expr
#line 423 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT GLOB " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2128 "sqlite3_parser.cpp"
    break;

  case 119: // like_expr: expr "NOT" "MATCH" expr "ESCAPE" expr
#line 424 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT MATCH " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2134 "sqlite3_parser.cpp"
    break;

  case 120: // like_expr: expr "NOT" "REGEXP" expr "ESCAPE" expr
#line 425 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT REGEXP " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2140 "sqlite3_parser.cpp"
    break;

  case 121: // exprlist_expr: expr
#line 429 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2146 "sqlite3_parser.cpp"
    break;

  case 122: // exprlist_expr: exprlist_expr "," expr
#line 430 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + ", " + yystack_[0].value.as < std::string > (); }
#line 2152 "sqlite3_parser.cpp"
    break;

  case 123: // function_expr: id "(" exprlist_expr ")"
#line 434 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2158 "sqlite3_parser.cpp"
    break;

  case 124: // function_expr: id "(" "DISTINCT" exprlist_expr ")"
#line 435 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + "(DISTINCT " + yystack_[1].value.as < std::string > () + ")"; }
#line 2164 "sqlite3_parser.cpp"
    break;

  case 125: // function_expr: id "(" ")"
#line 436 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + "()"; }
#line 2170 "sqlite3_parser.cpp"
    break;

  case 126: // function_expr: id "(" "*" ")"
#line 437 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(*)"; }
#line 2176 "sqlite3_parser.cpp"
    break;

  case 127: // isnull_expr: expr "ISNULL"
#line 441 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " ISNULL"; }
#line 2182 "sqlite3_parser.cpp"
    break;

  case 128: // isnull_expr: expr "NOTNULL"
#line 442 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " NOTNULL"; }
#line 2188 "sqlite3_parser.cpp"
    break;

  case 129: // isnull_expr: expr "NOT" "NULL"
#line 443 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " NOT NULL"; }
#line 2194 "sqlite3_parser.cpp"
    break;

  case 130: // between_expr: expr "BETWEEN" expr "AND BETWEEN" expr
#line 447 "sqlite3_parser.yy"
                                                                { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2200 "sqlite3_parser.cpp"
    break;

  case 131: // between_expr: expr "NOT" "BETWEEN" expr "AND BETWEEN" expr
#line 448 "sqlite3_parser.yy"
                                                                { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2206 "sqlite3_parser.cpp"
    break;

  case 132: // in_expr: expr "IN" "(" ")"
#line 452 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " IN ()"; }
#line 2212 "sqlite3_parser.cpp"
    break;

  case 133: // in_expr: expr "IN" "(" select_stmt ")"
#line 453 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2218 "sqlite3_parser.cpp"
    break;

  case 134: // in_expr: expr "IN" "(" exprlist_expr ")"
#line 454 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2224 "sqlite3_parser.cpp"
    break;

  case 135: // in_expr: expr "IN" id "." tableid
#line 455 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2230 "sqlite3_parser.cpp"
    break;

  case 136: // in_expr: expr "IN" tableid
#line 456 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2236 "sqlite3_parser.cpp"
    break;

  case 137: // in_expr: expr "IN" id "." id "(" ")"
#line 457 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 2242 "sqlite3_parser.cpp"
    break;

  case 138: // in_expr: expr "IN" id "." id "(" exprlist_expr ")"
#line 458 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2248 "sqlite3_parser.cpp"
    break;

  case 139: // in_expr: expr "IN" id "(" exprlist_expr ")"
#line 459 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2254 "sqlite3_parser.cpp"
    break;

  case 140: // in_expr: expr "NOT" "IN" "(" ")"
#line 460 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " NOT IN ()"; }
#line 2260 "sqlite3_parser.cpp"
    break;

  case 141: // in_expr: expr "NOT" "IN" "(" select_stmt ")"
#line 461 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2266 "sqlite3_parser.cpp"
    break;

  case 142: // in_expr: expr "NOT" "IN" "(" exprlist_expr ")"
#line 462 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2272 "sqlite3_parser.cpp"
    break;

  case 143: // in_expr: expr "NOT" "IN" id "." tableid
#line 463 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2278 "sqlite3_parser.cpp"
    break;

  case 144: // in_expr: expr "NOT" "IN" tableid
#line 464 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2284 "sqlite3_parser.cpp"
    break;

  case 145: // in_expr: expr "NOT" "IN" id "." id "(" ")"
#line 465 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 2290 "sqlite3_parser.cpp"
    break;

  case 146: // in_expr: expr "NOT" "IN" id "." id "(" exprlist_expr ")"
#line 466 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[8].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2296 "sqlite3_parser.cpp"
    break;

  case 147: // in_expr: expr "NOT" "IN" id "(" exprlist_expr ")"
#line 467 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " NOT IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2302 "sqlite3_parser.cpp"
    break;

  case 148: // whenthenlist_expr: "WHEN" expr "THEN" expr
#line 471 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "WHEN " + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 2308 "sqlite3_parser.cpp"
    break;

  case 149: // whenthenlist_expr: whenthenlist_expr "WHEN" expr "THEN" expr
#line 472 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " WHEN" + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 2314 "sqlite3_parser.cpp"
    break;

  case 150: // case_expr: "CASE" expr whenthenlist_expr "ELSE" expr "END"
#line 476 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2320 "sqlite3_parser.cpp"
    break;

  case 151: // case_expr: "CASE" expr whenthenlist_expr "END"
#line 477 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " END"; }
#line 2326 "sqlite3_parser.cpp"
    break;

  case 152: // case_expr: "CASE" whenthenlist_expr "ELSE" expr "END"
#line 478 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2332 "sqlite3_parser.cpp"
    break;

  case 153: // case_expr: "CASE" whenthenlist_expr "END"
#line 479 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2338 "sqlite3_parser.cpp"
    break;

  case 154: // raise_expr: "RAISE" "(" "IGNORE" ")"
#line 483 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(IGNORE)"; }
#line 2344 "sqlite3_parser.cpp"
    break;

  case 155: // raise_expr: "RAISE" "(" "ROLLBACK" "," "string literal" ")"
#line 484 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(ROLLBACK, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2350 "sqlite3_parser.cpp"
    break;

  case 156: // raise_expr: "RAISE" "(" "ABORT" "," "string literal" ")"
#line 485 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(ABORT, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2356 "sqlite3_parser.cpp"
    break;

  case 157: // raise_expr: "RAISE" "(" "FAIL" "," "string literal" ")"
#line 486 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(FAIL, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2362 "sqlite3_parser.cpp"
    break;

  case 158: // expr: literalvalue
#line 490 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2368 "sqlite3_parser.cpp"
    break;

  case 159: // expr: allowed_keywords_as_identifier
#line 491 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2374 "sqlite3_parser.cpp"
    break;

  case 160: // expr: "bind parameter"
#line 492 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2380 "sqlite3_parser.cpp"
    break;

  case 161: // expr: id "." id "." id
#line 493 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2386 "sqlite3_parser.cpp"
    break;

  case 162: // expr: id "." id
#line 494 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2392 "sqlite3_parser.cpp"
    break;

  case 163: // expr: id
#line 495 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2398 "sqlite3_parser.cpp"
    break;

  case 164: // expr: unary_expr
#line 496 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2404 "sqlite3_parser.cpp"
    break;

  case 165: // expr: binary_expr
#line 497 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2410 "sqlite3_parser.cpp"
    break;

  case 166: // expr: function_expr
#line 498 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2416 "sqlite3_parser.cpp"
    break;

  case 167: // expr: "(" exprlist_expr ")"
#line 499 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2422 "sqlite3_parser.cpp"
    break;

  case 168: // expr: "CAST" "(" expr "AS" type_name ")"
#line 500 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "CAST(" + yystack_[3].value.as < std::string > () + " AS " + yystack_[1].value.as < std::string > () + ")"; }
#line 2428 "sqlite3_parser.cpp"
    break;

  case 169: // expr: expr "COLLATE" id
#line 501 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " COLLATE " + yystack_[0].value.as < std::string > (); }
#line 2434 "sqlite3_parser.cpp"
    break;

  case 170: // expr: like_expr
#line 502 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2440 "sqlite3_parser.cpp"
    break;

  case 171: // expr: isnull_expr
#line 503 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2446 "sqlite3_parser.cpp"
    break;

  case 172: // expr: between_expr
#line 504 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2452 "sqlite3_parser.cpp"
    break;

  case 173: // expr: in_expr
#line 505 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2458 "sqlite3_parser.cpp"
    break;

  case 174: // expr: case_expr
#line 506 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2464 "sqlite3_parser.cpp"
    break;

  case 175: // expr: raise_expr
#line 507 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2470 "sqlite3_parser.cpp"
    break;

  case 176: // select_stmt: "SELECT"
#line 516 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "SELECT"; }
#line 2476 "sqlite3_parser.cpp"
    break;

  case 177: // optional_if_not_exists: %empty
#line 524 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = false; }
#line 2482 "sqlite3_parser.cpp"
    break;

  case 178: // optional_if_not_exists: "IF" "NOT" "EXISTS"
#line 525 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = true; }
#line 2488 "sqlite3_parser.cpp"
    break;

  case 179: // optional_sort_order: %empty
#line 529 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = ""; }
#line 2494 "sqlite3_parser.cpp"
    break;

  case 180: // optional_sort_order: "ASC"
#line 530 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "ASC"; }
#line 2500 "sqlite3_parser.cpp"
    break;

  case 181: // optional_sort_order: "DESC"
#line 531 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "DESC"; }
#line 2506 "sqlite3_parser.cpp"
    break;

  case 182: // optional_unique: %empty
#line 539 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = false; }
#line 2512 "sqlite3_parser.cpp"
    break;

  case 183: // optional_unique: "UNIQUE"
#line 540 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = true; }
#line 2518 "sqlite3_parser.cpp"
    break;

  case 184: // optional_where: %empty
#line 544 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = ""; }
#line 2524 "sqlite3_parser.cpp"
    break;

  case 185: // optional_where: "WHERE" expr
#line 545 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2530 "sqlite3_parser.cpp"
    break;

  case 186: // tableid_with_uninteresting_schema: id "." tableid
#line 549 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2536 "sqlite3_parser.cpp"
    break;

  case 187: // tableid_with_uninteresting_schema: "TEMP" "." tableid
#line 550 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2542 "sqlite3_parser.cpp"
    break;

  case 188: // tableid_with_uninteresting_schema: tableid
#line 551 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2548 "sqlite3_parser.cpp"
    break;

  case 189: // indexed_column: expr optional_sort_order
#line 555 "sqlite3_parser.yy"
                                                {
							// If the expression is only one column name and nothing else, treat it as a column name; otherwise as an expression.
							char quote = getIdentifierQuoteChar();
							if((quote == '[' && std::count(yystack_[1].value.as < std::string > ().begin(), yystack_[1].value.as < std::string > ().end(), quote) == 1 && yystack_[1].value.as < std::string > ().front() == '[' && yystack_[1].value.as < std::string > ().back() == ']') ||
								(quote != '[' && std::count(yystack_[1].value.as < std::string > ().begin(), yystack_[1].value.as < std::string > ().end(), quote) == 2 && yystack_[1].value.as < std::string > ().front() == quote && yystack_[1].value.as < std::string > ().back() == quote))
							{
								yylhs.value.as < sqlb::IndexedColumn > () = sqlb::IndexedColumn(unquote_text(yystack_[1].value.as < std::string > (), quote), false, yystack_[0].value.as < std::string > ());
							} else {
								yylhs.value.as < sqlb::IndexedColumn > () = sqlb::IndexedColumn(yystack_[1].value.as < std::string > (), true, yystack_[0].value.as < std::string > ());
							}
						}
#line 2564 "sqlite3_parser.cpp"
    break;

  case 190: // indexed_column_list: indexed_column
#line 569 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::IndexedColumnVector > () = sqlb::IndexedColumnVector(1, yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2570 "sqlite3_parser.cpp"
    break;

  case 191: // indexed_column_list: indexed_column_list "," indexed_column
#line 570 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::IndexedColumnVector > () = yystack_[2].value.as < sqlb::IndexedColumnVector > (); yylhs.value.as < sqlb::IndexedColumnVector > ().push_back(yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2576 "sqlite3_parser.cpp"
    break;

  case 192: // createindex_stmt: "CREATE" optional_unique "INDEX" optional_if_not_exists tableid_with_uninteresting_schema "ON" tableid "(" indexed_column_list ")" optional_where
#line 574 "sqlite3_parser.yy"
                                                                                                                                                                {
													yylhs.value.as < sqlb::IndexPtr > () = std::make_shared<sqlb::Index>(yystack_[6].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setTable(yystack_[4].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setUnique(yystack_[9].value.as < bool > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setWhereExpr(yystack_[0].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->fields = yystack_[2].value.as < sqlb::IndexedColumnVector > ();
													yylhs.value.as < sqlb::IndexPtr > ()->setFullyParsed(true);
												}
#line 2589 "sqlite3_parser.cpp"
    break;

  case 193: // optional_exprlist_with_paren: %empty
#line 589 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = {}; }
#line 2595 "sqlite3_parser.cpp"
    break;

  case 194: // optional_exprlist_with_paren: "(" ")"
#line 590 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = {}; }
#line 2601 "sqlite3_parser.cpp"
    break;

  case 195: // optional_exprlist_with_paren: "(" exprlist_expr ")"
#line 591 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 2607 "sqlite3_parser.cpp"
    break;

  case 196: // createvirtualtable_stmt: "CREATE" "VIRTUAL" "TABLE" optional_if_not_exists tableid_with_uninteresting_schema "USING" id optional_exprlist_with_paren
#line 595 "sqlite3_parser.yy"
                                                                                                                                {
													yylhs.value.as < sqlb::TablePtr > () = std::make_shared<sqlb::Table>(yystack_[3].value.as < std::string > ());
													yylhs.value.as < sqlb::TablePtr > ()->setVirtualUsing(yystack_[1].value.as < std::string > ());
													yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
												}
#line 2617 "sqlite3_parser.cpp"
    break;

  case 197: // optional_temporary: %empty
#line 607 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = false; }
#line 2623 "sqlite3_parser.cpp"
    break;

  case 198: // optional_temporary: "TEMP"
#line 608 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2629 "sqlite3_parser.cpp"
    break;

  case 199: // optional_temporary: "TEMPORARY"
#line 609 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2635 "sqlite3_parser.cpp"
    break;

  case 200: // optional_withoutrowid: %empty
#line 613 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = false; }
#line 2641 "sqlite3_parser.cpp"
    break;

  case 201: // optional_withoutrowid: "WITHOUT" "ROWID"
#line 614 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2647 "sqlite3_parser.cpp"
    break;

  case 202: // optional_conflictclause: %empty
#line 618 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = ""; }
#line 2653 "sqlite3_parser.cpp"
    break;

  case 203: // optional_conflictclause: "ON" "CONFLICT" "ROLLBACK"
#line 619 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2659 "sqlite3_parser.cpp"
    break;

  case 204: // optional_conflictclause: "ON" "CONFLICT" "ABORT"
#line 620 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2665 "sqlite3_parser.cpp"
    break;

  case 205: // optional_conflictclause: "ON" "CONFLICT" "FAIL"
#line 621 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2671 "sqlite3_parser.cpp"
    break;

  case 206: // optional_conflictclause: "ON" "CONFLICT" "IGNORE"
#line 622 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2677 "sqlite3_parser.cpp"
    break;

  case 207: // optional_conflictclause: "ON" "CONFLICT" "REPLACE"
#line 623 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2683 "sqlite3_parser.cpp"
    break;

  case 208: // optional_typename: %empty
#line 627 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = ""; }
#line 2689 "sqlite3_parser.cpp"
    break;

  case 209: // optional_typename: type_name
#line 628 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2695 "sqlite3_parser.cpp"
    break;

  case 210: // optional_storage_identifier: %empty
#line 632 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "VIRTUAL"; }
#line 2701 "sqlite3_parser.cpp"
    break;

  case 211: // optional_storage_identifier: "STORED"
#line 633 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "STORED"; }
#line 2707 "sqlite3_parser.cpp"
    break;

  case 212: // optional_storage_identifier: "VIRTUAL"
#line 634 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "VIRTUAL"; }
#line 2713 "sqlite3_parser.cpp"
    break;

  case 213: // optional_always_generated: %empty
#line 638 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = false; }
#line 2719 "sqlite3_parser.cpp"
    break;

  case 214: // optional_always_generated: "GENERATED" "ALWAYS"
#line 639 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2725 "sqlite3_parser.cpp"
    break;

  case 215: // columnconstraint: optional_constraintname "PRIMARY" "KEY" optional_sort_order optional_conflictclause
#line 643 "sqlite3_parser.yy"
                                                                                        {
												auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>(sqlb::IndexedColumnVector{sqlb::IndexedColumn("", false, yystack_[1].value.as < std::string > ())});
												pk->setName(yystack_[4].value.as < std::string > ());
												pk->setConflictAction(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > () = pk;
											}
#line 2736 "sqlite3_parser.cpp"
    break;

  case 216: // columnconstraint: optional_constraintname "PRIMARY" "KEY" optional_sort_order optional_conflictclause "AUTOINCREMENT"
#line 649 "sqlite3_parser.yy"
                                                                                                        {
												auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>(sqlb::IndexedColumnVector{sqlb::IndexedColumn("", false, yystack_[2].value.as < std::string > ())});
												pk->setName(yystack_[5].value.as < std::string > ());
												pk->setConflictAction(yystack_[1].value.as < std::string > ());
												pk->setAutoIncrement(true);
												yylhs.value.as < sqlb::ConstraintPtr > () = pk;
											}
#line 2748 "sqlite3_parser.cpp"
    break;

  case 217: // columnconstraint: optional_constraintname "NOT" "NULL" optional_conflictclause
#line 656 "sqlite3_parser.yy"
                                                                                        {
												auto nn = std::make_shared<sqlb::NotNullConstraint>();
												nn->setName(yystack_[3].value.as < std::string > ());
												nn->setConflictAction(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > () = nn;
											}
#line 2759 "sqlite3_parser.cpp"
    break;

  case 218: // columnconstraint: optional_constraintname "NULL"
#line 662 "sqlite3_parser.yy"
                                                                                        {
											}
#line 2766 "sqlite3_parser.cpp"
    break;

  case 219: // columnconstraint: optional_constraintname "UNIQUE" optional_conflictclause
#line 664 "sqlite3_parser.yy"
                                                                                        {
												auto u = std::make_shared<sqlb::UniqueConstraint>();
												u->setName(yystack_[2].value.as < std::string > ());
												u->setConflictAction(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > () = u;
											}
#line 2777 "sqlite3_parser.cpp"
    break;

  case 220: // columnconstraint: optional_constraintname "CHECK" "(" expr ")"
#line 670 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::CheckConstraint>(yystack_[1].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[4].value.as < std::string > ());
											}
#line 2786 "sqlite3_parser.cpp"
    break;

  case 221: // columnconstraint: optional_constraintname "DEFAULT" signednumber
#line 674 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2795 "sqlite3_parser.cpp"
    break;

  case 222: // columnconstraint: optional_constraintname "DEFAULT" literalvalue
#line 678 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2804 "sqlite3_parser.cpp"
    break;

  case 223: // columnconstraint: optional_constraintname "DEFAULT" id
#line 682 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2813 "sqlite3_parser.cpp"
    break;

  case 224: // columnconstraint: optional_constraintname "DEFAULT" allowed_keywords_as_identifier
#line 686 "sqlite3_parser.yy"
                                                                                        {	// We must allow the same keywords as unquoted default values as in the columnid context.
												// But we do not use columnid here in order to avoid reduce/reduce conflicts.
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2823 "sqlite3_parser.cpp"
    break;

  case 225: // columnconstraint: optional_constraintname "DEFAULT" "IF"
#line 691 "sqlite3_parser.yy"
                                                                                        {	// Same as above.
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2832 "sqlite3_parser.cpp"
    break;

  case 226: // columnconstraint: optional_constraintname "DEFAULT" "(" expr ")"
#line 695 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>("(" + yystack_[1].value.as < std::string > () + ")");
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[4].value.as < std::string > ());
											}
#line 2841 "sqlite3_parser.cpp"
    break;

  case 227: // columnconstraint: optional_constraintname "COLLATE" id
#line 699 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::CollateConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2850 "sqlite3_parser.cpp"
    break;

  case 228: // columnconstraint: optional_constraintname "REFERENCES" tableid optional_columnid_with_paren_list optional_fk_clause
#line 703 "sqlite3_parser.yy"
                                                                                                                {	// TODO Solve shift/reduce conflict. It is not super important though as shifting seems to be right here.
												auto fk = std::make_shared<sqlb::ForeignKeyClause>();
												fk->setName(yystack_[4].value.as < std::string > ());
												fk->setTable(yystack_[2].value.as < std::string > ());
												fk->setColumns(yystack_[1].value.as < sqlb::StringVector > ());
												fk->setConstraint(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > () = fk;
											}
#line 2863 "sqlite3_parser.cpp"
    break;

  case 229: // columnconstraint: optional_constraintname optional_always_generated "AS" "(" expr ")" optional_storage_identifier
#line 711 "sqlite3_parser.yy"
                                                                                                        {		// TODO Solve shift/reduce conflict.
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::GeneratedColumnConstraint>(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[6].value.as < std::string > ());
											}
#line 2872 "sqlite3_parser.cpp"
    break;

  case 230: // columnconstraint_list: columnconstraint
#line 718 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintVector > () = { yystack_[0].value.as < sqlb::ConstraintPtr > () }; }
#line 2878 "sqlite3_parser.cpp"
    break;

  case 231: // columnconstraint_list: columnconstraint_list columnconstraint
#line 719 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintVector > () = yystack_[1].value.as < sqlb::ConstraintVector > (); yylhs.value.as < sqlb::ConstraintVector > ().push_back(yystack_[0].value.as < sqlb::ConstraintPtr > ()); }
#line 2884 "sqlite3_parser.cpp"
    break;

  case 232: // columndef: columnid optional_typename columnconstraint_list
#line 723 "sqlite3_parser.yy"
                                                                {
								sqlb::Field f(yystack_[2].value.as < std::string > (), yystack_[1].value.as < std::string > ());
								sqlb::ConstraintVector table_constraints{};
								for(const auto& c : yystack_[0].value.as < sqlb::ConstraintVector > ())
								{
									if(!c)
										continue;

									switch(c->type())
									{
									case sqlb::Constraint::PrimaryKeyConstraintType:
									case sqlb::Constraint::ForeignKeyConstraintType:
									{
										// Primary key and foreign key constraints are converted to table constraints
										// because we cannot store them as column constraints at the moment.
										if(c->columnList().empty())
											c->setColumnList({yystack_[2].value.as < std::string > ()});
										else
											c->replaceInColumnList("", yystack_[2].value.as < std::string > ());
										table_constraints.push_back(c);
										break;
									}
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
									default:
										break;
									}
								}

								yylhs.value.as < ColumndefData > () = std::make_tuple(f, table_constraints);
							}
#line 2950 "sqlite3_parser.cpp"
    break;

  case 233: // columndef: columnid optional_typename
#line 784 "sqlite3_parser.yy"
                                                        { yylhs.value.as < ColumndefData > () = std::make_tuple(sqlb::Field(yystack_[1].value.as < std::string > (), yystack_[0].value.as < std::string > ()), sqlb::ConstraintVector{}); }
#line 2956 "sqlite3_parser.cpp"
    break;

  case 234: // columndef_list: columndef
#line 788 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::vector<ColumndefData> > () = {yystack_[0].value.as < ColumndefData > ()}; }
#line 2962 "sqlite3_parser.cpp"
    break;

  case 235: // columndef_list: columndef_list "," columndef
#line 789 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::vector<ColumndefData> > () = yystack_[2].value.as < std::vector<ColumndefData> > (); yylhs.value.as < std::vector<ColumndefData> > ().push_back(yystack_[0].value.as < ColumndefData > ()); }
#line 2968 "sqlite3_parser.cpp"
    break;

  case 236: // optional_constraintname: %empty
#line 793 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = ""; }
#line 2974 "sqlite3_parser.cpp"
    break;

  case 237: // optional_constraintname: "CONSTRAINT" id
#line 794 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2980 "sqlite3_parser.cpp"
    break;

  case 238: // optional_constraintname: "CONSTRAINT" "string literal"
#line 795 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2986 "sqlite3_parser.cpp"
    break;

  case 239: // columnid_list: columnid
#line 799 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = sqlb::StringVector(1, yystack_[0].value.as < std::string > ()); }
#line 2992 "sqlite3_parser.cpp"
    break;

  case 240: // columnid_list: columnid_list "," columnid
#line 800 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = yystack_[2].value.as < sqlb::StringVector > (); yylhs.value.as < sqlb::StringVector > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 2998 "sqlite3_parser.cpp"
    break;

  case 241: // optional_columnid_with_paren_list: %empty
#line 804 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = sqlb::StringVector(); }
#line 3004 "sqlite3_parser.cpp"
    break;

  case 242: // optional_columnid_with_paren_list: "(" columnid_list ")"
#line 805 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = yystack_[1].value.as < sqlb::StringVector > (); }
#line 3010 "sqlite3_parser.cpp"
    break;

  case 243: // fk_clause_part: "ON" "DELETE" "SET" "NULL"
#line 809 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3016 "sqlite3_parser.cpp"
    break;

  case 244: // fk_clause_part: "ON" "DELETE" "SET" "DEFAULT"
#line 810 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3022 "sqlite3_parser.cpp"
    break;

  case 245: // fk_clause_part: "ON" "DELETE" "CASCADE"
#line 811 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3028 "sqlite3_parser.cpp"
    break;

  case 246: // fk_clause_part: "ON" "DELETE" "RESTRICT"
#line 812 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3034 "sqlite3_parser.cpp"
    break;

  case 247: // fk_clause_part: "ON" "DELETE" "NO" "ACTION"
#line 813 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3040 "sqlite3_parser.cpp"
    break;

  case 248: // fk_clause_part: "ON" "UPDATE" "SET" "NULL"
#line 814 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3046 "sqlite3_parser.cpp"
    break;

  case 249: // fk_clause_part: "ON" "UPDATE" "SET" "DEFAULT"
#line 815 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3052 "sqlite3_parser.cpp"
    break;

  case 250: // fk_clause_part: "ON" "UPDATE" "CASCADE"
#line 816 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3058 "sqlite3_parser.cpp"
    break;

  case 251: // fk_clause_part: "ON" "UPDATE" "RESTRICT"
#line 817 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3064 "sqlite3_parser.cpp"
    break;

  case 252: // fk_clause_part: "ON" "UPDATE" "NO" "ACTION"
#line 818 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3070 "sqlite3_parser.cpp"
    break;

  case 253: // fk_clause_part: "ON" "INSERT" "SET" "NULL"
#line 819 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3076 "sqlite3_parser.cpp"
    break;

  case 254: // fk_clause_part: "ON" "INSERT" "SET" "DEFAULT"
#line 820 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3082 "sqlite3_parser.cpp"
    break;

  case 255: // fk_clause_part: "ON" "INSERT" "CASCADE"
#line 821 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3088 "sqlite3_parser.cpp"
    break;

  case 256: // fk_clause_part: "ON" "INSERT" "RESTRICT"
#line 822 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3094 "sqlite3_parser.cpp"
    break;

  case 257: // fk_clause_part: "ON" "INSERT" "NO" "ACTION"
#line 823 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3100 "sqlite3_parser.cpp"
    break;

  case 258: // fk_clause_part: "MATCH" id
#line 824 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3106 "sqlite3_parser.cpp"
    break;

  case 259: // fk_clause_part_list: fk_clause_part
#line 828 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3112 "sqlite3_parser.cpp"
    break;

  case 260: // fk_clause_part_list: fk_clause_part_list fk_clause_part
#line 829 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3118 "sqlite3_parser.cpp"
    break;

  case 261: // optional_fk_clause: %empty
#line 833 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = ""; }
#line 3124 "sqlite3_parser.cpp"
    break;

  case 262: // optional_fk_clause: fk_clause_part_list
#line 834 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3130 "sqlite3_parser.cpp"
    break;

  case 263: // optional_fk_clause: fk_clause_part_list "DEFERRABLE" "INITIALLY" "DEFERRED"
#line 835 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3136 "sqlite3_parser.cpp"
    break;

  case 264: // optional_fk_clause: fk_clause_part_list "DEFERRABLE" "INITIALLY" "IMMEDIATE"
#line 836 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3142 "sqlite3_parser.cpp"
    break;

  case 265: // optional_fk_clause: fk_clause_part_list "DEFERRABLE"
#line 837 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3148 "sqlite3_parser.cpp"
    break;

  case 266: // optional_fk_clause: fk_clause_part_list "NOT" "DEFERRABLE" "INITIALLY" "DEFERRED"
#line 838 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3154 "sqlite3_parser.cpp"
    break;

  case 267: // optional_fk_clause: fk_clause_part_list "NOT" "DEFERRABLE" "INITIALLY" "IMMEDIATE"
#line 839 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3160 "sqlite3_parser.cpp"
    break;

  case 268: // optional_fk_clause: fk_clause_part_list "NOT" "DEFERRABLE"
#line 840 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3166 "sqlite3_parser.cpp"
    break;

  case 269: // optional_fk_clause: "DEFERRABLE" "INITIALLY" "DEFERRED"
#line 841 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3172 "sqlite3_parser.cpp"
    break;

  case 270: // optional_fk_clause: "DEFERRABLE" "INITIALLY" "IMMEDIATE"
#line 842 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3178 "sqlite3_parser.cpp"
    break;

  case 271: // optional_fk_clause: "DEFERRABLE"
#line 843 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3184 "sqlite3_parser.cpp"
    break;

  case 272: // optional_fk_clause: "NOT" "DEFERRABLE" "INITIALLY" "DEFERRED"
#line 844 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3190 "sqlite3_parser.cpp"
    break;

  case 273: // optional_fk_clause: "NOT" "DEFERRABLE" "INITIALLY" "IMMEDIATE"
#line 845 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3196 "sqlite3_parser.cpp"
    break;

  case 274: // optional_fk_clause: "NOT" "DEFERRABLE"
#line 846 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3202 "sqlite3_parser.cpp"
    break;

  case 275: // tableconstraint: optional_constraintname "PRIMARY" "KEY" "(" indexed_column_list ")" optional_conflictclause
#line 850 "sqlite3_parser.yy"
                                                                                                        {
														auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>(yystack_[2].value.as < sqlb::IndexedColumnVector > ());
														pk->setName(yystack_[6].value.as < std::string > ());
														pk->setConflictAction(yystack_[0].value.as < std::string > ());
														yylhs.value.as < sqlb::ConstraintPtr > () = pk;
													}
#line 3213 "sqlite3_parser.cpp"
    break;

  case 276: // tableconstraint: optional_constraintname "PRIMARY" "KEY" "(" indexed_column_list "AUTOINCREMENT" ")" optional_conflictclause
#line 856 "sqlite3_parser.yy"
                                                                                                                {
														auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>(yystack_[3].value.as < sqlb::IndexedColumnVector > ());
														pk->setName(yystack_[7].value.as < std::string > ());
														pk->setConflictAction(yystack_[0].value.as < std::string > ());
														pk->setAutoIncrement(true);
														yylhs.value.as < sqlb::ConstraintPtr > () = pk;
													}
#line 3225 "sqlite3_parser.cpp"
    break;

  case 277: // tableconstraint: optional_constraintname "UNIQUE" "(" indexed_column_list ")" optional_conflictclause
#line 863 "sqlite3_parser.yy"
                                                                                                        {
														auto u = std::make_shared<sqlb::UniqueConstraint>(yystack_[2].value.as < sqlb::IndexedColumnVector > ());
														u->setName(yystack_[5].value.as < std::string > ());
														u->setConflictAction(yystack_[0].value.as < std::string > ());
														yylhs.value.as < sqlb::ConstraintPtr > () = u;
													}
#line 3236 "sqlite3_parser.cpp"
    break;

  case 278: // tableconstraint: optional_constraintname "CHECK" "(" expr ")"
#line 869 "sqlite3_parser.yy"
                                                                                                        {
														yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::CheckConstraint>(yystack_[1].value.as < std::string > ());
														yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[4].value.as < std::string > ());
													}
#line 3245 "sqlite3_parser.cpp"
    break;

  case 279: // tableconstraint: optional_constraintname "FOREIGN" "KEY" "(" columnid_list ")" "REFERENCES" tableid optional_columnid_with_paren_list optional_fk_clause
#line 873 "sqlite3_parser.yy"
                                                                                                                                                {
														yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::ForeignKeyClause>(yystack_[2].value.as < std::string > (), yystack_[1].value.as < sqlb::StringVector > (), yystack_[0].value.as < std::string > ());
														yylhs.value.as < sqlb::ConstraintPtr > ()->setColumnList(yystack_[5].value.as < sqlb::StringVector > ());
														yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[9].value.as < std::string > ());
													}
#line 3255 "sqlite3_parser.cpp"
    break;

  case 280: // tableconstraint_list: tableconstraint
#line 881 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintVector > () = {yystack_[0].value.as < sqlb::ConstraintPtr > ()}; }
#line 3261 "sqlite3_parser.cpp"
    break;

  case 281: // tableconstraint_list: tableconstraint_list "," tableconstraint
#line 882 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintVector > () = yystack_[2].value.as < sqlb::ConstraintVector > (); yylhs.value.as < sqlb::ConstraintVector > ().push_back(yystack_[0].value.as < sqlb::ConstraintPtr > ()); }
#line 3267 "sqlite3_parser.cpp"
    break;

  case 282: // tableconstraint_list: tableconstraint_list tableconstraint
#line 883 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintVector > () = yystack_[1].value.as < sqlb::ConstraintVector > (); yylhs.value.as < sqlb::ConstraintVector > ().push_back(yystack_[0].value.as < sqlb::ConstraintPtr > ()); }
#line 3273 "sqlite3_parser.cpp"
    break;

  case 283: // optional_tableconstraint_list: %empty
#line 887 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintVector > () = {}; }
#line 3279 "sqlite3_parser.cpp"
    break;

  case 284: // optional_tableconstraint_list: "," tableconstraint_list
#line 888 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintVector > () = yystack_[0].value.as < sqlb::ConstraintVector > (); }
#line 3285 "sqlite3_parser.cpp"
    break;

  case 285: // createtable_stmt: "CREATE" optional_temporary "TABLE" optional_if_not_exists tableid_with_uninteresting_schema "AS" select_stmt
#line 892 "sqlite3_parser.yy"
                                                                                                                        {
										yylhs.value.as < sqlb::TablePtr > () = std::make_shared<sqlb::Table>(yystack_[2].value.as < std::string > ());
										yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
									}
#line 3294 "sqlite3_parser.cpp"
    break;

  case 286: // createtable_stmt: "CREATE" optional_temporary "TABLE" optional_if_not_exists tableid_with_uninteresting_schema "(" columndef_list optional_tableconstraint_list ")" optional_withoutrowid
#line 896 "sqlite3_parser.yy"
                                                                                                                                                                                        {
										yylhs.value.as < sqlb::TablePtr > () = std::make_shared<sqlb::Table>(yystack_[5].value.as < std::string > ());
										yylhs.value.as < sqlb::TablePtr > ()->setWithoutRowidTable(yystack_[0].value.as < bool > ());
										yylhs.value.as < sqlb::TablePtr > ()->setConstraints(yystack_[2].value.as < sqlb::ConstraintVector > ());
										yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(true);

										for(const auto& column : yystack_[3].value.as < std::vector<ColumndefData> > ())
										{
											sqlb::Field f;
											sqlb::ConstraintVector c;
											std::tie(f, c) = column;

											yylhs.value.as < sqlb::TablePtr > ()->fields.push_back(f);
											for(const auto& i : c)
												yylhs.value.as < sqlb::TablePtr > ()->addConstraint(i);
										}
									}
#line 3316 "sqlite3_parser.cpp"
    break;


#line 3320 "sqlite3_parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        int yychecklim = yylast_ - yyn + 1;
        int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }



  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short parser::yypact_ninf_ = -342;

  const short parser::yytable_ninf_ = -285;

  const short
  parser::yypact_[] =
  {
     -10,   116,    45,    93,  -342,  -342,  -342,  -342,  -342,  -342,
      12,    61,    63,  -342,  -342,    40,    40,    40,    67,  2242,
    2242,  2242,   100,  -342,  -342,  -342,  -342,  -342,  -342,  -342,
    -342,  -342,  -342,  -342,  -342,  -342,  -342,  -342,  -342,  -342,
    -342,  -342,  -342,  -342,  -342,  -342,  -342,  -342,  -342,  -342,
    -342,  -342,  -342,  -342,  -342,  -342,  -342,  -342,  -342,  -342,
    -342,   172,  -342,  -342,  -342,  -342,  -342,  -342,  -342,   180,
    -342,  -342,    84,   120,    19,  -342,  2331,  2331,   -16,  2331,
    2153,   105,  -342,  -342,  -342,  -342,   203,   207,  -342,  -342,
    -342,  -342,  -342,  -342,  -342,  2331,  -342,   208,  -342,  -342,
     957,  -342,  1417,  -342,  1625,  -342,   103,  2064,   215,  1417,
    -342,  1417,  1417,  1417,  1072,   224,  -342,  -342,  -342,  -342,
    1417,  -342,   227,  -342,  -342,  -342,  -342,  -342,  -342,   121,
    -342,  -342,  -342,  -342,    21,  -342,  -342,  -342,  -342,  -342,
    -342,  3225,  2763,  -342,   201,     5,  -342,   -74,  -342,   110,
      -8,  -342,   -15,  -342,   111,   129,   205,  -342,  -342,  -342,
    1417,   -34,   454,  1417,  3335,     8,   612,   -16,  -342,  1417,
    1417,  1417,  1417,  1417,  1417,  1417,  1417,  1417,  1417,  1417,
    1417,  1417,  1417,  1417,  1417,  1417,  1417,  1417,  1417,  1417,
     -16,  1417,  1718,  1417,  -342,  1417,  1417,    68,  -342,  1417,
    1417,  -342,  -342,  -342,   112,  1417,   113,   128,  -342,  -342,
     214,  -342,  -342,  -342,   243,   -16,  1532,   218,   171,  -342,
     185,  2331,   196,   233,   274,   206,   212,   287,   249,  -342,
     202,  -342,  -342,  1919,  1417,  -342,  1417,   -11,  2840,   291,
     303,   306,   305,  -342,   308,  1417,   219,   309,  3225,   164,
     164,    13,    13,   150,    13,   150,   277,   371,   371,   316,
     316,   316,   316,   371,   371,   150,   150,  3335,  2917,  -342,
     173,   727,   248,  -342,   371,   223,   273,  1417,  1417,  1811,
    1417,  1417,  -342,  1417,  3302,   490,  1417,  -342,  -342,  -342,
    -342,  -342,     5,  1417,  -342,  1417,  -342,  -342,  -342,  -342,
    -342,  -342,   196,    -7,   317,   278,  -342,   319,  1417,   320,
     331,  1417,  -342,  -342,  1417,  2994,  2009,  1417,  -342,  2331,
     221,   222,  -342,   226,  -342,   250,  -342,   -16,  1417,  1417,
    -342,   251,   333,  1417,  2331,  1417,  1417,  3071,  1947,   842,
     259,  -342,  2048,  2468,  2550,  1417,  3225,   339,  2440,  2522,
    -342,   196,  2153,    57,    -9,  1417,  2604,  2153,  1417,   255,
    3225,  -342,  1417,  3148,   341,   342,   344,   345,  -342,  -342,
     371,   371,  -342,  -342,   264,   347,  -342,   371,   371,  1417,
    1417,  -342,   265,   348,  1417,  2331,  1417,  1417,  1417,   371,
    -342,  -342,  -342,   321,  -342,   268,   282,   -16,   311,   -20,
    -342,    88,  -342,  -342,  -342,  -342,  -342,  -342,  2686,  -342,
     269,   115,   196,  3225,  -342,  -342,  -342,  -342,  -342,  -342,
    1187,   371,   371,  -342,  -342,   297,   350,  -342,   371,   371,
     371,  -342,  -342,  2153,    44,  -342,   285,    -2,    11,    36,
     288,   312,  -342,    22,   275,   196,   357,  -342,  -342,   298,
    -342,  1302,  -342,  -342,  -342,    73,  -342,   334,  -342,   -30,
    -342,   336,  -342,   -24,  -342,   337,  -342,    59,    74,   302,
    -342,  -342,  -342,  2331,  -342,   196,  -342,  -342,   301,  -342,
    -342,  -342,  -342,  -342,  -342,  -342,  -342,  -342,  -342,  -342,
    -342,  -342,   106,   317,  -342,  -342,  -342,  -342,    57,  -342
  };

  const short
  parser::yydefact_[] =
  {
       0,   182,     0,     2,     4,     5,     6,   198,   199,   183,
       0,     0,     0,     1,     3,   177,   177,   177,     0,     0,
       0,     0,     0,    18,    19,    20,    21,    22,    23,    24,
      60,    59,    61,    25,    26,    27,    28,    29,    30,    31,
      32,    35,    37,    36,    33,    34,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    54,    53,    55,    56,    57,    16,    63,    17,    62,
      58,   188,     0,     0,     0,   178,     0,     0,     0,     0,
       0,     0,    54,    62,   187,   186,   193,     0,    66,    65,
      67,    68,    70,    69,    64,   208,   234,   283,   176,   285,
       0,   196,     0,    75,    77,   209,   236,   236,     0,     0,
     194,     0,     0,     0,     0,    23,    14,    13,    15,    12,
       0,    10,    43,    11,     7,     8,     9,   160,   158,   163,
     159,   164,   165,   170,     0,   166,   171,   172,   173,   174,
     175,   121,   179,   190,     0,     0,    76,     0,   230,   236,
     213,   235,     0,   280,   236,   200,     0,    81,    80,    82,
       0,     0,     0,     0,    83,     0,     0,     0,   195,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   127,     0,     0,     0,   128,     0,
       0,   180,   181,   189,   184,     0,     0,     0,    74,    73,
       0,   238,   237,   231,     0,     0,     0,     0,     0,   218,
       0,     0,   202,     0,     0,     0,     0,     0,   236,   282,
       0,   286,   167,     0,     0,   153,     0,     0,     0,     0,
       0,     0,     0,   125,     0,     0,     0,   162,   122,    88,
      89,    85,    86,    92,    87,    93,    84,    98,    99,    96,
      97,    94,    95,   100,   101,    90,    91,   103,     0,   169,
     106,     0,    62,   136,   102,   105,   107,     0,     0,     0,
       0,     0,   129,     0,   104,   108,     0,   192,   191,    71,
      72,    78,     0,     0,   227,     0,   225,   222,   223,   224,
     221,   214,   202,   179,   241,     0,   219,     0,     0,     0,
       0,     0,   281,   201,     0,     0,     0,     0,   151,     0,
       0,     0,   154,     0,   126,     0,   123,     0,     0,     0,
     132,     0,     0,     0,     0,     0,     0,     0,   110,     0,
      62,   144,   109,   111,   112,     0,   185,     0,     0,     0,
     217,   202,     0,   261,     0,     0,     0,     0,     0,     0,
     148,   152,     0,     0,     0,     0,     0,     0,   124,   161,
     130,   114,   134,   133,     0,    62,   135,   113,   115,     0,
       0,   140,     0,     0,     0,     0,     0,     0,     0,   116,
      79,   220,   226,   215,   239,     0,   271,     0,     0,     0,
     259,   262,   228,   204,   205,   206,   207,   203,     0,   278,
       0,     0,   202,   149,   150,   168,   156,   157,   155,   139,
       0,   131,   118,   142,   141,     0,    62,   143,   117,   119,
     120,   216,   242,     0,     0,   258,   274,     0,     0,     0,
     265,     0,   260,   210,     0,   202,     0,   277,   137,     0,
     147,     0,   240,   269,   270,     0,   245,     0,   246,     0,
     255,     0,   256,     0,   250,     0,   251,     0,     0,   268,
     211,   212,   229,     0,   275,   202,   138,   145,     0,   272,
     273,   247,   244,   243,   257,   254,   253,   252,   249,   248,
     263,   264,     0,   241,   276,   146,   266,   267,   261,   279
  };

  const short
  parser::yypgoto_[] =
  {
    -342,  -342,  -342,   157,   -19,   -13,   -67,  -341,   158,    83,
    -342,    58,  -342,  -342,  -342,  -104,  -342,  -342,  -342,  -342,
     232,  -342,  -342,   256,  -256,   134,    80,  -342,  -342,   149,
     183,  -290,  -342,  -342,  -342,  -342,  -342,  -299,  -342,  -342,
    -342,   240,  -342,   292,  -342,   -70,    38,   -95,    -1,  -342,
     -97,  -150,  -342,  -342,  -342
  };

  const short
  parser::yydefgoto_[] =
  {
       0,     2,     3,   128,   129,   130,    71,    95,   209,   210,
     104,   105,   131,   132,   133,   134,   135,   136,   137,   138,
     161,   139,   140,   141,    99,    19,   203,    11,   287,    72,
     143,   144,     4,   101,     5,    12,   231,   306,   106,   472,
     223,   148,   149,    96,    97,   152,   395,   353,   400,   401,
     402,   153,   154,   108,     6
  };

  const short
  parser::yytable_[] =
  {
      69,    69,    69,   350,   229,   156,    70,    70,    70,    84,
      85,   394,    87,   206,   207,   332,   394,   482,   403,   234,
     235,   359,    80,   485,   224,   168,   201,   169,   103,   177,
     437,   214,   215,     1,   456,   239,   150,   146,    66,   216,
     211,    68,   317,   318,   202,    13,   225,   460,   404,   483,
     438,    81,   393,   190,   217,   486,   405,    83,    83,    86,
      83,    93,   246,    70,    70,   240,    70,    94,   411,   218,
     226,   219,   464,   241,   457,   236,    83,   220,   312,   150,
     221,   406,    70,   383,   407,    83,   439,   461,    93,   458,
     227,    70,   452,   453,    94,   459,    66,   222,   236,    68,
      14,   242,   462,   277,    18,   396,   488,  -233,   463,  -233,
     454,    15,   465,   447,  -232,  -284,  -232,   228,   208,   445,
     470,   205,   479,   490,   166,   273,   167,   466,   212,    16,
     471,   278,   397,   467,   398,   279,   440,   399,   489,   480,
     491,   325,   280,   281,    22,   147,   474,   282,   247,   446,
      20,    21,   147,   147,   304,   496,    75,   283,   170,   171,
     172,   173,    17,   397,   175,   441,   177,   331,   399,    73,
      74,   269,   497,   272,   172,   173,   494,    76,   175,    70,
     177,   170,   171,   172,   173,    77,   174,   175,   176,   177,
     190,    78,   180,   181,   182,   183,   294,   298,   186,   187,
      79,    98,    83,   299,   190,   204,   100,   205,    70,   232,
     102,   169,   341,   190,   107,  -197,     7,     8,   291,   155,
     292,     9,   286,   326,    10,   169,   289,   163,   329,   374,
     165,   170,   171,   172,   173,   382,   174,   175,   176,   177,
     230,   290,   180,   181,   182,   183,   293,   301,   186,   187,
     302,   333,   103,   334,   368,   372,   169,   169,   303,   412,
     340,   205,   384,   190,   385,   307,    70,   376,   419,   423,
     169,   169,   432,   444,   433,   433,   305,   308,   335,   309,
     425,   170,   171,   172,   173,   310,   174,   175,   176,   177,
     311,   147,   180,   181,   182,   183,   313,   320,   186,   187,
      83,   450,   476,   169,   169,   495,    70,   169,   369,   321,
     322,   323,   324,   190,   327,   375,   449,   190,   427,   354,
     352,    70,   355,   357,   170,   171,   172,   173,   336,   174,
     175,   176,   177,    93,   358,   365,   366,   373,    93,    94,
     367,   186,   187,   390,    94,   415,   416,   478,   417,   418,
     420,   434,   424,   451,   455,   431,   190,   468,   142,   436,
     469,   475,   481,   473,   484,   487,   426,   157,   158,   159,
     162,   492,    70,   297,   300,   347,   164,   364,   435,   170,
     171,   172,   173,   351,   174,   175,   176,   177,   288,   213,
     180,   181,   182,   183,   237,   410,   186,   187,   498,   151,
     442,   499,     0,     0,     0,     0,   493,     0,     0,     0,
       0,   190,     0,     0,    93,     0,   233,     0,     0,   238,
      94,     0,     0,     0,     0,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,     0,   270,     0,   274,
       0,   275,   276,     0,    83,   284,   285,     0,     0,     0,
      70,   142,   170,   171,   172,   173,     0,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,     0,     0,     0,   188,     0,     0,     0,     0,   189,
     315,     0,   316,     0,   190,     0,     0,     0,   170,   171,
     172,   173,     0,   174,   175,   176,   177,     0,     0,   180,
     181,   182,   183,     0,     0,   186,   187,   191,     0,     0,
       0,   192,     0,     0,     0,   193,   194,     0,   195,   196,
     190,   197,   198,   337,   338,   199,   342,   343,     0,   344,
       0,     0,   346,   200,     0,   345,     0,     0,     0,   348,
       0,   349,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   160,   356,     0,     0,   142,     0,     0,
     360,     0,     0,   363,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   370,   371,     0,     0,     0,     0,
       0,   377,   378,     0,     0,     0,     0,     0,     0,     0,
       0,   389,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   408,     0,     0,   142,   109,   243,     0,   413,     0,
     111,   112,   244,     0,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   421,   422,     0,     0,    23,
      24,    25,   428,   429,   430,    26,     0,     0,    27,   114,
     115,     0,     0,    29,     0,     0,   116,   117,   118,     0,
       0,    33,     0,    34,   245,     0,    35,     0,     0,    36,
     119,    37,    38,     0,    39,    40,     0,    41,    42,     0,
       0,    43,     0,     0,     0,    44,    45,    46,    47,   120,
       0,   121,     0,     0,    48,    49,    50,     0,   122,    52,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,     0,    82,    62,     0,   123,    63,     0,     0,     0,
      64,     0,     0,    65,    66,   124,   125,    68,   126,   127,
     109,   330,     0,     0,     0,   111,   112,     0,     0,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,    24,    25,     0,     0,     0,
      26,     0,     0,    27,   114,   115,     0,     0,    29,     0,
       0,   116,   117,   118,     0,     0,    33,     0,    34,     0,
       0,    35,     0,     0,    36,   119,    37,    38,     0,    39,
      40,     0,    41,    42,     0,     0,    43,     0,     0,     0,
      44,    45,    46,    47,   120,     0,   121,     0,     0,    48,
      49,    50,     0,   122,    52,     0,    53,    54,    55,    56,
      57,    58,    59,    98,     0,    60,     0,    82,    62,     0,
     123,    63,     0,     0,     0,    64,     0,     0,    65,    66,
     124,   125,    68,   126,   127,   109,   381,     0,     0,     0,
     111,   112,     0,     0,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,     0,     0,     0,    26,     0,     0,    27,   114,
     115,     0,     0,    29,     0,     0,   116,   117,   118,     0,
       0,    33,     0,    34,     0,     0,    35,     0,     0,    36,
     119,    37,    38,     0,    39,    40,     0,    41,    42,     0,
       0,    43,     0,     0,     0,    44,    45,    46,    47,   120,
       0,   121,     0,     0,    48,    49,    50,     0,   122,    52,
       0,    53,    54,    55,    56,    57,    58,    59,    98,     0,
      60,     0,    82,    62,     0,   123,    63,     0,     0,     0,
      64,     0,     0,    65,    66,   124,   125,    68,   126,   127,
     109,   110,     0,     0,     0,   111,   112,     0,     0,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,    24,    25,     0,     0,     0,
      26,     0,     0,    27,   114,   115,     0,     0,    29,     0,
       0,   116,   117,   118,     0,     0,    33,     0,    34,     0,
       0,    35,     0,     0,    36,   119,    37,    38,     0,    39,
      40,     0,    41,    42,     0,     0,    43,     0,     0,     0,
      44,    45,    46,    47,   120,     0,   121,     0,     0,    48,
      49,    50,     0,   122,    52,     0,    53,    54,    55,    56,
      57,    58,    59,     0,     0,    60,     0,    82,    62,     0,
     123,    63,     0,     0,     0,    64,     0,     0,    65,    66,
     124,   125,    68,   126,   127,   109,     0,     0,     0,     0,
     111,   112,     0,     0,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,     0,     0,     0,    26,     0,     0,    27,   114,
     115,     0,     0,    29,     0,     0,   116,   117,   118,     0,
       0,    33,     0,    34,     0,     0,    35,     0,     0,    36,
     119,    37,    38,     0,    39,    40,     0,    41,    42,     0,
       0,    43,     0,     0,     0,    44,    45,    46,    47,   120,
       0,   121,     0,     0,    48,    49,    50,     0,   122,    52,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,     0,    82,    62,     0,   123,    63,     0,     0,     0,
      64,   160,     0,    65,    66,   124,   125,    68,   126,   127,
     109,   448,     0,     0,     0,   111,   112,     0,     0,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,    24,    25,     0,     0,     0,
      26,     0,     0,    27,   114,   115,     0,     0,    29,     0,
       0,   116,   117,   118,     0,     0,    33,     0,    34,     0,
       0,    35,     0,     0,    36,   119,    37,    38,     0,    39,
      40,     0,    41,    42,     0,     0,    43,     0,     0,     0,
      44,    45,    46,    47,   120,     0,   121,     0,     0,    48,
      49,    50,     0,   122,    52,     0,    53,    54,    55,    56,
      57,    58,    59,     0,     0,    60,     0,    82,    62,     0,
     123,    63,     0,     0,     0,    64,     0,     0,    65,    66,
     124,   125,    68,   126,   127,   109,   477,     0,     0,     0,
     111,   112,     0,     0,   113,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,     0,     0,     0,    26,     0,     0,    27,   114,
     115,     0,     0,    29,     0,     0,   116,   117,   118,     0,
       0,    33,     0,    34,     0,     0,    35,     0,     0,    36,
     119,    37,    38,     0,    39,    40,     0,    41,    42,     0,
       0,    43,     0,     0,     0,    44,    45,    46,    47,   120,
       0,   121,     0,     0,    48,    49,    50,     0,   122,    52,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,     0,    82,    62,     0,   123,    63,     0,     0,     0,
      64,     0,     0,    65,    66,   124,   125,    68,   126,   127,
     109,     0,     0,     0,     0,   111,   112,     0,     0,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,    24,    25,     0,     0,     0,
      26,     0,     0,    27,   114,   115,     0,     0,    29,     0,
       0,   116,   117,   118,     0,     0,    33,     0,    34,     0,
       0,    35,     0,     0,    36,   119,    37,    38,     0,    39,
      40,     0,    41,    42,     0,     0,    43,     0,     0,     0,
      44,    45,    46,    47,   120,     0,   121,     0,     0,    48,
      49,    50,     0,   122,    52,     0,    53,    54,    55,    56,
      57,    58,    59,     0,     0,    60,     0,    82,    62,     0,
     123,    63,     0,     0,     0,    64,     0,     0,    65,    66,
     124,   125,    68,   126,   127,   295,     0,     0,     0,     0,
     206,   207,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,     0,     0,     0,    26,     0,     0,    27,     0,
      28,     0,     0,    29,     0,     0,   116,   117,   118,     0,
       0,    33,     0,    34,     0,     0,    35,     0,     0,    36,
     119,    37,    38,     0,    39,    40,   296,    41,    42,     0,
       0,    43,     0,     0,     0,    44,    45,    46,    47,     0,
       0,   121,     0,     0,    48,    49,    50,     0,    51,    52,
       0,    53,    54,    55,    56,    57,    58,    59,   145,     0,
      60,     0,    82,    62,     0,   123,    63,     0,     0,     0,
      64,     0,     0,    65,    66,   124,   125,    68,   126,     0,
       0,     0,    23,    24,    25,     0,     0,     0,    26,     0,
       0,    27,     0,    28,     0,     0,    29,     0,     0,    30,
      31,    32,     0,     0,    33,     0,    34,     0,     0,    35,
       0,     0,    36,     0,    37,    38,     0,    39,    40,     0,
      41,    42,     0,     0,    43,     0,     0,     0,    44,    45,
      46,    47,     0,     0,     0,     0,     0,    48,    49,    50,
       0,    51,    52,     0,    53,    54,    55,    56,    57,    58,
      59,   271,     0,    60,     0,    82,    62,     0,     0,    63,
       0,     0,     0,    64,     0,     0,    65,    66,     0,    67,
      68,     0,     0,     0,     0,    23,    24,    25,     0,     0,
       0,    26,     0,     0,    27,     0,    28,     0,     0,    29,
       0,     0,    30,    31,    32,     0,     0,    33,     0,    34,
       0,     0,    35,     0,     0,    36,     0,    37,    38,     0,
      39,    40,     0,    41,    42,     0,     0,    43,     0,     0,
       0,    44,    45,    46,    47,     0,     0,     0,     0,     0,
      48,    49,    50,     0,    51,    52,     0,    53,    54,    55,
      56,    57,    58,    59,   339,     0,    60,     0,    82,    62,
       0,     0,    63,     0,     0,     0,    64,     0,     0,    65,
      66,     0,    67,    68,     0,     0,     0,     0,    23,    24,
      25,     0,     0,     0,    26,     0,     0,    27,     0,    28,
       0,     0,    29,     0,     0,    30,    31,    32,     0,     0,
      33,     0,    34,     0,     0,    35,     0,     0,    36,     0,
      37,    38,     0,    39,    40,     0,    41,    42,     0,     0,
      43,     0,     0,     0,    44,    45,    46,    47,     0,     0,
       0,     0,     0,    48,    49,    50,     0,    51,    52,     0,
      53,    54,    55,    56,    57,    58,    59,     0,     0,    60,
       0,    82,    62,     0,     0,    63,     0,     0,     0,    64,
       0,     0,    65,    66,     0,    67,    68,   170,   171,   172,
     173,     0,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,     0,     0,     0,   188,
       0,     0,     0,     0,   189,   170,   171,   172,   173,   190,
     174,   175,   176,   177,     0,     0,   180,   181,   182,   183,
       0,     0,   186,   187,     0,     0,     0,     0,     0,     0,
       0,     0,   191,     0,     0,     0,   192,   190,     0,     0,
     193,   194,     0,   195,   196,     0,   197,   198,     0,     0,
     199,     0,   380,     0,     0,     0,     0,     0,   200,     0,
       0,     0,     0,     0,     0,     0,     0,   170,   171,   172,
     173,   314,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,     0,     0,     0,   188,
       0,     0,     0,     0,   189,     0,     0,     0,     0,   190,
       0,     0,     0,     0,     0,     0,   170,   171,   172,   173,
       0,   174,   175,   176,   177,     0,     0,   180,   181,   182,
     183,     0,   191,   186,   187,     0,   192,     0,     0,     0,
     193,   194,     0,   195,   196,     0,   197,   198,   190,     0,
     199,    23,    24,    25,     0,     0,     0,    26,   200,     0,
      27,     0,    28,   386,     0,    29,   147,     0,    88,    89,
      90,   362,     0,    33,     0,    34,     0,     0,    35,     0,
       0,    36,     0,    37,    38,     0,    39,    40,    91,    41,
      42,     0,     0,    43,     0,     0,     0,    44,    45,    46,
      47,     0,     0,     0,     0,     0,    48,    49,    50,     0,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,     0,    82,    62,     0,     0,    63,     0,
       0,     0,    64,     0,     0,    65,    66,     0,    92,    68,
      23,    24,    25,     0,     0,     0,    26,     0,     0,    27,
       0,    28,     0,     0,    29,     0,     0,    88,    89,    90,
       0,     0,    33,     0,    34,     0,     0,    35,     0,     0,
      36,     0,    37,    38,     0,    39,    40,    91,    41,    42,
       0,     0,    43,     0,     0,     0,    44,    45,    46,    47,
       0,     0,     0,     0,     0,    48,    49,    50,     0,    51,
      52,     0,    53,    54,    55,    56,    57,    58,    59,     0,
       0,    60,     0,    82,    62,     0,     0,    63,     0,     0,
       0,    64,     0,     0,    65,    66,     0,    92,    68,    23,
      24,    25,     0,     0,     0,    26,     0,     0,    27,     0,
      28,     0,     0,    29,     0,     0,    30,    31,    32,     0,
       0,    33,     0,    34,     0,     0,    35,     0,     0,    36,
       0,    37,    38,     0,    39,    40,     0,    41,    42,     0,
       0,    43,     0,     0,     0,    44,    45,    46,    47,     0,
       0,     0,     0,     0,    48,    49,    50,     0,    51,    52,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,     0,    61,    62,     0,     0,    63,     0,     0,     0,
      64,     0,     0,    65,    66,     0,    67,    68,    23,    24,
      25,     0,     0,     0,    26,     0,     0,    27,     0,    28,
       0,     0,    29,     0,     0,    30,    31,    32,     0,     0,
      33,     0,    34,     0,     0,    35,     0,     0,    36,     0,
      37,    38,     0,    39,    40,     0,    41,    42,     0,     0,
      43,     0,     0,     0,    44,    45,    46,    47,     0,     0,
       0,     0,     0,    48,    49,    50,     0,    51,    52,     0,
      53,    54,    55,    56,    57,    58,    59,     0,     0,    60,
       0,    82,    62,     0,     0,    63,     0,     0,     0,    64,
       0,     0,    65,    66,   391,    67,    68,     0,   170,   171,
     172,   173,     0,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,     0,     0,     0,
     188,     0,     0,     0,     0,   189,   170,   171,   172,   173,
     190,   174,   175,   176,   177,     0,     0,   180,   181,   182,
     183,     0,     0,   186,   187,     0,     0,     0,     0,     0,
       0,     0,     0,   191,     0,     0,     0,   192,   190,     0,
       0,   193,   194,     0,   195,   196,     0,   197,   198,     0,
       0,   199,     0,   387,     0,     0,   392,     0,     0,   200,
     170,   171,   172,   173,     0,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,     0,
       0,     0,   188,     0,     0,     0,     0,   189,   170,   171,
     172,   173,   190,   174,   175,   176,   177,     0,     0,   180,
     181,   182,   183,     0,     0,   186,   187,     0,     0,     0,
       0,     0,     0,     0,     0,   191,     0,     0,     0,   192,
     190,     0,     0,   193,   194,     0,   195,   196,     0,   197,
     198,     0,     0,   199,     0,   388,     0,     0,   409,     0,
       0,   200,   170,   171,   172,   173,     0,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,     0,     0,     0,   188,     0,     0,     0,     0,   189,
       0,     0,     0,     0,   190,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   191,     0,     0,
       0,   192,     0,     0,     0,   193,   194,     0,   195,   196,
       0,   197,   198,     0,     0,   199,     0,     0,     0,     0,
     443,     0,     0,   200,   170,   171,   172,   173,     0,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,     0,     0,     0,   188,     0,     0,     0,
       0,   189,     0,     0,     0,     0,   190,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   191,
       0,     0,     0,   192,     0,     0,     0,   193,   194,     0,
     195,   196,     0,   197,   198,     0,     0,   199,     0,     0,
       0,   170,   171,   172,   173,   200,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
       0,     0,     0,   188,     0,     0,   201,     0,   189,     0,
       0,     0,     0,   190,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   202,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   191,     0,     0,     0,
     192,     0,     0,     0,   193,   194,     0,   195,   196,     0,
     197,   198,     0,     0,   199,     0,     0,     0,   170,   171,
     172,   173,   200,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,     0,     0,     0,
     188,     0,   319,     0,     0,   189,     0,     0,     0,     0,
     190,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   191,     0,     0,     0,   192,     0,     0,
       0,   193,   194,     0,   195,   196,     0,   197,   198,     0,
       0,   199,     0,     0,     0,   170,   171,   172,   173,   200,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,     0,     0,     0,   188,   328,     0,
       0,     0,   189,     0,     0,     0,     0,   190,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     191,     0,     0,     0,   192,     0,     0,     0,   193,   194,
       0,   195,   196,     0,   197,   198,     0,     0,   199,     0,
       0,     0,   170,   171,   172,   173,   200,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,     0,     0,     0,   188,     0,     0,     0,     0,   189,
       0,     0,     0,     0,   190,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   361,     0,
       0,     0,     0,     0,     0,     0,     0,   191,     0,     0,
       0,   192,     0,     0,     0,   193,   194,     0,   195,   196,
       0,   197,   198,     0,     0,   199,     0,     0,     0,   170,
     171,   172,   173,   200,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,     0,     0,
       0,   188,   379,     0,     0,     0,   189,     0,     0,     0,
       0,   190,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   191,     0,     0,     0,   192,     0,
       0,     0,   193,   194,     0,   195,   196,     0,   197,   198,
       0,     0,   199,     0,     0,     0,   170,   171,   172,   173,
     200,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,     0,     0,     0,   188,     0,
       0,     0,     0,   189,     0,     0,     0,     0,   190,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   414,     0,     0,     0,     0,     0,     0,     0,
       0,   191,     0,     0,     0,   192,     0,     0,     0,   193,
     194,     0,   195,   196,     0,   197,   198,     0,     0,   199,
       0,     0,     0,   170,   171,   172,   173,   200,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,     0,     0,     0,   188,     0,     0,     0,     0,
     189,     0,     0,     0,     0,   190,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   191,     0,
       0,     0,   192,     0,     0,     0,   193,   194,     0,   195,
     196,     0,   197,   198,     0,     0,   199,     0,     0,     0,
     170,   171,   172,   173,   200,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,     0,
       0,     0,   188,     0,     0,     0,     0,   189,     0,     0,
       0,     0,   190,   170,   171,   172,   173,     0,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,     0,     0,     0,   191,     0,     0,     0,   192,
     189,     0,     0,   193,   194,   190,   195,   196,     0,   197,
     198,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   200,     0,     0,     0,     0,     0,     0,   191,     0,
       0,     0,   192,     0,     0,     0,   193,   194,     0,   195,
     196,     0,   197,   198,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   200
  };

  const short
  parser::yycheck_[] =
  {
      19,    20,    21,   302,   154,   109,    19,    20,    21,    76,
      77,   352,    79,     8,     9,   271,   357,    47,    27,    53,
      54,   311,     3,    47,    39,     4,    33,     6,    95,    16,
      50,    39,    40,    43,    36,    27,   106,   104,   112,    47,
     114,   115,    53,    54,    51,     0,    61,    36,    57,    79,
      70,    32,   351,    40,    62,    79,    65,    76,    77,    78,
      79,    80,   166,    76,    77,    57,    79,    80,   358,    77,
      85,    79,    36,    65,    76,   109,    95,    85,   228,   149,
      88,    90,    95,   339,    93,   104,   106,    76,   107,    91,
     105,   104,   433,    49,   107,    97,   112,   105,   109,   115,
       7,    93,    91,    35,    64,    48,    47,     4,    97,     6,
      66,    99,    76,   412,     4,     4,     6,     6,   113,     4,
      98,     6,    49,    49,     3,   192,     5,    91,   147,    68,
     108,    63,    75,    97,    77,    67,    48,    80,    79,    66,
      66,   245,    74,    75,    77,    42,   445,    79,   167,    34,
      16,    17,    42,    42,   221,    49,    56,    89,     8,     9,
      10,    11,    99,    75,    14,    77,    16,   271,    80,    20,
      21,   190,    66,   192,    10,    11,   475,     5,    14,   192,
      16,     8,     9,    10,    11,     5,    13,    14,    15,    16,
      40,   107,    19,    20,    21,    22,   215,   216,    25,    26,
      80,    96,   221,   216,    40,     4,     3,     6,   221,     4,
       3,     6,   279,    40,     6,    99,   100,   101,     4,     4,
       6,   105,   110,     4,   108,     6,   113,     3,    55,   333,
       3,     8,     9,    10,    11,   339,    13,    14,    15,    16,
     111,   113,    19,    20,    21,    22,     3,    29,    25,    26,
      79,     3,   319,     5,     4,     4,     6,     6,    73,     4,
     279,     6,     3,    40,     5,    32,   279,   334,     4,     4,
       6,     6,     4,     4,     6,     6,    80,     3,    55,    73,
     384,     8,     9,    10,    11,    73,    13,    14,    15,    16,
       3,    42,    19,    20,    21,    22,    94,     6,    25,    26,
     319,     4,     4,     6,     6,     4,   319,     6,   327,     6,
       4,     6,     4,    40,     5,   334,   420,    40,   385,    41,
       3,   334,     3,     3,     8,     9,    10,    11,    55,    13,
      14,    15,    16,   352,     3,   114,   114,     4,   357,   352,
     114,    25,    26,     4,   357,     4,     4,   451,     4,     4,
       3,    69,     4,     3,    69,    34,    40,    69,   102,    48,
      48,     4,    28,    88,    28,    28,   385,   111,   112,   113,
     114,    69,   385,   216,   216,   292,   120,   319,   397,     8,
       9,    10,    11,   303,    13,    14,    15,    16,   205,   149,
      19,    20,    21,    22,   162,   357,    25,    26,   493,   107,
     401,   498,    -1,    -1,    -1,    -1,   473,    -1,    -1,    -1,
      -1,    40,    -1,    -1,   433,    -1,   160,    -1,    -1,   163,
     433,    -1,    -1,    -1,    -1,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    -1,   193,
      -1,   195,   196,    -1,   473,   199,   200,    -1,    -1,    -1,
     473,   205,     8,     9,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,
     234,    -1,   236,    -1,    40,    -1,    -1,    -1,     8,     9,
      10,    11,    -1,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    -1,    -1,    25,    26,    63,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    71,    72,    -1,    74,    75,
      40,    77,    78,   277,   278,    81,   280,   281,    -1,   283,
      -1,    -1,   286,    89,    -1,    55,    -1,    -1,    -1,   293,
      -1,   295,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   308,    -1,    -1,   311,    -1,    -1,
     314,    -1,    -1,   317,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   328,   329,    -1,    -1,    -1,    -1,
      -1,   335,   336,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   345,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   355,    -1,    -1,   358,     3,     4,    -1,   362,    -1,
       8,     9,    10,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   379,   380,    -1,    -1,    27,
      28,    29,   386,   387,   388,    33,    -1,    -1,    36,    37,
      38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,
      -1,    49,    -1,    51,    52,    -1,    54,    -1,    -1,    57,
      58,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      -1,    79,    -1,    -1,    82,    83,    84,    -1,    86,    87,
      -1,    89,    90,    91,    92,    93,    94,    95,    -1,    -1,
      98,    -1,   100,   101,    -1,   103,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,   112,   113,   114,   115,   116,   117,
       3,     4,    -1,    -1,    -1,     8,     9,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      33,    -1,    -1,    36,    37,    38,    -1,    -1,    41,    -1,
      -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    69,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    -1,    79,    -1,    -1,    82,
      83,    84,    -1,    86,    87,    -1,    89,    90,    91,    92,
      93,    94,    95,    96,    -1,    98,    -1,   100,   101,    -1,
     103,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,   112,
     113,   114,   115,   116,   117,     3,     4,    -1,    -1,    -1,
       8,     9,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,    37,
      38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,
      -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      -1,    79,    -1,    -1,    82,    83,    84,    -1,    86,    87,
      -1,    89,    90,    91,    92,    93,    94,    95,    96,    -1,
      98,    -1,   100,   101,    -1,   103,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,   112,   113,   114,   115,   116,   117,
       3,     4,    -1,    -1,    -1,     8,     9,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      33,    -1,    -1,    36,    37,    38,    -1,    -1,    41,    -1,
      -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    69,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    -1,    79,    -1,    -1,    82,
      83,    84,    -1,    86,    87,    -1,    89,    90,    91,    92,
      93,    94,    95,    -1,    -1,    98,    -1,   100,   101,    -1,
     103,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,   112,
     113,   114,   115,   116,   117,     3,    -1,    -1,    -1,    -1,
       8,     9,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,    37,
      38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,
      -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      -1,    79,    -1,    -1,    82,    83,    84,    -1,    86,    87,
      -1,    89,    90,    91,    92,    93,    94,    95,    -1,    -1,
      98,    -1,   100,   101,    -1,   103,   104,    -1,    -1,    -1,
     108,   109,    -1,   111,   112,   113,   114,   115,   116,   117,
       3,     4,    -1,    -1,    -1,     8,     9,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      33,    -1,    -1,    36,    37,    38,    -1,    -1,    41,    -1,
      -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    69,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    -1,    79,    -1,    -1,    82,
      83,    84,    -1,    86,    87,    -1,    89,    90,    91,    92,
      93,    94,    95,    -1,    -1,    98,    -1,   100,   101,    -1,
     103,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,   112,
     113,   114,   115,   116,   117,     3,     4,    -1,    -1,    -1,
       8,     9,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,    37,
      38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,
      -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      -1,    79,    -1,    -1,    82,    83,    84,    -1,    86,    87,
      -1,    89,    90,    91,    92,    93,    94,    95,    -1,    -1,
      98,    -1,   100,   101,    -1,   103,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,   112,   113,   114,   115,   116,   117,
       3,    -1,    -1,    -1,    -1,     8,     9,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      33,    -1,    -1,    36,    37,    38,    -1,    -1,    41,    -1,
      -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    69,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    -1,    79,    -1,    -1,    82,
      83,    84,    -1,    86,    87,    -1,    89,    90,    91,    92,
      93,    94,    95,    -1,    -1,    98,    -1,   100,   101,    -1,
     103,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,   112,
     113,   114,   115,   116,   117,     3,    -1,    -1,    -1,    -1,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,
      -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    -1,
      -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,
      -1,    79,    -1,    -1,    82,    83,    84,    -1,    86,    87,
      -1,    89,    90,    91,    92,    93,    94,    95,     3,    -1,
      98,    -1,   100,   101,    -1,   103,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,   112,   113,   114,   115,   116,    -1,
      -1,    -1,    27,    28,    29,    -1,    -1,    -1,    33,    -1,
      -1,    36,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      45,    46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    69,    -1,    -1,    -1,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,
      -1,    86,    87,    -1,    89,    90,    91,    92,    93,    94,
      95,     3,    -1,    98,    -1,   100,   101,    -1,    -1,   104,
      -1,    -1,    -1,   108,    -1,    -1,   111,   112,    -1,   114,
     115,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,
      -1,    33,    -1,    -1,    36,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    69,    -1,    -1,
      -1,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      82,    83,    84,    -1,    86,    87,    -1,    89,    90,    91,
      92,    93,    94,    95,     3,    -1,    98,    -1,   100,   101,
      -1,    -1,   104,    -1,    -1,    -1,   108,    -1,    -1,   111,
     112,    -1,   114,   115,    -1,    -1,    -1,    -1,    27,    28,
      29,    -1,    -1,    -1,    33,    -1,    -1,    36,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,    -1,
      49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    -1,    86,    87,    -1,
      89,    90,    91,    92,    93,    94,    95,    -1,    -1,    98,
      -1,   100,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,   112,    -1,   114,   115,     8,     9,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    35,     8,     9,    10,    11,    40,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    67,    40,    -1,    -1,
      71,    72,    -1,    74,    75,    -1,    77,    78,    -1,    -1,
      81,    -1,    55,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,     9,    10,
      11,   102,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,     8,     9,    10,    11,
      -1,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    -1,    63,    25,    26,    -1,    67,    -1,    -1,    -1,
      71,    72,    -1,    74,    75,    -1,    77,    78,    40,    -1,
      81,    27,    28,    29,    -1,    -1,    -1,    33,    89,    -1,
      36,    -1,    38,    55,    -1,    41,    42,    -1,    44,    45,
      46,   102,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    -1,    59,    60,    -1,    62,    63,    64,    65,
      66,    -1,    -1,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    -1,
      86,    87,    -1,    89,    90,    91,    92,    93,    94,    95,
      -1,    -1,    98,    -1,   100,   101,    -1,    -1,   104,    -1,
      -1,    -1,   108,    -1,    -1,   111,   112,    -1,   114,   115,
      27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,
      -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    -1,    59,    60,    -1,    62,    63,    64,    65,    66,
      -1,    -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    -1,    86,
      87,    -1,    89,    90,    91,    92,    93,    94,    95,    -1,
      -1,    98,    -1,   100,   101,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,   111,   112,    -1,   114,   115,    27,
      28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,
      -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      -1,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,    -1,    82,    83,    84,    -1,    86,    87,
      -1,    89,    90,    91,    92,    93,    94,    95,    -1,    -1,
      98,    -1,   100,   101,    -1,    -1,   104,    -1,    -1,    -1,
     108,    -1,    -1,   111,   112,    -1,   114,   115,    27,    28,
      29,    -1,    -1,    -1,    33,    -1,    -1,    36,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,    -1,
      49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    -1,    86,    87,    -1,
      89,    90,    91,    92,    93,    94,    95,    -1,    -1,    98,
      -1,   100,   101,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,   111,   112,     4,   114,   115,    -1,     8,     9,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    35,     8,     9,    10,    11,
      40,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    67,    40,    -1,
      -1,    71,    72,    -1,    74,    75,    -1,    77,    78,    -1,
      -1,    81,    -1,    55,    -1,    -1,     4,    -1,    -1,    89,
       8,     9,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    35,     8,     9,
      10,    11,    40,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    -1,    -1,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    67,
      40,    -1,    -1,    71,    72,    -1,    74,    75,    -1,    77,
      78,    -1,    -1,    81,    -1,    55,    -1,    -1,     4,    -1,
      -1,    89,     8,     9,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    71,    72,    -1,    74,    75,
      -1,    77,    78,    -1,    -1,    81,    -1,    -1,    -1,    -1,
       4,    -1,    -1,    89,     8,     9,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,    -1,
      74,    75,    -1,    77,    78,    -1,    -1,    81,    -1,    -1,
      -1,     8,     9,    10,    11,    89,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    71,    72,    -1,    74,    75,    -1,
      77,    78,    -1,    -1,    81,    -1,    -1,    -1,     8,     9,
      10,    11,    89,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      30,    -1,    32,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    71,    72,    -1,    74,    75,    -1,    77,    78,    -1,
      -1,    81,    -1,    -1,    -1,     8,     9,    10,    11,    89,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    31,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,
      -1,    74,    75,    -1,    77,    78,    -1,    -1,    81,    -1,
      -1,    -1,     8,     9,    10,    11,    89,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    71,    72,    -1,    74,    75,
      -1,    77,    78,    -1,    -1,    81,    -1,    -1,    -1,     8,
       9,    10,    11,    89,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      -1,    30,    31,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    71,    72,    -1,    74,    75,    -1,    77,    78,
      -1,    -1,    81,    -1,    -1,    -1,     8,     9,    10,    11,
      89,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    67,    -1,    -1,    -1,    71,
      72,    -1,    74,    75,    -1,    77,    78,    -1,    -1,    81,
      -1,    -1,    -1,     8,     9,    10,    11,    89,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    71,    72,    -1,    74,
      75,    -1,    77,    78,    -1,    -1,    81,    -1,    -1,    -1,
       8,     9,    10,    11,    89,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    40,     8,     9,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    63,    -1,    -1,    -1,    67,
      35,    -1,    -1,    71,    72,    40,    74,    75,    -1,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    71,    72,    -1,    74,
      75,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    43,   119,   120,   150,   152,   172,   100,   101,   105,
     108,   145,   153,     0,     7,    99,    68,    99,    64,   143,
     143,   143,    77,    27,    28,    29,    33,    36,    38,    41,
      44,    45,    46,    49,    51,    54,    57,    59,    60,    62,
      63,    65,    66,    69,    73,    74,    75,    76,    82,    83,
      84,    86,    87,    89,    90,    91,    92,    93,    94,    95,
      98,   100,   101,   104,   108,   111,   112,   114,   115,   122,
     123,   124,   147,   147,   147,    56,     5,     5,   107,    80,
       3,    32,   100,   122,   124,   124,   122,   124,    44,    45,
      46,    64,   114,   122,   123,   125,   161,   162,    96,   142,
       3,   151,     3,   124,   128,   129,   156,     6,   171,     3,
       4,     8,     9,    12,    37,    38,    44,    45,    46,    58,
      77,    79,    86,   103,   113,   114,   116,   117,   121,   122,
     123,   130,   131,   132,   133,   134,   135,   136,   137,   139,
     140,   141,   141,   148,   149,     3,   124,    42,   159,   160,
     163,   161,   163,   169,   170,     4,   133,   141,   141,   141,
     109,   138,   141,     3,   141,     3,     3,     5,     4,     6,
       8,     9,    10,    11,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    35,
      40,    63,    67,    71,    72,    74,    75,    77,    78,    81,
      89,    33,    51,   144,     4,     6,     8,     9,   113,   126,
     127,   114,   122,   159,    39,    40,    47,    62,    77,    79,
      85,    88,   105,   158,    39,    61,    85,   105,     6,   169,
     111,   154,     4,   141,    53,    54,   109,   138,   141,    27,
      57,    65,    93,     4,    10,    52,   133,   122,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   122,
     141,     3,   122,   124,   141,   141,   141,    35,    63,    67,
      74,    75,    79,    89,   141,   141,   110,   146,   148,   113,
     113,     4,     6,     3,   122,     3,    64,   121,   122,   123,
     126,    29,    79,    73,   124,    80,   155,    32,     3,    73,
      73,     3,   169,    94,   102,   141,   141,    53,    54,    32,
       6,     6,     4,     6,     4,   133,     4,     5,    31,    55,
       4,   133,   142,     3,     5,    55,    55,   141,   141,     3,
     122,   124,   141,   141,   141,    55,   141,   127,   141,   141,
     155,   144,     3,   165,    41,     3,   141,     3,     3,   149,
     141,    54,   102,   141,   129,   114,   114,   114,     4,   122,
     141,   141,     4,     4,   133,   122,   124,   141,   141,    31,
      55,     4,   133,   142,     3,     5,    55,    55,    55,   141,
       4,     4,     4,   155,   125,   164,    48,    75,    77,    80,
     166,   167,   168,    27,    57,    65,    90,    93,   141,     4,
     164,   149,     4,   141,    54,     4,     4,     4,     4,     4,
       3,   141,   141,     4,     4,   133,   122,   124,   141,   141,
     141,    34,     4,     6,    69,   122,    48,    50,    70,   106,
      48,    77,   166,     4,     4,     4,    34,   155,     4,   133,
       4,     3,   125,    49,    66,    69,    36,    76,    91,    97,
      36,    76,    91,    97,    36,    76,    91,    97,    69,    48,
      98,   108,   157,    88,   155,     4,     4,     4,   133,    49,
      66,    28,    47,    79,    28,    47,    79,    28,    47,    79,
      49,    66,    69,   124,   155,     4,    49,    66,   165,   168
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   118,   119,   119,   120,   120,   120,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   122,   122,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   124,   124,
     124,   124,   124,   124,   125,   125,   125,   125,   125,   125,
     125,   126,   126,   127,   127,   128,   128,   129,   129,   129,
     130,   130,   130,   130,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   133,   133,   134,   134,   134,   134,   135,   135,   135,
     136,   136,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   138,   138,
     139,   139,   139,   139,   140,   140,   140,   140,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   142,   143,   143,   144,
     144,   144,   145,   145,   146,   146,   147,   147,   147,   148,
     149,   149,   150,   151,   151,   151,   152,   153,   153,   153,
     154,   154,   155,   155,   155,   155,   155,   155,   156,   156,
     157,   157,   157,   158,   158,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     160,   160,   161,   161,   162,   162,   163,   163,   163,   164,
     164,   165,   165,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   167,
     167,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   169,   169,   169,   169,   169,
     170,   170,   170,   171,   171,   172,   172
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     2,     1,     4,     6,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     4,
       4,     4,     4,     5,     5,     5,     5,     6,     6,     6,
       6,     1,     3,     4,     5,     3,     4,     2,     2,     3,
       5,     6,     4,     5,     5,     5,     3,     7,     8,     6,
       5,     6,     6,     6,     4,     8,     9,     7,     4,     5,
       6,     4,     5,     3,     4,     6,     6,     6,     1,     1,
       1,     5,     3,     1,     1,     1,     1,     3,     6,     3,
       1,     1,     1,     1,     1,     1,     1,     0,     3,     0,
       1,     1,     0,     1,     0,     2,     3,     3,     1,     2,
       1,     3,    11,     0,     2,     3,     8,     0,     1,     1,
       0,     2,     0,     3,     3,     3,     3,     3,     0,     1,
       0,     1,     1,     0,     2,     5,     6,     4,     2,     3,
       5,     3,     3,     3,     3,     3,     5,     3,     5,     7,
       1,     2,     3,     2,     1,     3,     0,     2,     2,     1,
       3,     0,     3,     4,     4,     3,     3,     4,     4,     4,
       3,     3,     4,     4,     4,     3,     3,     4,     2,     1,
       2,     0,     1,     4,     4,     2,     5,     5,     3,     3,
       3,     1,     4,     4,     2,     7,     8,     6,     5,    10,
       1,     3,     2,     0,     2,     7,    10
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "\"(\"", "\")\"",
  "\".\"", "\",\"", "\";\"", "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"~\"",
  "\"&\"", "\"%\"", "\"|\"", "\"||\"", "\"=\"", "\"==\"", "\">\"",
  "\">=\"", "\"<\"", "\"<=\"", "\"!=\"", "\"<>\"", "\"<<\"", "\">>\"",
  "\"ABORT\"", "\"ACTION\"", "\"ALWAYS\"", "\"AND\"", "\"AND BETWEEN\"",
  "\"AS\"", "\"ASC\"", "\"AUTOINCREMENT\"", "\"BETWEEN\"", "\"CASCADE\"",
  "\"CASE\"", "\"CAST\"", "\"CHECK\"", "\"COLLATE\"", "\"CONFLICT\"",
  "\"CONSTRAINT\"", "\"CREATE\"", "\"CURRENT_DATE\"", "\"CURRENT_TIME\"",
  "\"CURRENT_TIMESTAMP\"", "\"DEFAULT\"", "\"DEFERRABLE\"", "\"DEFERRED\"",
  "\"DELETE\"", "\"DESC\"", "\"DISTINCT\"", "\"ELSE\"", "\"END\"",
  "\"ESCAPE\"", "\"EXISTS\"", "\"FAIL\"", "\"FALSE\"", "\"FILTER\"",
  "\"FOLLOWING\"", "\"FOREIGN\"", "\"GENERATED\"", "\"GLOB\"", "\"IF\"",
  "\"IGNORE\"", "\"IMMEDIATE\"", "\"IN\"", "\"INDEX\"", "\"INITIALLY\"",
  "\"INSERT\"", "\"IS\"", "\"ISNULL\"", "\"KEY\"", "\"LIKE\"", "\"MATCH\"",
  "\"NO\"", "\"NOT\"", "\"NOTNULL\"", "\"NULL\"", "\"ON\"", "\"OR\"",
  "\"OVER\"", "\"PARTITION\"", "\"PRECEDING\"", "\"PRIMARY\"", "\"RAISE\"",
  "\"RANGE\"", "\"REFERENCES\"", "\"REGEXP\"", "\"REPLACE\"",
  "\"RESTRICT\"", "\"RETURNING\"", "\"ROLLBACK\"", "\"ROWID\"", "\"ROWS\"",
  "\"SELECT\"", "\"SET\"", "\"STORED\"", "\"TABLE\"", "\"TEMP\"",
  "\"TEMPORARY\"", "\"THEN\"", "\"TRUE\"", "\"UNBOUNDED\"", "\"UNIQUE\"",
  "\"UPDATE\"", "\"USING\"", "\"VIRTUAL\"", "\"WHEN\"", "\"WHERE\"",
  "\"WITHOUT\"", "\"identifier\"", "\"numeric\"", "\"string literal\"",
  "\"quoted literal\"", "\"blob literal\"", "\"bind parameter\"",
  "$accept", "sql", "statement", "literalvalue", "id",
  "allowed_keywords_as_identifier", "tableid", "columnid", "signednumber",
  "signednumber_or_numeric", "typename_namelist", "type_name",
  "unary_expr", "binary_expr", "like_expr", "exprlist_expr",
  "function_expr", "isnull_expr", "between_expr", "in_expr",
  "whenthenlist_expr", "case_expr", "raise_expr", "expr", "select_stmt",
  "optional_if_not_exists", "optional_sort_order", "optional_unique",
  "optional_where", "tableid_with_uninteresting_schema", "indexed_column",
  "indexed_column_list", "createindex_stmt",
  "optional_exprlist_with_paren", "createvirtualtable_stmt",
  "optional_temporary", "optional_withoutrowid", "optional_conflictclause",
  "optional_typename", "optional_storage_identifier",
  "optional_always_generated", "columnconstraint", "columnconstraint_list",
  "columndef", "columndef_list", "optional_constraintname",
  "columnid_list", "optional_columnid_with_paren_list", "fk_clause_part",
  "fk_clause_part_list", "optional_fk_clause", "tableconstraint",
  "tableconstraint_list", "optional_tableconstraint_list",
  "createtable_stmt", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   263,   263,   264,   268,   269,   270,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   290,   291,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   339,   340,
     341,   342,   343,   344,   348,   349,   350,   351,   352,   353,
     354,   358,   359,   363,   364,   368,   369,   373,   374,   375,
     379,   380,   381,   382,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   429,   430,   434,   435,   436,   437,   441,   442,   443,
     447,   448,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   471,   472,
     476,   477,   478,   479,   483,   484,   485,   486,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   507,   516,   524,   525,   529,
     530,   531,   539,   540,   544,   545,   549,   550,   551,   555,
     569,   570,   574,   589,   590,   591,   595,   607,   608,   609,
     613,   614,   618,   619,   620,   621,   622,   623,   627,   628,
     632,   633,   634,   638,   639,   643,   649,   656,   662,   664,
     670,   674,   678,   682,   686,   691,   695,   699,   703,   711,
     718,   719,   723,   784,   788,   789,   793,   794,   795,   799,
     800,   804,   805,   809,   810,   811,   812,   813,   814,   815,
     816,   817,   818,   819,   820,   821,   822,   823,   824,   828,
     829,   833,   834,   835,   836,   837,   838,   839,   840,   841,
     842,   843,   844,   845,   846,   850,   856,   863,   869,   873,
     881,   882,   883,   887,   888,   892,   896
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 10 "sqlite3_parser.yy"
} } //  sqlb::parser 
#line 4741 "sqlite3_parser.cpp"

#line 915 "sqlite3_parser.yy"


void sqlb::parser::parser::error(const location_type& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}
