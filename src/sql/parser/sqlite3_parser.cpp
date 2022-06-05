// A Bison parser, made by GNU Bison 3.8.2.

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
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
#line 40 "sqlite3_parser.yy"

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

  /*---------.
  | symbol.  |
  `---------*/



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

      case symbol_kind::S_tableconstraint: // tableconstraint
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.YY_MOVE_OR_COPY< Constraints > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
        value.YY_MOVE_OR_COPY< sqlb::ConstraintPtr > (YY_MOVE (that.value));
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

      case symbol_kind::S_tableoption: // tableoption
      case symbol_kind::S_tableoptions_list: // tableoptions_list
      case symbol_kind::S_optional_tableoptions_list: // optional_tableoptions_list
        value.YY_MOVE_OR_COPY< std::bitset<sqlb::Table::NumOptions> > (YY_MOVE (that.value));
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
      case symbol_kind::S_STRICT: // "STRICT"
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

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
        value.YY_MOVE_OR_COPY< std::vector<sqlb::ConstraintPtr> > (YY_MOVE (that.value));
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

      case symbol_kind::S_tableconstraint: // tableconstraint
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.move< Constraints > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.move< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
        value.move< sqlb::ConstraintPtr > (YY_MOVE (that.value));
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

      case symbol_kind::S_tableoption: // tableoption
      case symbol_kind::S_tableoptions_list: // tableoptions_list
      case symbol_kind::S_optional_tableoptions_list: // optional_tableoptions_list
        value.move< std::bitset<sqlb::Table::NumOptions> > (YY_MOVE (that.value));
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
      case symbol_kind::S_STRICT: // "STRICT"
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

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
        value.move< std::vector<sqlb::ConstraintPtr> > (YY_MOVE (that.value));
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

      case symbol_kind::S_tableconstraint: // tableconstraint
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.copy< Constraints > (that.value);
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.copy< bool > (that.value);
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
        value.copy< sqlb::ConstraintPtr > (that.value);
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

      case symbol_kind::S_tableoption: // tableoption
      case symbol_kind::S_tableoptions_list: // tableoptions_list
      case symbol_kind::S_optional_tableoptions_list: // optional_tableoptions_list
        value.copy< std::bitset<sqlb::Table::NumOptions> > (that.value);
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
      case symbol_kind::S_STRICT: // "STRICT"
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

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
        value.copy< std::vector<sqlb::ConstraintPtr> > (that.value);
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

      case symbol_kind::S_tableconstraint: // tableconstraint
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.move< Constraints > (that.value);
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.move< bool > (that.value);
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
        value.move< sqlb::ConstraintPtr > (that.value);
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

      case symbol_kind::S_tableoption: // tableoption
      case symbol_kind::S_tableoptions_list: // tableoptions_list
      case symbol_kind::S_optional_tableoptions_list: // optional_tableoptions_list
        value.move< std::bitset<sqlb::Table::NumOptions> > (that.value);
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
      case symbol_kind::S_STRICT: // "STRICT"
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

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
        value.move< std::vector<sqlb::ConstraintPtr> > (that.value);
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
  parser::yypop_ (int n) YY_NOEXCEPT
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
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
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

      case symbol_kind::S_tableconstraint: // tableconstraint
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        yylhs.value.emplace< Constraints > ();
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        yylhs.value.emplace< bool > ();
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
        yylhs.value.emplace< sqlb::ConstraintPtr > ();
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

      case symbol_kind::S_tableoption: // tableoption
      case symbol_kind::S_tableoptions_list: // tableoptions_list
      case symbol_kind::S_optional_tableoptions_list: // optional_tableoptions_list
        yylhs.value.emplace< std::bitset<sqlb::Table::NumOptions> > ();
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
      case symbol_kind::S_STRICT: // "STRICT"
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

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
        yylhs.value.emplace< std::vector<sqlb::ConstraintPtr> > ();
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
#line 278 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::IndexPtr > (); }
#line 1489 "sqlite3_parser.cpp"
    break;

  case 5: // statement: createvirtualtable_stmt
#line 279 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::TablePtr > (); }
#line 1495 "sqlite3_parser.cpp"
    break;

  case 6: // statement: createtable_stmt
#line 280 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::TablePtr > (); }
#line 1501 "sqlite3_parser.cpp"
    break;

  case 7: // literalvalue: "numeric"
#line 288 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1507 "sqlite3_parser.cpp"
    break;

  case 8: // literalvalue: "string literal"
#line 289 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1513 "sqlite3_parser.cpp"
    break;

  case 9: // literalvalue: "blob literal"
#line 290 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1519 "sqlite3_parser.cpp"
    break;

  case 10: // literalvalue: "NULL"
#line 291 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1525 "sqlite3_parser.cpp"
    break;

  case 11: // literalvalue: "TRUE"
#line 292 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1531 "sqlite3_parser.cpp"
    break;

  case 12: // literalvalue: "FALSE"
#line 293 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1537 "sqlite3_parser.cpp"
    break;

  case 13: // literalvalue: "CURRENT_TIME"
#line 294 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1543 "sqlite3_parser.cpp"
    break;

  case 14: // literalvalue: "CURRENT_DATE"
#line 295 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1549 "sqlite3_parser.cpp"
    break;

  case 15: // literalvalue: "CURRENT_TIMESTAMP"
#line 296 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1555 "sqlite3_parser.cpp"
    break;

  case 16: // id: "identifier"
#line 300 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1561 "sqlite3_parser.cpp"
    break;

  case 17: // id: "quoted literal"
#line 301 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1567 "sqlite3_parser.cpp"
    break;

  case 18: // allowed_keywords_as_identifier: "ABORT"
#line 306 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1573 "sqlite3_parser.cpp"
    break;

  case 19: // allowed_keywords_as_identifier: "ACTION"
#line 307 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1579 "sqlite3_parser.cpp"
    break;

  case 20: // allowed_keywords_as_identifier: "ALWAYS"
#line 308 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1585 "sqlite3_parser.cpp"
    break;

  case 21: // allowed_keywords_as_identifier: "ASC"
#line 309 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1591 "sqlite3_parser.cpp"
    break;

  case 22: // allowed_keywords_as_identifier: "CASCADE"
#line 310 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1597 "sqlite3_parser.cpp"
    break;

  case 23: // allowed_keywords_as_identifier: "CAST"
#line 311 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1603 "sqlite3_parser.cpp"
    break;

  case 24: // allowed_keywords_as_identifier: "CONFLICT"
#line 312 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1609 "sqlite3_parser.cpp"
    break;

  case 25: // allowed_keywords_as_identifier: "DEFERRED"
#line 313 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1615 "sqlite3_parser.cpp"
    break;

  case 26: // allowed_keywords_as_identifier: "DESC"
#line 314 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1621 "sqlite3_parser.cpp"
    break;

  case 27: // allowed_keywords_as_identifier: "END"
#line 315 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1627 "sqlite3_parser.cpp"
    break;

  case 28: // allowed_keywords_as_identifier: "FAIL"
#line 316 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1633 "sqlite3_parser.cpp"
    break;

  case 29: // allowed_keywords_as_identifier: "FILTER"
#line 317 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1639 "sqlite3_parser.cpp"
    break;

  case 30: // allowed_keywords_as_identifier: "FOLLOWING"
#line 318 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1645 "sqlite3_parser.cpp"
    break;

  case 31: // allowed_keywords_as_identifier: "GENERATED"
#line 319 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1651 "sqlite3_parser.cpp"
    break;

  case 32: // allowed_keywords_as_identifier: "GLOB"
#line 320 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1657 "sqlite3_parser.cpp"
    break;

  case 33: // allowed_keywords_as_identifier: "KEY"
#line 321 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1663 "sqlite3_parser.cpp"
    break;

  case 34: // allowed_keywords_as_identifier: "LIKE"
#line 322 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1669 "sqlite3_parser.cpp"
    break;

  case 35: // allowed_keywords_as_identifier: "IGNORE"
#line 323 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1675 "sqlite3_parser.cpp"
    break;

  case 36: // allowed_keywords_as_identifier: "INITIALLY"
#line 324 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1681 "sqlite3_parser.cpp"
    break;

  case 37: // allowed_keywords_as_identifier: "IMMEDIATE"
#line 325 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1687 "sqlite3_parser.cpp"
    break;

  case 38: // allowed_keywords_as_identifier: "MATCH"
#line 326 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1693 "sqlite3_parser.cpp"
    break;

  case 39: // allowed_keywords_as_identifier: "NO"
#line 327 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1699 "sqlite3_parser.cpp"
    break;

  case 40: // allowed_keywords_as_identifier: "OVER"
#line 328 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1705 "sqlite3_parser.cpp"
    break;

  case 41: // allowed_keywords_as_identifier: "PARTITION"
#line 329 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1711 "sqlite3_parser.cpp"
    break;

  case 42: // allowed_keywords_as_identifier: "PRECEDING"
#line 330 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1717 "sqlite3_parser.cpp"
    break;

  case 43: // allowed_keywords_as_identifier: "RAISE"
#line 331 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1723 "sqlite3_parser.cpp"
    break;

  case 44: // allowed_keywords_as_identifier: "RANGE"
#line 332 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1729 "sqlite3_parser.cpp"
    break;

  case 45: // allowed_keywords_as_identifier: "REGEXP"
#line 333 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1735 "sqlite3_parser.cpp"
    break;

  case 46: // allowed_keywords_as_identifier: "REPLACE"
#line 334 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1741 "sqlite3_parser.cpp"
    break;

  case 47: // allowed_keywords_as_identifier: "RESTRICT"
#line 335 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1747 "sqlite3_parser.cpp"
    break;

  case 48: // allowed_keywords_as_identifier: "RETURNING"
#line 336 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1753 "sqlite3_parser.cpp"
    break;

  case 49: // allowed_keywords_as_identifier: "ROLLBACK"
#line 337 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1759 "sqlite3_parser.cpp"
    break;

  case 50: // allowed_keywords_as_identifier: "ROWID"
#line 338 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1765 "sqlite3_parser.cpp"
    break;

  case 51: // allowed_keywords_as_identifier: "ROWS"
#line 339 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1771 "sqlite3_parser.cpp"
    break;

  case 52: // allowed_keywords_as_identifier: "STORED"
#line 340 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1777 "sqlite3_parser.cpp"
    break;

  case 53: // allowed_keywords_as_identifier: "STRICT"
#line 341 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1783 "sqlite3_parser.cpp"
    break;

  case 54: // allowed_keywords_as_identifier: "TEMPORARY"
#line 342 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1789 "sqlite3_parser.cpp"
    break;

  case 55: // allowed_keywords_as_identifier: "TEMP"
#line 343 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1795 "sqlite3_parser.cpp"
    break;

  case 56: // allowed_keywords_as_identifier: "UNBOUNDED"
#line 344 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1801 "sqlite3_parser.cpp"
    break;

  case 57: // allowed_keywords_as_identifier: "VIRTUAL"
#line 345 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1807 "sqlite3_parser.cpp"
    break;

  case 58: // allowed_keywords_as_identifier: "WITHOUT"
#line 346 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1813 "sqlite3_parser.cpp"
    break;

  case 59: // tableid: allowed_keywords_as_identifier
#line 350 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1819 "sqlite3_parser.cpp"
    break;

  case 60: // tableid: "CURRENT_TIME"
#line 351 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1825 "sqlite3_parser.cpp"
    break;

  case 61: // tableid: "CURRENT_DATE"
#line 352 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1831 "sqlite3_parser.cpp"
    break;

  case 62: // tableid: "CURRENT_TIMESTAMP"
#line 353 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1837 "sqlite3_parser.cpp"
    break;

  case 63: // tableid: id
#line 354 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1843 "sqlite3_parser.cpp"
    break;

  case 64: // tableid: "string literal"
#line 355 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1849 "sqlite3_parser.cpp"
    break;

  case 65: // columnid: allowed_keywords_as_identifier
#line 359 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1855 "sqlite3_parser.cpp"
    break;

  case 66: // columnid: "CURRENT_TIME"
#line 360 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1861 "sqlite3_parser.cpp"
    break;

  case 67: // columnid: "CURRENT_DATE"
#line 361 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1867 "sqlite3_parser.cpp"
    break;

  case 68: // columnid: "CURRENT_TIMESTAMP"
#line 362 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1873 "sqlite3_parser.cpp"
    break;

  case 69: // columnid: "IF"
#line 363 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1879 "sqlite3_parser.cpp"
    break;

  case 70: // columnid: id
#line 364 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1885 "sqlite3_parser.cpp"
    break;

  case 71: // columnid: "string literal"
#line 365 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1891 "sqlite3_parser.cpp"
    break;

  case 72: // signednumber: "+" "numeric"
#line 369 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1897 "sqlite3_parser.cpp"
    break;

  case 73: // signednumber: "-" "numeric"
#line 370 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1903 "sqlite3_parser.cpp"
    break;

  case 74: // signednumber_or_numeric: signednumber
#line 374 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1909 "sqlite3_parser.cpp"
    break;

  case 75: // signednumber_or_numeric: "numeric"
#line 375 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1915 "sqlite3_parser.cpp"
    break;

  case 76: // typename_namelist: tableid
#line 379 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1921 "sqlite3_parser.cpp"
    break;

  case 77: // typename_namelist: typename_namelist tableid
#line 380 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 1927 "sqlite3_parser.cpp"
    break;

  case 78: // type_name: typename_namelist
#line 384 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1933 "sqlite3_parser.cpp"
    break;

  case 79: // type_name: typename_namelist "(" signednumber_or_numeric ")"
#line 385 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1939 "sqlite3_parser.cpp"
    break;

  case 80: // type_name: typename_namelist "(" signednumber_or_numeric "," signednumber_or_numeric ")"
#line 386 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + "(" + yystack_[3].value.as < std::string > () + ", " + yystack_[1].value.as < std::string > () + ")"; }
#line 1945 "sqlite3_parser.cpp"
    break;

  case 81: // unary_expr: "-" expr
#line 390 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1951 "sqlite3_parser.cpp"
    break;

  case 82: // unary_expr: "+" expr
#line 391 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1957 "sqlite3_parser.cpp"
    break;

  case 83: // unary_expr: "~" expr
#line 392 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "~" + yystack_[0].value.as < std::string > (); }
#line 1963 "sqlite3_parser.cpp"
    break;

  case 84: // unary_expr: "NOT" expr
#line 393 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "NOT " + yystack_[0].value.as < std::string > (); }
#line 1969 "sqlite3_parser.cpp"
    break;

  case 85: // binary_expr: expr "||" expr
#line 397 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " || " + yystack_[0].value.as < std::string > (); }
#line 1975 "sqlite3_parser.cpp"
    break;

  case 86: // binary_expr: expr "*" expr
#line 398 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " * " + yystack_[0].value.as < std::string > (); }
#line 1981 "sqlite3_parser.cpp"
    break;

  case 87: // binary_expr: expr "/" expr
#line 399 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " / " + yystack_[0].value.as < std::string > (); }
#line 1987 "sqlite3_parser.cpp"
    break;

  case 88: // binary_expr: expr "%" expr
#line 400 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " % " + yystack_[0].value.as < std::string > (); }
#line 1993 "sqlite3_parser.cpp"
    break;

  case 89: // binary_expr: expr "+" expr
#line 401 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " + " + yystack_[0].value.as < std::string > (); }
#line 1999 "sqlite3_parser.cpp"
    break;

  case 90: // binary_expr: expr "-" expr
#line 402 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " - " + yystack_[0].value.as < std::string > (); }
#line 2005 "sqlite3_parser.cpp"
    break;

  case 91: // binary_expr: expr "<<" expr
#line 403 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " << " + yystack_[0].value.as < std::string > (); }
#line 2011 "sqlite3_parser.cpp"
    break;

  case 92: // binary_expr: expr ">>" expr
#line 404 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >> " + yystack_[0].value.as < std::string > (); }
#line 2017 "sqlite3_parser.cpp"
    break;

  case 93: // binary_expr: expr "&" expr
#line 405 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " & " + yystack_[0].value.as < std::string > (); }
#line 2023 "sqlite3_parser.cpp"
    break;

  case 94: // binary_expr: expr "|" expr
#line 406 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " | " + yystack_[0].value.as < std::string > (); }
#line 2029 "sqlite3_parser.cpp"
    break;

  case 95: // binary_expr: expr "<" expr
#line 407 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " < " + yystack_[0].value.as < std::string > (); }
#line 2035 "sqlite3_parser.cpp"
    break;

  case 96: // binary_expr: expr "<=" expr
#line 408 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <= " + yystack_[0].value.as < std::string > (); }
#line 2041 "sqlite3_parser.cpp"
    break;

  case 97: // binary_expr: expr ">" expr
#line 409 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " > " + yystack_[0].value.as < std::string > (); }
#line 2047 "sqlite3_parser.cpp"
    break;

  case 98: // binary_expr: expr ">=" expr
#line 410 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >= " + yystack_[0].value.as < std::string > (); }
#line 2053 "sqlite3_parser.cpp"
    break;

  case 99: // binary_expr: expr "=" expr
#line 411 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " = " + yystack_[0].value.as < std::string > (); }
#line 2059 "sqlite3_parser.cpp"
    break;

  case 100: // binary_expr: expr "==" expr
#line 412 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " == " + yystack_[0].value.as < std::string > (); }
#line 2065 "sqlite3_parser.cpp"
    break;

  case 101: // binary_expr: expr "!=" expr
#line 413 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " != " + yystack_[0].value.as < std::string > (); }
#line 2071 "sqlite3_parser.cpp"
    break;

  case 102: // binary_expr: expr "<>" expr
#line 414 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <> " + yystack_[0].value.as < std::string > (); }
#line 2077 "sqlite3_parser.cpp"
    break;

  case 103: // binary_expr: expr "IS" expr
#line 415 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IS " + yystack_[0].value.as < std::string > (); }
#line 2083 "sqlite3_parser.cpp"
    break;

  case 104: // binary_expr: expr "AND" expr
#line 416 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2089 "sqlite3_parser.cpp"
    break;

  case 105: // binary_expr: expr "OR" expr
#line 417 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " OR " + yystack_[0].value.as < std::string > (); }
#line 2095 "sqlite3_parser.cpp"
    break;

  case 106: // like_expr: expr "LIKE" expr
#line 421 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " LIKE " + yystack_[0].value.as < std::string > (); }
#line 2101 "sqlite3_parser.cpp"
    break;

  case 107: // like_expr: expr "GLOB" expr
#line 422 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " GLOB " + yystack_[0].value.as < std::string > (); }
#line 2107 "sqlite3_parser.cpp"
    break;

  case 108: // like_expr: expr "MATCH" expr
#line 423 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " MATCH " + yystack_[0].value.as < std::string > (); }
#line 2113 "sqlite3_parser.cpp"
    break;

  case 109: // like_expr: expr "REGEXP" expr
#line 424 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " REGEXP " + yystack_[0].value.as < std::string > (); }
#line 2119 "sqlite3_parser.cpp"
    break;

  case 110: // like_expr: expr "NOT" "LIKE" expr
#line 425 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT LIKE " + yystack_[0].value.as < std::string > (); }
#line 2125 "sqlite3_parser.cpp"
    break;

  case 111: // like_expr: expr "NOT" "GLOB" expr
#line 426 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT GLOB " + yystack_[0].value.as < std::string > (); }
#line 2131 "sqlite3_parser.cpp"
    break;

  case 112: // like_expr: expr "NOT" "MATCH" expr
#line 427 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT MATCH " + yystack_[0].value.as < std::string > (); }
#line 2137 "sqlite3_parser.cpp"
    break;

  case 113: // like_expr: expr "NOT" "REGEXP" expr
#line 428 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT REGEXP " + yystack_[0].value.as < std::string > (); }
#line 2143 "sqlite3_parser.cpp"
    break;

  case 114: // like_expr: expr "LIKE" expr "ESCAPE" expr
#line 429 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " LIKE " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2149 "sqlite3_parser.cpp"
    break;

  case 115: // like_expr: expr "GLOB" expr "ESCAPE" expr
#line 430 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " GLOB " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2155 "sqlite3_parser.cpp"
    break;

  case 116: // like_expr: expr "MATCH" expr "ESCAPE" expr
#line 431 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " MATCH " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2161 "sqlite3_parser.cpp"
    break;

  case 117: // like_expr: expr "REGEXP" expr "ESCAPE" expr
#line 432 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " REGEXP " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2167 "sqlite3_parser.cpp"
    break;

  case 118: // like_expr: expr "NOT" "LIKE" expr "ESCAPE" expr
#line 433 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT LIKE " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2173 "sqlite3_parser.cpp"
    break;

  case 119: // like_expr: expr "NOT" "GLOB" expr "ESCAPE" expr
#line 434 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT GLOB " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2179 "sqlite3_parser.cpp"
    break;

  case 120: // like_expr: expr "NOT" "MATCH" expr "ESCAPE" expr
#line 435 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT MATCH " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2185 "sqlite3_parser.cpp"
    break;

  case 121: // like_expr: expr "NOT" "REGEXP" expr "ESCAPE" expr
#line 436 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT REGEXP " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2191 "sqlite3_parser.cpp"
    break;

  case 122: // exprlist_expr: expr
#line 440 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2197 "sqlite3_parser.cpp"
    break;

  case 123: // exprlist_expr: exprlist_expr "," expr
#line 441 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + ", " + yystack_[0].value.as < std::string > (); }
#line 2203 "sqlite3_parser.cpp"
    break;

  case 124: // function_expr: id "(" exprlist_expr ")"
#line 445 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2209 "sqlite3_parser.cpp"
    break;

  case 125: // function_expr: id "(" "DISTINCT" exprlist_expr ")"
#line 446 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + "(DISTINCT " + yystack_[1].value.as < std::string > () + ")"; }
#line 2215 "sqlite3_parser.cpp"
    break;

  case 126: // function_expr: id "(" ")"
#line 447 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + "()"; }
#line 2221 "sqlite3_parser.cpp"
    break;

  case 127: // function_expr: id "(" "*" ")"
#line 448 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(*)"; }
#line 2227 "sqlite3_parser.cpp"
    break;

  case 128: // isnull_expr: expr "ISNULL"
#line 452 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " ISNULL"; }
#line 2233 "sqlite3_parser.cpp"
    break;

  case 129: // isnull_expr: expr "NOTNULL"
#line 453 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " NOTNULL"; }
#line 2239 "sqlite3_parser.cpp"
    break;

  case 130: // isnull_expr: expr "NOT" "NULL"
#line 454 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " NOT NULL"; }
#line 2245 "sqlite3_parser.cpp"
    break;

  case 131: // between_expr: expr "BETWEEN" expr "AND BETWEEN" expr
#line 458 "sqlite3_parser.yy"
                                                                { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2251 "sqlite3_parser.cpp"
    break;

  case 132: // between_expr: expr "NOT" "BETWEEN" expr "AND BETWEEN" expr
#line 459 "sqlite3_parser.yy"
                                                                { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2257 "sqlite3_parser.cpp"
    break;

  case 133: // in_expr: expr "IN" "(" ")"
#line 463 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " IN ()"; }
#line 2263 "sqlite3_parser.cpp"
    break;

  case 134: // in_expr: expr "IN" "(" select_stmt ")"
#line 464 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2269 "sqlite3_parser.cpp"
    break;

  case 135: // in_expr: expr "IN" "(" exprlist_expr ")"
#line 465 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2275 "sqlite3_parser.cpp"
    break;

  case 136: // in_expr: expr "IN" id "." tableid
#line 466 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2281 "sqlite3_parser.cpp"
    break;

  case 137: // in_expr: expr "IN" tableid
#line 467 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2287 "sqlite3_parser.cpp"
    break;

  case 138: // in_expr: expr "IN" id "." id "(" ")"
#line 468 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 2293 "sqlite3_parser.cpp"
    break;

  case 139: // in_expr: expr "IN" id "." id "(" exprlist_expr ")"
#line 469 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2299 "sqlite3_parser.cpp"
    break;

  case 140: // in_expr: expr "IN" id "(" exprlist_expr ")"
#line 470 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2305 "sqlite3_parser.cpp"
    break;

  case 141: // in_expr: expr "NOT" "IN" "(" ")"
#line 471 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " NOT IN ()"; }
#line 2311 "sqlite3_parser.cpp"
    break;

  case 142: // in_expr: expr "NOT" "IN" "(" select_stmt ")"
#line 472 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2317 "sqlite3_parser.cpp"
    break;

  case 143: // in_expr: expr "NOT" "IN" "(" exprlist_expr ")"
#line 473 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2323 "sqlite3_parser.cpp"
    break;

  case 144: // in_expr: expr "NOT" "IN" id "." tableid
#line 474 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2329 "sqlite3_parser.cpp"
    break;

  case 145: // in_expr: expr "NOT" "IN" tableid
#line 475 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2335 "sqlite3_parser.cpp"
    break;

  case 146: // in_expr: expr "NOT" "IN" id "." id "(" ")"
#line 476 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 2341 "sqlite3_parser.cpp"
    break;

  case 147: // in_expr: expr "NOT" "IN" id "." id "(" exprlist_expr ")"
#line 477 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[8].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2347 "sqlite3_parser.cpp"
    break;

  case 148: // in_expr: expr "NOT" "IN" id "(" exprlist_expr ")"
#line 478 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " NOT IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2353 "sqlite3_parser.cpp"
    break;

  case 149: // whenthenlist_expr: "WHEN" expr "THEN" expr
#line 482 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "WHEN " + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 2359 "sqlite3_parser.cpp"
    break;

  case 150: // whenthenlist_expr: whenthenlist_expr "WHEN" expr "THEN" expr
#line 483 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " WHEN" + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 2365 "sqlite3_parser.cpp"
    break;

  case 151: // case_expr: "CASE" expr whenthenlist_expr "ELSE" expr "END"
#line 487 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2371 "sqlite3_parser.cpp"
    break;

  case 152: // case_expr: "CASE" expr whenthenlist_expr "END"
#line 488 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " END"; }
#line 2377 "sqlite3_parser.cpp"
    break;

  case 153: // case_expr: "CASE" whenthenlist_expr "ELSE" expr "END"
#line 489 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2383 "sqlite3_parser.cpp"
    break;

  case 154: // case_expr: "CASE" whenthenlist_expr "END"
#line 490 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2389 "sqlite3_parser.cpp"
    break;

  case 155: // raise_expr: "RAISE" "(" "IGNORE" ")"
#line 494 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(IGNORE)"; }
#line 2395 "sqlite3_parser.cpp"
    break;

  case 156: // raise_expr: "RAISE" "(" "ROLLBACK" "," "string literal" ")"
#line 495 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(ROLLBACK, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2401 "sqlite3_parser.cpp"
    break;

  case 157: // raise_expr: "RAISE" "(" "ABORT" "," "string literal" ")"
#line 496 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(ABORT, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2407 "sqlite3_parser.cpp"
    break;

  case 158: // raise_expr: "RAISE" "(" "FAIL" "," "string literal" ")"
#line 497 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(FAIL, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2413 "sqlite3_parser.cpp"
    break;

  case 159: // expr: literalvalue
#line 501 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2419 "sqlite3_parser.cpp"
    break;

  case 160: // expr: allowed_keywords_as_identifier
#line 502 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2425 "sqlite3_parser.cpp"
    break;

  case 161: // expr: "bind parameter"
#line 503 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2431 "sqlite3_parser.cpp"
    break;

  case 162: // expr: id "." id "." id
#line 504 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2437 "sqlite3_parser.cpp"
    break;

  case 163: // expr: id "." id
#line 505 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2443 "sqlite3_parser.cpp"
    break;

  case 164: // expr: id
#line 506 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2449 "sqlite3_parser.cpp"
    break;

  case 165: // expr: unary_expr
#line 507 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2455 "sqlite3_parser.cpp"
    break;

  case 166: // expr: binary_expr
#line 508 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2461 "sqlite3_parser.cpp"
    break;

  case 167: // expr: function_expr
#line 509 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2467 "sqlite3_parser.cpp"
    break;

  case 168: // expr: "(" exprlist_expr ")"
#line 510 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2473 "sqlite3_parser.cpp"
    break;

  case 169: // expr: "CAST" "(" expr "AS" type_name ")"
#line 511 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "CAST(" + yystack_[3].value.as < std::string > () + " AS " + yystack_[1].value.as < std::string > () + ")"; }
#line 2479 "sqlite3_parser.cpp"
    break;

  case 170: // expr: expr "COLLATE" id
#line 512 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " COLLATE " + yystack_[0].value.as < std::string > (); }
#line 2485 "sqlite3_parser.cpp"
    break;

  case 171: // expr: like_expr
#line 513 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2491 "sqlite3_parser.cpp"
    break;

  case 172: // expr: isnull_expr
#line 514 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2497 "sqlite3_parser.cpp"
    break;

  case 173: // expr: between_expr
#line 515 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2503 "sqlite3_parser.cpp"
    break;

  case 174: // expr: in_expr
#line 516 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2509 "sqlite3_parser.cpp"
    break;

  case 175: // expr: case_expr
#line 517 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2515 "sqlite3_parser.cpp"
    break;

  case 176: // expr: raise_expr
#line 518 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2521 "sqlite3_parser.cpp"
    break;

  case 177: // select_stmt: "SELECT"
#line 527 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "SELECT"; }
#line 2527 "sqlite3_parser.cpp"
    break;

  case 178: // optional_if_not_exists: %empty
#line 535 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = false; }
#line 2533 "sqlite3_parser.cpp"
    break;

  case 179: // optional_if_not_exists: "IF" "NOT" "EXISTS"
#line 536 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = true; }
#line 2539 "sqlite3_parser.cpp"
    break;

  case 180: // optional_sort_order: %empty
#line 540 "sqlite3_parser.yy"
                                                { }
#line 2545 "sqlite3_parser.cpp"
    break;

  case 181: // optional_sort_order: "ASC"
#line 541 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "ASC"; }
#line 2551 "sqlite3_parser.cpp"
    break;

  case 182: // optional_sort_order: "DESC"
#line 542 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "DESC"; }
#line 2557 "sqlite3_parser.cpp"
    break;

  case 183: // optional_unique: %empty
#line 550 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = false; }
#line 2563 "sqlite3_parser.cpp"
    break;

  case 184: // optional_unique: "UNIQUE"
#line 551 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = true; }
#line 2569 "sqlite3_parser.cpp"
    break;

  case 185: // optional_where: %empty
#line 555 "sqlite3_parser.yy"
                                                { }
#line 2575 "sqlite3_parser.cpp"
    break;

  case 186: // optional_where: "WHERE" expr
#line 556 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2581 "sqlite3_parser.cpp"
    break;

  case 187: // tableid_with_uninteresting_schema: id "." tableid
#line 560 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2587 "sqlite3_parser.cpp"
    break;

  case 188: // tableid_with_uninteresting_schema: "TEMP" "." tableid
#line 561 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2593 "sqlite3_parser.cpp"
    break;

  case 189: // tableid_with_uninteresting_schema: tableid
#line 562 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2599 "sqlite3_parser.cpp"
    break;

  case 190: // indexed_column: expr optional_sort_order
#line 566 "sqlite3_parser.yy"
                                                {
							// If the expression is only one column name and nothing else, treat it as a column name; otherwise as an expression.
							char quote = getIdentifierQuoteChar();
							if((quote == '[' && std::count(yystack_[1].value.as < std::string > ().begin(), yystack_[1].value.as < std::string > ().end(), quote) == 1 && yystack_[1].value.as < std::string > ().front() == '[' && yystack_[1].value.as < std::string > ().back() == ']') ||
								(quote != '[' && std::count(yystack_[1].value.as < std::string > ().begin(), yystack_[1].value.as < std::string > ().end(), quote) == 2 && yystack_[1].value.as < std::string > ().front() == quote && yystack_[1].value.as < std::string > ().back() == quote))
							{
								yylhs.value.as < sqlb::IndexedColumn > () = sqlb::IndexedColumn(unquote_text(yystack_[1].value.as < std::string > (), quote), false, yystack_[0].value.as < std::string > ());
							} else if(std::count(yystack_[1].value.as < std::string > ().begin(), yystack_[1].value.as < std::string > ().end(), '\'') == 2 && yystack_[1].value.as < std::string > ().front() == '\'' && yystack_[1].value.as < std::string > ().back() == '\'') {
								// Also remove single quotes when this actually is a string literal but looks like a columnid
								yylhs.value.as < sqlb::IndexedColumn > () = sqlb::IndexedColumn(unquote_text(yystack_[1].value.as < std::string > (), '\''), false, yystack_[0].value.as < std::string > ());
							} else {
								yylhs.value.as < sqlb::IndexedColumn > () = sqlb::IndexedColumn(yystack_[1].value.as < std::string > (), true, yystack_[0].value.as < std::string > ());
							}
						}
#line 2618 "sqlite3_parser.cpp"
    break;

  case 191: // indexed_column_list: indexed_column
#line 583 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::IndexedColumnVector > () = sqlb::IndexedColumnVector(1, yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2624 "sqlite3_parser.cpp"
    break;

  case 192: // indexed_column_list: indexed_column_list "," indexed_column
#line 584 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::IndexedColumnVector > () = yystack_[2].value.as < sqlb::IndexedColumnVector > (); yylhs.value.as < sqlb::IndexedColumnVector > ().push_back(yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2630 "sqlite3_parser.cpp"
    break;

  case 193: // createindex_stmt: "CREATE" optional_unique "INDEX" optional_if_not_exists tableid_with_uninteresting_schema "ON" tableid "(" indexed_column_list ")" optional_where
#line 588 "sqlite3_parser.yy"
                                                                                                                                                                {
													yylhs.value.as < sqlb::IndexPtr > () = std::make_shared<sqlb::Index>(yystack_[6].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setTable(yystack_[4].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setUnique(yystack_[9].value.as < bool > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setWhereExpr(yystack_[0].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->fields = yystack_[2].value.as < sqlb::IndexedColumnVector > ();
													yylhs.value.as < sqlb::IndexPtr > ()->setFullyParsed(true);
												}
#line 2643 "sqlite3_parser.cpp"
    break;

  case 194: // optional_exprlist_with_paren: %empty
#line 603 "sqlite3_parser.yy"
                                                { }
#line 2649 "sqlite3_parser.cpp"
    break;

  case 195: // optional_exprlist_with_paren: "(" ")"
#line 604 "sqlite3_parser.yy"
                                                { }
#line 2655 "sqlite3_parser.cpp"
    break;

  case 196: // optional_exprlist_with_paren: "(" exprlist_expr ")"
#line 605 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 2661 "sqlite3_parser.cpp"
    break;

  case 197: // createvirtualtable_stmt: "CREATE" "VIRTUAL" "TABLE" optional_if_not_exists tableid_with_uninteresting_schema "USING" id optional_exprlist_with_paren
#line 609 "sqlite3_parser.yy"
                                                                                                                                {
													yylhs.value.as < sqlb::TablePtr > () = std::make_shared<sqlb::Table>(yystack_[3].value.as < std::string > ());
													yylhs.value.as < sqlb::TablePtr > ()->setVirtualUsing(yystack_[1].value.as < std::string > ());
													yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
												}
#line 2671 "sqlite3_parser.cpp"
    break;

  case 198: // optional_temporary: %empty
#line 621 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = false; }
#line 2677 "sqlite3_parser.cpp"
    break;

  case 199: // optional_temporary: "TEMP"
#line 622 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2683 "sqlite3_parser.cpp"
    break;

  case 200: // optional_temporary: "TEMPORARY"
#line 623 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2689 "sqlite3_parser.cpp"
    break;

  case 201: // tableoption: "WITHOUT" "ROWID"
#line 627 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::bitset<sqlb::Table::NumOptions> > ().set(sqlb::Table::WithoutRowid, true); }
#line 2695 "sqlite3_parser.cpp"
    break;

  case 202: // tableoption: "STRICT"
#line 628 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::bitset<sqlb::Table::NumOptions> > ().set(sqlb::Table::Strict, true); }
#line 2701 "sqlite3_parser.cpp"
    break;

  case 203: // tableoptions_list: tableoption
#line 632 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::bitset<sqlb::Table::NumOptions> > () = yystack_[0].value.as < std::bitset<sqlb::Table::NumOptions> > (); }
#line 2707 "sqlite3_parser.cpp"
    break;

  case 204: // tableoptions_list: tableoptions_list "," tableoption
#line 633 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::bitset<sqlb::Table::NumOptions> > () = yystack_[2].value.as < std::bitset<sqlb::Table::NumOptions> > () | yystack_[0].value.as < std::bitset<sqlb::Table::NumOptions> > (); }
#line 2713 "sqlite3_parser.cpp"
    break;

  case 205: // tableoptions_list: tableoptions_list tableoption
#line 634 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::bitset<sqlb::Table::NumOptions> > () = yystack_[1].value.as < std::bitset<sqlb::Table::NumOptions> > () | yystack_[0].value.as < std::bitset<sqlb::Table::NumOptions> > (); }
#line 2719 "sqlite3_parser.cpp"
    break;

  case 206: // optional_tableoptions_list: %empty
#line 638 "sqlite3_parser.yy"
                                                        { }
#line 2725 "sqlite3_parser.cpp"
    break;

  case 207: // optional_tableoptions_list: tableoptions_list
#line 639 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::bitset<sqlb::Table::NumOptions> > () = yystack_[0].value.as < std::bitset<sqlb::Table::NumOptions> > (); }
#line 2731 "sqlite3_parser.cpp"
    break;

  case 208: // optional_conflictclause: %empty
#line 643 "sqlite3_parser.yy"
                                                        { }
#line 2737 "sqlite3_parser.cpp"
    break;

  case 209: // optional_conflictclause: "ON" "CONFLICT" "ROLLBACK"
#line 644 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2743 "sqlite3_parser.cpp"
    break;

  case 210: // optional_conflictclause: "ON" "CONFLICT" "ABORT"
#line 645 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2749 "sqlite3_parser.cpp"
    break;

  case 211: // optional_conflictclause: "ON" "CONFLICT" "FAIL"
#line 646 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2755 "sqlite3_parser.cpp"
    break;

  case 212: // optional_conflictclause: "ON" "CONFLICT" "IGNORE"
#line 647 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2761 "sqlite3_parser.cpp"
    break;

  case 213: // optional_conflictclause: "ON" "CONFLICT" "REPLACE"
#line 648 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2767 "sqlite3_parser.cpp"
    break;

  case 214: // optional_typename: %empty
#line 652 "sqlite3_parser.yy"
                                                { }
#line 2773 "sqlite3_parser.cpp"
    break;

  case 215: // optional_typename: type_name
#line 653 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2779 "sqlite3_parser.cpp"
    break;

  case 216: // optional_storage_identifier: %empty
#line 657 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "VIRTUAL"; }
#line 2785 "sqlite3_parser.cpp"
    break;

  case 217: // optional_storage_identifier: "STORED"
#line 658 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "STORED"; }
#line 2791 "sqlite3_parser.cpp"
    break;

  case 218: // optional_storage_identifier: "VIRTUAL"
#line 659 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "VIRTUAL"; }
#line 2797 "sqlite3_parser.cpp"
    break;

  case 219: // optional_always_generated: %empty
#line 663 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = false; }
#line 2803 "sqlite3_parser.cpp"
    break;

  case 220: // optional_always_generated: "GENERATED" "ALWAYS"
#line 664 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2809 "sqlite3_parser.cpp"
    break;

  case 221: // columnconstraint: optional_constraintname "PRIMARY" "KEY" optional_sort_order optional_conflictclause
#line 668 "sqlite3_parser.yy"
                                                                                        {
												auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>();
												pk->setName(yystack_[4].value.as < std::string > ());
												pk->setConflictAction(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > () = pk;
											}
#line 2820 "sqlite3_parser.cpp"
    break;

  case 222: // columnconstraint: optional_constraintname "PRIMARY" "KEY" optional_sort_order optional_conflictclause "AUTOINCREMENT"
#line 674 "sqlite3_parser.yy"
                                                                                                        {
												auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>();
												pk->setName(yystack_[5].value.as < std::string > ());
												pk->setConflictAction(yystack_[1].value.as < std::string > ());
												pk->setAutoIncrement(true);
												yylhs.value.as < sqlb::ConstraintPtr > () = pk;
											}
#line 2832 "sqlite3_parser.cpp"
    break;

  case 223: // columnconstraint: optional_constraintname "NOT" "NULL" optional_conflictclause
#line 681 "sqlite3_parser.yy"
                                                                                        {
												auto nn = std::make_shared<sqlb::NotNullConstraint>();
												nn->setName(yystack_[3].value.as < std::string > ());
												nn->setConflictAction(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > () = nn;
											}
#line 2843 "sqlite3_parser.cpp"
    break;

  case 224: // columnconstraint: optional_constraintname "NULL"
#line 687 "sqlite3_parser.yy"
                                                                                        {
											}
#line 2850 "sqlite3_parser.cpp"
    break;

  case 225: // columnconstraint: optional_constraintname "UNIQUE" optional_conflictclause
#line 689 "sqlite3_parser.yy"
                                                                                        {
												auto u = std::make_shared<sqlb::UniqueConstraint>();
												u->setName(yystack_[2].value.as < std::string > ());
												u->setConflictAction(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > () = u;
											}
#line 2861 "sqlite3_parser.cpp"
    break;

  case 226: // columnconstraint: optional_constraintname "CHECK" "(" expr ")"
#line 695 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::CheckConstraint>(yystack_[1].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[4].value.as < std::string > ());
											}
#line 2870 "sqlite3_parser.cpp"
    break;

  case 227: // columnconstraint: optional_constraintname "DEFAULT" signednumber
#line 699 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2879 "sqlite3_parser.cpp"
    break;

  case 228: // columnconstraint: optional_constraintname "DEFAULT" literalvalue
#line 703 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2888 "sqlite3_parser.cpp"
    break;

  case 229: // columnconstraint: optional_constraintname "DEFAULT" id
#line 707 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2897 "sqlite3_parser.cpp"
    break;

  case 230: // columnconstraint: optional_constraintname "DEFAULT" allowed_keywords_as_identifier
#line 711 "sqlite3_parser.yy"
                                                                                        {	// We must allow the same keywords as unquoted default values as in the columnid context.
												// But we do not use columnid here in order to avoid reduce/reduce conflicts.
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2907 "sqlite3_parser.cpp"
    break;

  case 231: // columnconstraint: optional_constraintname "DEFAULT" "IF"
#line 716 "sqlite3_parser.yy"
                                                                                        {	// Same as above.
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2916 "sqlite3_parser.cpp"
    break;

  case 232: // columnconstraint: optional_constraintname "DEFAULT" "(" expr ")"
#line 720 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>("(" + yystack_[1].value.as < std::string > () + ")");
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[4].value.as < std::string > ());
											}
#line 2925 "sqlite3_parser.cpp"
    break;

  case 233: // columnconstraint: optional_constraintname "COLLATE" id
#line 724 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::CollateConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2934 "sqlite3_parser.cpp"
    break;

  case 234: // columnconstraint: optional_constraintname "REFERENCES" tableid optional_columnid_with_paren_list optional_fk_clause
#line 728 "sqlite3_parser.yy"
                                                                                                                {	// TODO Solve shift/reduce conflict. It is not super important though as shifting seems to be right here.
												auto fk = std::make_shared<sqlb::ForeignKeyClause>();
												fk->setName(yystack_[4].value.as < std::string > ());
												fk->setTable(yystack_[2].value.as < std::string > ());
												fk->setColumns(yystack_[1].value.as < sqlb::StringVector > ());
												fk->setConstraint(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > () = fk;
											}
#line 2947 "sqlite3_parser.cpp"
    break;

  case 235: // columnconstraint: optional_constraintname optional_always_generated "AS" "(" expr ")" optional_storage_identifier
#line 736 "sqlite3_parser.yy"
                                                                                                        {		// TODO Solve shift/reduce conflict.
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::GeneratedColumnConstraint>(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[6].value.as < std::string > ());
											}
#line 2956 "sqlite3_parser.cpp"
    break;

  case 236: // columnconstraint_list: columnconstraint
#line 743 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::vector<sqlb::ConstraintPtr> > () = { yystack_[0].value.as < sqlb::ConstraintPtr > () }; }
#line 2962 "sqlite3_parser.cpp"
    break;

  case 237: // columnconstraint_list: columnconstraint_list columnconstraint
#line 744 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::vector<sqlb::ConstraintPtr> > () = yystack_[1].value.as < std::vector<sqlb::ConstraintPtr> > (); yylhs.value.as < std::vector<sqlb::ConstraintPtr> > ().push_back(yystack_[0].value.as < sqlb::ConstraintPtr > ()); }
#line 2968 "sqlite3_parser.cpp"
    break;

  case 238: // columndef: columnid optional_typename columnconstraint_list
#line 748 "sqlite3_parser.yy"
                                                                {
								sqlb::Field f(yystack_[2].value.as < std::string > (), yystack_[1].value.as < std::string > ());
								Constraints table_constraints;
								for(const auto& c : yystack_[0].value.as < std::vector<sqlb::ConstraintPtr> > ())
								{
									if(!c)
										continue;

									switch(c->type())
									{
									// Primary key and foreign key constraints are converted to table constraints
									// because we cannot store them as column constraints at the moment.
									case sqlb::Constraint::PrimaryKeyConstraintType:
										table_constraints.index.insert(std::make_pair(sqlb::IndexedColumnVector{sqlb::IndexedColumn(yystack_[2].value.as < std::string > (), false)}, std::dynamic_pointer_cast<sqlb::UniqueConstraint>(c)));
										break;
									case sqlb::Constraint::ForeignKeyConstraintType:
										table_constraints.fk.insert(std::make_pair(sqlb::StringVector{yystack_[2].value.as < std::string > ()}, std::dynamic_pointer_cast<sqlb::ForeignKeyClause>(c)));
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

								yylhs.value.as < ColumndefData > () = std::make_tuple(f, table_constraints);
							}
#line 3028 "sqlite3_parser.cpp"
    break;

  case 239: // columndef: columnid optional_typename
#line 803 "sqlite3_parser.yy"
                                                        { yylhs.value.as < ColumndefData > () = std::make_tuple(sqlb::Field(yystack_[1].value.as < std::string > (), yystack_[0].value.as < std::string > ()), Constraints{}); }
#line 3034 "sqlite3_parser.cpp"
    break;

  case 240: // columndef_list: columndef
#line 807 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::vector<ColumndefData> > () = {yystack_[0].value.as < ColumndefData > ()}; }
#line 3040 "sqlite3_parser.cpp"
    break;

  case 241: // columndef_list: columndef_list "," columndef
#line 808 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::vector<ColumndefData> > () = yystack_[2].value.as < std::vector<ColumndefData> > (); yylhs.value.as < std::vector<ColumndefData> > ().push_back(yystack_[0].value.as < ColumndefData > ()); }
#line 3046 "sqlite3_parser.cpp"
    break;

  case 242: // optional_constraintname: %empty
#line 812 "sqlite3_parser.yy"
                                                        { }
#line 3052 "sqlite3_parser.cpp"
    break;

  case 243: // optional_constraintname: "CONSTRAINT" id
#line 813 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3058 "sqlite3_parser.cpp"
    break;

  case 244: // optional_constraintname: "CONSTRAINT" "string literal"
#line 814 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3064 "sqlite3_parser.cpp"
    break;

  case 245: // columnid_list: columnid
#line 818 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = sqlb::StringVector(1, yystack_[0].value.as < std::string > ()); }
#line 3070 "sqlite3_parser.cpp"
    break;

  case 246: // columnid_list: columnid_list "," columnid
#line 819 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = yystack_[2].value.as < sqlb::StringVector > (); yylhs.value.as < sqlb::StringVector > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 3076 "sqlite3_parser.cpp"
    break;

  case 247: // optional_columnid_with_paren_list: %empty
#line 823 "sqlite3_parser.yy"
                                                        { }
#line 3082 "sqlite3_parser.cpp"
    break;

  case 248: // optional_columnid_with_paren_list: "(" columnid_list ")"
#line 824 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = yystack_[1].value.as < sqlb::StringVector > (); }
#line 3088 "sqlite3_parser.cpp"
    break;

  case 249: // fk_clause_part: "ON" "DELETE" "SET" "NULL"
#line 828 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3094 "sqlite3_parser.cpp"
    break;

  case 250: // fk_clause_part: "ON" "DELETE" "SET" "DEFAULT"
#line 829 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3100 "sqlite3_parser.cpp"
    break;

  case 251: // fk_clause_part: "ON" "DELETE" "CASCADE"
#line 830 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3106 "sqlite3_parser.cpp"
    break;

  case 252: // fk_clause_part: "ON" "DELETE" "RESTRICT"
#line 831 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3112 "sqlite3_parser.cpp"
    break;

  case 253: // fk_clause_part: "ON" "DELETE" "NO" "ACTION"
#line 832 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3118 "sqlite3_parser.cpp"
    break;

  case 254: // fk_clause_part: "ON" "UPDATE" "SET" "NULL"
#line 833 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3124 "sqlite3_parser.cpp"
    break;

  case 255: // fk_clause_part: "ON" "UPDATE" "SET" "DEFAULT"
#line 834 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3130 "sqlite3_parser.cpp"
    break;

  case 256: // fk_clause_part: "ON" "UPDATE" "CASCADE"
#line 835 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3136 "sqlite3_parser.cpp"
    break;

  case 257: // fk_clause_part: "ON" "UPDATE" "RESTRICT"
#line 836 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3142 "sqlite3_parser.cpp"
    break;

  case 258: // fk_clause_part: "ON" "UPDATE" "NO" "ACTION"
#line 837 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3148 "sqlite3_parser.cpp"
    break;

  case 259: // fk_clause_part: "ON" "INSERT" "SET" "NULL"
#line 838 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3154 "sqlite3_parser.cpp"
    break;

  case 260: // fk_clause_part: "ON" "INSERT" "SET" "DEFAULT"
#line 839 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3160 "sqlite3_parser.cpp"
    break;

  case 261: // fk_clause_part: "ON" "INSERT" "CASCADE"
#line 840 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3166 "sqlite3_parser.cpp"
    break;

  case 262: // fk_clause_part: "ON" "INSERT" "RESTRICT"
#line 841 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3172 "sqlite3_parser.cpp"
    break;

  case 263: // fk_clause_part: "ON" "INSERT" "NO" "ACTION"
#line 842 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3178 "sqlite3_parser.cpp"
    break;

  case 264: // fk_clause_part: "MATCH" id
#line 843 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3184 "sqlite3_parser.cpp"
    break;

  case 265: // fk_clause_part_list: fk_clause_part
#line 847 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3190 "sqlite3_parser.cpp"
    break;

  case 266: // fk_clause_part_list: fk_clause_part_list fk_clause_part
#line 848 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3196 "sqlite3_parser.cpp"
    break;

  case 267: // optional_fk_clause: %empty
#line 852 "sqlite3_parser.yy"
                                                                        { }
#line 3202 "sqlite3_parser.cpp"
    break;

  case 268: // optional_fk_clause: fk_clause_part_list
#line 853 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3208 "sqlite3_parser.cpp"
    break;

  case 269: // optional_fk_clause: fk_clause_part_list "DEFERRABLE" "INITIALLY" "DEFERRED"
#line 854 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3214 "sqlite3_parser.cpp"
    break;

  case 270: // optional_fk_clause: fk_clause_part_list "DEFERRABLE" "INITIALLY" "IMMEDIATE"
#line 855 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3220 "sqlite3_parser.cpp"
    break;

  case 271: // optional_fk_clause: fk_clause_part_list "DEFERRABLE"
#line 856 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3226 "sqlite3_parser.cpp"
    break;

  case 272: // optional_fk_clause: fk_clause_part_list "NOT" "DEFERRABLE" "INITIALLY" "DEFERRED"
#line 857 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3232 "sqlite3_parser.cpp"
    break;

  case 273: // optional_fk_clause: fk_clause_part_list "NOT" "DEFERRABLE" "INITIALLY" "IMMEDIATE"
#line 858 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3238 "sqlite3_parser.cpp"
    break;

  case 274: // optional_fk_clause: fk_clause_part_list "NOT" "DEFERRABLE"
#line 859 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3244 "sqlite3_parser.cpp"
    break;

  case 275: // optional_fk_clause: "DEFERRABLE" "INITIALLY" "DEFERRED"
#line 860 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3250 "sqlite3_parser.cpp"
    break;

  case 276: // optional_fk_clause: "DEFERRABLE" "INITIALLY" "IMMEDIATE"
#line 861 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3256 "sqlite3_parser.cpp"
    break;

  case 277: // optional_fk_clause: "DEFERRABLE"
#line 862 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3262 "sqlite3_parser.cpp"
    break;

  case 278: // optional_fk_clause: "NOT" "DEFERRABLE" "INITIALLY" "DEFERRED"
#line 863 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3268 "sqlite3_parser.cpp"
    break;

  case 279: // optional_fk_clause: "NOT" "DEFERRABLE" "INITIALLY" "IMMEDIATE"
#line 864 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3274 "sqlite3_parser.cpp"
    break;

  case 280: // optional_fk_clause: "NOT" "DEFERRABLE"
#line 865 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3280 "sqlite3_parser.cpp"
    break;

  case 281: // tableconstraint: optional_constraintname "PRIMARY" "KEY" "(" indexed_column_list ")" optional_conflictclause
#line 869 "sqlite3_parser.yy"
                                                                                                        {
														auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>();
														pk->setName(yystack_[6].value.as < std::string > ());
														pk->setConflictAction(yystack_[0].value.as < std::string > ());
														yylhs.value.as < Constraints > ().index.insert(std::make_pair(yystack_[2].value.as < sqlb::IndexedColumnVector > (), pk));
													}
#line 3291 "sqlite3_parser.cpp"
    break;

  case 282: // tableconstraint: optional_constraintname "PRIMARY" "KEY" "(" indexed_column_list "AUTOINCREMENT" ")" optional_conflictclause
#line 875 "sqlite3_parser.yy"
                                                                                                                {
														auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>();
														pk->setName(yystack_[7].value.as < std::string > ());
														pk->setConflictAction(yystack_[0].value.as < std::string > ());
														pk->setAutoIncrement(true);
														yylhs.value.as < Constraints > ().index.insert(std::make_pair(yystack_[3].value.as < sqlb::IndexedColumnVector > (), pk));
													}
#line 3303 "sqlite3_parser.cpp"
    break;

  case 283: // tableconstraint: optional_constraintname "UNIQUE" "(" indexed_column_list ")" optional_conflictclause
#line 882 "sqlite3_parser.yy"
                                                                                                        {
														auto u = std::make_shared<sqlb::UniqueConstraint>();
														u->setName(yystack_[5].value.as < std::string > ());
														u->setConflictAction(yystack_[0].value.as < std::string > ());
														sqlb::StringVector columns;
														yylhs.value.as < Constraints > ().index.insert(std::make_pair(yystack_[2].value.as < sqlb::IndexedColumnVector > (), u));
													}
#line 3315 "sqlite3_parser.cpp"
    break;

  case 284: // tableconstraint: optional_constraintname "CHECK" "(" expr ")"
#line 889 "sqlite3_parser.yy"
                                                                                                        {
														auto c = std::make_shared<sqlb::CheckConstraint>(yystack_[1].value.as < std::string > ());
														c->setName(yystack_[4].value.as < std::string > ());
														yylhs.value.as < Constraints > ().check.push_back(c);
													}
#line 3325 "sqlite3_parser.cpp"
    break;

  case 285: // tableconstraint: optional_constraintname "FOREIGN" "KEY" "(" columnid_list ")" "REFERENCES" tableid optional_columnid_with_paren_list optional_fk_clause
#line 894 "sqlite3_parser.yy"
                                                                                                                                                {
														auto f = std::make_shared<sqlb::ForeignKeyClause>(yystack_[2].value.as < std::string > (), yystack_[1].value.as < sqlb::StringVector > (), yystack_[0].value.as < std::string > ());
														f->setName(yystack_[9].value.as < std::string > ());
														yylhs.value.as < Constraints > ().fk.insert(std::make_pair(yystack_[5].value.as < sqlb::StringVector > (), f));
													}
#line 3335 "sqlite3_parser.cpp"
    break;

  case 286: // tableconstraint_list: tableconstraint
#line 902 "sqlite3_parser.yy"
                                                        { yylhs.value.as < Constraints > () = yystack_[0].value.as < Constraints > (); }
#line 3341 "sqlite3_parser.cpp"
    break;

  case 287: // tableconstraint_list: tableconstraint_list "," tableconstraint
#line 903 "sqlite3_parser.yy"
                                                        { yylhs.value.as < Constraints > () = yystack_[2].value.as < Constraints > (); yylhs.value.as < Constraints > ().index.insert(yystack_[0].value.as < Constraints > ().index.begin(), yystack_[0].value.as < Constraints > ().index.end()); yylhs.value.as < Constraints > ().fk.insert(yystack_[0].value.as < Constraints > ().fk.begin(), yystack_[0].value.as < Constraints > ().fk.end()); std::copy(yystack_[0].value.as < Constraints > ().check.begin(), yystack_[0].value.as < Constraints > ().check.end(), std::back_inserter(yylhs.value.as < Constraints > ().check)); }
#line 3347 "sqlite3_parser.cpp"
    break;

  case 288: // tableconstraint_list: tableconstraint_list tableconstraint
#line 904 "sqlite3_parser.yy"
                                                        { yylhs.value.as < Constraints > () = yystack_[1].value.as < Constraints > (); yylhs.value.as < Constraints > ().index.insert(yystack_[0].value.as < Constraints > ().index.begin(), yystack_[0].value.as < Constraints > ().index.end()); yylhs.value.as < Constraints > ().fk.insert(yystack_[0].value.as < Constraints > ().fk.begin(), yystack_[0].value.as < Constraints > ().fk.end()); std::copy(yystack_[0].value.as < Constraints > ().check.begin(), yystack_[0].value.as < Constraints > ().check.end(), std::back_inserter(yylhs.value.as < Constraints > ().check)); }
#line 3353 "sqlite3_parser.cpp"
    break;

  case 289: // optional_tableconstraint_list: %empty
#line 908 "sqlite3_parser.yy"
                                                        { }
#line 3359 "sqlite3_parser.cpp"
    break;

  case 290: // optional_tableconstraint_list: "," tableconstraint_list
#line 909 "sqlite3_parser.yy"
                                                        { yylhs.value.as < Constraints > () = yystack_[0].value.as < Constraints > (); }
#line 3365 "sqlite3_parser.cpp"
    break;

  case 291: // createtable_stmt: "CREATE" optional_temporary "TABLE" optional_if_not_exists tableid_with_uninteresting_schema "AS" select_stmt
#line 913 "sqlite3_parser.yy"
                                                                                                                        {
										yylhs.value.as < sqlb::TablePtr > () = std::make_shared<sqlb::Table>(yystack_[2].value.as < std::string > ());
										yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
									}
#line 3374 "sqlite3_parser.cpp"
    break;

  case 292: // createtable_stmt: "CREATE" optional_temporary "TABLE" optional_if_not_exists tableid_with_uninteresting_schema "(" columndef_list optional_tableconstraint_list ")" optional_tableoptions_list
#line 917 "sqlite3_parser.yy"
                                                                                                                                                                                                {
										yylhs.value.as < sqlb::TablePtr > () = std::make_shared<sqlb::Table>(yystack_[5].value.as < std::string > ());
										yylhs.value.as < sqlb::TablePtr > ()->setWithoutRowidTable(yystack_[0].value.as < std::bitset<sqlb::Table::NumOptions> > ().test(sqlb::Table::WithoutRowid));
										yylhs.value.as < sqlb::TablePtr > ()->setStrict(yystack_[0].value.as < std::bitset<sqlb::Table::NumOptions> > ().test(sqlb::Table::Strict));
										for(const auto& i : yystack_[2].value.as < Constraints > ().index)
											yylhs.value.as < sqlb::TablePtr > ()->addConstraint(i.first, i.second);
										for(const auto& i : yystack_[2].value.as < Constraints > ().fk)
											yylhs.value.as < sqlb::TablePtr > ()->addConstraint(i.first, i.second);
										for(const auto& i : yystack_[2].value.as < Constraints > ().check)
											yylhs.value.as < sqlb::TablePtr > ()->addConstraint(i);
										yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(true);

										for(const auto& column : yystack_[3].value.as < std::vector<ColumndefData> > ())
										{
											sqlb::Field f;
											Constraints c;
											std::tie(f, c) = column;

											yylhs.value.as < sqlb::TablePtr > ()->fields.push_back(f);
											for(const auto& i : c.index)
												yylhs.value.as < sqlb::TablePtr > ()->addConstraint(i.first, i.second);
											for(const auto& i : c.fk)
												yylhs.value.as < sqlb::TablePtr > ()->addConstraint(i.first, i.second);
											for(const auto& i : c.check)
												yylhs.value.as < sqlb::TablePtr > ()->addConstraint(i);
										}
									}
#line 3406 "sqlite3_parser.cpp"
    break;


#line 3410 "sqlite3_parser.cpp"

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

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
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


  const short parser::yypact_ninf_ = -344;

  const short parser::yytable_ninf_ = -291;

  const short
  parser::yypact_[] =
  {
     112,   152,   110,    23,  -344,  -344,  -344,  -344,  -344,  -344,
      33,   103,    75,  -344,  -344,   115,   115,   115,   109,  2224,
    2224,  2224,   136,  -344,  -344,  -344,  -344,  -344,  -344,  -344,
    -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,
    -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,
    -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,
    -344,  -344,   201,  -344,  -344,  -344,  -344,  -344,  -344,  -344,
     211,  -344,  -344,   135,   137,    44,  -344,  2314,  2314,   128,
    2314,  2134,   150,  -344,  -344,  -344,  -344,   248,   264,  -344,
    -344,  -344,  -344,  -344,  -344,  -344,  2314,  -344,   253,  -344,
    -344,   930,  -344,  1394,  -344,  1603,  -344,    46,  2044,   271,
    1394,  -344,  1394,  1394,  1394,  1046,   276,  -344,  -344,  -344,
    -344,  1394,  -344,   280,  -344,  -344,  -344,  -344,  -344,  -344,
      26,  -344,  -344,  -344,  -344,   144,  -344,  -344,  -344,  -344,
    -344,  -344,  3209,  2747,  -344,   206,    -5,  -344,   -80,  -344,
      99,    34,  -344,    -7,  -344,   126,   -78,   243,  -344,  -344,
    -344,  1394,   -30,   210,  1394,   473,    -2,   582,   128,  -344,
    1394,  1394,  1394,  1394,  1394,  1394,  1394,  1394,  1394,  1394,
    1394,  1394,  1394,  1394,  1394,  1394,  1394,  1394,  1394,  1394,
    1394,   128,  1394,  1696,  1394,  -344,  1394,  1394,    49,  -344,
    1394,  1394,  -344,  -344,  -344,   175,  1394,   183,   199,  -344,
    -344,   256,  -344,  -344,  -344,   312,   128,  1510,   260,   225,
    -344,   244,  2314,   236,   287,   315,   250,   252,   323,   293,
    -344,  -344,   234,  -344,     5,  -344,  -344,  1898,  1394,  -344,
    1394,    -9,  2824,   330,   331,   325,   332,  -344,   337,  1394,
     259,   338,  3209,   197,   197,    11,    11,    32,    11,    32,
     302,   174,   174,   151,   151,   151,   151,   174,   174,    32,
      32,   473,  2901,  -344,   420,   698,   263,  -344,   174,  1926,
    2028,  1394,  1394,  1789,  1394,  1394,  -344,  1394,  3286,  2452,
    1394,  -344,  -344,  -344,  -344,  -344,    -5,  1394,  -344,  1394,
    -344,  -344,  -344,  -344,  -344,  -344,   236,    69,   343,   306,
    -344,   345,  1394,   346,   348,  1394,  -344,  -344,   -78,  -344,
    1394,  2978,  1989,  1394,  -344,  2314,   237,   238,  -344,   239,
    -344,   265,  -344,   128,  1394,  1394,  -344,   270,   351,  1394,
    2314,  1394,  1394,  3055,  2534,   814,   275,  -344,  2616,  2698,
    2775,  1394,  3209,   352,  2424,  2506,  -344,   236,  2134,    59,
      -8,  1394,  2588,  2134,  1394,   286,  -344,  3209,  -344,  1394,
    3132,   353,   354,   355,   356,  -344,  -344,   174,   174,  -344,
    -344,   289,   359,  -344,   174,   174,  1394,  1394,  -344,   292,
     360,  1394,  2314,  1394,  1394,  1394,   174,  -344,  -344,  -344,
     329,  -344,   296,   298,   128,   317,   -32,  -344,    67,  -344,
    -344,  -344,  -344,  -344,  -344,  2670,  -344,   297,   139,   236,
    3209,  -344,  -344,  -344,  -344,  -344,  -344,  1162,   174,   174,
    -344,  -344,   301,   363,  -344,   174,   174,   174,  -344,  -344,
    2134,   104,  -344,   299,     3,    17,    30,   300,   322,  -344,
      37,   283,   236,   368,  -344,  -344,   304,  -344,  1278,  -344,
    -344,  -344,   156,  -344,   347,  -344,    79,  -344,   349,  -344,
      84,  -344,   350,  -344,    90,   189,   307,  -344,  -344,  -344,
    2314,  -344,   236,  -344,  -344,   305,  -344,  -344,  -344,  -344,
    -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,  -344,   190,
     343,  -344,  -344,  -344,  -344,    59,  -344
  };

  const short
  parser::yydefact_[] =
  {
       0,   183,     0,     2,     4,     5,     6,   199,   200,   184,
       0,     0,     0,     1,     3,   178,   178,   178,     0,     0,
       0,     0,     0,    18,    19,    20,    21,    22,    23,    24,
      61,    60,    62,    25,    26,    27,    28,    29,    30,    31,
      32,    35,    37,    36,    33,    34,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    55,    54,    56,    57,    58,    16,    64,    17,
      63,    59,   189,     0,     0,     0,   179,     0,     0,     0,
       0,     0,     0,    55,    63,   188,   187,   194,     0,    67,
      66,    68,    69,    71,    70,    65,   214,   240,   289,   177,
     291,     0,   197,     0,    76,    78,   215,   242,   242,     0,
       0,   195,     0,     0,     0,     0,    23,    14,    13,    15,
      12,     0,    10,    43,    11,     7,     8,     9,   161,   159,
     164,   160,   165,   166,   171,     0,   167,   172,   173,   174,
     175,   176,   122,   180,   191,     0,     0,    77,     0,   236,
     242,   219,   241,     0,   286,   242,   206,     0,    82,    81,
      83,     0,     0,     0,     0,    84,     0,     0,     0,   196,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   128,     0,     0,     0,   129,
       0,     0,   181,   182,   190,   185,     0,     0,     0,    75,
      74,     0,   244,   243,   237,     0,     0,     0,     0,     0,
     224,     0,     0,   208,     0,     0,     0,     0,     0,   242,
     288,   202,     0,   203,   207,   292,   168,     0,     0,   154,
       0,     0,     0,     0,     0,     0,     0,   126,     0,     0,
       0,   163,   123,    89,    90,    86,    87,    93,    88,    94,
      85,    99,   100,    97,    98,    95,    96,   101,   102,    91,
      92,   104,     0,   170,   107,     0,    63,   137,   103,   106,
     108,     0,     0,     0,     0,     0,   130,     0,   105,   109,
       0,   193,   192,    72,    73,    79,     0,     0,   233,     0,
     231,   228,   229,   230,   227,   220,   208,   180,   247,     0,
     225,     0,     0,     0,     0,     0,   287,   201,     0,   205,
       0,     0,     0,     0,   152,     0,     0,     0,   155,     0,
     127,     0,   124,     0,     0,     0,   133,     0,     0,     0,
       0,     0,     0,     0,   111,     0,    63,   145,   110,   112,
     113,     0,   186,     0,     0,     0,   223,   208,     0,   267,
       0,     0,     0,     0,     0,     0,   204,   149,   153,     0,
       0,     0,     0,     0,     0,   125,   162,   131,   115,   135,
     134,     0,    63,   136,   114,   116,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,   117,    80,   226,   232,
     221,   245,     0,   277,     0,     0,     0,   265,   268,   234,
     210,   211,   212,   213,   209,     0,   284,     0,     0,   208,
     150,   151,   169,   157,   158,   156,   140,     0,   132,   119,
     143,   142,     0,    63,   144,   118,   120,   121,   222,   248,
       0,     0,   264,   280,     0,     0,     0,   271,     0,   266,
     216,     0,   208,     0,   283,   138,     0,   148,     0,   246,
     275,   276,     0,   251,     0,   252,     0,   261,     0,   262,
       0,   256,     0,   257,     0,     0,   274,   217,   218,   235,
       0,   281,   208,   139,   146,     0,   278,   279,   253,   250,
     249,   263,   260,   259,   258,   255,   254,   269,   270,     0,
     247,   282,   147,   272,   273,   267,   285
  };

  const short
  parser::yypgoto_[] =
  {
    -344,  -344,  -344,   157,   -19,   -13,   -68,  -343,   164,    86,
    -344,    61,  -344,  -344,  -344,   -97,  -344,  -344,  -344,  -344,
     221,  -344,  -344,   219,  -258,   140,    80,  -344,  -344,   181,
     182,  -293,  -344,  -344,  -344,  -344,  -220,  -344,  -344,  -301,
    -344,  -344,  -344,   267,  -344,   310,  -344,   -81,    47,   -86,
      14,  -344,   -82,  -139,  -344,  -344,  -344
  };

  const short
  parser::yydefgoto_[] =
  {
       0,     2,     3,   129,   130,   131,    72,    96,   210,   211,
     105,   106,   132,   133,   134,   135,   136,   137,   138,   139,
     162,   140,   141,   142,   100,    19,   204,    11,   291,    73,
     144,   145,     4,   102,     5,    12,   233,   234,   235,   310,
     107,   479,   224,   149,   150,    97,    98,   153,   402,   359,
     407,   408,   409,   154,   155,   109,     6
  };

  const short
  parser::yytable_[] =
  {
      70,    70,    70,   207,   208,   356,    71,    71,    71,    85,
      86,   318,    88,   157,   319,   401,   230,   338,   444,   410,
     401,   231,   365,   238,   239,   243,   151,   178,   104,   167,
      14,   168,   225,    67,   232,   212,    69,   147,   445,   463,
     171,   172,   173,   174,   323,   324,   176,    81,   178,   411,
    -239,   191,  -239,   467,   226,   244,   400,   412,    84,    84,
      87,    84,    94,   245,    71,    71,   471,    71,    95,   151,
     250,   418,   191,   215,   216,   446,    82,    84,   227,   464,
     240,   217,   413,    71,   281,   414,    84,   390,   148,    94,
     316,   246,    71,   468,   465,    95,   218,   459,   366,   228,
     466,   240,   202,  -238,   231,  -238,   472,   403,   469,   209,
      13,   219,   282,   220,   470,   447,   283,   232,   454,   221,
     203,   473,   222,   284,   285,   277,   489,   474,   286,   213,
    -290,   492,   229,    15,   404,   477,   405,   495,   287,   406,
     223,   148,   404,   452,   448,   206,   478,   406,   169,   251,
     170,   481,   331,   460,   308,     1,    20,    21,   490,   171,
     172,   173,   174,   493,   175,   176,   177,   178,   148,   496,
     461,    16,   273,   453,   276,    17,   187,   188,   337,    18,
      71,   501,   171,   172,   173,   174,    22,   175,   176,   177,
     178,   191,    76,   181,   182,   183,   184,   298,   302,   187,
     188,    74,    75,    84,   303,   486,    77,   173,   174,    71,
     205,   176,   206,   178,   191,   347,    78,    80,   171,   172,
     173,   174,   487,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   191,   497,   503,
     189,    67,   381,    79,    69,   190,    99,   236,   389,   170,
     191,   101,  -198,     7,     8,   498,   504,   104,     9,   108,
     295,    10,   296,   332,   346,   170,   339,   103,   340,   375,
      71,   170,   383,   192,   379,   156,   170,   193,   391,   164,
     392,   194,   195,   166,   196,   197,   290,   198,   199,   305,
     419,   200,   206,   426,   432,   170,   430,   293,   170,   201,
     439,   451,   440,   440,   306,   457,    84,   170,   483,   502,
     170,   170,    71,   294,   376,   297,   309,   307,   312,   311,
     161,   382,   143,   313,   434,   314,   315,    71,   317,   328,
     456,   158,   159,   160,   163,   148,   326,   327,   329,    94,
     165,   330,   191,   333,    94,    95,   358,   360,   361,   363,
      95,   364,   372,   373,   374,   380,   397,   422,   423,   424,
     425,   485,   427,   438,   431,   443,   458,   441,   462,   475,
     476,   480,   482,   433,   301,   488,   499,   491,   494,    71,
     237,   304,   353,   242,   241,   442,   371,   357,   292,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     417,   274,   500,   278,   505,   279,   280,   214,   152,   288,
     289,    94,   449,   506,     0,   143,     0,    95,   171,   172,
     173,   174,     0,   175,   176,   177,   178,     0,     0,   181,
     182,   183,   184,     0,     0,   187,   188,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   321,     0,   322,
     191,    84,     0,     0,     0,     0,     0,    71,     0,     0,
       0,     0,     0,     0,     0,   335,     0,     0,     0,     0,
       0,   171,   172,   173,   174,     0,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     343,   344,     0,   348,   349,     0,   350,     0,   190,   352,
       0,     0,     0,   191,     0,     0,   354,     0,   355,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   362,     0,     0,   143,     0,   192,     0,     0,   367,
     193,     0,   370,     0,   194,   195,     0,   196,   197,     0,
     198,   199,     0,   377,   378,     0,     0,     0,     0,     0,
     384,   385,   201,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     415,     0,     0,   143,     0,   110,   247,     0,   420,     0,
     112,   113,   248,     0,   114,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   428,   429,     0,     0,    23,
      24,    25,   435,   436,   437,    26,     0,     0,    27,   115,
     116,     0,     0,    29,     0,     0,   117,   118,   119,     0,
       0,    33,     0,    34,   249,     0,    35,     0,     0,    36,
     120,    37,    38,     0,    39,    40,     0,    41,    42,     0,
       0,    43,     0,     0,     0,    44,    45,    46,    47,   121,
       0,   122,     0,     0,    48,    49,    50,     0,   123,    52,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,    61,     0,    83,    63,     0,   124,    64,     0,     0,
       0,    65,     0,     0,    66,    67,   125,   126,    69,   127,
     128,   110,   336,     0,     0,     0,   112,   113,     0,     0,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,    24,    25,     0,     0,
       0,    26,     0,     0,    27,   115,   116,     0,     0,    29,
       0,     0,   117,   118,   119,     0,     0,    33,     0,    34,
       0,     0,    35,     0,     0,    36,   120,    37,    38,     0,
      39,    40,     0,    41,    42,     0,     0,    43,     0,     0,
       0,    44,    45,    46,    47,   121,     0,   122,     0,     0,
      48,    49,    50,     0,   123,    52,     0,    53,    54,    55,
      56,    57,    58,    59,    99,     0,    60,    61,     0,    83,
      63,     0,   124,    64,     0,     0,     0,    65,     0,     0,
      66,    67,   125,   126,    69,   127,   128,   110,   388,     0,
       0,     0,   112,   113,     0,     0,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    23,    24,    25,     0,     0,     0,    26,     0,     0,
      27,   115,   116,     0,     0,    29,     0,     0,   117,   118,
     119,     0,     0,    33,     0,    34,     0,     0,    35,     0,
       0,    36,   120,    37,    38,     0,    39,    40,     0,    41,
      42,     0,     0,    43,     0,     0,     0,    44,    45,    46,
      47,   121,     0,   122,     0,     0,    48,    49,    50,     0,
     123,    52,     0,    53,    54,    55,    56,    57,    58,    59,
      99,     0,    60,    61,     0,    83,    63,     0,   124,    64,
       0,     0,     0,    65,     0,     0,    66,    67,   125,   126,
      69,   127,   128,   110,   111,     0,     0,     0,   112,   113,
       0,     0,   114,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    23,    24,    25,
       0,     0,     0,    26,     0,     0,    27,   115,   116,     0,
       0,    29,     0,     0,   117,   118,   119,     0,     0,    33,
       0,    34,     0,     0,    35,     0,     0,    36,   120,    37,
      38,     0,    39,    40,     0,    41,    42,     0,     0,    43,
       0,     0,     0,    44,    45,    46,    47,   121,     0,   122,
       0,     0,    48,    49,    50,     0,   123,    52,     0,    53,
      54,    55,    56,    57,    58,    59,     0,     0,    60,    61,
       0,    83,    63,     0,   124,    64,     0,     0,     0,    65,
       0,     0,    66,    67,   125,   126,    69,   127,   128,   110,
       0,     0,     0,     0,   112,   113,     0,     0,   114,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,    24,    25,     0,     0,     0,    26,
       0,     0,    27,   115,   116,     0,     0,    29,     0,     0,
     117,   118,   119,     0,     0,    33,     0,    34,     0,     0,
      35,     0,     0,    36,   120,    37,    38,     0,    39,    40,
       0,    41,    42,     0,     0,    43,     0,     0,     0,    44,
      45,    46,    47,   121,     0,   122,     0,     0,    48,    49,
      50,     0,   123,    52,     0,    53,    54,    55,    56,    57,
      58,    59,     0,     0,    60,    61,     0,    83,    63,     0,
     124,    64,     0,     0,     0,    65,   161,     0,    66,    67,
     125,   126,    69,   127,   128,   110,   455,     0,     0,     0,
     112,   113,     0,     0,   114,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,     0,     0,     0,    26,     0,     0,    27,   115,
     116,     0,     0,    29,     0,     0,   117,   118,   119,     0,
       0,    33,     0,    34,     0,     0,    35,     0,     0,    36,
     120,    37,    38,     0,    39,    40,     0,    41,    42,     0,
       0,    43,     0,     0,     0,    44,    45,    46,    47,   121,
       0,   122,     0,     0,    48,    49,    50,     0,   123,    52,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,    61,     0,    83,    63,     0,   124,    64,     0,     0,
       0,    65,     0,     0,    66,    67,   125,   126,    69,   127,
     128,   110,   484,     0,     0,     0,   112,   113,     0,     0,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,    24,    25,     0,     0,
       0,    26,     0,     0,    27,   115,   116,     0,     0,    29,
       0,     0,   117,   118,   119,     0,     0,    33,     0,    34,
       0,     0,    35,     0,     0,    36,   120,    37,    38,     0,
      39,    40,     0,    41,    42,     0,     0,    43,     0,     0,
       0,    44,    45,    46,    47,   121,     0,   122,     0,     0,
      48,    49,    50,     0,   123,    52,     0,    53,    54,    55,
      56,    57,    58,    59,     0,     0,    60,    61,     0,    83,
      63,     0,   124,    64,     0,     0,     0,    65,     0,     0,
      66,    67,   125,   126,    69,   127,   128,   110,     0,     0,
       0,     0,   112,   113,     0,     0,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    23,    24,    25,     0,     0,     0,    26,     0,     0,
      27,   115,   116,     0,     0,    29,     0,     0,   117,   118,
     119,     0,     0,    33,     0,    34,     0,     0,    35,     0,
       0,    36,   120,    37,    38,     0,    39,    40,     0,    41,
      42,     0,     0,    43,     0,     0,     0,    44,    45,    46,
      47,   121,     0,   122,     0,     0,    48,    49,    50,     0,
     123,    52,     0,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,    61,     0,    83,    63,     0,   124,    64,
       0,     0,     0,    65,     0,     0,    66,    67,   125,   126,
      69,   127,   128,   299,     0,     0,     0,     0,   207,   208,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    23,    24,    25,
       0,     0,     0,    26,     0,     0,    27,     0,    28,     0,
       0,    29,     0,     0,   117,   118,   119,     0,     0,    33,
       0,    34,     0,     0,    35,     0,     0,    36,   120,    37,
      38,     0,    39,    40,   300,    41,    42,     0,     0,    43,
       0,     0,     0,    44,    45,    46,    47,     0,     0,   122,
       0,     0,    48,    49,    50,     0,    51,    52,     0,    53,
      54,    55,    56,    57,    58,    59,   146,     0,    60,    61,
       0,    83,    63,     0,   124,    64,     0,     0,     0,    65,
       0,     0,    66,    67,   125,   126,    69,   127,     0,     0,
      23,    24,    25,     0,     0,     0,    26,     0,     0,    27,
       0,    28,     0,     0,    29,     0,     0,    30,    31,    32,
       0,     0,    33,     0,    34,     0,     0,    35,     0,     0,
      36,     0,    37,    38,     0,    39,    40,     0,    41,    42,
       0,     0,    43,     0,     0,     0,    44,    45,    46,    47,
       0,     0,     0,     0,     0,    48,    49,    50,     0,    51,
      52,     0,    53,    54,    55,    56,    57,    58,    59,   275,
       0,    60,    61,     0,    83,    63,     0,     0,    64,     0,
       0,     0,    65,     0,     0,    66,    67,     0,    68,    69,
       0,     0,     0,    23,    24,    25,     0,     0,     0,    26,
       0,     0,    27,     0,    28,     0,     0,    29,     0,     0,
      30,    31,    32,     0,     0,    33,     0,    34,     0,     0,
      35,     0,     0,    36,     0,    37,    38,     0,    39,    40,
       0,    41,    42,     0,     0,    43,     0,     0,     0,    44,
      45,    46,    47,     0,     0,     0,     0,     0,    48,    49,
      50,     0,    51,    52,     0,    53,    54,    55,    56,    57,
      58,    59,   345,     0,    60,    61,     0,    83,    63,     0,
       0,    64,     0,     0,     0,    65,     0,     0,    66,    67,
       0,    68,    69,     0,     0,     0,    23,    24,    25,     0,
       0,     0,    26,     0,     0,    27,     0,    28,     0,     0,
      29,     0,     0,    30,    31,    32,     0,     0,    33,     0,
      34,     0,     0,    35,     0,     0,    36,     0,    37,    38,
       0,    39,    40,     0,    41,    42,     0,     0,    43,     0,
       0,     0,    44,    45,    46,    47,     0,     0,     0,     0,
       0,    48,    49,    50,     0,    51,    52,     0,    53,    54,
      55,    56,    57,    58,    59,     0,     0,    60,    61,     0,
      83,    63,     0,     0,    64,     0,     0,     0,    65,     0,
       0,    66,    67,     0,    68,    69,   171,   172,   173,   174,
       0,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,     0,     0,     0,   189,     0,
       0,     0,     0,   190,   171,   172,   173,   174,   191,   175,
     176,   177,   178,     0,     0,   181,   182,   183,   184,     0,
       0,   187,   188,     0,     0,     0,     0,     0,     0,     0,
       0,   192,     0,     0,     0,   193,   191,     0,     0,   194,
     195,     0,   196,   197,     0,   198,   199,     0,     0,   200,
       0,   341,     0,     0,     0,     0,     0,   201,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   171,   172,   173,
     174,   320,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,     0,     0,     0,   189,
       0,     0,     0,     0,   190,     0,     0,     0,     0,   191,
       0,     0,     0,     0,     0,     0,   171,   172,   173,   174,
       0,   175,   176,   177,   178,     0,     0,   181,   182,   183,
     184,     0,   192,   187,   188,     0,   193,     0,     0,     0,
     194,   195,     0,   196,   197,     0,   198,   199,   191,     0,
     200,    23,    24,    25,     0,     0,     0,    26,   201,     0,
      27,     0,    28,   342,     0,    29,   148,     0,    89,    90,
      91,     0,   369,    33,     0,    34,     0,     0,    35,     0,
       0,    36,     0,    37,    38,     0,    39,    40,    92,    41,
      42,     0,     0,    43,     0,     0,     0,    44,    45,    46,
      47,     0,     0,     0,     0,     0,    48,    49,    50,     0,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,    61,     0,    83,    63,     0,     0,    64,
       0,     0,     0,    65,     0,     0,    66,    67,     0,    93,
      69,    23,    24,    25,     0,     0,     0,    26,     0,     0,
      27,     0,    28,     0,     0,    29,     0,     0,    89,    90,
      91,     0,     0,    33,     0,    34,     0,     0,    35,     0,
       0,    36,     0,    37,    38,     0,    39,    40,    92,    41,
      42,     0,     0,    43,     0,     0,     0,    44,    45,    46,
      47,     0,     0,     0,     0,     0,    48,    49,    50,     0,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,    61,     0,    83,    63,     0,     0,    64,
       0,     0,     0,    65,     0,     0,    66,    67,     0,    93,
      69,    23,    24,    25,     0,     0,     0,    26,     0,     0,
      27,     0,    28,     0,     0,    29,     0,     0,    30,    31,
      32,     0,     0,    33,     0,    34,     0,     0,    35,     0,
       0,    36,     0,    37,    38,     0,    39,    40,     0,    41,
      42,     0,     0,    43,     0,     0,     0,    44,    45,    46,
      47,     0,     0,     0,     0,     0,    48,    49,    50,     0,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,    61,     0,    62,    63,     0,     0,    64,
       0,     0,     0,    65,     0,     0,    66,    67,     0,    68,
      69,    23,    24,    25,     0,     0,     0,    26,     0,     0,
      27,     0,    28,     0,     0,    29,     0,     0,    30,    31,
      32,     0,     0,    33,     0,    34,     0,     0,    35,     0,
       0,    36,     0,    37,    38,     0,    39,    40,     0,    41,
      42,     0,     0,    43,     0,     0,     0,    44,    45,    46,
      47,     0,     0,     0,     0,     0,    48,    49,    50,     0,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,    61,     0,    83,    63,     0,     0,    64,
       0,     0,     0,    65,     0,     0,    66,    67,   398,    68,
      69,     0,   171,   172,   173,   174,     0,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,     0,     0,     0,   189,     0,     0,     0,     0,   190,
     171,   172,   173,   174,   191,   175,   176,   177,   178,     0,
       0,   181,   182,   183,   184,     0,     0,   187,   188,     0,
       0,     0,     0,     0,     0,     0,     0,   192,     0,     0,
       0,   193,   191,     0,     0,   194,   195,     0,   196,   197,
       0,   198,   199,     0,     0,   200,     0,   351,     0,     0,
     399,     0,     0,   201,   171,   172,   173,   174,     0,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,     0,     0,     0,   189,     0,     0,     0,
       0,   190,   171,   172,   173,   174,   191,   175,   176,   177,
     178,     0,     0,   181,   182,   183,   184,     0,     0,   187,
     188,     0,     0,     0,     0,     0,     0,     0,     0,   192,
       0,     0,     0,   193,   191,     0,     0,   194,   195,     0,
     196,   197,     0,   198,   199,     0,     0,   200,     0,   387,
       0,     0,   416,     0,     0,   201,   171,   172,   173,   174,
       0,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,     0,     0,     0,   189,     0,
       0,     0,     0,   190,   171,   172,   173,   174,   191,   175,
     176,   177,   178,     0,     0,   181,   182,   183,   184,     0,
       0,   187,   188,     0,     0,     0,     0,     0,     0,     0,
       0,   192,     0,     0,     0,   193,   191,     0,     0,   194,
     195,     0,   196,   197,     0,   198,   199,     0,     0,   200,
       0,   393,     0,     0,   450,     0,     0,   201,   171,   172,
     173,   174,     0,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,     0,     0,     0,
     189,     0,     0,     0,     0,   190,   171,   172,   173,   174,
     191,   175,   176,   177,   178,     0,     0,   181,   182,   183,
     184,     0,     0,   187,   188,     0,     0,     0,     0,     0,
       0,     0,     0,   192,     0,     0,     0,   193,   191,     0,
       0,   194,   195,     0,   196,   197,     0,   198,   199,     0,
       0,   200,     0,   394,     0,   171,   172,   173,   174,   201,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,     0,     0,     0,   189,     0,     0,
     202,     0,   190,   171,   172,   173,   174,   191,   175,   176,
     177,   178,     0,     0,   181,   182,   183,   184,   203,     0,
     187,   188,     0,     0,     0,     0,     0,     0,     0,     0,
     192,     0,     0,     0,   193,   191,     0,     0,   194,   195,
       0,   196,   197,     0,   198,   199,     0,     0,   200,     0,
     395,     0,   171,   172,   173,   174,   201,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,     0,     0,     0,   189,     0,   325,     0,     0,   190,
       0,     0,     0,     0,   191,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   192,     0,     0,
       0,   193,     0,     0,     0,   194,   195,     0,   196,   197,
       0,   198,   199,     0,     0,   200,     0,     0,     0,   171,
     172,   173,   174,   201,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,     0,     0,
       0,   189,   334,     0,     0,     0,   190,     0,     0,     0,
       0,   191,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   192,     0,     0,     0,   193,     0,
       0,     0,   194,   195,     0,   196,   197,     0,   198,   199,
       0,     0,   200,     0,     0,     0,   171,   172,   173,   174,
     201,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,     0,     0,     0,   189,     0,
       0,     0,     0,   190,     0,     0,     0,     0,   191,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   368,     0,     0,     0,     0,     0,     0,     0,
       0,   192,     0,     0,     0,   193,     0,     0,     0,   194,
     195,     0,   196,   197,     0,   198,   199,     0,     0,   200,
       0,     0,     0,   171,   172,   173,   174,   201,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,     0,     0,     0,   189,   386,     0,     0,     0,
     190,     0,     0,     0,     0,   191,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   192,     0,
       0,     0,   193,     0,     0,     0,   194,   195,     0,   196,
     197,     0,   198,   199,     0,     0,   200,     0,     0,     0,
     171,   172,   173,   174,   201,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,     0,
       0,     0,   189,     0,     0,     0,     0,   190,     0,     0,
       0,     0,   191,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   421,     0,     0,     0,
       0,     0,     0,     0,     0,   192,     0,     0,     0,   193,
       0,     0,     0,   194,   195,     0,   196,   197,     0,   198,
     199,     0,     0,   200,     0,     0,     0,   171,   172,   173,
     174,   201,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,     0,     0,     0,   189,
       0,     0,     0,     0,   190,     0,     0,     0,     0,   191,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   192,     0,     0,     0,   193,     0,     0,     0,
     194,   195,     0,   196,   197,     0,   198,   199,     0,     0,
     200,     0,     0,     0,   171,   172,   173,   174,   201,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,     0,     0,     0,   189,     0,     0,     0,
       0,   190,     0,     0,     0,     0,   191,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   192,
       0,     0,     0,   193,     0,     0,     0,   194,   195,     0,
     196,   197,     0,   198,   199,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   201
  };

  const short
  parser::yycheck_[] =
  {
      19,    20,    21,     8,     9,   306,    19,    20,    21,    77,
      78,     6,    80,   110,   234,   358,   155,   275,    50,    27,
     363,    99,   315,    53,    54,    27,   107,    16,    96,     3,
       7,     5,    39,   113,   112,   115,   116,   105,    70,    36,
       8,     9,    10,    11,    53,    54,    14,     3,    16,    57,
       4,    40,     6,    36,    61,    57,   357,    65,    77,    78,
      79,    80,    81,    65,    77,    78,    36,    80,    81,   150,
     167,   364,    40,    39,    40,   107,    32,    96,    85,    76,
     110,    47,    90,    96,    35,    93,   105,   345,    42,   108,
     229,    93,   105,    76,    91,   108,    62,   440,   318,   106,
      97,   110,    33,     4,    99,     6,    76,    48,    91,   114,
       0,    77,    63,    79,    97,    48,    67,   112,   419,    85,
      51,    91,    88,    74,    75,   193,    47,    97,    79,   148,
       4,    47,     6,   100,    75,    98,    77,    47,    89,    80,
     106,    42,    75,     4,    77,     6,   109,    80,     4,   168,
       6,   452,   249,    49,   222,    43,    16,    17,    79,     8,
       9,    10,    11,    79,    13,    14,    15,    16,    42,    79,
      66,    68,   191,    34,   193,   100,    25,    26,   275,    64,
     193,   482,     8,     9,    10,    11,    77,    13,    14,    15,
      16,    40,    56,    19,    20,    21,    22,   216,   217,    25,
      26,    20,    21,   222,   217,    49,     5,    10,    11,   222,
       4,    14,     6,    16,    40,   283,     5,    80,     8,     9,
      10,    11,    66,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    40,    49,    49,
      30,   113,   339,   108,   116,    35,    96,     4,   345,     6,
      40,     3,   100,   101,   102,    66,    66,   325,   106,     6,
       4,   109,     6,     4,   283,     6,     3,     3,     5,     4,
     283,     6,   340,    63,     4,     4,     6,    67,     3,     3,
       5,    71,    72,     3,    74,    75,   111,    77,    78,    29,
       4,    81,     6,     4,   391,     6,     4,   114,     6,    89,
       4,     4,     6,     6,    79,     4,   325,     6,     4,     4,
       6,     6,   325,   114,   333,     3,    80,    73,     3,    32,
     110,   340,   103,    73,   392,    73,     3,   340,    94,     4,
     427,   112,   113,   114,   115,    42,     6,     6,     6,   358,
     121,     4,    40,     5,   363,   358,     3,    41,     3,     3,
     363,     3,   115,   115,   115,     4,     4,     4,     4,     4,
       4,   458,     3,    34,     4,    48,     3,    69,    69,    69,
      48,    88,     4,   392,   217,    28,    69,    28,    28,   392,
     161,   217,   296,   164,   163,   404,   325,   307,   206,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     363,   192,   480,   194,   500,   196,   197,   150,   108,   200,
     201,   440,   408,   505,    -1,   206,    -1,   440,     8,     9,
      10,    11,    -1,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    -1,    -1,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   238,    -1,   240,
      40,   480,    -1,    -1,    -1,    -1,    -1,   480,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,     8,     9,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
     281,   282,    -1,   284,   285,    -1,   287,    -1,    35,   290,
      -1,    -1,    -1,    40,    -1,    -1,   297,    -1,   299,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   312,    -1,    -1,   315,    -1,    63,    -1,    -1,   320,
      67,    -1,   323,    -1,    71,    72,    -1,    74,    75,    -1,
      77,    78,    -1,   334,   335,    -1,    -1,    -1,    -1,    -1,
     341,   342,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     351,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     361,    -1,    -1,   364,    -1,     3,     4,    -1,   369,    -1,
       8,     9,    10,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   386,   387,    -1,    -1,    27,
      28,    29,   393,   394,   395,    33,    -1,    -1,    36,    37,
      38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,
      -1,    49,    -1,    51,    52,    -1,    54,    -1,    -1,    57,
      58,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      -1,    79,    -1,    -1,    82,    83,    84,    -1,    86,    87,
      -1,    89,    90,    91,    92,    93,    94,    95,    -1,    -1,
      98,    99,    -1,   101,   102,    -1,   104,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,   113,   114,   115,   116,   117,
     118,     3,     4,    -1,    -1,    -1,     8,     9,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,
      -1,    33,    -1,    -1,    36,    37,    38,    -1,    -1,    41,
      -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    69,    -1,    -1,
      -1,    73,    74,    75,    76,    77,    -1,    79,    -1,    -1,
      82,    83,    84,    -1,    86,    87,    -1,    89,    90,    91,
      92,    93,    94,    95,    96,    -1,    98,    99,    -1,   101,
     102,    -1,   104,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,   113,   114,   115,   116,   117,   118,     3,     4,    -1,
      -1,    -1,     8,     9,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,
      36,    37,    38,    -1,    -1,    41,    -1,    -1,    44,    45,
      46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    77,    -1,    79,    -1,    -1,    82,    83,    84,    -1,
      86,    87,    -1,    89,    90,    91,    92,    93,    94,    95,
      96,    -1,    98,    99,    -1,   101,   102,    -1,   104,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,   113,   114,   115,
     116,   117,   118,     3,     4,    -1,    -1,    -1,     8,     9,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      -1,    -1,    -1,    33,    -1,    -1,    36,    37,    38,    -1,
      -1,    41,    -1,    -1,    44,    45,    46,    -1,    -1,    49,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    69,
      -1,    -1,    -1,    73,    74,    75,    76,    77,    -1,    79,
      -1,    -1,    82,    83,    84,    -1,    86,    87,    -1,    89,
      90,    91,    92,    93,    94,    95,    -1,    -1,    98,    99,
      -1,   101,   102,    -1,   104,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,   113,   114,   115,   116,   117,   118,     3,
      -1,    -1,    -1,    -1,     8,     9,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,    33,
      -1,    -1,    36,    37,    38,    -1,    -1,    41,    -1,    -1,
      44,    45,    46,    -1,    -1,    49,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    59,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    -1,    69,    -1,    -1,    -1,    73,
      74,    75,    76,    77,    -1,    79,    -1,    -1,    82,    83,
      84,    -1,    86,    87,    -1,    89,    90,    91,    92,    93,
      94,    95,    -1,    -1,    98,    99,    -1,   101,   102,    -1,
     104,   105,    -1,    -1,    -1,   109,   110,    -1,   112,   113,
     114,   115,   116,   117,   118,     3,     4,    -1,    -1,    -1,
       8,     9,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,    37,
      38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,
      -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,    77,
      -1,    79,    -1,    -1,    82,    83,    84,    -1,    86,    87,
      -1,    89,    90,    91,    92,    93,    94,    95,    -1,    -1,
      98,    99,    -1,   101,   102,    -1,   104,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,   113,   114,   115,   116,   117,
     118,     3,     4,    -1,    -1,    -1,     8,     9,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,
      -1,    33,    -1,    -1,    36,    37,    38,    -1,    -1,    41,
      -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    69,    -1,    -1,
      -1,    73,    74,    75,    76,    77,    -1,    79,    -1,    -1,
      82,    83,    84,    -1,    86,    87,    -1,    89,    90,    91,
      92,    93,    94,    95,    -1,    -1,    98,    99,    -1,   101,
     102,    -1,   104,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,   113,   114,   115,   116,   117,   118,     3,    -1,    -1,
      -1,    -1,     8,     9,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,
      36,    37,    38,    -1,    -1,    41,    -1,    -1,    44,    45,
      46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    77,    -1,    79,    -1,    -1,    82,    83,    84,    -1,
      86,    87,    -1,    89,    90,    91,    92,    93,    94,    95,
      -1,    -1,    98,    99,    -1,   101,   102,    -1,   104,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,   113,   114,   115,
     116,   117,   118,     3,    -1,    -1,    -1,    -1,     8,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      -1,    -1,    -1,    33,    -1,    -1,    36,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    45,    46,    -1,    -1,    49,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    -1,    -1,    69,
      -1,    -1,    -1,    73,    74,    75,    76,    -1,    -1,    79,
      -1,    -1,    82,    83,    84,    -1,    86,    87,    -1,    89,
      90,    91,    92,    93,    94,    95,     3,    -1,    98,    99,
      -1,   101,   102,    -1,   104,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
      27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,
      -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    -1,    86,
      87,    -1,    89,    90,    91,    92,    93,    94,    95,     3,
      -1,    98,    99,    -1,   101,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,   113,    -1,   115,   116,
      -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,    33,
      -1,    -1,    36,    -1,    38,    -1,    -1,    41,    -1,    -1,
      44,    45,    46,    -1,    -1,    49,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    -1,    69,    -1,    -1,    -1,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    82,    83,
      84,    -1,    86,    87,    -1,    89,    90,    91,    92,    93,
      94,    95,     3,    -1,    98,    99,    -1,   101,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,   113,
      -1,   115,   116,    -1,    -1,    -1,    27,    28,    29,    -1,
      -1,    -1,    33,    -1,    -1,    36,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    69,    -1,
      -1,    -1,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      -1,    82,    83,    84,    -1,    86,    87,    -1,    89,    90,
      91,    92,    93,    94,    95,    -1,    -1,    98,    99,    -1,
     101,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,   113,    -1,   115,   116,     8,     9,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    35,     8,     9,    10,    11,    40,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    67,    40,    -1,    -1,    71,
      72,    -1,    74,    75,    -1,    77,    78,    -1,    -1,    81,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,     9,    10,
      11,   103,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,     8,     9,    10,    11,
      -1,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    -1,    63,    25,    26,    -1,    67,    -1,    -1,    -1,
      71,    72,    -1,    74,    75,    -1,    77,    78,    40,    -1,
      81,    27,    28,    29,    -1,    -1,    -1,    33,    89,    -1,
      36,    -1,    38,    55,    -1,    41,    42,    -1,    44,    45,
      46,    -1,   103,    49,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    -1,    59,    60,    -1,    62,    63,    64,    65,
      66,    -1,    -1,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    -1,
      86,    87,    -1,    89,    90,    91,    92,    93,    94,    95,
      -1,    -1,    98,    99,    -1,   101,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,   113,    -1,   115,
     116,    27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,
      36,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    45,
      46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    -1,    59,    60,    -1,    62,    63,    64,    65,
      66,    -1,    -1,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    -1,
      86,    87,    -1,    89,    90,    91,    92,    93,    94,    95,
      -1,    -1,    98,    99,    -1,   101,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,   113,    -1,   115,
     116,    27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,
      36,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    45,
      46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    -1,
      86,    87,    -1,    89,    90,    91,    92,    93,    94,    95,
      -1,    -1,    98,    99,    -1,   101,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,   113,    -1,   115,
     116,    27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,
      36,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    45,
      46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    -1,
      86,    87,    -1,    89,    90,    91,    92,    93,    94,    95,
      -1,    -1,    98,    99,    -1,   101,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,   113,     4,   115,
     116,    -1,     8,     9,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,
       8,     9,    10,    11,    40,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    -1,    -1,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    67,    40,    -1,    -1,    71,    72,    -1,    74,    75,
      -1,    77,    78,    -1,    -1,    81,    -1,    55,    -1,    -1,
       4,    -1,    -1,    89,     8,     9,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    35,     8,     9,    10,    11,    40,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    67,    40,    -1,    -1,    71,    72,    -1,
      74,    75,    -1,    77,    78,    -1,    -1,    81,    -1,    55,
      -1,    -1,     4,    -1,    -1,    89,     8,     9,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    35,     8,     9,    10,    11,    40,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    67,    40,    -1,    -1,    71,
      72,    -1,    74,    75,    -1,    77,    78,    -1,    -1,    81,
      -1,    55,    -1,    -1,     4,    -1,    -1,    89,     8,     9,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    35,     8,     9,    10,    11,
      40,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    67,    40,    -1,
      -1,    71,    72,    -1,    74,    75,    -1,    77,    78,    -1,
      -1,    81,    -1,    55,    -1,     8,     9,    10,    11,    89,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,     8,     9,    10,    11,    40,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    51,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    67,    40,    -1,    -1,    71,    72,
      -1,    74,    75,    -1,    77,    78,    -1,    -1,    81,    -1,
      55,    -1,     8,     9,    10,    11,    89,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    30,    -1,    32,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      25,    26,    -1,    -1,    -1,    30,    31,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    71,    72,    -1,    74,
      75,    -1,    77,    78,    -1,    -1,    81,    -1,    -1,    -1,
       8,     9,    10,    11,    89,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    67,
      -1,    -1,    -1,    71,    72,    -1,    74,    75,    -1,    77,
      78,    -1,    -1,    81,    -1,    -1,    -1,     8,     9,    10,
      11,    89,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      71,    72,    -1,    74,    75,    -1,    77,    78,    -1,    -1,
      81,    -1,    -1,    -1,     8,     9,    10,    11,    89,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,    -1,
      74,    75,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    43,   120,   121,   151,   153,   175,   101,   102,   106,
     109,   146,   154,     0,     7,   100,    68,   100,    64,   144,
     144,   144,    77,    27,    28,    29,    33,    36,    38,    41,
      44,    45,    46,    49,    51,    54,    57,    59,    60,    62,
      63,    65,    66,    69,    73,    74,    75,    76,    82,    83,
      84,    86,    87,    89,    90,    91,    92,    93,    94,    95,
      98,    99,   101,   102,   105,   109,   112,   113,   115,   116,
     123,   124,   125,   148,   148,   148,    56,     5,     5,   108,
      80,     3,    32,   101,   123,   125,   125,   123,   125,    44,
      45,    46,    64,   115,   123,   124,   126,   164,   165,    96,
     143,     3,   152,     3,   125,   129,   130,   159,     6,   174,
       3,     4,     8,     9,    12,    37,    38,    44,    45,    46,
      58,    77,    79,    86,   104,   114,   115,   117,   118,   122,
     123,   124,   131,   132,   133,   134,   135,   136,   137,   138,
     140,   141,   142,   142,   149,   150,     3,   125,    42,   162,
     163,   166,   164,   166,   172,   173,     4,   134,   142,   142,
     142,   110,   139,   142,     3,   142,     3,     3,     5,     4,
       6,     8,     9,    10,    11,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    30,
      35,    40,    63,    67,    71,    72,    74,    75,    77,    78,
      81,    89,    33,    51,   145,     4,     6,     8,     9,   114,
     127,   128,   115,   123,   162,    39,    40,    47,    62,    77,
      79,    85,    88,   106,   161,    39,    61,    85,   106,     6,
     172,    99,   112,   155,   156,   157,     4,   142,    53,    54,
     110,   139,   142,    27,    57,    65,    93,     4,    10,    52,
     134,   123,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   123,   142,     3,   123,   125,   142,   142,
     142,    35,    63,    67,    74,    75,    79,    89,   142,   142,
     111,   147,   149,   114,   114,     4,     6,     3,   123,     3,
      64,   122,   123,   124,   127,    29,    79,    73,   125,    80,
     158,    32,     3,    73,    73,     3,   172,    94,     6,   155,
     103,   142,   142,    53,    54,    32,     6,     6,     4,     6,
       4,   134,     4,     5,    31,    55,     4,   134,   143,     3,
       5,    55,    55,   142,   142,     3,   123,   125,   142,   142,
     142,    55,   142,   128,   142,   142,   158,   145,     3,   168,
      41,     3,   142,     3,     3,   150,   155,   142,    54,   103,
     142,   130,   115,   115,   115,     4,   123,   142,   142,     4,
       4,   134,   123,   125,   142,   142,    31,    55,     4,   134,
     143,     3,     5,    55,    55,    55,   142,     4,     4,     4,
     158,   126,   167,    48,    75,    77,    80,   169,   170,   171,
      27,    57,    65,    90,    93,   142,     4,   167,   150,     4,
     142,    54,     4,     4,     4,     4,     4,     3,   142,   142,
       4,     4,   134,   123,   125,   142,   142,   142,    34,     4,
       6,    69,   123,    48,    50,    70,   107,    48,    77,   169,
       4,     4,     4,    34,   158,     4,   134,     4,     3,   126,
      49,    66,    69,    36,    76,    91,    97,    36,    76,    91,
      97,    36,    76,    91,    97,    69,    48,    98,   109,   160,
      88,   158,     4,     4,     4,   134,    49,    66,    28,    47,
      79,    28,    47,    79,    28,    47,    79,    49,    66,    69,
     125,   158,     4,    49,    66,   168,   171
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   119,   120,   120,   121,   121,   121,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   123,   123,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   125,
     125,   125,   125,   125,   125,   126,   126,   126,   126,   126,
     126,   126,   127,   127,   128,   128,   129,   129,   130,   130,
     130,   131,   131,   131,   131,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   134,   134,   135,   135,   135,   135,   136,   136,
     136,   137,   137,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   139,
     139,   140,   140,   140,   140,   141,   141,   141,   141,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   143,   144,   144,
     145,   145,   145,   146,   146,   147,   147,   148,   148,   148,
     149,   150,   150,   151,   152,   152,   152,   153,   154,   154,
     154,   155,   155,   156,   156,   156,   157,   157,   158,   158,
     158,   158,   158,   158,   159,   159,   160,   160,   160,   161,
     161,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   163,   163,   164,   164,
     165,   165,   166,   166,   166,   167,   167,   168,   168,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   170,   170,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   172,   172,   172,   172,   172,   173,   173,   173,   174,
     174,   175,   175
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
       1,     1,     2,     2,     1,     1,     1,     2,     1,     4,
       6,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     4,     4,     4,     5,     5,     5,     5,     6,     6,
       6,     6,     1,     3,     4,     5,     3,     4,     2,     2,
       3,     5,     6,     4,     5,     5,     5,     3,     7,     8,
       6,     5,     6,     6,     6,     4,     8,     9,     7,     4,
       5,     6,     4,     5,     3,     4,     6,     6,     6,     1,
       1,     1,     5,     3,     1,     1,     1,     1,     3,     6,
       3,     1,     1,     1,     1,     1,     1,     1,     0,     3,
       0,     1,     1,     0,     1,     0,     2,     3,     3,     1,
       2,     1,     3,    11,     0,     2,     3,     8,     0,     1,
       1,     2,     1,     1,     3,     2,     0,     1,     0,     3,
       3,     3,     3,     3,     0,     1,     0,     1,     1,     0,
       2,     5,     6,     4,     2,     3,     5,     3,     3,     3,
       3,     3,     5,     3,     5,     7,     1,     2,     3,     2,
       1,     3,     0,     2,     2,     1,     3,     0,     3,     4,
       4,     3,     3,     4,     4,     4,     3,     3,     4,     4,
       4,     3,     3,     4,     2,     1,     2,     0,     1,     4,
       4,     2,     5,     5,     3,     3,     3,     1,     4,     4,
       2,     7,     8,     6,     5,    10,     1,     3,     2,     0,
       2,     7,    10
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
  "\"SELECT\"", "\"SET\"", "\"STORED\"", "\"STRICT\"", "\"TABLE\"",
  "\"TEMP\"", "\"TEMPORARY\"", "\"THEN\"", "\"TRUE\"", "\"UNBOUNDED\"",
  "\"UNIQUE\"", "\"UPDATE\"", "\"USING\"", "\"VIRTUAL\"", "\"WHEN\"",
  "\"WHERE\"", "\"WITHOUT\"", "\"identifier\"", "\"numeric\"",
  "\"string literal\"", "\"quoted literal\"", "\"blob literal\"",
  "\"bind parameter\"", "$accept", "sql", "statement", "literalvalue",
  "id", "allowed_keywords_as_identifier", "tableid", "columnid",
  "signednumber", "signednumber_or_numeric", "typename_namelist",
  "type_name", "unary_expr", "binary_expr", "like_expr", "exprlist_expr",
  "function_expr", "isnull_expr", "between_expr", "in_expr",
  "whenthenlist_expr", "case_expr", "raise_expr", "expr", "select_stmt",
  "optional_if_not_exists", "optional_sort_order", "optional_unique",
  "optional_where", "tableid_with_uninteresting_schema", "indexed_column",
  "indexed_column_list", "createindex_stmt",
  "optional_exprlist_with_paren", "createvirtualtable_stmt",
  "optional_temporary", "tableoption", "tableoptions_list",
  "optional_tableoptions_list", "optional_conflictclause",
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
       0,   273,   273,   274,   278,   279,   280,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   300,   301,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   350,
     351,   352,   353,   354,   355,   359,   360,   361,   362,   363,
     364,   365,   369,   370,   374,   375,   379,   380,   384,   385,
     386,   390,   391,   392,   393,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   440,   441,   445,   446,   447,   448,   452,   453,
     454,   458,   459,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   482,
     483,   487,   488,   489,   490,   494,   495,   496,   497,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   518,   527,   535,   536,
     540,   541,   542,   550,   551,   555,   556,   560,   561,   562,
     566,   583,   584,   588,   603,   604,   605,   609,   621,   622,
     623,   627,   628,   632,   633,   634,   638,   639,   643,   644,
     645,   646,   647,   648,   652,   653,   657,   658,   659,   663,
     664,   668,   674,   681,   687,   689,   695,   699,   703,   707,
     711,   716,   720,   724,   728,   736,   743,   744,   748,   803,
     807,   808,   812,   813,   814,   818,   819,   823,   824,   828,
     829,   830,   831,   832,   833,   834,   835,   836,   837,   838,
     839,   840,   841,   842,   843,   847,   848,   852,   853,   854,
     855,   856,   857,   858,   859,   860,   861,   862,   863,   864,
     865,   869,   875,   882,   889,   894,   902,   903,   904,   908,
     909,   913,   917
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
#line 4831 "sqlite3_parser.cpp"

#line 946 "sqlite3_parser.yy"


void sqlb::parser::parser::error(const location_type& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}
