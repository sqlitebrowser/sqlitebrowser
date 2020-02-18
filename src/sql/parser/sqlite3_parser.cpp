// A Bison parser, made by GNU Bison 3.5.1.

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

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "sqlite3_parser.hpp"


// Unqualified %code blocks.
#line 88 "sqlite3_parser.yy"

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
#line 159 "sqlite3_parser.cpp"


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
      return yystos_[+state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 158: // columnconstraint
        value.YY_MOVE_OR_COPY< ColumnConstraintInfo > (YY_MOVE (that.value));
        break;

      case 159: // columnconstraint_list
        value.YY_MOVE_OR_COPY< ColumnConstraintInfoVector > (YY_MOVE (that.value));
        break;

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

      case 168: // tableconstraint
        value.YY_MOVE_OR_COPY< sqlb::ConstraintPtr > (YY_MOVE (that.value));
        break;

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
    switch (that.type_get ())
    {
      case 158: // columnconstraint
        value.move< ColumnConstraintInfo > (YY_MOVE (that.value));
        break;

      case 159: // columnconstraint_list
        value.move< ColumnConstraintInfoVector > (YY_MOVE (that.value));
        break;

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

      case 168: // tableconstraint
        value.move< sqlb::ConstraintPtr > (YY_MOVE (that.value));
        break;

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
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 158: // columnconstraint
        value.copy< ColumnConstraintInfo > (that.value);
        break;

      case 159: // columnconstraint_list
        value.copy< ColumnConstraintInfoVector > (that.value);
        break;

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

      case 168: // tableconstraint
        value.copy< sqlb::ConstraintPtr > (that.value);
        break;

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
    switch (that.type_get ())
    {
      case 158: // columnconstraint
        value.move< ColumnConstraintInfo > (that.value);
        break;

      case 159: // columnconstraint_list
        value.move< ColumnConstraintInfoVector > (that.value);
        break;

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

      case 168: // tableconstraint
        value.move< sqlb::ConstraintPtr > (that.value);
        break;

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
      case 158: // columnconstraint
        yylhs.value.emplace< ColumnConstraintInfo > ();
        break;

      case 159: // columnconstraint_list
        yylhs.value.emplace< ColumnConstraintInfoVector > ();
        break;

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

      case 168: // tableconstraint
        yylhs.value.emplace< sqlb::ConstraintPtr > ();
        break;

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
#line 314 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::IndexPtr > (); }
#line 1492 "sqlite3_parser.cpp"
    break;

  case 5:
#line 315 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::TablePtr > (); }
#line 1498 "sqlite3_parser.cpp"
    break;

  case 6:
#line 316 "sqlite3_parser.yy"
                                        { drv.result = yystack_[0].value.as < sqlb::TablePtr > (); }
#line 1504 "sqlite3_parser.cpp"
    break;

  case 7:
#line 324 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1510 "sqlite3_parser.cpp"
    break;

  case 8:
#line 325 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1516 "sqlite3_parser.cpp"
    break;

  case 9:
#line 326 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1522 "sqlite3_parser.cpp"
    break;

  case 10:
#line 327 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1528 "sqlite3_parser.cpp"
    break;

  case 11:
#line 328 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1534 "sqlite3_parser.cpp"
    break;

  case 12:
#line 329 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1540 "sqlite3_parser.cpp"
    break;

  case 13:
#line 330 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1546 "sqlite3_parser.cpp"
    break;

  case 14:
#line 331 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1552 "sqlite3_parser.cpp"
    break;

  case 15:
#line 332 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1558 "sqlite3_parser.cpp"
    break;

  case 16:
#line 336 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1564 "sqlite3_parser.cpp"
    break;

  case 17:
#line 337 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1570 "sqlite3_parser.cpp"
    break;

  case 18:
#line 342 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1576 "sqlite3_parser.cpp"
    break;

  case 19:
#line 343 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1582 "sqlite3_parser.cpp"
    break;

  case 20:
#line 344 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1588 "sqlite3_parser.cpp"
    break;

  case 21:
#line 345 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1594 "sqlite3_parser.cpp"
    break;

  case 22:
#line 346 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1600 "sqlite3_parser.cpp"
    break;

  case 23:
#line 347 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1606 "sqlite3_parser.cpp"
    break;

  case 24:
#line 348 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1612 "sqlite3_parser.cpp"
    break;

  case 25:
#line 349 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1618 "sqlite3_parser.cpp"
    break;

  case 26:
#line 350 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1624 "sqlite3_parser.cpp"
    break;

  case 27:
#line 351 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1630 "sqlite3_parser.cpp"
    break;

  case 28:
#line 352 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1636 "sqlite3_parser.cpp"
    break;

  case 29:
#line 353 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1642 "sqlite3_parser.cpp"
    break;

  case 30:
#line 354 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1648 "sqlite3_parser.cpp"
    break;

  case 31:
#line 355 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1654 "sqlite3_parser.cpp"
    break;

  case 32:
#line 356 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1660 "sqlite3_parser.cpp"
    break;

  case 33:
#line 357 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1666 "sqlite3_parser.cpp"
    break;

  case 34:
#line 358 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1672 "sqlite3_parser.cpp"
    break;

  case 35:
#line 359 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1678 "sqlite3_parser.cpp"
    break;

  case 36:
#line 360 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1684 "sqlite3_parser.cpp"
    break;

  case 37:
#line 361 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1690 "sqlite3_parser.cpp"
    break;

  case 38:
#line 362 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1696 "sqlite3_parser.cpp"
    break;

  case 39:
#line 363 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1702 "sqlite3_parser.cpp"
    break;

  case 40:
#line 364 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1708 "sqlite3_parser.cpp"
    break;

  case 41:
#line 365 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1714 "sqlite3_parser.cpp"
    break;

  case 42:
#line 366 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1720 "sqlite3_parser.cpp"
    break;

  case 43:
#line 367 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1726 "sqlite3_parser.cpp"
    break;

  case 44:
#line 368 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1732 "sqlite3_parser.cpp"
    break;

  case 45:
#line 369 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1738 "sqlite3_parser.cpp"
    break;

  case 46:
#line 370 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1744 "sqlite3_parser.cpp"
    break;

  case 47:
#line 371 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1750 "sqlite3_parser.cpp"
    break;

  case 48:
#line 372 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1756 "sqlite3_parser.cpp"
    break;

  case 49:
#line 373 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1762 "sqlite3_parser.cpp"
    break;

  case 50:
#line 374 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1768 "sqlite3_parser.cpp"
    break;

  case 51:
#line 375 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1774 "sqlite3_parser.cpp"
    break;

  case 52:
#line 376 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1780 "sqlite3_parser.cpp"
    break;

  case 53:
#line 377 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1786 "sqlite3_parser.cpp"
    break;

  case 54:
#line 378 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1792 "sqlite3_parser.cpp"
    break;

  case 55:
#line 379 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1798 "sqlite3_parser.cpp"
    break;

  case 56:
#line 380 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1804 "sqlite3_parser.cpp"
    break;

  case 57:
#line 384 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1810 "sqlite3_parser.cpp"
    break;

  case 58:
#line 385 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1816 "sqlite3_parser.cpp"
    break;

  case 59:
#line 386 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1822 "sqlite3_parser.cpp"
    break;

  case 60:
#line 387 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1828 "sqlite3_parser.cpp"
    break;

  case 61:
#line 388 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1834 "sqlite3_parser.cpp"
    break;

  case 62:
#line 389 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1840 "sqlite3_parser.cpp"
    break;

  case 63:
#line 393 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1846 "sqlite3_parser.cpp"
    break;

  case 64:
#line 394 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1852 "sqlite3_parser.cpp"
    break;

  case 65:
#line 395 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1858 "sqlite3_parser.cpp"
    break;

  case 66:
#line 396 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1864 "sqlite3_parser.cpp"
    break;

  case 67:
#line 397 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1870 "sqlite3_parser.cpp"
    break;

  case 68:
#line 398 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1876 "sqlite3_parser.cpp"
    break;

  case 69:
#line 399 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1882 "sqlite3_parser.cpp"
    break;

  case 70:
#line 403 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1888 "sqlite3_parser.cpp"
    break;

  case 71:
#line 404 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1894 "sqlite3_parser.cpp"
    break;

  case 72:
#line 408 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1900 "sqlite3_parser.cpp"
    break;

  case 73:
#line 409 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1906 "sqlite3_parser.cpp"
    break;

  case 74:
#line 413 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1912 "sqlite3_parser.cpp"
    break;

  case 75:
#line 414 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 1918 "sqlite3_parser.cpp"
    break;

  case 76:
#line 418 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1924 "sqlite3_parser.cpp"
    break;

  case 77:
#line 419 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1930 "sqlite3_parser.cpp"
    break;

  case 78:
#line 420 "sqlite3_parser.yy"
                                                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + "(" + yystack_[3].value.as < std::string > () + ", " + yystack_[1].value.as < std::string > () + ")"; }
#line 1936 "sqlite3_parser.cpp"
    break;

  case 79:
#line 424 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1942 "sqlite3_parser.cpp"
    break;

  case 80:
#line 425 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1948 "sqlite3_parser.cpp"
    break;

  case 81:
#line 426 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "~" + yystack_[0].value.as < std::string > (); }
#line 1954 "sqlite3_parser.cpp"
    break;

  case 82:
#line 427 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = "NOT " + yystack_[0].value.as < std::string > (); }
#line 1960 "sqlite3_parser.cpp"
    break;

  case 83:
#line 431 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " || " + yystack_[0].value.as < std::string > (); }
#line 1966 "sqlite3_parser.cpp"
    break;

  case 84:
#line 432 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " * " + yystack_[0].value.as < std::string > (); }
#line 1972 "sqlite3_parser.cpp"
    break;

  case 85:
#line 433 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " / " + yystack_[0].value.as < std::string > (); }
#line 1978 "sqlite3_parser.cpp"
    break;

  case 86:
#line 434 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " % " + yystack_[0].value.as < std::string > (); }
#line 1984 "sqlite3_parser.cpp"
    break;

  case 87:
#line 435 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " + " + yystack_[0].value.as < std::string > (); }
#line 1990 "sqlite3_parser.cpp"
    break;

  case 88:
#line 436 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " - " + yystack_[0].value.as < std::string > (); }
#line 1996 "sqlite3_parser.cpp"
    break;

  case 89:
#line 437 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " << " + yystack_[0].value.as < std::string > (); }
#line 2002 "sqlite3_parser.cpp"
    break;

  case 90:
#line 438 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >> " + yystack_[0].value.as < std::string > (); }
#line 2008 "sqlite3_parser.cpp"
    break;

  case 91:
#line 439 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " & " + yystack_[0].value.as < std::string > (); }
#line 2014 "sqlite3_parser.cpp"
    break;

  case 92:
#line 440 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " | " + yystack_[0].value.as < std::string > (); }
#line 2020 "sqlite3_parser.cpp"
    break;

  case 93:
#line 441 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " < " + yystack_[0].value.as < std::string > (); }
#line 2026 "sqlite3_parser.cpp"
    break;

  case 94:
#line 442 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <= " + yystack_[0].value.as < std::string > (); }
#line 2032 "sqlite3_parser.cpp"
    break;

  case 95:
#line 443 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " > " + yystack_[0].value.as < std::string > (); }
#line 2038 "sqlite3_parser.cpp"
    break;

  case 96:
#line 444 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >= " + yystack_[0].value.as < std::string > (); }
#line 2044 "sqlite3_parser.cpp"
    break;

  case 97:
#line 445 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " = " + yystack_[0].value.as < std::string > (); }
#line 2050 "sqlite3_parser.cpp"
    break;

  case 98:
#line 446 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " == " + yystack_[0].value.as < std::string > (); }
#line 2056 "sqlite3_parser.cpp"
    break;

  case 99:
#line 447 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " != " + yystack_[0].value.as < std::string > (); }
#line 2062 "sqlite3_parser.cpp"
    break;

  case 100:
#line 448 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <> " + yystack_[0].value.as < std::string > (); }
#line 2068 "sqlite3_parser.cpp"
    break;

  case 101:
#line 449 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IS " + yystack_[0].value.as < std::string > (); }
#line 2074 "sqlite3_parser.cpp"
    break;

  case 102:
#line 450 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2080 "sqlite3_parser.cpp"
    break;

  case 103:
#line 451 "sqlite3_parser.yy"
                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " OR " + yystack_[0].value.as < std::string > (); }
#line 2086 "sqlite3_parser.cpp"
    break;

  case 104:
#line 455 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " LIKE " + yystack_[0].value.as < std::string > (); }
#line 2092 "sqlite3_parser.cpp"
    break;

  case 105:
#line 456 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " GLOB " + yystack_[0].value.as < std::string > (); }
#line 2098 "sqlite3_parser.cpp"
    break;

  case 106:
#line 457 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " MATCH " + yystack_[0].value.as < std::string > (); }
#line 2104 "sqlite3_parser.cpp"
    break;

  case 107:
#line 458 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " REGEXP " + yystack_[0].value.as < std::string > (); }
#line 2110 "sqlite3_parser.cpp"
    break;

  case 108:
#line 459 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT LIKE " + yystack_[0].value.as < std::string > (); }
#line 2116 "sqlite3_parser.cpp"
    break;

  case 109:
#line 460 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT GLOB " + yystack_[0].value.as < std::string > (); }
#line 2122 "sqlite3_parser.cpp"
    break;

  case 110:
#line 461 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT MATCH " + yystack_[0].value.as < std::string > (); }
#line 2128 "sqlite3_parser.cpp"
    break;

  case 111:
#line 462 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT REGEXP " + yystack_[0].value.as < std::string > (); }
#line 2134 "sqlite3_parser.cpp"
    break;

  case 112:
#line 463 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " LIKE " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2140 "sqlite3_parser.cpp"
    break;

  case 113:
#line 464 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " GLOB " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2146 "sqlite3_parser.cpp"
    break;

  case 114:
#line 465 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " MATCH " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2152 "sqlite3_parser.cpp"
    break;

  case 115:
#line 466 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " REGEXP " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2158 "sqlite3_parser.cpp"
    break;

  case 116:
#line 467 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT LIKE " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2164 "sqlite3_parser.cpp"
    break;

  case 117:
#line 468 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT GLOB " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2170 "sqlite3_parser.cpp"
    break;

  case 118:
#line 469 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT MATCH " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2176 "sqlite3_parser.cpp"
    break;

  case 119:
#line 470 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT REGEXP " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 2182 "sqlite3_parser.cpp"
    break;

  case 120:
#line 474 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2188 "sqlite3_parser.cpp"
    break;

  case 121:
#line 475 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + ", " + yystack_[0].value.as < std::string > (); }
#line 2194 "sqlite3_parser.cpp"
    break;

  case 122:
#line 479 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2200 "sqlite3_parser.cpp"
    break;

  case 123:
#line 480 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + "(DISTINCT " + yystack_[1].value.as < std::string > () + ")"; }
#line 2206 "sqlite3_parser.cpp"
    break;

  case 124:
#line 481 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + "()"; }
#line 2212 "sqlite3_parser.cpp"
    break;

  case 125:
#line 482 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(*)"; }
#line 2218 "sqlite3_parser.cpp"
    break;

  case 126:
#line 486 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " ISNULL"; }
#line 2224 "sqlite3_parser.cpp"
    break;

  case 127:
#line 487 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " NOTNULL"; }
#line 2230 "sqlite3_parser.cpp"
    break;

  case 128:
#line 488 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " NOT NULL"; }
#line 2236 "sqlite3_parser.cpp"
    break;

  case 129:
#line 492 "sqlite3_parser.yy"
                                                                { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2242 "sqlite3_parser.cpp"
    break;

  case 130:
#line 493 "sqlite3_parser.yy"
                                                                { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 2248 "sqlite3_parser.cpp"
    break;

  case 131:
#line 497 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " IN ()"; }
#line 2254 "sqlite3_parser.cpp"
    break;

  case 132:
#line 498 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2260 "sqlite3_parser.cpp"
    break;

  case 133:
#line 499 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2266 "sqlite3_parser.cpp"
    break;

  case 134:
#line 500 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2272 "sqlite3_parser.cpp"
    break;

  case 135:
#line 501 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2278 "sqlite3_parser.cpp"
    break;

  case 136:
#line 502 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 2284 "sqlite3_parser.cpp"
    break;

  case 137:
#line 503 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2290 "sqlite3_parser.cpp"
    break;

  case 138:
#line 504 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2296 "sqlite3_parser.cpp"
    break;

  case 139:
#line 505 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " NOT IN ()"; }
#line 2302 "sqlite3_parser.cpp"
    break;

  case 140:
#line 506 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2308 "sqlite3_parser.cpp"
    break;

  case 141:
#line 507 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 2314 "sqlite3_parser.cpp"
    break;

  case 142:
#line 508 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2320 "sqlite3_parser.cpp"
    break;

  case 143:
#line 509 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2326 "sqlite3_parser.cpp"
    break;

  case 144:
#line 510 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 2332 "sqlite3_parser.cpp"
    break;

  case 145:
#line 511 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[8].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2338 "sqlite3_parser.cpp"
    break;

  case 146:
#line 512 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " NOT IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2344 "sqlite3_parser.cpp"
    break;

  case 147:
#line 516 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "WHEN " + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 2350 "sqlite3_parser.cpp"
    break;

  case 148:
#line 517 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " WHEN" + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 2356 "sqlite3_parser.cpp"
    break;

  case 149:
#line 521 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2362 "sqlite3_parser.cpp"
    break;

  case 150:
#line 522 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " END"; }
#line 2368 "sqlite3_parser.cpp"
    break;

  case 151:
#line 523 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2374 "sqlite3_parser.cpp"
    break;

  case 152:
#line 524 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "CASE " + yystack_[1].value.as < std::string > () + " END"; }
#line 2380 "sqlite3_parser.cpp"
    break;

  case 153:
#line 528 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(IGNORE)"; }
#line 2386 "sqlite3_parser.cpp"
    break;

  case 154:
#line 529 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(ROLLBACK, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2392 "sqlite3_parser.cpp"
    break;

  case 155:
#line 530 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(ABORT, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2398 "sqlite3_parser.cpp"
    break;

  case 156:
#line 531 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "RAISE(FAIL, " + yystack_[1].value.as < std::string > () + ")"; }
#line 2404 "sqlite3_parser.cpp"
    break;

  case 157:
#line 535 "sqlite3_parser.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2410 "sqlite3_parser.cpp"
    break;

  case 158:
#line 536 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2416 "sqlite3_parser.cpp"
    break;

  case 159:
#line 537 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2422 "sqlite3_parser.cpp"
    break;

  case 160:
#line 538 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2428 "sqlite3_parser.cpp"
    break;

  case 161:
#line 539 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2434 "sqlite3_parser.cpp"
    break;

  case 162:
#line 540 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2440 "sqlite3_parser.cpp"
    break;

  case 163:
#line 541 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2446 "sqlite3_parser.cpp"
    break;

  case 164:
#line 542 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2452 "sqlite3_parser.cpp"
    break;

  case 165:
#line 543 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2458 "sqlite3_parser.cpp"
    break;

  case 166:
#line 544 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2464 "sqlite3_parser.cpp"
    break;

  case 167:
#line 545 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "CAST(" + yystack_[3].value.as < std::string > () + " AS " + yystack_[1].value.as < std::string > () + ")"; }
#line 2470 "sqlite3_parser.cpp"
    break;

  case 168:
#line 546 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " COLLATE " + yystack_[0].value.as < std::string > (); }
#line 2476 "sqlite3_parser.cpp"
    break;

  case 169:
#line 547 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2482 "sqlite3_parser.cpp"
    break;

  case 170:
#line 548 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2488 "sqlite3_parser.cpp"
    break;

  case 171:
#line 549 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2494 "sqlite3_parser.cpp"
    break;

  case 172:
#line 550 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2500 "sqlite3_parser.cpp"
    break;

  case 173:
#line 551 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2506 "sqlite3_parser.cpp"
    break;

  case 174:
#line 552 "sqlite3_parser.yy"
          { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2512 "sqlite3_parser.cpp"
    break;

  case 175:
#line 561 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "SELECT"; }
#line 2518 "sqlite3_parser.cpp"
    break;

  case 176:
#line 569 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = false; }
#line 2524 "sqlite3_parser.cpp"
    break;

  case 177:
#line 570 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = true; }
#line 2530 "sqlite3_parser.cpp"
    break;

  case 178:
#line 574 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = ""; }
#line 2536 "sqlite3_parser.cpp"
    break;

  case 179:
#line 575 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "ASC"; }
#line 2542 "sqlite3_parser.cpp"
    break;

  case 180:
#line 576 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = "DESC"; }
#line 2548 "sqlite3_parser.cpp"
    break;

  case 181:
#line 584 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = false; }
#line 2554 "sqlite3_parser.cpp"
    break;

  case 182:
#line 585 "sqlite3_parser.yy"
                                                { yylhs.value.as < bool > () = true; }
#line 2560 "sqlite3_parser.cpp"
    break;

  case 183:
#line 589 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = ""; }
#line 2566 "sqlite3_parser.cpp"
    break;

  case 184:
#line 590 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2572 "sqlite3_parser.cpp"
    break;

  case 185:
#line 594 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2578 "sqlite3_parser.cpp"
    break;

  case 186:
#line 595 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2584 "sqlite3_parser.cpp"
    break;

  case 187:
#line 596 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2590 "sqlite3_parser.cpp"
    break;

  case 188:
#line 600 "sqlite3_parser.yy"
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
#line 2606 "sqlite3_parser.cpp"
    break;

  case 189:
#line 614 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::IndexedColumnVector > () = sqlb::IndexedColumnVector(1, yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2612 "sqlite3_parser.cpp"
    break;

  case 190:
#line 615 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::IndexedColumnVector > () = yystack_[2].value.as < sqlb::IndexedColumnVector > (); yylhs.value.as < sqlb::IndexedColumnVector > ().push_back(yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2618 "sqlite3_parser.cpp"
    break;

  case 191:
#line 619 "sqlite3_parser.yy"
                                                                                                                                                                {
													yylhs.value.as < sqlb::IndexPtr > () = sqlb::IndexPtr(new sqlb::Index(yystack_[6].value.as < std::string > ()));
													yylhs.value.as < sqlb::IndexPtr > ()->setTable(yystack_[4].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setUnique(yystack_[9].value.as < bool > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setWhereExpr(yystack_[0].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->fields = yystack_[2].value.as < sqlb::IndexedColumnVector > ();
													yylhs.value.as < sqlb::IndexPtr > ()->setFullyParsed(true);
												}
#line 2631 "sqlite3_parser.cpp"
    break;

  case 192:
#line 634 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = {}; }
#line 2637 "sqlite3_parser.cpp"
    break;

  case 193:
#line 635 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = {}; }
#line 2643 "sqlite3_parser.cpp"
    break;

  case 194:
#line 636 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 2649 "sqlite3_parser.cpp"
    break;

  case 195:
#line 640 "sqlite3_parser.yy"
                                                                                                                                {
													yylhs.value.as < sqlb::TablePtr > () = sqlb::TablePtr(new sqlb::Table(yystack_[3].value.as < std::string > ()));
													yylhs.value.as < sqlb::TablePtr > ()->setVirtualUsing(yystack_[1].value.as < std::string > ());
													yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
												}
#line 2659 "sqlite3_parser.cpp"
    break;

  case 196:
#line 652 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = false; }
#line 2665 "sqlite3_parser.cpp"
    break;

  case 197:
#line 653 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2671 "sqlite3_parser.cpp"
    break;

  case 198:
#line 654 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2677 "sqlite3_parser.cpp"
    break;

  case 199:
#line 658 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = false; }
#line 2683 "sqlite3_parser.cpp"
    break;

  case 200:
#line 659 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2689 "sqlite3_parser.cpp"
    break;

  case 201:
#line 663 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = ""; }
#line 2695 "sqlite3_parser.cpp"
    break;

  case 202:
#line 664 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2701 "sqlite3_parser.cpp"
    break;

  case 203:
#line 665 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2707 "sqlite3_parser.cpp"
    break;

  case 204:
#line 666 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2713 "sqlite3_parser.cpp"
    break;

  case 205:
#line 667 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2719 "sqlite3_parser.cpp"
    break;

  case 206:
#line 668 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2725 "sqlite3_parser.cpp"
    break;

  case 207:
#line 672 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = ""; }
#line 2731 "sqlite3_parser.cpp"
    break;

  case 208:
#line 673 "sqlite3_parser.yy"
                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2737 "sqlite3_parser.cpp"
    break;

  case 209:
#line 677 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "VIRTUAL"; }
#line 2743 "sqlite3_parser.cpp"
    break;

  case 210:
#line 678 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "STORED"; }
#line 2749 "sqlite3_parser.cpp"
    break;

  case 211:
#line 679 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = "VIRTUAL"; }
#line 2755 "sqlite3_parser.cpp"
    break;

  case 212:
#line 683 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = false; }
#line 2761 "sqlite3_parser.cpp"
    break;

  case 213:
#line 684 "sqlite3_parser.yy"
                                                        { yylhs.value.as < bool > () = true; }
#line 2767 "sqlite3_parser.cpp"
    break;

  case 214:
#line 688 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::PrimaryKey;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = true;
												sqlb::PrimaryKeyConstraint* pk = new sqlb::PrimaryKeyConstraint({sqlb::IndexedColumn("", false, yystack_[1].value.as < std::string > ())});
												pk->setName(yystack_[4].value.as < std::string > ());
												pk->setConflictAction(yystack_[0].value.as < std::string > ());
												yylhs.value.as < ColumnConstraintInfo > ().table_constraint = sqlb::ConstraintPtr(pk);
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = true;
											}
#line 2781 "sqlite3_parser.cpp"
    break;

  case 215:
#line 697 "sqlite3_parser.yy"
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
#line 2796 "sqlite3_parser.cpp"
    break;

  case 216:
#line 707 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::NotNull;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[3].value.as < std::string > () == "" && yystack_[0].value.as < std::string > () == "");
											}
#line 2806 "sqlite3_parser.cpp"
    break;

  case 217:
#line 712 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::None;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = true;
											}
#line 2816 "sqlite3_parser.cpp"
    break;

  case 218:
#line 717 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Unique;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "" && yystack_[0].value.as < std::string > () == "");
											}
#line 2826 "sqlite3_parser.cpp"
    break;

  case 219:
#line 722 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Check;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[1].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[4].value.as < std::string > () == "");
											}
#line 2837 "sqlite3_parser.cpp"
    break;

  case 220:
#line 728 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Default;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[0].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "");
											}
#line 2848 "sqlite3_parser.cpp"
    break;

  case 221:
#line 734 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Default;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[0].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "");
											}
#line 2859 "sqlite3_parser.cpp"
    break;

  case 222:
#line 740 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Default;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[0].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "");
											}
#line 2870 "sqlite3_parser.cpp"
    break;

  case 223:
#line 746 "sqlite3_parser.yy"
                                                                                        {	// We must allow the same keywords as unquoted default values as in the columnid context.
												// But we do not use columnid here in order to avoid reduce/reduce conflicts.
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Default;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[0].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "");
											}
#line 2882 "sqlite3_parser.cpp"
    break;

  case 224:
#line 753 "sqlite3_parser.yy"
                                                                                        {	// Same as above.
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Default;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[0].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "");
											}
#line 2893 "sqlite3_parser.cpp"
    break;

  case 225:
#line 759 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Default;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = "(" + yystack_[1].value.as < std::string > () + ")";
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[4].value.as < std::string > () == "");
											}
#line 2904 "sqlite3_parser.cpp"
    break;

  case 226:
#line 765 "sqlite3_parser.yy"
                                                                                        {
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Collate;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().text = yystack_[0].value.as < std::string > ();
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = (yystack_[2].value.as < std::string > () == "");
											}
#line 2915 "sqlite3_parser.cpp"
    break;

  case 227:
#line 771 "sqlite3_parser.yy"
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
#line 2931 "sqlite3_parser.cpp"
    break;

  case 228:
#line 782 "sqlite3_parser.yy"
                                                                                                        {		// TODO Solve shift/reduce conflict.
												yylhs.value.as < ColumnConstraintInfo > ().type = ColumnConstraintInfo::Generated;
												yylhs.value.as < ColumnConstraintInfo > ().is_table_constraint = false;
												yylhs.value.as < ColumnConstraintInfo > ().generated_constraint.setExpression(yystack_[2].value.as < std::string > ());
												yylhs.value.as < ColumnConstraintInfo > ().generated_constraint.setStorage(yystack_[0].value.as < std::string > ());
												yylhs.value.as < ColumnConstraintInfo > ().generated_constraint.setName(yystack_[6].value.as < std::string > ());
												yylhs.value.as < ColumnConstraintInfo > ().fully_parsed = true;
											}
#line 2944 "sqlite3_parser.cpp"
    break;

  case 229:
#line 793 "sqlite3_parser.yy"
                                                        { yylhs.value.as < ColumnConstraintInfoVector > () = { yystack_[0].value.as < ColumnConstraintInfo > () }; }
#line 2950 "sqlite3_parser.cpp"
    break;

  case 230:
#line 794 "sqlite3_parser.yy"
                                                        { yylhs.value.as < ColumnConstraintInfoVector > () = yystack_[1].value.as < ColumnConstraintInfoVector > (); yylhs.value.as < ColumnConstraintInfoVector > ().push_back(yystack_[0].value.as < ColumnConstraintInfo > ()); }
#line 2956 "sqlite3_parser.cpp"
    break;

  case 231:
#line 798 "sqlite3_parser.yy"
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
										} else if(c.type == ColumnConstraintInfo::Generated) {
											f.setGenerated(c.generated_constraint);

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
										} else {
											fully_parsed = false;
										}
									}
								}

								yylhs.value.as < ColumndefData > () = std::make_tuple(f, table_constraints, fully_parsed);
							}
#line 3013 "sqlite3_parser.cpp"
    break;

  case 232:
#line 850 "sqlite3_parser.yy"
                                                        { yylhs.value.as < ColumndefData > () = std::make_tuple(sqlb::Field(yystack_[1].value.as < std::string > (), yystack_[0].value.as < std::string > ()), sqlb::ConstraintSet{}, true); }
#line 3019 "sqlite3_parser.cpp"
    break;

  case 233:
#line 854 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::vector<ColumndefData> > () = {yystack_[0].value.as < ColumndefData > ()}; }
#line 3025 "sqlite3_parser.cpp"
    break;

  case 234:
#line 855 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::vector<ColumndefData> > () = yystack_[2].value.as < std::vector<ColumndefData> > (); yylhs.value.as < std::vector<ColumndefData> > ().push_back(yystack_[0].value.as < ColumndefData > ()); }
#line 3031 "sqlite3_parser.cpp"
    break;

  case 235:
#line 859 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = ""; }
#line 3037 "sqlite3_parser.cpp"
    break;

  case 236:
#line 860 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3043 "sqlite3_parser.cpp"
    break;

  case 237:
#line 864 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = sqlb::StringVector(1, yystack_[0].value.as < std::string > ()); }
#line 3049 "sqlite3_parser.cpp"
    break;

  case 238:
#line 865 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = yystack_[2].value.as < sqlb::StringVector > (); yylhs.value.as < sqlb::StringVector > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 3055 "sqlite3_parser.cpp"
    break;

  case 239:
#line 869 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = sqlb::StringVector(); }
#line 3061 "sqlite3_parser.cpp"
    break;

  case 240:
#line 870 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::StringVector > () = yystack_[1].value.as < sqlb::StringVector > (); }
#line 3067 "sqlite3_parser.cpp"
    break;

  case 241:
#line 874 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3073 "sqlite3_parser.cpp"
    break;

  case 242:
#line 875 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3079 "sqlite3_parser.cpp"
    break;

  case 243:
#line 876 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3085 "sqlite3_parser.cpp"
    break;

  case 244:
#line 877 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3091 "sqlite3_parser.cpp"
    break;

  case 245:
#line 878 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3097 "sqlite3_parser.cpp"
    break;

  case 246:
#line 879 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3103 "sqlite3_parser.cpp"
    break;

  case 247:
#line 880 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3109 "sqlite3_parser.cpp"
    break;

  case 248:
#line 881 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3115 "sqlite3_parser.cpp"
    break;

  case 249:
#line 882 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3121 "sqlite3_parser.cpp"
    break;

  case 250:
#line 883 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3127 "sqlite3_parser.cpp"
    break;

  case 251:
#line 884 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3133 "sqlite3_parser.cpp"
    break;

  case 252:
#line 885 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3139 "sqlite3_parser.cpp"
    break;

  case 253:
#line 886 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3145 "sqlite3_parser.cpp"
    break;

  case 254:
#line 887 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3151 "sqlite3_parser.cpp"
    break;

  case 255:
#line 888 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3157 "sqlite3_parser.cpp"
    break;

  case 256:
#line 889 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3163 "sqlite3_parser.cpp"
    break;

  case 257:
#line 893 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3169 "sqlite3_parser.cpp"
    break;

  case 258:
#line 894 "sqlite3_parser.yy"
                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3175 "sqlite3_parser.cpp"
    break;

  case 259:
#line 898 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = ""; }
#line 3181 "sqlite3_parser.cpp"
    break;

  case 260:
#line 899 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3187 "sqlite3_parser.cpp"
    break;

  case 261:
#line 900 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3193 "sqlite3_parser.cpp"
    break;

  case 262:
#line 901 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3199 "sqlite3_parser.cpp"
    break;

  case 263:
#line 902 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3205 "sqlite3_parser.cpp"
    break;

  case 264:
#line 903 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3211 "sqlite3_parser.cpp"
    break;

  case 265:
#line 904 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3217 "sqlite3_parser.cpp"
    break;

  case 266:
#line 905 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3223 "sqlite3_parser.cpp"
    break;

  case 267:
#line 906 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3229 "sqlite3_parser.cpp"
    break;

  case 268:
#line 907 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3235 "sqlite3_parser.cpp"
    break;

  case 269:
#line 908 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 3241 "sqlite3_parser.cpp"
    break;

  case 270:
#line 909 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3247 "sqlite3_parser.cpp"
    break;

  case 271:
#line 910 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3253 "sqlite3_parser.cpp"
    break;

  case 272:
#line 911 "sqlite3_parser.yy"
                                                                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 3259 "sqlite3_parser.cpp"
    break;

  case 273:
#line 915 "sqlite3_parser.yy"
                                                                                                        {
														sqlb::PrimaryKeyConstraint* pk = new sqlb::PrimaryKeyConstraint(yystack_[2].value.as < sqlb::IndexedColumnVector > ());
														pk->setName(yystack_[6].value.as < std::string > ());
														pk->setConflictAction(yystack_[0].value.as < std::string > ());
														yylhs.value.as < sqlb::ConstraintPtr > () = sqlb::ConstraintPtr(pk);
													}
#line 3270 "sqlite3_parser.cpp"
    break;

  case 274:
#line 921 "sqlite3_parser.yy"
                                                                                                                {
														sqlb::PrimaryKeyConstraint* pk = new sqlb::PrimaryKeyConstraint(yystack_[3].value.as < sqlb::IndexedColumnVector > ());
														pk->setName(yystack_[7].value.as < std::string > ());
														pk->setConflictAction(yystack_[0].value.as < std::string > ());
														pk->setAutoIncrement(true);
														yylhs.value.as < sqlb::ConstraintPtr > () = sqlb::ConstraintPtr(pk);
													}
#line 3282 "sqlite3_parser.cpp"
    break;

  case 275:
#line 928 "sqlite3_parser.yy"
                                                                                                        {
														sqlb::UniqueConstraint* u = new sqlb::UniqueConstraint(yystack_[2].value.as < sqlb::IndexedColumnVector > ());
														u->setName(yystack_[5].value.as < std::string > ());
														u->setConflictAction(yystack_[0].value.as < std::string > ());
														yylhs.value.as < sqlb::ConstraintPtr > () = sqlb::ConstraintPtr(u);
													}
#line 3293 "sqlite3_parser.cpp"
    break;

  case 276:
#line 934 "sqlite3_parser.yy"
                                                                                                        {
														yylhs.value.as < sqlb::ConstraintPtr > () = sqlb::ConstraintPtr(new sqlb::CheckConstraint(yystack_[1].value.as < std::string > ()));
														yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[4].value.as < std::string > ());
													}
#line 3302 "sqlite3_parser.cpp"
    break;

  case 277:
#line 938 "sqlite3_parser.yy"
                                                                                                                                                {
														yylhs.value.as < sqlb::ConstraintPtr > () = sqlb::ConstraintPtr(new sqlb::ForeignKeyClause(yystack_[2].value.as < std::string > (), yystack_[1].value.as < sqlb::StringVector > (), yystack_[0].value.as < std::string > ()));
														yylhs.value.as < sqlb::ConstraintPtr > ()->setColumnList(yystack_[5].value.as < sqlb::StringVector > ());
														yylhs.value.as < sqlb::ConstraintPtr > ()->setName(yystack_[9].value.as < std::string > ());
													}
#line 3312 "sqlite3_parser.cpp"
    break;

  case 278:
#line 946 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintSet > () = {yystack_[0].value.as < sqlb::ConstraintPtr > ()}; }
#line 3318 "sqlite3_parser.cpp"
    break;

  case 279:
#line 947 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintSet > () = yystack_[2].value.as < sqlb::ConstraintSet > (); yylhs.value.as < sqlb::ConstraintSet > ().insert(yystack_[0].value.as < sqlb::ConstraintPtr > ()); }
#line 3324 "sqlite3_parser.cpp"
    break;

  case 280:
#line 948 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintSet > () = yystack_[1].value.as < sqlb::ConstraintSet > (); yylhs.value.as < sqlb::ConstraintSet > ().insert(yystack_[0].value.as < sqlb::ConstraintPtr > ()); }
#line 3330 "sqlite3_parser.cpp"
    break;

  case 281:
#line 952 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintSet > () = {}; }
#line 3336 "sqlite3_parser.cpp"
    break;

  case 282:
#line 953 "sqlite3_parser.yy"
                                                        { yylhs.value.as < sqlb::ConstraintSet > () = yystack_[0].value.as < sqlb::ConstraintSet > (); }
#line 3342 "sqlite3_parser.cpp"
    break;

  case 283:
#line 957 "sqlite3_parser.yy"
                                                                                                                        {
										yylhs.value.as < sqlb::TablePtr > () = sqlb::TablePtr(new sqlb::Table(yystack_[2].value.as < std::string > ()));
										yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
									}
#line 3351 "sqlite3_parser.cpp"
    break;

  case 284:
#line 961 "sqlite3_parser.yy"
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
#line 3376 "sqlite3_parser.cpp"
    break;


#line 3380 "sqlite3_parser.cpp"

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
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
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
    std::ptrdiff_t yycount = 0;
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
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */
    if (!yyla.empty ())
      {
        symbol_number_type yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];

        int yyn = yypact_[+yystate];
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
              if (yycheck_[yyx + yyn] == yyx && yyx != yy_error_token_
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
    std::ptrdiff_t yyi = 0;
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


  const short parser::yypact_ninf_ = -340;

  const short parser::yytable_ninf_ = -283;

  const short
  parser::yypact_[] =
  {
      -8,   189,    39,    91,  -340,  -340,  -340,  -340,  -340,  -340,
      17,    53,    43,  -340,  -340,    73,    73,    73,    30,  2227,
    2227,  2227,   108,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
     169,  -340,  -340,  -340,  -340,  -340,  -340,  -340,   174,  -340,
    -340,    83,   112,    22,  -340,  2315,  2315,   -78,  2315,  2139,
     103,  -340,  -340,  -340,  -340,   209,   229,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  2315,  -340,   212,  -340,  -340,   951,
    -340,  1407,  -340,  1613,  -340,    90,  2051,   235,  1407,  -340,
    1407,  1407,  1407,  1065,   250,  -340,  -340,  -340,  -340,  1407,
    -340,   274,  -340,  -340,  -340,  -340,  -340,  -340,    37,  -340,
    -340,  -340,  -340,   165,  -340,  -340,  -340,  -340,  -340,  -340,
    3208,  2746,  -340,   171,     5,  -340,   -78,  -340,   106,    -9,
    -340,   -18,  -340,   116,   134,   176,  -340,  -340,  -340,  1407,
     -35,   482,  1407,  3318,     7,   609,   -78,  -340,  1407,  1407,
    1407,  1407,  1407,  1407,  1407,  1407,  1407,  1407,  1407,  1407,
    1407,  1407,  1407,  1407,  1407,  1407,  1407,  1407,  1407,   -78,
    1407,  1705,  1407,  -340,  1407,  1407,    56,  -340,  1407,  1407,
    -340,  -340,  -340,   175,  1407,   179,   182,  -340,  -340,   193,
    -340,  -340,   280,   -78,  1521,   273,   228,  -340,   236,  2315,
     230,   276,   308,   239,   243,   315,   278,  -340,   237,  -340,
    -340,  1904,  1407,  -340,  1407,   -31,  2823,   319,   334,   337,
     338,  -340,   339,  1407,   231,   341,  3208,   232,   232,     4,
       4,   177,     4,   177,   307,   313,   313,   215,   215,   215,
     215,   313,   313,   177,   177,  3318,  2900,  -340,   141,   723,
     233,  -340,   313,   194,   260,  1407,  1407,  1797,  1407,  1407,
    -340,  1407,  3285,   454,  1407,  -340,  -340,  -340,  -340,  -340,
       5,  1407,  -340,  1407,  -340,  -340,  -340,  -340,  -340,  -340,
     230,    -1,   345,   309,  -340,   346,  1407,   348,   349,  1407,
    -340,  -340,  1407,  2977,  1993,  1407,  -340,  2315,   242,   244,
    -340,   245,  -340,   241,  -340,   -78,  1407,  1407,  -340,   246,
     350,  1407,  2315,  1407,  1407,  3054,  1932,   837,   251,  -340,
    2021,  2451,  2533,  1407,  3208,   355,  2423,  2505,  -340,   230,
    2139,    49,   -10,  1407,  2587,  2139,  1407,   253,  3208,  -340,
    1407,  3131,   356,   357,   358,   359,  -340,  -340,   313,   313,
    -340,  -340,   256,   366,  -340,   313,   313,  1407,  1407,  -340,
     257,   367,  1407,  2315,  1407,  1407,  1407,   313,  -340,  -340,
    -340,   343,  -340,   261,   303,   -78,   325,     1,  -340,    63,
    -340,  -340,  -340,  -340,  -340,  -340,  2669,  -340,   286,   110,
     230,  3208,  -340,  -340,  -340,  -340,  -340,  -340,  1179,   313,
     313,  -340,  -340,   291,   372,  -340,   313,   313,   313,  -340,
    -340,  2139,   -25,  -340,   310,     9,    12,    13,   311,   330,
    -340,    21,   293,   230,   378,  -340,  -340,   295,  -340,  1293,
    -340,  -340,  -340,   -20,  -340,   360,  -340,    54,  -340,   361,
    -340,    55,  -340,   362,  -340,    89,    93,   314,  -340,  -340,
    -340,  2315,  -340,   230,  -340,  -340,   299,  -340,  -340,  -340,
    -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,  -340,
     124,   345,  -340,  -340,  -340,  -340,    49,  -340
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
      67,    69,    68,    63,   207,   233,   281,   175,   283,     0,
     195,     0,    74,    76,   208,   235,   235,     0,     0,   193,
       0,     0,     0,     0,    23,    14,    13,    15,    12,     0,
      10,    43,    11,     7,     8,     9,   159,   157,   162,   158,
     163,   164,   169,     0,   165,   170,   171,   172,   173,   174,
     120,   178,   189,     0,     0,    75,     0,   229,   235,   212,
     234,     0,   278,   235,   199,     0,    80,    79,    81,     0,
       0,     0,     0,    82,     0,     0,     0,   194,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   126,     0,     0,     0,   127,     0,     0,
     179,   180,   188,   183,     0,     0,     0,    73,    72,     0,
     236,   230,     0,     0,     0,     0,     0,   217,     0,     0,
     201,     0,     0,     0,     0,     0,   235,   280,     0,   284,
     166,     0,     0,   152,     0,     0,     0,     0,     0,     0,
       0,   124,     0,     0,     0,   161,   121,    87,    88,    84,
      85,    91,    86,    92,    83,    97,    98,    95,    96,    93,
      94,    99,   100,    89,    90,   102,     0,   168,   105,     0,
      61,   135,   101,   104,   106,     0,     0,     0,     0,     0,
     128,     0,   103,   107,     0,   191,   190,    70,    71,    77,
       0,     0,   226,     0,   224,   221,   222,   223,   220,   213,
     201,   178,   239,     0,   218,     0,     0,     0,     0,     0,
     279,   200,     0,     0,     0,     0,   150,     0,     0,     0,
     153,     0,   125,     0,   122,     0,     0,     0,   131,     0,
       0,     0,     0,     0,     0,     0,   109,     0,    61,   143,
     108,   110,   111,     0,   184,     0,     0,     0,   216,   201,
       0,   259,     0,     0,     0,     0,     0,     0,   147,   151,
       0,     0,     0,     0,     0,     0,   123,   160,   129,   113,
     133,   132,     0,    61,   134,   112,   114,     0,     0,   139,
       0,     0,     0,     0,     0,     0,     0,   115,    78,   219,
     225,   214,   237,     0,   269,     0,     0,     0,   257,   260,
     227,   203,   204,   205,   206,   202,     0,   276,     0,     0,
     201,   148,   149,   167,   155,   156,   154,   138,     0,   130,
     117,   141,   140,     0,    61,   142,   116,   118,   119,   215,
     240,     0,     0,   256,   272,     0,     0,     0,   263,     0,
     258,   209,     0,   201,     0,   275,   136,     0,   146,     0,
     238,   267,   268,     0,   243,     0,   244,     0,   253,     0,
     254,     0,   248,     0,   249,     0,     0,   266,   210,   211,
     228,     0,   273,   201,   137,   144,     0,   270,   271,   245,
     242,   241,   255,   252,   251,   250,   247,   246,   261,   262,
       0,   239,   274,   145,   264,   265,   259,   277
  };

  const short
  parser::yypgoto_[] =
  {
    -340,  -340,  -340,   170,   -19,   -13,   -66,  -339,   172,    95,
    -340,    70,  -340,  -340,  -340,  -104,  -340,  -340,  -340,  -340,
     234,  -340,  -340,   255,  -254,   167,    92,  -340,  -340,   201,
     187,  -282,  -340,  -340,  -340,  -340,  -340,  -297,  -340,  -340,
    -340,   248,  -340,   288,  -340,   -79,    42,   -99,     0,  -340,
     -98,  -148,  -340,  -340,  -340
  };

  const short
  parser::yydefgoto_[] =
  {
      -1,     2,     3,   127,   128,   129,    70,    94,   208,   209,
     103,   104,   130,   131,   132,   133,   134,   135,   136,   137,
     160,   138,   139,   140,    98,    19,   202,    11,   285,    71,
     142,   143,     4,   100,     5,    12,   229,   304,   105,   470,
     221,   147,   148,    95,    96,   151,   393,   351,   398,   399,
     400,   152,   153,   107,     6
  };

  const short
  parser::yytable_[] =
  {
      68,    68,    68,   348,   155,   227,    69,    69,    69,    83,
      84,   392,    86,   205,   206,   330,   392,   401,   232,   233,
     176,   222,   315,   316,   451,    79,   149,   357,   102,   477,
     212,   213,   200,    65,   237,     1,    67,   145,   214,    13,
     165,   452,   166,   223,   189,   454,   478,   402,   458,   462,
     201,   435,   391,   215,    80,   403,    82,    82,    85,    82,
      92,   244,    69,    69,   238,    69,    93,   224,   216,   149,
     217,   436,   239,   234,   409,    82,   218,   234,   310,   219,
     404,    69,   405,   381,    82,   455,   225,    92,   459,   463,
      69,   275,   450,    93,  -232,   220,  -232,   394,    14,   240,
     456,   480,   483,   460,   464,   457,   437,    22,   461,   465,
    -231,   438,  -231,   445,   443,    15,   204,   207,   468,   276,
    -282,    16,   226,   277,   395,   271,   396,   210,   469,   397,
     278,   279,   146,   481,   484,   280,   486,    18,   395,   323,
     439,    17,   488,   397,   444,   281,   472,   245,   146,   169,
     170,   171,   172,   302,   173,   174,   175,   176,   146,   489,
     179,   180,   181,   182,    74,   329,   185,   186,   487,   167,
     267,   168,   270,   494,    75,   203,   492,   204,    69,    76,
     230,   189,   168,    20,    21,   169,   170,   171,   172,    77,
     495,   174,    78,   176,   292,   296,   327,   289,    97,   290,
      82,   297,   169,   170,   171,   172,    69,   173,   174,   175,
     176,   339,    99,   179,   180,   181,   182,   189,   106,   185,
     186,    72,    73,   169,   170,   171,   172,   372,   173,   174,
     175,   176,   101,   380,   189,   324,   331,   168,   332,   154,
     185,   186,   171,   172,   228,   366,   174,   168,   176,   333,
     370,   102,   168,   162,   382,   189,   383,   410,   338,   204,
     417,   421,   168,   168,    69,   430,   374,   431,   169,   170,
     171,   172,   189,   173,   174,   175,   176,   164,   423,   179,
     180,   181,   182,   291,   284,   185,   186,  -196,     7,     8,
     442,   287,   431,     9,   288,   448,    10,   168,    82,   474,
     189,   168,   299,   493,    69,   168,   367,   300,   305,   301,
     303,   306,   307,   373,   447,   334,   308,   425,   309,    69,
     146,   169,   170,   171,   172,   318,   173,   174,   175,   176,
     311,    92,   179,   180,   181,   182,    92,    93,   185,   186,
     319,   320,    93,   322,   321,   476,   325,   189,   350,   353,
     352,   355,   356,   189,   371,   363,   141,   364,   365,   388,
     413,   414,   415,   416,   424,   156,   157,   158,   161,   418,
      69,   422,   432,   434,   163,   449,   433,   429,   467,   453,
     466,   471,   473,   490,   295,   345,   298,   362,   479,   482,
     485,   286,   496,   349,   150,   235,   211,   408,   497,   440,
       0,     0,     0,     0,     0,   491,     0,     0,     0,     0,
       0,     0,    92,     0,   231,     0,     0,   236,    93,     0,
       0,     0,     0,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,     0,   268,     0,   272,     0,   273,
     274,     0,    82,   282,   283,     0,     0,     0,    69,   141,
       0,     0,   169,   170,   171,   172,     0,   173,   174,   175,
     176,     0,     0,   179,   180,   181,   182,     0,     0,   185,
     186,     0,     0,     0,     0,     0,     0,   313,     0,   314,
     169,   170,   171,   172,   189,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   343,
       0,     0,   187,     0,     0,     0,     0,   188,     0,     0,
       0,     0,   189,     0,     0,     0,     0,     0,     0,     0,
     335,   336,     0,   340,   341,     0,   342,     0,     0,   344,
       0,     0,     0,     0,     0,   190,   346,     0,   347,   191,
       0,     0,     0,   192,   193,     0,   194,   195,     0,   196,
     197,   354,     0,   198,   141,     0,     0,   358,     0,     0,
     361,   199,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   368,   369,     0,     0,     0,     0,     0,   375,   376,
     159,     0,     0,     0,     0,     0,     0,     0,   387,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   406,     0,
       0,   141,   108,   241,     0,   411,     0,   110,   111,   242,
       0,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   419,   420,     0,     0,    23,    24,    25,   426,
     427,   428,    26,     0,     0,    27,   113,   114,     0,     0,
      29,     0,     0,   115,   116,   117,     0,     0,    33,     0,
      34,   243,     0,    35,     0,     0,    36,   118,    37,    38,
       0,    39,    40,     0,    41,    42,     0,     0,    43,     0,
       0,     0,    44,    45,    46,    47,   119,     0,   120,     0,
       0,    48,    49,    50,     0,   121,    52,     0,    53,    54,
      55,    56,    57,    58,     0,     0,    59,     0,    81,    61,
       0,   122,    62,     0,     0,     0,    63,     0,     0,    64,
      65,   123,   124,    67,   125,   126,   108,   328,     0,     0,
       0,   110,   111,     0,     0,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,    24,    25,     0,     0,     0,    26,     0,     0,    27,
     113,   114,     0,     0,    29,     0,     0,   115,   116,   117,
       0,     0,    33,     0,    34,     0,     0,    35,     0,     0,
      36,   118,    37,    38,     0,    39,    40,     0,    41,    42,
       0,     0,    43,     0,     0,     0,    44,    45,    46,    47,
     119,     0,   120,     0,     0,    48,    49,    50,     0,   121,
      52,     0,    53,    54,    55,    56,    57,    58,    97,     0,
      59,     0,    81,    61,     0,   122,    62,     0,     0,     0,
      63,     0,     0,    64,    65,   123,   124,    67,   125,   126,
     108,   379,     0,     0,     0,   110,   111,     0,     0,   112,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,    24,    25,     0,     0,     0,
      26,     0,     0,    27,   113,   114,     0,     0,    29,     0,
       0,   115,   116,   117,     0,     0,    33,     0,    34,     0,
       0,    35,     0,     0,    36,   118,    37,    38,     0,    39,
      40,     0,    41,    42,     0,     0,    43,     0,     0,     0,
      44,    45,    46,    47,   119,     0,   120,     0,     0,    48,
      49,    50,     0,   121,    52,     0,    53,    54,    55,    56,
      57,    58,    97,     0,    59,     0,    81,    61,     0,   122,
      62,     0,     0,     0,    63,     0,     0,    64,    65,   123,
     124,    67,   125,   126,   108,   109,     0,     0,     0,   110,
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
       0,    64,    65,   123,   124,    67,   125,   126,   108,     0,
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
       0,     0,    63,   159,     0,    64,    65,   123,   124,    67,
     125,   126,   108,   446,     0,     0,     0,   110,   111,     0,
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
      65,   123,   124,    67,   125,   126,   108,   475,     0,     0,
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
      62,     0,     0,     0,    63,     0,     0,    64,    65,   123,
     124,    67,   125,   126,   293,     0,     0,     0,     0,   205,
     206,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    23,    24,
      25,     0,     0,     0,    26,     0,     0,    27,     0,    28,
       0,     0,    29,     0,     0,   115,   116,   117,     0,     0,
      33,     0,    34,     0,     0,    35,     0,     0,    36,   118,
      37,    38,     0,    39,    40,   294,    41,    42,     0,     0,
      43,     0,     0,     0,    44,    45,    46,    47,     0,     0,
     120,     0,     0,    48,    49,    50,     0,    51,    52,     0,
      53,    54,    55,    56,    57,    58,   144,     0,    59,     0,
      81,    61,     0,   122,    62,     0,     0,     0,    63,     0,
       0,    64,    65,   123,   124,    67,   125,     0,     0,     0,
      23,    24,    25,     0,     0,     0,    26,     0,     0,    27,
       0,    28,     0,     0,    29,     0,     0,    30,    31,    32,
       0,     0,    33,     0,    34,     0,     0,    35,     0,     0,
      36,     0,    37,    38,     0,    39,    40,     0,    41,    42,
       0,     0,    43,     0,     0,     0,    44,    45,    46,    47,
       0,     0,     0,     0,     0,    48,    49,    50,     0,    51,
      52,     0,    53,    54,    55,    56,    57,    58,   269,     0,
      59,     0,    81,    61,     0,     0,    62,     0,     0,     0,
      63,     0,     0,    64,    65,     0,    66,    67,     0,     0,
       0,     0,    23,    24,    25,     0,     0,     0,    26,     0,
       0,    27,     0,    28,     0,     0,    29,     0,     0,    30,
      31,    32,     0,     0,    33,     0,    34,     0,     0,    35,
       0,     0,    36,     0,    37,    38,     0,    39,    40,     0,
      41,    42,     0,     0,    43,     0,     0,     0,    44,    45,
      46,    47,     0,     0,     0,     0,     0,    48,    49,    50,
       0,    51,    52,     0,    53,    54,    55,    56,    57,    58,
     337,     0,    59,     0,    81,    61,     0,     0,    62,     0,
       0,     0,    63,     0,     0,    64,    65,     0,    66,    67,
       0,     0,     0,     0,    23,    24,    25,     0,     0,     0,
      26,     0,     0,    27,     0,    28,     0,     0,    29,     0,
       0,    30,    31,    32,     0,     0,    33,     0,    34,     0,
       0,    35,     0,     0,    36,     0,    37,    38,     0,    39,
      40,     0,    41,    42,     0,     0,    43,     0,     0,     0,
      44,    45,    46,    47,     0,     0,     0,     0,     0,    48,
      49,    50,     0,    51,    52,     0,    53,    54,    55,    56,
      57,    58,     0,     0,    59,     0,    81,    61,     0,     0,
      62,     0,     0,     0,    63,     0,     0,    64,    65,     0,
      66,    67,   169,   170,   171,   172,     0,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,     0,     0,     0,   187,     0,     0,     0,     0,   188,
     169,   170,   171,   172,   189,   173,   174,   175,   176,     0,
       0,   179,   180,   181,   182,     0,     0,   185,   186,     0,
       0,     0,     0,     0,     0,     0,     0,   190,     0,     0,
       0,   191,   189,     0,     0,   192,   193,     0,   194,   195,
       0,   196,   197,     0,     0,   198,     0,   378,     0,     0,
       0,     0,     0,   199,     0,     0,     0,     0,     0,     0,
       0,   169,   170,   171,   172,   312,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
       0,     0,     0,   187,     0,     0,     0,     0,   188,   169,
     170,   171,   172,   189,   173,   174,   175,   176,     0,     0,
     179,   180,   181,   182,     0,     0,   185,   186,     0,     0,
       0,     0,     0,     0,     0,     0,   190,     0,     0,     0,
     191,   189,     0,     0,   192,   193,     0,   194,   195,     0,
     196,   197,     0,     0,   198,     0,   384,     0,    23,    24,
      25,     0,   199,     0,    26,     0,     0,    27,     0,    28,
       0,     0,    29,   146,   360,    87,    88,    89,     0,     0,
      33,     0,    34,     0,     0,    35,     0,     0,    36,     0,
      37,    38,     0,    39,    40,    90,    41,    42,     0,     0,
      43,     0,     0,     0,    44,    45,    46,    47,     0,     0,
       0,     0,     0,    48,    49,    50,     0,    51,    52,     0,
      53,    54,    55,    56,    57,    58,     0,     0,    59,     0,
      81,    61,     0,     0,    62,     0,     0,     0,    63,     0,
       0,    64,    65,     0,    91,    67,    23,    24,    25,     0,
       0,     0,    26,     0,     0,    27,     0,    28,     0,     0,
      29,     0,     0,    87,    88,    89,     0,     0,    33,     0,
      34,     0,     0,    35,     0,     0,    36,     0,    37,    38,
       0,    39,    40,    90,    41,    42,     0,     0,    43,     0,
       0,     0,    44,    45,    46,    47,     0,     0,     0,     0,
       0,    48,    49,    50,     0,    51,    52,     0,    53,    54,
      55,    56,    57,    58,     0,     0,    59,     0,    81,    61,
       0,     0,    62,     0,     0,     0,    63,     0,     0,    64,
      65,     0,    91,    67,    23,    24,    25,     0,     0,     0,
      26,     0,     0,    27,     0,    28,     0,     0,    29,     0,
       0,    30,    31,    32,     0,     0,    33,     0,    34,     0,
       0,    35,     0,     0,    36,     0,    37,    38,     0,    39,
      40,     0,    41,    42,     0,     0,    43,     0,     0,     0,
      44,    45,    46,    47,     0,     0,     0,     0,     0,    48,
      49,    50,     0,    51,    52,     0,    53,    54,    55,    56,
      57,    58,     0,     0,    59,     0,    60,    61,     0,     0,
      62,     0,     0,     0,    63,     0,     0,    64,    65,     0,
      66,    67,    23,    24,    25,     0,     0,     0,    26,     0,
       0,    27,     0,    28,     0,     0,    29,     0,     0,    30,
      31,    32,     0,     0,    33,     0,    34,     0,     0,    35,
       0,     0,    36,     0,    37,    38,     0,    39,    40,     0,
      41,    42,     0,     0,    43,     0,     0,     0,    44,    45,
      46,    47,     0,     0,     0,     0,     0,    48,    49,    50,
       0,    51,    52,     0,    53,    54,    55,    56,    57,    58,
       0,     0,    59,     0,    81,    61,     0,     0,    62,     0,
       0,     0,    63,     0,     0,    64,    65,   389,    66,    67,
       0,   169,   170,   171,   172,     0,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
       0,     0,     0,   187,     0,     0,     0,     0,   188,   169,
     170,   171,   172,   189,   173,   174,   175,   176,     0,     0,
     179,   180,   181,   182,     0,     0,   185,   186,     0,     0,
       0,     0,     0,     0,     0,     0,   190,     0,     0,     0,
     191,   189,     0,     0,   192,   193,     0,   194,   195,     0,
     196,   197,     0,     0,   198,     0,   385,     0,     0,   390,
       0,     0,   199,   169,   170,   171,   172,     0,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,     0,     0,     0,   187,     0,     0,     0,     0,
     188,   169,   170,   171,   172,   189,   173,   174,   175,   176,
       0,     0,   179,   180,   181,   182,     0,     0,   185,   186,
       0,     0,     0,     0,     0,     0,     0,     0,   190,     0,
       0,     0,   191,   189,     0,     0,   192,   193,     0,   194,
     195,     0,   196,   197,     0,     0,   198,     0,   386,     0,
       0,   407,     0,     0,   199,   169,   170,   171,   172,     0,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,     0,     0,     0,   187,     0,     0,
       0,     0,   188,     0,     0,     0,     0,   189,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     190,     0,     0,     0,   191,     0,     0,     0,   192,   193,
       0,   194,   195,     0,   196,   197,     0,     0,   198,     0,
       0,     0,     0,   441,     0,     0,   199,   169,   170,   171,
     172,     0,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,     0,     0,   187,
       0,     0,     0,     0,   188,     0,     0,     0,     0,   189,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   190,     0,     0,     0,   191,     0,     0,     0,
     192,   193,     0,   194,   195,     0,   196,   197,     0,     0,
     198,     0,     0,     0,   169,   170,   171,   172,   199,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,     0,     0,   187,     0,     0,   200,
       0,   188,     0,     0,     0,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   201,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   190,
       0,     0,     0,   191,     0,     0,     0,   192,   193,     0,
     194,   195,     0,   196,   197,     0,     0,   198,     0,     0,
       0,   169,   170,   171,   172,   199,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
       0,     0,     0,   187,     0,   317,     0,     0,   188,     0,
       0,     0,     0,   189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   190,     0,     0,     0,
     191,     0,     0,     0,   192,   193,     0,   194,   195,     0,
     196,   197,     0,     0,   198,     0,     0,     0,   169,   170,
     171,   172,   199,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,     0,     0,
     187,   326,     0,     0,     0,   188,     0,     0,     0,     0,
     189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   190,     0,     0,     0,   191,     0,     0,
       0,   192,   193,     0,   194,   195,     0,   196,   197,     0,
       0,   198,     0,     0,     0,   169,   170,   171,   172,   199,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,     0,     0,     0,   187,     0,     0,
       0,     0,   188,     0,     0,     0,     0,   189,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   359,     0,     0,     0,     0,     0,     0,     0,     0,
     190,     0,     0,     0,   191,     0,     0,     0,   192,   193,
       0,   194,   195,     0,   196,   197,     0,     0,   198,     0,
       0,     0,   169,   170,   171,   172,   199,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,     0,     0,     0,   187,   377,     0,     0,     0,   188,
       0,     0,     0,     0,   189,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   190,     0,     0,
       0,   191,     0,     0,     0,   192,   193,     0,   194,   195,
       0,   196,   197,     0,     0,   198,     0,     0,     0,   169,
     170,   171,   172,   199,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,     0,
       0,   187,     0,     0,     0,     0,   188,     0,     0,     0,
       0,   189,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   412,     0,     0,     0,     0,
       0,     0,     0,     0,   190,     0,     0,     0,   191,     0,
       0,     0,   192,   193,     0,   194,   195,     0,   196,   197,
       0,     0,   198,     0,     0,     0,   169,   170,   171,   172,
     199,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,     0,     0,     0,   187,     0,
       0,     0,     0,   188,     0,     0,     0,     0,   189,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   190,     0,     0,     0,   191,     0,     0,     0,   192,
     193,     0,   194,   195,     0,   196,   197,     0,     0,   198,
       0,     0,     0,   169,   170,   171,   172,   199,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,     0,     0,     0,   187,     0,     0,     0,     0,
     188,     0,     0,     0,     0,   189,   169,   170,   171,   172,
       0,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,     0,     0,     0,   190,     0,
       0,     0,   191,   188,     0,     0,   192,   193,   189,   194,
     195,     0,   196,   197,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   199,     0,     0,     0,     0,     0,
       0,   190,     0,     0,     0,   191,     0,     0,     0,   192,
     193,     0,   194,   195,     0,   196,   197,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   199
  };

  const short
  parser::yycheck_[] =
  {
      19,    20,    21,   300,   108,   153,    19,    20,    21,    75,
      76,   350,    78,     8,     9,   269,   355,    27,    53,    54,
      16,    39,    53,    54,    49,     3,   105,   309,    94,    49,
      39,    40,    33,   111,    27,    43,   114,   103,    47,     0,
       3,    66,     5,    61,    40,    36,    66,    57,    36,    36,
      51,    50,   349,    62,    32,    65,    75,    76,    77,    78,
      79,   165,    75,    76,    57,    78,    79,    85,    77,   148,
      79,    70,    65,   108,   356,    94,    85,   108,   226,    88,
      90,    94,    92,   337,   103,    76,   104,   106,    76,    76,
     103,    35,   431,   106,     4,   104,     6,    48,     7,    92,
      91,    47,    47,    91,    91,    96,   105,    77,    96,    96,
       4,    48,     6,   410,     4,    98,     6,   112,    97,    63,
       4,    68,     6,    67,    75,   191,    77,   146,   107,    80,
      74,    75,    42,    79,    79,    79,    47,    64,    75,   243,
      77,    98,    49,    80,    34,    89,   443,   166,    42,     8,
       9,    10,    11,   219,    13,    14,    15,    16,    42,    66,
      19,    20,    21,    22,    56,   269,    25,    26,    79,     4,
     189,     6,   191,    49,     5,     4,   473,     6,   191,     5,
       4,    40,     6,    16,    17,     8,     9,    10,    11,   106,
      66,    14,    80,    16,   213,   214,    55,     4,    95,     6,
     219,   214,     8,     9,    10,    11,   219,    13,    14,    15,
      16,   277,     3,    19,    20,    21,    22,    40,     6,    25,
      26,    20,    21,     8,     9,    10,    11,   331,    13,    14,
      15,    16,     3,   337,    40,     4,     3,     6,     5,     4,
      25,    26,    10,    11,   110,     4,    14,     6,    16,    55,
       4,   317,     6,     3,     3,    40,     5,     4,   277,     6,
       4,     4,     6,     6,   277,     4,   332,     6,     8,     9,
      10,    11,    40,    13,    14,    15,    16,     3,   382,    19,
      20,    21,    22,     3,   109,    25,    26,    98,    99,   100,
       4,   112,     6,   104,   112,     4,   107,     6,   317,     4,
      40,     6,    29,     4,   317,     6,   325,    79,    32,    73,
      80,     3,    73,   332,   418,    55,    73,   383,     3,   332,
      42,     8,     9,    10,    11,     6,    13,    14,    15,    16,
      93,   350,    19,    20,    21,    22,   355,   350,    25,    26,
       6,     4,   355,     4,     6,   449,     5,    40,     3,     3,
      41,     3,     3,    40,     4,   113,   101,   113,   113,     4,
       4,     4,     4,     4,   383,   110,   111,   112,   113,     3,
     383,     4,    69,    48,   119,     3,   395,    34,    48,    69,
      69,    88,     4,    69,   214,   290,   214,   317,    28,    28,
      28,   204,   491,   301,   106,   161,   148,   355,   496,   399,
      -1,    -1,    -1,    -1,    -1,   471,    -1,    -1,    -1,    -1,
      -1,    -1,   431,    -1,   159,    -1,    -1,   162,   431,    -1,
      -1,    -1,    -1,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,    -1,   190,    -1,   192,    -1,   194,
     195,    -1,   471,   198,   199,    -1,    -1,    -1,   471,   204,
      -1,    -1,     8,     9,    10,    11,    -1,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,   234,
       8,     9,    10,    11,    40,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    55,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     275,   276,    -1,   278,   279,    -1,   281,    -1,    -1,   284,
      -1,    -1,    -1,    -1,    -1,    63,   291,    -1,   293,    67,
      -1,    -1,    -1,    71,    72,    -1,    74,    75,    -1,    77,
      78,   306,    -1,    81,   309,    -1,    -1,   312,    -1,    -1,
     315,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   326,   327,    -1,    -1,    -1,    -1,    -1,   333,   334,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   343,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   353,    -1,
      -1,   356,     3,     4,    -1,   360,    -1,     8,     9,    10,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   377,   378,    -1,    -1,    27,    28,    29,   384,
     385,   386,    33,    -1,    -1,    36,    37,    38,    -1,    -1,
      41,    -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,
      51,    52,    -1,    54,    -1,    -1,    57,    58,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    69,    -1,
      -1,    -1,    73,    74,    75,    76,    77,    -1,    79,    -1,
      -1,    82,    83,    84,    -1,    86,    87,    -1,    89,    90,
      91,    92,    93,    94,    -1,    -1,    97,    -1,    99,   100,
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
      87,    -1,    89,    90,    91,    92,    93,    94,    95,    -1,
      97,    -1,    99,   100,    -1,   102,   103,    -1,    -1,    -1,
     107,    -1,    -1,   110,   111,   112,   113,   114,   115,   116,
       3,     4,    -1,    -1,    -1,     8,     9,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,    -1,
      33,    -1,    -1,    36,    37,    38,    -1,    -1,    41,    -1,
      -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    69,    -1,    -1,    -1,
      73,    74,    75,    76,    77,    -1,    79,    -1,    -1,    82,
      83,    84,    -1,    86,    87,    -1,    89,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    99,   100,    -1,   102,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,   111,   112,
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
      -1,   110,   111,   112,   113,   114,   115,   116,     3,    -1,
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
      -1,    -1,   107,   108,    -1,   110,   111,   112,   113,   114,
     115,   116,     3,     4,    -1,    -1,    -1,     8,     9,    -1,
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
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,   111,   112,
     113,   114,   115,   116,     3,    -1,    -1,    -1,    -1,     8,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      29,    -1,    -1,    -1,    33,    -1,    -1,    36,    -1,    38,
      -1,    -1,    41,    -1,    -1,    44,    45,    46,    -1,    -1,
      49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    -1,    -1,
      69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,    -1,
      79,    -1,    -1,    82,    83,    84,    -1,    86,    87,    -1,
      89,    90,    91,    92,    93,    94,     3,    -1,    97,    -1,
      99,   100,    -1,   102,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,   111,   112,   113,   114,   115,    -1,    -1,    -1,
      27,    28,    29,    -1,    -1,    -1,    33,    -1,    -1,    36,
      -1,    38,    -1,    -1,    41,    -1,    -1,    44,    45,    46,
      -1,    -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    -1,    69,    -1,    -1,    -1,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    -1,    86,
      87,    -1,    89,    90,    91,    92,    93,    94,     3,    -1,
      97,    -1,    99,   100,    -1,    -1,   103,    -1,    -1,    -1,
     107,    -1,    -1,   110,   111,    -1,   113,   114,    -1,    -1,
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
      93,    94,    -1,    -1,    97,    -1,    99,   100,    -1,    -1,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,   111,    -1,
     113,   114,     8,     9,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,
       8,     9,    10,    11,    40,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    -1,    -1,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    67,    40,    -1,    -1,    71,    72,    -1,    74,    75,
      -1,    77,    78,    -1,    -1,    81,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     8,     9,    10,    11,   101,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,     8,
       9,    10,    11,    40,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      67,    40,    -1,    -1,    71,    72,    -1,    74,    75,    -1,
      77,    78,    -1,    -1,    81,    -1,    55,    -1,    27,    28,
      29,    -1,    89,    -1,    33,    -1,    -1,    36,    -1,    38,
      -1,    -1,    41,    42,   101,    44,    45,    46,    -1,    -1,
      49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,    -1,
      59,    60,    -1,    62,    63,    64,    65,    66,    -1,    -1,
      69,    -1,    -1,    -1,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    -1,    86,    87,    -1,
      89,    90,    91,    92,    93,    94,    -1,    -1,    97,    -1,
      99,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,   110,   111,    -1,   113,   114,    27,    28,    29,    -1,
      -1,    -1,    33,    -1,    -1,    36,    -1,    38,    -1,    -1,
      41,    -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    -1,    59,    60,
      -1,    62,    63,    64,    65,    66,    -1,    -1,    69,    -1,
      -1,    -1,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      -1,    82,    83,    84,    -1,    86,    87,    -1,    89,    90,
      91,    92,    93,    94,    -1,    -1,    97,    -1,    99,   100,
      -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,
     111,    -1,   113,   114,    27,    28,    29,    -1,    -1,    -1,
      33,    -1,    -1,    36,    -1,    38,    -1,    -1,    41,    -1,
      -1,    44,    45,    46,    -1,    -1,    49,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    69,    -1,    -1,    -1,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    -1,    86,    87,    -1,    89,    90,    91,    92,
      93,    94,    -1,    -1,    97,    -1,    99,   100,    -1,    -1,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,   111,    -1,
     113,   114,    27,    28,    29,    -1,    -1,    -1,    33,    -1,
      -1,    36,    -1,    38,    -1,    -1,    41,    -1,    -1,    44,
      45,    46,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    69,    -1,    -1,    -1,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    82,    83,    84,
      -1,    86,    87,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    99,   100,    -1,    -1,   103,    -1,
      -1,    -1,   107,    -1,    -1,   110,   111,     4,   113,   114,
      -1,     8,     9,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    35,     8,
       9,    10,    11,    40,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      67,    40,    -1,    -1,    71,    72,    -1,    74,    75,    -1,
      77,    78,    -1,    -1,    81,    -1,    55,    -1,    -1,     4,
      -1,    -1,    89,     8,     9,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      35,     8,     9,    10,    11,    40,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    -1,    -1,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    67,    40,    -1,    -1,    71,    72,    -1,    74,
      75,    -1,    77,    78,    -1,    -1,    81,    -1,    55,    -1,
      -1,     4,    -1,    -1,    89,     8,     9,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,
      -1,    74,    75,    -1,    77,    78,    -1,    -1,    81,    -1,
      -1,    -1,    -1,     4,    -1,    -1,    89,     8,     9,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      71,    72,    -1,    74,    75,    -1,    77,    78,    -1,    -1,
      81,    -1,    -1,    -1,     8,     9,    10,    11,    89,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,    -1,
      74,    75,    -1,    77,    78,    -1,    -1,    81,    -1,    -1,
      -1,     8,     9,    10,    11,    89,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    30,    -1,    32,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    71,    72,    -1,    74,    75,    -1,
      77,    78,    -1,    -1,    81,    -1,    -1,    -1,     8,     9,
      10,    11,    89,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    -1,
      30,    31,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    71,    72,    -1,    74,    75,    -1,    77,    78,    -1,
      -1,    81,    -1,    -1,    -1,     8,     9,    10,    11,    89,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,
      -1,    74,    75,    -1,    77,    78,    -1,    -1,    81,    -1,
      -1,    -1,     8,     9,    10,    11,    89,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    30,    31,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    71,    72,    -1,    74,    75,
      -1,    77,    78,    -1,    -1,    81,    -1,    -1,    -1,     8,
       9,    10,    11,    89,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    71,    72,    -1,    74,    75,    -1,    77,    78,
      -1,    -1,    81,    -1,    -1,    -1,     8,     9,    10,    11,
      89,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    67,    -1,    -1,    -1,    71,
      72,    -1,    74,    75,    -1,    77,    78,    -1,    -1,    81,
      -1,    -1,    -1,     8,     9,    10,    11,    89,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    40,     8,     9,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    67,    35,    -1,    -1,    71,    72,    40,    74,
      75,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    67,    -1,    -1,    -1,    71,
      72,    -1,    74,    75,    -1,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89
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
     121,   158,    39,    40,    47,    62,    77,    79,    85,    88,
     104,   157,    39,    61,    85,   104,     6,   168,   110,   153,
       4,   140,    53,    54,   108,   137,   140,    27,    57,    65,
      92,     4,    10,    52,   132,   121,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   121,   140,     3,
     121,   123,   140,   140,   140,    35,    63,    67,    74,    75,
      79,    89,   140,   140,   109,   145,   147,   112,   112,     4,
       6,     3,   121,     3,    64,   120,   121,   122,   125,    29,
      79,    73,   123,    80,   154,    32,     3,    73,    73,     3,
     168,    93,   101,   140,   140,    53,    54,    32,     6,     6,
       4,     6,     4,   132,     4,     5,    31,    55,     4,   132,
     141,     3,     5,    55,    55,   140,   140,     3,   121,   123,
     140,   140,   140,    55,   140,   126,   140,   140,   154,   143,
       3,   164,    41,     3,   140,     3,     3,   148,   140,    54,
     101,   140,   128,   113,   113,   113,     4,   121,   140,   140,
       4,     4,   132,   121,   123,   140,   140,    31,    55,     4,
     132,   141,     3,     5,    55,    55,    55,   140,     4,     4,
       4,   154,   124,   163,    48,    75,    77,    80,   165,   166,
     167,    27,    57,    65,    90,    92,   140,     4,   163,   148,
       4,   140,    54,     4,     4,     4,     4,     4,     3,   140,
     140,     4,     4,   132,   121,   123,   140,   140,   140,    34,
       4,     6,    69,   121,    48,    50,    70,   105,    48,    77,
     165,     4,     4,     4,    34,   154,     4,   132,     4,     3,
     124,    49,    66,    69,    36,    76,    91,    96,    36,    76,
      91,    96,    36,    76,    91,    96,    69,    48,    97,   107,
     156,    88,   154,     4,     4,     4,   132,    49,    66,    28,
      47,    79,    28,    47,    79,    28,    47,    79,    49,    66,
      69,   123,   154,     4,    49,    66,   164,   167
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
     159,   160,   160,   161,   161,   162,   162,   163,   163,   164,
     164,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   166,   166,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   168,   168,   168,   168,   168,   169,   169,
     169,   170,   170,   171,   171
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
       2,     3,     2,     1,     3,     0,     2,     1,     3,     0,
       3,     4,     4,     3,     3,     4,     4,     4,     3,     3,
       4,     4,     4,     3,     3,     4,     2,     1,     2,     0,
       1,     4,     4,     2,     5,     5,     3,     3,     3,     1,
       4,     4,     2,     7,     8,     6,     5,    10,     1,     3,
       2,     0,     2,     7,    10
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
  "\"ACTION\"", "\"ALWAYS\"", "\"AND\"", "\"AND BETWEEN\"", "\"AS\"",
  "\"ASC\"", "\"AUTOINCREMENT\"", "\"BETWEEN\"", "\"CASCADE\"", "\"CASE\"",
  "\"CAST\"", "\"CHECK\"", "\"COLLATE\"", "\"CONFLICT\"", "\"CONSTRAINT\"",
  "\"CREATE\"", "\"CURRENT_DATE\"", "\"CURRENT_TIME\"",
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

#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   309,   309,   310,   314,   315,   316,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   336,   337,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   384,   385,   386,
     387,   388,   389,   393,   394,   395,   396,   397,   398,   399,
     403,   404,   408,   409,   413,   414,   418,   419,   420,   424,
     425,   426,   427,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,   470,
     474,   475,   479,   480,   481,   482,   486,   487,   488,   492,
     493,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   512,   516,   517,   521,
     522,   523,   524,   528,   529,   530,   531,   535,   536,   537,
     538,   539,   540,   541,   542,   543,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   561,   569,   570,   574,   575,
     576,   584,   585,   589,   590,   594,   595,   596,   600,   614,
     615,   619,   634,   635,   636,   640,   652,   653,   654,   658,
     659,   663,   664,   665,   666,   667,   668,   672,   673,   677,
     678,   679,   683,   684,   688,   697,   707,   712,   717,   722,
     728,   734,   740,   746,   753,   759,   765,   771,   782,   793,
     794,   798,   850,   854,   855,   859,   860,   864,   865,   869,
     870,   874,   875,   876,   877,   878,   879,   880,   881,   882,
     883,   884,   885,   886,   887,   888,   889,   893,   894,   898,
     899,   900,   901,   902,   903,   904,   905,   906,   907,   908,
     909,   910,   911,   915,   921,   928,   934,   938,   946,   947,
     948,   952,   953,   957,   961
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
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
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
#line 4719 "sqlite3_parser.cpp"

#line 983 "sqlite3_parser.yy"


void sqlb::parser::parser::error(const location_type& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}
