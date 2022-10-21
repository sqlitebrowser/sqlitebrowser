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
#line 39 "sqlite3_parser.yy"

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
      case symbol_kind::S_columndef_list: // columndef_list
        value.YY_MOVE_OR_COPY< ColumnList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint: // tableconstraint
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.YY_MOVE_OR_COPY< TableConstraints > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
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

      case symbol_kind::S_columnconstraint: // columnconstraint
        value.YY_MOVE_OR_COPY< std::pair<std::shared_ptr<sqlb::PrimaryKeyConstraint>, std::shared_ptr<sqlb::ForeignKeyClause>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columndef: // columndef
        value.YY_MOVE_OR_COPY< std::shared_ptr<sqlb::Field> > (YY_MOVE (that.value));
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
      case symbol_kind::S_FROM: // "FROM"
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
      case symbol_kind::S_columndef_list: // columndef_list
        value.move< ColumnList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint: // tableconstraint
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.move< TableConstraints > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.move< bool > (YY_MOVE (that.value));
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

      case symbol_kind::S_columnconstraint: // columnconstraint
        value.move< std::pair<std::shared_ptr<sqlb::PrimaryKeyConstraint>, std::shared_ptr<sqlb::ForeignKeyClause>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columndef: // columndef
        value.move< std::shared_ptr<sqlb::Field> > (YY_MOVE (that.value));
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
      case symbol_kind::S_FROM: // "FROM"
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
      case symbol_kind::S_columndef_list: // columndef_list
        value.copy< ColumnList > (that.value);
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint: // tableconstraint
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.copy< TableConstraints > (that.value);
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.copy< bool > (that.value);
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

      case symbol_kind::S_columnconstraint: // columnconstraint
        value.copy< std::pair<std::shared_ptr<sqlb::PrimaryKeyConstraint>, std::shared_ptr<sqlb::ForeignKeyClause>> > (that.value);
        break;

      case symbol_kind::S_columndef: // columndef
        value.copy< std::shared_ptr<sqlb::Field> > (that.value);
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
      case symbol_kind::S_FROM: // "FROM"
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
      case symbol_kind::S_columndef_list: // columndef_list
        value.move< ColumnList > (that.value);
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint: // tableconstraint
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.move< TableConstraints > (that.value);
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.move< bool > (that.value);
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

      case symbol_kind::S_columnconstraint: // columnconstraint
        value.move< std::pair<std::shared_ptr<sqlb::PrimaryKeyConstraint>, std::shared_ptr<sqlb::ForeignKeyClause>> > (that.value);
        break;

      case symbol_kind::S_columndef: // columndef
        value.move< std::shared_ptr<sqlb::Field> > (that.value);
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
      case symbol_kind::S_FROM: // "FROM"
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
      case symbol_kind::S_columndef_list: // columndef_list
        yylhs.value.emplace< ColumnList > ();
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint: // tableconstraint
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        yylhs.value.emplace< TableConstraints > ();
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        yylhs.value.emplace< bool > ();
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

      case symbol_kind::S_columnconstraint: // columnconstraint
        yylhs.value.emplace< std::pair<std::shared_ptr<sqlb::PrimaryKeyConstraint>, std::shared_ptr<sqlb::ForeignKeyClause>> > ();
        break;

      case symbol_kind::S_columndef: // columndef
        yylhs.value.emplace< std::shared_ptr<sqlb::Field> > ();
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
      case symbol_kind::S_FROM: // "FROM"
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
#line 1479 "sqlite3_parser.cpp"
    break;

  case 5: // statement: createvirtualtable_stmt
#line 279 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::TablePtr > (); }
#line 1485 "sqlite3_parser.cpp"
    break;

  case 6: // statement: createtable_stmt
#line 280 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::TablePtr > (); }
#line 1491 "sqlite3_parser.cpp"
    break;

  case 7: // literalvalue: "numeric"
#line 288 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1497 "sqlite3_parser.cpp"
    break;

  case 8: // literalvalue: "string literal"
#line 289 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1503 "sqlite3_parser.cpp"
    break;

  case 9: // literalvalue: "blob literal"
#line 290 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1509 "sqlite3_parser.cpp"
    break;

  case 10: // literalvalue: "NULL"
#line 291 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1515 "sqlite3_parser.cpp"
    break;

  case 11: // literalvalue: "TRUE"
#line 292 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1521 "sqlite3_parser.cpp"
    break;

  case 12: // literalvalue: "FALSE"
#line 293 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1527 "sqlite3_parser.cpp"
    break;

  case 13: // literalvalue: "CURRENT_TIME"
#line 294 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1533 "sqlite3_parser.cpp"
    break;

  case 14: // literalvalue: "CURRENT_DATE"
#line 295 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1539 "sqlite3_parser.cpp"
    break;

  case 15: // literalvalue: "CURRENT_TIMESTAMP"
#line 296 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1545 "sqlite3_parser.cpp"
    break;

  case 16: // id: "identifier"
#line 300 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1551 "sqlite3_parser.cpp"
    break;

  case 17: // id: "quoted literal"
#line 301 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1557 "sqlite3_parser.cpp"
    break;

  case 18: // allowed_keywords_as_identifier: "ABORT"
#line 306 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1563 "sqlite3_parser.cpp"
    break;

  case 19: // allowed_keywords_as_identifier: "ACTION"
#line 307 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1569 "sqlite3_parser.cpp"
    break;

  case 20: // allowed_keywords_as_identifier: "ALWAYS"
#line 308 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1575 "sqlite3_parser.cpp"
    break;

  case 21: // allowed_keywords_as_identifier: "ASC"
#line 309 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1581 "sqlite3_parser.cpp"
    break;

  case 22: // allowed_keywords_as_identifier: "CASCADE"
#line 310 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1587 "sqlite3_parser.cpp"
    break;

  case 23: // allowed_keywords_as_identifier: "CAST"
#line 311 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1593 "sqlite3_parser.cpp"
    break;

  case 24: // allowed_keywords_as_identifier: "CONFLICT"
#line 312 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1599 "sqlite3_parser.cpp"
    break;

  case 25: // allowed_keywords_as_identifier: "DEFERRED"
#line 313 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1605 "sqlite3_parser.cpp"
    break;

  case 26: // allowed_keywords_as_identifier: "DESC"
#line 314 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1611 "sqlite3_parser.cpp"
    break;

  case 27: // allowed_keywords_as_identifier: "END"
#line 315 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1617 "sqlite3_parser.cpp"
    break;

  case 28: // allowed_keywords_as_identifier: "FAIL"
#line 316 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1623 "sqlite3_parser.cpp"
    break;

  case 29: // allowed_keywords_as_identifier: "FILTER"
#line 317 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1629 "sqlite3_parser.cpp"
    break;

  case 30: // allowed_keywords_as_identifier: "FOLLOWING"
#line 318 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1635 "sqlite3_parser.cpp"
    break;

  case 31: // allowed_keywords_as_identifier: "GENERATED"
#line 319 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1641 "sqlite3_parser.cpp"
    break;

  case 32: // allowed_keywords_as_identifier: "GLOB"
#line 320 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1647 "sqlite3_parser.cpp"
    break;

  case 33: // allowed_keywords_as_identifier: "KEY"
#line 321 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1653 "sqlite3_parser.cpp"
    break;

  case 34: // allowed_keywords_as_identifier: "LIKE"
#line 322 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1659 "sqlite3_parser.cpp"
    break;

  case 35: // allowed_keywords_as_identifier: "IGNORE"
#line 323 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1665 "sqlite3_parser.cpp"
    break;

  case 36: // allowed_keywords_as_identifier: "INITIALLY"
#line 324 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1671 "sqlite3_parser.cpp"
    break;

  case 37: // allowed_keywords_as_identifier: "IMMEDIATE"
#line 325 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1677 "sqlite3_parser.cpp"
    break;

  case 38: // allowed_keywords_as_identifier: "MATCH"
#line 326 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1683 "sqlite3_parser.cpp"
    break;

  case 39: // allowed_keywords_as_identifier: "NO"
#line 327 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1689 "sqlite3_parser.cpp"
    break;

  case 40: // allowed_keywords_as_identifier: "OVER"
#line 328 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1695 "sqlite3_parser.cpp"
    break;

  case 41: // allowed_keywords_as_identifier: "PARTITION"
#line 329 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1701 "sqlite3_parser.cpp"
    break;

  case 42: // allowed_keywords_as_identifier: "PRECEDING"
#line 330 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1707 "sqlite3_parser.cpp"
    break;

  case 43: // allowed_keywords_as_identifier: "RAISE"
#line 331 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1713 "sqlite3_parser.cpp"
    break;

  case 44: // allowed_keywords_as_identifier: "RANGE"
#line 332 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1719 "sqlite3_parser.cpp"
    break;

  case 45: // allowed_keywords_as_identifier: "REGEXP"
#line 333 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1725 "sqlite3_parser.cpp"
    break;

  case 46: // allowed_keywords_as_identifier: "REPLACE"
#line 334 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1731 "sqlite3_parser.cpp"
    break;

  case 47: // allowed_keywords_as_identifier: "RESTRICT"
#line 335 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1737 "sqlite3_parser.cpp"
    break;

  case 48: // allowed_keywords_as_identifier: "RETURNING"
#line 336 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1743 "sqlite3_parser.cpp"
    break;

  case 49: // allowed_keywords_as_identifier: "ROLLBACK"
#line 337 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1749 "sqlite3_parser.cpp"
    break;

  case 50: // allowed_keywords_as_identifier: "ROWID"
#line 338 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1755 "sqlite3_parser.cpp"
    break;

  case 51: // allowed_keywords_as_identifier: "ROWS"
#line 339 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1761 "sqlite3_parser.cpp"
    break;

  case 52: // allowed_keywords_as_identifier: "STORED"
#line 340 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1767 "sqlite3_parser.cpp"
    break;

  case 53: // allowed_keywords_as_identifier: "STRICT"
#line 341 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1773 "sqlite3_parser.cpp"
    break;

  case 54: // allowed_keywords_as_identifier: "TEMPORARY"
#line 342 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1779 "sqlite3_parser.cpp"
    break;

  case 55: // allowed_keywords_as_identifier: "TEMP"
#line 343 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1785 "sqlite3_parser.cpp"
    break;

  case 56: // allowed_keywords_as_identifier: "UNBOUNDED"
#line 344 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1791 "sqlite3_parser.cpp"
    break;

  case 57: // allowed_keywords_as_identifier: "VIRTUAL"
#line 345 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1797 "sqlite3_parser.cpp"
    break;

  case 58: // allowed_keywords_as_identifier: "WITHOUT"
#line 346 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1803 "sqlite3_parser.cpp"
    break;

  case 59: // tableid: allowed_keywords_as_identifier
#line 350 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1809 "sqlite3_parser.cpp"
    break;

  case 60: // tableid: "CURRENT_TIME"
#line 351 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1815 "sqlite3_parser.cpp"
    break;

  case 61: // tableid: "CURRENT_DATE"
#line 352 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1821 "sqlite3_parser.cpp"
    break;

  case 62: // tableid: "CURRENT_TIMESTAMP"
#line 353 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1827 "sqlite3_parser.cpp"
    break;

  case 63: // tableid: id
#line 354 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1833 "sqlite3_parser.cpp"
    break;

  case 64: // tableid: "string literal"
#line 355 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1839 "sqlite3_parser.cpp"
    break;

  case 65: // columnid: allowed_keywords_as_identifier
#line 359 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1845 "sqlite3_parser.cpp"
    break;

  case 66: // columnid: "CURRENT_TIME"
#line 360 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1851 "sqlite3_parser.cpp"
    break;

  case 67: // columnid: "CURRENT_DATE"
#line 361 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1857 "sqlite3_parser.cpp"
    break;

  case 68: // columnid: "CURRENT_TIMESTAMP"
#line 362 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1863 "sqlite3_parser.cpp"
    break;

  case 69: // columnid: "IF"
#line 363 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1869 "sqlite3_parser.cpp"
    break;

  case 70: // columnid: id
#line 364 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1875 "sqlite3_parser.cpp"
    break;

  case 71: // columnid: "string literal"
#line 365 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1881 "sqlite3_parser.cpp"
    break;

  case 72: // signednumber: "+" "numeric"
#line 369 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1887 "sqlite3_parser.cpp"
    break;

  case 73: // signednumber: "-" "numeric"
#line 370 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1893 "sqlite3_parser.cpp"
    break;

  case 74: // signednumber_or_numeric: signednumber
#line 374 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1899 "sqlite3_parser.cpp"
    break;

  case 75: // signednumber_or_numeric: "numeric"
#line 375 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1905 "sqlite3_parser.cpp"
    break;

  case 76: // typename_namelist: tableid
#line 379 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1911 "sqlite3_parser.cpp"
    break;

  case 77: // typename_namelist: typename_namelist tableid
#line 380 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 1917 "sqlite3_parser.cpp"
    break;

  case 78: // type_name: typename_namelist
#line 384 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1923 "sqlite3_parser.cpp"
    break;

  case 79: // type_name: typename_namelist "(" signednumber_or_numeric ")"
#line 385 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1929 "sqlite3_parser.cpp"
    break;

  case 80: // type_name: typename_namelist "(" signednumber_or_numeric "," signednumber_or_numeric ")"
#line 386 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + "(" + yystack_[3].value.as < std::string > () + ", " + yystack_[1].value.as < std::string > () + ")"; }
#line 1935 "sqlite3_parser.cpp"
    break;

  case 81: // unary_expr: "-" expr
#line 390 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1941 "sqlite3_parser.cpp"
    break;

  case 82: // unary_expr: "+" expr
#line 391 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1947 "sqlite3_parser.cpp"
    break;

  case 83: // unary_expr: "~" expr
#line 392 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "~" + yystack_[0].value.as < std::string > (); }
#line 1953 "sqlite3_parser.cpp"
    break;

  case 84: // unary_expr: "NOT" expr
#line 393 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "NOT " + yystack_[0].value.as < std::string > (); }
#line 1959 "sqlite3_parser.cpp"
    break;

  case 85: // binary_expr: expr "||" expr
#line 397 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " || " + yystack_[0].value.as < std::string > (); }
#line 1965 "sqlite3_parser.cpp"
    break;

  case 86: // binary_expr: expr "*" expr
#line 398 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " * " + yystack_[0].value.as < std::string > (); }
#line 1971 "sqlite3_parser.cpp"
    break;

  case 87: // binary_expr: expr "/" expr
#line 399 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " / " + yystack_[0].value.as < std::string > (); }
#line 1977 "sqlite3_parser.cpp"
    break;

  case 88: // binary_expr: expr "%" expr
#line 400 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " % " + yystack_[0].value.as < std::string > (); }
#line 1983 "sqlite3_parser.cpp"
    break;

  case 89: // binary_expr: expr "+" expr
#line 401 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " + " + yystack_[0].value.as < std::string > (); }
#line 1989 "sqlite3_parser.cpp"
    break;

  case 90: // binary_expr: expr "-" expr
#line 402 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " - " + yystack_[0].value.as < std::string > (); }
#line 1995 "sqlite3_parser.cpp"
    break;

  case 91: // binary_expr: expr "<<" expr
#line 403 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " << " + yystack_[0].value.as < std::string > (); }
#line 2001 "sqlite3_parser.cpp"
    break;

  case 92: // binary_expr: expr ">>" expr
#line 404 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >> " + yystack_[0].value.as < std::string > (); }
#line 2007 "sqlite3_parser.cpp"
    break;

  case 93: // binary_expr: expr "&" expr
#line 405 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " & " + yystack_[0].value.as < std::string > (); }
#line 2013 "sqlite3_parser.cpp"
    break;

  case 94: // binary_expr: expr "|" expr
#line 406 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " | " + yystack_[0].value.as < std::string > (); }
#line 2019 "sqlite3_parser.cpp"
    break;

  case 95: // binary_expr: expr "<" expr
#line 407 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " < " + yystack_[0].value.as < std::string > (); }
#line 2025 "sqlite3_parser.cpp"
    break;

  case 96: // binary_expr: expr "<=" expr
#line 408 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <= " + yystack_[0].value.as < std::string > (); }
#line 2031 "sqlite3_parser.cpp"
    break;

  case 97: // binary_expr: expr ">" expr
#line 409 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " > " + yystack_[0].value.as < std::string > (); }
#line 2037 "sqlite3_parser.cpp"
    break;

  case 98: // binary_expr: expr ">=" expr
#line 410 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >= " + yystack_[0].value.as < std::string > (); }
#line 2043 "sqlite3_parser.cpp"
    break;

  case 99: // binary_expr: expr "=" expr
#line 411 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " = " + yystack_[0].value.as < std::string > (); }
#line 2049 "sqlite3_parser.cpp"
    break;

  case 100: // binary_expr: expr "==" expr
#line 412 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " == " + yystack_[0].value.as < std::string > (); }
#line 2055 "sqlite3_parser.cpp"
    break;

  case 101: // binary_expr: expr "!=" expr
#line 413 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " != " + yystack_[0].value.as < std::string > (); }
#line 2061 "sqlite3_parser.cpp"
    break;

  case 102: // binary_expr: expr "<>" expr
#line 414 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <> " + yystack_[0].value.as < std::string > (); }
#line 2067 "sqlite3_parser.cpp"
    break;

  case 103: // binary_expr: expr "IS" expr
#line 415 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IS " + yystack_[0].value.as < std::string > (); }
#line 2073 "sqlite3_parser.cpp"
    break;

  case 104: // binary_expr: expr "IS" "DISTINCT" "FROM" expr
#line 416 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IS DISTINCT FROM " + yystack_[0].value.as < std::string > (); }
#line 2079 "sqlite3_parser.cpp"
    break;

  case 105: // binary_expr: expr "IS" "NOT" "DISTINCT" "FROM" expr
#line 417 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " IS NOT DISTINCT FROM " + yystack_[0].value.as < std::string > (); }
#line 2085 "sqlite3_parser.cpp"
    break;

  case 106: // binary_expr: expr "AND" expr
#line 418 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2091 "sqlite3_parser.cpp"
    break;

  case 107: // binary_expr: expr "OR" expr
#line 419 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " OR " + yystack_[0].value.as < std::string > (); }
#line 2097 "sqlite3_parser.cpp"
    break;

  case 108: // like_expr: expr "LIKE" expr
#line 423 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " LIKE " + yystack_[0].value.as < std::string > (); }
#line 2103 "sqlite3_parser.cpp"
    break;

  case 109: // like_expr: expr "GLOB" expr
#line 424 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " GLOB " + yystack_[0].value.as < std::string > (); }
#line 2109 "sqlite3_parser.cpp"
    break;

  case 110: // like_expr: expr "MATCH" expr
#line 425 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " MATCH " + yystack_[0].value.as < std::string > (); }
#line 2115 "sqlite3_parser.cpp"
    break;

  case 111: // like_expr: expr "REGEXP" expr
#line 426 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " REGEXP " + yystack_[0].value.as < std::string > (); }
#line 2121 "sqlite3_parser.cpp"
    break;

  case 112: // like_expr: expr "NOT" "LIKE" expr
#line 427 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT LIKE " + yystack_[0].value.as < std::string > (); }
#line 2127 "sqlite3_parser.cpp"
    break;

  case 113: // like_expr: expr "NOT" "GLOB" expr
#line 428 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT GLOB " + yystack_[0].value.as < std::string > (); }
#line 2133 "sqlite3_parser.cpp"
    break;

  case 114: // like_expr: expr "NOT" "MATCH" expr
#line 429 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT MATCH " + yystack_[0].value.as < std::string > (); }
#line 2139 "sqlite3_parser.cpp"
    break;

  case 115: // like_expr: expr "NOT" "REGEXP" expr
#line 430 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT REGEXP " + yystack_[0].value.as < std::string > (); }
#line 2145 "sqlite3_parser.cpp"
    break;

  case 116: // like_expr: expr "LIKE" expr "ESCAPE" expr
#line 431 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " LIKE " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2151 "sqlite3_parser.cpp"
    break;

  case 117: // like_expr: expr "GLOB" expr "ESCAPE" expr
#line 432 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " GLOB " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2157 "sqlite3_parser.cpp"
    break;

  case 118: // like_expr: expr "MATCH" expr "ESCAPE" expr
#line 433 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " MATCH " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2163 "sqlite3_parser.cpp"
    break;

  case 119: // like_expr: expr "REGEXP" expr "ESCAPE" expr
#line 434 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " REGEXP " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2169 "sqlite3_parser.cpp"
    break;

  case 120: // like_expr: expr "NOT" "LIKE" expr "ESCAPE" expr
#line 435 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT LIKE " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2175 "sqlite3_parser.cpp"
    break;

  case 121: // like_expr: expr "NOT" "GLOB" expr "ESCAPE" expr
#line 436 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT GLOB " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2181 "sqlite3_parser.cpp"
    break;

  case 122: // like_expr: expr "NOT" "MATCH" expr "ESCAPE" expr
#line 437 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT MATCH " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2187 "sqlite3_parser.cpp"
    break;

  case 123: // like_expr: expr "NOT" "REGEXP" expr "ESCAPE" expr
#line 438 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT REGEXP " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2193 "sqlite3_parser.cpp"
    break;

  case 124: // exprlist_expr: expr
#line 442 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2199 "sqlite3_parser.cpp"
    break;

  case 125: // exprlist_expr: exprlist_expr "," expr
#line 443 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + ", " + yystack_[0].value.as < std::string > (); }
#line 2205 "sqlite3_parser.cpp"
    break;

  case 126: // function_expr: id "(" exprlist_expr ")"
#line 447 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2211 "sqlite3_parser.cpp"
    break;

  case 127: // function_expr: id "(" "DISTINCT" exprlist_expr ")"
#line 448 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + "(DISTINCT " + yystack_[1].value.as < std::string > () + ")"; }
#line 2217 "sqlite3_parser.cpp"
    break;

  case 128: // function_expr: id "(" ")"
#line 449 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + "()"; }
#line 2223 "sqlite3_parser.cpp"
    break;

  case 129: // function_expr: id "(" "*" ")"
#line 450 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(*)"; }
#line 2229 "sqlite3_parser.cpp"
    break;

  case 130: // isnull_expr: expr "ISNULL"
#line 454 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " ISNULL"; }
#line 2235 "sqlite3_parser.cpp"
    break;

  case 131: // isnull_expr: expr "NOTNULL"
#line 455 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " NOTNULL"; }
#line 2241 "sqlite3_parser.cpp"
    break;

  case 132: // isnull_expr: expr "NOT" "NULL"
#line 456 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " NOT NULL"; }
#line 2247 "sqlite3_parser.cpp"
    break;

  case 133: // between_expr: expr "BETWEEN" expr "AND BETWEEN" expr
#line 460 "sqlite3_parser.yy"
                                                                { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2253 "sqlite3_parser.cpp"
    break;

  case 134: // between_expr: expr "NOT" "BETWEEN" expr "AND BETWEEN" expr
#line 461 "sqlite3_parser.yy"
                                                                { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2259 "sqlite3_parser.cpp"
    break;

  case 135: // in_expr: expr "IN" "(" ")"
#line 465 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " IN ()"; }
#line 2265 "sqlite3_parser.cpp"
    break;

  case 136: // in_expr: expr "IN" "(" select_stmt ")"
#line 466 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2271 "sqlite3_parser.cpp"
    break;

  case 137: // in_expr: expr "IN" "(" exprlist_expr ")"
#line 467 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2277 "sqlite3_parser.cpp"
    break;

  case 138: // in_expr: expr "IN" id "." tableid
#line 468 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2283 "sqlite3_parser.cpp"
    break;

  case 139: // in_expr: expr "IN" tableid
#line 469 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2289 "sqlite3_parser.cpp"
    break;

  case 140: // in_expr: expr "IN" id "." id "(" ")"
#line 470 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 2295 "sqlite3_parser.cpp"
    break;

  case 141: // in_expr: expr "IN" id "." id "(" exprlist_expr ")"
#line 471 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2301 "sqlite3_parser.cpp"
    break;

  case 142: // in_expr: expr "IN" id "(" exprlist_expr ")"
#line 472 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2307 "sqlite3_parser.cpp"
    break;

  case 143: // in_expr: expr "NOT" "IN" "(" ")"
#line 473 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " NOT IN ()"; }
#line 2313 "sqlite3_parser.cpp"
    break;

  case 144: // in_expr: expr "NOT" "IN" "(" select_stmt ")"
#line 474 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2319 "sqlite3_parser.cpp"
    break;

  case 145: // in_expr: expr "NOT" "IN" "(" exprlist_expr ")"
#line 475 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2325 "sqlite3_parser.cpp"
    break;

  case 146: // in_expr: expr "NOT" "IN" id "." tableid
#line 476 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2331 "sqlite3_parser.cpp"
    break;

  case 147: // in_expr: expr "NOT" "IN" tableid
#line 477 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2337 "sqlite3_parser.cpp"
    break;

  case 148: // in_expr: expr "NOT" "IN" id "." id "(" ")"
#line 478 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 2343 "sqlite3_parser.cpp"
    break;

  case 149: // in_expr: expr "NOT" "IN" id "." id "(" exprlist_expr ")"
#line 479 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[8].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2349 "sqlite3_parser.cpp"
    break;

  case 150: // in_expr: expr "NOT" "IN" id "(" exprlist_expr ")"
#line 480 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " NOT IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2355 "sqlite3_parser.cpp"
    break;

  case 151: // whenthenlist_expr: "WHEN" expr "THEN" expr
#line 484 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "WHEN " + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 2361 "sqlite3_parser.cpp"
    break;

  case 152: // whenthenlist_expr: whenthenlist_expr "WHEN" expr "THEN" expr
#line 485 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " WHEN" + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 2367 "sqlite3_parser.cpp"
    break;

  case 153: // case_expr: "CASE" expr whenthenlist_expr "ELSE" expr "END"
#line 489 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2373 "sqlite3_parser.cpp"
    break;

  case 154: // case_expr: "CASE" expr whenthenlist_expr "END"
#line 490 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " END"; }
#line 2379 "sqlite3_parser.cpp"
    break;

  case 155: // case_expr: "CASE" whenthenlist_expr "ELSE" expr "END"
#line 491 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2385 "sqlite3_parser.cpp"
    break;

  case 156: // case_expr: "CASE" whenthenlist_expr "END"
#line 492 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2391 "sqlite3_parser.cpp"
    break;

  case 157: // raise_expr: "RAISE" "(" "IGNORE" ")"
#line 496 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(IGNORE)"; }
#line 2397 "sqlite3_parser.cpp"
    break;

  case 158: // raise_expr: "RAISE" "(" "ROLLBACK" "," "string literal" ")"
#line 497 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(ROLLBACK, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2403 "sqlite3_parser.cpp"
    break;

  case 159: // raise_expr: "RAISE" "(" "ABORT" "," "string literal" ")"
#line 498 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(ABORT, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2409 "sqlite3_parser.cpp"
    break;

  case 160: // raise_expr: "RAISE" "(" "FAIL" "," "string literal" ")"
#line 499 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(FAIL, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2415 "sqlite3_parser.cpp"
    break;

  case 161: // expr: literalvalue
#line 503 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2421 "sqlite3_parser.cpp"
    break;

  case 162: // expr: allowed_keywords_as_identifier
#line 504 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2427 "sqlite3_parser.cpp"
    break;

  case 163: // expr: "bind parameter"
#line 505 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2433 "sqlite3_parser.cpp"
    break;

  case 164: // expr: id "." id "." id
#line 506 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2439 "sqlite3_parser.cpp"
    break;

  case 165: // expr: id "." id
#line 507 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2445 "sqlite3_parser.cpp"
    break;

  case 166: // expr: id
#line 508 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2451 "sqlite3_parser.cpp"
    break;

  case 167: // expr: unary_expr
#line 509 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2457 "sqlite3_parser.cpp"
    break;

  case 168: // expr: binary_expr
#line 510 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2463 "sqlite3_parser.cpp"
    break;

  case 169: // expr: function_expr
#line 511 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2469 "sqlite3_parser.cpp"
    break;

  case 170: // expr: "(" exprlist_expr ")"
#line 512 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2475 "sqlite3_parser.cpp"
    break;

  case 171: // expr: "CAST" "(" expr "AS" type_name ")"
#line 513 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "CAST(" + yystack_[3].value.as < std::string > () + " AS " + yystack_[1].value.as < std::string > () + ")"; }
#line 2481 "sqlite3_parser.cpp"
    break;

  case 172: // expr: expr "COLLATE" id
#line 514 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " COLLATE " + yystack_[0].value.as < std::string > (); }
#line 2487 "sqlite3_parser.cpp"
    break;

  case 173: // expr: like_expr
#line 515 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2493 "sqlite3_parser.cpp"
    break;

  case 174: // expr: isnull_expr
#line 516 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2499 "sqlite3_parser.cpp"
    break;

  case 175: // expr: between_expr
#line 517 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2505 "sqlite3_parser.cpp"
    break;

  case 176: // expr: in_expr
#line 518 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2511 "sqlite3_parser.cpp"
    break;

  case 177: // expr: case_expr
#line 519 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2517 "sqlite3_parser.cpp"
    break;

  case 178: // expr: raise_expr
#line 520 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2523 "sqlite3_parser.cpp"
    break;

  case 179: // select_stmt: "SELECT"
#line 529 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "SELECT"; }
#line 2529 "sqlite3_parser.cpp"
    break;

  case 180: // optional_if_not_exists: %empty
#line 537 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = false; }
#line 2535 "sqlite3_parser.cpp"
    break;

  case 181: // optional_if_not_exists: "IF" "NOT" "EXISTS"
#line 538 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = true; }
#line 2541 "sqlite3_parser.cpp"
    break;

  case 182: // optional_sort_order: %empty
#line 542 "sqlite3_parser.yy"
                                                { }
#line 2547 "sqlite3_parser.cpp"
    break;

  case 183: // optional_sort_order: "ASC"
#line 543 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "ASC"; }
#line 2553 "sqlite3_parser.cpp"
    break;

  case 184: // optional_sort_order: "DESC"
#line 544 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "DESC"; }
#line 2559 "sqlite3_parser.cpp"
    break;

  case 185: // optional_unique: %empty
#line 552 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = false; }
#line 2565 "sqlite3_parser.cpp"
    break;

  case 186: // optional_unique: "UNIQUE"
#line 553 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = true; }
#line 2571 "sqlite3_parser.cpp"
    break;

  case 187: // optional_where: %empty
#line 557 "sqlite3_parser.yy"
                                                { }
#line 2577 "sqlite3_parser.cpp"
    break;

  case 188: // optional_where: "WHERE" expr
#line 558 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2583 "sqlite3_parser.cpp"
    break;

  case 189: // tableid_with_uninteresting_schema: id "." tableid
#line 562 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2589 "sqlite3_parser.cpp"
    break;

  case 190: // tableid_with_uninteresting_schema: "TEMP" "." tableid
#line 563 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2595 "sqlite3_parser.cpp"
    break;

  case 191: // tableid_with_uninteresting_schema: tableid
#line 564 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2601 "sqlite3_parser.cpp"
    break;

  case 192: // indexed_column: expr optional_sort_order
#line 568 "sqlite3_parser.yy"
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
#line 2620 "sqlite3_parser.cpp"
    break;

  case 193: // indexed_column_list: indexed_column
#line 585 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::IndexedColumnVector > () = sqlb::IndexedColumnVector(1, yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2626 "sqlite3_parser.cpp"
    break;

  case 194: // indexed_column_list: indexed_column_list "," indexed_column
#line 586 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::IndexedColumnVector > () = yystack_[2].value.as < sqlb::IndexedColumnVector > (); yylhs.value.as < sqlb::IndexedColumnVector > ().push_back(yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2632 "sqlite3_parser.cpp"
    break;

  case 195: // createindex_stmt: "CREATE" optional_unique "INDEX" optional_if_not_exists tableid_with_uninteresting_schema "ON" tableid "(" indexed_column_list ")" optional_where
#line 590 "sqlite3_parser.yy"
                                                                                                                                                                {
													yylhs.value.as < sqlb::IndexPtr > () = std::make_shared<sqlb::Index>(yystack_[6].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setTable(yystack_[4].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setUnique(yystack_[9].value.as < bool > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setWhereExpr(yystack_[0].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->fields = yystack_[2].value.as < sqlb::IndexedColumnVector > ();
													yylhs.value.as < sqlb::IndexPtr > ()->setFullyParsed(true);
												}
#line 2645 "sqlite3_parser.cpp"
    break;

  case 196: // optional_exprlist_with_paren: %empty
#line 605 "sqlite3_parser.yy"
                                                { }
#line 2651 "sqlite3_parser.cpp"
    break;

  case 197: // optional_exprlist_with_paren: "(" ")"
#line 606 "sqlite3_parser.yy"
                                                { }
#line 2657 "sqlite3_parser.cpp"
    break;

  case 198: // optional_exprlist_with_paren: "(" exprlist_expr ")"
#line 607 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 2663 "sqlite3_parser.cpp"
    break;

  case 199: // createvirtualtable_stmt: "CREATE" "VIRTUAL" "TABLE" optional_if_not_exists tableid_with_uninteresting_schema "USING" id optional_exprlist_with_paren
#line 611 "sqlite3_parser.yy"
                                                                                                                                {
													yylhs.value.as < sqlb::TablePtr > () = std::make_shared<sqlb::Table>(yystack_[3].value.as < std::string > ());
													yylhs.value.as < sqlb::TablePtr > ()->setVirtualUsing(yystack_[1].value.as < std::string > ());
													yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
												}
#line 2673 "sqlite3_parser.cpp"
    break;

  case 200: // optional_temporary: %empty
#line 623 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = false; }
#line 2679 "sqlite3_parser.cpp"
    break;

  case 201: // optional_temporary: "TEMP"
#line 624 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2685 "sqlite3_parser.cpp"
    break;

  case 202: // optional_temporary: "TEMPORARY"
#line 625 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2691 "sqlite3_parser.cpp"
    break;

  case 203: // tableoption: "WITHOUT" "ROWID"
#line 629 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::bitset<sqlb::Table::NumOptions> > ().set(sqlb::Table::WithoutRowid, true); }
#line 2697 "sqlite3_parser.cpp"
    break;

  case 204: // tableoption: "STRICT"
#line 630 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::bitset<sqlb::Table::NumOptions> > ().set(sqlb::Table::Strict, true); }
#line 2703 "sqlite3_parser.cpp"
    break;

  case 205: // tableoptions_list: tableoption
#line 634 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::bitset<sqlb::Table::NumOptions> > () = yystack_[0].value.as < std::bitset<sqlb::Table::NumOptions> > (); }
#line 2709 "sqlite3_parser.cpp"
    break;

  case 206: // tableoptions_list: tableoptions_list "," tableoption
#line 635 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::bitset<sqlb::Table::NumOptions> > () = yystack_[2].value.as < std::bitset<sqlb::Table::NumOptions> > () | yystack_[0].value.as < std::bitset<sqlb::Table::NumOptions> > (); }
#line 2715 "sqlite3_parser.cpp"
    break;

  case 207: // tableoptions_list: tableoptions_list tableoption
#line 636 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::bitset<sqlb::Table::NumOptions> > () = yystack_[1].value.as < std::bitset<sqlb::Table::NumOptions> > () | yystack_[0].value.as < std::bitset<sqlb::Table::NumOptions> > (); }
#line 2721 "sqlite3_parser.cpp"
    break;

  case 208: // optional_tableoptions_list: %empty
#line 640 "sqlite3_parser.yy"
                                                        { }
#line 2727 "sqlite3_parser.cpp"
    break;

  case 209: // optional_tableoptions_list: tableoptions_list
#line 641 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::bitset<sqlb::Table::NumOptions> > () = yystack_[0].value.as < std::bitset<sqlb::Table::NumOptions> > (); }
#line 2733 "sqlite3_parser.cpp"
    break;

  case 210: // optional_conflictclause: %empty
#line 645 "sqlite3_parser.yy"
                                                        { }
#line 2739 "sqlite3_parser.cpp"
    break;

  case 211: // optional_conflictclause: "ON" "CONFLICT" "ROLLBACK"
#line 646 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2745 "sqlite3_parser.cpp"
    break;

  case 212: // optional_conflictclause: "ON" "CONFLICT" "ABORT"
#line 647 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2751 "sqlite3_parser.cpp"
    break;

  case 213: // optional_conflictclause: "ON" "CONFLICT" "FAIL"
#line 648 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2757 "sqlite3_parser.cpp"
    break;

  case 214: // optional_conflictclause: "ON" "CONFLICT" "IGNORE"
#line 649 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2763 "sqlite3_parser.cpp"
    break;

  case 215: // optional_conflictclause: "ON" "CONFLICT" "REPLACE"
#line 650 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2769 "sqlite3_parser.cpp"
    break;

  case 216: // optional_typename: %empty
#line 654 "sqlite3_parser.yy"
                                                { }
#line 2775 "sqlite3_parser.cpp"
    break;

  case 217: // optional_typename: type_name
#line 655 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2781 "sqlite3_parser.cpp"
    break;

  case 218: // optional_storage_identifier: %empty
#line 659 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "VIRTUAL"; }
#line 2787 "sqlite3_parser.cpp"
    break;

  case 219: // optional_storage_identifier: "STORED"
#line 660 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "STORED"; }
#line 2793 "sqlite3_parser.cpp"
    break;

  case 220: // optional_storage_identifier: "VIRTUAL"
#line 661 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "VIRTUAL"; }
#line 2799 "sqlite3_parser.cpp"
    break;

  case 221: // optional_always_generated: %empty
#line 665 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = false; }
#line 2805 "sqlite3_parser.cpp"
    break;

  case 222: // optional_always_generated: "GENERATED" "ALWAYS"
#line 666 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2811 "sqlite3_parser.cpp"
    break;

  case 223: // columnconstraint: optional_constraintname "PRIMARY" "KEY" optional_sort_order optional_conflictclause
#line 670 "sqlite3_parser.yy"
                                                                                        {
												auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>();
												pk->setName(yystack_[4].value.as < std::string > ());
												pk->setConflictAction(yystack_[0].value.as < std::string > ());
												yylhs.value.as < std::pair<std::shared_ptr<sqlb::PrimaryKeyConstraint>, std::shared_ptr<sqlb::ForeignKeyClause>> > ().first = pk;
											}
#line 2822 "sqlite3_parser.cpp"
    break;

  case 224: // columnconstraint: optional_constraintname "PRIMARY" "KEY" optional_sort_order optional_conflictclause "AUTOINCREMENT"
#line 676 "sqlite3_parser.yy"
                                                                                                        {
												auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>();
												pk->setName(yystack_[5].value.as < std::string > ());
												pk->setConflictAction(yystack_[1].value.as < std::string > ());
												pk->setAutoIncrement(true);
												yylhs.value.as < std::pair<std::shared_ptr<sqlb::PrimaryKeyConstraint>, std::shared_ptr<sqlb::ForeignKeyClause>> > ().first = pk;
											}
#line 2834 "sqlite3_parser.cpp"
    break;

  case 225: // columnconstraint: optional_constraintname "NOT" "NULL" optional_conflictclause
#line 683 "sqlite3_parser.yy"
                                                                                        {
												auto nn = std::make_shared<sqlb::NotNullConstraint>();
												nn->setName(yystack_[3].value.as < std::string > ());
												nn->setConflictAction(yystack_[0].value.as < std::string > ());
												drv.last_table_column->setNotNull(nn);
											}
#line 2845 "sqlite3_parser.cpp"
    break;

  case 226: // columnconstraint: optional_constraintname "NULL"
#line 689 "sqlite3_parser.yy"
                                                                                        {
											}
#line 2852 "sqlite3_parser.cpp"
    break;

  case 227: // columnconstraint: optional_constraintname "UNIQUE" optional_conflictclause
#line 691 "sqlite3_parser.yy"
                                                                                        {
												auto u = std::make_shared<sqlb::UniqueConstraint>();
												u->setName(yystack_[2].value.as < std::string > ());
												u->setConflictAction(yystack_[0].value.as < std::string > ());
												drv.last_table_column->setUnique(u);
											}
#line 2863 "sqlite3_parser.cpp"
    break;

  case 228: // columnconstraint: optional_constraintname "CHECK" "(" expr ")"
#line 697 "sqlite3_parser.yy"
                                                                                        {
												auto c = std::make_shared<sqlb::CheckConstraint>(yystack_[1].value.as < std::string > ());
												c->setName(yystack_[4].value.as < std::string > ());
												drv.last_table_column->setCheck(c);
											}
#line 2873 "sqlite3_parser.cpp"
    break;

  case 229: // columnconstraint: optional_constraintname "DEFAULT" signednumber
#line 702 "sqlite3_parser.yy"
                                                                                        {
												auto d = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												d->setName(yystack_[2].value.as < std::string > ());
												drv.last_table_column->setDefaultValue(d);
											}
#line 2883 "sqlite3_parser.cpp"
    break;

  case 230: // columnconstraint: optional_constraintname "DEFAULT" literalvalue
#line 707 "sqlite3_parser.yy"
                                                                                        {
												auto d = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												d->setName(yystack_[2].value.as < std::string > ());
												drv.last_table_column->setDefaultValue(d);
											}
#line 2893 "sqlite3_parser.cpp"
    break;

  case 231: // columnconstraint: optional_constraintname "DEFAULT" id
#line 712 "sqlite3_parser.yy"
                                                                                        {
												auto d = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												d->setName(yystack_[2].value.as < std::string > ());
												drv.last_table_column->setDefaultValue(d);
											}
#line 2903 "sqlite3_parser.cpp"
    break;

  case 232: // columnconstraint: optional_constraintname "DEFAULT" allowed_keywords_as_identifier
#line 717 "sqlite3_parser.yy"
                                                                                        {	// We must allow the same keywords as unquoted default values as in the columnid context.
												// But we do not use columnid here in order to avoid reduce/reduce conflicts.
												auto d = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												d->setName(yystack_[2].value.as < std::string > ());
												drv.last_table_column->setDefaultValue(d);
											}
#line 2914 "sqlite3_parser.cpp"
    break;

  case 233: // columnconstraint: optional_constraintname "DEFAULT" "IF"
#line 723 "sqlite3_parser.yy"
                                                                                        {	// Same as above.
												auto d = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												d->setName(yystack_[2].value.as < std::string > ());
												drv.last_table_column->setDefaultValue(d);
											}
#line 2924 "sqlite3_parser.cpp"
    break;

  case 234: // columnconstraint: optional_constraintname "DEFAULT" "(" expr ")"
#line 728 "sqlite3_parser.yy"
                                                                                        {
												auto d = std::make_shared<sqlb::DefaultConstraint>("(" + yystack_[1].value.as < std::string > () + ")");
												d->setName(yystack_[4].value.as < std::string > ());
												drv.last_table_column->setDefaultValue(d);
											}
#line 2934 "sqlite3_parser.cpp"
    break;

  case 235: // columnconstraint: optional_constraintname "COLLATE" id
#line 733 "sqlite3_parser.yy"
                                                                                        {
												auto c = std::make_shared<sqlb::CollateConstraint>(yystack_[0].value.as < std::string > ());
												c->setName(yystack_[2].value.as < std::string > ());
												drv.last_table_column->setCollation(c);
											}
#line 2944 "sqlite3_parser.cpp"
    break;

  case 236: // columnconstraint: optional_constraintname "REFERENCES" tableid optional_columnid_with_paren_list optional_fk_clause
#line 738 "sqlite3_parser.yy"
                                                                                                                {	// TODO Solve shift/reduce conflict. It is not super important though as shifting seems to be right here.
												auto fk = std::make_shared<sqlb::ForeignKeyClause>();
												fk->setName(yystack_[4].value.as < std::string > ());
												fk->setTable(yystack_[2].value.as < std::string > ());
												fk->setColumns(yystack_[1].value.as < sqlb::StringVector > ());
												fk->setConstraint(yystack_[0].value.as < std::string > ());
												yylhs.value.as < std::pair<std::shared_ptr<sqlb::PrimaryKeyConstraint>, std::shared_ptr<sqlb::ForeignKeyClause>> > ().second = fk;
											}
#line 2957 "sqlite3_parser.cpp"
    break;

  case 237: // columnconstraint: optional_constraintname optional_always_generated "AS" "(" expr ")" optional_storage_identifier
#line 746 "sqlite3_parser.yy"
                                                                                                        {		// TODO Solve shift/reduce conflict.
												auto g = std::make_shared<sqlb::GeneratedColumnConstraint>(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ());
												g->setName(yystack_[6].value.as < std::string > ());

												drv.last_table_column->setGenerated(g);

												// This is a hack which removes any "GENERATED ALWAYS" from the end of the type name.
												// As of now these are shifted to the type instead of reducing the type when seeing the GENERATED identifier.
												// TODO Remove this once the grammar is conflict free
												const std::string generated_always = "GENERATED ALWAYS";
												if(drv.last_table_column->type().size() >= generated_always.size() && drv.last_table_column->type().compare(drv.last_table_column->type().size() - generated_always.size(), generated_always.size(), generated_always) == 0)
												{
													std::string type = drv.last_table_column->type().substr(0, drv.last_table_column->type().size()-generated_always.size());
													if(type.back() == ' ')
														type.pop_back();
													drv.last_table_column->setType(type);
												}
											}
#line 2980 "sqlite3_parser.cpp"
    break;

  case 238: // columnconstraint_list: %empty
#line 767 "sqlite3_parser.yy"
                                                        { }
#line 2986 "sqlite3_parser.cpp"
    break;

  case 239: // columnconstraint_list: columnconstraint_list columnconstraint
#line 768 "sqlite3_parser.yy"
                                                        {
								yylhs.value.as < TableConstraints > () = yystack_[1].value.as < TableConstraints > ();

								// Primary key and foreign key constraints are converted to table constraints
								// because we cannot store them as column constraints at the moment.
								if(yystack_[0].value.as < std::pair<std::shared_ptr<sqlb::PrimaryKeyConstraint>, std::shared_ptr<sqlb::ForeignKeyClause>> > ().first)
									yylhs.value.as < TableConstraints > ().index.insert(std::make_pair(sqlb::IndexedColumnVector{sqlb::IndexedColumn(drv.last_table_column->name(), false)}, yystack_[0].value.as < std::pair<std::shared_ptr<sqlb::PrimaryKeyConstraint>, std::shared_ptr<sqlb::ForeignKeyClause>> > ().first));
								if(yystack_[0].value.as < std::pair<std::shared_ptr<sqlb::PrimaryKeyConstraint>, std::shared_ptr<sqlb::ForeignKeyClause>> > ().second)
									yylhs.value.as < TableConstraints > ().fk.insert(std::make_pair(sqlb::StringVector{drv.last_table_column->name()}, yystack_[0].value.as < std::pair<std::shared_ptr<sqlb::PrimaryKeyConstraint>, std::shared_ptr<sqlb::ForeignKeyClause>> > ().second));
							}
#line 3001 "sqlite3_parser.cpp"
    break;

  case 240: // columndef: columnid optional_typename
#line 781 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::shared_ptr<sqlb::Field> > () = std::make_shared<sqlb::Field>(yystack_[1].value.as < std::string > (), yystack_[0].value.as < std::string > ()); drv.last_table_column = yylhs.value.as < std::shared_ptr<sqlb::Field> > (); }
#line 3007 "sqlite3_parser.cpp"
    break;

  case 241: // columndef_list: columndef columnconstraint_list
#line 785 "sqlite3_parser.yy"
                                                                { yylhs.value.as < ColumnList > ().first.push_back(yystack_[1].value.as < std::shared_ptr<sqlb::Field> > ()); yylhs.value.as < ColumnList > ().second = yystack_[0].value.as < TableConstraints > (); }
#line 3013 "sqlite3_parser.cpp"
    break;

  case 242: // columndef_list: columndef_list "," columndef columnconstraint_list
#line 786 "sqlite3_parser.yy"
                                                                { yylhs.value.as < ColumnList > () = yystack_[3].value.as < ColumnList > (); yylhs.value.as < ColumnList > ().first.push_back(yystack_[1].value.as < std::shared_ptr<sqlb::Field> > ()); yylhs.value.as < ColumnList > ().second.index.insert(yystack_[0].value.as < TableConstraints > ().index.begin(), yystack_[0].value.as < TableConstraints > ().index.end()); yylhs.value.as < ColumnList > ().second.fk.insert(yystack_[0].value.as < TableConstraints > ().fk.begin(), yystack_[0].value.as < TableConstraints > ().fk.end()); }
#line 3019 "sqlite3_parser.cpp"
    break;

  case 243: // optional_constraintname: %empty
#line 790 "sqlite3_parser.yy"
                                                        { }
#line 3025 "sqlite3_parser.cpp"
    break;

  case 244: // optional_constraintname: "CONSTRAINT" id
#line 791 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3031 "sqlite3_parser.cpp"
    break;

  case 245: // optional_constraintname: "CONSTRAINT" "string literal"
#line 792 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3037 "sqlite3_parser.cpp"
    break;

  case 246: // columnid_list: columnid
#line 796 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = sqlb::StringVector(1, yystack_[0].value.as < std::string > ()); }
#line 3043 "sqlite3_parser.cpp"
    break;

  case 247: // columnid_list: columnid_list "," columnid
#line 797 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = yystack_[2].value.as < sqlb::StringVector > (); yylhs.value.as < sqlb::StringVector > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 3049 "sqlite3_parser.cpp"
    break;

  case 248: // optional_columnid_with_paren_list: %empty
#line 801 "sqlite3_parser.yy"
                                                        { }
#line 3055 "sqlite3_parser.cpp"
    break;

  case 249: // optional_columnid_with_paren_list: "(" columnid_list ")"
#line 802 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = yystack_[1].value.as < sqlb::StringVector > (); }
#line 3061 "sqlite3_parser.cpp"
    break;

  case 250: // fk_clause_part: "ON" "DELETE" "SET" "NULL"
#line 806 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3067 "sqlite3_parser.cpp"
    break;

  case 251: // fk_clause_part: "ON" "DELETE" "SET" "DEFAULT"
#line 807 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3073 "sqlite3_parser.cpp"
    break;

  case 252: // fk_clause_part: "ON" "DELETE" "CASCADE"
#line 808 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3079 "sqlite3_parser.cpp"
    break;

  case 253: // fk_clause_part: "ON" "DELETE" "RESTRICT"
#line 809 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3085 "sqlite3_parser.cpp"
    break;

  case 254: // fk_clause_part: "ON" "DELETE" "NO" "ACTION"
#line 810 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3091 "sqlite3_parser.cpp"
    break;

  case 255: // fk_clause_part: "ON" "UPDATE" "SET" "NULL"
#line 811 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3097 "sqlite3_parser.cpp"
    break;

  case 256: // fk_clause_part: "ON" "UPDATE" "SET" "DEFAULT"
#line 812 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3103 "sqlite3_parser.cpp"
    break;

  case 257: // fk_clause_part: "ON" "UPDATE" "CASCADE"
#line 813 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3109 "sqlite3_parser.cpp"
    break;

  case 258: // fk_clause_part: "ON" "UPDATE" "RESTRICT"
#line 814 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3115 "sqlite3_parser.cpp"
    break;

  case 259: // fk_clause_part: "ON" "UPDATE" "NO" "ACTION"
#line 815 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3121 "sqlite3_parser.cpp"
    break;

  case 260: // fk_clause_part: "ON" "INSERT" "SET" "NULL"
#line 816 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3127 "sqlite3_parser.cpp"
    break;

  case 261: // fk_clause_part: "ON" "INSERT" "SET" "DEFAULT"
#line 817 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3133 "sqlite3_parser.cpp"
    break;

  case 262: // fk_clause_part: "ON" "INSERT" "CASCADE"
#line 818 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3139 "sqlite3_parser.cpp"
    break;

  case 263: // fk_clause_part: "ON" "INSERT" "RESTRICT"
#line 819 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3145 "sqlite3_parser.cpp"
    break;

  case 264: // fk_clause_part: "ON" "INSERT" "NO" "ACTION"
#line 820 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3151 "sqlite3_parser.cpp"
    break;

  case 265: // fk_clause_part: "MATCH" id
#line 821 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3157 "sqlite3_parser.cpp"
    break;

  case 266: // fk_clause_part_list: fk_clause_part
#line 825 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3163 "sqlite3_parser.cpp"
    break;

  case 267: // fk_clause_part_list: fk_clause_part_list fk_clause_part
#line 826 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3169 "sqlite3_parser.cpp"
    break;

  case 268: // optional_fk_clause: %empty
#line 830 "sqlite3_parser.yy"
                                                                        { }
#line 3175 "sqlite3_parser.cpp"
    break;

  case 269: // optional_fk_clause: fk_clause_part_list
#line 831 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3181 "sqlite3_parser.cpp"
    break;

  case 270: // optional_fk_clause: fk_clause_part_list "DEFERRABLE" "INITIALLY" "DEFERRED"
#line 832 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3187 "sqlite3_parser.cpp"
    break;

  case 271: // optional_fk_clause: fk_clause_part_list "DEFERRABLE" "INITIALLY" "IMMEDIATE"
#line 833 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3193 "sqlite3_parser.cpp"
    break;

  case 272: // optional_fk_clause: fk_clause_part_list "DEFERRABLE"
#line 834 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3199 "sqlite3_parser.cpp"
    break;

  case 273: // optional_fk_clause: fk_clause_part_list "NOT" "DEFERRABLE" "INITIALLY" "DEFERRED"
#line 835 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3205 "sqlite3_parser.cpp"
    break;

  case 274: // optional_fk_clause: fk_clause_part_list "NOT" "DEFERRABLE" "INITIALLY" "IMMEDIATE"
#line 836 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3211 "sqlite3_parser.cpp"
    break;

  case 275: // optional_fk_clause: fk_clause_part_list "NOT" "DEFERRABLE"
#line 837 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3217 "sqlite3_parser.cpp"
    break;

  case 276: // optional_fk_clause: "DEFERRABLE" "INITIALLY" "DEFERRED"
#line 838 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3223 "sqlite3_parser.cpp"
    break;

  case 277: // optional_fk_clause: "DEFERRABLE" "INITIALLY" "IMMEDIATE"
#line 839 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3229 "sqlite3_parser.cpp"
    break;

  case 278: // optional_fk_clause: "DEFERRABLE"
#line 840 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3235 "sqlite3_parser.cpp"
    break;

  case 279: // optional_fk_clause: "NOT" "DEFERRABLE" "INITIALLY" "DEFERRED"
#line 841 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3241 "sqlite3_parser.cpp"
    break;

  case 280: // optional_fk_clause: "NOT" "DEFERRABLE" "INITIALLY" "IMMEDIATE"
#line 842 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3247 "sqlite3_parser.cpp"
    break;

  case 281: // optional_fk_clause: "NOT" "DEFERRABLE"
#line 843 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3253 "sqlite3_parser.cpp"
    break;

  case 282: // tableconstraint: optional_constraintname "PRIMARY" "KEY" "(" indexed_column_list ")" optional_conflictclause
#line 847 "sqlite3_parser.yy"
                                                                                                        {
														auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>();
														pk->setName(yystack_[6].value.as < std::string > ());
														pk->setConflictAction(yystack_[0].value.as < std::string > ());
														yylhs.value.as < TableConstraints > ().index.insert(std::make_pair(yystack_[2].value.as < sqlb::IndexedColumnVector > (), pk));
													}
#line 3264 "sqlite3_parser.cpp"
    break;

  case 283: // tableconstraint: optional_constraintname "PRIMARY" "KEY" "(" indexed_column_list "AUTOINCREMENT" ")" optional_conflictclause
#line 853 "sqlite3_parser.yy"
                                                                                                                {
														auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>();
														pk->setName(yystack_[7].value.as < std::string > ());
														pk->setConflictAction(yystack_[0].value.as < std::string > ());
														pk->setAutoIncrement(true);
														yylhs.value.as < TableConstraints > ().index.insert(std::make_pair(yystack_[3].value.as < sqlb::IndexedColumnVector > (), pk));
													}
#line 3276 "sqlite3_parser.cpp"
    break;

  case 284: // tableconstraint: optional_constraintname "UNIQUE" "(" indexed_column_list ")" optional_conflictclause
#line 860 "sqlite3_parser.yy"
                                                                                                        {
														auto u = std::make_shared<sqlb::UniqueConstraint>();
														u->setName(yystack_[5].value.as < std::string > ());
														u->setConflictAction(yystack_[0].value.as < std::string > ());
														sqlb::StringVector columns;
														yylhs.value.as < TableConstraints > ().index.insert(std::make_pair(yystack_[2].value.as < sqlb::IndexedColumnVector > (), u));
													}
#line 3288 "sqlite3_parser.cpp"
    break;

  case 285: // tableconstraint: optional_constraintname "CHECK" "(" expr ")"
#line 867 "sqlite3_parser.yy"
                                                                                                        {
														auto c = std::make_shared<sqlb::CheckConstraint>(yystack_[1].value.as < std::string > ());
														c->setName(yystack_[4].value.as < std::string > ());
														yylhs.value.as < TableConstraints > ().check.push_back(c);
													}
#line 3298 "sqlite3_parser.cpp"
    break;

  case 286: // tableconstraint: optional_constraintname "FOREIGN" "KEY" "(" columnid_list ")" "REFERENCES" tableid optional_columnid_with_paren_list optional_fk_clause
#line 872 "sqlite3_parser.yy"
                                                                                                                                                {
														auto f = std::make_shared<sqlb::ForeignKeyClause>(yystack_[2].value.as < std::string > (), yystack_[1].value.as < sqlb::StringVector > (), yystack_[0].value.as < std::string > ());
														f->setName(yystack_[9].value.as < std::string > ());
														yylhs.value.as < TableConstraints > ().fk.insert(std::make_pair(yystack_[5].value.as < sqlb::StringVector > (), f));
													}
#line 3308 "sqlite3_parser.cpp"
    break;

  case 287: // tableconstraint_list: tableconstraint
#line 880 "sqlite3_parser.yy"
                                                        { yylhs.value.as < TableConstraints > () = yystack_[0].value.as < TableConstraints > (); }
#line 3314 "sqlite3_parser.cpp"
    break;

  case 288: // tableconstraint_list: tableconstraint_list "," tableconstraint
#line 881 "sqlite3_parser.yy"
                                                        { yylhs.value.as < TableConstraints > () = yystack_[2].value.as < TableConstraints > (); yylhs.value.as < TableConstraints > ().index.insert(yystack_[0].value.as < TableConstraints > ().index.begin(), yystack_[0].value.as < TableConstraints > ().index.end()); yylhs.value.as < TableConstraints > ().fk.insert(yystack_[0].value.as < TableConstraints > ().fk.begin(), yystack_[0].value.as < TableConstraints > ().fk.end()); std::copy(yystack_[0].value.as < TableConstraints > ().check.begin(), yystack_[0].value.as < TableConstraints > ().check.end(), std::back_inserter(yylhs.value.as < TableConstraints > ().check)); }
#line 3320 "sqlite3_parser.cpp"
    break;

  case 289: // tableconstraint_list: tableconstraint_list tableconstraint
#line 882 "sqlite3_parser.yy"
                                                        { yylhs.value.as < TableConstraints > () = yystack_[1].value.as < TableConstraints > (); yylhs.value.as < TableConstraints > ().index.insert(yystack_[0].value.as < TableConstraints > ().index.begin(), yystack_[0].value.as < TableConstraints > ().index.end()); yylhs.value.as < TableConstraints > ().fk.insert(yystack_[0].value.as < TableConstraints > ().fk.begin(), yystack_[0].value.as < TableConstraints > ().fk.end()); std::copy(yystack_[0].value.as < TableConstraints > ().check.begin(), yystack_[0].value.as < TableConstraints > ().check.end(), std::back_inserter(yylhs.value.as < TableConstraints > ().check)); }
#line 3326 "sqlite3_parser.cpp"
    break;

  case 290: // optional_tableconstraint_list: %empty
#line 886 "sqlite3_parser.yy"
                                                        { }
#line 3332 "sqlite3_parser.cpp"
    break;

  case 291: // optional_tableconstraint_list: "," tableconstraint_list
#line 887 "sqlite3_parser.yy"
                                                        { yylhs.value.as < TableConstraints > () = yystack_[0].value.as < TableConstraints > (); }
#line 3338 "sqlite3_parser.cpp"
    break;

  case 292: // createtable_stmt: "CREATE" optional_temporary "TABLE" optional_if_not_exists tableid_with_uninteresting_schema "AS" select_stmt
#line 891 "sqlite3_parser.yy"
                                                                                                                        {
										yylhs.value.as < sqlb::TablePtr > () = std::make_shared<sqlb::Table>(yystack_[2].value.as < std::string > ());
										yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
									}
#line 3347 "sqlite3_parser.cpp"
    break;

  case 293: // createtable_stmt: "CREATE" optional_temporary "TABLE" optional_if_not_exists tableid_with_uninteresting_schema "(" columndef_list optional_tableconstraint_list ")" optional_tableoptions_list
#line 895 "sqlite3_parser.yy"
                                                                                                                                                                                                {
										yylhs.value.as < sqlb::TablePtr > () = std::make_shared<sqlb::Table>(yystack_[5].value.as < std::string > ());
										yylhs.value.as < sqlb::TablePtr > ()->setWithoutRowidTable(yystack_[0].value.as < std::bitset<sqlb::Table::NumOptions> > ().test(sqlb::Table::WithoutRowid));
										yylhs.value.as < sqlb::TablePtr > ()->setStrict(yystack_[0].value.as < std::bitset<sqlb::Table::NumOptions> > ().test(sqlb::Table::Strict));
										for(const auto& i : yystack_[2].value.as < TableConstraints > ().index)
											yylhs.value.as < sqlb::TablePtr > ()->addConstraint(i.first, i.second);
										for(const auto& i : yystack_[2].value.as < TableConstraints > ().fk)
											yylhs.value.as < sqlb::TablePtr > ()->addConstraint(i.first, i.second);
										for(const auto& i : yystack_[2].value.as < TableConstraints > ().check)
											yylhs.value.as < sqlb::TablePtr > ()->addConstraint(i);
										yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(true);

										for(const auto& f : yystack_[3].value.as < ColumnList > ().first)
											yylhs.value.as < sqlb::TablePtr > ()->fields.push_back(*f);
										for(const auto& pk : yystack_[3].value.as < ColumnList > ().second.index)
											yylhs.value.as < sqlb::TablePtr > ()->addConstraint(pk.first, pk.second);
										for(const auto& fk : yystack_[3].value.as < ColumnList > ().second.fk)
											yylhs.value.as < sqlb::TablePtr > ()->addConstraint(fk.first, fk.second);
									}
#line 3371 "sqlite3_parser.cpp"
    break;


#line 3375 "sqlite3_parser.cpp"

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


  const short parser::yypact_ninf_ = -349;

  const short parser::yytable_ninf_ = -292;

  const short
  parser::yypact_[] =
  {
      11,   145,   123,   106,  -349,  -349,  -349,  -349,  -349,  -349,
      51,    67,    58,  -349,  -349,   117,   117,   117,    86,  2483,
    2483,  2483,   130,  -349,  -349,  -349,  -349,  -349,  -349,  -349,
    -349,  -349,  -349,  -349,  -349,  -349,  -349,  -349,  -349,  -349,
    -349,  -349,  -349,  -349,  -349,  -349,  -349,  -349,  -349,  -349,
    -349,  -349,  -349,  -349,  -349,  -349,  -349,  -349,  -349,  -349,
    -349,  -349,   168,  -349,  -349,  -349,  -349,  -349,  -349,  -349,
     173,  -349,  -349,    97,   160,    99,  -349,  2574,  2574,    -8,
    2574,  2392,   129,  -349,  -349,  -349,  -349,   242,   247,  -349,
    -349,  -349,  -349,  -349,  -349,  -349,  2574,  -349,   251,  -349,
    -349,  1034,  -349,  1736,  -349,  1947,  -349,  -349,    28,  2301,
     249,  1736,  -349,  1736,  1736,  1736,  1151,   257,  -349,  -349,
    -349,  -349,  1736,  -349,   258,  -349,  -349,  -349,  -349,  -349,
    -349,   102,  -349,  -349,  -349,  -349,   113,  -349,  -349,  -349,
    -349,  -349,  -349,  3480,  3012,  -349,   122,     7,  -349,   -71,
    -349,    61,  -349,   -14,  -349,    34,   -74,   165,  -349,  -349,
    -349,  1736,   -32,   214,  1736,  3592,     9,   683,    -8,  -349,
    1736,  1736,  1736,  1736,  1736,  1736,  1736,  1736,  1736,  1736,
    1736,  1736,  1736,  1736,  1736,  1736,  1736,  1736,  1736,  1736,
    1736,    -8,  1736,  2041,  1268,  -349,  1736,  1736,    80,  -349,
    1736,  1736,  -349,  -349,  -349,   164,  1736,   169,   170,  -349,
    -349,   175,  -349,  -349,   285,    -8,  1853,   262,   222,  -349,
     232,  2574,   236,   286,    38,   316,   246,   248,   318,   282,
    -349,  -349,   231,  -349,    -2,  -349,  -349,   542,  1736,  -349,
    1736,   -30,  3090,   321,   322,   327,   326,  -349,   329,  1736,
     203,   332,  3480,   151,   151,   126,   126,   202,   126,   202,
     296,   179,   179,   254,   254,   254,   254,   179,   179,   202,
     202,  3592,  3168,  -349,   331,   800,   180,  -349,   276,  1385,
     179,   387,   514,  1736,  1736,  2135,  1736,  1736,  -349,  1736,
    3558,  2285,  1736,  -349,  -349,  -349,  -349,  -349,     7,  1736,
    -349,  1736,  -349,  -349,  -349,  -349,  -349,  -349,   236,     2,
     352,   293,  -349,   355,  1736,   356,   357,  1736,  -349,  -349,
     -74,  -349,  1736,  3246,  2245,  1736,  -349,  2574,   245,   250,
    -349,   253,  -349,   252,  -349,    -8,  1736,  1736,  -349,   267,
     358,  1736,  2574,  1736,   302,  1736,  1736,  3324,  2719,   917,
     272,  -349,  2802,  2885,  3664,  1736,  3480,   359,  2685,  2768,
    -349,   236,  2392,    56,    -9,  1736,  2851,  2392,  1736,   277,
    -349,  3480,  -349,  1736,  3402,   361,   364,   366,   368,  -349,
    -349,   179,   179,  -349,  -349,   291,   370,  -349,  -349,  1736,
     179,   179,  1736,  1736,  -349,   294,   371,  1736,  2574,  1736,
    1736,  1736,   179,  -349,  -349,  -349,   340,  -349,   297,   307,
      -8,   328,   -21,  -349,    62,  -349,  -349,  -349,  -349,  -349,
    -349,  2934,  -349,   301,   112,   236,  3480,  -349,  -349,  -349,
    -349,  -349,  -349,  1502,  -349,   179,   179,  -349,  -349,   305,
     375,  -349,   179,   179,   179,  -349,  -349,  2392,   -18,  -349,
     310,    -3,    35,    37,   311,   334,  -349,   -58,   295,   236,
     379,  -349,  -349,   306,  -349,  1619,  -349,  -349,  -349,    96,
    -349,   360,  -349,   -17,  -349,   362,  -349,     4,  -349,   365,
    -349,     8,   108,   317,  -349,  -349,  -349,  2574,  -349,   236,
    -349,  -349,   309,  -349,  -349,  -349,  -349,  -349,  -349,  -349,
    -349,  -349,  -349,  -349,  -349,  -349,   109,   352,  -349,  -349,
    -349,  -349,    56,  -349
  };

  const short
  parser::yydefact_[] =
  {
       0,   185,     0,     2,     4,     5,     6,   201,   202,   186,
       0,     0,     0,     1,     3,   180,   180,   180,     0,     0,
       0,     0,     0,    18,    19,    20,    21,    22,    23,    24,
      61,    60,    62,    25,    26,    27,    28,    29,    30,    31,
      32,    35,    37,    36,    33,    34,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    55,    54,    56,    57,    58,    16,    64,    17,
      63,    59,   191,     0,     0,     0,   181,     0,     0,     0,
       0,     0,     0,    55,    63,   190,   189,   196,     0,    67,
      66,    68,    69,    71,    70,    65,   216,   238,   290,   179,
     292,     0,   199,     0,    76,    78,   217,   240,   243,   243,
       0,     0,   197,     0,     0,     0,     0,    23,    14,    13,
      15,    12,     0,    10,    43,    11,     7,     8,     9,   163,
     161,   166,   162,   167,   168,   173,     0,   169,   174,   175,
     176,   177,   178,   124,   182,   193,     0,     0,    77,     0,
     239,   221,   238,     0,   287,   243,   208,     0,    82,    81,
      83,     0,     0,     0,     0,    84,     0,     0,     0,   198,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   130,     0,     0,     0,   131,
       0,     0,   183,   184,   192,   187,     0,     0,     0,    75,
      74,     0,   245,   244,     0,     0,     0,     0,     0,   226,
       0,     0,   210,     0,   243,     0,     0,     0,     0,   243,
     289,   204,     0,   205,   209,   293,   170,     0,     0,   156,
       0,     0,     0,     0,     0,     0,     0,   128,     0,     0,
       0,   165,   125,    89,    90,    86,    87,    93,    88,    94,
      85,    99,   100,    97,    98,    95,    96,   101,   102,    91,
      92,   106,     0,   172,   109,     0,    63,   139,     0,     0,
     103,   108,   110,     0,     0,     0,     0,     0,   132,     0,
     107,   111,     0,   195,   194,    72,    73,    79,     0,     0,
     235,     0,   233,   230,   231,   232,   229,   222,   210,   182,
     248,     0,   227,     0,     0,     0,     0,     0,   288,   203,
       0,   207,     0,     0,     0,     0,   154,     0,     0,     0,
     157,     0,   129,     0,   126,     0,     0,     0,   135,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   113,     0,
      63,   147,   112,   114,   115,     0,   188,     0,     0,     0,
     225,   210,     0,   268,     0,     0,     0,     0,     0,     0,
     206,   151,   155,     0,     0,     0,     0,     0,     0,   127,
     164,   133,   117,   137,   136,     0,    63,   138,   104,     0,
     116,   118,     0,     0,   143,     0,     0,     0,     0,     0,
       0,     0,   119,    80,   228,   234,   223,   246,     0,   278,
       0,     0,     0,   266,   269,   236,   212,   213,   214,   215,
     211,     0,   285,     0,     0,   210,   152,   153,   171,   159,
     160,   158,   142,     0,   105,   134,   121,   145,   144,     0,
      63,   146,   120,   122,   123,   224,   249,     0,     0,   265,
     281,     0,     0,     0,   272,     0,   267,   218,     0,   210,
       0,   284,   140,     0,   150,     0,   247,   276,   277,     0,
     252,     0,   253,     0,   262,     0,   263,     0,   257,     0,
     258,     0,     0,   275,   219,   220,   237,     0,   282,   210,
     141,   148,     0,   279,   280,   254,   251,   250,   264,   261,
     260,   259,   256,   255,   270,   271,     0,   248,   283,   149,
     273,   274,   268,   286
  };

  const short
  parser::yypgoto_[] =
  {
    -349,  -349,  -349,   176,   -19,   -13,   -68,  -348,   178,    91,
    -349,    72,  -349,  -349,  -349,   -98,  -349,  -349,  -349,  -349,
     241,  -349,  -349,   308,  -258,   198,   101,  -349,  -349,   200,
     199,  -290,  -349,  -349,  -349,  -349,  -223,  -349,  -349,  -305,
    -349,  -349,  -349,  -349,   263,   320,  -349,  -103,    47,   -91,
       3,  -349,   -94,  -135,  -349,  -349,  -349
  };

  const short
  parser::yydefgoto_[] =
  {
       0,     2,     3,   130,   131,   132,    72,    96,   210,   211,
     105,   106,   133,   134,   135,   136,   137,   138,   139,   140,
     162,   141,   142,   143,   100,    19,   204,    11,   293,    73,
     145,   146,     4,   102,     5,    12,   233,   234,   235,   312,
     107,   486,   223,   150,   108,    97,    98,   153,   408,   363,
     413,   414,   415,   154,   155,   110,     6
  };

  const short
  parser::yytable_[] =
  {
      70,    70,    70,   360,   320,   151,    71,    71,    71,    85,
      86,   321,    88,   157,   407,   207,   208,   340,   416,   407,
     230,   238,   239,   325,   326,   225,   231,   369,   104,   451,
     496,   467,  -241,   470,  -241,   202,   243,   148,  -291,   232,
     229,   484,  -242,    67,  -242,   212,    69,   226,   417,   468,
     452,   499,   485,   203,     1,   502,   406,   418,    84,    84,
      87,    84,    94,   497,    71,    71,   244,    71,    95,   250,
     149,   474,   227,   478,   471,   245,   149,    84,   424,   240,
     149,   240,   419,    71,   500,   420,    84,   453,   503,   472,
      94,   396,    71,   228,   318,   473,    95,   370,   231,   466,
     214,   215,    81,   246,   409,   167,    67,   168,   216,    69,
     454,   232,   475,    14,   479,   283,   459,   169,   206,   170,
     461,   151,   209,    13,   217,   277,   205,   476,   206,   480,
     213,    82,   410,   477,   411,   481,    16,   412,   410,   218,
     455,   219,   178,   412,   284,   493,   460,   220,   285,   251,
     221,   333,    15,   310,   488,   286,   287,   504,   510,    17,
     288,   173,   174,   494,    22,   176,   191,   178,   222,   236,
     289,   170,   273,    77,   276,   505,   511,   339,    78,   297,
      71,   298,    18,   341,   508,   342,    76,   171,   172,   173,
     174,   191,   175,   176,   177,   178,   300,   304,   181,   182,
     183,   184,    84,   305,   187,   188,    79,   334,    71,   170,
     171,   172,   173,   174,    20,    21,   176,   351,   178,   191,
      74,    75,   171,   172,   173,   174,    99,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,    80,   191,   385,   189,   101,  -200,     7,     8,   190,
     103,   395,     9,   156,   191,    10,   379,   109,   170,   104,
     164,   166,   171,   172,   173,   174,   350,   175,   176,   177,
     178,   383,    71,   170,   387,   397,   292,   398,   192,   187,
     188,   425,   193,   206,   295,   296,   194,   195,   299,   196,
     197,   307,   198,   199,   191,   432,   200,   170,   437,   439,
     170,   446,   308,   447,   201,   458,   309,   447,    84,   464,
     490,   170,   170,   509,    71,   170,   380,   311,   313,   314,
     315,   317,   316,   386,   149,   161,   319,   328,   329,    71,
     441,   330,   331,   332,   364,   463,   191,   335,   343,   171,
     172,   173,   174,    94,   175,   176,   177,   178,    94,    95,
     181,   182,   183,   184,    95,   362,   187,   188,   365,   367,
     368,   376,   384,   403,   389,   428,   377,   492,   429,   378,
     430,   191,   431,   433,   445,   438,   450,   448,   465,   440,
     469,   482,   483,   489,   487,    71,   337,   506,   495,   357,
     498,   449,   303,   501,   306,   171,   172,   173,   174,   375,
     175,   176,   177,   178,   241,   294,   181,   182,   183,   184,
     361,   144,   187,   188,   423,   224,   512,   456,   513,   507,
       0,   158,   159,   160,   163,     0,     0,   191,    94,   152,
     165,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    84,   237,
       0,     0,   242,     0,    71,     0,     0,     0,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,     0,
     274,     0,   280,     0,   281,   282,     0,     0,   290,   291,
       0,     0,     0,     0,   144,     0,     0,     0,     0,     0,
       0,     0,   171,   172,   173,   174,     0,   175,   176,   177,
     178,     0,     0,   181,   182,   183,   184,     0,     0,   187,
     188,     0,     0,     0,     0,     0,   323,     0,   324,     0,
     171,   172,   173,   174,   191,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   346,
       0,     0,   189,     0,     0,     0,     0,   190,     0,     0,
       0,     0,   191,     0,     0,     0,     0,   165,     0,     0,
       0,   347,   348,     0,   352,   353,     0,   354,     0,     0,
     356,     0,     0,     0,     0,     0,   192,   358,     0,   359,
     193,     0,     0,     0,   194,   195,     0,   196,   197,     0,
     198,   199,   366,     0,   200,   144,     0,     0,     0,     0,
     371,     0,   201,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   381,   382,   322,     0,     0,     0,
       0,   388,     0,   390,   391,     0,     0,     0,     0,     0,
       0,     0,     0,   402,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   421,     0,     0,   144,     0,     0,     0,
       0,   426,     0,     0,     0,     0,   111,   247,     0,     0,
       0,   113,   114,   248,     0,   115,     0,   434,     0,     0,
     435,   436,     0,     0,     0,     0,     0,   442,   443,   444,
      23,    24,    25,     0,     0,     0,    26,     0,     0,    27,
     116,   117,     0,     0,    29,     0,     0,   118,   119,   120,
       0,     0,    33,     0,    34,   249,     0,    35,     0,     0,
      36,   121,    37,    38,     0,     0,    39,    40,     0,    41,
      42,     0,     0,    43,     0,     0,     0,    44,    45,    46,
      47,   122,     0,   123,     0,     0,    48,    49,    50,     0,
     124,    52,     0,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,    61,     0,    83,    63,     0,   125,    64,
       0,     0,     0,    65,     0,     0,    66,    67,   126,   127,
      69,   128,   129,   111,   338,     0,     0,     0,   113,   114,
       0,     0,   115,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    23,    24,    25,
       0,     0,     0,    26,     0,     0,    27,   116,   117,     0,
       0,    29,     0,     0,   118,   119,   120,     0,     0,    33,
       0,    34,     0,     0,    35,     0,     0,    36,   121,    37,
      38,     0,     0,    39,    40,     0,    41,    42,     0,     0,
      43,     0,     0,     0,    44,    45,    46,    47,   122,     0,
     123,     0,     0,    48,    49,    50,     0,   124,    52,     0,
      53,    54,    55,    56,    57,    58,    59,    99,     0,    60,
      61,     0,    83,    63,     0,   125,    64,     0,     0,     0,
      65,     0,     0,    66,    67,   126,   127,    69,   128,   129,
     111,   394,     0,     0,     0,   113,   114,     0,     0,   115,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,    24,    25,     0,     0,     0,
      26,     0,     0,    27,   116,   117,     0,     0,    29,     0,
       0,   118,   119,   120,     0,     0,    33,     0,    34,     0,
       0,    35,     0,     0,    36,   121,    37,    38,     0,     0,
      39,    40,     0,    41,    42,     0,     0,    43,     0,     0,
       0,    44,    45,    46,    47,   122,     0,   123,     0,     0,
      48,    49,    50,     0,   124,    52,     0,    53,    54,    55,
      56,    57,    58,    59,    99,     0,    60,    61,     0,    83,
      63,     0,   125,    64,     0,     0,     0,    65,     0,     0,
      66,    67,   126,   127,    69,   128,   129,   111,   112,     0,
       0,     0,   113,   114,     0,     0,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    23,    24,    25,     0,     0,     0,    26,     0,     0,
      27,   116,   117,     0,     0,    29,     0,     0,   118,   119,
     120,     0,     0,    33,     0,    34,     0,     0,    35,     0,
       0,    36,   121,    37,    38,     0,     0,    39,    40,     0,
      41,    42,     0,     0,    43,     0,     0,     0,    44,    45,
      46,    47,   122,     0,   123,     0,     0,    48,    49,    50,
       0,   124,    52,     0,    53,    54,    55,    56,    57,    58,
      59,     0,     0,    60,    61,     0,    83,    63,     0,   125,
      64,     0,     0,     0,    65,     0,     0,    66,    67,   126,
     127,    69,   128,   129,   111,     0,     0,     0,     0,   113,
     114,     0,     0,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    23,    24,
      25,     0,     0,     0,    26,     0,     0,    27,   116,   117,
       0,     0,    29,     0,     0,   118,   119,   120,     0,     0,
      33,     0,    34,     0,     0,    35,     0,     0,    36,   121,
      37,    38,     0,     0,    39,    40,     0,    41,    42,     0,
       0,    43,     0,     0,     0,    44,    45,    46,    47,   122,
       0,   123,     0,     0,    48,    49,    50,     0,   124,    52,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,    61,     0,    83,    63,     0,   125,    64,     0,     0,
       0,    65,   161,     0,    66,    67,   126,   127,    69,   128,
     129,   111,     0,     0,     0,     0,   113,   114,     0,     0,
     115,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,    24,    25,     0,     0,
       0,    26,     0,     0,    27,   116,   117,     0,     0,    29,
       0,     0,   118,   119,   120,     0,     0,    33,     0,    34,
     278,     0,    35,     0,     0,    36,   121,    37,    38,     0,
       0,    39,    40,     0,    41,    42,     0,     0,    43,     0,
       0,     0,    44,    45,    46,    47,   279,     0,   123,     0,
       0,    48,    49,    50,     0,   124,    52,     0,    53,    54,
      55,    56,    57,    58,    59,     0,     0,    60,    61,     0,
      83,    63,     0,   125,    64,     0,     0,     0,    65,     0,
       0,    66,    67,   126,   127,    69,   128,   129,   111,     0,
       0,     0,     0,   113,   114,     0,     0,   115,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    23,    24,    25,     0,     0,     0,    26,     0,
       0,    27,   116,   117,     0,     0,    29,     0,     0,   118,
     119,   120,     0,     0,    33,     0,    34,   344,     0,    35,
       0,     0,    36,   121,    37,    38,     0,     0,    39,    40,
       0,    41,    42,     0,     0,    43,     0,     0,     0,    44,
      45,    46,    47,   122,     0,   123,     0,     0,    48,    49,
      50,     0,   124,    52,     0,    53,    54,    55,    56,    57,
      58,    59,     0,     0,    60,    61,     0,    83,    63,     0,
     125,    64,     0,     0,     0,    65,     0,     0,    66,    67,
     126,   127,    69,   128,   129,   111,   462,     0,     0,     0,
     113,   114,     0,     0,   115,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
      24,    25,     0,     0,     0,    26,     0,     0,    27,   116,
     117,     0,     0,    29,     0,     0,   118,   119,   120,     0,
       0,    33,     0,    34,     0,     0,    35,     0,     0,    36,
     121,    37,    38,     0,     0,    39,    40,     0,    41,    42,
       0,     0,    43,     0,     0,     0,    44,    45,    46,    47,
     122,     0,   123,     0,     0,    48,    49,    50,     0,   124,
      52,     0,    53,    54,    55,    56,    57,    58,    59,     0,
       0,    60,    61,     0,    83,    63,     0,   125,    64,     0,
       0,     0,    65,     0,     0,    66,    67,   126,   127,    69,
     128,   129,   111,   491,     0,     0,     0,   113,   114,     0,
       0,   115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,    24,    25,     0,
       0,     0,    26,     0,     0,    27,   116,   117,     0,     0,
      29,     0,     0,   118,   119,   120,     0,     0,    33,     0,
      34,     0,     0,    35,     0,     0,    36,   121,    37,    38,
       0,     0,    39,    40,     0,    41,    42,     0,     0,    43,
       0,     0,     0,    44,    45,    46,    47,   122,     0,   123,
       0,     0,    48,    49,    50,     0,   124,    52,     0,    53,
      54,    55,    56,    57,    58,    59,     0,     0,    60,    61,
       0,    83,    63,     0,   125,    64,     0,     0,     0,    65,
       0,     0,    66,    67,   126,   127,    69,   128,   129,   111,
       0,     0,     0,     0,   113,   114,     0,     0,   115,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,    24,    25,     0,     0,     0,    26,
       0,     0,    27,   116,   117,     0,     0,    29,     0,     0,
     118,   119,   120,     0,     0,    33,     0,    34,     0,     0,
      35,     0,     0,    36,   121,    37,    38,     0,     0,    39,
      40,     0,    41,    42,     0,     0,    43,     0,     0,     0,
      44,    45,    46,    47,   122,     0,   123,     0,     0,    48,
      49,    50,     0,   124,    52,     0,    53,    54,    55,    56,
      57,    58,    59,     0,     0,    60,    61,     0,    83,    63,
       0,   125,    64,     0,     0,     0,    65,     0,     0,    66,
      67,   126,   127,    69,   128,   129,   301,     0,     0,     0,
       0,   207,   208,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,    24,    25,     0,     0,     0,    26,     0,     0,    27,
       0,    28,     0,     0,    29,     0,     0,   118,   119,   120,
       0,     0,    33,     0,    34,     0,     0,    35,     0,     0,
      36,   121,    37,    38,     0,     0,    39,    40,   302,    41,
      42,     0,     0,    43,     0,     0,     0,    44,    45,    46,
      47,     0,     0,   123,     0,     0,    48,    49,    50,     0,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
     147,     0,    60,    61,     0,    83,    63,     0,   125,    64,
       0,     0,     0,    65,     0,     0,    66,    67,   126,   127,
      69,   128,     0,     0,    23,    24,    25,     0,     0,     0,
      26,     0,     0,    27,     0,    28,     0,     0,    29,     0,
       0,    30,    31,    32,     0,     0,    33,     0,    34,     0,
       0,    35,     0,     0,    36,     0,    37,    38,     0,     0,
      39,    40,     0,    41,    42,     0,     0,    43,     0,     0,
       0,    44,    45,    46,    47,     0,     0,     0,     0,     0,
      48,    49,    50,     0,    51,    52,     0,    53,    54,    55,
      56,    57,    58,    59,   275,     0,    60,    61,     0,    83,
      63,     0,     0,    64,     0,     0,     0,    65,     0,     0,
      66,    67,     0,    68,    69,     0,     0,     0,    23,    24,
      25,     0,     0,     0,    26,     0,     0,    27,     0,    28,
       0,     0,    29,     0,     0,    30,    31,    32,     0,     0,
      33,     0,    34,     0,     0,    35,     0,     0,    36,     0,
      37,    38,     0,     0,    39,    40,     0,    41,    42,     0,
       0,    43,     0,     0,     0,    44,    45,    46,    47,     0,
       0,     0,     0,     0,    48,    49,    50,     0,    51,    52,
       0,    53,    54,    55,    56,    57,    58,    59,   349,     0,
      60,    61,     0,    83,    63,     0,     0,    64,     0,     0,
       0,    65,     0,     0,    66,    67,     0,    68,    69,     0,
       0,     0,    23,    24,    25,     0,     0,     0,    26,     0,
       0,    27,     0,    28,     0,     0,    29,     0,     0,    30,
      31,    32,     0,     0,    33,     0,    34,     0,     0,    35,
       0,     0,    36,     0,    37,    38,     0,     0,    39,    40,
       0,    41,    42,     0,     0,    43,     0,     0,     0,    44,
      45,    46,    47,     0,     0,     0,     0,     0,    48,    49,
      50,     0,    51,    52,     0,    53,    54,    55,    56,    57,
      58,    59,     0,     0,    60,    61,     0,    83,    63,     0,
       0,    64,     0,     0,     0,    65,     0,     0,    66,    67,
       0,    68,    69,   171,   172,   173,   174,     0,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,     0,     0,     0,   189,     0,     0,     0,     0,
     190,     0,     0,     0,     0,   191,     0,     0,     0,     0,
       0,     0,     0,   171,   172,   173,   174,     0,   175,   176,
     177,   178,     0,     0,   181,   182,   183,   184,     0,   192,
     187,   188,     0,   193,     0,     0,     0,   194,   195,     0,
     196,   197,     0,   198,   199,   191,     0,   200,    23,    24,
      25,     0,     0,     0,    26,   201,     0,    27,     0,    28,
     355,     0,    29,   149,     0,    89,    90,    91,     0,   373,
      33,     0,    34,     0,     0,    35,     0,     0,    36,     0,
      37,    38,     0,     0,    39,    40,    92,    41,    42,     0,
       0,    43,     0,     0,     0,    44,    45,    46,    47,     0,
       0,     0,     0,     0,    48,    49,    50,     0,    51,    52,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,    61,     0,    83,    63,     0,     0,    64,     0,     0,
       0,    65,     0,     0,    66,    67,     0,    93,    69,    23,
      24,    25,     0,     0,     0,    26,     0,     0,    27,     0,
      28,     0,     0,    29,     0,     0,    89,    90,    91,     0,
       0,    33,     0,    34,     0,     0,    35,     0,     0,    36,
       0,    37,    38,     0,     0,    39,    40,    92,    41,    42,
       0,     0,    43,     0,     0,     0,    44,    45,    46,    47,
       0,     0,     0,     0,     0,    48,    49,    50,     0,    51,
      52,     0,    53,    54,    55,    56,    57,    58,    59,     0,
       0,    60,    61,     0,    83,    63,     0,     0,    64,     0,
       0,     0,    65,     0,     0,    66,    67,     0,    93,    69,
      23,    24,    25,     0,     0,     0,    26,     0,     0,    27,
       0,    28,     0,     0,    29,     0,     0,    30,    31,    32,
       0,     0,    33,     0,    34,     0,     0,    35,     0,     0,
      36,     0,    37,    38,     0,     0,    39,    40,     0,    41,
      42,     0,     0,    43,     0,     0,     0,    44,    45,    46,
      47,     0,     0,     0,     0,     0,    48,    49,    50,     0,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,    61,     0,    62,    63,     0,     0,    64,
       0,     0,     0,    65,     0,     0,    66,    67,     0,    68,
      69,    23,    24,    25,     0,     0,     0,    26,     0,     0,
      27,     0,    28,     0,     0,    29,     0,     0,    30,    31,
      32,     0,     0,    33,     0,    34,     0,     0,    35,     0,
       0,    36,     0,    37,    38,     0,     0,    39,    40,     0,
      41,    42,     0,     0,    43,     0,     0,     0,    44,    45,
      46,    47,     0,     0,     0,     0,     0,    48,    49,    50,
       0,    51,    52,     0,    53,    54,    55,    56,    57,    58,
      59,     0,     0,    60,    61,     0,    83,    63,     0,     0,
      64,     0,     0,     0,    65,     0,     0,    66,    67,   404,
      68,    69,     0,   171,   172,   173,   174,     0,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,     0,     0,     0,   189,     0,     0,     0,     0,
     190,     0,     0,     0,     0,   191,     0,   171,   172,   173,
     174,     0,   175,   176,   177,   178,     0,     0,   181,   182,
     183,   184,     0,     0,   187,   188,     0,     0,     0,   192,
       0,     0,     0,   193,     0,     0,     0,   194,   195,   191,
     196,   197,     0,   198,   199,     0,     0,   200,     0,     0,
       0,     0,   405,     0,   393,   201,   171,   172,   173,   174,
       0,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,     0,     0,     0,   189,     0,
       0,     0,     0,   190,     0,     0,     0,     0,   191,     0,
     171,   172,   173,   174,     0,   175,   176,   177,   178,     0,
       0,   181,   182,   183,   184,     0,     0,   187,   188,     0,
       0,     0,   192,     0,     0,     0,   193,     0,     0,     0,
     194,   195,   191,   196,   197,     0,   198,   199,     0,     0,
     200,     0,     0,     0,     0,   422,     0,   399,   201,   171,
     172,   173,   174,     0,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,     0,     0,
       0,   189,     0,     0,     0,     0,   190,     0,     0,     0,
       0,   191,     0,   171,   172,   173,   174,     0,   175,   176,
     177,   178,     0,     0,   181,   182,   183,   184,     0,     0,
     187,   188,     0,     0,     0,   192,     0,     0,     0,   193,
       0,     0,     0,   194,   195,   191,   196,   197,     0,   198,
     199,     0,     0,   200,     0,     0,     0,     0,   457,     0,
     400,   201,   171,   172,   173,   174,     0,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,     0,     0,     0,   189,     0,     0,     0,     0,   190,
       0,     0,     0,     0,   191,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   192,     0,
       0,     0,   193,     0,     0,     0,   194,   195,     0,   196,
     197,     0,   198,   199,     0,     0,   200,     0,     0,     0,
     171,   172,   173,   174,   201,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,     0,
       0,     0,   189,     0,     0,   202,     0,   190,     0,     0,
       0,     0,   191,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   203,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   192,     0,     0,     0,
     193,     0,     0,     0,   194,   195,     0,   196,   197,     0,
     198,   199,     0,     0,   200,     0,     0,     0,   171,   172,
     173,   174,   201,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,     0,     0,     0,
     189,     0,   327,     0,     0,   190,     0,     0,     0,     0,
     191,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   192,     0,     0,     0,   193,     0,
       0,     0,   194,   195,     0,   196,   197,     0,   198,   199,
       0,     0,   200,     0,     0,     0,   171,   172,   173,   174,
     201,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,     0,     0,     0,   189,   336,
       0,     0,     0,   190,     0,     0,     0,     0,   191,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   192,     0,     0,     0,   193,     0,     0,     0,
     194,   195,     0,   196,   197,     0,   198,   199,     0,     0,
     200,     0,     0,     0,   171,   172,   173,   174,   201,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,     0,     0,     0,   189,     0,     0,     0,
       0,   190,     0,     0,     0,     0,   191,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     372,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     192,     0,     0,     0,   193,     0,     0,     0,   194,   195,
       0,   196,   197,     0,   198,   199,     0,     0,   200,     0,
       0,     0,   171,   172,   173,   174,   201,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,     0,     0,     0,   189,   392,     0,     0,     0,   190,
       0,     0,     0,     0,   191,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   192,     0,
       0,     0,   193,     0,     0,     0,   194,   195,     0,   196,
     197,     0,   198,   199,     0,     0,   200,     0,     0,     0,
     171,   172,   173,   174,   201,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,     0,
       0,     0,   189,     0,     0,     0,     0,   190,     0,     0,
       0,     0,   191,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   192,     0,     0,     0,
     193,     0,     0,     0,   194,   195,     0,   196,   197,     0,
     198,   199,     0,     0,   200,     0,     0,     0,   171,   172,
     173,   174,   201,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,     0,     0,     0,
     189,     0,     0,     0,     0,   190,     0,     0,     0,     0,
     191,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   192,     0,     0,     0,   193,     0,
       0,     0,   194,   195,     0,   196,   197,     0,   198,   199,
       0,     0,   200,     0,     0,     0,   171,   172,   173,   174,
     201,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,     0,     0,     0,   189,     0,
       0,     0,     0,   190,     0,     0,     0,     0,   191,     0,
     171,   172,   173,   174,     0,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,     0,
       0,     0,   192,     0,     0,     0,   193,   190,     0,     0,
     194,   195,   191,   196,   197,     0,   198,   199,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   201,     0,
       0,     0,     0,     0,     0,     0,   192,     0,     0,     0,
     193,     0,     0,     0,   194,   195,     0,   196,   197,     0,
     198,   199,   171,   172,   173,   174,     0,   175,   176,   177,
     178,     0,   201,   181,   182,   183,   184,     0,     0,   187,
     188,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   191,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   401
  };

  const short
  parser::yycheck_[] =
  {
      19,    20,    21,   308,     6,   108,    19,    20,    21,    77,
      78,   234,    80,   111,   362,     8,     9,   275,    27,   367,
     155,    53,    54,    53,    54,    39,   100,   317,    96,    50,
      47,    49,     4,    36,     6,    33,    27,   105,     4,   113,
       6,    99,     4,   114,     6,   116,   117,    61,    57,    67,
      71,    47,   110,    51,    43,    47,   361,    66,    77,    78,
      79,    80,    81,    80,    77,    78,    57,    80,    81,   167,
      42,    36,    86,    36,    77,    66,    42,    96,   368,   111,
      42,   111,    91,    96,    80,    94,   105,   108,    80,    92,
     109,   349,   105,   107,   229,    98,   109,   320,   100,   447,
      39,    40,     3,    94,    48,     3,   114,     5,    47,   117,
      48,   113,    77,     7,    77,    35,     4,     4,     6,     6,
     425,   224,   115,     0,    63,   193,     4,    92,     6,    92,
     149,    32,    76,    98,    78,    98,    69,    81,    76,    78,
      78,    80,    16,    81,    64,    49,    34,    86,    68,   168,
      89,   249,   101,   221,   459,    75,    76,    49,    49,   101,
      80,    10,    11,    67,    78,    14,    40,    16,   107,     4,
      90,     6,   191,     5,   193,    67,    67,   275,     5,     4,
     193,     6,    65,     3,   489,     5,    56,     8,     9,    10,
      11,    40,    13,    14,    15,    16,   215,   216,    19,    20,
      21,    22,   221,   216,    25,    26,   109,     4,   221,     6,
       8,     9,    10,    11,    16,    17,    14,   285,    16,    40,
      20,    21,     8,     9,    10,    11,    97,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    81,    40,   341,    30,     3,   101,   102,   103,    35,
       3,   349,   107,     4,    40,   110,     4,     6,     6,   327,
       3,     3,     8,     9,    10,    11,   285,    13,    14,    15,
      16,     4,   285,     6,   342,     3,   112,     5,    64,    25,
      26,     4,    68,     6,   115,   115,    72,    73,     3,    75,
      76,    29,    78,    79,    40,     4,    82,     6,     4,   397,
       6,     4,    80,     6,    90,     4,    74,     6,   327,     4,
       4,     6,     6,     4,   327,     6,   335,    81,    32,     3,
      74,     3,    74,   342,    42,   111,    95,     6,     6,   342,
     398,     4,     6,     4,    41,   433,    40,     5,    62,     8,
       9,    10,    11,   362,    13,    14,    15,    16,   367,   362,
      19,    20,    21,    22,   367,     3,    25,    26,     3,     3,
       3,   116,     4,     4,    62,     4,   116,   465,     4,   116,
       4,    40,     4,     3,    34,     4,    48,    70,     3,   398,
      70,    70,    48,     4,    89,   398,    55,    70,    28,   298,
      28,   410,   216,    28,   216,     8,     9,    10,    11,   327,
      13,    14,    15,    16,   163,   206,    19,    20,    21,    22,
     309,   103,    25,    26,   367,   152,   507,   414,   512,   487,
      -1,   113,   114,   115,   116,    -1,    -1,    40,   447,   109,
     122,    -1,    -1,    -1,   447,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   487,   161,
      -1,    -1,   164,    -1,   487,    -1,    -1,    -1,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,    -1,
     192,    -1,   194,    -1,   196,   197,    -1,    -1,   200,   201,
      -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     8,     9,    10,    11,    -1,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,   238,    -1,   240,    -1,
       8,     9,    10,    11,    40,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    55,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   279,    -1,    -1,
      -1,   283,   284,    -1,   286,   287,    -1,   289,    -1,    -1,
     292,    -1,    -1,    -1,    -1,    -1,    64,   299,    -1,   301,
      68,    -1,    -1,    -1,    72,    73,    -1,    75,    76,    -1,
      78,    79,   314,    -1,    82,   317,    -1,    -1,    -1,    -1,
     322,    -1,    90,   325,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   336,   337,   104,    -1,    -1,    -1,
      -1,   343,    -1,   345,   346,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   355,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   365,    -1,    -1,   368,    -1,    -1,    -1,
      -1,   373,    -1,    -1,    -1,    -1,     3,     4,    -1,    -1,
      -1,     8,     9,    10,    -1,    12,    -1,   389,    -1,    -1,
     392,   393,    -1,    -1,    -1,    -1,    -1,   399,   400,   401,
      27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,
      37,    38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,
      -1,    -1,    49,    -1,    51,    52,    -1,    54,    -1,    -1,
      57,    58,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    -1,    70,    -1,    -1,    -1,    74,    75,    76,
      77,    78,    -1,    80,    -1,    -1,    83,    84,    85,    -1,
      87,    88,    -1,    90,    91,    92,    93,    94,    95,    96,
      -1,    -1,    99,   100,    -1,   102,   103,    -1,   105,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,     3,     4,    -1,    -1,    -1,     8,     9,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      -1,    -1,    -1,    33,    -1,    -1,    36,    37,    38,    -1,
      -1,    41,    -1,    -1,    44,    45,    46,    -1,    -1,    49,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    -1,
      70,    -1,    -1,    -1,    74,    75,    76,    77,    78,    -1,
      80,    -1,    -1,    83,    84,    85,    -1,    87,    88,    -1,
      90,    91,    92,    93,    94,    95,    96,    97,    -1,    99,
     100,    -1,   102,   103,    -1,   105,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,   114,   115,   116,   117,   118,   119,
       3,     4,    -1,    -1,    -1,     8,     9,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      33,    -1,    -1,    36,    37,    38,    -1,    -1,    41,    -1,
      -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    -1,    70,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    80,    -1,    -1,
      83,    84,    85,    -1,    87,    88,    -1,    90,    91,    92,
      93,    94,    95,    96,    97,    -1,    99,   100,    -1,   102,
     103,    -1,   105,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,   114,   115,   116,   117,   118,   119,     3,     4,    -1,
      -1,    -1,     8,     9,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,
      36,    37,    38,    -1,    -1,    41,    -1,    -1,    44,    45,
      46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    -1,    70,    -1,    -1,    -1,    74,    75,
      76,    77,    78,    -1,    80,    -1,    -1,    83,    84,    85,
      -1,    87,    88,    -1,    90,    91,    92,    93,    94,    95,
      96,    -1,    -1,    99,   100,    -1,   102,   103,    -1,   105,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,     3,    -1,    -1,    -1,    -1,     8,
       9,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      29,    -1,    -1,    -1,    33,    -1,    -1,    36,    37,    38,
      -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,    -1,
      49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      -1,    70,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      -1,    80,    -1,    -1,    83,    84,    85,    -1,    87,    88,
      -1,    90,    91,    92,    93,    94,    95,    96,    -1,    -1,
      99,   100,    -1,   102,   103,    -1,   105,   106,    -1,    -1,
      -1,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,     3,    -1,    -1,    -1,    -1,     8,     9,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,
      -1,    33,    -1,    -1,    36,    37,    38,    -1,    -1,    41,
      -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,
      52,    -1,    54,    -1,    -1,    57,    58,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    -1,    70,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    -1,    80,    -1,
      -1,    83,    84,    85,    -1,    87,    88,    -1,    90,    91,
      92,    93,    94,    95,    96,    -1,    -1,    99,   100,    -1,
     102,   103,    -1,   105,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,   114,   115,   116,   117,   118,   119,     3,    -1,
      -1,    -1,    -1,     8,     9,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    -1,    -1,    -1,    33,    -1,
      -1,    36,    37,    38,    -1,    -1,    41,    -1,    -1,    44,
      45,    46,    -1,    -1,    49,    -1,    51,    52,    -1,    54,
      -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    -1,    70,    -1,    -1,    -1,    74,
      75,    76,    77,    78,    -1,    80,    -1,    -1,    83,    84,
      85,    -1,    87,    88,    -1,    90,    91,    92,    93,    94,
      95,    96,    -1,    -1,    99,   100,    -1,   102,   103,    -1,
     105,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,     3,     4,    -1,    -1,    -1,
       8,     9,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,    37,
      38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,
      -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    -1,    70,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    80,    -1,    -1,    83,    84,    85,    -1,    87,
      88,    -1,    90,    91,    92,    93,    94,    95,    96,    -1,
      -1,    99,   100,    -1,   102,   103,    -1,   105,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,   114,   115,   116,   117,
     118,   119,     3,     4,    -1,    -1,    -1,     8,     9,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,
      -1,    -1,    33,    -1,    -1,    36,    37,    38,    -1,    -1,
      41,    -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    -1,    70,
      -1,    -1,    -1,    74,    75,    76,    77,    78,    -1,    80,
      -1,    -1,    83,    84,    85,    -1,    87,    88,    -1,    90,
      91,    92,    93,    94,    95,    96,    -1,    -1,    99,   100,
      -1,   102,   103,    -1,   105,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,   114,   115,   116,   117,   118,   119,     3,
      -1,    -1,    -1,    -1,     8,     9,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,    33,
      -1,    -1,    36,    37,    38,    -1,    -1,    41,    -1,    -1,
      44,    45,    46,    -1,    -1,    49,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    -1,    70,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    80,    -1,    -1,    83,
      84,    85,    -1,    87,    88,    -1,    90,    91,    92,    93,
      94,    95,    96,    -1,    -1,    99,   100,    -1,   102,   103,
      -1,   105,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
     114,   115,   116,   117,   118,   119,     3,    -1,    -1,    -1,
      -1,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,
      -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    59,    60,    -1,    -1,    63,    64,    65,    66,
      67,    -1,    -1,    70,    -1,    -1,    -1,    74,    75,    76,
      77,    -1,    -1,    80,    -1,    -1,    83,    84,    85,    -1,
      87,    88,    -1,    90,    91,    92,    93,    94,    95,    96,
       3,    -1,    99,   100,    -1,   102,   103,    -1,   105,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,   114,   115,   116,
     117,   118,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      33,    -1,    -1,    36,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    -1,    70,    -1,    -1,
      -1,    74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      83,    84,    85,    -1,    87,    88,    -1,    90,    91,    92,
      93,    94,    95,    96,     3,    -1,    99,   100,    -1,   102,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,   114,    -1,   116,   117,    -1,    -1,    -1,    27,    28,
      29,    -1,    -1,    -1,    33,    -1,    -1,    36,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,    -1,
      49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      -1,    70,    -1,    -1,    -1,    74,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    83,    84,    85,    -1,    87,    88,
      -1,    90,    91,    92,    93,    94,    95,    96,     3,    -1,
      99,   100,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,   114,    -1,   116,   117,    -1,
      -1,    -1,    27,    28,    29,    -1,    -1,    -1,    33,    -1,
      -1,    36,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      45,    46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    -1,    70,    -1,    -1,    -1,    74,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    83,    84,
      85,    -1,    87,    88,    -1,    90,    91,    92,    93,    94,
      95,    96,    -1,    -1,    99,   100,    -1,   102,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,   114,
      -1,   116,   117,     8,     9,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     8,     9,    10,    11,    -1,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    -1,    64,
      25,    26,    -1,    68,    -1,    -1,    -1,    72,    73,    -1,
      75,    76,    -1,    78,    79,    40,    -1,    82,    27,    28,
      29,    -1,    -1,    -1,    33,    90,    -1,    36,    -1,    38,
      55,    -1,    41,    42,    -1,    44,    45,    46,    -1,   104,
      49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    -1,
      59,    60,    -1,    -1,    63,    64,    65,    66,    67,    -1,
      -1,    70,    -1,    -1,    -1,    74,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    83,    84,    85,    -1,    87,    88,
      -1,    90,    91,    92,    93,    94,    95,    96,    -1,    -1,
      99,   100,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,   114,    -1,   116,   117,    27,
      28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,
      -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      -1,    59,    60,    -1,    -1,    63,    64,    65,    66,    67,
      -1,    -1,    70,    -1,    -1,    -1,    74,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    83,    84,    85,    -1,    87,
      88,    -1,    90,    91,    92,    93,    94,    95,    96,    -1,
      -1,    99,   100,    -1,   102,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,   114,    -1,   116,   117,
      27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,
      -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    -1,    70,    -1,    -1,    -1,    74,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    83,    84,    85,    -1,
      87,    88,    -1,    90,    91,    92,    93,    94,    95,    96,
      -1,    -1,    99,   100,    -1,   102,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,   114,    -1,   116,
     117,    27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,
      36,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,    45,
      46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    -1,    70,    -1,    -1,    -1,    74,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    83,    84,    85,
      -1,    87,    88,    -1,    90,    91,    92,    93,    94,    95,
      96,    -1,    -1,    99,   100,    -1,   102,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,   114,     4,
     116,   117,    -1,     8,     9,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    40,    -1,     8,     9,    10,
      11,    -1,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    -1,    -1,    25,    26,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    72,    73,    40,
      75,    76,    -1,    78,    79,    -1,    -1,    82,    -1,    -1,
      -1,    -1,     4,    -1,    55,    90,     8,     9,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    -1,
       8,     9,    10,    11,    -1,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    -1,    -1,    25,    26,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      72,    73,    40,    75,    76,    -1,    78,    79,    -1,    -1,
      82,    -1,    -1,    -1,    -1,     4,    -1,    55,    90,     8,
       9,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    40,    -1,     8,     9,    10,    11,    -1,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    -1,    -1,
      25,    26,    -1,    -1,    -1,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    72,    73,    40,    75,    76,    -1,    78,
      79,    -1,    -1,    82,    -1,    -1,    -1,    -1,     4,    -1,
      55,    90,     8,     9,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    72,    73,    -1,    75,
      76,    -1,    78,    79,    -1,    -1,    82,    -1,    -1,    -1,
       8,     9,    10,    11,    90,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    72,    73,    -1,    75,    76,    -1,
      78,    79,    -1,    -1,    82,    -1,    -1,    -1,     8,     9,
      10,    11,    90,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      30,    -1,    32,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    72,    73,    -1,    75,    76,    -1,    78,    79,
      -1,    -1,    82,    -1,    -1,    -1,     8,     9,    10,    11,
      90,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    31,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      72,    73,    -1,    75,    76,    -1,    78,    79,    -1,    -1,
      82,    -1,    -1,    -1,     8,     9,    10,    11,    90,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    72,    73,
      -1,    75,    76,    -1,    78,    79,    -1,    -1,    82,    -1,
      -1,    -1,     8,     9,    10,    11,    90,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    30,    31,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    72,    73,    -1,    75,
      76,    -1,    78,    79,    -1,    -1,    82,    -1,    -1,    -1,
       8,     9,    10,    11,    90,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    72,    73,    -1,    75,    76,    -1,
      78,    79,    -1,    -1,    82,    -1,    -1,    -1,     8,     9,
      10,    11,    90,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    72,    73,    -1,    75,    76,    -1,    78,    79,
      -1,    -1,    82,    -1,    -1,    -1,     8,     9,    10,    11,
      90,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    -1,
       8,     9,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    68,    35,    -1,    -1,
      72,    73,    40,    75,    76,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    72,    73,    -1,    75,    76,    -1,
      78,    79,     8,     9,    10,    11,    -1,    13,    14,    15,
      16,    -1,    90,    19,    20,    21,    22,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    43,   121,   122,   152,   154,   176,   102,   103,   107,
     110,   147,   155,     0,     7,   101,    69,   101,    65,   145,
     145,   145,    78,    27,    28,    29,    33,    36,    38,    41,
      44,    45,    46,    49,    51,    54,    57,    59,    60,    63,
      64,    66,    67,    70,    74,    75,    76,    77,    83,    84,
      85,    87,    88,    90,    91,    92,    93,    94,    95,    96,
      99,   100,   102,   103,   106,   110,   113,   114,   116,   117,
     124,   125,   126,   149,   149,   149,    56,     5,     5,   109,
      81,     3,    32,   102,   124,   126,   126,   124,   126,    44,
      45,    46,    65,   116,   124,   125,   127,   165,   166,    97,
     144,     3,   153,     3,   126,   130,   131,   160,   164,     6,
     175,     3,     4,     8,     9,    12,    37,    38,    44,    45,
      46,    58,    78,    80,    87,   105,   115,   116,   118,   119,
     123,   124,   125,   132,   133,   134,   135,   136,   137,   138,
     139,   141,   142,   143,   143,   150,   151,     3,   126,    42,
     163,   167,   165,   167,   173,   174,     4,   135,   143,   143,
     143,   111,   140,   143,     3,   143,     3,     3,     5,     4,
       6,     8,     9,    10,    11,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    30,
      35,    40,    64,    68,    72,    73,    75,    76,    78,    79,
      82,    90,    33,    51,   146,     4,     6,     8,     9,   115,
     128,   129,   116,   124,    39,    40,    47,    63,    78,    80,
      86,    89,   107,   162,   164,    39,    61,    86,   107,     6,
     173,   100,   113,   156,   157,   158,     4,   143,    53,    54,
     111,   140,   143,    27,    57,    66,    94,     4,    10,    52,
     135,   124,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   124,   143,     3,   124,   126,    52,    78,
     143,   143,   143,    35,    64,    68,    75,    76,    80,    90,
     143,   143,   112,   148,   150,   115,   115,     4,     6,     3,
     124,     3,    65,   123,   124,   125,   128,    29,    80,    74,
     126,    81,   159,    32,     3,    74,    74,     3,   173,    95,
       6,   156,   104,   143,   143,    53,    54,    32,     6,     6,
       4,     6,     4,   135,     4,     5,    31,    55,     4,   135,
     144,     3,     5,    62,    52,    55,    55,   143,   143,     3,
     124,   126,   143,   143,   143,    55,   143,   129,   143,   143,
     159,   146,     3,   169,    41,     3,   143,     3,     3,   151,
     156,   143,    54,   104,   143,   131,   116,   116,   116,     4,
     124,   143,   143,     4,     4,   135,   124,   126,   143,    62,
     143,   143,    31,    55,     4,   135,   144,     3,     5,    55,
      55,    55,   143,     4,     4,     4,   159,   127,   168,    48,
      76,    78,    81,   170,   171,   172,    27,    57,    66,    91,
      94,   143,     4,   168,   151,     4,   143,    54,     4,     4,
       4,     4,     4,     3,   143,   143,   143,     4,     4,   135,
     124,   126,   143,   143,   143,    34,     4,     6,    70,   124,
      48,    50,    71,   108,    48,    78,   170,     4,     4,     4,
      34,   159,     4,   135,     4,     3,   127,    49,    67,    70,
      36,    77,    92,    98,    36,    77,    92,    98,    36,    77,
      92,    98,    70,    48,    99,   110,   161,    89,   159,     4,
       4,     4,   135,    49,    67,    28,    47,    80,    28,    47,
      80,    28,    47,    80,    49,    67,    70,   126,   159,     4,
      49,    67,   169,   172
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   120,   121,   121,   122,   122,   122,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   124,   124,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   126,
     126,   126,   126,   126,   126,   127,   127,   127,   127,   127,
     127,   127,   128,   128,   129,   129,   130,   130,   131,   131,
     131,   132,   132,   132,   132,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   135,   135,   136,   136,   136,   136,
     137,   137,   137,   138,   138,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   140,   140,   141,   141,   141,   141,   142,   142,   142,
     142,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   144,
     145,   145,   146,   146,   146,   147,   147,   148,   148,   149,
     149,   149,   150,   151,   151,   152,   153,   153,   153,   154,
     155,   155,   155,   156,   156,   157,   157,   157,   158,   158,
     159,   159,   159,   159,   159,   159,   160,   160,   161,   161,
     161,   162,   162,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   164,   164,
     165,   166,   166,   167,   167,   167,   168,   168,   169,   169,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   171,   171,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   173,   173,   173,   173,   173,   174,   174,   174,
     175,   175,   176,   176
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
       3,     3,     3,     3,     5,     6,     3,     3,     3,     3,
       3,     3,     4,     4,     4,     4,     5,     5,     5,     5,
       6,     6,     6,     6,     1,     3,     4,     5,     3,     4,
       2,     2,     3,     5,     6,     4,     5,     5,     5,     3,
       7,     8,     6,     5,     6,     6,     6,     4,     8,     9,
       7,     4,     5,     6,     4,     5,     3,     4,     6,     6,
       6,     1,     1,     1,     5,     3,     1,     1,     1,     1,
       3,     6,     3,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     0,     1,     1,     0,     1,     0,     2,     3,
       3,     1,     2,     1,     3,    11,     0,     2,     3,     8,
       0,     1,     1,     2,     1,     1,     3,     2,     0,     1,
       0,     3,     3,     3,     3,     3,     0,     1,     0,     1,
       1,     0,     2,     5,     6,     4,     2,     3,     5,     3,
       3,     3,     3,     3,     5,     3,     5,     7,     0,     2,
       2,     2,     4,     0,     2,     2,     1,     3,     0,     3,
       4,     4,     3,     3,     4,     4,     4,     3,     3,     4,
       4,     4,     3,     3,     4,     2,     1,     2,     0,     1,
       4,     4,     2,     5,     5,     3,     3,     3,     1,     4,
       4,     2,     7,     8,     6,     5,    10,     1,     3,     2,
       0,     2,     7,    10
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
  "\"FOLLOWING\"", "\"FOREIGN\"", "\"FROM\"", "\"GENERATED\"", "\"GLOB\"",
  "\"IF\"", "\"IGNORE\"", "\"IMMEDIATE\"", "\"IN\"", "\"INDEX\"",
  "\"INITIALLY\"", "\"INSERT\"", "\"IS\"", "\"ISNULL\"", "\"KEY\"",
  "\"LIKE\"", "\"MATCH\"", "\"NO\"", "\"NOT\"", "\"NOTNULL\"", "\"NULL\"",
  "\"ON\"", "\"OR\"", "\"OVER\"", "\"PARTITION\"", "\"PRECEDING\"",
  "\"PRIMARY\"", "\"RAISE\"", "\"RANGE\"", "\"REFERENCES\"", "\"REGEXP\"",
  "\"REPLACE\"", "\"RESTRICT\"", "\"RETURNING\"", "\"ROLLBACK\"",
  "\"ROWID\"", "\"ROWS\"", "\"SELECT\"", "\"SET\"", "\"STORED\"",
  "\"STRICT\"", "\"TABLE\"", "\"TEMP\"", "\"TEMPORARY\"", "\"THEN\"",
  "\"TRUE\"", "\"UNBOUNDED\"", "\"UNIQUE\"", "\"UPDATE\"", "\"USING\"",
  "\"VIRTUAL\"", "\"WHEN\"", "\"WHERE\"", "\"WITHOUT\"", "\"identifier\"",
  "\"numeric\"", "\"string literal\"", "\"quoted literal\"",
  "\"blob literal\"", "\"bind parameter\"", "$accept", "sql", "statement",
  "literalvalue", "id", "allowed_keywords_as_identifier", "tableid",
  "columnid", "signednumber", "signednumber_or_numeric",
  "typename_namelist", "type_name", "unary_expr", "binary_expr",
  "like_expr", "exprlist_expr", "function_expr", "isnull_expr",
  "between_expr", "in_expr", "whenthenlist_expr", "case_expr",
  "raise_expr", "expr", "select_stmt", "optional_if_not_exists",
  "optional_sort_order", "optional_unique", "optional_where",
  "tableid_with_uninteresting_schema", "indexed_column",
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
     412,   413,   414,   415,   416,   417,   418,   419,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   442,   443,   447,   448,   449,   450,
     454,   455,   456,   460,   461,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   484,   485,   489,   490,   491,   492,   496,   497,   498,
     499,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,   529,
     537,   538,   542,   543,   544,   552,   553,   557,   558,   562,
     563,   564,   568,   585,   586,   590,   605,   606,   607,   611,
     623,   624,   625,   629,   630,   634,   635,   636,   640,   641,
     645,   646,   647,   648,   649,   650,   654,   655,   659,   660,
     661,   665,   666,   670,   676,   683,   689,   691,   697,   702,
     707,   712,   717,   723,   728,   733,   738,   746,   767,   768,
     781,   785,   786,   790,   791,   792,   796,   797,   801,   802,
     806,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,   817,   818,   819,   820,   821,   825,   826,   830,   831,
     832,   833,   834,   835,   836,   837,   838,   839,   840,   841,
     842,   843,   847,   853,   860,   867,   872,   880,   881,   882,
     886,   887,   891,   895
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
#line 4868 "sqlite3_parser.cpp"

#line 916 "sqlite3_parser.yy"


void sqlb::parser::parser::error(const location_type& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}
