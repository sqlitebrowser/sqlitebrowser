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
#line 29 "sqlite3_parser.yy"

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
      case 138: // optional_if_not_exists
      case 140: // optional_unique
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case 145: // createindex_stmt
        value.YY_MOVE_OR_COPY< sqlb::IndexPtr > (YY_MOVE (that.value));
        break;

      case 143: // indexed_column
        value.YY_MOVE_OR_COPY< sqlb::IndexedColumn > (YY_MOVE (that.value));
        break;

      case 144: // indexed_column_list
        value.YY_MOVE_OR_COPY< sqlb::IndexedColumnVector > (YY_MOVE (that.value));
        break;

      case 147: // createvirtualtable_stmt
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
      case 121: // signednumber
      case 122: // signednumber_or_numeric
      case 123: // typename_namelist
      case 124: // type_name
      case 125: // unary_expr
      case 126: // binary_expr
      case 127: // like_expr
      case 128: // exprlist_expr
      case 129: // function_expr
      case 130: // isnull_expr
      case 131: // between_expr
      case 132: // in_expr
      case 133: // whenthenlist_expr
      case 134: // case_expr
      case 135: // raise_expr
      case 136: // expr
      case 137: // select_stmt
      case 139: // optional_sort_order
      case 141: // optional_where
      case 142: // tableid_with_uninteresting_schema
      case 146: // optional_exprlist_with_paren
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
    switch (that.type_get ())
    {
      case 138: // optional_if_not_exists
      case 140: // optional_unique
        value.move< bool > (YY_MOVE (that.value));
        break;

      case 145: // createindex_stmt
        value.move< sqlb::IndexPtr > (YY_MOVE (that.value));
        break;

      case 143: // indexed_column
        value.move< sqlb::IndexedColumn > (YY_MOVE (that.value));
        break;

      case 144: // indexed_column_list
        value.move< sqlb::IndexedColumnVector > (YY_MOVE (that.value));
        break;

      case 147: // createvirtualtable_stmt
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
      case 121: // signednumber
      case 122: // signednumber_or_numeric
      case 123: // typename_namelist
      case 124: // type_name
      case 125: // unary_expr
      case 126: // binary_expr
      case 127: // like_expr
      case 128: // exprlist_expr
      case 129: // function_expr
      case 130: // isnull_expr
      case 131: // between_expr
      case 132: // in_expr
      case 133: // whenthenlist_expr
      case 134: // case_expr
      case 135: // raise_expr
      case 136: // expr
      case 137: // select_stmt
      case 139: // optional_sort_order
      case 141: // optional_where
      case 142: // tableid_with_uninteresting_schema
      case 146: // optional_exprlist_with_paren
        value.move< std::string > (YY_MOVE (that.value));
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
      case 138: // optional_if_not_exists
      case 140: // optional_unique
        value.move< bool > (that.value);
        break;

      case 145: // createindex_stmt
        value.move< sqlb::IndexPtr > (that.value);
        break;

      case 143: // indexed_column
        value.move< sqlb::IndexedColumn > (that.value);
        break;

      case 144: // indexed_column_list
        value.move< sqlb::IndexedColumnVector > (that.value);
        break;

      case 147: // createvirtualtable_stmt
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
      case 121: // signednumber
      case 122: // signednumber_or_numeric
      case 123: // typename_namelist
      case 124: // type_name
      case 125: // unary_expr
      case 126: // binary_expr
      case 127: // like_expr
      case 128: // exprlist_expr
      case 129: // function_expr
      case 130: // isnull_expr
      case 131: // between_expr
      case 132: // in_expr
      case 133: // whenthenlist_expr
      case 134: // case_expr
      case 135: // raise_expr
      case 136: // expr
      case 137: // select_stmt
      case 139: // optional_sort_order
      case 141: // optional_where
      case 142: // tableid_with_uninteresting_schema
      case 146: // optional_exprlist_with_paren
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
      case 138: // optional_if_not_exists
      case 140: // optional_unique
        yylhs.value.emplace< bool > ();
        break;

      case 145: // createindex_stmt
        yylhs.value.emplace< sqlb::IndexPtr > ();
        break;

      case 143: // indexed_column
        yylhs.value.emplace< sqlb::IndexedColumn > ();
        break;

      case 144: // indexed_column_list
        yylhs.value.emplace< sqlb::IndexedColumnVector > ();
        break;

      case 147: // createvirtualtable_stmt
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
      case 121: // signednumber
      case 122: // signednumber_or_numeric
      case 123: // typename_namelist
      case 124: // type_name
      case 125: // unary_expr
      case 126: // binary_expr
      case 127: // like_expr
      case 128: // exprlist_expr
      case 129: // function_expr
      case 130: // isnull_expr
      case 131: // between_expr
      case 132: // in_expr
      case 133: // whenthenlist_expr
      case 134: // case_expr
      case 135: // raise_expr
      case 136: // expr
      case 137: // select_stmt
      case 139: // optional_sort_order
      case 141: // optional_where
      case 142: // tableid_with_uninteresting_schema
      case 146: // optional_exprlist_with_paren
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
  case 4:
#line 230 "sqlite3_parser.yy"
    { drv.result = yystack_[0].value.as < sqlb::IndexPtr > (); }
#line 1118 "sqlite3_parser.cpp"
    break;

  case 5:
#line 231 "sqlite3_parser.yy"
    { drv.result = yystack_[0].value.as < sqlb::TablePtr > (); }
#line 1124 "sqlite3_parser.cpp"
    break;

  case 6:
#line 239 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1130 "sqlite3_parser.cpp"
    break;

  case 7:
#line 240 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1136 "sqlite3_parser.cpp"
    break;

  case 8:
#line 241 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1142 "sqlite3_parser.cpp"
    break;

  case 9:
#line 242 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1148 "sqlite3_parser.cpp"
    break;

  case 10:
#line 243 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1154 "sqlite3_parser.cpp"
    break;

  case 11:
#line 244 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1160 "sqlite3_parser.cpp"
    break;

  case 12:
#line 245 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1166 "sqlite3_parser.cpp"
    break;

  case 13:
#line 246 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1172 "sqlite3_parser.cpp"
    break;

  case 14:
#line 247 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1178 "sqlite3_parser.cpp"
    break;

  case 15:
#line 251 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1184 "sqlite3_parser.cpp"
    break;

  case 16:
#line 252 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1190 "sqlite3_parser.cpp"
    break;

  case 17:
#line 257 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1196 "sqlite3_parser.cpp"
    break;

  case 18:
#line 258 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1202 "sqlite3_parser.cpp"
    break;

  case 19:
#line 259 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1208 "sqlite3_parser.cpp"
    break;

  case 20:
#line 260 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1214 "sqlite3_parser.cpp"
    break;

  case 21:
#line 261 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1220 "sqlite3_parser.cpp"
    break;

  case 22:
#line 262 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1226 "sqlite3_parser.cpp"
    break;

  case 23:
#line 263 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1232 "sqlite3_parser.cpp"
    break;

  case 24:
#line 264 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1238 "sqlite3_parser.cpp"
    break;

  case 25:
#line 265 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1244 "sqlite3_parser.cpp"
    break;

  case 26:
#line 266 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1250 "sqlite3_parser.cpp"
    break;

  case 27:
#line 267 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1256 "sqlite3_parser.cpp"
    break;

  case 28:
#line 268 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1262 "sqlite3_parser.cpp"
    break;

  case 29:
#line 269 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1268 "sqlite3_parser.cpp"
    break;

  case 30:
#line 270 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1274 "sqlite3_parser.cpp"
    break;

  case 31:
#line 271 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1280 "sqlite3_parser.cpp"
    break;

  case 32:
#line 272 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1286 "sqlite3_parser.cpp"
    break;

  case 33:
#line 273 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1292 "sqlite3_parser.cpp"
    break;

  case 34:
#line 274 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1298 "sqlite3_parser.cpp"
    break;

  case 35:
#line 275 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1304 "sqlite3_parser.cpp"
    break;

  case 36:
#line 276 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1310 "sqlite3_parser.cpp"
    break;

  case 37:
#line 277 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1316 "sqlite3_parser.cpp"
    break;

  case 38:
#line 278 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1322 "sqlite3_parser.cpp"
    break;

  case 39:
#line 279 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1328 "sqlite3_parser.cpp"
    break;

  case 40:
#line 280 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1334 "sqlite3_parser.cpp"
    break;

  case 41:
#line 281 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1340 "sqlite3_parser.cpp"
    break;

  case 42:
#line 282 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1346 "sqlite3_parser.cpp"
    break;

  case 43:
#line 283 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1352 "sqlite3_parser.cpp"
    break;

  case 44:
#line 284 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1358 "sqlite3_parser.cpp"
    break;

  case 45:
#line 285 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1364 "sqlite3_parser.cpp"
    break;

  case 46:
#line 286 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1370 "sqlite3_parser.cpp"
    break;

  case 47:
#line 287 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1376 "sqlite3_parser.cpp"
    break;

  case 48:
#line 288 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1382 "sqlite3_parser.cpp"
    break;

  case 49:
#line 289 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1388 "sqlite3_parser.cpp"
    break;

  case 50:
#line 290 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1394 "sqlite3_parser.cpp"
    break;

  case 51:
#line 291 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1400 "sqlite3_parser.cpp"
    break;

  case 52:
#line 292 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1406 "sqlite3_parser.cpp"
    break;

  case 53:
#line 296 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1412 "sqlite3_parser.cpp"
    break;

  case 54:
#line 297 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1418 "sqlite3_parser.cpp"
    break;

  case 55:
#line 298 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1424 "sqlite3_parser.cpp"
    break;

  case 56:
#line 299 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1430 "sqlite3_parser.cpp"
    break;

  case 57:
#line 300 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1436 "sqlite3_parser.cpp"
    break;

  case 58:
#line 301 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1442 "sqlite3_parser.cpp"
    break;

  case 59:
#line 315 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1448 "sqlite3_parser.cpp"
    break;

  case 60:
#line 316 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1454 "sqlite3_parser.cpp"
    break;

  case 61:
#line 320 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1460 "sqlite3_parser.cpp"
    break;

  case 62:
#line 321 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1466 "sqlite3_parser.cpp"
    break;

  case 63:
#line 325 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1472 "sqlite3_parser.cpp"
    break;

  case 64:
#line 326 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 1478 "sqlite3_parser.cpp"
    break;

  case 65:
#line 330 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1484 "sqlite3_parser.cpp"
    break;

  case 66:
#line 331 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1490 "sqlite3_parser.cpp"
    break;

  case 67:
#line 332 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + "(" + yystack_[3].value.as < std::string > () + ", " + yystack_[1].value.as < std::string > () + ")"; }
#line 1496 "sqlite3_parser.cpp"
    break;

  case 68:
#line 336 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1502 "sqlite3_parser.cpp"
    break;

  case 69:
#line 337 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1508 "sqlite3_parser.cpp"
    break;

  case 70:
#line 338 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "~" + yystack_[0].value.as < std::string > (); }
#line 1514 "sqlite3_parser.cpp"
    break;

  case 71:
#line 339 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "NOT " + yystack_[0].value.as < std::string > (); }
#line 1520 "sqlite3_parser.cpp"
    break;

  case 72:
#line 343 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " || " + yystack_[0].value.as < std::string > (); }
#line 1526 "sqlite3_parser.cpp"
    break;

  case 73:
#line 344 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " * " + yystack_[0].value.as < std::string > (); }
#line 1532 "sqlite3_parser.cpp"
    break;

  case 74:
#line 345 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " / " + yystack_[0].value.as < std::string > (); }
#line 1538 "sqlite3_parser.cpp"
    break;

  case 75:
#line 346 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " % " + yystack_[0].value.as < std::string > (); }
#line 1544 "sqlite3_parser.cpp"
    break;

  case 76:
#line 347 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " + " + yystack_[0].value.as < std::string > (); }
#line 1550 "sqlite3_parser.cpp"
    break;

  case 77:
#line 348 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " - " + yystack_[0].value.as < std::string > (); }
#line 1556 "sqlite3_parser.cpp"
    break;

  case 78:
#line 349 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " << " + yystack_[0].value.as < std::string > (); }
#line 1562 "sqlite3_parser.cpp"
    break;

  case 79:
#line 350 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >> " + yystack_[0].value.as < std::string > (); }
#line 1568 "sqlite3_parser.cpp"
    break;

  case 80:
#line 351 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " & " + yystack_[0].value.as < std::string > (); }
#line 1574 "sqlite3_parser.cpp"
    break;

  case 81:
#line 352 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " | " + yystack_[0].value.as < std::string > (); }
#line 1580 "sqlite3_parser.cpp"
    break;

  case 82:
#line 353 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " < " + yystack_[0].value.as < std::string > (); }
#line 1586 "sqlite3_parser.cpp"
    break;

  case 83:
#line 354 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <= " + yystack_[0].value.as < std::string > (); }
#line 1592 "sqlite3_parser.cpp"
    break;

  case 84:
#line 355 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " > " + yystack_[0].value.as < std::string > (); }
#line 1598 "sqlite3_parser.cpp"
    break;

  case 85:
#line 356 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >= " + yystack_[0].value.as < std::string > (); }
#line 1604 "sqlite3_parser.cpp"
    break;

  case 86:
#line 357 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " = " + yystack_[0].value.as < std::string > (); }
#line 1610 "sqlite3_parser.cpp"
    break;

  case 87:
#line 358 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " == " + yystack_[0].value.as < std::string > (); }
#line 1616 "sqlite3_parser.cpp"
    break;

  case 88:
#line 359 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " != " + yystack_[0].value.as < std::string > (); }
#line 1622 "sqlite3_parser.cpp"
    break;

  case 89:
#line 360 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <> " + yystack_[0].value.as < std::string > (); }
#line 1628 "sqlite3_parser.cpp"
    break;

  case 90:
#line 361 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IS " + yystack_[0].value.as < std::string > (); }
#line 1634 "sqlite3_parser.cpp"
    break;

  case 91:
#line 362 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 1640 "sqlite3_parser.cpp"
    break;

  case 92:
#line 363 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " OR " + yystack_[0].value.as < std::string > (); }
#line 1646 "sqlite3_parser.cpp"
    break;

  case 93:
#line 367 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " LIKE " + yystack_[0].value.as < std::string > (); }
#line 1652 "sqlite3_parser.cpp"
    break;

  case 94:
#line 368 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " GLOB " + yystack_[0].value.as < std::string > (); }
#line 1658 "sqlite3_parser.cpp"
    break;

  case 95:
#line 369 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " MATCH " + yystack_[0].value.as < std::string > (); }
#line 1664 "sqlite3_parser.cpp"
    break;

  case 96:
#line 370 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " REGEXP " + yystack_[0].value.as < std::string > (); }
#line 1670 "sqlite3_parser.cpp"
    break;

  case 97:
#line 371 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT LIKE " + yystack_[0].value.as < std::string > (); }
#line 1676 "sqlite3_parser.cpp"
    break;

  case 98:
#line 372 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT GLOB " + yystack_[0].value.as < std::string > (); }
#line 1682 "sqlite3_parser.cpp"
    break;

  case 99:
#line 373 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT MATCH " + yystack_[0].value.as < std::string > (); }
#line 1688 "sqlite3_parser.cpp"
    break;

  case 100:
#line 374 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT REGEXP " + yystack_[0].value.as < std::string > (); }
#line 1694 "sqlite3_parser.cpp"
    break;

  case 101:
#line 375 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " LIKE " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1700 "sqlite3_parser.cpp"
    break;

  case 102:
#line 376 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " GLOB " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1706 "sqlite3_parser.cpp"
    break;

  case 103:
#line 377 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " MATCH " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1712 "sqlite3_parser.cpp"
    break;

  case 104:
#line 378 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " REGEXP " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1718 "sqlite3_parser.cpp"
    break;

  case 105:
#line 379 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT LIKE " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1724 "sqlite3_parser.cpp"
    break;

  case 106:
#line 380 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT GLOB " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1730 "sqlite3_parser.cpp"
    break;

  case 107:
#line 381 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT MATCH " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1736 "sqlite3_parser.cpp"
    break;

  case 108:
#line 382 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT REGEXP " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1742 "sqlite3_parser.cpp"
    break;

  case 109:
#line 386 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1748 "sqlite3_parser.cpp"
    break;

  case 110:
#line 387 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + ", " + yystack_[0].value.as < std::string > (); }
#line 1754 "sqlite3_parser.cpp"
    break;

  case 111:
#line 391 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1760 "sqlite3_parser.cpp"
    break;

  case 112:
#line 392 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + "(DISTINCT " + yystack_[1].value.as < std::string > () + ")"; }
#line 1766 "sqlite3_parser.cpp"
    break;

  case 113:
#line 393 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + "()"; }
#line 1772 "sqlite3_parser.cpp"
    break;

  case 114:
#line 394 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(*)"; }
#line 1778 "sqlite3_parser.cpp"
    break;

  case 115:
#line 398 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " ISNULL"; }
#line 1784 "sqlite3_parser.cpp"
    break;

  case 116:
#line 399 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " NOTNULL"; }
#line 1790 "sqlite3_parser.cpp"
    break;

  case 117:
#line 400 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " NOT NULL"; }
#line 1796 "sqlite3_parser.cpp"
    break;

  case 118:
#line 404 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 1802 "sqlite3_parser.cpp"
    break;

  case 119:
#line 405 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 1808 "sqlite3_parser.cpp"
    break;

  case 120:
#line 409 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " IN ()"; }
#line 1814 "sqlite3_parser.cpp"
    break;

  case 121:
#line 410 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 1820 "sqlite3_parser.cpp"
    break;

  case 122:
#line 411 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 1826 "sqlite3_parser.cpp"
    break;

  case 123:
#line 412 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1832 "sqlite3_parser.cpp"
    break;

  case 124:
#line 413 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1838 "sqlite3_parser.cpp"
    break;

  case 125:
#line 414 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 1844 "sqlite3_parser.cpp"
    break;

  case 126:
#line 415 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1850 "sqlite3_parser.cpp"
    break;

  case 127:
#line 416 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1856 "sqlite3_parser.cpp"
    break;

  case 128:
#line 417 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " NOT IN ()"; }
#line 1862 "sqlite3_parser.cpp"
    break;

  case 129:
#line 418 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 1868 "sqlite3_parser.cpp"
    break;

  case 130:
#line 419 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 1874 "sqlite3_parser.cpp"
    break;

  case 131:
#line 420 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1880 "sqlite3_parser.cpp"
    break;

  case 132:
#line 421 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1886 "sqlite3_parser.cpp"
    break;

  case 133:
#line 422 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 1892 "sqlite3_parser.cpp"
    break;

  case 134:
#line 423 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[8].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1898 "sqlite3_parser.cpp"
    break;

  case 135:
#line 424 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " NOT IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1904 "sqlite3_parser.cpp"
    break;

  case 136:
#line 428 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "WHEN " + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 1910 "sqlite3_parser.cpp"
    break;

  case 137:
#line 429 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " WHEN" + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 1916 "sqlite3_parser.cpp"
    break;

  case 138:
#line 433 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CASE " + yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 1922 "sqlite3_parser.cpp"
    break;

  case 139:
#line 434 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CASE " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " END"; }
#line 1928 "sqlite3_parser.cpp"
    break;

  case 140:
#line 435 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CASE " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 1934 "sqlite3_parser.cpp"
    break;

  case 141:
#line 436 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CASE " + yystack_[1].value.as < std::string > () + " END"; }
#line 1940 "sqlite3_parser.cpp"
    break;

  case 142:
#line 440 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "RAISE(IGNORE)"; }
#line 1946 "sqlite3_parser.cpp"
    break;

  case 143:
#line 441 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "RAISE(ROLLBACK, " + yystack_[1].value.as < std::string > () + ")"; }
#line 1952 "sqlite3_parser.cpp"
    break;

  case 144:
#line 442 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "RAISE(ABORT, " + yystack_[1].value.as < std::string > () + ")"; }
#line 1958 "sqlite3_parser.cpp"
    break;

  case 145:
#line 443 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "RAISE(FAIL, " + yystack_[1].value.as < std::string > () + ")"; }
#line 1964 "sqlite3_parser.cpp"
    break;

  case 146:
#line 447 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1970 "sqlite3_parser.cpp"
    break;

  case 147:
#line 448 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1976 "sqlite3_parser.cpp"
    break;

  case 148:
#line 449 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1982 "sqlite3_parser.cpp"
    break;

  case 149:
#line 450 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1988 "sqlite3_parser.cpp"
    break;

  case 150:
#line 451 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1994 "sqlite3_parser.cpp"
    break;

  case 151:
#line 452 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 2000 "sqlite3_parser.cpp"
    break;

  case 152:
#line 453 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2006 "sqlite3_parser.cpp"
    break;

  case 153:
#line 454 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2012 "sqlite3_parser.cpp"
    break;

  case 154:
#line 455 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2018 "sqlite3_parser.cpp"
    break;

  case 155:
#line 456 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 2024 "sqlite3_parser.cpp"
    break;

  case 156:
#line 457 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CAST(" + yystack_[3].value.as < std::string > () + " AS " + yystack_[1].value.as < std::string > () + ")"; }
#line 2030 "sqlite3_parser.cpp"
    break;

  case 157:
#line 458 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " COLLATE " + yystack_[0].value.as < std::string > (); }
#line 2036 "sqlite3_parser.cpp"
    break;

  case 158:
#line 459 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2042 "sqlite3_parser.cpp"
    break;

  case 159:
#line 460 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2048 "sqlite3_parser.cpp"
    break;

  case 160:
#line 461 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2054 "sqlite3_parser.cpp"
    break;

  case 161:
#line 462 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2060 "sqlite3_parser.cpp"
    break;

  case 162:
#line 463 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2066 "sqlite3_parser.cpp"
    break;

  case 163:
#line 464 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2072 "sqlite3_parser.cpp"
    break;

  case 164:
#line 473 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "SELECT"; }
#line 2078 "sqlite3_parser.cpp"
    break;

  case 165:
#line 481 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = false; }
#line 2084 "sqlite3_parser.cpp"
    break;

  case 166:
#line 482 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = true; }
#line 2090 "sqlite3_parser.cpp"
    break;

  case 167:
#line 486 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = ""; }
#line 2096 "sqlite3_parser.cpp"
    break;

  case 168:
#line 487 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "ASC"; }
#line 2102 "sqlite3_parser.cpp"
    break;

  case 169:
#line 488 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "DESC"; }
#line 2108 "sqlite3_parser.cpp"
    break;

  case 170:
#line 496 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = false; }
#line 2114 "sqlite3_parser.cpp"
    break;

  case 171:
#line 497 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = true; }
#line 2120 "sqlite3_parser.cpp"
    break;

  case 172:
#line 501 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = ""; }
#line 2126 "sqlite3_parser.cpp"
    break;

  case 173:
#line 502 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2132 "sqlite3_parser.cpp"
    break;

  case 174:
#line 506 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2138 "sqlite3_parser.cpp"
    break;

  case 175:
#line 507 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2144 "sqlite3_parser.cpp"
    break;

  case 176:
#line 508 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2150 "sqlite3_parser.cpp"
    break;

  case 177:
#line 512 "sqlite3_parser.yy"
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
#line 2166 "sqlite3_parser.cpp"
    break;

  case 178:
#line 526 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::IndexedColumnVector > () = sqlb::IndexedColumnVector(1, yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2172 "sqlite3_parser.cpp"
    break;

  case 179:
#line 527 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::IndexedColumnVector > () = yystack_[2].value.as < sqlb::IndexedColumnVector > (); yylhs.value.as < sqlb::IndexedColumnVector > ().push_back(yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2178 "sqlite3_parser.cpp"
    break;

  case 180:
#line 531 "sqlite3_parser.yy"
    {
													yylhs.value.as < sqlb::IndexPtr > () = sqlb::IndexPtr(new sqlb::Index(yystack_[6].value.as < std::string > ()));
													yylhs.value.as < sqlb::IndexPtr > ()->setTable(yystack_[4].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setUnique(yystack_[9].value.as < bool > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setWhereExpr(yystack_[0].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->fields = yystack_[2].value.as < sqlb::IndexedColumnVector > ();
													yylhs.value.as < sqlb::IndexPtr > ()->setFullyParsed(true);
												}
#line 2191 "sqlite3_parser.cpp"
    break;

  case 181:
#line 542 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = {}; }
#line 2197 "sqlite3_parser.cpp"
    break;

  case 182:
#line 543 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = {}; }
#line 2203 "sqlite3_parser.cpp"
    break;

  case 183:
#line 544 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 2209 "sqlite3_parser.cpp"
    break;

  case 184:
#line 548 "sqlite3_parser.yy"
    {
													yylhs.value.as < sqlb::TablePtr > () = sqlb::TablePtr(new sqlb::Table(yystack_[3].value.as < std::string > ()));
													yylhs.value.as < sqlb::TablePtr > ()->setVirtualUsing(yystack_[1].value.as < std::string > ());
													yylhs.value.as < sqlb::TablePtr > ()->setFullyParsed(false);
												}
#line 2219 "sqlite3_parser.cpp"
    break;


#line 2223 "sqlite3_parser.cpp"

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


  const signed char parser::yypact_ninf_ = -128;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -40,   -44,     6,    28,  -128,  -128,  -128,   -80,   -27,  -128,
    -128,    -1,    -1,   -11,  1705,  1705,    10,  -128,  -128,  -128,
    -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,
    -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,
    -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,
    -128,    66,  -128,  -128,  -128,  -128,  -128,  -128,  -128,    71,
    -128,  -128,   -21,     7,  -128,  1790,  1790,   -49,  1790,  -128,
    -128,  -128,  -128,    94,   111,   745,  -128,  1189,  1189,  -128,
    1189,  1189,  1189,   856,   123,  -128,  -128,  -128,  -128,  1189,
    -128,   136,  -128,  -128,  -128,  -128,  -128,  -128,    41,  -128,
    -128,  -128,  -128,    44,  -128,  -128,  -128,  -128,  -128,  -128,
    2344,  1894,  -128,    64,    82,  -128,  -128,  -128,  1189,   -39,
       8,  1189,  2451,   -18,   412,   -49,  -128,  1189,  1189,  1189,
    1189,  1189,  1189,  1189,  1189,  1189,  1189,  1189,  1189,  1189,
    1189,  1189,  1189,  1189,  1189,  1189,  1189,  1189,   -49,  1189,
    1279,  1189,  -128,  1189,  1189,     2,  -128,  1189,  1189,  -128,
    -128,  -128,    11,  1189,  -128,  1563,  1189,  -128,  1189,   -12,
    1969,   132,   141,   137,   146,  -128,   138,  1189,   103,   154,
    2344,    80,    80,     4,     4,   169,     4,   169,   128,  2371,
    2371,    90,    90,    90,    90,  2371,  2371,   169,   169,  2451,
    2044,  -128,   135,   523,   105,  -128,  2371,   315,  1590,  1189,
    1189,  1369,  1189,  1189,  -128,  1189,  2419,  1676,  1189,  -128,
    -128,  1189,  2119,  1649,  1189,  -128,  1790,    48,    59,  -128,
      60,  -128,   114,  -128,   -49,  1189,  1189,  -128,  -128,   117,
     167,  1189,  1790,  1189,  1189,  2194,  1921,   634,   119,  -128,
    1996,  2071,  2221,  1189,  2344,  2344,  -128,  1189,  2269,  -128,
    1459,   171,   172,   177,   178,  -128,  -128,  2371,  2371,  -128,
    -128,   121,   170,  -128,  2371,  2371,  1189,  1189,  -128,   126,
     182,  1189,  1790,  1189,  1189,  1189,  2371,  2344,  -128,     3,
    -128,  -128,  -128,  -128,  -128,  -128,   967,  2371,  2371,  -128,
    -128,   127,   184,  -128,  2371,  2371,  2371,    79,    81,  -128,
    -128,   131,  -128,   158,  -128,  1078,  -128,  -128,  -128,     3,
    -128,  -128,   159,   187,  -128,  -128
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,   170,     0,     2,     4,     5,   171,     0,     0,     1,
       3,   165,   165,     0,     0,     0,     0,    17,    18,    19,
      20,    21,    22,    55,    54,    56,    23,    24,    25,    26,
      27,    28,    29,    32,    34,    33,    30,    31,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    49,    48,    50,    51,    52,    15,    58,    16,    57,
      53,   176,     0,     0,   166,     0,     0,     0,     0,    49,
      57,   175,   174,   181,     0,     0,   184,     0,     0,   182,
       0,     0,     0,     0,    21,    13,    12,    14,    11,     0,
       9,    40,    10,     6,     7,     8,   148,   146,   151,   147,
     152,   153,   158,     0,   154,   159,   160,   161,   162,   163,
     109,   167,   178,     0,     0,    69,    68,    70,     0,     0,
       0,     0,    71,     0,     0,     0,   183,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   115,     0,     0,     0,   116,     0,     0,   168,
     169,   177,   172,     0,   155,     0,     0,   141,     0,     0,
       0,     0,     0,     0,     0,   113,     0,     0,     0,   150,
     110,    76,    77,    73,    74,    80,    75,    81,    72,    86,
      87,    84,    85,    82,    83,    88,    89,    78,    79,    91,
       0,   157,    94,     0,    57,   124,    90,    93,    95,     0,
       0,     0,     0,     0,   117,     0,    92,    96,     0,   180,
     179,     0,     0,     0,     0,   139,     0,     0,     0,   142,
       0,   114,     0,   111,     0,     0,     0,   120,   164,     0,
       0,     0,     0,     0,     0,     0,    98,     0,    57,   132,
      97,    99,   100,     0,   173,   136,   140,     0,     0,    63,
      65,     0,     0,     0,     0,   112,   149,   118,   102,   122,
     121,     0,    57,   123,   101,   103,     0,     0,   128,     0,
       0,     0,     0,     0,     0,     0,   104,   137,   138,     0,
      64,   156,   144,   145,   143,   127,     0,   119,   106,   130,
     129,     0,    57,   131,   105,   107,   108,     0,     0,    62,
      61,     0,   125,     0,   135,     0,    59,    60,    66,     0,
     126,   133,     0,     0,   134,    67
  };

  const short
  parser::yypgoto_[] =
  {
    -128,  -128,  -128,  -128,   -14,   -10,   -58,  -128,  -127,  -128,
    -128,  -128,  -128,  -128,   -75,  -128,  -128,  -128,  -128,    73,
    -128,  -128,   153,   -53,   183,  -128,  -128,  -128,   181,    35,
    -128,  -128,  -128,  -128
  };

  const short
  parser::yydefgoto_[] =
  {
      -1,     2,     3,    97,    98,    99,    61,   310,   311,   260,
     261,   100,   101,   102,   103,   104,   105,   106,   107,   119,
     108,   109,   110,   240,    14,   161,     8,   219,    62,   112,
     113,     4,    76,     5
  };

  const unsigned short
  parser::yytable_[] =
  {
      59,    59,     1,   114,    60,    60,     9,    71,    72,   171,
      74,   307,   308,   166,   167,    11,   128,   129,   130,   131,
     135,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,    10,   209,   146,   172,    12,
     224,   225,   147,   148,   124,   173,   125,   148,   126,   178,
     127,    70,    70,    73,    70,    60,    60,     6,    60,    56,
       7,    13,    58,   210,    16,    64,   168,   211,   162,   149,
     163,    65,   174,   150,   212,   213,    66,   151,   152,   214,
     153,   154,    67,   155,   156,    68,   164,   157,   127,   215,
     130,   131,   205,   168,   133,   158,   135,    75,   128,   129,
     130,   131,   232,   132,   133,   134,   135,   233,   241,   127,
     242,   179,   309,   118,    77,   144,   145,   218,   265,   148,
     127,   269,   281,   127,   282,   295,   121,   127,   239,   148,
     299,   314,   127,   127,   201,   318,   204,   319,   227,   123,
      60,   229,   231,   128,   129,   130,   131,   228,   132,   133,
     134,   135,   230,   249,   138,   139,   140,   141,   262,   234,
     144,   145,   320,   324,   127,   127,   271,   148,   259,   263,
     264,   270,   279,   296,   148,   291,   292,   128,   129,   130,
     131,   293,   294,   133,   273,   135,   300,   315,   316,   236,
     317,   325,   323,   169,   280,    15,    63,   248,   220,     0,
       0,    60,   290,     0,     0,     0,   301,     0,   148,     0,
       0,     0,    70,     0,     0,     0,    60,     0,     0,     0,
     266,   313,     0,     0,   303,     0,     0,     0,   272,     0,
     111,     0,    60,   115,   116,   117,   120,     0,     0,     0,
     322,     0,   122,     0,     0,     0,    70,     0,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
       0,   165,    60,     0,   170,     0,     0,     0,     0,     0,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,     0,   202,     0,   206,     0,   207,   208,     0,     0,
     216,   217,     0,     0,     0,     0,   111,     0,     0,   222,
       0,   223,     0,   128,   129,   130,   131,     0,   132,   133,
     134,   135,     0,     0,   138,   139,   140,   141,     0,     0,
     144,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   148,     0,     0,     0,     0,     0,
       0,     0,   245,   246,     0,   250,   251,     0,   252,   243,
       0,   254,     0,     0,   255,     0,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   267,   268,
       0,     0,     0,     0,     0,     0,   274,   275,     0,     0,
       0,     0,     0,     0,     0,     0,   286,     0,     0,     0,
     287,     0,     0,     0,     0,    78,   175,     0,     0,     0,
      80,    81,   176,     0,    82,     0,     0,     0,     0,   297,
     298,     0,     0,     0,     0,     0,   304,   305,   306,    17,
      18,     0,     0,     0,    19,     0,     0,    20,    83,    84,
       0,     0,    22,     0,     0,    85,    86,    87,     0,     0,
      26,     0,    27,   177,     0,    28,     0,     0,    29,    88,
      30,    31,     0,    32,     0,    33,    34,     0,     0,    35,
       0,     0,     0,    36,    37,    38,    39,    89,     0,    90,
       0,     0,    40,    41,    42,     0,    91,    44,     0,    45,
      46,    47,    48,    49,    50,     0,     0,     0,    69,    52,
       0,    92,    53,     0,     0,     0,    54,     0,     0,    55,
      56,    93,    94,    58,    95,    96,    78,   237,     0,     0,
       0,    80,    81,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      17,    18,     0,     0,     0,    19,     0,     0,    20,    83,
      84,     0,     0,    22,     0,     0,    85,    86,    87,     0,
       0,    26,     0,    27,     0,     0,    28,     0,     0,    29,
      88,    30,    31,     0,    32,     0,    33,    34,     0,     0,
      35,     0,     0,     0,    36,    37,    38,    39,    89,     0,
      90,     0,     0,    40,    41,    42,     0,    91,    44,     0,
      45,    46,    47,    48,    49,    50,   238,     0,     0,    69,
      52,     0,    92,    53,     0,     0,     0,    54,     0,     0,
      55,    56,    93,    94,    58,    95,    96,    78,   278,     0,
       0,     0,    80,    81,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    17,    18,     0,     0,     0,    19,     0,     0,    20,
      83,    84,     0,     0,    22,     0,     0,    85,    86,    87,
       0,     0,    26,     0,    27,     0,     0,    28,     0,     0,
      29,    88,    30,    31,     0,    32,     0,    33,    34,     0,
       0,    35,     0,     0,     0,    36,    37,    38,    39,    89,
       0,    90,     0,     0,    40,    41,    42,     0,    91,    44,
       0,    45,    46,    47,    48,    49,    50,   238,     0,     0,
      69,    52,     0,    92,    53,     0,     0,     0,    54,     0,
       0,    55,    56,    93,    94,    58,    95,    96,    78,    79,
       0,     0,     0,    80,    81,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    17,    18,     0,     0,     0,    19,     0,     0,
      20,    83,    84,     0,     0,    22,     0,     0,    85,    86,
      87,     0,     0,    26,     0,    27,     0,     0,    28,     0,
       0,    29,    88,    30,    31,     0,    32,     0,    33,    34,
       0,     0,    35,     0,     0,     0,    36,    37,    38,    39,
      89,     0,    90,     0,     0,    40,    41,    42,     0,    91,
      44,     0,    45,    46,    47,    48,    49,    50,     0,     0,
       0,    69,    52,     0,    92,    53,     0,     0,     0,    54,
       0,     0,    55,    56,    93,    94,    58,    95,    96,    78,
       0,     0,     0,     0,    80,    81,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    17,    18,     0,     0,     0,    19,     0,
       0,    20,    83,    84,     0,     0,    22,     0,     0,    85,
      86,    87,     0,     0,    26,     0,    27,     0,     0,    28,
       0,     0,    29,    88,    30,    31,     0,    32,     0,    33,
      34,     0,     0,    35,     0,     0,     0,    36,    37,    38,
      39,    89,     0,    90,     0,     0,    40,    41,    42,     0,
      91,    44,     0,    45,    46,    47,    48,    49,    50,     0,
       0,     0,    69,    52,     0,    92,    53,     0,     0,     0,
      54,   118,     0,    55,    56,    93,    94,    58,    95,    96,
      78,   312,     0,     0,     0,    80,    81,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    17,    18,     0,     0,     0,    19,
       0,     0,    20,    83,    84,     0,     0,    22,     0,     0,
      85,    86,    87,     0,     0,    26,     0,    27,     0,     0,
      28,     0,     0,    29,    88,    30,    31,     0,    32,     0,
      33,    34,     0,     0,    35,     0,     0,     0,    36,    37,
      38,    39,    89,     0,    90,     0,     0,    40,    41,    42,
       0,    91,    44,     0,    45,    46,    47,    48,    49,    50,
       0,     0,     0,    69,    52,     0,    92,    53,     0,     0,
       0,    54,     0,     0,    55,    56,    93,    94,    58,    95,
      96,    78,   321,     0,     0,     0,    80,    81,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    17,    18,     0,     0,     0,
      19,     0,     0,    20,    83,    84,     0,     0,    22,     0,
       0,    85,    86,    87,     0,     0,    26,     0,    27,     0,
       0,    28,     0,     0,    29,    88,    30,    31,     0,    32,
       0,    33,    34,     0,     0,    35,     0,     0,     0,    36,
      37,    38,    39,    89,     0,    90,     0,     0,    40,    41,
      42,     0,    91,    44,     0,    45,    46,    47,    48,    49,
      50,     0,     0,     0,    69,    52,     0,    92,    53,     0,
       0,     0,    54,     0,     0,    55,    56,    93,    94,    58,
      95,    96,    78,     0,     0,     0,     0,    80,    81,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    17,    18,     0,     0,
       0,    19,     0,     0,    20,    83,    84,     0,     0,    22,
       0,     0,    85,    86,    87,     0,     0,    26,     0,    27,
       0,     0,    28,     0,     0,    29,    88,    30,    31,     0,
      32,     0,    33,    34,     0,     0,    35,     0,     0,     0,
      36,    37,    38,    39,    89,     0,    90,     0,     0,    40,
      41,    42,     0,    91,    44,     0,    45,    46,    47,    48,
      49,    50,   203,     0,     0,    69,    52,     0,    92,    53,
       0,     0,     0,    54,     0,     0,    55,    56,    93,    94,
      58,    95,    96,     0,     0,     0,    17,    18,     0,     0,
       0,    19,     0,     0,    20,     0,    21,     0,     0,    22,
       0,     0,    23,    24,    25,     0,     0,    26,     0,    27,
       0,     0,    28,     0,     0,    29,     0,    30,    31,     0,
      32,     0,    33,    34,     0,     0,    35,     0,     0,     0,
      36,    37,    38,    39,     0,     0,     0,     0,     0,    40,
      41,    42,     0,    43,    44,     0,    45,    46,    47,    48,
      49,    50,   247,     0,     0,    69,    52,     0,     0,    53,
       0,     0,     0,    54,     0,     0,    55,    56,     0,    57,
      58,     0,     0,     0,     0,     0,    17,    18,     0,     0,
       0,    19,     0,     0,    20,     0,    21,     0,     0,    22,
       0,     0,    23,    24,    25,     0,     0,    26,     0,    27,
       0,     0,    28,     0,     0,    29,     0,    30,    31,     0,
      32,     0,    33,    34,     0,     0,    35,     0,     0,     0,
      36,    37,    38,    39,     0,     0,     0,     0,     0,    40,
      41,    42,     0,    43,    44,     0,    45,    46,    47,    48,
      49,    50,   289,     0,     0,    69,    52,     0,     0,    53,
       0,     0,     0,    54,     0,     0,    55,    56,     0,    57,
      58,     0,     0,     0,     0,     0,    17,    18,     0,     0,
       0,    19,     0,     0,    20,     0,    21,     0,     0,    22,
       0,     0,    23,    24,    25,     0,     0,    26,     0,    27,
       0,     0,    28,     0,     0,    29,     0,    30,    31,     0,
      32,     0,    33,    34,     0,     0,    35,     0,     0,     0,
      36,    37,    38,    39,     0,     0,     0,     0,     0,    40,
      41,    42,     0,    43,    44,     0,    45,    46,    47,    48,
      49,    50,     0,     0,     0,    69,    52,     0,     0,    53,
       0,     0,     0,    54,     0,     0,    55,    56,     0,    57,
      58,   128,   129,   130,   131,     0,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
       0,     0,   146,     0,     0,     0,     0,   147,   128,   129,
     130,   131,   148,   132,   133,   134,   135,     0,     0,   138,
     139,   140,   141,     0,     0,   144,   145,     0,     0,     0,
       0,     0,     0,     0,   149,     0,     0,     0,   150,   148,
       0,     0,   151,   152,     0,   153,   154,     0,   155,   156,
       0,     0,   157,     0,   244,     0,     0,     0,     0,     0,
     158,     0,     0,     0,     0,     0,     0,   128,   129,   130,
     131,   221,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,     0,     0,   146,     0,
       0,     0,     0,   147,   128,   129,   130,   131,   148,   132,
     133,   134,   135,     0,     0,   138,   139,   140,   141,     0,
       0,   144,   145,     0,     0,     0,     0,     0,     0,     0,
     149,     0,     0,     0,   150,   148,     0,     0,   151,   152,
       0,   153,   154,     0,   155,   156,     0,     0,   157,     0,
     253,     0,    17,    18,     0,     0,   158,    19,     0,     0,
      20,     0,    21,     0,     0,    22,     0,   257,    23,    24,
      25,     0,     0,    26,     0,    27,     0,     0,    28,     0,
       0,    29,     0,    30,    31,     0,    32,     0,    33,    34,
       0,     0,    35,     0,     0,     0,    36,    37,    38,    39,
       0,     0,     0,     0,     0,    40,    41,    42,     0,    43,
      44,     0,    45,    46,    47,    48,    49,    50,     0,     0,
       0,    51,    52,     0,     0,    53,     0,     0,     0,    54,
       0,     0,    55,    56,     0,    57,    58,    17,    18,     0,
       0,     0,    19,     0,     0,    20,     0,    21,     0,     0,
      22,     0,     0,    23,    24,    25,     0,     0,    26,     0,
      27,     0,     0,    28,     0,     0,    29,     0,    30,    31,
       0,    32,     0,    33,    34,     0,     0,    35,     0,     0,
       0,    36,    37,    38,    39,     0,     0,     0,     0,     0,
      40,    41,    42,     0,    43,    44,     0,    45,    46,    47,
      48,    49,    50,     0,     0,     0,    69,    52,     0,     0,
      53,     0,     0,     0,    54,     0,     0,    55,    56,     0,
      57,    58,   128,   129,   130,   131,     0,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,     0,     0,   146,     0,     0,   159,     0,   147,   128,
     129,   130,   131,   148,   132,   133,   134,   135,     0,     0,
     138,   139,   140,   141,   160,     0,   144,   145,     0,     0,
       0,     0,     0,     0,     0,   149,     0,     0,     0,   150,
     148,     0,     0,   151,   152,     0,   153,   154,     0,   155,
     156,     0,     0,   157,     0,   277,     0,   128,   129,   130,
     131,   158,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,     0,     0,   146,     0,
     226,     0,     0,   147,   128,   129,   130,   131,   148,   132,
     133,   134,   135,     0,     0,   138,   139,   140,   141,     0,
       0,   144,   145,     0,     0,     0,     0,     0,     0,     0,
     149,     0,     0,     0,   150,   148,     0,     0,   151,   152,
       0,   153,   154,     0,   155,   156,     0,     0,   157,     0,
     283,     0,   128,   129,   130,   131,   158,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,     0,     0,   146,   235,     0,     0,     0,   147,   128,
     129,   130,   131,   148,   132,   133,   134,   135,     0,     0,
     138,   139,   140,   141,     0,     0,   144,   145,     0,     0,
       0,     0,     0,     0,     0,   149,     0,     0,     0,   150,
     148,     0,     0,   151,   152,     0,   153,   154,     0,   155,
     156,     0,     0,   157,     0,   284,     0,   128,   129,   130,
     131,   158,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,     0,     0,   146,     0,
       0,     0,     0,   147,     0,     0,     0,     0,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   256,     0,     0,     0,     0,     0,     0,     0,
     149,     0,     0,     0,   150,     0,     0,     0,   151,   152,
       0,   153,   154,     0,   155,   156,     0,     0,   157,     0,
       0,     0,   128,   129,   130,   131,   158,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,     0,     0,   146,   276,     0,     0,     0,   147,   128,
     129,   130,   131,   148,   132,   133,   134,   135,     0,     0,
     138,   139,   140,   141,     0,     0,   144,   145,     0,     0,
       0,     0,     0,     0,     0,   149,     0,     0,     0,   150,
     148,     0,     0,   151,   152,     0,   153,   154,     0,   155,
     156,     0,     0,   157,     0,   285,     0,   128,   129,   130,
     131,   158,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,     0,     0,   146,     0,
       0,     0,     0,   147,     0,     0,     0,     0,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   288,     0,     0,     0,     0,     0,     0,     0,
     149,     0,     0,     0,   150,     0,     0,     0,   151,   152,
       0,   153,   154,     0,   155,   156,     0,     0,   157,     0,
       0,     0,   128,   129,   130,   131,   158,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,     0,     0,   146,     0,     0,     0,     0,   147,   128,
     129,   130,   131,   148,   132,   133,   134,   135,     0,     0,
     138,   139,   140,   141,     0,     0,   144,   145,     0,     0,
       0,     0,     0,     0,     0,   149,     0,     0,     0,   150,
     148,     0,     0,   151,   152,     0,   153,   154,     0,   155,
     156,     0,     0,   157,     0,     0,     0,   128,   129,   130,
     131,   158,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,     0,     0,   146,     0,
       0,     0,     0,   147,     0,     0,     0,     0,   148,   128,
     129,   130,   131,     0,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,     0,     0,
     149,     0,     0,     0,   150,   147,     0,     0,   151,   152,
     148,   153,   154,     0,   155,   156,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   158,     0,     0,     0,
       0,     0,   149,     0,     0,     0,   150,     0,     0,     0,
     151,   152,     0,   153,   154,     0,   155,   156,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   158
  };

  const short
  parser::yycheck_[] =
  {
      14,    15,    42,    78,    14,    15,     0,    65,    66,    27,
      68,     8,     9,    52,    53,    95,     8,     9,    10,    11,
      16,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     7,    34,    29,    56,    66,
      52,    53,    34,    39,     3,    63,     5,    39,     4,   124,
       6,    65,    66,    67,    68,    65,    66,   101,    68,   108,
     104,    62,   111,    61,    75,    55,   105,    65,     4,    61,
       6,     5,    90,    65,    72,    73,     5,    69,    70,    77,
      72,    73,   103,    75,    76,    78,     4,    79,     6,    87,
      10,    11,   150,   105,    14,    87,    16,     3,     8,     9,
      10,    11,   177,    13,    14,    15,    16,     4,     3,     6,
       5,   125,   109,   105,     3,    25,    26,   106,     4,    39,
       6,     4,     3,     6,     5,     4,     3,     6,   203,    39,
       4,     4,     6,     6,   148,     4,   150,     6,     6,     3,
     150,     4,     4,     8,     9,    10,    11,     6,    13,    14,
      15,    16,     6,   211,    19,    20,    21,    22,   110,     5,
      25,    26,     4,     4,     6,     6,   241,    39,   226,   110,
     110,     4,   247,     3,    39,     4,     4,     8,     9,    10,
      11,     4,     4,    14,   242,    16,     4,     3,   109,    54,
     109,     4,   319,   120,   247,    12,    15,   211,   163,    -1,
      -1,   211,   260,    -1,    -1,    -1,   281,    -1,    39,    -1,
      -1,    -1,   226,    -1,    -1,    -1,   226,    -1,    -1,    -1,
     234,   296,    -1,    -1,   282,    -1,    -1,    -1,   242,    -1,
      77,    -1,   242,    80,    81,    82,    83,    -1,    -1,    -1,
     315,    -1,    89,    -1,    -1,    -1,   260,    -1,    -1,    -1,
     260,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   282,    -1,
      -1,   118,   282,    -1,   121,    -1,    -1,    -1,    -1,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,    -1,   149,    -1,   151,    -1,   153,   154,    -1,    -1,
     157,   158,    -1,    -1,    -1,    -1,   163,    -1,    -1,   166,
      -1,   168,    -1,     8,     9,    10,    11,    -1,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   210,    -1,   212,   213,    -1,   215,    54,
      -1,   218,    -1,    -1,   221,    -1,    -1,   224,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,
      -1,    -1,    -1,    -1,    -1,    -1,   243,   244,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   253,    -1,    -1,    -1,
     257,    -1,    -1,    -1,    -1,     3,     4,    -1,    -1,    -1,
       8,     9,    10,    -1,    12,    -1,    -1,    -1,    -1,   276,
     277,    -1,    -1,    -1,    -1,    -1,   283,   284,   285,    27,
      28,    -1,    -1,    -1,    32,    -1,    -1,    35,    36,    37,
      -1,    -1,    40,    -1,    -1,    43,    44,    45,    -1,    -1,
      48,    -1,    50,    51,    -1,    53,    -1,    -1,    56,    57,
      58,    59,    -1,    61,    -1,    63,    64,    -1,    -1,    67,
      -1,    -1,    -1,    71,    72,    73,    74,    75,    -1,    77,
      -1,    -1,    80,    81,    82,    -1,    84,    85,    -1,    87,
      88,    89,    90,    91,    92,    -1,    -1,    -1,    96,    97,
      -1,    99,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,
     108,   109,   110,   111,   112,   113,     3,     4,    -1,    -1,
      -1,     8,     9,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    32,    -1,    -1,    35,    36,
      37,    -1,    -1,    40,    -1,    -1,    43,    44,    45,    -1,
      -1,    48,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      57,    58,    59,    -1,    61,    -1,    63,    64,    -1,    -1,
      67,    -1,    -1,    -1,    71,    72,    73,    74,    75,    -1,
      77,    -1,    -1,    80,    81,    82,    -1,    84,    85,    -1,
      87,    88,    89,    90,    91,    92,    93,    -1,    -1,    96,
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
      -1,    87,    88,    89,    90,    91,    92,    93,    -1,    -1,
      96,    97,    -1,    99,   100,    -1,    -1,    -1,   104,    -1,
      -1,   107,   108,   109,   110,   111,   112,   113,     3,     4,
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
      -1,    -1,    -1,    -1,     8,     9,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    32,    -1,
      -1,    35,    36,    37,    -1,    -1,    40,    -1,    -1,    43,
      44,    45,    -1,    -1,    48,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    57,    58,    59,    -1,    61,    -1,    63,
      64,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    -1,    77,    -1,    -1,    80,    81,    82,    -1,
      84,    85,    -1,    87,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    96,    97,    -1,    99,   100,    -1,    -1,    -1,
     104,   105,    -1,   107,   108,   109,   110,   111,   112,   113,
       3,     4,    -1,    -1,    -1,     8,     9,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    32,
      -1,    -1,    35,    36,    37,    -1,    -1,    40,    -1,    -1,
      43,    44,    45,    -1,    -1,    48,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    57,    58,    59,    -1,    61,    -1,
      63,    64,    -1,    -1,    67,    -1,    -1,    -1,    71,    72,
      73,    74,    75,    -1,    77,    -1,    -1,    80,    81,    82,
      -1,    84,    85,    -1,    87,    88,    89,    90,    91,    92,
      -1,    -1,    -1,    96,    97,    -1,    99,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,   108,   109,   110,   111,   112,
     113,     3,     4,    -1,    -1,    -1,     8,     9,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
      32,    -1,    -1,    35,    36,    37,    -1,    -1,    40,    -1,
      -1,    43,    44,    45,    -1,    -1,    48,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    57,    58,    59,    -1,    61,
      -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,    71,
      72,    73,    74,    75,    -1,    77,    -1,    -1,    80,    81,
      82,    -1,    84,    85,    -1,    87,    88,    89,    90,    91,
      92,    -1,    -1,    -1,    96,    97,    -1,    99,   100,    -1,
      -1,    -1,   104,    -1,    -1,   107,   108,   109,   110,   111,
     112,   113,     3,    -1,    -1,    -1,    -1,     8,     9,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    32,    -1,    -1,    35,    36,    37,    -1,    -1,    40,
      -1,    -1,    43,    44,    45,    -1,    -1,    48,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,
      71,    72,    73,    74,    75,    -1,    77,    -1,    -1,    80,
      81,    82,    -1,    84,    85,    -1,    87,    88,    89,    90,
      91,    92,     3,    -1,    -1,    96,    97,    -1,    99,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,   108,   109,   110,
     111,   112,   113,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    32,    -1,    -1,    35,    -1,    37,    -1,    -1,    40,
      -1,    -1,    43,    44,    45,    -1,    -1,    48,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    58,    59,    -1,
      61,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    80,
      81,    82,    -1,    84,    85,    -1,    87,    88,    89,    90,
      91,    92,     3,    -1,    -1,    96,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,   110,
     111,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    32,    -1,    -1,    35,    -1,    37,    -1,    -1,    40,
      -1,    -1,    43,    44,    45,    -1,    -1,    48,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    58,    59,    -1,
      61,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    80,
      81,    82,    -1,    84,    85,    -1,    87,    88,    89,    90,
      91,    92,     3,    -1,    -1,    96,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,   110,
     111,    -1,    -1,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    32,    -1,    -1,    35,    -1,    37,    -1,    -1,    40,
      -1,    -1,    43,    44,    45,    -1,    -1,    48,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    58,    59,    -1,
      61,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,
      71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    80,
      81,    82,    -1,    84,    85,    -1,    87,    88,    89,    90,
      91,    92,    -1,    -1,    -1,    96,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,   110,
     111,     8,     9,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    29,    -1,    -1,    -1,    -1,    34,     8,     9,
      10,    11,    39,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    -1,    -1,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,    39,
      -1,    -1,    69,    70,    -1,    72,    73,    -1,    75,    76,
      -1,    -1,    79,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,     8,     9,    10,
      11,    98,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    34,     8,     9,    10,    11,    39,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    65,    39,    -1,    -1,    69,    70,
      -1,    72,    73,    -1,    75,    76,    -1,    -1,    79,    -1,
      54,    -1,    27,    28,    -1,    -1,    87,    32,    -1,    -1,
      35,    -1,    37,    -1,    -1,    40,    -1,    98,    43,    44,
      45,    -1,    -1,    48,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    58,    59,    -1,    61,    -1,    63,    64,
      -1,    -1,    67,    -1,    -1,    -1,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    80,    81,    82,    -1,    84,
      85,    -1,    87,    88,    89,    90,    91,    92,    -1,    -1,
      -1,    96,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,
      -1,    -1,   107,   108,    -1,   110,   111,    27,    28,    -1,
      -1,    -1,    32,    -1,    -1,    35,    -1,    37,    -1,    -1,
      40,    -1,    -1,    43,    44,    45,    -1,    -1,    48,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    58,    59,
      -1,    61,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,
      -1,    71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      80,    81,    82,    -1,    84,    85,    -1,    87,    88,    89,
      90,    91,    92,    -1,    -1,    -1,    96,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,   108,    -1,
     110,   111,     8,     9,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    29,    -1,    -1,    32,    -1,    34,     8,
       9,    10,    11,    39,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    50,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,
      39,    -1,    -1,    69,    70,    -1,    72,    73,    -1,    75,
      76,    -1,    -1,    79,    -1,    54,    -1,     8,     9,    10,
      11,    87,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    -1,
      31,    -1,    -1,    34,     8,     9,    10,    11,    39,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    -1,
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    65,    39,    -1,    -1,    69,    70,
      -1,    72,    73,    -1,    75,    76,    -1,    -1,    79,    -1,
      54,    -1,     8,     9,    10,    11,    87,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    29,    30,    -1,    -1,    -1,    34,     8,
       9,    10,    11,    39,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,
      39,    -1,    -1,    69,    70,    -1,    72,    73,    -1,    75,
      76,    -1,    -1,    79,    -1,    54,    -1,     8,     9,    10,
      11,    87,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    65,    -1,    -1,    -1,    69,    70,
      -1,    72,    73,    -1,    75,    76,    -1,    -1,    79,    -1,
      -1,    -1,     8,     9,    10,    11,    87,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    29,    30,    -1,    -1,    -1,    34,     8,
       9,    10,    11,    39,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,
      39,    -1,    -1,    69,    70,    -1,    72,    73,    -1,    75,
      76,    -1,    -1,    79,    -1,    54,    -1,     8,     9,    10,
      11,    87,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    65,    -1,    -1,    -1,    69,    70,
      -1,    72,    73,    -1,    75,    76,    -1,    -1,    79,    -1,
      -1,    -1,     8,     9,    10,    11,    87,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    29,    -1,    -1,    -1,    -1,    34,     8,
       9,    10,    11,    39,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    65,
      39,    -1,    -1,    69,    70,    -1,    72,    73,    -1,    75,
      76,    -1,    -1,    79,    -1,    -1,    -1,     8,     9,    10,
      11,    87,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    39,     8,
       9,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      61,    -1,    -1,    -1,    65,    34,    -1,    -1,    69,    70,
      39,    72,    73,    -1,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      69,    70,    -1,    72,    73,    -1,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    42,   115,   116,   145,   147,   101,   104,   140,     0,
       7,    95,    66,    62,   138,   138,    75,    27,    28,    32,
      35,    37,    40,    43,    44,    45,    48,    50,    53,    56,
      58,    59,    61,    63,    64,    67,    71,    72,    73,    74,
      80,    81,    82,    84,    85,    87,    88,    89,    90,    91,
      92,    96,    97,   100,   104,   107,   108,   110,   111,   118,
     119,   120,   142,   142,    55,     5,     5,   103,    78,    96,
     118,   120,   120,   118,   120,     3,   146,     3,     3,     4,
       8,     9,    12,    36,    37,    43,    44,    45,    57,    75,
      77,    84,    99,   109,   110,   112,   113,   117,   118,   119,
     125,   126,   127,   128,   129,   130,   131,   132,   134,   135,
     136,   136,   143,   144,   128,   136,   136,   136,   105,   133,
     136,     3,   136,     3,     3,     5,     4,     6,     8,     9,
      10,    11,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    29,    34,    39,    61,
      65,    69,    70,    72,    73,    75,    76,    79,    87,    32,
      50,   139,     4,     6,     4,   136,    52,    53,   105,   133,
     136,    27,    56,    63,    90,     4,    10,    51,   128,   118,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   118,   136,     3,   118,   120,   136,   136,   136,    34,
      61,    65,    72,    73,    77,    87,   136,   136,   106,   141,
     143,    98,   136,   136,    52,    53,    31,     6,     6,     4,
       6,     4,   128,     4,     5,    30,    54,     4,    93,   128,
     137,     3,     5,    54,    54,   136,   136,     3,   118,   120,
     136,   136,   136,    54,   136,   136,    53,    98,   136,   120,
     123,   124,   110,   110,   110,     4,   118,   136,   136,     4,
       4,   128,   118,   120,   136,   136,    30,    54,     4,   128,
     137,     3,     5,    54,    54,    54,   136,   136,    53,     3,
     120,     4,     4,     4,     4,     4,     3,   136,   136,     4,
       4,   128,   118,   120,   136,   136,   136,     8,     9,   109,
     121,   122,     4,   128,     4,     3,   109,   109,     4,     6,
       4,     4,   128,   122,     4,     4
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   114,   115,   115,   116,   116,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   118,   118,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   120,   120,   120,   120,   120,   120,   121,
     121,   122,   122,   123,   123,   124,   124,   124,   125,   125,
     125,   125,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   128,
     128,   129,   129,   129,   129,   130,   130,   130,   131,   131,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   133,   133,   134,   134,
     134,   134,   135,   135,   135,   135,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   137,   138,   138,   139,   139,   139,
     140,   140,   141,   141,   142,   142,   142,   143,   144,   144,
     145,   146,   146,   146,   147
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     2,     1,     4,     6,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     4,
       4,     5,     5,     5,     5,     6,     6,     6,     6,     1,
       3,     4,     5,     3,     4,     2,     2,     3,     5,     6,
       4,     5,     5,     5,     3,     7,     8,     6,     5,     6,
       6,     6,     4,     8,     9,     7,     4,     5,     6,     4,
       5,     3,     4,     6,     6,     6,     1,     1,     1,     5,
       3,     1,     1,     1,     1,     3,     6,     3,     1,     1,
       1,     1,     1,     1,     1,     0,     3,     0,     1,     1,
       0,     1,     0,     2,     3,     3,     1,     2,     1,     3,
      11,     0,     2,     3,     8
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
  "signednumber", "signednumber_or_numeric", "typename_namelist",
  "type_name", "unary_expr", "binary_expr", "like_expr", "exprlist_expr",
  "function_expr", "isnull_expr", "between_expr", "in_expr",
  "whenthenlist_expr", "case_expr", "raise_expr", "expr", "select_stmt",
  "optional_if_not_exists", "optional_sort_order", "optional_unique",
  "optional_where", "tableid_with_uninteresting_schema", "indexed_column",
  "indexed_column_list", "createindex_stmt",
  "optional_exprlist_with_paren", "createvirtualtable_stmt", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short
  parser::yyrline_[] =
  {
       0,   225,   225,   226,   230,   231,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   251,   252,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   296,   297,   298,   299,   300,   301,   315,
     316,   320,   321,   325,   326,   330,   331,   332,   336,   337,
     338,   339,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   386,
     387,   391,   392,   393,   394,   398,   399,   400,   404,   405,
     409,   410,   411,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   424,   428,   429,   433,   434,
     435,   436,   440,   441,   442,   443,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   473,   481,   482,   486,   487,   488,
     496,   497,   501,   502,   506,   507,   508,   512,   526,   527,
     531,   542,   543,   544,   548
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
#line 3294 "sqlite3_parser.cpp"

#line 555 "sqlite3_parser.yy"


void sqlb::parser::parser::error(const location_type& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}
