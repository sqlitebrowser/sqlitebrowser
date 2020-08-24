// A Bison parser, made by GNU Bison 3.6.3.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

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

#line 68 "sqlite3_parser.cpp"


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
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
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
#line 161 "sqlite3_parser.cpp"

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
      case 160: // columndef
        value.YY_MOVE_OR_COPY< ColumndefData > (YY_MOVE (that.value));
        break;

      case 142: // optional_if_not_exists
      case 144: // optional_unique
      case 152: // optional_temporary
      case 153: // optional_withoutrowid
      case 157: // optional_always_generated
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case 158: // columnconstraint
      case 168: // tableconstraint
        value.YY_MOVE_OR_COPY< sqlb::ConstraintPtr > (YY_MOVE (that.value));
        break;

      case 159: // columnconstraint_list
      case 169: // tableconstraint_list
      case 170: // optional_tableconstraint_list
        value.YY_MOVE_OR_COPY< sqlb::ConstraintSet > (YY_MOVE (that.value));
        break;

      case 149: // createindex_stmt
        value.YY_MOVE_OR_COPY< sqlb::IndexPtr > (YY_MOVE (that.value));
        break;

      case 147: // indexed_column
        value.YY_MOVE_OR_COPY< sqlb::IndexedColumn > (YY_MOVE (that.value));
        break;

      case 148: // indexed_column_list
        value.YY_MOVE_OR_COPY< sqlb::IndexedColumnVector > (YY_MOVE (that.value));
        break;

      case 163: // columnid_list
      case 164: // optional_columnid_with_paren_list
        value.YY_MOVE_OR_COPY< sqlb::StringVector > (YY_MOVE (that.value));
        break;

      case 151: // createvirtualtable_stmt
      case 171: // createtable_stmt
        value.YY_MOVE_OR_COPY< sqlb::TablePtr > (YY_MOVE (that.value));
        break;

      case 27: // "ABORT"
      case 28: // "ACTION"
      case 29: // "ALWAYS"
      case 30: // "AND"
      case 31: // "AND BETWEEN"
      case 32: // "AS"
      case 33: // "ASC"
      case 34: // "AUTOINCREMENT"
      case 35: // "BETWEEN"
      case 36: // "CASCADE"
      case 37: // "CASE"
      case 38: // "CAST"
      case 39: // "CHECK"
      case 40: // "COLLATE"
      case 41: // "CONFLICT"
      case 42: // "CONSTRAINT"
      case 43: // "CREATE"
      case 44: // "CURRENT_DATE"
      case 45: // "CURRENT_TIME"
      case 46: // "CURRENT_TIMESTAMP"
      case 47: // "DEFAULT"
      case 48: // "DEFERRABLE"
      case 49: // "DEFERRED"
      case 50: // "DELETE"
      case 51: // "DESC"
      case 52: // "DISTINCT"
      case 53: // "ELSE"
      case 54: // "END"
      case 55: // "ESCAPE"
      case 56: // "EXISTS"
      case 57: // "FAIL"
      case 58: // "FALSE"
      case 59: // "FILTER"
      case 60: // "FOLLOWING"
      case 61: // "FOREIGN"
      case 62: // "GENERATED"
      case 63: // "GLOB"
      case 64: // "IF"
      case 65: // "IGNORE"
      case 66: // "IMMEDIATE"
      case 67: // "IN"
      case 68: // "INDEX"
      case 69: // "INITIALLY"
      case 70: // "INSERT"
      case 71: // "IS"
      case 72: // "ISNULL"
      case 73: // "KEY"
      case 74: // "LIKE"
      case 75: // "MATCH"
      case 76: // "NO"
      case 77: // "NOT"
      case 78: // "NOTNULL"
      case 79: // "NULL"
      case 80: // "ON"
      case 81: // "OR"
      case 82: // "OVER"
      case 83: // "PARTITION"
      case 84: // "PRECEDING"
      case 85: // "PRIMARY"
      case 86: // "RAISE"
      case 87: // "RANGE"
      case 88: // "REFERENCES"
      case 89: // "REGEXP"
      case 90: // "REPLACE"
      case 91: // "RESTRICT"
      case 92: // "ROLLBACK"
      case 93: // "ROWID"
      case 94: // "ROWS"
      case 95: // "SELECT"
      case 96: // "SET"
      case 97: // "STORED"
      case 98: // "TABLE"
      case 99: // "TEMP"
      case 100: // "TEMPORARY"
      case 101: // "THEN"
      case 102: // "TRUE"
      case 103: // "UNBOUNDED"
      case 104: // "UNIQUE"
      case 105: // "UPDATE"
      case 106: // "USING"
      case 107: // "VIRTUAL"
      case 108: // "WHEN"
      case 109: // "WHERE"
      case 110: // "WITHOUT"
      case 111: // "identifier"
      case 112: // "numeric"
      case 113: // "string literal"
      case 114: // "quoted literal"
      case 115: // "blob literal"
      case 116: // "bind parameter"
      case 120: // literalvalue
      case 121: // id
      case 122: // allowed_keywords_as_identifier
      case 123: // tableid
      case 124: // columnid
      case 125: // signednumber
      case 126: // signednumber_or_numeric
      case 127: // typename_namelist
      case 128: // type_name
      case 129: // unary_expr
      case 130: // binary_expr
      case 131: // like_expr
      case 132: // exprlist_expr
      case 133: // function_expr
      case 134: // isnull_expr
      case 135: // between_expr
      case 136: // in_expr
      case 137: // whenthenlist_expr
      case 138: // case_expr
      case 139: // raise_expr
      case 140: // expr
      case 141: // select_stmt
      case 143: // optional_sort_order
      case 145: // optional_where
      case 146: // tableid_with_uninteresting_schema
      case 150: // optional_exprlist_with_paren
      case 154: // optional_conflictclause
      case 155: // optional_typename
      case 156: // optional_storage_identifier
      case 162: // optional_constraintname
      case 165: // fk_clause_part
      case 166: // fk_clause_part_list
      case 167: // optional_fk_clause
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 161: // columndef_list
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
      case 160: // columndef
        value.move< ColumndefData > (YY_MOVE (that.value));
        break;

      case 142: // optional_if_not_exists
      case 144: // optional_unique
      case 152: // optional_temporary
      case 153: // optional_withoutrowid
      case 157: // optional_always_generated
        value.move< bool > (YY_MOVE (that.value));
        break;

      case 158: // columnconstraint
      case 168: // tableconstraint
        value.move< sqlb::ConstraintPtr > (YY_MOVE (that.value));
        break;

      case 159: // columnconstraint_list
      case 169: // tableconstraint_list
      case 170: // optional_tableconstraint_list
        value.move< sqlb::ConstraintSet > (YY_MOVE (that.value));
        break;

      case 149: // createindex_stmt
        value.move< sqlb::IndexPtr > (YY_MOVE (that.value));
        break;

      case 147: // indexed_column
        value.move< sqlb::IndexedColumn > (YY_MOVE (that.value));
        break;

      case 148: // indexed_column_list
        value.move< sqlb::IndexedColumnVector > (YY_MOVE (that.value));
        break;

      case 163: // columnid_list
      case 164: // optional_columnid_with_paren_list
        value.move< sqlb::StringVector > (YY_MOVE (that.value));
        break;

      case 151: // createvirtualtable_stmt
      case 171: // createtable_stmt
        value.move< sqlb::TablePtr > (YY_MOVE (that.value));
        break;

      case 27: // "ABORT"
      case 28: // "ACTION"
      case 29: // "ALWAYS"
      case 30: // "AND"
      case 31: // "AND BETWEEN"
      case 32: // "AS"
      case 33: // "ASC"
      case 34: // "AUTOINCREMENT"
      case 35: // "BETWEEN"
      case 36: // "CASCADE"
      case 37: // "CASE"
      case 38: // "CAST"
      case 39: // "CHECK"
      case 40: // "COLLATE"
      case 41: // "CONFLICT"
      case 42: // "CONSTRAINT"
      case 43: // "CREATE"
      case 44: // "CURRENT_DATE"
      case 45: // "CURRENT_TIME"
      case 46: // "CURRENT_TIMESTAMP"
      case 47: // "DEFAULT"
      case 48: // "DEFERRABLE"
      case 49: // "DEFERRED"
      case 50: // "DELETE"
      case 51: // "DESC"
      case 52: // "DISTINCT"
      case 53: // "ELSE"
      case 54: // "END"
      case 55: // "ESCAPE"
      case 56: // "EXISTS"
      case 57: // "FAIL"
      case 58: // "FALSE"
      case 59: // "FILTER"
      case 60: // "FOLLOWING"
      case 61: // "FOREIGN"
      case 62: // "GENERATED"
      case 63: // "GLOB"
      case 64: // "IF"
      case 65: // "IGNORE"
      case 66: // "IMMEDIATE"
      case 67: // "IN"
      case 68: // "INDEX"
      case 69: // "INITIALLY"
      case 70: // "INSERT"
      case 71: // "IS"
      case 72: // "ISNULL"
      case 73: // "KEY"
      case 74: // "LIKE"
      case 75: // "MATCH"
      case 76: // "NO"
      case 77: // "NOT"
      case 78: // "NOTNULL"
      case 79: // "NULL"
      case 80: // "ON"
      case 81: // "OR"
      case 82: // "OVER"
      case 83: // "PARTITION"
      case 84: // "PRECEDING"
      case 85: // "PRIMARY"
      case 86: // "RAISE"
      case 87: // "RANGE"
      case 88: // "REFERENCES"
      case 89: // "REGEXP"
      case 90: // "REPLACE"
      case 91: // "RESTRICT"
      case 92: // "ROLLBACK"
      case 93: // "ROWID"
      case 94: // "ROWS"
      case 95: // "SELECT"
      case 96: // "SET"
      case 97: // "STORED"
      case 98: // "TABLE"
      case 99: // "TEMP"
      case 100: // "TEMPORARY"
      case 101: // "THEN"
      case 102: // "TRUE"
      case 103: // "UNBOUNDED"
      case 104: // "UNIQUE"
      case 105: // "UPDATE"
      case 106: // "USING"
      case 107: // "VIRTUAL"
      case 108: // "WHEN"
      case 109: // "WHERE"
      case 110: // "WITHOUT"
      case 111: // "identifier"
      case 112: // "numeric"
      case 113: // "string literal"
      case 114: // "quoted literal"
      case 115: // "blob literal"
      case 116: // "bind parameter"
      case 120: // literalvalue
      case 121: // id
      case 122: // allowed_keywords_as_identifier
      case 123: // tableid
      case 124: // columnid
      case 125: // signednumber
      case 126: // signednumber_or_numeric
      case 127: // typename_namelist
      case 128: // type_name
      case 129: // unary_expr
      case 130: // binary_expr
      case 131: // like_expr
      case 132: // exprlist_expr
      case 133: // function_expr
      case 134: // isnull_expr
      case 135: // between_expr
      case 136: // in_expr
      case 137: // whenthenlist_expr
      case 138: // case_expr
      case 139: // raise_expr
      case 140: // expr
      case 141: // select_stmt
      case 143: // optional_sort_order
      case 145: // optional_where
      case 146: // tableid_with_uninteresting_schema
      case 150: // optional_exprlist_with_paren
      case 154: // optional_conflictclause
      case 155: // optional_typename
      case 156: // optional_storage_identifier
      case 162: // optional_constraintname
      case 165: // fk_clause_part
      case 166: // fk_clause_part_list
      case 167: // optional_fk_clause
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 161: // columndef_list
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
      case 160: // columndef
        value.copy< ColumndefData > (that.value);
        break;

      case 142: // optional_if_not_exists
      case 144: // optional_unique
      case 152: // optional_temporary
      case 153: // optional_withoutrowid
      case 157: // optional_always_generated
        value.copy< bool > (that.value);
        break;

      case 158: // columnconstraint
      case 168: // tableconstraint
        value.copy< sqlb::ConstraintPtr > (that.value);
        break;

      case 159: // columnconstraint_list
      case 169: // tableconstraint_list
      case 170: // optional_tableconstraint_list
        value.copy< sqlb::ConstraintSet > (that.value);
        break;

      case 149: // createindex_stmt
        value.copy< sqlb::IndexPtr > (that.value);
        break;

      case 147: // indexed_column
        value.copy< sqlb::IndexedColumn > (that.value);
        break;

      case 148: // indexed_column_list
        value.copy< sqlb::IndexedColumnVector > (that.value);
        break;

      case 163: // columnid_list
      case 164: // optional_columnid_with_paren_list
        value.copy< sqlb::StringVector > (that.value);
        break;

      case 151: // createvirtualtable_stmt
      case 171: // createtable_stmt
        value.copy< sqlb::TablePtr > (that.value);
        break;

      case 27: // "ABORT"
      case 28: // "ACTION"
      case 29: // "ALWAYS"
      case 30: // "AND"
      case 31: // "AND BETWEEN"
      case 32: // "AS"
      case 33: // "ASC"
      case 34: // "AUTOINCREMENT"
      case 35: // "BETWEEN"
      case 36: // "CASCADE"
      case 37: // "CASE"
      case 38: // "CAST"
      case 39: // "CHECK"
      case 40: // "COLLATE"
      case 41: // "CONFLICT"
      case 42: // "CONSTRAINT"
      case 43: // "CREATE"
      case 44: // "CURRENT_DATE"
      case 45: // "CURRENT_TIME"
      case 46: // "CURRENT_TIMESTAMP"
      case 47: // "DEFAULT"
      case 48: // "DEFERRABLE"
      case 49: // "DEFERRED"
      case 50: // "DELETE"
      case 51: // "DESC"
      case 52: // "DISTINCT"
      case 53: // "ELSE"
      case 54: // "END"
      case 55: // "ESCAPE"
      case 56: // "EXISTS"
      case 57: // "FAIL"
      case 58: // "FALSE"
      case 59: // "FILTER"
      case 60: // "FOLLOWING"
      case 61: // "FOREIGN"
      case 62: // "GENERATED"
      case 63: // "GLOB"
      case 64: // "IF"
      case 65: // "IGNORE"
      case 66: // "IMMEDIATE"
      case 67: // "IN"
      case 68: // "INDEX"
      case 69: // "INITIALLY"
      case 70: // "INSERT"
      case 71: // "IS"
      case 72: // "ISNULL"
      case 73: // "KEY"
      case 74: // "LIKE"
      case 75: // "MATCH"
      case 76: // "NO"
      case 77: // "NOT"
      case 78: // "NOTNULL"
      case 79: // "NULL"
      case 80: // "ON"
      case 81: // "OR"
      case 82: // "OVER"
      case 83: // "PARTITION"
      case 84: // "PRECEDING"
      case 85: // "PRIMARY"
      case 86: // "RAISE"
      case 87: // "RANGE"
      case 88: // "REFERENCES"
      case 89: // "REGEXP"
      case 90: // "REPLACE"
      case 91: // "RESTRICT"
      case 92: // "ROLLBACK"
      case 93: // "ROWID"
      case 94: // "ROWS"
      case 95: // "SELECT"
      case 96: // "SET"
      case 97: // "STORED"
      case 98: // "TABLE"
      case 99: // "TEMP"
      case 100: // "TEMPORARY"
      case 101: // "THEN"
      case 102: // "TRUE"
      case 103: // "UNBOUNDED"
      case 104: // "UNIQUE"
      case 105: // "UPDATE"
      case 106: // "USING"
      case 107: // "VIRTUAL"
      case 108: // "WHEN"
      case 109: // "WHERE"
      case 110: // "WITHOUT"
      case 111: // "identifier"
      case 112: // "numeric"
      case 113: // "string literal"
      case 114: // "quoted literal"
      case 115: // "blob literal"
      case 116: // "bind parameter"
      case 120: // literalvalue
      case 121: // id
      case 122: // allowed_keywords_as_identifier
      case 123: // tableid
      case 124: // columnid
      case 125: // signednumber
      case 126: // signednumber_or_numeric
      case 127: // typename_namelist
      case 128: // type_name
      case 129: // unary_expr
      case 130: // binary_expr
      case 131: // like_expr
      case 132: // exprlist_expr
      case 133: // function_expr
      case 134: // isnull_expr
      case 135: // between_expr
      case 136: // in_expr
      case 137: // whenthenlist_expr
      case 138: // case_expr
      case 139: // raise_expr
      case 140: // expr
      case 141: // select_stmt
      case 143: // optional_sort_order
      case 145: // optional_where
      case 146: // tableid_with_uninteresting_schema
      case 150: // optional_exprlist_with_paren
      case 154: // optional_conflictclause
      case 155: // optional_typename
      case 156: // optional_storage_identifier
      case 162: // optional_constraintname
      case 165: // fk_clause_part
      case 166: // fk_clause_part_list
      case 167: // optional_fk_clause
        value.copy< std::string > (that.value);
        break;

      case 161: // columndef_list
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
      case 160: // columndef
        value.move< ColumndefData > (that.value);
        break;

      case 142: // optional_if_not_exists
      case 144: // optional_unique
      case 152: // optional_temporary
      case 153: // optional_withoutrowid
      case 157: // optional_always_generated
        value.move< bool > (that.value);
        break;

      case 158: // columnconstraint
      case 168: // tableconstraint
        value.move< sqlb::ConstraintPtr > (that.value);
        break;

      case 159: // columnconstraint_list
      case 169: // tableconstraint_list
      case 170: // optional_tableconstraint_list
        value.move< sqlb::ConstraintSet > (that.value);
        break;

      case 149: // createindex_stmt
        value.move< sqlb::IndexPtr > (that.value);
        break;

      case 147: // indexed_column
        value.move< sqlb::IndexedColumn > (that.value);
        break;

      case 148: // indexed_column_list
        value.move< sqlb::IndexedColumnVector > (that.value);
        break;

      case 163: // columnid_list
      case 164: // optional_columnid_with_paren_list
        value.move< sqlb::StringVector > (that.value);
        break;

      case 151: // createvirtualtable_stmt
      case 171: // createtable_stmt
        value.move< sqlb::TablePtr > (that.value);
        break;

      case 27: // "ABORT"
      case 28: // "ACTION"
      case 29: // "ALWAYS"
      case 30: // "AND"
      case 31: // "AND BETWEEN"
      case 32: // "AS"
      case 33: // "ASC"
      case 34: // "AUTOINCREMENT"
      case 35: // "BETWEEN"
      case 36: // "CASCADE"
      case 37: // "CASE"
      case 38: // "CAST"
      case 39: // "CHECK"
      case 40: // "COLLATE"
      case 41: // "CONFLICT"
      case 42: // "CONSTRAINT"
      case 43: // "CREATE"
      case 44: // "CURRENT_DATE"
      case 45: // "CURRENT_TIME"
      case 46: // "CURRENT_TIMESTAMP"
      case 47: // "DEFAULT"
      case 48: // "DEFERRABLE"
      case 49: // "DEFERRED"
      case 50: // "DELETE"
      case 51: // "DESC"
      case 52: // "DISTINCT"
      case 53: // "ELSE"
      case 54: // "END"
      case 55: // "ESCAPE"
      case 56: // "EXISTS"
      case 57: // "FAIL"
      case 58: // "FALSE"
      case 59: // "FILTER"
      case 60: // "FOLLOWING"
      case 61: // "FOREIGN"
      case 62: // "GENERATED"
      case 63: // "GLOB"
      case 64: // "IF"
      case 65: // "IGNORE"
      case 66: // "IMMEDIATE"
      case 67: // "IN"
      case 68: // "INDEX"
      case 69: // "INITIALLY"
      case 70: // "INSERT"
      case 71: // "IS"
      case 72: // "ISNULL"
      case 73: // "KEY"
      case 74: // "LIKE"
      case 75: // "MATCH"
      case 76: // "NO"
      case 77: // "NOT"
      case 78: // "NOTNULL"
      case 79: // "NULL"
      case 80: // "ON"
      case 81: // "OR"
      case 82: // "OVER"
      case 83: // "PARTITION"
      case 84: // "PRECEDING"
      case 85: // "PRIMARY"
      case 86: // "RAISE"
      case 87: // "RANGE"
      case 88: // "REFERENCES"
      case 89: // "REGEXP"
      case 90: // "REPLACE"
      case 91: // "RESTRICT"
      case 92: // "ROLLBACK"
      case 93: // "ROWID"
      case 94: // "ROWS"
      case 95: // "SELECT"
      case 96: // "SET"
      case 97: // "STORED"
      case 98: // "TABLE"
      case 99: // "TEMP"
      case 100: // "TEMPORARY"
      case 101: // "THEN"
      case 102: // "TRUE"
      case 103: // "UNBOUNDED"
      case 104: // "UNIQUE"
      case 105: // "UPDATE"
      case 106: // "USING"
      case 107: // "VIRTUAL"
      case 108: // "WHEN"
      case 109: // "WHERE"
      case 110: // "WITHOUT"
      case 111: // "identifier"
      case 112: // "numeric"
      case 113: // "string literal"
      case 114: // "quoted literal"
      case 115: // "blob literal"
      case 116: // "bind parameter"
      case 120: // literalvalue
      case 121: // id
      case 122: // allowed_keywords_as_identifier
      case 123: // tableid
      case 124: // columnid
      case 125: // signednumber
      case 126: // signednumber_or_numeric
      case 127: // typename_namelist
      case 128: // type_name
      case 129: // unary_expr
      case 130: // binary_expr
      case 131: // like_expr
      case 132: // exprlist_expr
      case 133: // function_expr
      case 134: // isnull_expr
      case 135: // between_expr
      case 136: // in_expr
      case 137: // whenthenlist_expr
      case 138: // case_expr
      case 139: // raise_expr
      case 140: // expr
      case 141: // select_stmt
      case 143: // optional_sort_order
      case 145: // optional_where
      case 146: // tableid_with_uninteresting_schema
      case 150: // optional_exprlist_with_paren
      case 154: // optional_conflictclause
      case 155: // optional_typename
      case 156: // optional_storage_identifier
      case 162: // optional_constraintname
      case 165: // fk_clause_part
      case 166: // fk_clause_part_list
      case 167: // optional_fk_clause
        value.move< std::string > (that.value);
        break;

      case 161: // columndef_list
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
    YYUSE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YYUSE (yykind);
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
      case 160: // columndef
        yylhs.value.emplace< ColumndefData > ();
        break;

      case 142: // optional_if_not_exists
      case 144: // optional_unique
      case 152: // optional_temporary
      case 153: // optional_withoutrowid
      case 157: // optional_always_generated
        yylhs.value.emplace< bool > ();
        break;

      case 158: // columnconstraint
      case 168: // tableconstraint
        yylhs.value.emplace< sqlb::ConstraintPtr > ();
        break;

      case 159: // columnconstraint_list
      case 169: // tableconstraint_list
      case 170: // optional_tableconstraint_list
        yylhs.value.emplace< sqlb::ConstraintSet > ();
        break;

      case 149: // createindex_stmt
        yylhs.value.emplace< sqlb::IndexPtr > ();
        break;

      case 147: // indexed_column
        yylhs.value.emplace< sqlb::IndexedColumn > ();
        break;

      case 148: // indexed_column_list
        yylhs.value.emplace< sqlb::IndexedColumnVector > ();
        break;

      case 163: // columnid_list
      case 164: // optional_columnid_with_paren_list
        yylhs.value.emplace< sqlb::StringVector > ();
        break;

      case 151: // createvirtualtable_stmt
      case 171: // createtable_stmt
        yylhs.value.emplace< sqlb::TablePtr > ();
        break;

      case 27: // "ABORT"
      case 28: // "ACTION"
      case 29: // "ALWAYS"
      case 30: // "AND"
      case 31: // "AND BETWEEN"
      case 32: // "AS"
      case 33: // "ASC"
      case 34: // "AUTOINCREMENT"
      case 35: // "BETWEEN"
      case 36: // "CASCADE"
      case 37: // "CASE"
      case 38: // "CAST"
      case 39: // "CHECK"
      case 40: // "COLLATE"
      case 41: // "CONFLICT"
      case 42: // "CONSTRAINT"
      case 43: // "CREATE"
      case 44: // "CURRENT_DATE"
      case 45: // "CURRENT_TIME"
      case 46: // "CURRENT_TIMESTAMP"
      case 47: // "DEFAULT"
      case 48: // "DEFERRABLE"
      case 49: // "DEFERRED"
      case 50: // "DELETE"
      case 51: // "DESC"
      case 52: // "DISTINCT"
      case 53: // "ELSE"
      case 54: // "END"
      case 55: // "ESCAPE"
      case 56: // "EXISTS"
      case 57: // "FAIL"
      case 58: // "FALSE"
      case 59: // "FILTER"
      case 60: // "FOLLOWING"
      case 61: // "FOREIGN"
      case 62: // "GENERATED"
      case 63: // "GLOB"
      case 64: // "IF"
      case 65: // "IGNORE"
      case 66: // "IMMEDIATE"
      case 67: // "IN"
      case 68: // "INDEX"
      case 69: // "INITIALLY"
      case 70: // "INSERT"
      case 71: // "IS"
      case 72: // "ISNULL"
      case 73: // "KEY"
      case 74: // "LIKE"
      case 75: // "MATCH"
      case 76: // "NO"
      case 77: // "NOT"
      case 78: // "NOTNULL"
      case 79: // "NULL"
      case 80: // "ON"
      case 81: // "OR"
      case 82: // "OVER"
      case 83: // "PARTITION"
      case 84: // "PRECEDING"
      case 85: // "PRIMARY"
      case 86: // "RAISE"
      case 87: // "RANGE"
      case 88: // "REFERENCES"
      case 89: // "REGEXP"
      case 90: // "REPLACE"
      case 91: // "RESTRICT"
      case 92: // "ROLLBACK"
      case 93: // "ROWID"
      case 94: // "ROWS"
      case 95: // "SELECT"
      case 96: // "SET"
      case 97: // "STORED"
      case 98: // "TABLE"
      case 99: // "TEMP"
      case 100: // "TEMPORARY"
      case 101: // "THEN"
      case 102: // "TRUE"
      case 103: // "UNBOUNDED"
      case 104: // "UNIQUE"
      case 105: // "UPDATE"
      case 106: // "USING"
      case 107: // "VIRTUAL"
      case 108: // "WHEN"
      case 109: // "WHERE"
      case 110: // "WITHOUT"
      case 111: // "identifier"
      case 112: // "numeric"
      case 113: // "string literal"
      case 114: // "quoted literal"
      case 115: // "blob literal"
      case 116: // "bind parameter"
      case 120: // literalvalue
      case 121: // id
      case 122: // allowed_keywords_as_identifier
      case 123: // tableid
      case 124: // columnid
      case 125: // signednumber
      case 126: // signednumber_or_numeric
      case 127: // typename_namelist
      case 128: // type_name
      case 129: // unary_expr
      case 130: // binary_expr
      case 131: // like_expr
      case 132: // exprlist_expr
      case 133: // function_expr
      case 134: // isnull_expr
      case 135: // between_expr
      case 136: // in_expr
      case 137: // whenthenlist_expr
      case 138: // case_expr
      case 139: // raise_expr
      case 140: // expr
      case 141: // select_stmt
      case 143: // optional_sort_order
      case 145: // optional_where
      case 146: // tableid_with_uninteresting_schema
      case 150: // optional_exprlist_with_paren
      case 154: // optional_conflictclause
      case 155: // optional_typename
      case 156: // optional_storage_identifier
      case 162: // optional_constraintname
      case 165: // fk_clause_part
      case 166: // fk_clause_part_list
      case 167: // optional_fk_clause
        yylhs.value.emplace< std::string > ();
        break;

      case 161: // columndef_list
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
  case 4:
#line 259 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::IndexPtr > (); }
#line 1431 "sqlite3_parser.cpp"
    break;

  case 5:
#line 260 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::TablePtr > (); }
#line 1437 "sqlite3_parser.cpp"
    break;

  case 6:
#line 261 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::TablePtr > (); }
#line 1443 "sqlite3_parser.cpp"
    break;

  case 7:
#line 269 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1449 "sqlite3_parser.cpp"
    break;

  case 8:
#line 270 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1455 "sqlite3_parser.cpp"
    break;

  case 9:
#line 271 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1461 "sqlite3_parser.cpp"
    break;

  case 10:
#line 272 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1467 "sqlite3_parser.cpp"
    break;

  case 11:
#line 273 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1473 "sqlite3_parser.cpp"
    break;

  case 12:
#line 274 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1479 "sqlite3_parser.cpp"
    break;

  case 13:
#line 275 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1485 "sqlite3_parser.cpp"
    break;

  case 14:
#line 276 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1491 "sqlite3_parser.cpp"
    break;

  case 15:
#line 277 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1497 "sqlite3_parser.cpp"
    break;

  case 16:
#line 281 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1503 "sqlite3_parser.cpp"
    break;

  case 17:
#line 282 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1509 "sqlite3_parser.cpp"
    break;

  case 18:
#line 287 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1515 "sqlite3_parser.cpp"
    break;

  case 19:
#line 288 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1521 "sqlite3_parser.cpp"
    break;

  case 20:
#line 289 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1527 "sqlite3_parser.cpp"
    break;

  case 21:
#line 290 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1533 "sqlite3_parser.cpp"
    break;

  case 22:
#line 291 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1539 "sqlite3_parser.cpp"
    break;

  case 23:
#line 292 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1545 "sqlite3_parser.cpp"
    break;

  case 24:
#line 293 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1551 "sqlite3_parser.cpp"
    break;

  case 25:
#line 294 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1557 "sqlite3_parser.cpp"
    break;

  case 26:
#line 295 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1563 "sqlite3_parser.cpp"
    break;

  case 27:
#line 296 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1569 "sqlite3_parser.cpp"
    break;

  case 28:
#line 297 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1575 "sqlite3_parser.cpp"
    break;

  case 29:
#line 298 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1581 "sqlite3_parser.cpp"
    break;

  case 30:
#line 299 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1587 "sqlite3_parser.cpp"
    break;

  case 31:
#line 300 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1593 "sqlite3_parser.cpp"
    break;

  case 32:
#line 301 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1599 "sqlite3_parser.cpp"
    break;

  case 33:
#line 302 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1605 "sqlite3_parser.cpp"
    break;

  case 34:
#line 303 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1611 "sqlite3_parser.cpp"
    break;

  case 35:
#line 304 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1617 "sqlite3_parser.cpp"
    break;

  case 36:
#line 305 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1623 "sqlite3_parser.cpp"
    break;

  case 37:
#line 306 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1629 "sqlite3_parser.cpp"
    break;

  case 38:
#line 307 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1635 "sqlite3_parser.cpp"
    break;

  case 39:
#line 308 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1641 "sqlite3_parser.cpp"
    break;

  case 40:
#line 309 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1647 "sqlite3_parser.cpp"
    break;

  case 41:
#line 310 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1653 "sqlite3_parser.cpp"
    break;

  case 42:
#line 311 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1659 "sqlite3_parser.cpp"
    break;

  case 43:
#line 312 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1665 "sqlite3_parser.cpp"
    break;

  case 44:
#line 313 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1671 "sqlite3_parser.cpp"
    break;

  case 45:
#line 314 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1677 "sqlite3_parser.cpp"
    break;

  case 46:
#line 315 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1683 "sqlite3_parser.cpp"
    break;

  case 47:
#line 316 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1689 "sqlite3_parser.cpp"
    break;

  case 48:
#line 317 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1695 "sqlite3_parser.cpp"
    break;

  case 49:
#line 318 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1701 "sqlite3_parser.cpp"
    break;

  case 50:
#line 319 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1707 "sqlite3_parser.cpp"
    break;

  case 51:
#line 320 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1713 "sqlite3_parser.cpp"
    break;

  case 52:
#line 321 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1719 "sqlite3_parser.cpp"
    break;

  case 53:
#line 322 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1725 "sqlite3_parser.cpp"
    break;

  case 54:
#line 323 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1731 "sqlite3_parser.cpp"
    break;

  case 55:
#line 324 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1737 "sqlite3_parser.cpp"
    break;

  case 56:
#line 325 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1743 "sqlite3_parser.cpp"
    break;

  case 57:
#line 329 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1749 "sqlite3_parser.cpp"
    break;

  case 58:
#line 330 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1755 "sqlite3_parser.cpp"
    break;

  case 59:
#line 331 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1761 "sqlite3_parser.cpp"
    break;

  case 60:
#line 332 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1767 "sqlite3_parser.cpp"
    break;

  case 61:
#line 333 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1773 "sqlite3_parser.cpp"
    break;

  case 62:
#line 334 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1779 "sqlite3_parser.cpp"
    break;

  case 63:
#line 338 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1785 "sqlite3_parser.cpp"
    break;

  case 64:
#line 339 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1791 "sqlite3_parser.cpp"
    break;

  case 65:
#line 340 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1797 "sqlite3_parser.cpp"
    break;

  case 66:
#line 341 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1803 "sqlite3_parser.cpp"
    break;

  case 67:
#line 342 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1809 "sqlite3_parser.cpp"
    break;

  case 68:
#line 343 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1815 "sqlite3_parser.cpp"
    break;

  case 69:
#line 344 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1821 "sqlite3_parser.cpp"
    break;

  case 70:
#line 348 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1827 "sqlite3_parser.cpp"
    break;

  case 71:
#line 349 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1833 "sqlite3_parser.cpp"
    break;

  case 72:
#line 353 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1839 "sqlite3_parser.cpp"
    break;

  case 73:
#line 354 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1845 "sqlite3_parser.cpp"
    break;

  case 74:
#line 358 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1851 "sqlite3_parser.cpp"
    break;

  case 75:
#line 359 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 1857 "sqlite3_parser.cpp"
    break;

  case 76:
#line 363 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1863 "sqlite3_parser.cpp"
    break;

  case 77:
#line 364 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1869 "sqlite3_parser.cpp"
    break;

  case 78:
#line 365 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + "(" + yystack_[3].value.as < std::string > () + ", " + yystack_[1].value.as < std::string > () + ")"; }
#line 1875 "sqlite3_parser.cpp"
    break;

  case 79:
#line 369 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1881 "sqlite3_parser.cpp"
    break;

  case 80:
#line 370 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1887 "sqlite3_parser.cpp"
    break;

  case 81:
#line 371 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "~" + yystack_[0].value.as < std::string > (); }
#line 1893 "sqlite3_parser.cpp"
    break;

  case 82:
#line 372 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "NOT " + yystack_[0].value.as < std::string > (); }
#line 1899 "sqlite3_parser.cpp"
    break;

  case 83:
#line 376 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " || " + yystack_[0].value.as < std::string > (); }
#line 1905 "sqlite3_parser.cpp"
    break;

  case 84:
#line 377 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " * " + yystack_[0].value.as < std::string > (); }
#line 1911 "sqlite3_parser.cpp"
    break;

  case 85:
#line 378 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " / " + yystack_[0].value.as < std::string > (); }
#line 1917 "sqlite3_parser.cpp"
    break;

  case 86:
#line 379 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " % " + yystack_[0].value.as < std::string > (); }
#line 1923 "sqlite3_parser.cpp"
    break;

  case 87:
#line 380 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " + " + yystack_[0].value.as < std::string > (); }
#line 1929 "sqlite3_parser.cpp"
    break;

  case 88:
#line 381 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " - " + yystack_[0].value.as < std::string > (); }
#line 1935 "sqlite3_parser.cpp"
    break;

  case 89:
#line 382 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " << " + yystack_[0].value.as < std::string > (); }
#line 1941 "sqlite3_parser.cpp"
    break;

  case 90:
#line 383 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >> " + yystack_[0].value.as < std::string > (); }
#line 1947 "sqlite3_parser.cpp"
    break;

  case 91:
#line 384 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " & " + yystack_[0].value.as < std::string > (); }
#line 1953 "sqlite3_parser.cpp"
    break;

  case 92:
#line 385 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " | " + yystack_[0].value.as < std::string > (); }
#line 1959 "sqlite3_parser.cpp"
    break;

  case 93:
#line 386 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " < " + yystack_[0].value.as < std::string > (); }
#line 1965 "sqlite3_parser.cpp"
    break;

  case 94:
#line 387 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <= " + yystack_[0].value.as < std::string > (); }
#line 1971 "sqlite3_parser.cpp"
    break;

  case 95:
#line 388 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " > " + yystack_[0].value.as < std::string > (); }
#line 1977 "sqlite3_parser.cpp"
    break;

  case 96:
#line 389 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >= " + yystack_[0].value.as < std::string > (); }
#line 1983 "sqlite3_parser.cpp"
    break;

  case 97:
#line 390 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " = " + yystack_[0].value.as < std::string > (); }
#line 1989 "sqlite3_parser.cpp"
    break;

  case 98:
#line 391 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " == " + yystack_[0].value.as < std::string > (); }
#line 1995 "sqlite3_parser.cpp"
    break;

  case 99:
#line 392 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " != " + yystack_[0].value.as < std::string > (); }
#line 2001 "sqlite3_parser.cpp"
    break;

  case 100:
#line 393 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <> " + yystack_[0].value.as < std::string > (); }
#line 2007 "sqlite3_parser.cpp"
    break;

  case 101:
#line 394 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IS " + yystack_[0].value.as < std::string > (); }
#line 2013 "sqlite3_parser.cpp"
    break;

  case 102:
#line 395 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2019 "sqlite3_parser.cpp"
    break;

  case 103:
#line 396 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " OR " + yystack_[0].value.as < std::string > (); }
#line 2025 "sqlite3_parser.cpp"
    break;

  case 104:
#line 400 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " LIKE " + yystack_[0].value.as < std::string > (); }
#line 2031 "sqlite3_parser.cpp"
    break;

  case 105:
#line 401 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " GLOB " + yystack_[0].value.as < std::string > (); }
#line 2037 "sqlite3_parser.cpp"
    break;

  case 106:
#line 402 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " MATCH " + yystack_[0].value.as < std::string > (); }
#line 2043 "sqlite3_parser.cpp"
    break;

  case 107:
#line 403 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " REGEXP " + yystack_[0].value.as < std::string > (); }
#line 2049 "sqlite3_parser.cpp"
    break;

  case 108:
#line 404 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT LIKE " + yystack_[0].value.as < std::string > (); }
#line 2055 "sqlite3_parser.cpp"
    break;

  case 109:
#line 405 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT GLOB " + yystack_[0].value.as < std::string > (); }
#line 2061 "sqlite3_parser.cpp"
    break;

  case 110:
#line 406 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT MATCH " + yystack_[0].value.as < std::string > (); }
#line 2067 "sqlite3_parser.cpp"
    break;

  case 111:
#line 407 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT REGEXP " + yystack_[0].value.as < std::string > (); }
#line 2073 "sqlite3_parser.cpp"
    break;

  case 112:
#line 408 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " LIKE " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2079 "sqlite3_parser.cpp"
    break;

  case 113:
#line 409 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " GLOB " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2085 "sqlite3_parser.cpp"
    break;

  case 114:
#line 410 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " MATCH " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2091 "sqlite3_parser.cpp"
    break;

  case 115:
#line 411 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " REGEXP " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2097 "sqlite3_parser.cpp"
    break;

  case 116:
#line 412 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT LIKE " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2103 "sqlite3_parser.cpp"
    break;

  case 117:
#line 413 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT GLOB " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2109 "sqlite3_parser.cpp"
    break;

  case 118:
#line 414 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT MATCH " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2115 "sqlite3_parser.cpp"
    break;

  case 119:
#line 415 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT REGEXP " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2121 "sqlite3_parser.cpp"
    break;

  case 120:
#line 419 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2127 "sqlite3_parser.cpp"
    break;

  case 121:
#line 420 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + ", " + yystack_[0].value.as < std::string > (); }
#line 2133 "sqlite3_parser.cpp"
    break;

  case 122:
#line 424 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2139 "sqlite3_parser.cpp"
    break;

  case 123:
#line 425 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + "(DISTINCT " + yystack_[1].value.as < std::string > () + ")"; }
#line 2145 "sqlite3_parser.cpp"
    break;

  case 124:
#line 426 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + "()"; }
#line 2151 "sqlite3_parser.cpp"
    break;

  case 125:
#line 427 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(*)"; }
#line 2157 "sqlite3_parser.cpp"
    break;

  case 126:
#line 431 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " ISNULL"; }
#line 2163 "sqlite3_parser.cpp"
    break;

  case 127:
#line 432 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " NOTNULL"; }
#line 2169 "sqlite3_parser.cpp"
    break;

  case 128:
#line 433 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " NOT NULL"; }
#line 2175 "sqlite3_parser.cpp"
    break;

  case 129:
#line 437 "sqlite3_parser.yy"
                                                                { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2181 "sqlite3_parser.cpp"
    break;

  case 130:
#line 438 "sqlite3_parser.yy"
                                                                { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2187 "sqlite3_parser.cpp"
    break;

  case 131:
#line 442 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " IN ()"; }
#line 2193 "sqlite3_parser.cpp"
    break;

  case 132:
#line 443 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2199 "sqlite3_parser.cpp"
    break;

  case 133:
#line 444 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2205 "sqlite3_parser.cpp"
    break;

  case 134:
#line 445 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2211 "sqlite3_parser.cpp"
    break;

  case 135:
#line 446 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2217 "sqlite3_parser.cpp"
    break;

  case 136:
#line 447 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 2223 "sqlite3_parser.cpp"
    break;

  case 137:
#line 448 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2229 "sqlite3_parser.cpp"
    break;

  case 138:
#line 449 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2235 "sqlite3_parser.cpp"
    break;

  case 139:
#line 450 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " NOT IN ()"; }
#line 2241 "sqlite3_parser.cpp"
    break;

  case 140:
#line 451 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2247 "sqlite3_parser.cpp"
    break;

  case 141:
#line 452 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2253 "sqlite3_parser.cpp"
    break;

  case 142:
#line 453 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2259 "sqlite3_parser.cpp"
    break;

  case 143:
#line 454 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2265 "sqlite3_parser.cpp"
    break;

  case 144:
#line 455 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 2271 "sqlite3_parser.cpp"
    break;

  case 145:
#line 456 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[8].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2277 "sqlite3_parser.cpp"
    break;

  case 146:
#line 457 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " NOT IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2283 "sqlite3_parser.cpp"
    break;

  case 147:
#line 461 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "WHEN " + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 2289 "sqlite3_parser.cpp"
    break;

  case 148:
#line 462 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " WHEN" + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 2295 "sqlite3_parser.cpp"
    break;

  case 149:
#line 466 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2301 "sqlite3_parser.cpp"
    break;

  case 150:
#line 467 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " END"; }
#line 2307 "sqlite3_parser.cpp"
    break;

  case 151:
#line 468 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2313 "sqlite3_parser.cpp"
    break;

  case 152:
#line 469 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2319 "sqlite3_parser.cpp"
    break;

  case 153:
#line 473 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(IGNORE)"; }
#line 2325 "sqlite3_parser.cpp"
    break;

  case 154:
#line 474 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(ROLLBACK, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2331 "sqlite3_parser.cpp"
    break;

  case 155:
#line 475 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(ABORT, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2337 "sqlite3_parser.cpp"
    break;

  case 156:
#line 476 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(FAIL, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2343 "sqlite3_parser.cpp"
    break;

  case 157:
#line 480 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2349 "sqlite3_parser.cpp"
    break;

  case 158:
#line 481 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2355 "sqlite3_parser.cpp"
    break;

  case 159:
#line 482 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2361 "sqlite3_parser.cpp"
    break;

  case 160:
#line 483 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2367 "sqlite3_parser.cpp"
    break;

  case 161:
#line 484 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2373 "sqlite3_parser.cpp"
    break;

  case 162:
#line 485 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2379 "sqlite3_parser.cpp"
    break;

  case 163:
#line 486 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2385 "sqlite3_parser.cpp"
    break;

  case 164:
#line 487 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2391 "sqlite3_parser.cpp"
    break;

  case 165:
#line 488 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2397 "sqlite3_parser.cpp"
    break;

  case 166:
#line 489 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2403 "sqlite3_parser.cpp"
    break;

  case 167:
#line 490 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "CAST(" + yystack_[3].value.as < std::string > () + " AS " + yystack_[1].value.as < std::string > () + ")"; }
#line 2409 "sqlite3_parser.cpp"
    break;

  case 168:
#line 491 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " COLLATE " + yystack_[0].value.as < std::string > (); }
#line 2415 "sqlite3_parser.cpp"
    break;

  case 169:
#line 492 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2421 "sqlite3_parser.cpp"
    break;

  case 170:
#line 493 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2427 "sqlite3_parser.cpp"
    break;

  case 171:
#line 494 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2433 "sqlite3_parser.cpp"
    break;

  case 172:
#line 495 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2439 "sqlite3_parser.cpp"
    break;

  case 173:
#line 496 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2445 "sqlite3_parser.cpp"
    break;

  case 174:
#line 497 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2451 "sqlite3_parser.cpp"
    break;

  case 175:
#line 506 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "SELECT"; }
#line 2457 "sqlite3_parser.cpp"
    break;

  case 176:
#line 514 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = false; }
#line 2463 "sqlite3_parser.cpp"
    break;

  case 177:
#line 515 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = true; }
#line 2469 "sqlite3_parser.cpp"
    break;

  case 178:
#line 519 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = ""; }
#line 2475 "sqlite3_parser.cpp"
    break;

  case 179:
#line 520 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "ASC"; }
#line 2481 "sqlite3_parser.cpp"
    break;

  case 180:
#line 521 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "DESC"; }
#line 2487 "sqlite3_parser.cpp"
    break;

  case 181:
#line 529 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = false; }
#line 2493 "sqlite3_parser.cpp"
    break;

  case 182:
#line 530 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = true; }
#line 2499 "sqlite3_parser.cpp"
    break;

  case 183:
#line 534 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = ""; }
#line 2505 "sqlite3_parser.cpp"
    break;

  case 184:
#line 535 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2511 "sqlite3_parser.cpp"
    break;

  case 185:
#line 539 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2517 "sqlite3_parser.cpp"
    break;

  case 186:
#line 540 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2523 "sqlite3_parser.cpp"
    break;

  case 187:
#line 541 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2529 "sqlite3_parser.cpp"
    break;

  case 188:
#line 545 "sqlite3_parser.yy"
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
#line 2545 "sqlite3_parser.cpp"
    break;

  case 189:
#line 559 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::IndexedColumnVector > () = sqlb::IndexedColumnVector(1, yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2551 "sqlite3_parser.cpp"
    break;

  case 190:
#line 560 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::IndexedColumnVector > () = yystack_[2].value.as < sqlb::IndexedColumnVector > (); yylhs.value.as < sqlb::IndexedColumnVector > ().push_back(yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2557 "sqlite3_parser.cpp"
    break;

  case 191:
#line 564 "sqlite3_parser.yy"
                                                                                                                                                                {
													yylhs.value.as < sqlb::IndexPtr > () = std::make_shared<sqlb::Index>(yystack_[6].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setTable(yystack_[4].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setUnique(yystack_[9].value.as < bool > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setWhereExpr(yystack_[0].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->fields = yystack_[2].value.as < sqlb::IndexedColumnVector > ();
													yylhs.value.as < sqlb::IndexPtr > ()->setFullyParsed(true);
												}
#line 2570 "sqlite3_parser.cpp"
    break;

  case 192:
#line 579 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = {}; }
#line 2576 "sqlite3_parser.cpp"
    break;

  case 193:
#line 580 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = {}; }
#line 2582 "sqlite3_parser.cpp"
    break;

  case 194:
#line 581 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 2588 "sqlite3_parser.cpp"
    break;

  case 195:
#line 585 "sqlite3_parser.yy"
                                                                                                                                {
													yylhs.value.as < sqlb::TablePtr > () = std::make_shared<sqlb::Table>(yystack_[3].value.as < std::string > ());
													yylhs.value.as < sqlb::TablePtr > ()->setVirtualUsing(yystack_[1].value.as < std::string > ());
													yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
												}
#line 2598 "sqlite3_parser.cpp"
    break;

  case 196:
#line 597 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = false; }
#line 2604 "sqlite3_parser.cpp"
    break;

  case 197:
#line 598 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2610 "sqlite3_parser.cpp"
    break;

  case 198:
#line 599 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2616 "sqlite3_parser.cpp"
    break;

  case 199:
#line 603 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = false; }
#line 2622 "sqlite3_parser.cpp"
    break;

  case 200:
#line 604 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2628 "sqlite3_parser.cpp"
    break;

  case 201:
#line 608 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = ""; }
#line 2634 "sqlite3_parser.cpp"
    break;

  case 202:
#line 609 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2640 "sqlite3_parser.cpp"
    break;

  case 203:
#line 610 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2646 "sqlite3_parser.cpp"
    break;

  case 204:
#line 611 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2652 "sqlite3_parser.cpp"
    break;

  case 205:
#line 612 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2658 "sqlite3_parser.cpp"
    break;

  case 206:
#line 613 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2664 "sqlite3_parser.cpp"
    break;

  case 207:
#line 617 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = ""; }
#line 2670 "sqlite3_parser.cpp"
    break;

  case 208:
#line 618 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2676 "sqlite3_parser.cpp"
    break;

  case 209:
#line 622 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "VIRTUAL"; }
#line 2682 "sqlite3_parser.cpp"
    break;

  case 210:
#line 623 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "STORED"; }
#line 2688 "sqlite3_parser.cpp"
    break;

  case 211:
#line 624 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "VIRTUAL"; }
#line 2694 "sqlite3_parser.cpp"
    break;

  case 212:
#line 628 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = false; }
#line 2700 "sqlite3_parser.cpp"
    break;

  case 213:
#line 629 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2706 "sqlite3_parser.cpp"
    break;

  case 214:
#line 633 "sqlite3_parser.yy"
                                                                                        {
												auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>(sqlb::IndexedColumnVector{sqlb::IndexedColumn("", false, yystack_[1].value.as < std::string > ())});
												pk->setName(yystack_[4].value.as < std::string > ());
												pk->setConflictAction(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > () = pk;
											}
#line 2717 "sqlite3_parser.cpp"
    break;

  case 215:
#line 639 "sqlite3_parser.yy"
                                                                                                        {
												auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>(sqlb::IndexedColumnVector{sqlb::IndexedColumn("", false, yystack_[2].value.as < std::string > ())});
												pk->setName(yystack_[5].value.as < std::string > ());
												pk->setConflictAction(yystack_[1].value.as < std::string > ());
												pk->setAutoIncrement(true);
												yylhs.value.as < sqlb::ConstraintPtr > () = pk;
											}
#line 2729 "sqlite3_parser.cpp"
    break;

  case 216:
#line 646 "sqlite3_parser.yy"
                                                                                        {
												auto nn = std::make_shared<sqlb::NotNullConstraint>();
												nn->setName(yystack_[3].value.as < std::string > ());
												nn->setConflictAction(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > () = nn;
											}
#line 2740 "sqlite3_parser.cpp"
    break;

  case 217:
#line 652 "sqlite3_parser.yy"
                                                                                        {
											}
#line 2747 "sqlite3_parser.cpp"
    break;

  case 218:
#line 654 "sqlite3_parser.yy"
                                                                                        {
												auto u = std::make_shared<sqlb::UniqueConstraint>();
												u->setName(yystack_[2].value.as < std::string > ());
												u->setConflictAction(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > () = u;
											}
#line 2758 "sqlite3_parser.cpp"
    break;

  case 219:
#line 660 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::CheckConstraint>(yystack_[1].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[4].value.as < std::string > ());
											}
#line 2767 "sqlite3_parser.cpp"
    break;

  case 220:
#line 664 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2776 "sqlite3_parser.cpp"
    break;

  case 221:
#line 668 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2785 "sqlite3_parser.cpp"
    break;

  case 222:
#line 672 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2794 "sqlite3_parser.cpp"
    break;

  case 223:
#line 676 "sqlite3_parser.yy"
                                                                                        {	// We must allow the same keywords as unquoted default values as in the columnid context.
												// But we do not use columnid here in order to avoid reduce/reduce conflicts.
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2804 "sqlite3_parser.cpp"
    break;

  case 224:
#line 681 "sqlite3_parser.yy"
                                                                                        {	// Same as above.
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2813 "sqlite3_parser.cpp"
    break;

  case 225:
#line 685 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::DefaultConstraint>("(" + yystack_[1].value.as < std::string > () + ")");
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[4].value.as < std::string > ());
											}
#line 2822 "sqlite3_parser.cpp"
    break;

  case 226:
#line 689 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::CollateConstraint>(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[2].value.as < std::string > ());
											}
#line 2831 "sqlite3_parser.cpp"
    break;

  case 227:
#line 693 "sqlite3_parser.yy"
                                                                                                                {	// TODO Solve shift/reduce conflict. It is not super important though as shifting seems to be right here.
												auto fk = std::make_shared<sqlb::ForeignKeyClause>();
												fk->setName(yystack_[4].value.as < std::string > ());
												fk->setTable(yystack_[2].value.as < std::string > ());
												fk->setColumns(yystack_[1].value.as < sqlb::StringVector > ());
												fk->setConstraint(yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > () = fk;
											}
#line 2844 "sqlite3_parser.cpp"
    break;

  case 228:
#line 701 "sqlite3_parser.yy"
                                                                                                        {		// TODO Solve shift/reduce conflict.
												yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::GeneratedColumnConstraint>(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ());
												yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[6].value.as < std::string > ());
											}
#line 2853 "sqlite3_parser.cpp"
    break;

  case 229:
#line 708 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintSet > () = { yystack_[0].value.as < sqlb::ConstraintPtr > () }; }
#line 2859 "sqlite3_parser.cpp"
    break;

  case 230:
#line 709 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintSet > () = yystack_[1].value.as < sqlb::ConstraintSet > (); yylhs.value.as < sqlb::ConstraintSet > ().insert(yystack_[0].value.as < sqlb::ConstraintPtr > ()); }
#line 2865 "sqlite3_parser.cpp"
    break;

  case 231:
#line 713 "sqlite3_parser.yy"
                                                                {
								sqlb::Field f(yystack_[2].value.as < std::string > (), yystack_[1].value.as < std::string > ());
								sqlb::ConstraintSet table_constraints{};
								for(const auto& c : yystack_[0].value.as < sqlb::ConstraintSet > ())
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
										table_constraints.insert(c);
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
									}
								}

								yylhs.value.as < ColumndefData > () = std::make_tuple(f, table_constraints);
							}
#line 2929 "sqlite3_parser.cpp"
    break;

  case 232:
#line 772 "sqlite3_parser.yy"
                                                        { yylhs.value.as < ColumndefData > () = std::make_tuple(sqlb::Field(yystack_[1].value.as < std::string > (), yystack_[0].value.as < std::string > ()), sqlb::ConstraintSet{}); }
#line 2935 "sqlite3_parser.cpp"
    break;

  case 233:
#line 776 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::vector<ColumndefData> > () = {yystack_[0].value.as < ColumndefData > ()}; }
#line 2941 "sqlite3_parser.cpp"
    break;

  case 234:
#line 777 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::vector<ColumndefData> > () = yystack_[2].value.as < std::vector<ColumndefData> > (); yylhs.value.as < std::vector<ColumndefData> > ().push_back(yystack_[0].value.as < ColumndefData > ()); }
#line 2947 "sqlite3_parser.cpp"
    break;

  case 235:
#line 781 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = ""; }
#line 2953 "sqlite3_parser.cpp"
    break;

  case 236:
#line 782 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2959 "sqlite3_parser.cpp"
    break;

  case 237:
#line 783 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2965 "sqlite3_parser.cpp"
    break;

  case 238:
#line 787 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = sqlb::StringVector(1, yystack_[0].value.as < std::string > ()); }
#line 2971 "sqlite3_parser.cpp"
    break;

  case 239:
#line 788 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = yystack_[2].value.as < sqlb::StringVector > (); yylhs.value.as < sqlb::StringVector > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 2977 "sqlite3_parser.cpp"
    break;

  case 240:
#line 792 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = sqlb::StringVector(); }
#line 2983 "sqlite3_parser.cpp"
    break;

  case 241:
#line 793 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = yystack_[1].value.as < sqlb::StringVector > (); }
#line 2989 "sqlite3_parser.cpp"
    break;

  case 242:
#line 797 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2995 "sqlite3_parser.cpp"
    break;

  case 243:
#line 798 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3001 "sqlite3_parser.cpp"
    break;

  case 244:
#line 799 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3007 "sqlite3_parser.cpp"
    break;

  case 245:
#line 800 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3013 "sqlite3_parser.cpp"
    break;

  case 246:
#line 801 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3019 "sqlite3_parser.cpp"
    break;

  case 247:
#line 802 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3025 "sqlite3_parser.cpp"
    break;

  case 248:
#line 803 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3031 "sqlite3_parser.cpp"
    break;

  case 249:
#line 804 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3037 "sqlite3_parser.cpp"
    break;

  case 250:
#line 805 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3043 "sqlite3_parser.cpp"
    break;

  case 251:
#line 806 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3049 "sqlite3_parser.cpp"
    break;

  case 252:
#line 807 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3055 "sqlite3_parser.cpp"
    break;

  case 253:
#line 808 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3061 "sqlite3_parser.cpp"
    break;

  case 254:
#line 809 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3067 "sqlite3_parser.cpp"
    break;

  case 255:
#line 810 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3073 "sqlite3_parser.cpp"
    break;

  case 256:
#line 811 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3079 "sqlite3_parser.cpp"
    break;

  case 257:
#line 812 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3085 "sqlite3_parser.cpp"
    break;

  case 258:
#line 816 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3091 "sqlite3_parser.cpp"
    break;

  case 259:
#line 817 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3097 "sqlite3_parser.cpp"
    break;

  case 260:
#line 821 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = ""; }
#line 3103 "sqlite3_parser.cpp"
    break;

  case 261:
#line 822 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3109 "sqlite3_parser.cpp"
    break;

  case 262:
#line 823 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3115 "sqlite3_parser.cpp"
    break;

  case 263:
#line 824 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3121 "sqlite3_parser.cpp"
    break;

  case 264:
#line 825 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3127 "sqlite3_parser.cpp"
    break;

  case 265:
#line 826 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3133 "sqlite3_parser.cpp"
    break;

  case 266:
#line 827 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3139 "sqlite3_parser.cpp"
    break;

  case 267:
#line 828 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3145 "sqlite3_parser.cpp"
    break;

  case 268:
#line 829 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3151 "sqlite3_parser.cpp"
    break;

  case 269:
#line 830 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3157 "sqlite3_parser.cpp"
    break;

  case 270:
#line 831 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3163 "sqlite3_parser.cpp"
    break;

  case 271:
#line 832 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3169 "sqlite3_parser.cpp"
    break;

  case 272:
#line 833 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3175 "sqlite3_parser.cpp"
    break;

  case 273:
#line 834 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3181 "sqlite3_parser.cpp"
    break;

  case 274:
#line 838 "sqlite3_parser.yy"
                                                                                                        {
														auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>(yystack_[2].value.as < sqlb::IndexedColumnVector > ());
														pk->setName(yystack_[6].value.as < std::string > ());
														pk->setConflictAction(yystack_[0].value.as < std::string > ());
														yylhs.value.as < sqlb::ConstraintPtr > () = pk;
													}
#line 3192 "sqlite3_parser.cpp"
    break;

  case 275:
#line 844 "sqlite3_parser.yy"
                                                                                                                {
														auto pk = std::make_shared<sqlb::PrimaryKeyConstraint>(yystack_[3].value.as < sqlb::IndexedColumnVector > ());
														pk->setName(yystack_[7].value.as < std::string > ());
														pk->setConflictAction(yystack_[0].value.as < std::string > ());
														pk->setAutoIncrement(true);
														yylhs.value.as < sqlb::ConstraintPtr > () = pk;
													}
#line 3204 "sqlite3_parser.cpp"
    break;

  case 276:
#line 851 "sqlite3_parser.yy"
                                                                                                        {
														auto u = std::make_shared<sqlb::UniqueConstraint>(yystack_[2].value.as < sqlb::IndexedColumnVector > ());
														u->setName(yystack_[5].value.as < std::string > ());
														u->setConflictAction(yystack_[0].value.as < std::string > ());
														yylhs.value.as < sqlb::ConstraintPtr > () = u;
													}
#line 3215 "sqlite3_parser.cpp"
    break;

  case 277:
#line 857 "sqlite3_parser.yy"
                                                                                                        {
														yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::CheckConstraint>(yystack_[1].value.as < std::string > ());
														yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[4].value.as < std::string > ());
													}
#line 3224 "sqlite3_parser.cpp"
    break;

  case 278:
#line 861 "sqlite3_parser.yy"
                                                                                                                                                {
														yylhs.value.as < sqlb::ConstraintPtr > () = std::make_shared<sqlb::ForeignKeyClause>(yystack_[2].value.as < std::string > (), yystack_[1].value.as < sqlb::StringVector > (), yystack_[0].value.as < std::string > ());
														yylhs.value.as < sqlb::ConstraintPtr > ()->setColumnList(yystack_[5].value.as < sqlb::StringVector > ());
														yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[9].value.as < std::string > ());
													}
#line 3234 "sqlite3_parser.cpp"
    break;

  case 279:
#line 869 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintSet > () = {yystack_[0].value.as < sqlb::ConstraintPtr > ()}; }
#line 3240 "sqlite3_parser.cpp"
    break;

  case 280:
#line 870 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintSet > () = yystack_[2].value.as < sqlb::ConstraintSet > (); yylhs.value.as < sqlb::ConstraintSet > ().insert(yystack_[0].value.as < sqlb::ConstraintPtr > ()); }
#line 3246 "sqlite3_parser.cpp"
    break;

  case 281:
#line 871 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintSet > () = yystack_[1].value.as < sqlb::ConstraintSet > (); yylhs.value.as < sqlb::ConstraintSet > ().insert(yystack_[0].value.as < sqlb::ConstraintPtr > ()); }
#line 3252 "sqlite3_parser.cpp"
    break;

  case 282:
#line 875 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintSet > () = {}; }
#line 3258 "sqlite3_parser.cpp"
    break;

  case 283:
#line 876 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintSet > () = yystack_[0].value.as < sqlb::ConstraintSet > (); }
#line 3264 "sqlite3_parser.cpp"
    break;

  case 284:
#line 880 "sqlite3_parser.yy"
                                                                                                                        {
										yylhs.value.as < sqlb::TablePtr > () = std::make_shared<sqlb::Table>(yystack_[2].value.as < std::string > ());
										yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
									}
#line 3273 "sqlite3_parser.cpp"
    break;

  case 285:
#line 884 "sqlite3_parser.yy"
                                                                                                                                                                                        {
										yylhs.value.as < sqlb::TablePtr > () = std::make_shared<sqlb::Table>(yystack_[5].value.as < std::string > ());
										yylhs.value.as < sqlb::TablePtr > ()->setWithoutRowidTable(yystack_[0].value.as < bool > ());
										yylhs.value.as < sqlb::TablePtr > ()->setConstraints(yystack_[2].value.as < sqlb::ConstraintSet > ());
										yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(true);

										for(const auto& column : yystack_[3].value.as < std::vector<ColumndefData> > ())
										{
											sqlb::Field f;
											sqlb::ConstraintSet c;
											std::tie(f, c) = column;

											yylhs.value.as < sqlb::TablePtr > ()->fields.push_back(f);
											for(const auto& i : c)
												yylhs.value.as < sqlb::TablePtr > ()->addConstraint(i);
										}
									}
#line 3295 "sqlite3_parser.cpp"
    break;


#line 3299 "sqlite3_parser.cpp"

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


  const short parser::yypact_ninf_ = -341;

  const short parser::yytable_ninf_ = -284;

  const short
  parser::yypact_[] =
  {
      -8,   157,    30,    41,  -341,  -341,  -341,  -341,  -341,  -341,
     -59,    23,   -26,  -341,  -341,    68,    68,    68,    59,  2140,
    2140,  2140,    89,  -341,  -341,  -341,  -341,  -341,  -341,  -341,
    -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,
    -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,
    -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,
     150,  -341,  -341,  -341,  -341,  -341,  -341,  -341,   160,  -341,
    -341,    45,   100,    17,  -341,  2228,  2228,    48,  2228,  2052,
      79,  -341,  -341,  -341,  -341,   180,   187,  -341,  -341,  -341,
    -341,  -341,  -341,  -341,  2228,  -341,   193,  -341,  -341,   953,
    -341,  1409,  -341,  1615,  -341,    27,  1964,   200,  1409,  -341,
    1409,  1409,  1409,  1067,   208,  -341,  -341,  -341,  -341,  1409,
    -341,   212,  -341,  -341,  -341,  -341,  -341,  -341,   205,  -341,
    -341,  -341,  -341,   210,  -341,  -341,  -341,  -341,  -341,  -341,
    3121,  2659,  -341,   218,     5,  -341,    92,  -341,    32,    94,
    -341,   -18,  -341,    36,   113,   265,  -341,  -341,  -341,  1409,
     -35,    86,  1409,  3231,    58,   611,    48,  -341,  1409,  1409,
    1409,  1409,  1409,  1409,  1409,  1409,  1409,  1409,  1409,  1409,
    1409,  1409,  1409,  1409,  1409,  1409,  1409,  1409,  1409,    48,
    1409,  1707,  1409,  -341,  1409,  1409,   146,  -341,  1409,  1409,
    -341,  -341,  -341,   117,  1409,   121,   133,  -341,  -341,   266,
    -341,  -341,  -341,   227,    48,  1523,   211,   167,  -341,   178,
    2228,   182,   234,   278,   204,   209,   285,   251,  -341,   201,
    -341,  -341,   453,  1409,  -341,  1409,   -31,  2736,   294,   297,
     308,   303,  -341,   317,  1409,   269,   321,  3121,   128,   128,
      10,    10,   177,    10,   177,   301,   314,   314,   370,   370,
     370,   370,   314,   314,   177,   177,  3231,  2813,  -341,   228,
     725,   271,  -341,   314,   276,   489,  1409,  1409,  1799,  1409,
    1409,  -341,  1409,  3198,  1934,  1409,  -341,  -341,  -341,  -341,
    -341,     5,  1409,  -341,  1409,  -341,  -341,  -341,  -341,  -341,
    -341,   182,    -6,   356,   311,  -341,   357,  1409,   358,   359,
    1409,  -341,  -341,  1409,  2890,  1906,  1409,  -341,  2228,   250,
     257,  -341,   259,  -341,   274,  -341,    48,  1409,  1409,  -341,
     300,   360,  1409,  2228,  1409,  1409,  2967,  2364,   839,   305,
    -341,  2446,  2528,  2610,  1409,  3121,   369,  2336,  2418,  -341,
     182,  2052,   152,   -10,  1409,  2500,  2052,  1409,   307,  3121,
    -341,  1409,  3044,   372,   378,   383,   384,  -341,  -341,   314,
     314,  -341,  -341,   313,   371,  -341,   314,   314,  1409,  1409,
    -341,   338,   385,  1409,  2228,  1409,  1409,  1409,   314,  -341,
    -341,  -341,   363,  -341,   341,   322,    48,   342,   -16,  -341,
     183,  -341,  -341,  -341,  -341,  -341,  -341,  2582,  -341,   344,
     118,   182,  3121,  -341,  -341,  -341,  -341,  -341,  -341,  1181,
     314,   314,  -341,  -341,   345,   389,  -341,   314,   314,   314,
    -341,  -341,  2052,   -20,  -341,   324,    52,    93,   101,   325,
     350,  -341,   -56,   312,   182,   395,  -341,  -341,   349,  -341,
    1295,  -341,  -341,  -341,     4,  -341,   373,  -341,   -15,  -341,
     374,  -341,    -3,  -341,   375,  -341,    51,    65,   335,  -341,
    -341,  -341,  2228,  -341,   182,  -341,  -341,   352,  -341,  -341,
    -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,
    -341,    69,   356,  -341,  -341,  -341,  -341,   152,  -341
  };

  const short
  parser::yydefact_[] =
  {
       0,   181,     0,     2,     4,     5,     6,   197,   198,   182,
       0,     0,     0,     1,     3,   176,   176,   176,     0,     0,
       0,     0,     0,    18,    19,    20,    21,    22,    23,    24,
      59,    58,    60,    25,    26,    27,    28,    29,    30,    31,
      32,    35,    37,    36,    33,    34,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      53,    52,    54,    55,    56,    16,    62,    17,    61,    57,
     187,     0,     0,     0,   177,     0,     0,     0,     0,     0,
       0,    53,    61,   186,   185,   192,     0,    65,    64,    66,
      67,    69,    68,    63,   207,   233,   282,   175,   284,     0,
     195,     0,    74,    76,   208,   235,   235,     0,     0,   193,
       0,     0,     0,     0,    23,    14,    13,    15,    12,     0,
      10,    43,    11,     7,     8,     9,   159,   157,   162,   158,
     163,   164,   169,     0,   165,   170,   171,   172,   173,   174,
     120,   178,   189,     0,     0,    75,     0,   229,   235,   212,
     234,     0,   279,   235,   199,     0,    80,    79,    81,     0,
       0,     0,     0,    82,     0,     0,     0,   194,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   126,     0,     0,     0,   127,     0,     0,
     179,   180,   188,   183,     0,     0,     0,    73,    72,     0,
     237,   236,   230,     0,     0,     0,     0,     0,   217,     0,
       0,   201,     0,     0,     0,     0,     0,   235,   281,     0,
     285,   166,     0,     0,   152,     0,     0,     0,     0,     0,
       0,     0,   124,     0,     0,     0,   161,   121,    87,    88,
      84,    85,    91,    86,    92,    83,    97,    98,    95,    96,
      93,    94,    99,   100,    89,    90,   102,     0,   168,   105,
       0,    61,   135,   101,   104,   106,     0,     0,     0,     0,
       0,   128,     0,   103,   107,     0,   191,   190,    70,    71,
      77,     0,     0,   226,     0,   224,   221,   222,   223,   220,
     213,   201,   178,   240,     0,   218,     0,     0,     0,     0,
       0,   280,   200,     0,     0,     0,     0,   150,     0,     0,
       0,   153,     0,   125,     0,   122,     0,     0,     0,   131,
       0,     0,     0,     0,     0,     0,     0,   109,     0,    61,
     143,   108,   110,   111,     0,   184,     0,     0,     0,   216,
     201,     0,   260,     0,     0,     0,     0,     0,     0,   147,
     151,     0,     0,     0,     0,     0,     0,   123,   160,   129,
     113,   133,   132,     0,    61,   134,   112,   114,     0,     0,
     139,     0,     0,     0,     0,     0,     0,     0,   115,    78,
     219,   225,   214,   238,     0,   270,     0,     0,     0,   258,
     261,   227,   203,   204,   205,   206,   202,     0,   277,     0,
       0,   201,   148,   149,   167,   155,   156,   154,   138,     0,
     130,   117,   141,   140,     0,    61,   142,   116,   118,   119,
     215,   241,     0,     0,   257,   273,     0,     0,     0,   264,
       0,   259,   209,     0,   201,     0,   276,   136,     0,   146,
       0,   239,   268,   269,     0,   244,     0,   245,     0,   254,
       0,   255,     0,   249,     0,   250,     0,     0,   267,   210,
     211,   228,     0,   274,   201,   137,   144,     0,   271,   272,
     246,   243,   242,   256,   253,   252,   251,   248,   247,   262,
     263,     0,   240,   275,   145,   265,   266,   260,   278
  };

  const short
  parser::yypgoto_[] =
  {
    -341,  -341,  -341,   190,   -19,   -13,   -66,  -340,   192,   120,
    -341,    90,  -341,  -341,  -341,  -104,  -341,  -341,  -341,  -341,
     248,  -341,  -341,   256,  -255,   103,   110,  -341,  -341,   198,
     213,  -286,  -341,  -341,  -341,  -341,  -341,  -298,  -341,  -341,
    -341,   268,  -341,   315,  -341,   -80,    64,   -78,    22,  -341,
     -74,  -148,  -341,  -341,  -341
  };

  const short
  parser::yydefgoto_[] =
  {
      -1,     2,     3,   127,   128,   129,    70,    94,   208,   209,
     103,   104,   130,   131,   132,   133,   134,   135,   136,   137,
     160,   138,   139,   140,    98,    19,   202,    11,   286,    71,
     142,   143,     4,   100,     5,    12,   230,   305,   105,   471,
     222,   147,   148,    95,    96,   151,   394,   352,   399,   400,
     401,   152,   153,   107,     6
  };

  const short
  parser::yytable_[] =
  {
      68,    68,    68,   349,   155,   228,    69,    69,    69,    83,
      84,   393,    86,   205,   206,   331,   393,   402,   233,   234,
      79,   223,   316,   317,   358,   149,   176,   200,   102,   452,
      13,  -232,   481,  -232,   436,     1,  -231,   145,  -231,    15,
    -283,   469,   227,   224,   484,   201,   453,   403,    14,    80,
     189,   470,   392,   478,   437,   404,    82,    82,    85,    82,
      92,   245,    69,    69,   482,    69,    93,   225,   149,   146,
     479,   410,    17,   235,   146,    82,   485,   235,   146,   311,
     405,    69,   406,   382,    82,   238,   226,    92,   455,   438,
      69,    16,   451,    93,   169,   170,   171,   172,   487,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   446,   489,   239,   187,   207,   495,    20,
      21,   188,   444,   240,   204,   272,   189,   211,   456,   459,
     488,   490,    18,   213,   214,   496,    22,   463,   171,   172,
     324,   215,   174,   457,   176,    74,   473,   246,   458,   190,
     241,    77,   445,   191,   303,    75,   216,   192,   193,    65,
     194,   195,    67,   196,   197,    76,   330,   198,   189,   460,
     268,   217,   271,   218,    97,   199,   493,   464,    69,   219,
      78,   276,   220,    99,   461,   169,   170,   171,   172,   462,
     101,   174,   465,   176,   159,   293,   297,   466,   221,   106,
     395,    82,   298,    65,   154,   210,    67,    69,   165,   277,
     166,   162,   340,   278,   167,   164,   168,   189,    72,    73,
     279,   280,   203,   229,   204,   281,   285,   396,   373,   397,
     292,   439,   398,   288,   381,   282,   169,   170,   171,   172,
     300,   173,   174,   175,   176,   289,   301,   179,   180,   181,
     182,   302,   102,   185,   186,  -196,     7,     8,   396,   339,
     440,     9,   304,   398,    10,    69,   306,   375,   189,   231,
     290,   168,   291,   325,   332,   168,   333,   308,   367,   424,
     168,   307,   309,   328,   169,   170,   171,   172,   310,   173,
     174,   175,   176,   146,   312,   179,   180,   181,   182,    82,
     319,   185,   186,   320,   371,    69,   168,   368,   383,   322,
     384,   411,   321,   204,   374,   448,   189,   418,   426,   168,
      69,   323,   169,   170,   171,   172,   326,   173,   174,   175,
     176,   334,    92,   179,   180,   181,   182,    92,    93,   185,
     186,   189,   422,    93,   168,   431,   477,   432,   443,   449,
     432,   168,   353,   475,   189,   168,   494,   141,   168,   351,
     354,   356,   357,   364,   372,   425,   156,   157,   158,   161,
     365,    69,   366,   389,   419,   163,   414,   434,   169,   170,
     171,   172,   415,   173,   174,   175,   176,   416,   417,   423,
     435,   433,   450,   454,   467,   185,   186,   430,   468,   474,
     472,   480,   483,   486,   491,   296,   492,   299,   363,   236,
     189,   346,   350,    92,   497,   232,   212,   287,   237,    93,
     409,   150,   441,   498,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,     0,   269,     0,   273,     0,
     274,   275,     0,    82,   283,   284,     0,     0,     0,    69,
     141,   169,   170,   171,   172,     0,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
       0,     0,     0,   187,     0,     0,     0,     0,   188,   314,
       0,   315,     0,   189,     0,     0,     0,   169,   170,   171,
     172,     0,   173,   174,   175,   176,     0,     0,   179,   180,
     181,   182,     0,     0,   185,   186,   190,     0,     0,     0,
     191,     0,     0,     0,   192,   193,     0,   194,   195,   189,
     196,   197,   336,   337,   198,   341,   342,     0,   343,     0,
       0,   345,   199,     0,   335,     0,     0,     0,   347,     0,
     348,     0,     0,     0,   313,     0,     0,     0,     0,     0,
       0,     0,     0,   355,     0,     0,   141,     0,     0,   359,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   369,   370,     0,     0,     0,     0,     0,
     376,   377,     0,     0,     0,     0,     0,     0,     0,     0,
     388,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     407,     0,     0,   141,   108,   242,     0,   412,     0,   110,
     111,   243,     0,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   420,   421,     0,     0,    23,    24,
      25,   427,   428,   429,    26,     0,     0,    27,   113,   114,
       0,     0,    29,     0,     0,   115,   116,   117,     0,     0,
      33,     0,    34,   244,     0,    35,     0,     0,    36,   118,
      37,    38,     0,    39,    40,     0,    41,    42,     0,     0,
      43,     0,     0,     0,    44,    45,    46,    47,   119,     0,
     120,     0,     0,    48,    49,    50,     0,   121,    52,     0,
      53,    54,    55,    56,    57,    58,     0,     0,    59,     0,
      81,    61,     0,   122,    62,     0,     0,     0,    63,     0,
       0,    64,    65,   123,   124,    67,   125,   126,   108,   329,
       0,     0,     0,   110,   111,     0,     0,   112,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    23,    24,    25,     0,     0,     0,    26,     0,
       0,    27,   113,   114,     0,     0,    29,     0,     0,   115,
     116,   117,     0,     0,    33,     0,    34,     0,     0,    35,
       0,     0,    36,   118,    37,    38,     0,    39,    40,     0,
      41,    42,     0,     0,    43,     0,     0,     0,    44,    45,
      46,    47,   119,     0,   120,     0,     0,    48,    49,    50,
       0,   121,    52,     0,    53,    54,    55,    56,    57,    58,
      97,     0,    59,     0,    81,    61,     0,   122,    62,     0,
       0,     0,    63,     0,     0,    64,    65,   123,   124,    67,
     125,   126,   108,   380,     0,     0,     0,   110,   111,     0,
       0,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,    24,    25,     0,
       0,     0,    26,     0,     0,    27,   113,   114,     0,     0,
      29,     0,     0,   115,   116,   117,     0,     0,    33,     0,
      34,     0,     0,    35,     0,     0,    36,   118,    37,    38,
       0,    39,    40,     0,    41,    42,     0,     0,    43,     0,
       0,     0,    44,    45,    46,    47,   119,     0,   120,     0,
       0,    48,    49,    50,     0,   121,    52,     0,    53,    54,
      55,    56,    57,    58,    97,     0,    59,     0,    81,    61,
       0,   122,    62,     0,     0,     0,    63,     0,     0,    64,
      65,   123,   124,    67,   125,   126,   108,   109,     0,     0,
       0,   110,   111,     0,     0,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,    24,    25,     0,     0,     0,    26,     0,     0,    27,
     113,   114,     0,     0,    29,     0,     0,   115,   116,   117,
       0,     0,    33,     0,    34,     0,     0,    35,     0,     0,
      36,   118,    37,    38,     0,    39,    40,     0,    41,    42,
       0,     0,    43,     0,     0,     0,    44,    45,    46,    47,
     119,     0,   120,     0,     0,    48,    49,    50,     0,   121,
      52,     0,    53,    54,    55,    56,    57,    58,     0,     0,
      59,     0,    81,    61,     0,   122,    62,     0,     0,     0,
      63,     0,     0,    64,    65,   123,   124,    67,   125,   126,
     108,     0,     0,     0,     0,   110,   111,     0,     0,   112,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,    24,    25,     0,     0,     0,
      26,     0,     0,    27,   113,   114,     0,     0,    29,     0,
       0,   115,   116,   117,     0,     0,    33,     0,    34,     0,
       0,    35,     0,     0,    36,   118,    37,    38,     0,    39,
      40,     0,    41,    42,     0,     0,    43,     0,     0,     0,
      44,    45,    46,    47,   119,     0,   120,     0,     0,    48,
      49,    50,     0,   121,    52,     0,    53,    54,    55,    56,
      57,    58,     0,     0,    59,     0,    81,    61,     0,   122,
      62,     0,     0,     0,    63,   159,     0,    64,    65,   123,
     124,    67,   125,   126,   108,   447,     0,     0,     0,   110,
     111,     0,     0,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    23,    24,
      25,     0,     0,     0,    26,     0,     0,    27,   113,   114,
       0,     0,    29,     0,     0,   115,   116,   117,     0,     0,
      33,     0,    34,     0,     0,    35,     0,     0,    36,   118,
      37,    38,     0,    39,    40,     0,    41,    42,     0,     0,
      43,     0,     0,     0,    44,    45,    46,    47,   119,     0,
     120,     0,     0,    48,    49,    50,     0,   121,    52,     0,
      53,    54,    55,    56,    57,    58,     0,     0,    59,     0,
      81,    61,     0,   122,    62,     0,     0,     0,    63,     0,
       0,    64,    65,   123,   124,    67,   125,   126,   108,   476,
       0,     0,     0,   110,   111,     0,     0,   112,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    23,    24,    25,     0,     0,     0,    26,     0,
       0,    27,   113,   114,     0,     0,    29,     0,     0,   115,
     116,   117,     0,     0,    33,     0,    34,     0,     0,    35,
       0,     0,    36,   118,    37,    38,     0,    39,    40,     0,
      41,    42,     0,     0,    43,     0,     0,     0,    44,    45,
      46,    47,   119,     0,   120,     0,     0,    48,    49,    50,
       0,   121,    52,     0,    53,    54,    55,    56,    57,    58,
       0,     0,    59,     0,    81,    61,     0,   122,    62,     0,
       0,     0,    63,     0,     0,    64,    65,   123,   124,    67,
     125,   126,   108,     0,     0,     0,     0,   110,   111,     0,
       0,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,    24,    25,     0,
       0,     0,    26,     0,     0,    27,   113,   114,     0,     0,
      29,     0,     0,   115,   116,   117,     0,     0,    33,     0,
      34,     0,     0,    35,     0,     0,    36,   118,    37,    38,
       0,    39,    40,     0,    41,    42,     0,     0,    43,     0,
       0,     0,    44,    45,    46,    47,   119,     0,   120,     0,
       0,    48,    49,    50,     0,   121,    52,     0,    53,    54,
      55,    56,    57,    58,     0,     0,    59,     0,    81,    61,
       0,   122,    62,     0,     0,     0,    63,     0,     0,    64,
      65,   123,   124,    67,   125,   126,   294,     0,     0,     0,
       0,   205,   206,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,    24,    25,     0,     0,     0,    26,     0,     0,    27,
       0,    28,     0,     0,    29,     0,     0,   115,   116,   117,
       0,     0,    33,     0,    34,     0,     0,    35,     0,     0,
      36,   118,    37,    38,     0,    39,    40,   295,    41,    42,
       0,     0,    43,     0,     0,     0,    44,    45,    46,    47,
       0,     0,   120,     0,     0,    48,    49,    50,     0,    51,
      52,     0,    53,    54,    55,    56,    57,    58,   144,     0,
      59,     0,    81,    61,     0,   122,    62,     0,     0,     0,
      63,     0,     0,    64,    65,   123,   124,    67,   125,     0,
       0,     0,    23,    24,    25,     0,     0,     0,    26,     0,
       0,    27,     0,    28,     0,     0,    29,     0,     0,    30,
      31,    32,     0,     0,    33,     0,    34,     0,     0,    35,
       0,     0,    36,     0,    37,    38,     0,    39,    40,     0,
      41,    42,     0,     0,    43,     0,     0,     0,    44,    45,
      46,    47,     0,     0,     0,     0,     0,    48,    49,    50,
       0,    51,    52,     0,    53,    54,    55,    56,    57,    58,
     270,     0,    59,     0,    81,    61,     0,     0,    62,     0,
       0,     0,    63,     0,     0,    64,    65,     0,    66,    67,
       0,     0,     0,     0,    23,    24,    25,     0,     0,     0,
      26,     0,     0,    27,     0,    28,     0,     0,    29,     0,
       0,    30,    31,    32,     0,     0,    33,     0,    34,     0,
       0,    35,     0,     0,    36,     0,    37,    38,     0,    39,
      40,     0,    41,    42,     0,     0,    43,     0,     0,     0,
      44,    45,    46,    47,     0,     0,     0,     0,     0,    48,
      49,    50,     0,    51,    52,     0,    53,    54,    55,    56,
      57,    58,   338,     0,    59,     0,    81,    61,     0,     0,
      62,     0,     0,     0,    63,     0,     0,    64,    65,     0,
      66,    67,     0,     0,     0,     0,    23,    24,    25,     0,
       0,     0,    26,     0,     0,    27,     0,    28,     0,     0,
      29,     0,     0,    30,    31,    32,     0,     0,    33,     0,
      34,     0,     0,    35,     0,     0,    36,     0,    37,    38,
       0,    39,    40,     0,    41,    42,     0,     0,    43,     0,
       0,     0,    44,    45,    46,    47,     0,     0,     0,     0,
       0,    48,    49,    50,     0,    51,    52,     0,    53,    54,
      55,    56,    57,    58,     0,     0,    59,     0,    81,    61,
       0,     0,    62,     0,     0,     0,    63,     0,     0,    64,
      65,     0,    66,    67,   169,   170,   171,   172,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,     0,     0,   187,     0,     0,     0,
       0,   188,   169,   170,   171,   172,   189,   173,   174,   175,
     176,     0,     0,   179,   180,   181,   182,     0,     0,   185,
     186,     0,     0,     0,     0,     0,     0,     0,     0,   190,
       0,     0,     0,   191,   189,     0,     0,   192,   193,     0,
     194,   195,     0,   196,   197,     0,     0,   198,     0,   344,
       0,    23,    24,    25,     0,   199,     0,    26,     0,     0,
      27,     0,    28,     0,     0,    29,   146,   361,    87,    88,
      89,     0,     0,    33,     0,    34,     0,     0,    35,     0,
       0,    36,     0,    37,    38,     0,    39,    40,    90,    41,
      42,     0,     0,    43,     0,     0,     0,    44,    45,    46,
      47,     0,     0,     0,     0,     0,    48,    49,    50,     0,
      51,    52,     0,    53,    54,    55,    56,    57,    58,     0,
       0,    59,     0,    81,    61,     0,     0,    62,     0,     0,
       0,    63,     0,     0,    64,    65,     0,    91,    67,    23,
      24,    25,     0,     0,     0,    26,     0,     0,    27,     0,
      28,     0,     0,    29,     0,     0,    87,    88,    89,     0,
       0,    33,     0,    34,     0,     0,    35,     0,     0,    36,
       0,    37,    38,     0,    39,    40,    90,    41,    42,     0,
       0,    43,     0,     0,     0,    44,    45,    46,    47,     0,
       0,     0,     0,     0,    48,    49,    50,     0,    51,    52,
       0,    53,    54,    55,    56,    57,    58,     0,     0,    59,
       0,    81,    61,     0,     0,    62,     0,     0,     0,    63,
       0,     0,    64,    65,     0,    91,    67,    23,    24,    25,
       0,     0,     0,    26,     0,     0,    27,     0,    28,     0,
       0,    29,     0,     0,    30,    31,    32,     0,     0,    33,
       0,    34,     0,     0,    35,     0,     0,    36,     0,    37,
      38,     0,    39,    40,     0,    41,    42,     0,     0,    43,
       0,     0,     0,    44,    45,    46,    47,     0,     0,     0,
       0,     0,    48,    49,    50,     0,    51,    52,     0,    53,
      54,    55,    56,    57,    58,     0,     0,    59,     0,    60,
      61,     0,     0,    62,     0,     0,     0,    63,     0,     0,
      64,    65,     0,    66,    67,    23,    24,    25,     0,     0,
       0,    26,     0,     0,    27,     0,    28,     0,     0,    29,
       0,     0,    30,    31,    32,     0,     0,    33,     0,    34,
       0,     0,    35,     0,     0,    36,     0,    37,    38,     0,
      39,    40,     0,    41,    42,     0,     0,    43,     0,     0,
       0,    44,    45,    46,    47,     0,     0,     0,     0,     0,
      48,    49,    50,     0,    51,    52,     0,    53,    54,    55,
      56,    57,    58,     0,     0,    59,     0,    81,    61,     0,
       0,    62,     0,     0,     0,    63,     0,     0,    64,    65,
     390,    66,    67,     0,   169,   170,   171,   172,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,     0,     0,   187,     0,     0,     0,
       0,   188,   169,   170,   171,   172,   189,   173,   174,   175,
     176,     0,     0,   179,   180,   181,   182,     0,     0,   185,
     186,     0,     0,     0,     0,     0,     0,     0,     0,   190,
       0,     0,     0,   191,   189,     0,     0,   192,   193,     0,
     194,   195,     0,   196,   197,     0,     0,   198,     0,   379,
       0,     0,   391,     0,     0,   199,   169,   170,   171,   172,
       0,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,     0,     0,     0,   187,     0,
       0,     0,     0,   188,   169,   170,   171,   172,   189,   173,
     174,   175,   176,     0,     0,   179,   180,   181,   182,     0,
       0,   185,   186,     0,     0,     0,     0,     0,     0,     0,
       0,   190,     0,     0,     0,   191,   189,     0,     0,   192,
     193,     0,   194,   195,     0,   196,   197,     0,     0,   198,
       0,   385,     0,     0,   408,     0,     0,   199,   169,   170,
     171,   172,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,     0,     0,
     187,     0,     0,     0,     0,   188,   169,   170,   171,   172,
     189,   173,   174,   175,   176,     0,     0,   179,   180,   181,
     182,     0,     0,   185,   186,     0,     0,     0,     0,     0,
       0,     0,     0,   190,     0,     0,     0,   191,   189,     0,
       0,   192,   193,     0,   194,   195,     0,   196,   197,     0,
       0,   198,     0,   386,     0,     0,   442,     0,     0,   199,
     169,   170,   171,   172,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,     0,
       0,     0,   187,     0,     0,     0,     0,   188,   169,   170,
     171,   172,   189,   173,   174,   175,   176,     0,     0,   179,
     180,   181,   182,     0,     0,   185,   186,     0,     0,     0,
       0,     0,     0,     0,     0,   190,     0,     0,     0,   191,
     189,     0,     0,   192,   193,     0,   194,   195,     0,   196,
     197,     0,     0,   198,     0,   387,     0,   169,   170,   171,
     172,   199,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,     0,     0,   187,
       0,     0,   200,     0,   188,     0,     0,     0,     0,   189,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     201,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   190,     0,     0,     0,   191,     0,     0,     0,
     192,   193,     0,   194,   195,     0,   196,   197,     0,     0,
     198,     0,     0,     0,   169,   170,   171,   172,   199,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,     0,     0,   187,     0,   318,     0,
       0,   188,     0,     0,     0,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   190,
       0,     0,     0,   191,     0,     0,     0,   192,   193,     0,
     194,   195,     0,   196,   197,     0,     0,   198,     0,     0,
       0,   169,   170,   171,   172,   199,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
       0,     0,     0,   187,   327,     0,     0,     0,   188,     0,
       0,     0,     0,   189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   190,     0,     0,     0,
     191,     0,     0,     0,   192,   193,     0,   194,   195,     0,
     196,   197,     0,     0,   198,     0,     0,     0,   169,   170,
     171,   172,   199,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,     0,     0,
     187,     0,     0,     0,     0,   188,     0,     0,     0,     0,
     189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   360,     0,     0,     0,     0,     0,
       0,     0,     0,   190,     0,     0,     0,   191,     0,     0,
       0,   192,   193,     0,   194,   195,     0,   196,   197,     0,
       0,   198,     0,     0,     0,   169,   170,   171,   172,   199,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,     0,     0,     0,   187,   378,     0,
       0,     0,   188,     0,     0,     0,     0,   189,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     190,     0,     0,     0,   191,     0,     0,     0,   192,   193,
       0,   194,   195,     0,   196,   197,     0,     0,   198,     0,
       0,     0,   169,   170,   171,   172,   199,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,     0,     0,     0,   187,     0,     0,     0,     0,   188,
       0,     0,     0,     0,   189,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   413,     0,
       0,     0,     0,     0,     0,     0,     0,   190,     0,     0,
       0,   191,     0,     0,     0,   192,   193,     0,   194,   195,
       0,   196,   197,     0,     0,   198,     0,     0,     0,   169,
     170,   171,   172,   199,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,     0,
       0,   187,     0,     0,     0,     0,   188,     0,     0,     0,
       0,   189,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   190,     0,     0,     0,   191,     0,
       0,     0,   192,   193,     0,   194,   195,     0,   196,   197,
       0,     0,   198,     0,     0,     0,   169,   170,   171,   172,
     199,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,     0,     0,     0,   187,     0,
       0,     0,     0,   188,     0,     0,     0,     0,   189,   169,
     170,   171,   172,     0,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,     0,
       0,   190,     0,     0,     0,   191,   188,     0,     0,   192,
     193,   189,   194,   195,     0,   196,   197,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   199,     0,     0,
       0,     0,     0,     0,   190,     0,     0,     0,   191,     0,
       0,     0,   192,   193,     0,   194,   195,     0,   196,   197,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     199
  };

  const short
  parser::yycheck_[] =
  {
      19,    20,    21,   301,   108,   153,    19,    20,    21,    75,
      76,   351,    78,     8,     9,   270,   356,    27,    53,    54,
       3,    39,    53,    54,   310,   105,    16,    33,    94,    49,
       0,     4,    47,     6,    50,    43,     4,   103,     6,    98,
       4,    97,     6,    61,    47,    51,    66,    57,     7,    32,
      40,   107,   350,    49,    70,    65,    75,    76,    77,    78,
      79,   165,    75,    76,    79,    78,    79,    85,   148,    42,
      66,   357,    98,   108,    42,    94,    79,   108,    42,   227,
      90,    94,    92,   338,   103,    27,   104,   106,    36,   105,
     103,    68,   432,   106,     8,     9,    10,    11,    47,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,   411,    49,    57,    30,   112,    49,    16,
      17,    35,     4,    65,     6,   191,    40,   146,    76,    36,
      79,    66,    64,    39,    40,    66,    77,    36,    10,    11,
     244,    47,    14,    91,    16,    56,   444,   166,    96,    63,
      92,   106,    34,    67,   220,     5,    62,    71,    72,   111,
      74,    75,   114,    77,    78,     5,   270,    81,    40,    76,
     189,    77,   191,    79,    95,    89,   474,    76,   191,    85,
      80,    35,    88,     3,    91,     8,     9,    10,    11,    96,
       3,    14,    91,    16,   108,   214,   215,    96,   104,     6,
      48,   220,   215,   111,     4,   113,   114,   220,     3,    63,
       5,     3,   278,    67,     4,     3,     6,    40,    20,    21,
      74,    75,     4,   110,     6,    79,   109,    75,   332,    77,
       3,    48,    80,   112,   338,    89,     8,     9,    10,    11,
      29,    13,    14,    15,    16,   112,    79,    19,    20,    21,
      22,    73,   318,    25,    26,    98,    99,   100,    75,   278,
      77,   104,    80,    80,   107,   278,    32,   333,    40,     4,
       4,     6,     6,     4,     3,     6,     5,    73,     4,   383,
       6,     3,    73,    55,     8,     9,    10,    11,     3,    13,
      14,    15,    16,    42,    93,    19,    20,    21,    22,   318,
       6,    25,    26,     6,     4,   318,     6,   326,     3,     6,
       5,     4,     4,     6,   333,   419,    40,     4,   384,     6,
     333,     4,     8,     9,    10,    11,     5,    13,    14,    15,
      16,    55,   351,    19,    20,    21,    22,   356,   351,    25,
      26,    40,     4,   356,     6,     4,   450,     6,     4,     4,
       6,     6,    41,     4,    40,     6,     4,   101,     6,     3,
       3,     3,     3,   113,     4,   384,   110,   111,   112,   113,
     113,   384,   113,     4,     3,   119,     4,   396,     8,     9,
      10,    11,     4,    13,    14,    15,    16,     4,     4,     4,
      48,    69,     3,    69,    69,    25,    26,    34,    48,     4,
      88,    28,    28,    28,    69,   215,   472,   215,   318,   161,
      40,   291,   302,   432,   492,   159,   148,   204,   162,   432,
     356,   106,   400,   497,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,    -1,   190,    -1,   192,    -1,
     194,   195,    -1,   472,   198,   199,    -1,    -1,    -1,   472,
     204,     8,     9,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,   233,
      -1,   235,    -1,    40,    -1,    -1,    -1,     8,     9,    10,
      11,    -1,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    -1,    -1,    25,    26,    63,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    71,    72,    -1,    74,    75,    40,
      77,    78,   276,   277,    81,   279,   280,    -1,   282,    -1,
      -1,   285,    89,    -1,    55,    -1,    -1,    -1,   292,    -1,
     294,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   307,    -1,    -1,   310,    -1,    -1,   313,
      -1,    -1,   316,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   327,   328,    -1,    -1,    -1,    -1,    -1,
     334,   335,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     344,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     354,    -1,    -1,   357,     3,     4,    -1,   361,    -1,     8,
       9,    10,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   378,   379,    -1,    -1,    27,    28,
      29,   385,   386,   387,    33,    -1,    -1,    36,    37,    38,
      -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,    -1,
      49,    -1,    51,    52,    -1,    54,    -1,    -1,    57,    58,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      69,    -1,    -1,    -1,    73,    74,    75,    76,    77,    -1,
      79,    -1,    -1,    82,    83,    84,    -1,    86,    87,    -1,
      89,    90,    91,    92,    93,    94,    -1,    -1,    97,    -1,
      99,   100,    -1,   102,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,   111,   112,   113,   114,   115,   116,     3,     4,
      -1,    -1,    -1,     8,     9,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    -1,    -1,    -1,    33,    -1,
      -1,    36,    37,    38,    -1,    -1,    41,    -1,    -1,    44,
      45,    46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    69,    -1,    -1,    -1,    73,    74,
      75,    76,    77,    -1,    79,    -1,    -1,    82,    83,    84,
      -1,    86,    87,    -1,    89,    90,    91,    92,    93,    94,
      95,    -1,    97,    -1,    99,   100,    -1,   102,   103,    -1,
      -1,    -1,   107,    -1,    -1,   110,   111,   112,   113,   114,
     115,   116,     3,     4,    -1,    -1,    -1,     8,     9,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,
      -1,    -1,    33,    -1,    -1,    36,    37,    38,    -1,    -1,
      41,    -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    69,    -1,
      -1,    -1,    73,    74,    75,    76,    77,    -1,    79,    -1,
      -1,    82,    83,    84,    -1,    86,    87,    -1,    89,    90,
      91,    92,    93,    94,    95,    -1,    97,    -1,    99,   100,
      -1,   102,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,
     111,   112,   113,   114,   115,   116,     3,     4,    -1,    -1,
      -1,     8,     9,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,
      37,    38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,
      -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,
      77,    -1,    79,    -1,    -1,    82,    83,    84,    -1,    86,
      87,    -1,    89,    90,    91,    92,    93,    94,    -1,    -1,
      97,    -1,    99,   100,    -1,   102,   103,    -1,    -1,    -1,
     107,    -1,    -1,   110,   111,   112,   113,   114,   115,   116,
       3,    -1,    -1,    -1,    -1,     8,     9,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      33,    -1,    -1,    36,    37,    38,    -1,    -1,    41,    -1,
      -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    69,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    -1,    79,    -1,    -1,    82,
      83,    84,    -1,    86,    87,    -1,    89,    90,    91,    92,
      93,    94,    -1,    -1,    97,    -1,    99,   100,    -1,   102,
     103,    -1,    -1,    -1,   107,   108,    -1,   110,   111,   112,
     113,   114,   115,   116,     3,     4,    -1,    -1,    -1,     8,
       9,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      29,    -1,    -1,    -1,    33,    -1,    -1,    36,    37,    38,
      -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,    -1,
      49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      69,    -1,    -1,    -1,    73,    74,    75,    76,    77,    -1,
      79,    -1,    -1,    82,    83,    84,    -1,    86,    87,    -1,
      89,    90,    91,    92,    93,    94,    -1,    -1,    97,    -1,
      99,   100,    -1,   102,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,   111,   112,   113,   114,   115,   116,     3,     4,
      -1,    -1,    -1,     8,     9,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    -1,    -1,    -1,    33,    -1,
      -1,    36,    37,    38,    -1,    -1,    41,    -1,    -1,    44,
      45,    46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    58,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    69,    -1,    -1,    -1,    73,    74,
      75,    76,    77,    -1,    79,    -1,    -1,    82,    83,    84,
      -1,    86,    87,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    99,   100,    -1,   102,   103,    -1,
      -1,    -1,   107,    -1,    -1,   110,   111,   112,   113,   114,
     115,   116,     3,    -1,    -1,    -1,    -1,     8,     9,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,
      -1,    -1,    33,    -1,    -1,    36,    37,    38,    -1,    -1,
      41,    -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    58,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    69,    -1,
      -1,    -1,    73,    74,    75,    76,    77,    -1,    79,    -1,
      -1,    82,    83,    84,    -1,    86,    87,    -1,    89,    90,
      91,    92,    93,    94,    -1,    -1,    97,    -1,    99,   100,
      -1,   102,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,
     111,   112,   113,   114,   115,   116,     3,    -1,    -1,    -1,
      -1,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,
      -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      -1,    -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,
      -1,    -1,    79,    -1,    -1,    82,    83,    84,    -1,    86,
      87,    -1,    89,    90,    91,    92,    93,    94,     3,    -1,
      97,    -1,    99,   100,    -1,   102,   103,    -1,    -1,    -1,
     107,    -1,    -1,   110,   111,   112,   113,   114,   115,    -1,
      -1,    -1,    27,    28,    29,    -1,    -1,    -1,    33,    -1,
      -1,    36,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      45,    46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    69,    -1,    -1,    -1,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,
      -1,    86,    87,    -1,    89,    90,    91,    92,    93,    94,
       3,    -1,    97,    -1,    99,   100,    -1,    -1,   103,    -1,
      -1,    -1,   107,    -1,    -1,   110,   111,    -1,   113,   114,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      33,    -1,    -1,    36,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    69,    -1,    -1,    -1,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    -1,    86,    87,    -1,    89,    90,    91,    92,
      93,    94,     3,    -1,    97,    -1,    99,   100,    -1,    -1,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,   111,    -1,
     113,   114,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,
      -1,    -1,    33,    -1,    -1,    36,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    69,    -1,
      -1,    -1,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      -1,    82,    83,    84,    -1,    86,    87,    -1,    89,    90,
      91,    92,    93,    94,    -1,    -1,    97,    -1,    99,   100,
      -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,
     111,    -1,   113,   114,     8,     9,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    35,     8,     9,    10,    11,    40,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    67,    40,    -1,    -1,    71,    72,    -1,
      74,    75,    -1,    77,    78,    -1,    -1,    81,    -1,    55,
      -1,    27,    28,    29,    -1,    89,    -1,    33,    -1,    -1,
      36,    -1,    38,    -1,    -1,    41,    42,   101,    44,    45,
      46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    -1,    59,    60,    -1,    62,    63,    64,    65,
      66,    -1,    -1,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,    -1,
      86,    87,    -1,    89,    90,    91,    92,    93,    94,    -1,
      -1,    97,    -1,    99,   100,    -1,    -1,   103,    -1,    -1,
      -1,   107,    -1,    -1,   110,   111,    -1,   113,   114,    27,
      28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,    -1,
      38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,
      -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      -1,    59,    60,    -1,    62,    63,    64,    65,    66,    -1,
      -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,    -1,    82,    83,    84,    -1,    86,    87,
      -1,    89,    90,    91,    92,    93,    94,    -1,    -1,    97,
      -1,    99,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,   111,    -1,   113,   114,    27,    28,    29,
      -1,    -1,    -1,    33,    -1,    -1,    36,    -1,    38,    -1,
      -1,    41,    -1,    -1,    44,    45,    46,    -1,    -1,    49,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    -1,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    69,
      -1,    -1,    -1,    73,    74,    75,    76,    -1,    -1,    -1,
      -1,    -1,    82,    83,    84,    -1,    86,    87,    -1,    89,
      90,    91,    92,    93,    94,    -1,    -1,    97,    -1,    99,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,   111,    -1,   113,   114,    27,    28,    29,    -1,    -1,
      -1,    33,    -1,    -1,    36,    -1,    38,    -1,    -1,    41,
      -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    69,    -1,    -1,
      -1,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      82,    83,    84,    -1,    86,    87,    -1,    89,    90,    91,
      92,    93,    94,    -1,    -1,    97,    -1,    99,   100,    -1,
      -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,   111,
       4,   113,   114,    -1,     8,     9,    10,    11,    -1,    13,
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
      -1,    81,    -1,    55,    -1,    -1,     4,    -1,    -1,    89,
       8,     9,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    35,     8,     9,
      10,    11,    40,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    -1,    -1,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    67,
      40,    -1,    -1,    71,    72,    -1,    74,    75,    -1,    77,
      78,    -1,    -1,    81,    -1,    55,    -1,     8,     9,    10,
      11,    89,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      71,    72,    -1,    74,    75,    -1,    77,    78,    -1,    -1,
      81,    -1,    -1,    -1,     8,     9,    10,    11,    89,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    30,    -1,    32,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,    -1,
      74,    75,    -1,    77,    78,    -1,    -1,    81,    -1,    -1,
      -1,     8,     9,    10,    11,    89,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    30,    31,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    71,    72,    -1,    74,    75,    -1,
      77,    78,    -1,    -1,    81,    -1,    -1,    -1,     8,     9,
      10,    11,    89,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
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
      -1,    30,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    71,    72,    -1,    74,    75,    -1,    77,    78,
      -1,    -1,    81,    -1,    -1,    -1,     8,     9,    10,    11,
      89,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,     8,
       9,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    67,    35,    -1,    -1,    71,
      72,    40,    74,    75,    -1,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    71,    72,    -1,    74,    75,    -1,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    43,   118,   119,   149,   151,   171,    99,   100,   104,
     107,   144,   152,     0,     7,    98,    68,    98,    64,   142,
     142,   142,    77,    27,    28,    29,    33,    36,    38,    41,
      44,    45,    46,    49,    51,    54,    57,    59,    60,    62,
      63,    65,    66,    69,    73,    74,    75,    76,    82,    83,
      84,    86,    87,    89,    90,    91,    92,    93,    94,    97,
      99,   100,   103,   107,   110,   111,   113,   114,   121,   122,
     123,   146,   146,   146,    56,     5,     5,   106,    80,     3,
      32,    99,   121,   123,   123,   121,   123,    44,    45,    46,
      64,   113,   121,   122,   124,   160,   161,    95,   141,     3,
     150,     3,   123,   127,   128,   155,     6,   170,     3,     4,
       8,     9,    12,    37,    38,    44,    45,    46,    58,    77,
      79,    86,   102,   112,   113,   115,   116,   120,   121,   122,
     129,   130,   131,   132,   133,   134,   135,   136,   138,   139,
     140,   140,   147,   148,     3,   123,    42,   158,   159,   162,
     160,   162,   168,   169,     4,   132,   140,   140,   140,   108,
     137,   140,     3,   140,     3,     3,     5,     4,     6,     8,
       9,    10,    11,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    30,    35,    40,
      63,    67,    71,    72,    74,    75,    77,    78,    81,    89,
      33,    51,   143,     4,     6,     8,     9,   112,   125,   126,
     113,   121,   158,    39,    40,    47,    62,    77,    79,    85,
      88,   104,   157,    39,    61,    85,   104,     6,   168,   110,
     153,     4,   140,    53,    54,   108,   137,   140,    27,    57,
      65,    92,     4,    10,    52,   132,   121,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   121,   140,
       3,   121,   123,   140,   140,   140,    35,    63,    67,    74,
      75,    79,    89,   140,   140,   109,   145,   147,   112,   112,
       4,     6,     3,   121,     3,    64,   120,   121,   122,   125,
      29,    79,    73,   123,    80,   154,    32,     3,    73,    73,
       3,   168,    93,   101,   140,   140,    53,    54,    32,     6,
       6,     4,     6,     4,   132,     4,     5,    31,    55,     4,
     132,   141,     3,     5,    55,    55,   140,   140,     3,   121,
     123,   140,   140,   140,    55,   140,   126,   140,   140,   154,
     143,     3,   164,    41,     3,   140,     3,     3,   148,   140,
      54,   101,   140,   128,   113,   113,   113,     4,   121,   140,
     140,     4,     4,   132,   121,   123,   140,   140,    31,    55,
       4,   132,   141,     3,     5,    55,    55,    55,   140,     4,
       4,     4,   154,   124,   163,    48,    75,    77,    80,   165,
     166,   167,    27,    57,    65,    90,    92,   140,     4,   163,
     148,     4,   140,    54,     4,     4,     4,     4,     4,     3,
     140,   140,     4,     4,   132,   121,   123,   140,   140,   140,
      34,     4,     6,    69,   121,    48,    50,    70,   105,    48,
      77,   165,     4,     4,     4,    34,   154,     4,   132,     4,
       3,   124,    49,    66,    69,    36,    76,    91,    96,    36,
      76,    91,    96,    36,    76,    91,    96,    69,    48,    97,
     107,   156,    88,   154,     4,     4,     4,   132,    49,    66,
      28,    47,    79,    28,    47,    79,    28,    47,    79,    49,
      66,    69,   123,   154,     4,    49,    66,   164,   167
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   117,   118,   118,   119,   119,   119,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   121,   121,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   123,   123,   123,
     123,   123,   123,   124,   124,   124,   124,   124,   124,   124,
     125,   125,   126,   126,   127,   127,   128,   128,   128,   129,
     129,   129,   129,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     132,   132,   133,   133,   133,   133,   134,   134,   134,   135,
     135,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   137,   137,   138,
     138,   138,   138,   139,   139,   139,   139,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   141,   142,   142,   143,   143,
     143,   144,   144,   145,   145,   146,   146,   146,   147,   148,
     148,   149,   150,   150,   150,   151,   152,   152,   152,   153,
     153,   154,   154,   154,   154,   154,   154,   155,   155,   156,
     156,   156,   157,   157,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   159,
     159,   160,   160,   161,   161,   162,   162,   162,   163,   163,
     164,   164,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   166,   166,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   168,   168,   168,   168,   168,   169,
     169,   169,   170,   170,   171,   171
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
       2,     2,     1,     1,     1,     2,     1,     4,     6,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     4,
       4,     4,     5,     5,     5,     5,     6,     6,     6,     6,
       1,     3,     4,     5,     3,     4,     2,     2,     3,     5,
       6,     4,     5,     5,     5,     3,     7,     8,     6,     5,
       6,     6,     6,     4,     8,     9,     7,     4,     5,     6,
       4,     5,     3,     4,     6,     6,     6,     1,     1,     1,
       5,     3,     1,     1,     1,     1,     3,     6,     3,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     0,     1,
       1,     0,     1,     0,     2,     3,     3,     1,     2,     1,
       3,    11,     0,     2,     3,     8,     0,     1,     1,     0,
       2,     0,     3,     3,     3,     3,     3,     0,     1,     0,
       1,     1,     0,     2,     5,     6,     4,     2,     3,     5,
       3,     3,     3,     3,     3,     5,     3,     5,     7,     1,
       2,     3,     2,     1,     3,     0,     2,     2,     1,     3,
       0,     3,     4,     4,     3,     3,     4,     4,     4,     3,
       3,     4,     4,     4,     3,     3,     4,     2,     1,     2,
       0,     1,     4,     4,     2,     5,     5,     3,     3,     3,
       1,     4,     4,     2,     7,     8,     6,     5,    10,     1,
       3,     2,     0,     2,     7,    10
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
  "\"RESTRICT\"", "\"ROLLBACK\"", "\"ROWID\"", "\"ROWS\"", "\"SELECT\"",
  "\"SET\"", "\"STORED\"", "\"TABLE\"", "\"TEMP\"", "\"TEMPORARY\"",
  "\"THEN\"", "\"TRUE\"", "\"UNBOUNDED\"", "\"UNIQUE\"", "\"UPDATE\"",
  "\"USING\"", "\"VIRTUAL\"", "\"WHEN\"", "\"WHERE\"", "\"WITHOUT\"",
  "\"identifier\"", "\"numeric\"", "\"string literal\"",
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
       0,   254,   254,   255,   259,   260,   261,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   281,   282,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   329,   330,   331,
     332,   333,   334,   338,   339,   340,   341,   342,   343,   344,
     348,   349,   353,   354,   358,   359,   363,   364,   365,   369,
     370,   371,   372,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     419,   420,   424,   425,   426,   427,   431,   432,   433,   437,
     438,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   461,   462,   466,
     467,   468,   469,   473,   474,   475,   476,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   506,   514,   515,   519,   520,
     521,   529,   530,   534,   535,   539,   540,   541,   545,   559,
     560,   564,   579,   580,   581,   585,   597,   598,   599,   603,
     604,   608,   609,   610,   611,   612,   613,   617,   618,   622,
     623,   624,   628,   629,   633,   639,   646,   652,   654,   660,
     664,   668,   672,   676,   681,   685,   689,   693,   701,   708,
     709,   713,   772,   776,   777,   781,   782,   783,   787,   788,
     792,   793,   797,   798,   799,   800,   801,   802,   803,   804,
     805,   806,   807,   808,   809,   810,   811,   812,   816,   817,
     821,   822,   823,   824,   825,   826,   827,   828,   829,   830,
     831,   832,   833,   834,   838,   844,   851,   857,   861,   869,
     870,   871,   875,   876,   880,   884
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
#line 4700 "sqlite3_parser.cpp"

#line 903 "sqlite3_parser.yy"


void sqlb::parser::parser::error(const location_type& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}
