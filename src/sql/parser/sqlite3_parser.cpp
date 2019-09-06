// A Bison parser, made by GNU Bison 3.4.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2019 Free Software Foundation, Inc.

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

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "sqlite3_parser.hpp"


// Unqualified %code blocks.
#line 85 "sqlite3_parser.yy"

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

#line 67 "sqlite3_parser.cpp"


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


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

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
      yystack_print_ ();                \
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
#line 162 "sqlite3_parser.cpp"


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


  /// Build a parser object.
  parser::parser (yyscan_t yyscanner_yyarg, ParserDriver& drv_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      yyscanner (yyscanner_yyarg),
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
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

  parser::symbol_number_type
  parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 153: // columnconstraint
        value.YY_MOVE_OR_COPY< ColumnConstraintInfo > (YY_MOVE (that.value));
        break;

      case 154: // columnconstraint_list
        value.YY_MOVE_OR_COPY< ColumnConstraintInfoVector > (YY_MOVE (that.value));
        break;

      case 155: // columndef
        value.YY_MOVE_OR_COPY< ColumndefData > (YY_MOVE (that.value));
        break;

      case 139: // optional_if_not_exists
      case 141: // optional_unique
      case 149: // optional_temporary
      case 150: // optional_withoutrowid
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case 163: // tableconstraint
        value.YY_MOVE_OR_COPY< sqlb::ConstraintPtr > (YY_MOVE (that.value));
        break;

      case 164: // tableconstraint_list
      case 165: // optional_tableconstraint_list
        value.YY_MOVE_OR_COPY< sqlb::ConstraintSet > (YY_MOVE (that.value));
        break;

      case 146: // createindex_stmt
        value.YY_MOVE_OR_COPY< sqlb::IndexPtr > (YY_MOVE (that.value));
        break;

      case 144: // indexed_column
        value.YY_MOVE_OR_COPY< sqlb::IndexedColumn > (YY_MOVE (that.value));
        break;

      case 145: // indexed_column_list
        value.YY_MOVE_OR_COPY< sqlb::IndexedColumnVector > (YY_MOVE (that.value));
        break;

      case 158: // columnid_list
      case 159: // optional_columnid_with_paren_list
        value.YY_MOVE_OR_COPY< sqlb::StringVector > (YY_MOVE (that.value));
        break;

      case 148: // createvirtualtable_stmt
      case 166: // createtable_stmt
        value.YY_MOVE_OR_COPY< sqlb::TablePtr > (YY_MOVE (that.value));
        break;

      case 27: // "ABORT"
      case 28: // "ACTION"
      case 29: // "AND"
      case 30: // "AND BETWEEN"
      case 31: // "AS"
      case 32: // "ASC"
      case 33: // "AUTOINCREMENT"
      case 34: // "BETWEEN"
      case 35: // "CASCADE"
      case 36: // "CASE"
      case 37: // "CAST"
      case 38: // "CHECK"
      case 39: // "COLLATE"
      case 40: // "CONFLICT"
      case 41: // "CONSTRAINT"
      case 42: // "CREATE"
      case 43: // "CURRENT_DATE"
      case 44: // "CURRENT_TIME"
      case 45: // "CURRENT_TIMESTAMP"
      case 46: // "DEFAULT"
      case 47: // "DEFERRABLE"
      case 48: // "DEFERRED"
      case 49: // "DELETE"
      case 50: // "DESC"
      case 51: // "DISTINCT"
      case 52: // "ELSE"
      case 53: // "END"
      case 54: // "ESCAPE"
      case 55: // "EXISTS"
      case 56: // "FAIL"
      case 57: // "FALSE"
      case 58: // "FILTER"
      case 59: // "FOLLOWING"
      case 60: // "FOREIGN"
      case 61: // "GLOB"
      case 62: // "IF"
      case 63: // "IGNORE"
      case 64: // "IMMEDIATE"
      case 65: // "IN"
      case 66: // "INDEX"
      case 67: // "INITIALLY"
      case 68: // "INSERT"
      case 69: // "IS"
      case 70: // "ISNULL"
      case 71: // "KEY"
      case 72: // "LIKE"
      case 73: // "MATCH"
      case 74: // "NO"
      case 75: // "NOT"
      case 76: // "NOTNULL"
      case 77: // "NULL"
      case 78: // "ON"
      case 79: // "OR"
      case 80: // "OVER"
      case 81: // "PARTITION"
      case 82: // "PRECEDING"
      case 83: // "PRIMARY"
      case 84: // "RAISE"
      case 85: // "RANGE"
      case 86: // "REFERENCES"
      case 87: // "REGEXP"
      case 88: // "REPLACE"
      case 89: // "RESTRICT"
      case 90: // "ROLLBACK"
      case 91: // "ROWID"
      case 92: // "ROWS"
      case 93: // "SELECT"
      case 94: // "SET"
      case 95: // "TABLE"
      case 96: // "TEMP"
      case 97: // "TEMPORARY"
      case 98: // "THEN"
      case 99: // "TRUE"
      case 100: // "UNBOUNDED"
      case 101: // "UNIQUE"
      case 102: // "UPDATE"
      case 103: // "USING"
      case 104: // "VIRTUAL"
      case 105: // "WHEN"
      case 106: // "WHERE"
      case 107: // "WITHOUT"
      case 108: // "identifier"
      case 109: // "numeric"
      case 110: // "string literal"
      case 111: // "quoted literal"
      case 112: // "blob literal"
      case 113: // "bind parameter"
      case 117: // literalvalue
      case 118: // id
      case 119: // allowed_keywords_as_identifier
      case 120: // tableid
      case 121: // columnid
      case 122: // signednumber
      case 123: // signednumber_or_numeric
      case 124: // typename_namelist
      case 125: // type_name
      case 126: // unary_expr
      case 127: // binary_expr
      case 128: // like_expr
      case 129: // exprlist_expr
      case 130: // function_expr
      case 131: // isnull_expr
      case 132: // between_expr
      case 133: // in_expr
      case 134: // whenthenlist_expr
      case 135: // case_expr
      case 136: // raise_expr
      case 137: // expr
      case 138: // select_stmt
      case 140: // optional_sort_order
      case 142: // optional_where
      case 143: // tableid_with_uninteresting_schema
      case 147: // optional_exprlist_with_paren
      case 151: // optional_conflictclause
      case 152: // optional_typename
      case 157: // optional_constraintname
      case 160: // fk_clause_part
      case 161: // fk_clause_part_list
      case 162: // optional_fk_clause
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 156: // columndef_list
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
    switch (that.type_get ())
    {
      case 153: // columnconstraint
        value.move< ColumnConstraintInfo > (YY_MOVE (that.value));
        break;

      case 154: // columnconstraint_list
        value.move< ColumnConstraintInfoVector > (YY_MOVE (that.value));
        break;

      case 155: // columndef
        value.move< ColumndefData > (YY_MOVE (that.value));
        break;

      case 139: // optional_if_not_exists
      case 141: // optional_unique
      case 149: // optional_temporary
      case 150: // optional_withoutrowid
        value.move< bool > (YY_MOVE (that.value));
        break;

      case 163: // tableconstraint
        value.move< sqlb::ConstraintPtr > (YY_MOVE (that.value));
        break;

      case 164: // tableconstraint_list
      case 165: // optional_tableconstraint_list
        value.move< sqlb::ConstraintSet > (YY_MOVE (that.value));
        break;

      case 146: // createindex_stmt
        value.move< sqlb::IndexPtr > (YY_MOVE (that.value));
        break;

      case 144: // indexed_column
        value.move< sqlb::IndexedColumn > (YY_MOVE (that.value));
        break;

      case 145: // indexed_column_list
        value.move< sqlb::IndexedColumnVector > (YY_MOVE (that.value));
        break;

      case 158: // columnid_list
      case 159: // optional_columnid_with_paren_list
        value.move< sqlb::StringVector > (YY_MOVE (that.value));
        break;

      case 148: // createvirtualtable_stmt
      case 166: // createtable_stmt
        value.move< sqlb::TablePtr > (YY_MOVE (that.value));
        break;

      case 27: // "ABORT"
      case 28: // "ACTION"
      case 29: // "AND"
      case 30: // "AND BETWEEN"
      case 31: // "AS"
      case 32: // "ASC"
      case 33: // "AUTOINCREMENT"
      case 34: // "BETWEEN"
      case 35: // "CASCADE"
      case 36: // "CASE"
      case 37: // "CAST"
      case 38: // "CHECK"
      case 39: // "COLLATE"
      case 40: // "CONFLICT"
      case 41: // "CONSTRAINT"
      case 42: // "CREATE"
      case 43: // "CURRENT_DATE"
      case 44: // "CURRENT_TIME"
      case 45: // "CURRENT_TIMESTAMP"
      case 46: // "DEFAULT"
      case 47: // "DEFERRABLE"
      case 48: // "DEFERRED"
      case 49: // "DELETE"
      case 50: // "DESC"
      case 51: // "DISTINCT"
      case 52: // "ELSE"
      case 53: // "END"
      case 54: // "ESCAPE"
      case 55: // "EXISTS"
      case 56: // "FAIL"
      case 57: // "FALSE"
      case 58: // "FILTER"
      case 59: // "FOLLOWING"
      case 60: // "FOREIGN"
      case 61: // "GLOB"
      case 62: // "IF"
      case 63: // "IGNORE"
      case 64: // "IMMEDIATE"
      case 65: // "IN"
      case 66: // "INDEX"
      case 67: // "INITIALLY"
      case 68: // "INSERT"
      case 69: // "IS"
      case 70: // "ISNULL"
      case 71: // "KEY"
      case 72: // "LIKE"
      case 73: // "MATCH"
      case 74: // "NO"
      case 75: // "NOT"
      case 76: // "NOTNULL"
      case 77: // "NULL"
      case 78: // "ON"
      case 79: // "OR"
      case 80: // "OVER"
      case 81: // "PARTITION"
      case 82: // "PRECEDING"
      case 83: // "PRIMARY"
      case 84: // "RAISE"
      case 85: // "RANGE"
      case 86: // "REFERENCES"
      case 87: // "REGEXP"
      case 88: // "REPLACE"
      case 89: // "RESTRICT"
      case 90: // "ROLLBACK"
      case 91: // "ROWID"
      case 92: // "ROWS"
      case 93: // "SELECT"
      case 94: // "SET"
      case 95: // "TABLE"
      case 96: // "TEMP"
      case 97: // "TEMPORARY"
      case 98: // "THEN"
      case 99: // "TRUE"
      case 100: // "UNBOUNDED"
      case 101: // "UNIQUE"
      case 102: // "UPDATE"
      case 103: // "USING"
      case 104: // "VIRTUAL"
      case 105: // "WHEN"
      case 106: // "WHERE"
      case 107: // "WITHOUT"
      case 108: // "identifier"
      case 109: // "numeric"
      case 110: // "string literal"
      case 111: // "quoted literal"
      case 112: // "blob literal"
      case 113: // "bind parameter"
      case 117: // literalvalue
      case 118: // id
      case 119: // allowed_keywords_as_identifier
      case 120: // tableid
      case 121: // columnid
      case 122: // signednumber
      case 123: // signednumber_or_numeric
      case 124: // typename_namelist
      case 125: // type_name
      case 126: // unary_expr
      case 127: // binary_expr
      case 128: // like_expr
      case 129: // exprlist_expr
      case 130: // function_expr
      case 131: // isnull_expr
      case 132: // between_expr
      case 133: // in_expr
      case 134: // whenthenlist_expr
      case 135: // case_expr
      case 136: // raise_expr
      case 137: // expr
      case 138: // select_stmt
      case 140: // optional_sort_order
      case 142: // optional_where
      case 143: // tableid_with_uninteresting_schema
      case 147: // optional_exprlist_with_paren
      case 151: // optional_conflictclause
      case 152: // optional_typename
      case 157: // optional_constraintname
      case 160: // fk_clause_part
      case 161: // fk_clause_part_list
      case 162: // optional_fk_clause
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 156: // columndef_list
        value.move< std::vector<ColumndefData> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 153: // columnconstraint
        value.move< ColumnConstraintInfo > (that.value);
        break;

      case 154: // columnconstraint_list
        value.move< ColumnConstraintInfoVector > (that.value);
        break;

      case 155: // columndef
        value.move< ColumndefData > (that.value);
        break;

      case 139: // optional_if_not_exists
      case 141: // optional_unique
      case 149: // optional_temporary
      case 150: // optional_withoutrowid
        value.move< bool > (that.value);
        break;

      case 163: // tableconstraint
        value.move< sqlb::ConstraintPtr > (that.value);
        break;

      case 164: // tableconstraint_list
      case 165: // optional_tableconstraint_list
        value.move< sqlb::ConstraintSet > (that.value);
        break;

      case 146: // createindex_stmt
        value.move< sqlb::IndexPtr > (that.value);
        break;

      case 144: // indexed_column
        value.move< sqlb::IndexedColumn > (that.value);
        break;

      case 145: // indexed_column_list
        value.move< sqlb::IndexedColumnVector > (that.value);
        break;

      case 158: // columnid_list
      case 159: // optional_columnid_with_paren_list
        value.move< sqlb::StringVector > (that.value);
        break;

      case 148: // createvirtualtable_stmt
      case 166: // createtable_stmt
        value.move< sqlb::TablePtr > (that.value);
        break;

      case 27: // "ABORT"
      case 28: // "ACTION"
      case 29: // "AND"
      case 30: // "AND BETWEEN"
      case 31: // "AS"
      case 32: // "ASC"
      case 33: // "AUTOINCREMENT"
      case 34: // "BETWEEN"
      case 35: // "CASCADE"
      case 36: // "CASE"
      case 37: // "CAST"
      case 38: // "CHECK"
      case 39: // "COLLATE"
      case 40: // "CONFLICT"
      case 41: // "CONSTRAINT"
      case 42: // "CREATE"
      case 43: // "CURRENT_DATE"
      case 44: // "CURRENT_TIME"
      case 45: // "CURRENT_TIMESTAMP"
      case 46: // "DEFAULT"
      case 47: // "DEFERRABLE"
      case 48: // "DEFERRED"
      case 49: // "DELETE"
      case 50: // "DESC"
      case 51: // "DISTINCT"
      case 52: // "ELSE"
      case 53: // "END"
      case 54: // "ESCAPE"
      case 55: // "EXISTS"
      case 56: // "FAIL"
      case 57: // "FALSE"
      case 58: // "FILTER"
      case 59: // "FOLLOWING"
      case 60: // "FOREIGN"
      case 61: // "GLOB"
      case 62: // "IF"
      case 63: // "IGNORE"
      case 64: // "IMMEDIATE"
      case 65: // "IN"
      case 66: // "INDEX"
      case 67: // "INITIALLY"
      case 68: // "INSERT"
      case 69: // "IS"
      case 70: // "ISNULL"
      case 71: // "KEY"
      case 72: // "LIKE"
      case 73: // "MATCH"
      case 74: // "NO"
      case 75: // "NOT"
      case 76: // "NOTNULL"
      case 77: // "NULL"
      case 78: // "ON"
      case 79: // "OR"
      case 80: // "OVER"
      case 81: // "PARTITION"
      case 82: // "PRECEDING"
      case 83: // "PRIMARY"
      case 84: // "RAISE"
      case 85: // "RANGE"
      case 86: // "REFERENCES"
      case 87: // "REGEXP"
      case 88: // "REPLACE"
      case 89: // "RESTRICT"
      case 90: // "ROLLBACK"
      case 91: // "ROWID"
      case 92: // "ROWS"
      case 93: // "SELECT"
      case 94: // "SET"
      case 95: // "TABLE"
      case 96: // "TEMP"
      case 97: // "TEMPORARY"
      case 98: // "THEN"
      case 99: // "TRUE"
      case 100: // "UNBOUNDED"
      case 101: // "UNIQUE"
      case 102: // "UPDATE"
      case 103: // "USING"
      case 104: // "VIRTUAL"
      case 105: // "WHEN"
      case 106: // "WHERE"
      case 107: // "WITHOUT"
      case 108: // "identifier"
      case 109: // "numeric"
      case 110: // "string literal"
      case 111: // "quoted literal"
      case 112: // "blob literal"
      case 113: // "bind parameter"
      case 117: // literalvalue
      case 118: // id
      case 119: // allowed_keywords_as_identifier
      case 120: // tableid
      case 121: // columnid
      case 122: // signednumber
      case 123: // signednumber_or_numeric
      case 124: // typename_namelist
      case 125: // type_name
      case 126: // unary_expr
      case 127: // binary_expr
      case 128: // like_expr
      case 129: // exprlist_expr
      case 130: // function_expr
      case 131: // isnull_expr
      case 132: // between_expr
      case 133: // in_expr
      case 134: // whenthenlist_expr
      case 135: // case_expr
      case 136: // raise_expr
      case 137: // expr
      case 138: // select_stmt
      case 140: // optional_sort_order
      case 142: // optional_where
      case 143: // tableid_with_uninteresting_schema
      case 147: // optional_exprlist_with_paren
      case 151: // optional_conflictclause
      case 152: // optional_typename
      case 157: // optional_constraintname
      case 160: // fk_clause_part
      case 161: // fk_clause_part_list
      case 162: // optional_fk_clause
        value.move< std::string > (that.value);
        break;

      case 156: // columndef_list
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
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
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
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
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
    // State.
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
    YYCDEBUG << "Entering state " << yystack_[0].state << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
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

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

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
    yypush_ ("Shifting", yyn, YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
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
      case 153: // columnconstraint
        yylhs.value.emplace< ColumnConstraintInfo > ();
        break;

      case 154: // columnconstraint_list
        yylhs.value.emplace< ColumnConstraintInfoVector > ();
        break;

      case 155: // columndef
        yylhs.value.emplace< ColumndefData > ();
        break;

      case 139: // optional_if_not_exists
      case 141: // optional_unique
      case 149: // optional_temporary
      case 150: // optional_withoutrowid
        yylhs.value.emplace< bool > ();
        break;

      case 163: // tableconstraint
        yylhs.value.emplace< sqlb::ConstraintPtr > ();
        break;

      case 164: // tableconstraint_list
      case 165: // optional_tableconstraint_list
        yylhs.value.emplace< sqlb::ConstraintSet > ();
        break;

      case 146: // createindex_stmt
        yylhs.value.emplace< sqlb::IndexPtr > ();
        break;

      case 144: // indexed_column
        yylhs.value.emplace< sqlb::IndexedColumn > ();
        break;

      case 145: // indexed_column_list
        yylhs.value.emplace< sqlb::IndexedColumnVector > ();
        break;

      case 158: // columnid_list
      case 159: // optional_columnid_with_paren_list
        yylhs.value.emplace< sqlb::StringVector > ();
        break;

      case 148: // createvirtualtable_stmt
      case 166: // createtable_stmt
        yylhs.value.emplace< sqlb::TablePtr > ();
        break;

      case 27: // "ABORT"
      case 28: // "ACTION"
      case 29: // "AND"
      case 30: // "AND BETWEEN"
      case 31: // "AS"
      case 32: // "ASC"
      case 33: // "AUTOINCREMENT"
      case 34: // "BETWEEN"
      case 35: // "CASCADE"
      case 36: // "CASE"
      case 37: // "CAST"
      case 38: // "CHECK"
      case 39: // "COLLATE"
      case 40: // "CONFLICT"
      case 41: // "CONSTRAINT"
      case 42: // "CREATE"
      case 43: // "CURRENT_DATE"
      case 44: // "CURRENT_TIME"
      case 45: // "CURRENT_TIMESTAMP"
      case 46: // "DEFAULT"
      case 47: // "DEFERRABLE"
      case 48: // "DEFERRED"
      case 49: // "DELETE"
      case 50: // "DESC"
      case 51: // "DISTINCT"
      case 52: // "ELSE"
      case 53: // "END"
      case 54: // "ESCAPE"
      case 55: // "EXISTS"
      case 56: // "FAIL"
      case 57: // "FALSE"
      case 58: // "FILTER"
      case 59: // "FOLLOWING"
      case 60: // "FOREIGN"
      case 61: // "GLOB"
      case 62: // "IF"
      case 63: // "IGNORE"
      case 64: // "IMMEDIATE"
      case 65: // "IN"
      case 66: // "INDEX"
      case 67: // "INITIALLY"
      case 68: // "INSERT"
      case 69: // "IS"
      case 70: // "ISNULL"
      case 71: // "KEY"
      case 72: // "LIKE"
      case 73: // "MATCH"
      case 74: // "NO"
      case 75: // "NOT"
      case 76: // "NOTNULL"
      case 77: // "NULL"
      case 78: // "ON"
      case 79: // "OR"
      case 80: // "OVER"
      case 81: // "PARTITION"
      case 82: // "PRECEDING"
      case 83: // "PRIMARY"
      case 84: // "RAISE"
      case 85: // "RANGE"
      case 86: // "REFERENCES"
      case 87: // "REGEXP"
      case 88: // "REPLACE"
      case 89: // "RESTRICT"
      case 90: // "ROLLBACK"
      case 91: // "ROWID"
      case 92: // "ROWS"
      case 93: // "SELECT"
      case 94: // "SET"
      case 95: // "TABLE"
      case 96: // "TEMP"
      case 97: // "TEMPORARY"
      case 98: // "THEN"
      case 99: // "TRUE"
      case 100: // "UNBOUNDED"
      case 101: // "UNIQUE"
      case 102: // "UPDATE"
      case 103: // "USING"
      case 104: // "VIRTUAL"
      case 105: // "WHEN"
      case 106: // "WHERE"
      case 107: // "WITHOUT"
      case 108: // "identifier"
      case 109: // "numeric"
      case 110: // "string literal"
      case 111: // "quoted literal"
      case 112: // "blob literal"
      case 113: // "bind parameter"
      case 117: // literalvalue
      case 118: // id
      case 119: // allowed_keywords_as_identifier
      case 120: // tableid
      case 121: // columnid
      case 122: // signednumber
      case 123: // signednumber_or_numeric
      case 124: // typename_namelist
      case 125: // type_name
      case 126: // unary_expr
      case 127: // binary_expr
      case 128: // like_expr
      case 129: // exprlist_expr
      case 130: // function_expr
      case 131: // isnull_expr
      case 132: // between_expr
      case 133: // in_expr
      case 134: // whenthenlist_expr
      case 135: // case_expr
      case 136: // raise_expr
      case 137: // expr
      case 138: // select_stmt
      case 140: // optional_sort_order
      case 142: // optional_where
      case 143: // tableid_with_uninteresting_schema
      case 147: // optional_exprlist_with_paren
      case 151: // optional_conflictclause
      case 152: // optional_typename
      case 157: // optional_constraintname
      case 160: // fk_clause_part
      case 161: // fk_clause_part_list
      case 162: // optional_fk_clause
        yylhs.value.emplace< std::string > ();
        break;

      case 156: // columndef_list
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
#line 306 "sqlite3_parser.yy"
    { drv.result = yystack_[0].value.as < sqlb::IndexPtr > (); }
#line 1278 "sqlite3_parser.cpp"
    break;

  case 5:
#line 307 "sqlite3_parser.yy"
    { drv.result = yystack_[0].value.as < sqlb::TablePtr > (); }
#line 1284 "sqlite3_parser.cpp"
    break;

  case 6:
#line 308 "sqlite3_parser.yy"
    { drv.result = yystack_[0].value.as < sqlb::TablePtr > (); }
#line 1290 "sqlite3_parser.cpp"
    break;

  case 7:
#line 316 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1296 "sqlite3_parser.cpp"
    break;

  case 8:
#line 317 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1302 "sqlite3_parser.cpp"
    break;

  case 9:
#line 318 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1308 "sqlite3_parser.cpp"
    break;

  case 10:
#line 319 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1314 "sqlite3_parser.cpp"
    break;

  case 11:
#line 320 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1320 "sqlite3_parser.cpp"
    break;

  case 12:
#line 321 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1326 "sqlite3_parser.cpp"
    break;

  case 13:
#line 322 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1332 "sqlite3_parser.cpp"
    break;

  case 14:
#line 323 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1338 "sqlite3_parser.cpp"
    break;

  case 15:
#line 324 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1344 "sqlite3_parser.cpp"
    break;

  case 16:
#line 328 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1350 "sqlite3_parser.cpp"
    break;

  case 17:
#line 329 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1356 "sqlite3_parser.cpp"
    break;

  case 18:
#line 334 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1362 "sqlite3_parser.cpp"
    break;

  case 19:
#line 335 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1368 "sqlite3_parser.cpp"
    break;

  case 20:
#line 336 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1374 "sqlite3_parser.cpp"
    break;

  case 21:
#line 337 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1380 "sqlite3_parser.cpp"
    break;

  case 22:
#line 338 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1386 "sqlite3_parser.cpp"
    break;

  case 23:
#line 339 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1392 "sqlite3_parser.cpp"
    break;

  case 24:
#line 340 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1398 "sqlite3_parser.cpp"
    break;

  case 25:
#line 341 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1404 "sqlite3_parser.cpp"
    break;

  case 26:
#line 342 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1410 "sqlite3_parser.cpp"
    break;

  case 27:
#line 343 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1416 "sqlite3_parser.cpp"
    break;

  case 28:
#line 344 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1422 "sqlite3_parser.cpp"
    break;

  case 29:
#line 345 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1428 "sqlite3_parser.cpp"
    break;

  case 30:
#line 346 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1434 "sqlite3_parser.cpp"
    break;

  case 31:
#line 347 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1440 "sqlite3_parser.cpp"
    break;

  case 32:
#line 348 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1446 "sqlite3_parser.cpp"
    break;

  case 33:
#line 349 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1452 "sqlite3_parser.cpp"
    break;

  case 34:
#line 350 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1458 "sqlite3_parser.cpp"
    break;

  case 35:
#line 351 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1464 "sqlite3_parser.cpp"
    break;

  case 36:
#line 352 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1470 "sqlite3_parser.cpp"
    break;

  case 37:
#line 353 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1476 "sqlite3_parser.cpp"
    break;

  case 38:
#line 354 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1482 "sqlite3_parser.cpp"
    break;

  case 39:
#line 355 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1488 "sqlite3_parser.cpp"
    break;

  case 40:
#line 356 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1494 "sqlite3_parser.cpp"
    break;

  case 41:
#line 357 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1500 "sqlite3_parser.cpp"
    break;

  case 42:
#line 358 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1506 "sqlite3_parser.cpp"
    break;

  case 43:
#line 359 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1512 "sqlite3_parser.cpp"
    break;

  case 44:
#line 360 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1518 "sqlite3_parser.cpp"
    break;

  case 45:
#line 361 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1524 "sqlite3_parser.cpp"
    break;

  case 46:
#line 362 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1530 "sqlite3_parser.cpp"
    break;

  case 47:
#line 363 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1536 "sqlite3_parser.cpp"
    break;

  case 48:
#line 364 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1542 "sqlite3_parser.cpp"
    break;

  case 49:
#line 365 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1548 "sqlite3_parser.cpp"
    break;

  case 50:
#line 366 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1554 "sqlite3_parser.cpp"
    break;

  case 51:
#line 367 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1560 "sqlite3_parser.cpp"
    break;

  case 52:
#line 368 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1566 "sqlite3_parser.cpp"
    break;

  case 53:
#line 369 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1572 "sqlite3_parser.cpp"
    break;

  case 54:
#line 373 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1578 "sqlite3_parser.cpp"
    break;

  case 55:
#line 374 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1584 "sqlite3_parser.cpp"
    break;

  case 56:
#line 375 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1590 "sqlite3_parser.cpp"
    break;

  case 57:
#line 376 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1596 "sqlite3_parser.cpp"
    break;

  case 58:
#line 377 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1602 "sqlite3_parser.cpp"
    break;

  case 59:
#line 378 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1608 "sqlite3_parser.cpp"
    break;

  case 60:
#line 382 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1614 "sqlite3_parser.cpp"
    break;

  case 61:
#line 383 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1620 "sqlite3_parser.cpp"
    break;

  case 62:
#line 384 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1626 "sqlite3_parser.cpp"
    break;

  case 63:
#line 385 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1632 "sqlite3_parser.cpp"
    break;

  case 64:
#line 386 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1638 "sqlite3_parser.cpp"
    break;

  case 65:
#line 387 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1644 "sqlite3_parser.cpp"
    break;

  case 66:
#line 388 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1650 "sqlite3_parser.cpp"
    break;

  case 67:
#line 392 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1656 "sqlite3_parser.cpp"
    break;

  case 68:
#line 393 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1662 "sqlite3_parser.cpp"
    break;

  case 69:
#line 397 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1668 "sqlite3_parser.cpp"
    break;

  case 70:
#line 398 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1674 "sqlite3_parser.cpp"
    break;

  case 71:
#line 402 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1680 "sqlite3_parser.cpp"
    break;

  case 72:
#line 403 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 1686 "sqlite3_parser.cpp"
    break;

  case 73:
#line 407 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1692 "sqlite3_parser.cpp"
    break;

  case 74:
#line 408 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1698 "sqlite3_parser.cpp"
    break;

  case 75:
#line 409 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + "(" + yystack_[3].value.as < std::string > () + ", " + yystack_[1].value.as < std::string > () + ")"; }
#line 1704 "sqlite3_parser.cpp"
    break;

  case 76:
#line 413 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1710 "sqlite3_parser.cpp"
    break;

  case 77:
#line 414 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1716 "sqlite3_parser.cpp"
    break;

  case 78:
#line 415 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "~" + yystack_[0].value.as < std::string > (); }
#line 1722 "sqlite3_parser.cpp"
    break;

  case 79:
#line 416 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "NOT " + yystack_[0].value.as < std::string > (); }
#line 1728 "sqlite3_parser.cpp"
    break;

  case 80:
#line 420 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " || " + yystack_[0].value.as < std::string > (); }
#line 1734 "sqlite3_parser.cpp"
    break;

  case 81:
#line 421 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " * " + yystack_[0].value.as < std::string > (); }
#line 1740 "sqlite3_parser.cpp"
    break;

  case 82:
#line 422 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " / " + yystack_[0].value.as < std::string > (); }
#line 1746 "sqlite3_parser.cpp"
    break;

  case 83:
#line 423 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " % " + yystack_[0].value.as < std::string > (); }
#line 1752 "sqlite3_parser.cpp"
    break;

  case 84:
#line 424 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " + " + yystack_[0].value.as < std::string > (); }
#line 1758 "sqlite3_parser.cpp"
    break;

  case 85:
#line 425 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " - " + yystack_[0].value.as < std::string > (); }
#line 1764 "sqlite3_parser.cpp"
    break;

  case 86:
#line 426 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " << " + yystack_[0].value.as < std::string > (); }
#line 1770 "sqlite3_parser.cpp"
    break;

  case 87:
#line 427 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >> " + yystack_[0].value.as < std::string > (); }
#line 1776 "sqlite3_parser.cpp"
    break;

  case 88:
#line 428 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " & " + yystack_[0].value.as < std::string > (); }
#line 1782 "sqlite3_parser.cpp"
    break;

  case 89:
#line 429 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " | " + yystack_[0].value.as < std::string > (); }
#line 1788 "sqlite3_parser.cpp"
    break;

  case 90:
#line 430 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " < " + yystack_[0].value.as < std::string > (); }
#line 1794 "sqlite3_parser.cpp"
    break;

  case 91:
#line 431 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <= " + yystack_[0].value.as < std::string > (); }
#line 1800 "sqlite3_parser.cpp"
    break;

  case 92:
#line 432 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " > " + yystack_[0].value.as < std::string > (); }
#line 1806 "sqlite3_parser.cpp"
    break;

  case 93:
#line 433 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >= " + yystack_[0].value.as < std::string > (); }
#line 1812 "sqlite3_parser.cpp"
    break;

  case 94:
#line 434 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " = " + yystack_[0].value.as < std::string > (); }
#line 1818 "sqlite3_parser.cpp"
    break;

  case 95:
#line 435 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " == " + yystack_[0].value.as < std::string > (); }
#line 1824 "sqlite3_parser.cpp"
    break;

  case 96:
#line 436 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " != " + yystack_[0].value.as < std::string > (); }
#line 1830 "sqlite3_parser.cpp"
    break;

  case 97:
#line 437 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <> " + yystack_[0].value.as < std::string > (); }
#line 1836 "sqlite3_parser.cpp"
    break;

  case 98:
#line 438 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IS " + yystack_[0].value.as < std::string > (); }
#line 1842 "sqlite3_parser.cpp"
    break;

  case 99:
#line 439 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 1848 "sqlite3_parser.cpp"
    break;

  case 100:
#line 440 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " OR " + yystack_[0].value.as < std::string > (); }
#line 1854 "sqlite3_parser.cpp"
    break;

  case 101:
#line 444 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " LIKE " + yystack_[0].value.as < std::string > (); }
#line 1860 "sqlite3_parser.cpp"
    break;

  case 102:
#line 445 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " GLOB " + yystack_[0].value.as < std::string > (); }
#line 1866 "sqlite3_parser.cpp"
    break;

  case 103:
#line 446 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " MATCH " + yystack_[0].value.as < std::string > (); }
#line 1872 "sqlite3_parser.cpp"
    break;

  case 104:
#line 447 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " REGEXP " + yystack_[0].value.as < std::string > (); }
#line 1878 "sqlite3_parser.cpp"
    break;

  case 105:
#line 448 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT LIKE " + yystack_[0].value.as < std::string > (); }
#line 1884 "sqlite3_parser.cpp"
    break;

  case 106:
#line 449 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT GLOB " + yystack_[0].value.as < std::string > (); }
#line 1890 "sqlite3_parser.cpp"
    break;

  case 107:
#line 450 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT MATCH " + yystack_[0].value.as < std::string > (); }
#line 1896 "sqlite3_parser.cpp"
    break;

  case 108:
#line 451 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT REGEXP " + yystack_[0].value.as < std::string > (); }
#line 1902 "sqlite3_parser.cpp"
    break;

  case 109:
#line 452 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " LIKE " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1908 "sqlite3_parser.cpp"
    break;

  case 110:
#line 453 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " GLOB " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1914 "sqlite3_parser.cpp"
    break;

  case 111:
#line 454 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " MATCH " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1920 "sqlite3_parser.cpp"
    break;

  case 112:
#line 455 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " REGEXP " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1926 "sqlite3_parser.cpp"
    break;

  case 113:
#line 456 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT LIKE " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1932 "sqlite3_parser.cpp"
    break;

  case 114:
#line 457 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT GLOB " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1938 "sqlite3_parser.cpp"
    break;

  case 115:
#line 458 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT MATCH " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1944 "sqlite3_parser.cpp"
    break;

  case 116:
#line 459 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT REGEXP " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1950 "sqlite3_parser.cpp"
    break;

  case 117:
#line 463 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1956 "sqlite3_parser.cpp"
    break;

  case 118:
#line 464 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + ", " + yystack_[0].value.as < std::string > (); }
#line 1962 "sqlite3_parser.cpp"
    break;

  case 119:
#line 468 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1968 "sqlite3_parser.cpp"
    break;

  case 120:
#line 469 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + "(DISTINCT " + yystack_[1].value.as < std::string > () + ")"; }
#line 1974 "sqlite3_parser.cpp"
    break;

  case 121:
#line 470 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + "()"; }
#line 1980 "sqlite3_parser.cpp"
    break;

  case 122:
#line 471 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(*)"; }
#line 1986 "sqlite3_parser.cpp"
    break;

  case 123:
#line 475 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " ISNULL"; }
#line 1992 "sqlite3_parser.cpp"
    break;

  case 124:
#line 476 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " NOTNULL"; }
#line 1998 "sqlite3_parser.cpp"
    break;

  case 125:
#line 477 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " NOT NULL"; }
#line 2004 "sqlite3_parser.cpp"
    break;

  case 126:
#line 481 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2010 "sqlite3_parser.cpp"
    break;

  case 127:
#line 482 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2016 "sqlite3_parser.cpp"
    break;

  case 128:
#line 486 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " IN ()"; }
#line 2022 "sqlite3_parser.cpp"
    break;

  case 129:
#line 487 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2028 "sqlite3_parser.cpp"
    break;

  case 130:
#line 488 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2034 "sqlite3_parser.cpp"
    break;

  case 131:
#line 489 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2040 "sqlite3_parser.cpp"
    break;

  case 132:
#line 490 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2046 "sqlite3_parser.cpp"
    break;

  case 133:
#line 491 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 2052 "sqlite3_parser.cpp"
    break;

  case 134:
#line 492 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2058 "sqlite3_parser.cpp"
    break;

  case 135:
#line 493 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2064 "sqlite3_parser.cpp"
    break;

  case 136:
#line 494 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " NOT IN ()"; }
#line 2070 "sqlite3_parser.cpp"
    break;

  case 137:
#line 495 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2076 "sqlite3_parser.cpp"
    break;

  case 138:
#line 496 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2082 "sqlite3_parser.cpp"
    break;

  case 139:
#line 497 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2088 "sqlite3_parser.cpp"
    break;

  case 140:
#line 498 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2094 "sqlite3_parser.cpp"
    break;

  case 141:
#line 499 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 2100 "sqlite3_parser.cpp"
    break;

  case 142:
#line 500 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[8].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2106 "sqlite3_parser.cpp"
    break;

  case 143:
#line 501 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " NOT IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2112 "sqlite3_parser.cpp"
    break;

  case 144:
#line 505 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "WHEN " + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 2118 "sqlite3_parser.cpp"
    break;

  case 145:
#line 506 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " WHEN" + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 2124 "sqlite3_parser.cpp"
    break;

  case 146:
#line 510 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CASE " + yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2130 "sqlite3_parser.cpp"
    break;

  case 147:
#line 511 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CASE " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " END"; }
#line 2136 "sqlite3_parser.cpp"
    break;

  case 148:
#line 512 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CASE " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2142 "sqlite3_parser.cpp"
    break;

  case 149:
#line 513 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CASE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2148 "sqlite3_parser.cpp"
    break;

  case 150:
#line 517 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "RAISE(IGNORE)"; }
#line 2154 "sqlite3_parser.cpp"
    break;

  case 151:
#line 518 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "RAISE(ROLLBACK, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2160 "sqlite3_parser.cpp"
    break;

  case 152:
#line 519 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "RAISE(ABORT, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2166 "sqlite3_parser.cpp"
    break;

  case 153:
#line 520 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "RAISE(FAIL, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2172 "sqlite3_parser.cpp"
    break;

  case 154:
#line 524 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2178 "sqlite3_parser.cpp"
    break;

  case 155:
#line 525 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2184 "sqlite3_parser.cpp"
    break;

  case 156:
#line 526 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2190 "sqlite3_parser.cpp"
    break;

  case 157:
#line 527 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2196 "sqlite3_parser.cpp"
    break;

  case 158:
#line 528 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2202 "sqlite3_parser.cpp"
    break;

  case 159:
#line 529 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2208 "sqlite3_parser.cpp"
    break;

  case 160:
#line 530 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2214 "sqlite3_parser.cpp"
    break;

  case 161:
#line 531 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2220 "sqlite3_parser.cpp"
    break;

  case 162:
#line 532 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2226 "sqlite3_parser.cpp"
    break;

  case 163:
#line 533 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2232 "sqlite3_parser.cpp"
    break;

  case 164:
#line 534 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CAST(" + yystack_[3].value.as < std::string > () + " AS " + yystack_[1].value.as < std::string > () + ")"; }
#line 2238 "sqlite3_parser.cpp"
    break;

  case 165:
#line 535 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " COLLATE " + yystack_[0].value.as < std::string > (); }
#line 2244 "sqlite3_parser.cpp"
    break;

  case 166:
#line 536 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2250 "sqlite3_parser.cpp"
    break;

  case 167:
#line 537 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2256 "sqlite3_parser.cpp"
    break;

  case 168:
#line 538 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2262 "sqlite3_parser.cpp"
    break;

  case 169:
#line 539 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2268 "sqlite3_parser.cpp"
    break;

  case 170:
#line 540 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2274 "sqlite3_parser.cpp"
    break;

  case 171:
#line 541 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2280 "sqlite3_parser.cpp"
    break;

  case 172:
#line 550 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "SELECT"; }
#line 2286 "sqlite3_parser.cpp"
    break;

  case 173:
#line 558 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = false; }
#line 2292 "sqlite3_parser.cpp"
    break;

  case 174:
#line 559 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = true; }
#line 2298 "sqlite3_parser.cpp"
    break;

  case 175:
#line 563 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = ""; }
#line 2304 "sqlite3_parser.cpp"
    break;

  case 176:
#line 564 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "ASC"; }
#line 2310 "sqlite3_parser.cpp"
    break;

  case 177:
#line 565 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "DESC"; }
#line 2316 "sqlite3_parser.cpp"
    break;

  case 178:
#line 573 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = false; }
#line 2322 "sqlite3_parser.cpp"
    break;

  case 179:
#line 574 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = true; }
#line 2328 "sqlite3_parser.cpp"
    break;

  case 180:
#line 578 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = ""; }
#line 2334 "sqlite3_parser.cpp"
    break;

  case 181:
#line 579 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2340 "sqlite3_parser.cpp"
    break;

  case 182:
#line 583 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2346 "sqlite3_parser.cpp"
    break;

  case 183:
#line 584 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2352 "sqlite3_parser.cpp"
    break;

  case 184:
#line 585 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2358 "sqlite3_parser.cpp"
    break;

  case 185:
#line 589 "sqlite3_parser.yy"
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
#line 2374 "sqlite3_parser.cpp"
    break;

  case 186:
#line 603 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::IndexedColumnVector > () = sqlb::IndexedColumnVector(1, yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2380 "sqlite3_parser.cpp"
    break;

  case 187:
#line 604 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::IndexedColumnVector > () = yystack_[2].value.as < sqlb::IndexedColumnVector > (); yylhs.value.as < sqlb::IndexedColumnVector > ().push_back(yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2386 "sqlite3_parser.cpp"
    break;

  case 188:
#line 608 "sqlite3_parser.yy"
    {
													yylhs.value.as < sqlb::IndexPtr > () = sqlb::IndexPtr(new sqlb::Index(yystack_[6].value.as < std::string > ()));
													yylhs.value.as < sqlb::IndexPtr > ()->setTable(yystack_[4].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setUnique(yystack_[9].value.as < bool > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setWhereExpr(yystack_[0].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->fields = yystack_[2].value.as < sqlb::IndexedColumnVector > ();
													yylhs.value.as < sqlb::IndexPtr > ()->setFullyParsed(true);
												}
#line 2399 "sqlite3_parser.cpp"
    break;

  case 189:
#line 623 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = {}; }
#line 2405 "sqlite3_parser.cpp"
    break;

  case 190:
#line 624 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = {}; }
#line 2411 "sqlite3_parser.cpp"
    break;

  case 191:
#line 625 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 2417 "sqlite3_parser.cpp"
    break;

  case 192:
#line 629 "sqlite3_parser.yy"
    {
													yylhs.value.as < sqlb::TablePtr > () = sqlb::TablePtr(new sqlb::Table(yystack_[3].value.as < std::string > ()));
													yylhs.value.as < sqlb::TablePtr > ()->setVirtualUsing(yystack_[1].value.as < std::string > ());
													yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
												}
#line 2427 "sqlite3_parser.cpp"
    break;

  case 193:
#line 641 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = false; }
#line 2433 "sqlite3_parser.cpp"
    break;

  case 194:
#line 642 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = true; }
#line 2439 "sqlite3_parser.cpp"
    break;

  case 195:
#line 643 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = true; }
#line 2445 "sqlite3_parser.cpp"
    break;

  case 196:
#line 647 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = false; }
#line 2451 "sqlite3_parser.cpp"
    break;

  case 197:
#line 648 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = true; }
#line 2457 "sqlite3_parser.cpp"
    break;

  case 198:
#line 652 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = ""; }
#line 2463 "sqlite3_parser.cpp"
    break;

  case 199:
#line 653 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2469 "sqlite3_parser.cpp"
    break;

  case 200:
#line 654 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2475 "sqlite3_parser.cpp"
    break;

  case 201:
#line 655 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2481 "sqlite3_parser.cpp"
    break;

  case 202:
#line 656 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2487 "sqlite3_parser.cpp"
    break;

  case 203:
#line 657 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2493 "sqlite3_parser.cpp"
    break;

  case 204:
#line 661 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = ""; }
#line 2499 "sqlite3_parser.cpp"
    break;

  case 205:
#line 662 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2505 "sqlite3_parser.cpp"
    break;

  case 206:
#line 666 "sqlite3_parser.yy"
    {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::PrimaryKey;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = true;
												sqlb::PrimaryKeyConstraint* pk = new sqlb::PrimaryKeyConstraint({sqlb::IndexedColumn("", false, yystack_[1].value.as < std::string > ())});
												pk->setName(yystack_[4].value.as < std::string > ());
												pk->setConflictAction(yystack_[0].value.as < std::string > ());
												yylhs.value.as < ColumnConstraintInfo > ().table_constraint = sqlb::ConstraintPtr(pk);
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = true;
											}
#line 2519 "sqlite3_parser.cpp"
    break;

  case 207:
#line 675 "sqlite3_parser.yy"
    {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::PrimaryKey;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = true;
												sqlb::PrimaryKeyConstraint* pk = new sqlb::PrimaryKeyConstraint({sqlb::IndexedColumn("", false, yystack_[2].value.as < std::string > ())});
												pk->setName(yystack_[5].value.as < std::string > ());
												pk->setConflictAction(yystack_[1].value.as < std::string > ());
												pk->setAutoIncrement(true);
												yylhs.value.as < ColumnConstraintInfo > ().table_constraint = sqlb::ConstraintPtr(pk);
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = true;
											}
#line 2534 "sqlite3_parser.cpp"
    break;

  case 208:
#line 685 "sqlite3_parser.yy"
    {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::NotNull;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[3].value.as < std::string > () == "" && yystack_[0].value.as < std::string > () == "");
											}
#line 2544 "sqlite3_parser.cpp"
    break;

  case 209:
#line 690 "sqlite3_parser.yy"
    {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::None;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = true;
											}
#line 2554 "sqlite3_parser.cpp"
    break;

  case 210:
#line 695 "sqlite3_parser.yy"
    {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Unique;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "" && yystack_[0].value.as < std::string > () == "");
											}
#line 2564 "sqlite3_parser.cpp"
    break;

  case 211:
#line 700 "sqlite3_parser.yy"
    {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Check;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[1].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[4].value.as < std::string > () == "");
											}
#line 2575 "sqlite3_parser.cpp"
    break;

  case 212:
#line 706 "sqlite3_parser.yy"
    {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Default;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[0].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "");
											}
#line 2586 "sqlite3_parser.cpp"
    break;

  case 213:
#line 712 "sqlite3_parser.yy"
    {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Default;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[0].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "");
											}
#line 2597 "sqlite3_parser.cpp"
    break;

  case 214:
#line 718 "sqlite3_parser.yy"
    {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Default;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[0].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "");
											}
#line 2608 "sqlite3_parser.cpp"
    break;

  case 215:
#line 724 "sqlite3_parser.yy"
    {	// We must allow the same keywords as unquoted default values as in the columnid context.
												// But we do not use columnid here in order to avoid reduce/reduce conflicts.
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Default;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[0].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "");
											}
#line 2620 "sqlite3_parser.cpp"
    break;

  case 216:
#line 731 "sqlite3_parser.yy"
    {	// Same as above.
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Default;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[0].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "");
											}
#line 2631 "sqlite3_parser.cpp"
    break;

  case 217:
#line 737 "sqlite3_parser.yy"
    {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Default;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = "(" + yystack_[1].value.as < std::string > () + ")";
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[4].value.as < std::string > () == "");
											}
#line 2642 "sqlite3_parser.cpp"
    break;

  case 218:
#line 743 "sqlite3_parser.yy"
    {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Collate;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[0].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "");
											}
#line 2653 "sqlite3_parser.cpp"
    break;

  case 219:
#line 749 "sqlite3_parser.yy"
    {	// TODO Solve shift/reduce conflict. It is not super important though as shifting seems to be right here.
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::ForeignKey;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = true;
												sqlb::ForeignKeyClause* fk = new sqlb::ForeignKeyClause();
												fk->setName(yystack_[4].value.as < std::string > ());
												fk->setTable(yystack_[2].value.as < std::string > ());
												fk->setColumns(yystack_[1].value.as < sqlb::StringVector > ());
												fk->setConstraint(yystack_[0].value.as < std::string > ());
												yylhs.value.as < ColumnConstraintInfo > ().table_constraint = sqlb::ConstraintPtr(fk);
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = true;
											}
#line 2669 "sqlite3_parser.cpp"
    break;

  case 220:
#line 763 "sqlite3_parser.yy"
    { yylhs.value.as < ColumnConstraintInfoVector > () = { yystack_[0].value.as < ColumnConstraintInfo > () }; }
#line 2675 "sqlite3_parser.cpp"
    break;

  case 221:
#line 764 "sqlite3_parser.yy"
    { yylhs.value.as < ColumnConstraintInfoVector > () = yystack_[1].value.as < ColumnConstraintInfoVector > (); yylhs.value.as < ColumnConstraintInfoVector > ().push_back(yystack_[0].value.as < ColumnConstraintInfo > ()); }
#line 2681 "sqlite3_parser.cpp"
    break;

  case 222:
#line 768 "sqlite3_parser.yy"
    {
								sqlb::Field f(yystack_[2].value.as < std::string > (), yystack_[1].value.as < std::string > ());
								bool fully_parsed = true;
								sqlb::ConstraintSet table_constraints{};
								for(auto c : yystack_[0].value.as < ColumnConstraintInfoVector > ())
								{
									if(c.fully_parsed == false)
										fully_parsed = false;

									if(c.type == ColumnConstraintInfo::None)
										continue;

									if(c.is_table_constraint)
									{
										if(c.table_constraint->columnList().empty())
											c.table_constraint->setColumnList({yystack_[2].value.as < std::string > ()});
										else
											c.table_constraint->replaceInColumnList("", yystack_[2].value.as < std::string > ());
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

								yylhs.value.as < ColumndefData > () = std::make_tuple(f, table_constraints, fully_parsed);
							}
#line 2724 "sqlite3_parser.cpp"
    break;

  case 223:
#line 806 "sqlite3_parser.yy"
    { yylhs.value.as < ColumndefData > () = std::make_tuple(sqlb::Field(yystack_[1].value.as < std::string > (), yystack_[0].value.as < std::string > ()), sqlb::ConstraintSet{}, true); }
#line 2730 "sqlite3_parser.cpp"
    break;

  case 224:
#line 810 "sqlite3_parser.yy"
    { yylhs.value.as < std::vector<ColumndefData> > () = {yystack_[0].value.as < ColumndefData > ()}; }
#line 2736 "sqlite3_parser.cpp"
    break;

  case 225:
#line 811 "sqlite3_parser.yy"
    { yylhs.value.as < std::vector<ColumndefData> > () = yystack_[2].value.as < std::vector<ColumndefData> > (); yylhs.value.as < std::vector<ColumndefData> > ().push_back(yystack_[0].value.as < ColumndefData > ()); }
#line 2742 "sqlite3_parser.cpp"
    break;

  case 226:
#line 815 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = ""; }
#line 2748 "sqlite3_parser.cpp"
    break;

  case 227:
#line 816 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2754 "sqlite3_parser.cpp"
    break;

  case 228:
#line 820 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::StringVector > () = sqlb::StringVector(1, yystack_[0].value.as < std::string > ()); }
#line 2760 "sqlite3_parser.cpp"
    break;

  case 229:
#line 821 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::StringVector > () = yystack_[2].value.as < sqlb::StringVector > (); yylhs.value.as < sqlb::StringVector > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 2766 "sqlite3_parser.cpp"
    break;

  case 230:
#line 825 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::StringVector > () = sqlb::StringVector(); }
#line 2772 "sqlite3_parser.cpp"
    break;

  case 231:
#line 826 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::StringVector > () = yystack_[1].value.as < sqlb::StringVector > (); }
#line 2778 "sqlite3_parser.cpp"
    break;

  case 232:
#line 830 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2784 "sqlite3_parser.cpp"
    break;

  case 233:
#line 831 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2790 "sqlite3_parser.cpp"
    break;

  case 234:
#line 832 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2796 "sqlite3_parser.cpp"
    break;

  case 235:
#line 833 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2802 "sqlite3_parser.cpp"
    break;

  case 236:
#line 834 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2808 "sqlite3_parser.cpp"
    break;

  case 237:
#line 835 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2814 "sqlite3_parser.cpp"
    break;

  case 238:
#line 836 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2820 "sqlite3_parser.cpp"
    break;

  case 239:
#line 837 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2826 "sqlite3_parser.cpp"
    break;

  case 240:
#line 838 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2832 "sqlite3_parser.cpp"
    break;

  case 241:
#line 839 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2838 "sqlite3_parser.cpp"
    break;

  case 242:
#line 840 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2844 "sqlite3_parser.cpp"
    break;

  case 243:
#line 841 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2850 "sqlite3_parser.cpp"
    break;

  case 244:
#line 842 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2856 "sqlite3_parser.cpp"
    break;

  case 245:
#line 843 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2862 "sqlite3_parser.cpp"
    break;

  case 246:
#line 844 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2868 "sqlite3_parser.cpp"
    break;

  case 247:
#line 845 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2874 "sqlite3_parser.cpp"
    break;

  case 248:
#line 849 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2880 "sqlite3_parser.cpp"
    break;

  case 249:
#line 850 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2886 "sqlite3_parser.cpp"
    break;

  case 250:
#line 854 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = ""; }
#line 2892 "sqlite3_parser.cpp"
    break;

  case 251:
#line 855 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2898 "sqlite3_parser.cpp"
    break;

  case 252:
#line 856 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2904 "sqlite3_parser.cpp"
    break;

  case 253:
#line 857 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2910 "sqlite3_parser.cpp"
    break;

  case 254:
#line 858 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2916 "sqlite3_parser.cpp"
    break;

  case 255:
#line 859 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2922 "sqlite3_parser.cpp"
    break;

  case 256:
#line 860 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2928 "sqlite3_parser.cpp"
    break;

  case 257:
#line 861 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2934 "sqlite3_parser.cpp"
    break;

  case 258:
#line 862 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2940 "sqlite3_parser.cpp"
    break;

  case 259:
#line 863 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2946 "sqlite3_parser.cpp"
    break;

  case 260:
#line 864 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2952 "sqlite3_parser.cpp"
    break;

  case 261:
#line 865 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2958 "sqlite3_parser.cpp"
    break;

  case 262:
#line 866 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2964 "sqlite3_parser.cpp"
    break;

  case 263:
#line 867 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 2970 "sqlite3_parser.cpp"
    break;

  case 264:
#line 871 "sqlite3_parser.yy"
    {
														sqlb::PrimaryKeyConstraint* pk = new sqlb::PrimaryKeyConstraint(yystack_[2].value.as < sqlb::IndexedColumnVector > ());
														pk->setName(yystack_[6].value.as < std::string > ());
														pk->setConflictAction(yystack_[0].value.as < std::string > ());
														yylhs.value.as < sqlb::ConstraintPtr > () = sqlb::ConstraintPtr(pk);
													}
#line 2981 "sqlite3_parser.cpp"
    break;

  case 265:
#line 877 "sqlite3_parser.yy"
    {
														sqlb::PrimaryKeyConstraint* pk = new sqlb::PrimaryKeyConstraint(yystack_[3].value.as < sqlb::IndexedColumnVector > ());
														pk->setName(yystack_[7].value.as < std::string > ());
														pk->setConflictAction(yystack_[0].value.as < std::string > ());
														pk->setAutoIncrement(true);
														yylhs.value.as < sqlb::ConstraintPtr > () = sqlb::ConstraintPtr(pk);
													}
#line 2993 "sqlite3_parser.cpp"
    break;

  case 266:
#line 884 "sqlite3_parser.yy"
    {
														sqlb::UniqueConstraint* u = new sqlb::UniqueConstraint(yystack_[2].value.as < sqlb::IndexedColumnVector > ());
														u->setName(yystack_[5].value.as < std::string > ());
														u->setConflictAction(yystack_[0].value.as < std::string > ());
														yylhs.value.as < sqlb::ConstraintPtr > () = sqlb::ConstraintPtr(u);
													}
#line 3004 "sqlite3_parser.cpp"
    break;

  case 267:
#line 890 "sqlite3_parser.yy"
    {
														yylhs.value.as < sqlb::ConstraintPtr > () = sqlb::ConstraintPtr(new sqlb::CheckConstraint(yystack_[1].value.as < std::string > ()));
														yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[4].value.as < std::string > ());
													}
#line 3013 "sqlite3_parser.cpp"
    break;

  case 268:
#line 894 "sqlite3_parser.yy"
    {
														yylhs.value.as < sqlb::ConstraintPtr > () = sqlb::ConstraintPtr(new sqlb::ForeignKeyClause(yystack_[2].value.as < std::string > (), yystack_[1].value.as < sqlb::StringVector > (), yystack_[0].value.as < std::string > ()));
														yylhs.value.as < sqlb::ConstraintPtr > ()->setColumnList(yystack_[5].value.as < sqlb::StringVector > ());
														yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[9].value.as < std::string > ());
													}
#line 3023 "sqlite3_parser.cpp"
    break;

  case 269:
#line 902 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::ConstraintSet > () = {yystack_[0].value.as < sqlb::ConstraintPtr > ()}; }
#line 3029 "sqlite3_parser.cpp"
    break;

  case 270:
#line 903 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::ConstraintSet > () = yystack_[2].value.as < sqlb::ConstraintSet > (); yylhs.value.as < sqlb::ConstraintSet > ().insert(yystack_[0].value.as < sqlb::ConstraintPtr > ()); }
#line 3035 "sqlite3_parser.cpp"
    break;

  case 271:
#line 904 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::ConstraintSet > () = yystack_[1].value.as < sqlb::ConstraintSet > (); yylhs.value.as < sqlb::ConstraintSet > ().insert(yystack_[0].value.as < sqlb::ConstraintPtr > ()); }
#line 3041 "sqlite3_parser.cpp"
    break;

  case 272:
#line 908 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::ConstraintSet > () = {}; }
#line 3047 "sqlite3_parser.cpp"
    break;

  case 273:
#line 909 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::ConstraintSet > () = yystack_[0].value.as < sqlb::ConstraintSet > (); }
#line 3053 "sqlite3_parser.cpp"
    break;

  case 274:
#line 913 "sqlite3_parser.yy"
    {
										yylhs.value.as < sqlb::TablePtr > () = sqlb::TablePtr(new sqlb::Table(yystack_[2].value.as < std::string > ()));
										yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
									}
#line 3062 "sqlite3_parser.cpp"
    break;

  case 275:
#line 917 "sqlite3_parser.yy"
    {
										yylhs.value.as < sqlb::TablePtr > () = sqlb::TablePtr(new sqlb::Table(yystack_[5].value.as < std::string > ()));
										yylhs.value.as < sqlb::TablePtr > ()->setWithoutRowidTable(yystack_[0].value.as < bool > ());
										yylhs.value.as < sqlb::TablePtr > ()->setConstraints(yystack_[2].value.as < sqlb::ConstraintSet > ());
										yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(true);

										for(const auto& column : yystack_[3].value.as < std::vector<ColumndefData> > ())
										{
											sqlb::Field f;
											sqlb::ConstraintSet c;
											bool fully_parsed;
											std::tie(f, c, fully_parsed) = column;

											if(fully_parsed == false)
												yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
											yylhs.value.as < sqlb::TablePtr > ()->fields.push_back(f);
											for(const auto& i : c)
												yylhs.value.as < sqlb::TablePtr > ()->addConstraint(i);
										}
									}
#line 3087 "sqlite3_parser.cpp"
    break;


#line 3091 "sqlite3_parser.cpp"

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
      YY_STACK_PRINT ();

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
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
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
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
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

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
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

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

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
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

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
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short parser::yypact_ninf_ = -333;

  const short parser::yytable_ninf_ = -274;

  const short
  parser::yypact_[] =
  {
       1,    98,    51,    66,  -333,  -333,  -333,  -333,  -333,  -333,
      44,    10,    47,  -333,  -333,    96,    96,    96,    21,  2202,
    2202,  2202,   110,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,   193,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,   205,  -333,  -333,   111,   143,
      32,  -333,  2287,  2287,   -20,  2287,  2117,   131,  -333,  -333,
    -333,  -333,   227,   228,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  2287,  -333,   230,  -333,  -333,   961,  -333,  1405,  -333,
    1606,  -333,    25,  2032,   237,  1405,  -333,  1405,  1405,  1405,
    1072,   239,  -333,  -333,  -333,  -333,  1405,  -333,   257,  -333,
    -333,  -333,  -333,  -333,  -333,    27,  -333,  -333,  -333,  -333,
     126,  -333,  -333,  -333,  -333,  -333,  -333,  2997,  2547,  -333,
     134,     5,  -333,   -20,  -333,    30,    89,  -333,   -18,  -333,
      34,   158,   180,  -333,  -333,  -333,  1405,   -35,    84,  1405,
    3104,    22,   628,   -20,  -333,  1405,  1405,  1405,  1405,  1405,
    1405,  1405,  1405,  1405,  1405,  1405,  1405,  1405,  1405,  1405,
    1405,  1405,  1405,  1405,  1405,  1405,   -20,  1405,  1696,  1405,
    -333,  1405,  1405,   139,  -333,  1405,  1405,  -333,  -333,  -333,
     177,  1405,   176,   188,  -333,  -333,   181,  -333,  -333,   283,
     -20,  1516,   225,  -333,   232,  2287,   231,   298,   242,   243,
     305,   269,  -333,   226,  -333,  -333,  1890,  1405,  -333,  1405,
     -31,  2622,   309,   310,   315,   314,  -333,   317,  1405,   223,
     318,  2997,   241,   241,     9,     9,   209,     9,   209,   286,
    2574,  2574,   279,   279,   279,   279,  2574,  2574,   209,   209,
    3104,  2697,  -333,   224,   739,   210,  -333,  2574,   332,   391,
    1405,  1405,  1786,  1405,  1405,  -333,  1405,  3072,   476,  1405,
    -333,  -333,  -333,  -333,  -333,     5,  1405,  -333,  1405,  -333,
    -333,  -333,  -333,  -333,   231,    -6,   319,   287,  -333,  1405,
     323,   327,  1405,  -333,  -333,  1405,  2772,  1976,  1405,  -333,
    2287,   221,   222,  -333,   229,  -333,   250,  -333,   -20,  1405,
    1405,  -333,   255,   330,  1405,  2287,  1405,  1405,  2847,  1917,
     850,   261,  -333,  2003,  2419,  2499,  1405,  2997,   331,   503,
    2392,  -333,   231,  2117,   -28,    92,  2472,  2117,  1405,   263,
    2997,  -333,  1405,  2922,   334,   340,   345,   346,  -333,  -333,
    2574,  2574,  -333,  -333,   264,   333,  -333,  2574,  2574,  1405,
    1405,  -333,   267,   351,  1405,  2287,  1405,  1405,  1405,  2574,
    -333,  -333,  -333,   326,  -333,   270,   293,   -20,   316,   -22,
    -333,   130,  -333,  -333,  -333,  -333,  -333,  -333,  -333,   271,
      35,   231,  2997,  -333,  -333,  -333,  -333,  -333,  -333,  1183,
    2574,  2574,  -333,  -333,   275,   359,  -333,  2574,  2574,  2574,
    -333,  -333,  2117,    67,  -333,   297,    52,    87,    94,   299,
     320,  -333,   284,   231,   361,  -333,  -333,   278,  -333,  1294,
    -333,  -333,  -333,    69,  -333,   341,  -333,   -13,  -333,   344,
    -333,    70,  -333,   347,  -333,    74,    73,   306,  2287,  -333,
     231,  -333,  -333,   294,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,    86,   319,  -333,
    -333,  -333,  -333,   -28,  -333
  };

  const unsigned short
  parser::yydefact_[] =
  {
       0,   178,     0,     2,     4,     5,     6,   194,   195,   179,
       0,     0,     0,     1,     3,   173,   173,   173,     0,     0,
       0,     0,     0,    18,    19,    20,    21,    22,    23,    56,
      55,    57,    24,    25,    26,    27,    28,    29,    30,    33,
      35,    34,    31,    32,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    50,    49,    51,
      52,    53,    16,    59,    17,    58,    54,   184,     0,     0,
       0,   174,     0,     0,     0,     0,     0,     0,    50,    58,
     183,   182,   189,     0,    62,    61,    63,    64,    66,    65,
      60,   204,   224,   272,   172,   274,     0,   192,     0,    71,
      73,   205,   226,   226,     0,     0,   190,     0,     0,     0,
       0,    22,    14,    13,    15,    12,     0,    10,    41,    11,
       7,     8,     9,   156,   154,   159,   155,   160,   161,   166,
       0,   162,   167,   168,   169,   170,   171,   117,   175,   186,
       0,     0,    72,     0,   220,   226,     0,   225,     0,   269,
     226,   196,     0,    77,    76,    78,     0,     0,     0,     0,
      79,     0,     0,     0,   191,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     123,     0,     0,     0,   124,     0,     0,   176,   177,   185,
     180,     0,     0,     0,    70,    69,     0,   227,   221,     0,
       0,     0,     0,   209,     0,     0,   198,     0,     0,     0,
       0,   226,   271,     0,   275,   163,     0,     0,   149,     0,
       0,     0,     0,     0,     0,     0,   121,     0,     0,     0,
     158,   118,    84,    85,    81,    82,    88,    83,    89,    80,
      94,    95,    92,    93,    90,    91,    96,    97,    86,    87,
      99,     0,   165,   102,     0,    58,   132,    98,   101,   103,
       0,     0,     0,     0,     0,   125,     0,   100,   104,     0,
     188,   187,    67,    68,    74,     0,     0,   218,     0,   216,
     213,   214,   215,   212,   198,   175,   230,     0,   210,     0,
       0,     0,     0,   270,   197,     0,     0,     0,     0,   147,
       0,     0,     0,   150,     0,   122,     0,   119,     0,     0,
       0,   128,     0,     0,     0,     0,     0,     0,     0,   106,
       0,    58,   140,   105,   107,   108,     0,   181,     0,     0,
       0,   208,   198,     0,   250,     0,     0,     0,     0,     0,
     144,   148,     0,     0,     0,     0,     0,     0,   120,   157,
     126,   110,   130,   129,     0,    58,   131,   109,   111,     0,
       0,   136,     0,     0,     0,     0,     0,     0,     0,   112,
      75,   211,   217,   206,   228,     0,   260,     0,     0,     0,
     248,   251,   219,   200,   201,   202,   203,   199,   267,     0,
       0,   198,   145,   146,   164,   152,   153,   151,   135,     0,
     127,   114,   138,   137,     0,    58,   139,   113,   115,   116,
     207,   231,     0,     0,   247,   263,     0,     0,     0,   254,
       0,   249,     0,   198,     0,   266,   133,     0,   143,     0,
     229,   258,   259,     0,   234,     0,   235,     0,   244,     0,
     245,     0,   239,     0,   240,     0,     0,   257,     0,   264,
     198,   134,   141,     0,   261,   262,   236,   233,   232,   246,
     243,   242,   241,   238,   237,   252,   253,     0,   230,   265,
     142,   255,   256,   250,   268
  };

  const short
  parser::yypgoto_[] =
  {
    -333,  -333,  -333,   163,   -19,   -14,   -63,  -332,   165,    93,
    -333,    71,  -333,  -333,  -333,  -102,  -333,  -333,  -333,  -333,
     219,  -333,  -333,   281,  -248,   162,    85,  -333,  -333,   186,
     182,  -279,  -333,  -333,  -333,  -333,  -333,  -290,  -333,   240,
    -333,   289,  -333,   -78,    37,   -96,    -4,  -333,   -90,  -142,
    -333,  -333,  -333
  };

  const short
  parser::yydefgoto_[] =
  {
      -1,     2,     3,   124,   125,   126,    67,    91,   205,   206,
     100,   101,   127,   128,   129,   130,   131,   132,   133,   134,
     157,   135,   136,   137,    95,    19,   199,    11,   280,    68,
     139,   140,     4,    97,     5,    12,   224,   298,   102,   144,
     145,    92,    93,   148,   385,   344,   390,   391,   392,   149,
     150,   104,     6
  };

  const short
  parser::yytable_[] =
  {
      65,    65,    65,   152,   341,    66,    66,    66,   222,    80,
      81,   384,    83,   202,   203,   384,   323,   227,   228,   386,
     217,   308,   309,   349,   146,   173,   197,   426,    99,  -223,
     162,  -223,   163,   467,  -222,    76,  -222,   142,  -273,   433,
     221,   201,   218,     1,   198,   387,   427,   388,   186,   232,
     389,    13,   383,    79,    79,    82,    79,    89,    66,    66,
     239,    66,    90,    77,   468,   219,   143,   146,   434,   400,
     229,   143,    79,    14,   229,   143,    16,    66,   233,   303,
     428,    79,   373,   220,    89,   234,    66,   444,    62,    90,
     440,    64,   166,   167,   168,   169,    22,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   435,   235,   184,   204,   441,   470,   464,   185,   393,
     473,   475,   448,   186,   207,   266,   445,   209,   210,   452,
     164,   442,   165,   465,   481,   211,   316,   476,   200,    15,
     201,   446,    17,   459,   240,   187,   447,   471,   394,   188,
     482,   474,   296,   189,   190,   395,   191,   192,    18,   193,
     194,   449,   322,   195,   212,    71,   213,   262,   453,   265,
     479,   196,   214,   270,    66,   215,   450,   429,    20,    21,
     396,   451,   397,   454,   225,   284,   165,   285,   455,   156,
     216,   287,   291,  -193,     7,     8,    79,   292,    72,     9,
     271,    66,    10,   387,   272,   430,    69,    70,   389,   332,
      73,   273,   274,   324,    74,   325,   275,   166,   167,   168,
     169,    75,   364,   171,    94,   173,   276,   317,   372,   165,
      96,    98,   166,   167,   168,   169,   103,   170,   171,   172,
     173,   151,   159,   176,   177,   178,   179,    99,   186,   182,
     183,   168,   169,   331,   358,   171,   165,   173,    66,   362,
     161,   165,   366,   186,   374,   223,   375,   401,   408,   201,
     165,   412,   414,   165,   421,   432,   422,   422,   320,   438,
     186,   165,   461,   279,   165,   282,   286,   166,   167,   168,
     169,    79,   170,   171,   172,   173,    66,   283,   480,   359,
     165,   299,   294,   295,   182,   183,   365,   437,   302,   297,
     143,    66,   416,   300,   301,   311,   312,   304,   186,   313,
     314,   315,   343,   318,    89,   186,   347,   345,    89,    90,
     348,   355,   356,    90,   363,   380,   409,   463,   404,   357,
     166,   167,   168,   169,   405,   170,   171,   172,   173,   406,
     407,   176,   177,   178,   179,   413,   415,   182,   183,   420,
     423,    66,   439,   425,   443,   460,   456,   457,   424,   466,
     458,   186,   469,   477,   290,   472,   293,   230,   338,   138,
     342,   354,   483,   281,   399,   208,   326,   431,   153,   154,
     155,   158,   147,   484,     0,   478,     0,   160,     0,   166,
     167,   168,   169,    89,   170,   171,   172,   173,    90,     0,
     176,   177,   178,   179,     0,     0,   182,   183,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     186,     0,     0,     0,     0,     0,     0,   226,     0,    79,
     231,     0,     0,     0,    66,   327,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,     0,   263,     0,
     267,     0,   268,   269,     0,     0,   277,   278,     0,     0,
       0,     0,   138,     0,   166,   167,   168,   169,     0,   170,
     171,   172,   173,     0,     0,   176,   177,   178,   179,     0,
       0,   182,   183,     0,     0,     0,     0,   381,   306,     0,
     307,   166,   167,   168,   169,   186,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     336,     0,   184,     0,     0,     0,     0,   185,     0,     0,
       0,     0,   186,     0,     0,     0,     0,     0,     0,     0,
       0,   328,   329,     0,   333,   334,     0,   335,     0,     0,
     337,     0,     0,     0,   187,     0,     0,   339,   188,   340,
       0,     0,   189,   190,     0,   191,   192,     0,   193,   194,
     346,     0,   195,   138,     0,     0,   350,     0,     0,   353,
     196,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     360,   361,     0,     0,     0,     0,     0,   367,   368,     0,
       0,     0,     0,     0,     0,     0,     0,   379,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   138,
       0,   105,   236,   402,     0,     0,   107,   108,   237,     0,
     109,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     410,   411,     0,     0,     0,    23,    24,   417,   418,   419,
      25,     0,     0,    26,   110,   111,     0,     0,    28,     0,
       0,   112,   113,   114,     0,     0,    32,     0,    33,   238,
       0,    34,     0,     0,    35,   115,    36,    37,     0,    38,
       0,    39,    40,     0,     0,    41,     0,     0,     0,    42,
      43,    44,    45,   116,     0,   117,     0,     0,    46,    47,
      48,     0,   118,    50,     0,    51,    52,    53,    54,    55,
      56,     0,     0,     0,    78,    58,     0,   119,    59,     0,
       0,     0,    60,     0,     0,    61,    62,   120,   121,    64,
     122,   123,   105,   321,     0,     0,     0,   107,   108,     0,
       0,   109,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,    24,     0,     0,
       0,    25,     0,     0,    26,   110,   111,     0,     0,    28,
       0,     0,   112,   113,   114,     0,     0,    32,     0,    33,
       0,     0,    34,     0,     0,    35,   115,    36,    37,     0,
      38,     0,    39,    40,     0,     0,    41,     0,     0,     0,
      42,    43,    44,    45,   116,     0,   117,     0,     0,    46,
      47,    48,     0,   118,    50,     0,    51,    52,    53,    54,
      55,    56,    94,     0,     0,    78,    58,     0,   119,    59,
       0,     0,     0,    60,     0,     0,    61,    62,   120,   121,
      64,   122,   123,   105,   371,     0,     0,     0,   107,   108,
       0,     0,   109,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    23,    24,     0,
       0,     0,    25,     0,     0,    26,   110,   111,     0,     0,
      28,     0,     0,   112,   113,   114,     0,     0,    32,     0,
      33,     0,     0,    34,     0,     0,    35,   115,    36,    37,
       0,    38,     0,    39,    40,     0,     0,    41,     0,     0,
       0,    42,    43,    44,    45,   116,     0,   117,     0,     0,
      46,    47,    48,     0,   118,    50,     0,    51,    52,    53,
      54,    55,    56,    94,     0,     0,    78,    58,     0,   119,
      59,     0,     0,     0,    60,     0,     0,    61,    62,   120,
     121,    64,   122,   123,   105,   106,     0,     0,     0,   107,
     108,     0,     0,   109,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    23,    24,
       0,     0,     0,    25,     0,     0,    26,   110,   111,     0,
       0,    28,     0,     0,   112,   113,   114,     0,     0,    32,
       0,    33,     0,     0,    34,     0,     0,    35,   115,    36,
      37,     0,    38,     0,    39,    40,     0,     0,    41,     0,
       0,     0,    42,    43,    44,    45,   116,     0,   117,     0,
       0,    46,    47,    48,     0,   118,    50,     0,    51,    52,
      53,    54,    55,    56,     0,     0,     0,    78,    58,     0,
     119,    59,     0,     0,     0,    60,     0,     0,    61,    62,
     120,   121,    64,   122,   123,   105,     0,     0,     0,     0,
     107,   108,     0,     0,   109,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
      24,     0,     0,     0,    25,     0,     0,    26,   110,   111,
       0,     0,    28,     0,     0,   112,   113,   114,     0,     0,
      32,     0,    33,     0,     0,    34,     0,     0,    35,   115,
      36,    37,     0,    38,     0,    39,    40,     0,     0,    41,
       0,     0,     0,    42,    43,    44,    45,   116,     0,   117,
       0,     0,    46,    47,    48,     0,   118,    50,     0,    51,
      52,    53,    54,    55,    56,     0,     0,     0,    78,    58,
       0,   119,    59,     0,     0,     0,    60,   156,     0,    61,
      62,   120,   121,    64,   122,   123,   105,   436,     0,     0,
       0,   107,   108,     0,     0,   109,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,    24,     0,     0,     0,    25,     0,     0,    26,   110,
     111,     0,     0,    28,     0,     0,   112,   113,   114,     0,
       0,    32,     0,    33,     0,     0,    34,     0,     0,    35,
     115,    36,    37,     0,    38,     0,    39,    40,     0,     0,
      41,     0,     0,     0,    42,    43,    44,    45,   116,     0,
     117,     0,     0,    46,    47,    48,     0,   118,    50,     0,
      51,    52,    53,    54,    55,    56,     0,     0,     0,    78,
      58,     0,   119,    59,     0,     0,     0,    60,     0,     0,
      61,    62,   120,   121,    64,   122,   123,   105,   462,     0,
       0,     0,   107,   108,     0,     0,   109,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    23,    24,     0,     0,     0,    25,     0,     0,    26,
     110,   111,     0,     0,    28,     0,     0,   112,   113,   114,
       0,     0,    32,     0,    33,     0,     0,    34,     0,     0,
      35,   115,    36,    37,     0,    38,     0,    39,    40,     0,
       0,    41,     0,     0,     0,    42,    43,    44,    45,   116,
       0,   117,     0,     0,    46,    47,    48,     0,   118,    50,
       0,    51,    52,    53,    54,    55,    56,     0,     0,     0,
      78,    58,     0,   119,    59,     0,     0,     0,    60,     0,
       0,    61,    62,   120,   121,    64,   122,   123,   105,     0,
       0,     0,     0,   107,   108,     0,     0,   109,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    23,    24,     0,     0,     0,    25,     0,     0,
      26,   110,   111,     0,     0,    28,     0,     0,   112,   113,
     114,     0,     0,    32,     0,    33,     0,     0,    34,     0,
       0,    35,   115,    36,    37,     0,    38,     0,    39,    40,
       0,     0,    41,     0,     0,     0,    42,    43,    44,    45,
     116,     0,   117,     0,     0,    46,    47,    48,     0,   118,
      50,     0,    51,    52,    53,    54,    55,    56,     0,     0,
       0,    78,    58,     0,   119,    59,     0,     0,     0,    60,
       0,     0,    61,    62,   120,   121,    64,   122,   123,   288,
       0,     0,     0,     0,   202,   203,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,    24,     0,     0,     0,    25,     0,
       0,    26,     0,    27,     0,     0,    28,     0,     0,   112,
     113,   114,     0,     0,    32,     0,    33,     0,     0,    34,
       0,     0,    35,   115,    36,    37,     0,    38,   289,    39,
      40,     0,     0,    41,     0,     0,     0,    42,    43,    44,
      45,     0,     0,   117,     0,     0,    46,    47,    48,     0,
      49,    50,     0,    51,    52,    53,    54,    55,    56,   141,
       0,     0,    78,    58,     0,   119,    59,     0,     0,     0,
      60,     0,     0,    61,    62,   120,   121,    64,   122,     0,
       0,     0,     0,    23,    24,     0,     0,     0,    25,     0,
       0,    26,     0,    27,     0,     0,    28,     0,     0,    29,
      30,    31,     0,     0,    32,     0,    33,     0,     0,    34,
       0,     0,    35,     0,    36,    37,     0,    38,     0,    39,
      40,     0,     0,    41,     0,     0,     0,    42,    43,    44,
      45,     0,     0,     0,     0,     0,    46,    47,    48,     0,
      49,    50,     0,    51,    52,    53,    54,    55,    56,   264,
       0,     0,    78,    58,     0,     0,    59,     0,     0,     0,
      60,     0,     0,    61,    62,     0,    63,    64,     0,     0,
       0,     0,     0,    23,    24,     0,     0,     0,    25,     0,
       0,    26,     0,    27,     0,     0,    28,     0,     0,    29,
      30,    31,     0,     0,    32,     0,    33,     0,     0,    34,
       0,     0,    35,     0,    36,    37,     0,    38,     0,    39,
      40,     0,     0,    41,     0,     0,     0,    42,    43,    44,
      45,     0,     0,     0,     0,     0,    46,    47,    48,     0,
      49,    50,     0,    51,    52,    53,    54,    55,    56,   330,
       0,     0,    78,    58,     0,     0,    59,     0,     0,     0,
      60,     0,     0,    61,    62,     0,    63,    64,     0,     0,
       0,     0,     0,    23,    24,     0,     0,     0,    25,     0,
       0,    26,     0,    27,     0,     0,    28,     0,     0,    29,
      30,    31,     0,     0,    32,     0,    33,     0,     0,    34,
       0,     0,    35,     0,    36,    37,     0,    38,     0,    39,
      40,     0,     0,    41,     0,     0,     0,    42,    43,    44,
      45,     0,     0,     0,     0,     0,    46,    47,    48,     0,
      49,    50,     0,    51,    52,    53,    54,    55,    56,     0,
       0,     0,    78,    58,     0,     0,    59,     0,     0,     0,
      60,     0,     0,    61,    62,     0,    63,    64,   166,   167,
     168,   169,     0,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,     0,     0,   184,
       0,     0,     0,     0,   185,   166,   167,   168,   169,   186,
     170,   171,   172,   173,     0,     0,   176,   177,   178,   179,
       0,     0,   182,   183,     0,     0,     0,     0,     0,     0,
       0,   187,     0,     0,     0,   188,   186,     0,     0,   189,
     190,     0,   191,   192,     0,   193,   194,     0,     0,   195,
       0,   370,     0,     0,     0,     0,     0,   196,     0,     0,
       0,     0,     0,     0,   166,   167,   168,   169,   305,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,     0,     0,   184,     0,     0,     0,     0,
     185,   166,   167,   168,   169,   186,   170,   171,   172,   173,
       0,     0,   176,   177,   178,   179,     0,     0,   182,   183,
       0,     0,     0,     0,     0,     0,     0,   187,     0,     0,
       0,   188,   186,     0,     0,   189,   190,     0,   191,   192,
       0,   193,   194,     0,     0,   195,     0,   376,     0,    23,
      24,     0,     0,   196,    25,     0,     0,    26,     0,    27,
       0,     0,    28,   143,   352,    84,    85,    86,     0,     0,
      32,     0,    33,     0,     0,    34,     0,     0,    35,     0,
      36,    37,     0,    38,    87,    39,    40,     0,     0,    41,
       0,     0,     0,    42,    43,    44,    45,     0,     0,     0,
       0,     0,    46,    47,    48,     0,    49,    50,     0,    51,
      52,    53,    54,    55,    56,     0,     0,     0,    78,    58,
       0,     0,    59,     0,     0,     0,    60,     0,     0,    61,
      62,     0,    88,    64,    23,    24,     0,     0,     0,    25,
       0,     0,    26,     0,    27,     0,     0,    28,     0,     0,
      84,    85,    86,     0,     0,    32,     0,    33,     0,     0,
      34,     0,     0,    35,     0,    36,    37,     0,    38,    87,
      39,    40,     0,     0,    41,     0,     0,     0,    42,    43,
      44,    45,     0,     0,     0,     0,     0,    46,    47,    48,
       0,    49,    50,     0,    51,    52,    53,    54,    55,    56,
       0,     0,     0,    78,    58,     0,     0,    59,     0,     0,
       0,    60,     0,     0,    61,    62,     0,    88,    64,    23,
      24,     0,     0,     0,    25,     0,     0,    26,     0,    27,
       0,     0,    28,     0,     0,    29,    30,    31,     0,     0,
      32,     0,    33,     0,     0,    34,     0,     0,    35,     0,
      36,    37,     0,    38,     0,    39,    40,     0,     0,    41,
       0,     0,     0,    42,    43,    44,    45,     0,     0,     0,
       0,     0,    46,    47,    48,     0,    49,    50,     0,    51,
      52,    53,    54,    55,    56,     0,     0,     0,    57,    58,
       0,     0,    59,     0,     0,     0,    60,     0,     0,    61,
      62,     0,    63,    64,    23,    24,     0,     0,     0,    25,
       0,     0,    26,     0,    27,     0,     0,    28,     0,     0,
      29,    30,    31,     0,     0,    32,     0,    33,     0,     0,
      34,     0,     0,    35,     0,    36,    37,     0,    38,     0,
      39,    40,     0,     0,    41,     0,     0,     0,    42,    43,
      44,    45,     0,     0,     0,     0,     0,    46,    47,    48,
       0,    49,    50,     0,    51,    52,    53,    54,    55,    56,
       0,     0,     0,    78,    58,     0,     0,    59,     0,     0,
       0,    60,     0,     0,    61,    62,   382,    63,    64,     0,
     166,   167,   168,   169,     0,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,     0,
       0,   184,     0,     0,     0,     0,   185,   166,   167,   168,
     169,   186,   170,   171,   172,   173,     0,     0,   176,   177,
     178,   179,     0,     0,   182,   183,     0,     0,     0,     0,
       0,     0,     0,   187,     0,     0,     0,   188,   186,     0,
       0,   189,   190,     0,   191,   192,     0,   193,   194,     0,
       0,   195,     0,   377,     0,     0,   398,     0,     0,   196,
     166,   167,   168,   169,     0,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,     0,
       0,   184,     0,     0,     0,     0,   185,   166,   167,   168,
     169,   186,   170,   171,   172,   173,     0,     0,   176,   177,
     178,   179,     0,     0,   182,   183,     0,     0,     0,     0,
       0,     0,     0,   187,     0,     0,     0,   188,   186,     0,
       0,   189,   190,     0,   191,   192,     0,   193,   194,     0,
       0,   195,     0,   378,     0,   166,   167,   168,   169,   196,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,     0,     0,   184,     0,     0,   197,
       0,   185,   166,   167,   168,   169,   186,   170,   171,   172,
     173,     0,     0,   176,   177,   178,   179,   198,     0,   182,
     183,     0,     0,     0,     0,     0,     0,     0,   187,     0,
       0,     0,   188,   186,     0,     0,   189,   190,     0,   191,
     192,     0,   193,   194,     0,     0,   195,     0,     0,     0,
     166,   167,   168,   169,   196,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,     0,
       0,   184,     0,   310,     0,     0,   185,     0,     0,     0,
       0,   186,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   187,     0,     0,     0,   188,     0,     0,
       0,   189,   190,     0,   191,   192,     0,   193,   194,     0,
       0,   195,     0,     0,     0,   166,   167,   168,   169,   196,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,     0,     0,   184,   319,     0,     0,
       0,   185,     0,     0,     0,     0,   186,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   187,     0,
       0,     0,   188,     0,     0,     0,   189,   190,     0,   191,
     192,     0,   193,   194,     0,     0,   195,     0,     0,     0,
     166,   167,   168,   169,   196,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,     0,
       0,   184,     0,     0,     0,     0,   185,     0,     0,     0,
       0,   186,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   351,     0,     0,     0,     0,
       0,     0,     0,   187,     0,     0,     0,   188,     0,     0,
       0,   189,   190,     0,   191,   192,     0,   193,   194,     0,
       0,   195,     0,     0,     0,   166,   167,   168,   169,   196,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,     0,     0,   184,   369,     0,     0,
       0,   185,     0,     0,     0,     0,   186,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   187,     0,
       0,     0,   188,     0,     0,     0,   189,   190,     0,   191,
     192,     0,   193,   194,     0,     0,   195,     0,     0,     0,
     166,   167,   168,   169,   196,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,     0,
       0,   184,     0,     0,     0,     0,   185,     0,     0,     0,
       0,   186,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,   187,     0,     0,     0,   188,     0,     0,
       0,   189,   190,     0,   191,   192,     0,   193,   194,     0,
       0,   195,     0,     0,     0,   166,   167,   168,   169,   196,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,     0,     0,   184,     0,     0,     0,
       0,   185,     0,     0,     0,     0,   186,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   187,     0,
       0,     0,   188,     0,     0,     0,   189,   190,     0,   191,
     192,     0,   193,   194,     0,     0,   195,     0,     0,     0,
     166,   167,   168,   169,   196,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,     0,
       0,   184,     0,     0,     0,     0,   185,     0,     0,     0,
       0,   186,   166,   167,   168,   169,     0,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,     0,     0,   187,     0,     0,     0,   188,   185,     0,
       0,   189,   190,   186,   191,   192,     0,   193,   194,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   196,
       0,     0,     0,     0,     0,   187,     0,     0,     0,   188,
       0,     0,     0,   189,   190,     0,   191,   192,     0,   193,
     194,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   196
  };

  const short
  parser::yycheck_[] =
  {
      19,    20,    21,   105,   294,    19,    20,    21,   150,    72,
      73,   343,    75,     8,     9,   347,   264,    52,    53,    47,
      38,    52,    53,   302,   102,    16,    32,    49,    91,     4,
       3,     6,     5,    46,     4,     3,     6,   100,     4,     4,
       6,     6,    60,    42,    50,    73,    68,    75,    39,    27,
      78,     0,   342,    72,    73,    74,    75,    76,    72,    73,
     162,    75,    76,    31,    77,    83,    41,   145,    33,   348,
     105,    41,    91,     7,   105,    41,    66,    91,    56,   221,
     102,   100,   330,   101,   103,    63,   100,    35,   108,   103,
     422,   111,     8,     9,    10,    11,    75,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,   401,    90,    29,   109,    48,    46,    48,    34,    27,
      46,    48,    35,    39,   143,   188,    74,    38,    39,    35,
       4,    64,     6,    64,    48,    46,   238,    64,     4,    95,
       6,    89,    95,   433,   163,    61,    94,    77,    56,    65,
      64,    77,   215,    69,    70,    63,    72,    73,    62,    75,
      76,    74,   264,    79,    75,    55,    77,   186,    74,   188,
     460,    87,    83,    34,   188,    86,    89,    47,    16,    17,
      88,    94,    90,    89,     4,     4,     6,     6,    94,   105,
     101,   210,   211,    95,    96,    97,   215,   211,     5,   101,
      61,   215,   104,    73,    65,    75,    20,    21,    78,   272,
       5,    72,    73,     3,   103,     5,    77,     8,     9,    10,
      11,    78,   324,    14,    93,    16,    87,     4,   330,     6,
       3,     3,     8,     9,    10,    11,     6,    13,    14,    15,
      16,     4,     3,    19,    20,    21,    22,   310,    39,    25,
      26,    10,    11,   272,     4,    14,     6,    16,   272,     4,
       3,     6,   325,    39,     3,   107,     5,     4,     4,     6,
       6,     4,   374,     6,     4,     4,     6,     6,    54,     4,
      39,     6,     4,   106,     6,   109,     3,     8,     9,    10,
      11,   310,    13,    14,    15,    16,   310,   109,     4,   318,
       6,     3,    77,    71,    25,    26,   325,   409,     3,    78,
      41,   325,   375,    71,    71,     6,     6,    91,    39,     4,
       6,     4,     3,     5,   343,    39,     3,    40,   347,   343,
       3,   110,   110,   347,     4,     4,     3,   439,     4,   110,
       8,     9,    10,    11,     4,    13,    14,    15,    16,     4,
       4,    19,    20,    21,    22,     4,   375,    25,    26,    33,
      67,   375,     3,    47,    67,     4,    67,    47,   387,    28,
      86,    39,    28,    67,   211,    28,   211,   158,   285,    98,
     295,   310,   478,   201,   347,   145,    54,   391,   107,   108,
     109,   110,   103,   483,    -1,   458,    -1,   116,    -1,     8,
       9,    10,    11,   422,    13,    14,    15,    16,   422,    -1,
      19,    20,    21,    22,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,   458,
     159,    -1,    -1,    -1,   458,    54,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,    -1,   187,    -1,
     189,    -1,   191,   192,    -1,    -1,   195,   196,    -1,    -1,
      -1,    -1,   201,    -1,     8,     9,    10,    11,    -1,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,     4,   227,    -1,
     229,     8,     9,    10,    11,    39,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      54,    -1,    29,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   270,   271,    -1,   273,   274,    -1,   276,    -1,    -1,
     279,    -1,    -1,    -1,    61,    -1,    -1,   286,    65,   288,
      -1,    -1,    69,    70,    -1,    72,    73,    -1,    75,    76,
     299,    -1,    79,   302,    -1,    -1,   305,    -1,    -1,   308,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     319,   320,    -1,    -1,    -1,    -1,    -1,   326,   327,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   336,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   348,
      -1,     3,     4,   352,    -1,    -1,     8,     9,    10,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     369,   370,    -1,    -1,    -1,    27,    28,   376,   377,   378,
      32,    -1,    -1,    35,    36,    37,    -1,    -1,    40,    -1,
      -1,    43,    44,    45,    -1,    -1,    48,    -1,    50,    51,
      -1,    53,    -1,    -1,    56,    57,    58,    59,    -1,    61,
      -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,    71,
      72,    73,    74,    75,    -1,    77,    -1,    -1,    80,    81,
      82,    -1,    84,    85,    -1,    87,    88,    89,    90,    91,
      92,    -1,    -1,    -1,    96,    97,    -1,    99,   100,    -1,
      -1,    -1,   104,    -1,    -1,   107,   108,   109,   110,   111,
     112,   113,     3,     4,    -1,    -1,    -1,     8,     9,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    32,    -1,    -1,    35,    36,    37,    -1,    -1,    40,
      -1,    -1,    43,    44,    45,    -1,    -1,    48,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,
      71,    72,    73,    74,    75,    -1,    77,    -1,    -1,    80,
      81,    82,    -1,    84,    85,    -1,    87,    88,    89,    90,
      91,    92,    93,    -1,    -1,    96,    97,    -1,    99,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,   108,   109,   110,
     111,   112,   113,     3,     4,    -1,    -1,    -1,     8,     9,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    32,    -1,    -1,    35,    36,    37,    -1,    -1,
      40,    -1,    -1,    43,    44,    45,    -1,    -1,    48,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    57,    58,    59,
      -1,    61,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,
      -1,    71,    72,    73,    74,    75,    -1,    77,    -1,    -1,
      80,    81,    82,    -1,    84,    85,    -1,    87,    88,    89,
      90,    91,    92,    93,    -1,    -1,    96,    97,    -1,    99,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,   108,   109,
     110,   111,   112,   113,     3,     4,    -1,    -1,    -1,     8,
       9,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    32,    -1,    -1,    35,    36,    37,    -1,
      -1,    40,    -1,    -1,    43,    44,    45,    -1,    -1,    48,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    57,    58,
      59,    -1,    61,    -1,    63,    64,    -1,    -1,    67,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    -1,    77,    -1,
      -1,    80,    81,    82,    -1,    84,    85,    -1,    87,    88,
      89,    90,    91,    92,    -1,    -1,    -1,    96,    97,    -1,
      99,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,   108,
     109,   110,   111,   112,   113,     3,    -1,    -1,    -1,    -1,
       8,     9,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    32,    -1,    -1,    35,    36,    37,
      -1,    -1,    40,    -1,    -1,    43,    44,    45,    -1,    -1,
      48,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    57,
      58,    59,    -1,    61,    -1,    63,    64,    -1,    -1,    67,
      -1,    -1,    -1,    71,    72,    73,    74,    75,    -1,    77,
      -1,    -1,    80,    81,    82,    -1,    84,    85,    -1,    87,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    96,    97,
      -1,    99,   100,    -1,    -1,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,   112,   113,     3,     4,    -1,    -1,
      -1,     8,     9,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    32,    -1,    -1,    35,    36,
      37,    -1,    -1,    40,    -1,    -1,    43,    44,    45,    -1,
      -1,    48,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      57,    58,    59,    -1,    61,    -1,    63,    64,    -1,    -1,
      67,    -1,    -1,    -1,    71,    72,    73,    74,    75,    -1,
      77,    -1,    -1,    80,    81,    82,    -1,    84,    85,    -1,
      87,    88,    89,    90,    91,    92,    -1,    -1,    -1,    96,
      97,    -1,    99,   100,    -1,    -1,    -1,   104,    -1,    -1,
     107,   108,   109,   110,   111,   112,   113,     3,     4,    -1,
      -1,    -1,     8,     9,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    32,    -1,    -1,    35,
      36,    37,    -1,    -1,    40,    -1,    -1,    43,    44,    45,
      -1,    -1,    48,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    -1,    63,    64,    -1,
      -1,    67,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      -1,    77,    -1,    -1,    80,    81,    82,    -1,    84,    85,
      -1,    87,    88,    89,    90,    91,    92,    -1,    -1,    -1,
      96,    97,    -1,    99,   100,    -1,    -1,    -1,   104,    -1,
      -1,   107,   108,   109,   110,   111,   112,   113,     3,    -1,
      -1,    -1,    -1,     8,     9,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    32,    -1,    -1,
      35,    36,    37,    -1,    -1,    40,    -1,    -1,    43,    44,
      45,    -1,    -1,    48,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    57,    58,    59,    -1,    61,    -1,    63,    64,
      -1,    -1,    67,    -1,    -1,    -1,    71,    72,    73,    74,
      75,    -1,    77,    -1,    -1,    80,    81,    82,    -1,    84,
      85,    -1,    87,    88,    89,    90,    91,    92,    -1,    -1,
      -1,    96,    97,    -1,    99,   100,    -1,    -1,    -1,   104,
      -1,    -1,   107,   108,   109,   110,   111,   112,   113,     3,
      -1,    -1,    -1,    -1,     8,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    32,    -1,
      -1,    35,    -1,    37,    -1,    -1,    40,    -1,    -1,    43,
      44,    45,    -1,    -1,    48,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,    73,
      74,    -1,    -1,    77,    -1,    -1,    80,    81,    82,    -1,
      84,    85,    -1,    87,    88,    89,    90,    91,    92,     3,
      -1,    -1,    96,    97,    -1,    99,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,   108,   109,   110,   111,   112,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    32,    -1,
      -1,    35,    -1,    37,    -1,    -1,    40,    -1,    -1,    43,
      44,    45,    -1,    -1,    48,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    58,    59,    -1,    61,    -1,    63,
      64,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,    -1,
      84,    85,    -1,    87,    88,    89,    90,    91,    92,     3,
      -1,    -1,    96,    97,    -1,    -1,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,   108,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    32,    -1,
      -1,    35,    -1,    37,    -1,    -1,    40,    -1,    -1,    43,
      44,    45,    -1,    -1,    48,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    58,    59,    -1,    61,    -1,    63,
      64,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,    -1,
      84,    85,    -1,    87,    88,    89,    90,    91,    92,     3,
      -1,    -1,    96,    97,    -1,    -1,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,   108,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    32,    -1,
      -1,    35,    -1,    37,    -1,    -1,    40,    -1,    -1,    43,
      44,    45,    -1,    -1,    48,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    58,    59,    -1,    61,    -1,    63,
      64,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,    -1,
      84,    85,    -1,    87,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    96,    97,    -1,    -1,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,   108,    -1,   110,   111,     8,     9,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    29,
      -1,    -1,    -1,    -1,    34,     8,     9,    10,    11,    39,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    -1,    -1,    65,    39,    -1,    -1,    69,
      70,    -1,    72,    73,    -1,    75,    76,    -1,    -1,    79,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,     8,     9,    10,    11,    98,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      34,     8,     9,    10,    11,    39,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    -1,    -1,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    65,    39,    -1,    -1,    69,    70,    -1,    72,    73,
      -1,    75,    76,    -1,    -1,    79,    -1,    54,    -1,    27,
      28,    -1,    -1,    87,    32,    -1,    -1,    35,    -1,    37,
      -1,    -1,    40,    41,    98,    43,    44,    45,    -1,    -1,
      48,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      58,    59,    -1,    61,    62,    63,    64,    -1,    -1,    67,
      -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    80,    81,    82,    -1,    84,    85,    -1,    87,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    96,    97,
      -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,
     108,    -1,   110,   111,    27,    28,    -1,    -1,    -1,    32,
      -1,    -1,    35,    -1,    37,    -1,    -1,    40,    -1,    -1,
      43,    44,    45,    -1,    -1,    48,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    58,    59,    -1,    61,    62,
      63,    64,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,
      -1,    84,    85,    -1,    87,    88,    89,    90,    91,    92,
      -1,    -1,    -1,    96,    97,    -1,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,   108,    -1,   110,   111,    27,
      28,    -1,    -1,    -1,    32,    -1,    -1,    35,    -1,    37,
      -1,    -1,    40,    -1,    -1,    43,    44,    45,    -1,    -1,
      48,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      58,    59,    -1,    61,    -1,    63,    64,    -1,    -1,    67,
      -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    80,    81,    82,    -1,    84,    85,    -1,    87,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    96,    97,
      -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,
     108,    -1,   110,   111,    27,    28,    -1,    -1,    -1,    32,
      -1,    -1,    35,    -1,    37,    -1,    -1,    40,    -1,    -1,
      43,    44,    45,    -1,    -1,    48,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    58,    59,    -1,    61,    -1,
      63,    64,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,
      -1,    84,    85,    -1,    87,    88,    89,    90,    91,    92,
      -1,    -1,    -1,    96,    97,    -1,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,   108,     4,   110,   111,    -1,
       8,     9,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    34,     8,     9,    10,
      11,    39,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    65,    39,    -1,
      -1,    69,    70,    -1,    72,    73,    -1,    75,    76,    -1,
      -1,    79,    -1,    54,    -1,    -1,     4,    -1,    -1,    87,
       8,     9,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    34,     8,     9,    10,
      11,    39,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    65,    39,    -1,
      -1,    69,    70,    -1,    72,    73,    -1,    75,    76,    -1,
      -1,    79,    -1,    54,    -1,     8,     9,    10,    11,    87,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    -1,    -1,    32,
      -1,    34,     8,     9,    10,    11,    39,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    50,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    65,    39,    -1,    -1,    69,    70,    -1,    72,
      73,    -1,    75,    76,    -1,    -1,    79,    -1,    -1,    -1,
       8,     9,    10,    11,    87,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    -1,    31,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    69,    70,    -1,    72,    73,    -1,    75,    76,    -1,
      -1,    79,    -1,    -1,    -1,     8,     9,    10,    11,    87,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    65,    -1,    -1,    -1,    69,    70,    -1,    72,
      73,    -1,    75,    76,    -1,    -1,    79,    -1,    -1,    -1,
       8,     9,    10,    11,    87,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    69,    70,    -1,    72,    73,    -1,    75,    76,    -1,
      -1,    79,    -1,    -1,    -1,     8,     9,    10,    11,    87,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    65,    -1,    -1,    -1,    69,    70,    -1,    72,
      73,    -1,    75,    76,    -1,    -1,    79,    -1,    -1,    -1,
       8,     9,    10,    11,    87,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    69,    70,    -1,    72,    73,    -1,    75,    76,    -1,
      -1,    79,    -1,    -1,    -1,     8,     9,    10,    11,    87,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    65,    -1,    -1,    -1,    69,    70,    -1,    72,
      73,    -1,    75,    76,    -1,    -1,    79,    -1,    -1,    -1,
       8,     9,    10,    11,    87,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    39,     8,     9,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    61,    -1,    -1,    -1,    65,    34,    -1,
      -1,    69,    70,    39,    72,    73,    -1,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    69,    70,    -1,    72,    73,    -1,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    42,   115,   116,   146,   148,   166,    96,    97,   101,
     104,   141,   149,     0,     7,    95,    66,    95,    62,   139,
     139,   139,    75,    27,    28,    32,    35,    37,    40,    43,
      44,    45,    48,    50,    53,    56,    58,    59,    61,    63,
      64,    67,    71,    72,    73,    74,    80,    81,    82,    84,
      85,    87,    88,    89,    90,    91,    92,    96,    97,   100,
     104,   107,   108,   110,   111,   118,   119,   120,   143,   143,
     143,    55,     5,     5,   103,    78,     3,    31,    96,   118,
     120,   120,   118,   120,    43,    44,    45,    62,   110,   118,
     119,   121,   155,   156,    93,   138,     3,   147,     3,   120,
     124,   125,   152,     6,   165,     3,     4,     8,     9,    12,
      36,    37,    43,    44,    45,    57,    75,    77,    84,    99,
     109,   110,   112,   113,   117,   118,   119,   126,   127,   128,
     129,   130,   131,   132,   133,   135,   136,   137,   137,   144,
     145,     3,   120,    41,   153,   154,   157,   155,   157,   163,
     164,     4,   129,   137,   137,   137,   105,   134,   137,     3,
     137,     3,     3,     5,     4,     6,     8,     9,    10,    11,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    29,    34,    39,    61,    65,    69,
      70,    72,    73,    75,    76,    79,    87,    32,    50,   140,
       4,     6,     8,     9,   109,   122,   123,   118,   153,    38,
      39,    46,    75,    77,    83,    86,   101,    38,    60,    83,
     101,     6,   163,   107,   150,     4,   137,    52,    53,   105,
     134,   137,    27,    56,    63,    90,     4,    10,    51,   129,
     118,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   118,   137,     3,   118,   120,   137,   137,   137,
      34,    61,    65,    72,    73,    77,    87,   137,   137,   106,
     142,   144,   109,   109,     4,     6,     3,   118,     3,    62,
     117,   118,   119,   122,    77,    71,   120,    78,   151,     3,
      71,    71,     3,   163,    91,    98,   137,   137,    52,    53,
      31,     6,     6,     4,     6,     4,   129,     4,     5,    30,
      54,     4,   129,   138,     3,     5,    54,    54,   137,   137,
       3,   118,   120,   137,   137,   137,    54,   137,   123,   137,
     137,   151,   140,     3,   159,    40,   137,     3,     3,   145,
     137,    53,    98,   137,   125,   110,   110,   110,     4,   118,
     137,   137,     4,     4,   129,   118,   120,   137,   137,    30,
      54,     4,   129,   138,     3,     5,    54,    54,    54,   137,
       4,     4,     4,   151,   121,   158,    47,    73,    75,    78,
     160,   161,   162,    27,    56,    63,    88,    90,     4,   158,
     145,     4,   137,    53,     4,     4,     4,     4,     4,     3,
     137,   137,     4,     4,   129,   118,   120,   137,   137,   137,
      33,     4,     6,    67,   118,    47,    49,    68,   102,    47,
      75,   160,     4,     4,    33,   151,     4,   129,     4,     3,
     121,    48,    64,    67,    35,    74,    89,    94,    35,    74,
      89,    94,    35,    74,    89,    94,    67,    47,    86,   151,
       4,     4,     4,   129,    48,    64,    28,    46,    77,    28,
      46,    77,    28,    46,    77,    48,    64,    67,   120,   151,
       4,    48,    64,   159,   162
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   114,   115,   115,   116,   116,   116,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   118,   118,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   120,   120,   120,   120,   120,   120,
     121,   121,   121,   121,   121,   121,   121,   122,   122,   123,
     123,   124,   124,   125,   125,   125,   126,   126,   126,   126,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   129,   129,   130,
     130,   130,   130,   131,   131,   131,   132,   132,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   134,   134,   135,   135,   135,   135,
     136,   136,   136,   136,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   138,   139,   139,   140,   140,   140,   141,   141,
     142,   142,   143,   143,   143,   144,   145,   145,   146,   147,
     147,   147,   148,   149,   149,   149,   150,   150,   151,   151,
     151,   151,   151,   151,   152,   152,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     154,   154,   155,   155,   156,   156,   157,   157,   158,   158,
     159,   159,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   161,   161,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   163,   163,   163,   163,   163,   164,
     164,   164,   165,   165,   166,   166
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     2,     1,     4,     6,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     4,     4,     4,     4,     5,
       5,     5,     5,     6,     6,     6,     6,     1,     3,     4,
       5,     3,     4,     2,     2,     3,     5,     6,     4,     5,
       5,     5,     3,     7,     8,     6,     5,     6,     6,     6,
       4,     8,     9,     7,     4,     5,     6,     4,     5,     3,
       4,     6,     6,     6,     1,     1,     1,     5,     3,     1,
       1,     1,     1,     3,     6,     3,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     0,     1,     1,     0,     1,
       0,     2,     3,     3,     1,     2,     1,     3,    11,     0,
       2,     3,     8,     0,     1,     1,     0,     2,     0,     3,
       3,     3,     3,     3,     0,     1,     5,     6,     4,     2,
       3,     5,     3,     3,     3,     3,     3,     5,     3,     5,
       1,     2,     3,     2,     1,     3,     0,     2,     1,     3,
       0,     3,     4,     4,     3,     3,     4,     4,     4,     3,
       3,     4,     4,     4,     3,     3,     4,     2,     1,     2,
       0,     1,     4,     4,     2,     5,     5,     3,     3,     3,
       1,     4,     4,     2,     7,     8,     6,     5,    10,     1,
       3,     2,     0,     2,     7,    10
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"(\"", "\")\"", "\".\"",
  "\",\"", "\";\"", "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"~\"", "\"&\"",
  "\"%\"", "\"|\"", "\"||\"", "\"=\"", "\"==\"", "\">\"", "\">=\"",
  "\"<\"", "\"<=\"", "\"!=\"", "\"<>\"", "\"<<\"", "\">>\"", "\"ABORT\"",
  "\"ACTION\"", "\"AND\"", "\"AND BETWEEN\"", "\"AS\"", "\"ASC\"",
  "\"AUTOINCREMENT\"", "\"BETWEEN\"", "\"CASCADE\"", "\"CASE\"",
  "\"CAST\"", "\"CHECK\"", "\"COLLATE\"", "\"CONFLICT\"", "\"CONSTRAINT\"",
  "\"CREATE\"", "\"CURRENT_DATE\"", "\"CURRENT_TIME\"",
  "\"CURRENT_TIMESTAMP\"", "\"DEFAULT\"", "\"DEFERRABLE\"", "\"DEFERRED\"",
  "\"DELETE\"", "\"DESC\"", "\"DISTINCT\"", "\"ELSE\"", "\"END\"",
  "\"ESCAPE\"", "\"EXISTS\"", "\"FAIL\"", "\"FALSE\"", "\"FILTER\"",
  "\"FOLLOWING\"", "\"FOREIGN\"", "\"GLOB\"", "\"IF\"", "\"IGNORE\"",
  "\"IMMEDIATE\"", "\"IN\"", "\"INDEX\"", "\"INITIALLY\"", "\"INSERT\"",
  "\"IS\"", "\"ISNULL\"", "\"KEY\"", "\"LIKE\"", "\"MATCH\"", "\"NO\"",
  "\"NOT\"", "\"NOTNULL\"", "\"NULL\"", "\"ON\"", "\"OR\"", "\"OVER\"",
  "\"PARTITION\"", "\"PRECEDING\"", "\"PRIMARY\"", "\"RAISE\"",
  "\"RANGE\"", "\"REFERENCES\"", "\"REGEXP\"", "\"REPLACE\"",
  "\"RESTRICT\"", "\"ROLLBACK\"", "\"ROWID\"", "\"ROWS\"", "\"SELECT\"",
  "\"SET\"", "\"TABLE\"", "\"TEMP\"", "\"TEMPORARY\"", "\"THEN\"",
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
  "optional_temporary", "optional_withoutrowid", "optional_conflictclause",
  "optional_typename", "columnconstraint", "columnconstraint_list",
  "columndef", "columndef_list", "optional_constraintname",
  "columnid_list", "optional_columnid_with_paren_list", "fk_clause_part",
  "fk_clause_part_list", "optional_fk_clause", "tableconstraint",
  "tableconstraint_list", "optional_tableconstraint_list",
  "createtable_stmt", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short
  parser::yyrline_[] =
  {
       0,   301,   301,   302,   306,   307,   308,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   328,   329,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   373,   374,   375,   376,   377,   378,
     382,   383,   384,   385,   386,   387,   388,   392,   393,   397,
     398,   402,   403,   407,   408,   409,   413,   414,   415,   416,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   463,   464,   468,
     469,   470,   471,   475,   476,   477,   481,   482,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   505,   506,   510,   511,   512,   513,
     517,   518,   519,   520,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,   541,   550,   558,   559,   563,   564,   565,   573,   574,
     578,   579,   583,   584,   585,   589,   603,   604,   608,   623,
     624,   625,   629,   641,   642,   643,   647,   648,   652,   653,
     654,   655,   656,   657,   661,   662,   666,   675,   685,   690,
     695,   700,   706,   712,   718,   724,   731,   737,   743,   749,
     763,   764,   768,   806,   810,   811,   815,   816,   820,   821,
     825,   826,   830,   831,   832,   833,   834,   835,   836,   837,
     838,   839,   840,   841,   842,   843,   844,   845,   849,   850,
     854,   855,   856,   857,   858,   859,   860,   861,   862,   863,
     864,   865,   866,   867,   871,   877,   884,   890,   894,   902,
     903,   904,   908,   909,   913,   917
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned yylno = yyrline_[yyrule];
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
#line 4381 "sqlite3_parser.cpp"

#line 939 "sqlite3_parser.yy"


void sqlb::parser::parser::error(const location_type& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}
