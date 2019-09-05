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
#line 228 "sqlite3_parser.yy"
    { drv.result = yystack_[0].value.as < sqlb::IndexPtr > (); }
#line 1098 "sqlite3_parser.cpp"
    break;

  case 5:
#line 236 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1104 "sqlite3_parser.cpp"
    break;

  case 6:
#line 237 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1110 "sqlite3_parser.cpp"
    break;

  case 7:
#line 238 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1116 "sqlite3_parser.cpp"
    break;

  case 8:
#line 239 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1122 "sqlite3_parser.cpp"
    break;

  case 9:
#line 240 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1128 "sqlite3_parser.cpp"
    break;

  case 10:
#line 241 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1134 "sqlite3_parser.cpp"
    break;

  case 11:
#line 242 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1140 "sqlite3_parser.cpp"
    break;

  case 12:
#line 243 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1146 "sqlite3_parser.cpp"
    break;

  case 13:
#line 244 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1152 "sqlite3_parser.cpp"
    break;

  case 14:
#line 248 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1158 "sqlite3_parser.cpp"
    break;

  case 15:
#line 249 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1164 "sqlite3_parser.cpp"
    break;

  case 16:
#line 254 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1170 "sqlite3_parser.cpp"
    break;

  case 17:
#line 255 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1176 "sqlite3_parser.cpp"
    break;

  case 18:
#line 256 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1182 "sqlite3_parser.cpp"
    break;

  case 19:
#line 257 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1188 "sqlite3_parser.cpp"
    break;

  case 20:
#line 258 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1194 "sqlite3_parser.cpp"
    break;

  case 21:
#line 259 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1200 "sqlite3_parser.cpp"
    break;

  case 22:
#line 260 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1206 "sqlite3_parser.cpp"
    break;

  case 23:
#line 261 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1212 "sqlite3_parser.cpp"
    break;

  case 24:
#line 262 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1218 "sqlite3_parser.cpp"
    break;

  case 25:
#line 263 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1224 "sqlite3_parser.cpp"
    break;

  case 26:
#line 264 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1230 "sqlite3_parser.cpp"
    break;

  case 27:
#line 265 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1236 "sqlite3_parser.cpp"
    break;

  case 28:
#line 266 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1242 "sqlite3_parser.cpp"
    break;

  case 29:
#line 267 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1248 "sqlite3_parser.cpp"
    break;

  case 30:
#line 268 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1254 "sqlite3_parser.cpp"
    break;

  case 31:
#line 269 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1260 "sqlite3_parser.cpp"
    break;

  case 32:
#line 270 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1266 "sqlite3_parser.cpp"
    break;

  case 33:
#line 271 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1272 "sqlite3_parser.cpp"
    break;

  case 34:
#line 272 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1278 "sqlite3_parser.cpp"
    break;

  case 35:
#line 273 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1284 "sqlite3_parser.cpp"
    break;

  case 36:
#line 274 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1290 "sqlite3_parser.cpp"
    break;

  case 37:
#line 275 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1296 "sqlite3_parser.cpp"
    break;

  case 38:
#line 276 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1302 "sqlite3_parser.cpp"
    break;

  case 39:
#line 277 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1308 "sqlite3_parser.cpp"
    break;

  case 40:
#line 278 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1314 "sqlite3_parser.cpp"
    break;

  case 41:
#line 279 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1320 "sqlite3_parser.cpp"
    break;

  case 42:
#line 280 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1326 "sqlite3_parser.cpp"
    break;

  case 43:
#line 281 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1332 "sqlite3_parser.cpp"
    break;

  case 44:
#line 282 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1338 "sqlite3_parser.cpp"
    break;

  case 45:
#line 283 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1344 "sqlite3_parser.cpp"
    break;

  case 46:
#line 284 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1350 "sqlite3_parser.cpp"
    break;

  case 47:
#line 285 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1356 "sqlite3_parser.cpp"
    break;

  case 48:
#line 286 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1362 "sqlite3_parser.cpp"
    break;

  case 49:
#line 287 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1368 "sqlite3_parser.cpp"
    break;

  case 50:
#line 288 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1374 "sqlite3_parser.cpp"
    break;

  case 51:
#line 289 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1380 "sqlite3_parser.cpp"
    break;

  case 52:
#line 293 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1386 "sqlite3_parser.cpp"
    break;

  case 53:
#line 294 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1392 "sqlite3_parser.cpp"
    break;

  case 54:
#line 295 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1398 "sqlite3_parser.cpp"
    break;

  case 55:
#line 296 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1404 "sqlite3_parser.cpp"
    break;

  case 56:
#line 297 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1410 "sqlite3_parser.cpp"
    break;

  case 57:
#line 298 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = unquote_text(yystack_[0].value.as < std::string > (), '\''); }
#line 1416 "sqlite3_parser.cpp"
    break;

  case 58:
#line 312 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1422 "sqlite3_parser.cpp"
    break;

  case 59:
#line 313 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1428 "sqlite3_parser.cpp"
    break;

  case 60:
#line 317 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1434 "sqlite3_parser.cpp"
    break;

  case 61:
#line 318 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1440 "sqlite3_parser.cpp"
    break;

  case 62:
#line 322 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1446 "sqlite3_parser.cpp"
    break;

  case 63:
#line 323 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " " + yystack_[0].value.as < std::string > (); }
#line 1452 "sqlite3_parser.cpp"
    break;

  case 64:
#line 327 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1458 "sqlite3_parser.cpp"
    break;

  case 65:
#line 328 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1464 "sqlite3_parser.cpp"
    break;

  case 66:
#line 329 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + "(" + yystack_[3].value.as < std::string > () + ", " + yystack_[1].value.as < std::string > () + ")"; }
#line 1470 "sqlite3_parser.cpp"
    break;

  case 67:
#line 333 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "-" + yystack_[0].value.as < std::string > (); }
#line 1476 "sqlite3_parser.cpp"
    break;

  case 68:
#line 334 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "+" + yystack_[0].value.as < std::string > (); }
#line 1482 "sqlite3_parser.cpp"
    break;

  case 69:
#line 335 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "~" + yystack_[0].value.as < std::string > (); }
#line 1488 "sqlite3_parser.cpp"
    break;

  case 70:
#line 336 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "NOT " + yystack_[0].value.as < std::string > (); }
#line 1494 "sqlite3_parser.cpp"
    break;

  case 71:
#line 340 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " || " + yystack_[0].value.as < std::string > (); }
#line 1500 "sqlite3_parser.cpp"
    break;

  case 72:
#line 341 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " * " + yystack_[0].value.as < std::string > (); }
#line 1506 "sqlite3_parser.cpp"
    break;

  case 73:
#line 342 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " / " + yystack_[0].value.as < std::string > (); }
#line 1512 "sqlite3_parser.cpp"
    break;

  case 74:
#line 343 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " % " + yystack_[0].value.as < std::string > (); }
#line 1518 "sqlite3_parser.cpp"
    break;

  case 75:
#line 344 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " + " + yystack_[0].value.as < std::string > (); }
#line 1524 "sqlite3_parser.cpp"
    break;

  case 76:
#line 345 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " - " + yystack_[0].value.as < std::string > (); }
#line 1530 "sqlite3_parser.cpp"
    break;

  case 77:
#line 346 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " << " + yystack_[0].value.as < std::string > (); }
#line 1536 "sqlite3_parser.cpp"
    break;

  case 78:
#line 347 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >> " + yystack_[0].value.as < std::string > (); }
#line 1542 "sqlite3_parser.cpp"
    break;

  case 79:
#line 348 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " & " + yystack_[0].value.as < std::string > (); }
#line 1548 "sqlite3_parser.cpp"
    break;

  case 80:
#line 349 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " | " + yystack_[0].value.as < std::string > (); }
#line 1554 "sqlite3_parser.cpp"
    break;

  case 81:
#line 350 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " < " + yystack_[0].value.as < std::string > (); }
#line 1560 "sqlite3_parser.cpp"
    break;

  case 82:
#line 351 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <= " + yystack_[0].value.as < std::string > (); }
#line 1566 "sqlite3_parser.cpp"
    break;

  case 83:
#line 352 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " > " + yystack_[0].value.as < std::string > (); }
#line 1572 "sqlite3_parser.cpp"
    break;

  case 84:
#line 353 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " >= " + yystack_[0].value.as < std::string > (); }
#line 1578 "sqlite3_parser.cpp"
    break;

  case 85:
#line 354 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " = " + yystack_[0].value.as < std::string > (); }
#line 1584 "sqlite3_parser.cpp"
    break;

  case 86:
#line 355 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " == " + yystack_[0].value.as < std::string > (); }
#line 1590 "sqlite3_parser.cpp"
    break;

  case 87:
#line 356 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " != " + yystack_[0].value.as < std::string > (); }
#line 1596 "sqlite3_parser.cpp"
    break;

  case 88:
#line 357 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " <> " + yystack_[0].value.as < std::string > (); }
#line 1602 "sqlite3_parser.cpp"
    break;

  case 89:
#line 358 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IS " + yystack_[0].value.as < std::string > (); }
#line 1608 "sqlite3_parser.cpp"
    break;

  case 90:
#line 359 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 1614 "sqlite3_parser.cpp"
    break;

  case 91:
#line 360 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " OR " + yystack_[0].value.as < std::string > (); }
#line 1620 "sqlite3_parser.cpp"
    break;

  case 92:
#line 364 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " LIKE " + yystack_[0].value.as < std::string > (); }
#line 1626 "sqlite3_parser.cpp"
    break;

  case 93:
#line 365 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " GLOB " + yystack_[0].value.as < std::string > (); }
#line 1632 "sqlite3_parser.cpp"
    break;

  case 94:
#line 366 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " MATCH " + yystack_[0].value.as < std::string > (); }
#line 1638 "sqlite3_parser.cpp"
    break;

  case 95:
#line 367 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " REGEXP " + yystack_[0].value.as < std::string > (); }
#line 1644 "sqlite3_parser.cpp"
    break;

  case 96:
#line 368 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT LIKE " + yystack_[0].value.as < std::string > (); }
#line 1650 "sqlite3_parser.cpp"
    break;

  case 97:
#line 369 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT GLOB " + yystack_[0].value.as < std::string > (); }
#line 1656 "sqlite3_parser.cpp"
    break;

  case 98:
#line 370 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT MATCH " + yystack_[0].value.as < std::string > (); }
#line 1662 "sqlite3_parser.cpp"
    break;

  case 99:
#line 371 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT REGEXP " + yystack_[0].value.as < std::string > (); }
#line 1668 "sqlite3_parser.cpp"
    break;

  case 100:
#line 372 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " LIKE " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1674 "sqlite3_parser.cpp"
    break;

  case 101:
#line 373 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " GLOB " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1680 "sqlite3_parser.cpp"
    break;

  case 102:
#line 374 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " MATCH " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1686 "sqlite3_parser.cpp"
    break;

  case 103:
#line 375 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " REGEXP " + yystack_[2].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1692 "sqlite3_parser.cpp"
    break;

  case 104:
#line 376 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT LIKE " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1698 "sqlite3_parser.cpp"
    break;

  case 105:
#line 377 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT GLOB " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1704 "sqlite3_parser.cpp"
    break;

  case 106:
#line 378 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT MATCH " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1710 "sqlite3_parser.cpp"
    break;

  case 107:
#line 379 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT REGEXP " + yystack_[3].value.as < std::string > () + " ESCAPE " + yystack_[0].value.as < std::string > (); }
#line 1716 "sqlite3_parser.cpp"
    break;

  case 108:
#line 383 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1722 "sqlite3_parser.cpp"
    break;

  case 109:
#line 384 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + ", " + yystack_[0].value.as < std::string > (); }
#line 1728 "sqlite3_parser.cpp"
    break;

  case 110:
#line 388 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1734 "sqlite3_parser.cpp"
    break;

  case 111:
#line 389 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + "(DISTINCT " + yystack_[1].value.as < std::string > () + ")"; }
#line 1740 "sqlite3_parser.cpp"
    break;

  case 112:
#line 390 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + "()"; }
#line 1746 "sqlite3_parser.cpp"
    break;

  case 113:
#line 391 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + "(*)"; }
#line 1752 "sqlite3_parser.cpp"
    break;

  case 114:
#line 395 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " ISNULL"; }
#line 1758 "sqlite3_parser.cpp"
    break;

  case 115:
#line 396 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + " NOTNULL"; }
#line 1764 "sqlite3_parser.cpp"
    break;

  case 116:
#line 397 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " NOT NULL"; }
#line 1770 "sqlite3_parser.cpp"
    break;

  case 117:
#line 401 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 1776 "sqlite3_parser.cpp"
    break;

  case 118:
#line 402 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT BETWEEN " + yystack_[2].value.as < std::string > () + " AND " + yystack_[0].value.as < std::string > (); }
#line 1782 "sqlite3_parser.cpp"
    break;

  case 119:
#line 406 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " IN ()"; }
#line 1788 "sqlite3_parser.cpp"
    break;

  case 120:
#line 407 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 1794 "sqlite3_parser.cpp"
    break;

  case 121:
#line 408 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 1800 "sqlite3_parser.cpp"
    break;

  case 122:
#line 409 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1806 "sqlite3_parser.cpp"
    break;

  case 123:
#line 410 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1812 "sqlite3_parser.cpp"
    break;

  case 124:
#line 411 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 1818 "sqlite3_parser.cpp"
    break;

  case 125:
#line 412 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1824 "sqlite3_parser.cpp"
    break;

  case 126:
#line 413 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1830 "sqlite3_parser.cpp"
    break;

  case 127:
#line 414 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " NOT IN ()"; }
#line 1836 "sqlite3_parser.cpp"
    break;

  case 128:
#line 415 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 1842 "sqlite3_parser.cpp"
    break;

  case 129:
#line 416 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN (" + yystack_[1].value.as < std::string > () + ")"; }
#line 1848 "sqlite3_parser.cpp"
    break;

  case 130:
#line 417 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[5].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1854 "sqlite3_parser.cpp"
    break;

  case 131:
#line 418 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1860 "sqlite3_parser.cpp"
    break;

  case 132:
#line 419 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[7].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + yystack_[2].value.as < std::string > () + "()"; }
#line 1866 "sqlite3_parser.cpp"
    break;

  case 133:
#line 420 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[8].value.as < std::string > () + " NOT IN " + sqlb::escapeIdentifier(yystack_[5].value.as < std::string > ()) + "." + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1872 "sqlite3_parser.cpp"
    break;

  case 134:
#line 421 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[6].value.as < std::string > () + " NOT IN " + yystack_[3].value.as < std::string > () + "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1878 "sqlite3_parser.cpp"
    break;

  case 135:
#line 425 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "WHEN " + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 1884 "sqlite3_parser.cpp"
    break;

  case 136:
#line 426 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[4].value.as < std::string > () + " WHEN" + yystack_[2].value.as < std::string > () + " THEN " + yystack_[0].value.as < std::string > (); }
#line 1890 "sqlite3_parser.cpp"
    break;

  case 137:
#line 430 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CASE " + yystack_[4].value.as < std::string > () + " " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 1896 "sqlite3_parser.cpp"
    break;

  case 138:
#line 431 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CASE " + yystack_[2].value.as < std::string > () + " " + yystack_[1].value.as < std::string > () + " END"; }
#line 1902 "sqlite3_parser.cpp"
    break;

  case 139:
#line 432 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CASE " + yystack_[3].value.as < std::string > () + " ELSE " + yystack_[1].value.as < std::string > () + " END"; }
#line 1908 "sqlite3_parser.cpp"
    break;

  case 140:
#line 433 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CASE " + yystack_[1].value.as < std::string > () + " END"; }
#line 1914 "sqlite3_parser.cpp"
    break;

  case 141:
#line 437 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "RAISE(IGNORE)"; }
#line 1920 "sqlite3_parser.cpp"
    break;

  case 142:
#line 438 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "RAISE(ROLLBACK, " + yystack_[1].value.as < std::string > () + ")"; }
#line 1926 "sqlite3_parser.cpp"
    break;

  case 143:
#line 439 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "RAISE(ABORT, " + yystack_[1].value.as < std::string > () + ")"; }
#line 1932 "sqlite3_parser.cpp"
    break;

  case 144:
#line 440 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "RAISE(FAIL, " + yystack_[1].value.as < std::string > () + ")"; }
#line 1938 "sqlite3_parser.cpp"
    break;

  case 145:
#line 444 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1944 "sqlite3_parser.cpp"
    break;

  case 146:
#line 445 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1950 "sqlite3_parser.cpp"
    break;

  case 147:
#line 446 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1956 "sqlite3_parser.cpp"
    break;

  case 148:
#line 447 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[4].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1962 "sqlite3_parser.cpp"
    break;

  case 149:
#line 448 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[2].value.as < std::string > ()) + "." + sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1968 "sqlite3_parser.cpp"
    break;

  case 150:
#line 449 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = sqlb::escapeIdentifier(yystack_[0].value.as < std::string > ()); }
#line 1974 "sqlite3_parser.cpp"
    break;

  case 151:
#line 450 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1980 "sqlite3_parser.cpp"
    break;

  case 152:
#line 451 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1986 "sqlite3_parser.cpp"
    break;

  case 153:
#line 452 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1992 "sqlite3_parser.cpp"
    break;

  case 154:
#line 453 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1998 "sqlite3_parser.cpp"
    break;

  case 155:
#line 454 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "CAST(" + yystack_[3].value.as < std::string > () + " AS " + yystack_[1].value.as < std::string > () + ")"; }
#line 2004 "sqlite3_parser.cpp"
    break;

  case 156:
#line 455 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + " COLLATE " + yystack_[0].value.as < std::string > (); }
#line 2010 "sqlite3_parser.cpp"
    break;

  case 157:
#line 456 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2016 "sqlite3_parser.cpp"
    break;

  case 158:
#line 457 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2022 "sqlite3_parser.cpp"
    break;

  case 159:
#line 458 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2028 "sqlite3_parser.cpp"
    break;

  case 160:
#line 459 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2034 "sqlite3_parser.cpp"
    break;

  case 161:
#line 460 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2040 "sqlite3_parser.cpp"
    break;

  case 162:
#line 461 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2046 "sqlite3_parser.cpp"
    break;

  case 163:
#line 470 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "SELECT"; }
#line 2052 "sqlite3_parser.cpp"
    break;

  case 164:
#line 478 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = false; }
#line 2058 "sqlite3_parser.cpp"
    break;

  case 165:
#line 479 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = true; }
#line 2064 "sqlite3_parser.cpp"
    break;

  case 166:
#line 483 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = ""; }
#line 2070 "sqlite3_parser.cpp"
    break;

  case 167:
#line 484 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "ASC"; }
#line 2076 "sqlite3_parser.cpp"
    break;

  case 168:
#line 485 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = "DESC"; }
#line 2082 "sqlite3_parser.cpp"
    break;

  case 169:
#line 493 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = false; }
#line 2088 "sqlite3_parser.cpp"
    break;

  case 170:
#line 494 "sqlite3_parser.yy"
    { yylhs.value.as < bool > () = true; }
#line 2094 "sqlite3_parser.cpp"
    break;

  case 171:
#line 498 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = ""; }
#line 2100 "sqlite3_parser.cpp"
    break;

  case 172:
#line 499 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2106 "sqlite3_parser.cpp"
    break;

  case 173:
#line 503 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2112 "sqlite3_parser.cpp"
    break;

  case 174:
#line 504 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2118 "sqlite3_parser.cpp"
    break;

  case 175:
#line 505 "sqlite3_parser.yy"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2124 "sqlite3_parser.cpp"
    break;

  case 176:
#line 509 "sqlite3_parser.yy"
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
#line 2140 "sqlite3_parser.cpp"
    break;

  case 177:
#line 523 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::IndexedColumnVector > () = sqlb::IndexedColumnVector(1, yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2146 "sqlite3_parser.cpp"
    break;

  case 178:
#line 524 "sqlite3_parser.yy"
    { yylhs.value.as < sqlb::IndexedColumnVector > () = yystack_[2].value.as < sqlb::IndexedColumnVector > (); yylhs.value.as < sqlb::IndexedColumnVector > ().push_back(yystack_[0].value.as < sqlb::IndexedColumn > ()); }
#line 2152 "sqlite3_parser.cpp"
    break;

  case 179:
#line 528 "sqlite3_parser.yy"
    {
													yylhs.value.as < sqlb::IndexPtr > () = sqlb::IndexPtr(new sqlb::Index(yystack_[6].value.as < std::string > ()));
													yylhs.value.as < sqlb::IndexPtr > ()->setTable(yystack_[4].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setUnique(yystack_[9].value.as < bool > ());
													yylhs.value.as < sqlb::IndexPtr > ()->setWhereExpr(yystack_[0].value.as < std::string > ());
													yylhs.value.as < sqlb::IndexPtr > ()->fields = yystack_[2].value.as < sqlb::IndexedColumnVector > ();
													yylhs.value.as < sqlb::IndexPtr > ()->setFullyParsed(true);
												}
#line 2165 "sqlite3_parser.cpp"
    break;


#line 2169 "sqlite3_parser.cpp"

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


  const signed char parser::yypact_ninf_ = -117;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -30,   -64,    38,    37,  -117,  -117,   -21,  -117,  -117,     5,
       2,  1582,     8,  -117,  -117,  -117,  -117,  -117,  -117,  -117,
    -117,  -117,  -117,  -117,  -117,  -117,  -117,  -117,  -117,  -117,
    -117,  -117,  -117,  -117,  -117,  -117,  -117,  -117,  -117,  -117,
    -117,  -117,  -117,  -117,  -117,  -117,  -117,    76,  -117,  -117,
    -117,  -117,  -117,  -117,  -117,    84,  -117,  -117,    26,  -117,
    1667,  1667,  1667,  -117,  -117,  -117,  -117,   121,  1066,  1066,
    1066,  1066,  1066,   733,   123,  -117,  -117,  -117,  -117,  1066,
    -117,   124,  -117,  -117,  -117,  -117,  -117,  -117,    28,  -117,
    -117,  -117,  -117,  -117,  -117,  -117,  -117,  -117,  -117,  1771,
    -117,    52,    56,  2221,  -117,  -117,  -117,  1066,   -17,     0,
    1066,  2328,     1,   400,   -81,  1066,  1066,  1066,  1066,  1066,
    1066,  1066,  1066,  1066,  1066,  1066,  1066,  1066,  1066,  1066,
    1066,  1066,  1066,  1066,  -117,  1066,   -81,  -117,  1066,  1156,
    1066,  -117,  1066,  1066,    13,  -117,  1066,  1066,  -117,     4,
    1066,  -117,  1066,  1440,  1066,  -117,  1066,   -12,  1846,   126,
     137,   144,   143,  -117,   148,  1066,    62,   164,    32,    32,
      16,    16,    98,    16,    98,   114,  2098,  2098,   321,   321,
     321,   321,  2098,  2098,    98,    98,  2328,  1921,  -117,   125,
     511,    77,  -117,  2098,   146,   302,  1066,  1066,  1246,  1066,
    1066,  -117,  1066,  2296,  1467,  1066,  -117,  -117,  2221,  1066,
    1996,  1526,  1066,  -117,  1667,    20,    60,  -117,    63,  -117,
      88,  -117,   -81,  1066,  1066,  -117,  -117,    91,   171,  1066,
    1667,  1066,  1066,  2071,  1553,   622,    93,  -117,  1798,  1873,
    1948,  1066,  2221,  2221,  -117,  1066,  2146,  -117,  1336,   172,
     173,   176,   177,  -117,  -117,  2098,  2098,  -117,  -117,    95,
     179,  -117,  2098,  2098,  1066,  1066,  -117,   107,   180,  1066,
    1667,  1066,  1066,  1066,  2098,  2221,  -117,    -7,  -117,  -117,
    -117,  -117,  -117,  -117,   844,  2098,  2098,  -117,  -117,   111,
     183,  -117,  2098,  2098,  2098,    74,    79,  -117,  -117,   112,
    -117,   115,  -117,   955,  -117,  -117,  -117,    -7,  -117,  -117,
     116,   185,  -117,  -117
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,   169,     0,     2,     4,   170,     0,     1,     3,   164,
       0,     0,     0,    16,    17,    18,    19,    20,    21,    54,
      53,    55,    22,    23,    24,    25,    26,    27,    28,    31,
      33,    32,    29,    30,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    48,    47,    49,
      50,    51,    14,    57,    15,    56,    52,   175,     0,   165,
       0,     0,     0,    48,    56,   174,   173,     0,     0,     0,
       0,     0,     0,     0,    20,    12,    11,    13,    10,     0,
       8,    39,     9,     5,     6,     7,   147,   145,   150,   146,
     151,   152,   157,   153,   158,   159,   160,   161,   162,   166,
     177,     0,     0,   108,    68,    67,    69,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,     0,   168,     0,     0,
       0,   114,     0,     0,     0,   115,     0,     0,   176,   171,
       0,   154,     0,     0,     0,   140,     0,     0,     0,     0,
       0,     0,     0,   112,     0,     0,     0,   149,    75,    76,
      72,    73,    79,    74,    80,    71,    85,    86,    83,    84,
      81,    82,    87,    88,    77,    78,    90,     0,   156,    93,
       0,    56,   123,    89,    92,    94,     0,     0,     0,     0,
       0,   116,     0,    91,    95,     0,   179,   178,   109,     0,
       0,     0,     0,   138,     0,     0,     0,   141,     0,   113,
       0,   110,     0,     0,     0,   119,   163,     0,     0,     0,
       0,     0,     0,     0,    97,     0,    56,   131,    96,    98,
      99,     0,   172,   135,   139,     0,     0,    62,    64,     0,
       0,     0,     0,   111,   148,   117,   101,   121,   120,     0,
      56,   122,   100,   102,     0,     0,   127,     0,     0,     0,
       0,     0,     0,     0,   103,   136,   137,     0,    63,   155,
     143,   144,   142,   126,     0,   118,   105,   129,   128,     0,
      56,   130,   104,   106,   107,     0,     0,    61,    60,     0,
     124,     0,   134,     0,    58,    59,    65,     0,   125,   132,
       0,     0,   133,    66
  };

  const short
  parser::yypgoto_[] =
  {
    -117,  -117,  -117,  -117,   -11,    -8,   -56,  -117,  -116,  -117,
    -117,  -117,  -117,  -117,  -106,  -117,  -117,  -117,  -117,    85,
    -117,  -117,   153,   -42,  -117,  -117,  -117,  -117,  -117,    45,
    -117,  -117
  };

  const short
  parser::yydefgoto_[] =
  {
      -1,     2,     3,    87,    88,    89,    57,   298,   299,   248,
     249,    90,    91,    92,   102,    93,    94,    95,    96,   108,
      97,    98,   103,   228,    11,   148,     6,   206,    58,   100,
     101,     4
  };

  const unsigned short
  parser::yytable_[] =
  {
      55,   295,   296,    56,    65,    66,    67,   166,   115,   116,
     117,   118,     1,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,    52,   159,   133,
      54,   113,   122,   114,   135,   154,   155,     5,     7,   136,
     212,   213,   117,   118,     8,     9,   120,   196,   122,    64,
      64,    64,    56,    56,    56,   136,   149,   160,   150,   220,
     151,   138,   152,    59,   161,   139,   221,    10,   152,   140,
     141,   136,   142,   143,   197,   144,   145,    12,   198,   146,
     229,    60,   230,   192,   227,   199,   200,   147,   156,    61,
     201,   162,   253,   156,   152,   257,   269,   152,   270,   283,
     202,   152,   297,   167,    62,   107,   115,   116,   117,   118,
     205,   287,   120,   152,   122,   302,   306,   152,   307,   308,
     312,   152,   152,   259,    68,   188,   110,   112,   191,   267,
     250,    56,   215,   115,   116,   117,   118,   136,   119,   120,
     121,   122,   237,   216,   125,   126,   127,   128,   217,   218,
     131,   132,   219,   136,   115,   116,   117,   118,   247,   119,
     120,   121,   122,   289,   136,   125,   126,   127,   128,   222,
     251,   131,   132,   252,   261,   258,   279,   280,   301,   224,
     281,   282,   284,   304,   288,   136,   303,   236,   305,   313,
      56,   311,   278,   268,   157,   207,     0,   310,     0,     0,
     231,     0,     0,    64,     0,     0,    56,     0,     0,     0,
       0,   254,     0,     0,   291,     0,     0,     0,     0,   260,
       0,    99,    56,   104,   105,   106,   109,     0,     0,     0,
       0,     0,   111,     0,     0,     0,     0,    64,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   290,
     153,     0,    56,   158,     0,     0,     0,     0,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,   187,     0,
       0,   189,     0,   193,     0,   194,   195,     0,     0,   203,
     204,     0,     0,    99,     0,   208,     0,   210,     0,   211,
     115,   116,   117,   118,     0,   119,   120,   121,   122,     0,
       0,   125,   126,   127,   128,     0,     0,   131,   132,   115,
     116,   117,   118,     0,   119,   120,   121,   122,     0,     0,
       0,   136,     0,     0,     0,     0,   131,   132,     0,   233,
     234,     0,   238,   239,     0,   240,   232,     0,   242,     0,
     136,     0,   243,     0,     0,   246,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   255,   256,     0,     0,
       0,     0,     0,     0,   262,   263,     0,     0,     0,     0,
       0,     0,     0,     0,   274,     0,     0,     0,   275,     0,
       0,     0,     0,    69,   163,     0,     0,     0,    70,    71,
     164,     0,    72,     0,     0,     0,     0,   285,   286,     0,
       0,     0,     0,     0,   292,   293,   294,    13,    14,     0,
       0,     0,    15,     0,     0,    16,    73,    74,     0,     0,
      18,     0,     0,    75,    76,    77,     0,     0,    22,     0,
      23,   165,     0,    24,     0,     0,    25,    78,    26,    27,
       0,    28,     0,    29,    30,     0,     0,    31,     0,     0,
       0,    32,    33,    34,    35,    79,     0,    80,     0,     0,
      36,    37,    38,     0,    81,    40,     0,    41,    42,    43,
      44,    45,    46,     0,     0,     0,    63,    48,     0,    82,
      49,     0,     0,     0,    50,     0,     0,    51,    52,    83,
      84,    54,    85,    86,    69,   225,     0,     0,     0,    70,
      71,     0,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    13,    14,
       0,     0,     0,    15,     0,     0,    16,    73,    74,     0,
       0,    18,     0,     0,    75,    76,    77,     0,     0,    22,
       0,    23,     0,     0,    24,     0,     0,    25,    78,    26,
      27,     0,    28,     0,    29,    30,     0,     0,    31,     0,
       0,     0,    32,    33,    34,    35,    79,     0,    80,     0,
       0,    36,    37,    38,     0,    81,    40,     0,    41,    42,
      43,    44,    45,    46,   226,     0,     0,    63,    48,     0,
      82,    49,     0,     0,     0,    50,     0,     0,    51,    52,
      83,    84,    54,    85,    86,    69,   266,     0,     0,     0,
      70,    71,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    13,
      14,     0,     0,     0,    15,     0,     0,    16,    73,    74,
       0,     0,    18,     0,     0,    75,    76,    77,     0,     0,
      22,     0,    23,     0,     0,    24,     0,     0,    25,    78,
      26,    27,     0,    28,     0,    29,    30,     0,     0,    31,
       0,     0,     0,    32,    33,    34,    35,    79,     0,    80,
       0,     0,    36,    37,    38,     0,    81,    40,     0,    41,
      42,    43,    44,    45,    46,   226,     0,     0,    63,    48,
       0,    82,    49,     0,     0,     0,    50,     0,     0,    51,
      52,    83,    84,    54,    85,    86,    69,     0,     0,     0,
       0,    70,    71,     0,     0,    72,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      13,    14,     0,     0,     0,    15,     0,     0,    16,    73,
      74,     0,     0,    18,     0,     0,    75,    76,    77,     0,
       0,    22,     0,    23,     0,     0,    24,     0,     0,    25,
      78,    26,    27,     0,    28,     0,    29,    30,     0,     0,
      31,     0,     0,     0,    32,    33,    34,    35,    79,     0,
      80,     0,     0,    36,    37,    38,     0,    81,    40,     0,
      41,    42,    43,    44,    45,    46,     0,     0,     0,    63,
      48,     0,    82,    49,     0,     0,     0,    50,   107,     0,
      51,    52,    83,    84,    54,    85,    86,    69,   300,     0,
       0,     0,    70,    71,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    13,    14,     0,     0,     0,    15,     0,     0,    16,
      73,    74,     0,     0,    18,     0,     0,    75,    76,    77,
       0,     0,    22,     0,    23,     0,     0,    24,     0,     0,
      25,    78,    26,    27,     0,    28,     0,    29,    30,     0,
       0,    31,     0,     0,     0,    32,    33,    34,    35,    79,
       0,    80,     0,     0,    36,    37,    38,     0,    81,    40,
       0,    41,    42,    43,    44,    45,    46,     0,     0,     0,
      63,    48,     0,    82,    49,     0,     0,     0,    50,     0,
       0,    51,    52,    83,    84,    54,    85,    86,    69,   309,
       0,     0,     0,    70,    71,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    13,    14,     0,     0,     0,    15,     0,     0,
      16,    73,    74,     0,     0,    18,     0,     0,    75,    76,
      77,     0,     0,    22,     0,    23,     0,     0,    24,     0,
       0,    25,    78,    26,    27,     0,    28,     0,    29,    30,
       0,     0,    31,     0,     0,     0,    32,    33,    34,    35,
      79,     0,    80,     0,     0,    36,    37,    38,     0,    81,
      40,     0,    41,    42,    43,    44,    45,    46,     0,     0,
       0,    63,    48,     0,    82,    49,     0,     0,     0,    50,
       0,     0,    51,    52,    83,    84,    54,    85,    86,    69,
       0,     0,     0,     0,    70,    71,     0,     0,    72,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    13,    14,     0,     0,     0,    15,     0,
       0,    16,    73,    74,     0,     0,    18,     0,     0,    75,
      76,    77,     0,     0,    22,     0,    23,     0,     0,    24,
       0,     0,    25,    78,    26,    27,     0,    28,     0,    29,
      30,     0,     0,    31,     0,     0,     0,    32,    33,    34,
      35,    79,     0,    80,     0,     0,    36,    37,    38,     0,
      81,    40,     0,    41,    42,    43,    44,    45,    46,   190,
       0,     0,    63,    48,     0,    82,    49,     0,     0,     0,
      50,     0,     0,    51,    52,    83,    84,    54,    85,    86,
       0,     0,     0,    13,    14,     0,     0,     0,    15,     0,
       0,    16,     0,    17,     0,     0,    18,     0,     0,    19,
      20,    21,     0,     0,    22,     0,    23,     0,     0,    24,
       0,     0,    25,     0,    26,    27,     0,    28,     0,    29,
      30,     0,     0,    31,     0,     0,     0,    32,    33,    34,
      35,     0,     0,     0,     0,     0,    36,    37,    38,     0,
      39,    40,     0,    41,    42,    43,    44,    45,    46,   235,
       0,     0,    63,    48,     0,     0,    49,     0,     0,     0,
      50,     0,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,    13,    14,     0,     0,     0,    15,     0,
       0,    16,     0,    17,     0,     0,    18,     0,     0,    19,
      20,    21,     0,     0,    22,     0,    23,     0,     0,    24,
       0,     0,    25,     0,    26,    27,     0,    28,     0,    29,
      30,     0,     0,    31,     0,     0,     0,    32,    33,    34,
      35,     0,     0,     0,     0,     0,    36,    37,    38,     0,
      39,    40,     0,    41,    42,    43,    44,    45,    46,   277,
       0,     0,    63,    48,     0,     0,    49,     0,     0,     0,
      50,     0,     0,    51,    52,     0,    53,    54,     0,     0,
       0,     0,     0,    13,    14,     0,     0,     0,    15,     0,
       0,    16,     0,    17,     0,     0,    18,     0,     0,    19,
      20,    21,     0,     0,    22,     0,    23,     0,     0,    24,
       0,     0,    25,     0,    26,    27,     0,    28,     0,    29,
      30,     0,     0,    31,     0,     0,     0,    32,    33,    34,
      35,     0,     0,     0,     0,     0,    36,    37,    38,     0,
      39,    40,     0,    41,    42,    43,    44,    45,    46,     0,
       0,     0,    63,    48,     0,     0,    49,     0,     0,     0,
      50,     0,     0,    51,    52,     0,    53,    54,   115,   116,
     117,   118,     0,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,     0,     0,   133,
       0,     0,     0,     0,   135,   115,   116,   117,   118,   136,
     119,   120,   121,   122,     0,     0,   125,   126,   127,   128,
       0,     0,   131,   132,     0,     0,     0,     0,     0,     0,
       0,   138,     0,     0,     0,   139,   136,     0,     0,   140,
     141,     0,   142,   143,     0,   144,   145,     0,     0,   146,
       0,   241,     0,     0,     0,     0,     0,   147,     0,     0,
       0,     0,     0,     0,   115,   116,   117,   118,   209,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,     0,     0,   133,     0,     0,     0,     0,
     135,   115,   116,   117,   118,   136,   119,   120,   121,   122,
       0,     0,   125,   126,   127,   128,     0,     0,   131,   132,
       0,     0,     0,     0,     0,     0,     0,   138,     0,     0,
       0,   139,   136,     0,     0,   140,   141,     0,   142,   143,
       0,   144,   145,     0,     0,   146,     0,   265,     0,    13,
      14,     0,     0,   147,    15,     0,     0,    16,     0,    17,
       0,     0,    18,     0,   245,    19,    20,    21,     0,     0,
      22,     0,    23,     0,     0,    24,     0,     0,    25,     0,
      26,    27,     0,    28,     0,    29,    30,     0,     0,    31,
       0,     0,     0,    32,    33,    34,    35,     0,     0,     0,
       0,     0,    36,    37,    38,     0,    39,    40,     0,    41,
      42,    43,    44,    45,    46,     0,     0,     0,    47,    48,
       0,     0,    49,     0,     0,     0,    50,     0,     0,    51,
      52,     0,    53,    54,    13,    14,     0,     0,     0,    15,
       0,     0,    16,     0,    17,     0,     0,    18,     0,     0,
      19,    20,    21,     0,     0,    22,     0,    23,     0,     0,
      24,     0,     0,    25,     0,    26,    27,     0,    28,     0,
      29,    30,     0,     0,    31,     0,     0,     0,    32,    33,
      34,    35,     0,     0,     0,     0,     0,    36,    37,    38,
       0,    39,    40,     0,    41,    42,    43,    44,    45,    46,
       0,     0,     0,    63,    48,     0,     0,    49,     0,     0,
       0,    50,     0,     0,    51,    52,     0,    53,    54,   115,
     116,   117,   118,     0,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,     0,     0,
     133,     0,     0,   134,     0,   135,   115,   116,   117,   118,
     136,   119,   120,   121,   122,     0,     0,   125,   126,   127,
     128,   137,     0,   131,   132,     0,     0,     0,     0,     0,
       0,     0,   138,     0,     0,     0,   139,   136,     0,     0,
     140,   141,     0,   142,   143,     0,   144,   145,     0,     0,
     146,     0,   271,     0,   115,   116,   117,   118,   147,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,     0,     0,   133,     0,   214,     0,     0,
     135,   115,   116,   117,   118,   136,   119,   120,   121,   122,
       0,     0,   125,   126,   127,   128,     0,     0,   131,   132,
       0,     0,     0,     0,     0,     0,     0,   138,     0,     0,
       0,   139,   136,     0,     0,   140,   141,     0,   142,   143,
       0,   144,   145,     0,     0,   146,     0,   272,     0,   115,
     116,   117,   118,   147,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,     0,     0,
     133,   223,     0,     0,     0,   135,   115,   116,   117,   118,
     136,   119,   120,   121,   122,     0,     0,   125,   126,   127,
     128,     0,     0,   131,   132,     0,     0,     0,     0,     0,
       0,     0,   138,     0,     0,     0,   139,   136,     0,     0,
     140,   141,     0,   142,   143,     0,   144,   145,     0,     0,
     146,     0,   273,     0,   115,   116,   117,   118,   147,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,     0,     0,   133,     0,     0,     0,     0,
     135,     0,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   244,
       0,     0,     0,     0,     0,     0,     0,   138,     0,     0,
       0,   139,     0,     0,     0,   140,   141,     0,   142,   143,
       0,   144,   145,     0,     0,   146,     0,     0,     0,   115,
     116,   117,   118,   147,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,     0,     0,
     133,   264,     0,     0,     0,   135,   115,   116,   117,   118,
     136,   119,   120,   121,   122,     0,     0,   125,   126,   127,
     128,     0,     0,   131,   132,     0,     0,     0,     0,     0,
       0,     0,   138,     0,     0,     0,   139,   136,     0,     0,
     140,   141,     0,   142,   143,     0,   144,   145,     0,     0,
     146,     0,     0,     0,   115,   116,   117,   118,   147,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,     0,     0,   133,     0,     0,     0,     0,
     135,     0,     0,     0,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,   138,     0,     0,
       0,   139,     0,     0,     0,   140,   141,     0,   142,   143,
       0,   144,   145,     0,     0,   146,     0,     0,     0,   115,
     116,   117,   118,   147,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,     0,     0,
     133,     0,     0,     0,     0,   135,     0,     0,     0,     0,
     136,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   138,     0,     0,     0,   139,     0,     0,     0,
     140,   141,     0,   142,   143,     0,   144,   145,     0,     0,
     146,     0,     0,     0,   115,   116,   117,   118,   147,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,     0,     0,   133,     0,     0,     0,     0,
     135,     0,     0,     0,     0,   136,   115,   116,   117,   118,
       0,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,     0,     0,   138,     0,     0,
       0,   139,   135,     0,     0,   140,   141,   136,   142,   143,
       0,   144,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   147,     0,     0,     0,     0,     0,   138,
       0,     0,     0,   139,     0,     0,     0,   140,   141,     0,
     142,   143,     0,   144,   145,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   147
  };

  const short
  parser::yycheck_[] =
  {
      11,     8,     9,    11,    60,    61,    62,   113,     8,     9,
      10,    11,    42,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,   108,    27,    29,
     111,     3,    16,     5,    34,    52,    53,   101,     0,    39,
      52,    53,    10,    11,     7,    66,    14,    34,    16,    60,
      61,    62,    60,    61,    62,    39,     4,    56,     6,   165,
       4,    61,     6,    55,    63,    65,     4,    62,     6,    69,
      70,    39,    72,    73,    61,    75,    76,    75,    65,    79,
       3,     5,     5,   139,   190,    72,    73,    87,   105,     5,
      77,    90,     4,   105,     6,     4,     3,     6,     5,     4,
      87,     6,   109,   114,    78,   105,     8,     9,    10,    11,
     106,     4,    14,     6,    16,     4,     4,     6,     6,     4,
       4,     6,     6,   229,     3,   136,     3,     3,   139,   235,
     110,   139,     6,     8,     9,    10,    11,    39,    13,    14,
      15,    16,   198,     6,    19,    20,    21,    22,     4,     6,
      25,    26,     4,    39,     8,     9,    10,    11,   214,    13,
      14,    15,    16,   269,    39,    19,    20,    21,    22,     5,
     110,    25,    26,   110,   230,     4,     4,     4,   284,    54,
       4,     4,     3,   109,     4,    39,     3,   198,   109,     4,
     198,   307,   248,   235,   109,   150,    -1,   303,    -1,    -1,
      54,    -1,    -1,   214,    -1,    -1,   214,    -1,    -1,    -1,
      -1,   222,    -1,    -1,   270,    -1,    -1,    -1,    -1,   230,
      -1,    68,   230,    70,    71,    72,    73,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,   248,    -1,    -1,
     248,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   270,
     107,    -1,   270,   110,    -1,    -1,    -1,    -1,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,    -1,   135,    -1,
      -1,   138,    -1,   140,    -1,   142,   143,    -1,    -1,   146,
     147,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,   156,
       8,     9,    10,    11,    -1,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    -1,    -1,    25,    26,     8,
       9,    10,    11,    -1,    13,    14,    15,    16,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    25,    26,    -1,   196,
     197,    -1,   199,   200,    -1,   202,    54,    -1,   205,    -1,
      39,    -1,   209,    -1,    -1,   212,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   223,   224,    -1,    -1,
      -1,    -1,    -1,    -1,   231,   232,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   241,    -1,    -1,    -1,   245,    -1,
      -1,    -1,    -1,     3,     4,    -1,    -1,    -1,     8,     9,
      10,    -1,    12,    -1,    -1,    -1,    -1,   264,   265,    -1,
      -1,    -1,    -1,    -1,   271,   272,   273,    27,    28,    -1,
      -1,    -1,    32,    -1,    -1,    35,    36,    37,    -1,    -1,
      40,    -1,    -1,    43,    44,    45,    -1,    -1,    48,    -1,
      50,    51,    -1,    53,    -1,    -1,    56,    57,    58,    59,
      -1,    61,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,
      -1,    71,    72,    73,    74,    75,    -1,    77,    -1,    -1,
      80,    81,    82,    -1,    84,    85,    -1,    87,    88,    89,
      90,    91,    92,    -1,    -1,    -1,    96,    97,    -1,    99,
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
      89,    90,    91,    92,    93,    -1,    -1,    96,    97,    -1,
      99,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,   108,
     109,   110,   111,   112,   113,     3,     4,    -1,    -1,    -1,
       8,     9,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    32,    -1,    -1,    35,    36,    37,
      -1,    -1,    40,    -1,    -1,    43,    44,    45,    -1,    -1,
      48,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    57,
      58,    59,    -1,    61,    -1,    63,    64,    -1,    -1,    67,
      -1,    -1,    -1,    71,    72,    73,    74,    75,    -1,    77,
      -1,    -1,    80,    81,    82,    -1,    84,    85,    -1,    87,
      88,    89,    90,    91,    92,    93,    -1,    -1,    96,    97,
      -1,    99,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,
     108,   109,   110,   111,   112,   113,     3,    -1,    -1,    -1,
      -1,     8,     9,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    32,    -1,    -1,    35,    36,
      37,    -1,    -1,    40,    -1,    -1,    43,    44,    45,    -1,
      -1,    48,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      57,    58,    59,    -1,    61,    -1,    63,    64,    -1,    -1,
      67,    -1,    -1,    -1,    71,    72,    73,    74,    75,    -1,
      77,    -1,    -1,    80,    81,    82,    -1,    84,    85,    -1,
      87,    88,    89,    90,    91,    92,    -1,    -1,    -1,    96,
      97,    -1,    99,   100,    -1,    -1,    -1,   104,   105,    -1,
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
      84,    85,    -1,    87,    88,    89,    90,    91,    92,     3,
      -1,    -1,    96,    97,    -1,    99,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,   108,   109,   110,   111,   112,   113,
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
      -1,    -1,    40,    -1,    98,    43,    44,    45,    -1,    -1,
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
      -1,   104,    -1,    -1,   107,   108,    -1,   110,   111,     8,
       9,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    -1,    -1,    32,    -1,    34,     8,     9,    10,    11,
      39,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    50,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    65,    39,    -1,    -1,
      69,    70,    -1,    72,    73,    -1,    75,    76,    -1,    -1,
      79,    -1,    54,    -1,     8,     9,    10,    11,    87,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    -1,    31,    -1,    -1,
      34,     8,     9,    10,    11,    39,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    -1,    -1,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    65,    39,    -1,    -1,    69,    70,    -1,    72,    73,
      -1,    75,    76,    -1,    -1,    79,    -1,    54,    -1,     8,
       9,    10,    11,    87,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    30,    -1,    -1,    -1,    34,     8,     9,    10,    11,
      39,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    65,    39,    -1,    -1,
      69,    70,    -1,    72,    73,    -1,    75,    76,    -1,    -1,
      79,    -1,    54,    -1,     8,     9,    10,    11,    87,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    69,    70,    -1,    72,    73,
      -1,    75,    76,    -1,    -1,    79,    -1,    -1,    -1,     8,
       9,    10,    11,    87,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    30,    -1,    -1,    -1,    34,     8,     9,    10,    11,
      39,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    65,    39,    -1,    -1,
      69,    70,    -1,    72,    73,    -1,    75,    76,    -1,    -1,
      79,    -1,    -1,    -1,     8,     9,    10,    11,    87,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    69,    70,    -1,    72,    73,
      -1,    75,    76,    -1,    -1,    79,    -1,    -1,    -1,     8,
       9,    10,    11,    87,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      69,    70,    -1,    72,    73,    -1,    75,    76,    -1,    -1,
      79,    -1,    -1,    -1,     8,     9,    10,    11,    87,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    39,     8,     9,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    61,    -1,    -1,
      -1,    65,    34,    -1,    -1,    69,    70,    39,    72,    73,
      -1,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    65,    -1,    -1,    -1,    69,    70,    -1,
      72,    73,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    42,   115,   116,   145,   101,   140,     0,     7,    66,
      62,   138,    75,    27,    28,    32,    35,    37,    40,    43,
      44,    45,    48,    50,    53,    56,    58,    59,    61,    63,
      64,    67,    71,    72,    73,    74,    80,    81,    82,    84,
      85,    87,    88,    89,    90,    91,    92,    96,    97,   100,
     104,   107,   108,   110,   111,   118,   119,   120,   142,    55,
       5,     5,    78,    96,   118,   120,   120,   120,     3,     3,
       8,     9,    12,    36,    37,    43,    44,    45,    57,    75,
      77,    84,    99,   109,   110,   112,   113,   117,   118,   119,
     125,   126,   127,   129,   130,   131,   132,   134,   135,   136,
     143,   144,   128,   136,   136,   136,   136,   105,   133,   136,
       3,   136,     3,     3,     5,     8,     9,    10,    11,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    29,    32,    34,    39,    50,    61,    65,
      69,    70,    72,    73,    75,    76,    79,    87,   139,     4,
       6,     4,     6,   136,    52,    53,   105,   133,   136,    27,
      56,    63,    90,     4,    10,    51,   128,   118,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   118,   136,
       3,   118,   120,   136,   136,   136,    34,    61,    65,    72,
      73,    77,    87,   136,   136,   106,   141,   143,   136,    98,
     136,   136,    52,    53,    31,     6,     6,     4,     6,     4,
     128,     4,     5,    30,    54,     4,    93,   128,   137,     3,
       5,    54,    54,   136,   136,     3,   118,   120,   136,   136,
     136,    54,   136,   136,    53,    98,   136,   120,   123,   124,
     110,   110,   110,     4,   118,   136,   136,     4,     4,   128,
     118,   120,   136,   136,    30,    54,     4,   128,   137,     3,
       5,    54,    54,    54,   136,   136,    53,     3,   120,     4,
       4,     4,     4,     4,     3,   136,   136,     4,     4,   128,
     118,   120,   136,   136,   136,     8,     9,   109,   121,   122,
       4,   128,     4,     3,   109,   109,     4,     6,     4,     4,
     128,   122,     4,     4
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   114,   115,   115,   116,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   118,   118,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   120,   120,   120,   120,   120,   120,   121,   121,
     122,   122,   123,   123,   124,   124,   124,   125,   125,   125,
     125,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   128,   128,
     129,   129,   129,   129,   130,   130,   130,   131,   131,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   133,   133,   134,   134,   134,
     134,   135,   135,   135,   135,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   137,   138,   138,   139,   139,   139,   140,
     140,   141,   141,   142,   142,   142,   143,   144,   144,   145
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     2,     1,     4,     6,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     4,     4,     4,
       5,     5,     5,     5,     6,     6,     6,     6,     1,     3,
       4,     5,     3,     4,     2,     2,     3,     5,     6,     4,
       5,     5,     5,     3,     7,     8,     6,     5,     6,     6,
       6,     4,     8,     9,     7,     4,     5,     6,     4,     5,
       3,     4,     6,     6,     6,     1,     1,     1,     5,     3,
       1,     1,     1,     1,     3,     6,     3,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     0,     1,     1,     0,
       1,     0,     2,     3,     3,     1,     2,     1,     3,    11
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
  "indexed_column_list", "createindex_stmt", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short
  parser::yyrline_[] =
  {
       0,   223,   223,   224,   228,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   248,   249,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   293,   294,   295,   296,   297,   298,   312,   313,
     317,   318,   322,   323,   327,   328,   329,   333,   334,   335,
     336,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   383,   384,
     388,   389,   390,   391,   395,   396,   397,   401,   402,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   425,   426,   430,   431,   432,
     433,   437,   438,   439,   440,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   470,   478,   479,   483,   484,   485,   493,
     494,   498,   499,   503,   504,   505,   509,   523,   524,   528
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
#line 3209 "sqlite3_parser.cpp"

#line 538 "sqlite3_parser.yy"


void sqlb::parser::parser::error(const location_type& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}
