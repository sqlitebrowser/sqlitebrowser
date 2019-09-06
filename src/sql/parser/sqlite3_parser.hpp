// A Bison parser, made by GNU Bison 3.4.1.

// Skeleton interface for Bison LALR(1) parsers in C++

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


/**
 ** \file sqlite3_parser.hpp
 ** Define the  sqlb::parser ::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.

#ifndef YY_YY_SQLITE3_PARSER_HPP_INCLUDED
# define YY_YY_SQLITE3_PARSER_HPP_INCLUDED
// //                    "%code requires" blocks.
#line 12 "sqlite3_parser.yy"

	#include <string>
	#include "../sqlitetypes.h"
	#include "../ObjectIdentifier.h"
	namespace sqlb { namespace parser { class ParserDriver; } }
	typedef void* yyscan_t;

#line 56 "sqlite3_parser.hpp"

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "sqlite3_location.h"
#include <typeinfo>
#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

#line 10 "sqlite3_parser.yy"
namespace  sqlb { namespace parser  {
#line 172 "sqlite3_parser.hpp"




  /// A Bison parser.
  class parser
  {
  public:
#ifndef YYSTYPE
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class semantic_type
  {
  public:
    /// Type of *this.
    typedef semantic_type self_type;

    /// Empty construction.
    semantic_type () YY_NOEXCEPT
      : yybuffer_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    semantic_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YYASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

    /// Destruction, allowed only if empty.
    ~semantic_type () YY_NOEXCEPT
    {
      YYASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
    /// Prohibit blind copies.
    self_type& operator= (const self_type&);
    semantic_type (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // optional_if_not_exists
      // optional_unique
      char dummy1[sizeof (bool)];

      // createindex_stmt
      char dummy2[sizeof (sqlb::IndexPtr)];

      // indexed_column
      char dummy3[sizeof (sqlb::IndexedColumn)];

      // indexed_column_list
      char dummy4[sizeof (sqlb::IndexedColumnVector)];

      // createvirtualtable_stmt
      char dummy5[sizeof (sqlb::TablePtr)];

      // "ABORT"
      // "ACTION"
      // "AND"
      // "AND BETWEEN"
      // "AS"
      // "ASC"
      // "AUTOINCREMENT"
      // "BETWEEN"
      // "CASCADE"
      // "CASE"
      // "CAST"
      // "CHECK"
      // "COLLATE"
      // "CONFLICT"
      // "CONSTRAINT"
      // "CREATE"
      // "CURRENT_DATE"
      // "CURRENT_TIME"
      // "CURRENT_TIMESTAMP"
      // "DEFAULT"
      // "DEFERRABLE"
      // "DEFERRED"
      // "DELETE"
      // "DESC"
      // "DISTINCT"
      // "ELSE"
      // "END"
      // "ESCAPE"
      // "EXISTS"
      // "FAIL"
      // "FALSE"
      // "FILTER"
      // "FOLLOWING"
      // "FOREIGN"
      // "GLOB"
      // "IF"
      // "IGNORE"
      // "IMMEDIATE"
      // "IN"
      // "INDEX"
      // "INITIALLY"
      // "INSERT"
      // "IS"
      // "ISNULL"
      // "KEY"
      // "LIKE"
      // "MATCH"
      // "NO"
      // "NOT"
      // "NOTNULL"
      // "NULL"
      // "ON"
      // "OR"
      // "OVER"
      // "PARTITION"
      // "PRECEDING"
      // "PRIMARY"
      // "RAISE"
      // "RANGE"
      // "REFERENCES"
      // "REGEXP"
      // "REPLACE"
      // "RESTRICT"
      // "ROLLBACK"
      // "ROWID"
      // "ROWS"
      // "SELECT"
      // "SET"
      // "TABLE"
      // "TEMP"
      // "TEMPORARY"
      // "THEN"
      // "TRUE"
      // "UNBOUNDED"
      // "UNIQUE"
      // "UPDATE"
      // "USING"
      // "VIRTUAL"
      // "WHEN"
      // "WHERE"
      // "WITHOUT"
      // "identifier"
      // "numeric"
      // "string literal"
      // "quoted literal"
      // "blob literal"
      // "bind parameter"
      // literalvalue
      // id
      // allowed_keywords_as_identifier
      // tableid
      // signednumber
      // signednumber_or_numeric
      // typename_namelist
      // type_name
      // unary_expr
      // binary_expr
      // like_expr
      // exprlist_expr
      // function_expr
      // isnull_expr
      // between_expr
      // in_expr
      // whenthenlist_expr
      // case_expr
      // raise_expr
      // expr
      // select_stmt
      // optional_sort_order
      // optional_where
      // tableid_with_uninteresting_schema
      // optional_exprlist_with_paren
      char dummy6[sizeof (std::string)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[size];
    } yybuffer_;

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        TOK_EOF = 0,
        TOK_LPAREN = 258,
        TOK_RPAREN = 259,
        TOK_DOT = 260,
        TOK_COMMA = 261,
        TOK_SEMI = 262,
        TOK_PLUS = 263,
        TOK_MINUS = 264,
        TOK_STAR = 265,
        TOK_SLASH = 266,
        TOK_TILDE = 267,
        TOK_AMPERSAND = 268,
        TOK_PERCENT = 269,
        TOK_BITOR = 270,
        TOK_OROP = 271,
        TOK_EQUAL = 272,
        TOK_EQUAL2 = 273,
        TOK_GREATER = 274,
        TOK_GREATEREQUAL = 275,
        TOK_LOWER = 276,
        TOK_LOWEREQUAL = 277,
        TOK_UNEQUAL = 278,
        TOK_UNEQUAL2 = 279,
        TOK_BITWISELEFT = 280,
        TOK_BITWISERIGHT = 281,
        TOK_ABORT = 282,
        TOK_ACTION = 283,
        TOK_AND = 284,
        TOK_AND_BETWEEN = 285,
        TOK_AS = 286,
        TOK_ASC = 287,
        TOK_AUTOINCREMENT = 288,
        TOK_BETWEEN = 289,
        TOK_CASCADE = 290,
        TOK_CASE = 291,
        TOK_CAST = 292,
        TOK_CHECK = 293,
        TOK_COLLATE = 294,
        TOK_CONFLICT = 295,
        TOK_CONSTRAINT = 296,
        TOK_CREATE = 297,
        TOK_CURRENT_DATE = 298,
        TOK_CURRENT_TIME = 299,
        TOK_CURRENT_TIMESTAMP = 300,
        TOK_DEFAULT = 301,
        TOK_DEFERRABLE = 302,
        TOK_DEFERRED = 303,
        TOK_DELETE = 304,
        TOK_DESC = 305,
        TOK_DISTINCT = 306,
        TOK_ELSE = 307,
        TOK_END = 308,
        TOK_ESCAPE = 309,
        TOK_EXISTS = 310,
        TOK_FAIL = 311,
        TOK_FALSE = 312,
        TOK_FILTER = 313,
        TOK_FOLLOWING = 314,
        TOK_FOREIGN = 315,
        TOK_GLOB = 316,
        TOK_IF = 317,
        TOK_IGNORE = 318,
        TOK_IMMEDIATE = 319,
        TOK_IN = 320,
        TOK_INDEX = 321,
        TOK_INITIALLY = 322,
        TOK_INSERT = 323,
        TOK_IS = 324,
        TOK_ISNULL = 325,
        TOK_KEY = 326,
        TOK_LIKE = 327,
        TOK_MATCH = 328,
        TOK_NO = 329,
        TOK_NOT = 330,
        TOK_NOTNULL = 331,
        TOK_NULL = 332,
        TOK_ON = 333,
        TOK_OR = 334,
        TOK_OVER = 335,
        TOK_PARTITION = 336,
        TOK_PRECEDING = 337,
        TOK_PRIMARY = 338,
        TOK_RAISE = 339,
        TOK_RANGE = 340,
        TOK_REFERENCES = 341,
        TOK_REGEXP = 342,
        TOK_REPLACE = 343,
        TOK_RESTRICT = 344,
        TOK_ROLLBACK = 345,
        TOK_ROWID = 346,
        TOK_ROWS = 347,
        TOK_SELECT = 348,
        TOK_SET = 349,
        TOK_TABLE = 350,
        TOK_TEMP = 351,
        TOK_TEMPORARY = 352,
        TOK_THEN = 353,
        TOK_TRUE = 354,
        TOK_UNBOUNDED = 355,
        TOK_UNIQUE = 356,
        TOK_UPDATE = 357,
        TOK_USING = 358,
        TOK_VIRTUAL = 359,
        TOK_WHEN = 360,
        TOK_WHERE = 361,
        TOK_WITHOUT = 362,
        TOK_IDENTIFIER = 363,
        TOK_NUMERIC = 364,
        TOK_STRINGLITERAL = 365,
        TOK_QUOTEDLITERAL = 366,
        TOK_BLOBLITERAL = 367,
        TOK_BINDPARAMETER = 368
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ()
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that);
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, bool&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const bool& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sqlb::IndexPtr&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sqlb::IndexPtr& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sqlb::IndexedColumn&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sqlb::IndexedColumn& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sqlb::IndexedColumnVector&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sqlb::IndexedColumnVector& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sqlb::TablePtr&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sqlb::TablePtr& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }

      /// Destroy contents, and record that is empty.
      void clear ()
      {
        // User destructor.
        symbol_number_type yytype = this->type_get ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yytype)
        {
       default:
          break;
        }

        // Type destructor.
switch (yytype)
    {
      case 138: // optional_if_not_exists
      case 140: // optional_unique
        value.template destroy< bool > ();
        break;

      case 145: // createindex_stmt
        value.template destroy< sqlb::IndexPtr > ();
        break;

      case 143: // indexed_column
        value.template destroy< sqlb::IndexedColumn > ();
        break;

      case 144: // indexed_column_list
        value.template destroy< sqlb::IndexedColumnVector > ();
        break;

      case 147: // createvirtualtable_stmt
        value.template destroy< sqlb::TablePtr > ();
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
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_type (by_type&& that);
#endif

      /// Copy constructor.
      by_type (const by_type& that);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The token.
      token_type token () const YY_NOEXCEPT;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_type>
    {
      /// Superclass.
      typedef basic_symbol<by_type> super_type;

      /// Empty symbol.
      symbol_type () {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type(token_type (tok), std::move (l))
      {
        YYASSERT (tok == token::TOK_EOF || tok == token::TOK_LPAREN || tok == token::TOK_RPAREN || tok == token::TOK_DOT || tok == token::TOK_COMMA || tok == token::TOK_SEMI || tok == token::TOK_PLUS || tok == token::TOK_MINUS || tok == token::TOK_STAR || tok == token::TOK_SLASH || tok == token::TOK_TILDE || tok == token::TOK_AMPERSAND || tok == token::TOK_PERCENT || tok == token::TOK_BITOR || tok == token::TOK_OROP || tok == token::TOK_EQUAL || tok == token::TOK_EQUAL2 || tok == token::TOK_GREATER || tok == token::TOK_GREATEREQUAL || tok == token::TOK_LOWER || tok == token::TOK_LOWEREQUAL || tok == token::TOK_UNEQUAL || tok == token::TOK_UNEQUAL2 || tok == token::TOK_BITWISELEFT || tok == token::TOK_BITWISERIGHT);
      }
#else
      symbol_type (int tok, const location_type& l)
        : super_type(token_type (tok), l)
      {
        YYASSERT (tok == token::TOK_EOF || tok == token::TOK_LPAREN || tok == token::TOK_RPAREN || tok == token::TOK_DOT || tok == token::TOK_COMMA || tok == token::TOK_SEMI || tok == token::TOK_PLUS || tok == token::TOK_MINUS || tok == token::TOK_STAR || tok == token::TOK_SLASH || tok == token::TOK_TILDE || tok == token::TOK_AMPERSAND || tok == token::TOK_PERCENT || tok == token::TOK_BITOR || tok == token::TOK_OROP || tok == token::TOK_EQUAL || tok == token::TOK_EQUAL2 || tok == token::TOK_GREATER || tok == token::TOK_GREATEREQUAL || tok == token::TOK_LOWER || tok == token::TOK_LOWEREQUAL || tok == token::TOK_UNEQUAL || tok == token::TOK_UNEQUAL2 || tok == token::TOK_BITWISELEFT || tok == token::TOK_BITWISERIGHT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YYASSERT (tok == token::TOK_ABORT || tok == token::TOK_ACTION || tok == token::TOK_AND || tok == token::TOK_AND_BETWEEN || tok == token::TOK_AS || tok == token::TOK_ASC || tok == token::TOK_AUTOINCREMENT || tok == token::TOK_BETWEEN || tok == token::TOK_CASCADE || tok == token::TOK_CASE || tok == token::TOK_CAST || tok == token::TOK_CHECK || tok == token::TOK_COLLATE || tok == token::TOK_CONFLICT || tok == token::TOK_CONSTRAINT || tok == token::TOK_CREATE || tok == token::TOK_CURRENT_DATE || tok == token::TOK_CURRENT_TIME || tok == token::TOK_CURRENT_TIMESTAMP || tok == token::TOK_DEFAULT || tok == token::TOK_DEFERRABLE || tok == token::TOK_DEFERRED || tok == token::TOK_DELETE || tok == token::TOK_DESC || tok == token::TOK_DISTINCT || tok == token::TOK_ELSE || tok == token::TOK_END || tok == token::TOK_ESCAPE || tok == token::TOK_EXISTS || tok == token::TOK_FAIL || tok == token::TOK_FALSE || tok == token::TOK_FILTER || tok == token::TOK_FOLLOWING || tok == token::TOK_FOREIGN || tok == token::TOK_GLOB || tok == token::TOK_IF || tok == token::TOK_IGNORE || tok == token::TOK_IMMEDIATE || tok == token::TOK_IN || tok == token::TOK_INDEX || tok == token::TOK_INITIALLY || tok == token::TOK_INSERT || tok == token::TOK_IS || tok == token::TOK_ISNULL || tok == token::TOK_KEY || tok == token::TOK_LIKE || tok == token::TOK_MATCH || tok == token::TOK_NO || tok == token::TOK_NOT || tok == token::TOK_NOTNULL || tok == token::TOK_NULL || tok == token::TOK_ON || tok == token::TOK_OR || tok == token::TOK_OVER || tok == token::TOK_PARTITION || tok == token::TOK_PRECEDING || tok == token::TOK_PRIMARY || tok == token::TOK_RAISE || tok == token::TOK_RANGE || tok == token::TOK_REFERENCES || tok == token::TOK_REGEXP || tok == token::TOK_REPLACE || tok == token::TOK_RESTRICT || tok == token::TOK_ROLLBACK || tok == token::TOK_ROWID || tok == token::TOK_ROWS || tok == token::TOK_SELECT || tok == token::TOK_SET || tok == token::TOK_TABLE || tok == token::TOK_TEMP || tok == token::TOK_TEMPORARY || tok == token::TOK_THEN || tok == token::TOK_TRUE || tok == token::TOK_UNBOUNDED || tok == token::TOK_UNIQUE || tok == token::TOK_UPDATE || tok == token::TOK_USING || tok == token::TOK_VIRTUAL || tok == token::TOK_WHEN || tok == token::TOK_WHERE || tok == token::TOK_WITHOUT || tok == token::TOK_IDENTIFIER || tok == token::TOK_NUMERIC || tok == token::TOK_STRINGLITERAL || tok == token::TOK_QUOTEDLITERAL || tok == token::TOK_BLOBLITERAL || tok == token::TOK_BINDPARAMETER);
      }
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YYASSERT (tok == token::TOK_ABORT || tok == token::TOK_ACTION || tok == token::TOK_AND || tok == token::TOK_AND_BETWEEN || tok == token::TOK_AS || tok == token::TOK_ASC || tok == token::TOK_AUTOINCREMENT || tok == token::TOK_BETWEEN || tok == token::TOK_CASCADE || tok == token::TOK_CASE || tok == token::TOK_CAST || tok == token::TOK_CHECK || tok == token::TOK_COLLATE || tok == token::TOK_CONFLICT || tok == token::TOK_CONSTRAINT || tok == token::TOK_CREATE || tok == token::TOK_CURRENT_DATE || tok == token::TOK_CURRENT_TIME || tok == token::TOK_CURRENT_TIMESTAMP || tok == token::TOK_DEFAULT || tok == token::TOK_DEFERRABLE || tok == token::TOK_DEFERRED || tok == token::TOK_DELETE || tok == token::TOK_DESC || tok == token::TOK_DISTINCT || tok == token::TOK_ELSE || tok == token::TOK_END || tok == token::TOK_ESCAPE || tok == token::TOK_EXISTS || tok == token::TOK_FAIL || tok == token::TOK_FALSE || tok == token::TOK_FILTER || tok == token::TOK_FOLLOWING || tok == token::TOK_FOREIGN || tok == token::TOK_GLOB || tok == token::TOK_IF || tok == token::TOK_IGNORE || tok == token::TOK_IMMEDIATE || tok == token::TOK_IN || tok == token::TOK_INDEX || tok == token::TOK_INITIALLY || tok == token::TOK_INSERT || tok == token::TOK_IS || tok == token::TOK_ISNULL || tok == token::TOK_KEY || tok == token::TOK_LIKE || tok == token::TOK_MATCH || tok == token::TOK_NO || tok == token::TOK_NOT || tok == token::TOK_NOTNULL || tok == token::TOK_NULL || tok == token::TOK_ON || tok == token::TOK_OR || tok == token::TOK_OVER || tok == token::TOK_PARTITION || tok == token::TOK_PRECEDING || tok == token::TOK_PRIMARY || tok == token::TOK_RAISE || tok == token::TOK_RANGE || tok == token::TOK_REFERENCES || tok == token::TOK_REGEXP || tok == token::TOK_REPLACE || tok == token::TOK_RESTRICT || tok == token::TOK_ROLLBACK || tok == token::TOK_ROWID || tok == token::TOK_ROWS || tok == token::TOK_SELECT || tok == token::TOK_SET || tok == token::TOK_TABLE || tok == token::TOK_TEMP || tok == token::TOK_TEMPORARY || tok == token::TOK_THEN || tok == token::TOK_TRUE || tok == token::TOK_UNBOUNDED || tok == token::TOK_UNIQUE || tok == token::TOK_UPDATE || tok == token::TOK_USING || tok == token::TOK_VIRTUAL || tok == token::TOK_WHEN || tok == token::TOK_WHERE || tok == token::TOK_WITHOUT || tok == token::TOK_IDENTIFIER || tok == token::TOK_NUMERIC || tok == token::TOK_STRINGLITERAL || tok == token::TOK_QUOTEDLITERAL || tok == token::TOK_BLOBLITERAL || tok == token::TOK_BINDPARAMETER);
      }
#endif
    };

    /// Build a parser object.
    parser (yyscan_t yyscanner_yyarg, ParserDriver& drv_yyarg);
    virtual ~parser ();

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    // Implementation of make_symbol for each symbol type.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EOF (location_type l)
      {
        return symbol_type (token::TOK_EOF, std::move (l));
      }
#else
      static
      symbol_type
      make_EOF (const location_type& l)
      {
        return symbol_type (token::TOK_EOF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LPAREN (location_type l)
      {
        return symbol_type (token::TOK_LPAREN, std::move (l));
      }
#else
      static
      symbol_type
      make_LPAREN (const location_type& l)
      {
        return symbol_type (token::TOK_LPAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RPAREN (location_type l)
      {
        return symbol_type (token::TOK_RPAREN, std::move (l));
      }
#else
      static
      symbol_type
      make_RPAREN (const location_type& l)
      {
        return symbol_type (token::TOK_RPAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DOT (location_type l)
      {
        return symbol_type (token::TOK_DOT, std::move (l));
      }
#else
      static
      symbol_type
      make_DOT (const location_type& l)
      {
        return symbol_type (token::TOK_DOT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMA (location_type l)
      {
        return symbol_type (token::TOK_COMMA, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMA (const location_type& l)
      {
        return symbol_type (token::TOK_COMMA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SEMI (location_type l)
      {
        return symbol_type (token::TOK_SEMI, std::move (l));
      }
#else
      static
      symbol_type
      make_SEMI (const location_type& l)
      {
        return symbol_type (token::TOK_SEMI, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PLUS (location_type l)
      {
        return symbol_type (token::TOK_PLUS, std::move (l));
      }
#else
      static
      symbol_type
      make_PLUS (const location_type& l)
      {
        return symbol_type (token::TOK_PLUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MINUS (location_type l)
      {
        return symbol_type (token::TOK_MINUS, std::move (l));
      }
#else
      static
      symbol_type
      make_MINUS (const location_type& l)
      {
        return symbol_type (token::TOK_MINUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STAR (location_type l)
      {
        return symbol_type (token::TOK_STAR, std::move (l));
      }
#else
      static
      symbol_type
      make_STAR (const location_type& l)
      {
        return symbol_type (token::TOK_STAR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SLASH (location_type l)
      {
        return symbol_type (token::TOK_SLASH, std::move (l));
      }
#else
      static
      symbol_type
      make_SLASH (const location_type& l)
      {
        return symbol_type (token::TOK_SLASH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TILDE (location_type l)
      {
        return symbol_type (token::TOK_TILDE, std::move (l));
      }
#else
      static
      symbol_type
      make_TILDE (const location_type& l)
      {
        return symbol_type (token::TOK_TILDE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AMPERSAND (location_type l)
      {
        return symbol_type (token::TOK_AMPERSAND, std::move (l));
      }
#else
      static
      symbol_type
      make_AMPERSAND (const location_type& l)
      {
        return symbol_type (token::TOK_AMPERSAND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PERCENT (location_type l)
      {
        return symbol_type (token::TOK_PERCENT, std::move (l));
      }
#else
      static
      symbol_type
      make_PERCENT (const location_type& l)
      {
        return symbol_type (token::TOK_PERCENT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BITOR (location_type l)
      {
        return symbol_type (token::TOK_BITOR, std::move (l));
      }
#else
      static
      symbol_type
      make_BITOR (const location_type& l)
      {
        return symbol_type (token::TOK_BITOR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OROP (location_type l)
      {
        return symbol_type (token::TOK_OROP, std::move (l));
      }
#else
      static
      symbol_type
      make_OROP (const location_type& l)
      {
        return symbol_type (token::TOK_OROP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EQUAL (location_type l)
      {
        return symbol_type (token::TOK_EQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_EQUAL (const location_type& l)
      {
        return symbol_type (token::TOK_EQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EQUAL2 (location_type l)
      {
        return symbol_type (token::TOK_EQUAL2, std::move (l));
      }
#else
      static
      symbol_type
      make_EQUAL2 (const location_type& l)
      {
        return symbol_type (token::TOK_EQUAL2, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GREATER (location_type l)
      {
        return symbol_type (token::TOK_GREATER, std::move (l));
      }
#else
      static
      symbol_type
      make_GREATER (const location_type& l)
      {
        return symbol_type (token::TOK_GREATER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GREATEREQUAL (location_type l)
      {
        return symbol_type (token::TOK_GREATEREQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_GREATEREQUAL (const location_type& l)
      {
        return symbol_type (token::TOK_GREATEREQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOWER (location_type l)
      {
        return symbol_type (token::TOK_LOWER, std::move (l));
      }
#else
      static
      symbol_type
      make_LOWER (const location_type& l)
      {
        return symbol_type (token::TOK_LOWER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOWEREQUAL (location_type l)
      {
        return symbol_type (token::TOK_LOWEREQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_LOWEREQUAL (const location_type& l)
      {
        return symbol_type (token::TOK_LOWEREQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UNEQUAL (location_type l)
      {
        return symbol_type (token::TOK_UNEQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_UNEQUAL (const location_type& l)
      {
        return symbol_type (token::TOK_UNEQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UNEQUAL2 (location_type l)
      {
        return symbol_type (token::TOK_UNEQUAL2, std::move (l));
      }
#else
      static
      symbol_type
      make_UNEQUAL2 (const location_type& l)
      {
        return symbol_type (token::TOK_UNEQUAL2, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BITWISELEFT (location_type l)
      {
        return symbol_type (token::TOK_BITWISELEFT, std::move (l));
      }
#else
      static
      symbol_type
      make_BITWISELEFT (const location_type& l)
      {
        return symbol_type (token::TOK_BITWISELEFT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BITWISERIGHT (location_type l)
      {
        return symbol_type (token::TOK_BITWISERIGHT, std::move (l));
      }
#else
      static
      symbol_type
      make_BITWISERIGHT (const location_type& l)
      {
        return symbol_type (token::TOK_BITWISERIGHT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ABORT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_ABORT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ABORT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_ABORT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ACTION (std::string v, location_type l)
      {
        return symbol_type (token::TOK_ACTION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ACTION (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_ACTION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AND (std::string v, location_type l)
      {
        return symbol_type (token::TOK_AND, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_AND (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_AND, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AND_BETWEEN (std::string v, location_type l)
      {
        return symbol_type (token::TOK_AND_BETWEEN, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_AND_BETWEEN (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_AND_BETWEEN, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AS (std::string v, location_type l)
      {
        return symbol_type (token::TOK_AS, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_AS (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_AS, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASC (std::string v, location_type l)
      {
        return symbol_type (token::TOK_ASC, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ASC (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_ASC, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AUTOINCREMENT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_AUTOINCREMENT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_AUTOINCREMENT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_AUTOINCREMENT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BETWEEN (std::string v, location_type l)
      {
        return symbol_type (token::TOK_BETWEEN, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_BETWEEN (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_BETWEEN, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CASCADE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_CASCADE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CASCADE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_CASCADE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CASE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_CASE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CASE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_CASE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CAST (std::string v, location_type l)
      {
        return symbol_type (token::TOK_CAST, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CAST (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_CAST, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CHECK (std::string v, location_type l)
      {
        return symbol_type (token::TOK_CHECK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CHECK (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_CHECK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COLLATE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_COLLATE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_COLLATE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_COLLATE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONFLICT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_CONFLICT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CONFLICT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_CONFLICT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONSTRAINT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_CONSTRAINT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CONSTRAINT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_CONSTRAINT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CREATE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_CREATE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CREATE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_CREATE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CURRENT_DATE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_CURRENT_DATE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CURRENT_DATE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_CURRENT_DATE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CURRENT_TIME (std::string v, location_type l)
      {
        return symbol_type (token::TOK_CURRENT_TIME, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CURRENT_TIME (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_CURRENT_TIME, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CURRENT_TIMESTAMP (std::string v, location_type l)
      {
        return symbol_type (token::TOK_CURRENT_TIMESTAMP, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CURRENT_TIMESTAMP (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_CURRENT_TIMESTAMP, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DEFAULT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_DEFAULT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DEFAULT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_DEFAULT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DEFERRABLE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_DEFERRABLE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DEFERRABLE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_DEFERRABLE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DEFERRED (std::string v, location_type l)
      {
        return symbol_type (token::TOK_DEFERRED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DEFERRED (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_DEFERRED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DELETE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_DELETE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DELETE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_DELETE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DESC (std::string v, location_type l)
      {
        return symbol_type (token::TOK_DESC, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DESC (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_DESC, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DISTINCT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_DISTINCT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DISTINCT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_DISTINCT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ELSE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_ELSE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ELSE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_ELSE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_END (std::string v, location_type l)
      {
        return symbol_type (token::TOK_END, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_END (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_END, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ESCAPE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_ESCAPE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ESCAPE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_ESCAPE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXISTS (std::string v, location_type l)
      {
        return symbol_type (token::TOK_EXISTS, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_EXISTS (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_EXISTS, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FAIL (std::string v, location_type l)
      {
        return symbol_type (token::TOK_FAIL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FAIL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_FAIL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FALSE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_FALSE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FALSE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_FALSE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FILTER (std::string v, location_type l)
      {
        return symbol_type (token::TOK_FILTER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FILTER (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_FILTER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FOLLOWING (std::string v, location_type l)
      {
        return symbol_type (token::TOK_FOLLOWING, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FOLLOWING (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_FOLLOWING, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FOREIGN (std::string v, location_type l)
      {
        return symbol_type (token::TOK_FOREIGN, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FOREIGN (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_FOREIGN, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GLOB (std::string v, location_type l)
      {
        return symbol_type (token::TOK_GLOB, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_GLOB (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_GLOB, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IF (std::string v, location_type l)
      {
        return symbol_type (token::TOK_IF, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IF (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_IF, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IGNORE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_IGNORE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IGNORE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_IGNORE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IMMEDIATE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_IMMEDIATE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IMMEDIATE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_IMMEDIATE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IN (std::string v, location_type l)
      {
        return symbol_type (token::TOK_IN, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IN (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_IN, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INDEX (std::string v, location_type l)
      {
        return symbol_type (token::TOK_INDEX, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INDEX (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_INDEX, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INITIALLY (std::string v, location_type l)
      {
        return symbol_type (token::TOK_INITIALLY, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INITIALLY (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_INITIALLY, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INSERT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_INSERT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INSERT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_INSERT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IS (std::string v, location_type l)
      {
        return symbol_type (token::TOK_IS, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IS (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_IS, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ISNULL (std::string v, location_type l)
      {
        return symbol_type (token::TOK_ISNULL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ISNULL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_ISNULL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KEY (std::string v, location_type l)
      {
        return symbol_type (token::TOK_KEY, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_KEY (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_KEY, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LIKE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_LIKE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_LIKE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_LIKE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MATCH (std::string v, location_type l)
      {
        return symbol_type (token::TOK_MATCH, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_MATCH (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_MATCH, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NO (std::string v, location_type l)
      {
        return symbol_type (token::TOK_NO, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NO (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_NO, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NOT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_NOT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NOT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_NOT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NOTNULL (std::string v, location_type l)
      {
        return symbol_type (token::TOK_NOTNULL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NOTNULL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_NOTNULL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NULL (std::string v, location_type l)
      {
        return symbol_type (token::TOK_NULL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NULL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_NULL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ON (std::string v, location_type l)
      {
        return symbol_type (token::TOK_ON, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ON (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_ON, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OR (std::string v, location_type l)
      {
        return symbol_type (token::TOK_OR, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_OR (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_OR, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OVER (std::string v, location_type l)
      {
        return symbol_type (token::TOK_OVER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_OVER (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_OVER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PARTITION (std::string v, location_type l)
      {
        return symbol_type (token::TOK_PARTITION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_PARTITION (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_PARTITION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PRECEDING (std::string v, location_type l)
      {
        return symbol_type (token::TOK_PRECEDING, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_PRECEDING (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_PRECEDING, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PRIMARY (std::string v, location_type l)
      {
        return symbol_type (token::TOK_PRIMARY, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_PRIMARY (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_PRIMARY, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RAISE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_RAISE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_RAISE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_RAISE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RANGE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_RANGE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_RANGE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_RANGE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_REFERENCES (std::string v, location_type l)
      {
        return symbol_type (token::TOK_REFERENCES, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_REFERENCES (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_REFERENCES, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_REGEXP (std::string v, location_type l)
      {
        return symbol_type (token::TOK_REGEXP, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_REGEXP (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_REGEXP, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_REPLACE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_REPLACE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_REPLACE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_REPLACE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RESTRICT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_RESTRICT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_RESTRICT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_RESTRICT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ROLLBACK (std::string v, location_type l)
      {
        return symbol_type (token::TOK_ROLLBACK, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ROLLBACK (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_ROLLBACK, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ROWID (std::string v, location_type l)
      {
        return symbol_type (token::TOK_ROWID, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ROWID (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_ROWID, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ROWS (std::string v, location_type l)
      {
        return symbol_type (token::TOK_ROWS, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ROWS (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_ROWS, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SELECT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_SELECT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_SELECT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_SELECT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SET (std::string v, location_type l)
      {
        return symbol_type (token::TOK_SET, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_SET (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_SET, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TABLE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_TABLE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_TABLE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_TABLE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEMP (std::string v, location_type l)
      {
        return symbol_type (token::TOK_TEMP, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_TEMP (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_TEMP, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEMPORARY (std::string v, location_type l)
      {
        return symbol_type (token::TOK_TEMPORARY, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_TEMPORARY (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_TEMPORARY, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_THEN (std::string v, location_type l)
      {
        return symbol_type (token::TOK_THEN, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_THEN (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_THEN, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TRUE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_TRUE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_TRUE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_TRUE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UNBOUNDED (std::string v, location_type l)
      {
        return symbol_type (token::TOK_UNBOUNDED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_UNBOUNDED (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_UNBOUNDED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UNIQUE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_UNIQUE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_UNIQUE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_UNIQUE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UPDATE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_UPDATE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_UPDATE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_UPDATE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_USING (std::string v, location_type l)
      {
        return symbol_type (token::TOK_USING, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_USING (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_USING, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VIRTUAL (std::string v, location_type l)
      {
        return symbol_type (token::TOK_VIRTUAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_VIRTUAL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_VIRTUAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WHEN (std::string v, location_type l)
      {
        return symbol_type (token::TOK_WHEN, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_WHEN (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_WHEN, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WHERE (std::string v, location_type l)
      {
        return symbol_type (token::TOK_WHERE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_WHERE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_WHERE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WITHOUT (std::string v, location_type l)
      {
        return symbol_type (token::TOK_WITHOUT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_WITHOUT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_WITHOUT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IDENTIFIER (std::string v, location_type l)
      {
        return symbol_type (token::TOK_IDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IDENTIFIER (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_IDENTIFIER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NUMERIC (std::string v, location_type l)
      {
        return symbol_type (token::TOK_NUMERIC, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NUMERIC (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_NUMERIC, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRINGLITERAL (std::string v, location_type l)
      {
        return symbol_type (token::TOK_STRINGLITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_STRINGLITERAL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_STRINGLITERAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_QUOTEDLITERAL (std::string v, location_type l)
      {
        return symbol_type (token::TOK_QUOTEDLITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_QUOTEDLITERAL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_QUOTEDLITERAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BLOBLITERAL (std::string v, location_type l)
      {
        return symbol_type (token::TOK_BLOBLITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_BLOBLITERAL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_BLOBLITERAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BINDPARAMETER (std::string v, location_type l)
      {
        return symbol_type (token::TOK_BINDPARAMETER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_BINDPARAMETER (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_BINDPARAMETER, v, l);
      }
#endif


  private:
    /// This class is not copyable.
    parser (const parser&);
    parser& operator= (const parser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const unsigned short yytable_[];

  static const short yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::reverse_iterator iterator;
      typedef typename S::const_reverse_iterator const_iterator;
      typedef typename S::size_type size_type;

      stack (size_type n = 200)
        : seq_ (n)
      {}

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (size_type i)
      {
        return seq_[size () - 1 - i];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (int i)
      {
        return operator[] (size_type (i));
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (size_type i) const
      {
        return seq_[size () - 1 - i];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (int i) const
      {
        return operator[] (size_type (i));
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (int n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      size_type
      size () const YY_NOEXCEPT
      {
        return seq_.size ();
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.rbegin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.rend ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, int range)
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (int i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        int range_;
      };

    private:
      stack (const stack&);
      stack& operator= (const stack&);
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 2538,     ///< Last index in yytable_.
      yynnts_ = 34,  ///< Number of nonterminal symbols.
      yyfinal_ = 9, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 114  ///< Number of tokens.
    };


    // User arguments.
    yyscan_t yyscanner;
    ParserDriver& drv;
  };

  inline
  parser::token_number_type
  parser::yytranslate_ (token_type t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113
    };
    const unsigned user_token_number_max_ = 368;
    const token_number_type undef_token_ = 2;

    if (static_cast<int> (t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value ()
    , location (std::move (that.location))
  {
    switch (this->type_get ())
    {
      case 138: // optional_if_not_exists
      case 140: // optional_unique
        value.move< bool > (std::move (that.value));
        break;

      case 145: // createindex_stmt
        value.move< sqlb::IndexPtr > (std::move (that.value));
        break;

      case 143: // indexed_column
        value.move< sqlb::IndexedColumn > (std::move (that.value));
        break;

      case 144: // indexed_column_list
        value.move< sqlb::IndexedColumnVector > (std::move (that.value));
        break;

      case 147: // createvirtualtable_stmt
        value.move< sqlb::TablePtr > (std::move (that.value));
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
        value.move< std::string > (std::move (that.value));
        break;

      default:
        break;
    }

  }
#endif

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->type_get ())
    {
      case 138: // optional_if_not_exists
      case 140: // optional_unique
        value.copy< bool > (YY_MOVE (that.value));
        break;

      case 145: // createindex_stmt
        value.copy< sqlb::IndexPtr > (YY_MOVE (that.value));
        break;

      case 143: // indexed_column
        value.copy< sqlb::IndexedColumn > (YY_MOVE (that.value));
        break;

      case 144: // indexed_column_list
        value.copy< sqlb::IndexedColumnVector > (YY_MOVE (that.value));
        break;

      case 147: // createvirtualtable_stmt
        value.copy< sqlb::TablePtr > (YY_MOVE (that.value));
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
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->type_get ())
    {
      case 138: // optional_if_not_exists
      case 140: // optional_unique
        value.move< bool > (YY_MOVE (s.value));
        break;

      case 145: // createindex_stmt
        value.move< sqlb::IndexPtr > (YY_MOVE (s.value));
        break;

      case 143: // indexed_column
        value.move< sqlb::IndexedColumn > (YY_MOVE (s.value));
        break;

      case 144: // indexed_column_list
        value.move< sqlb::IndexedColumnVector > (YY_MOVE (s.value));
        break;

      case 147: // createvirtualtable_stmt
        value.move< sqlb::TablePtr > (YY_MOVE (s.value));
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
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_type.
  inline
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  inline
  parser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  inline
  parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }

  inline
  parser::token_type
  parser::by_type::token () const YY_NOEXCEPT
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368
    };
    return token_type (yytoken_number_[type]);
  }

#line 10 "sqlite3_parser.yy"
} } //  sqlb::parser 
#line 3688 "sqlite3_parser.hpp"





#endif // !YY_YY_SQLITE3_PARSER_HPP_INCLUDED
