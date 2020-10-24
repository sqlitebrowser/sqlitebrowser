// A Bison parser, made by GNU Bison 3.7.2.

// Skeleton interface for Bison LALR(1) parsers in C++

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


/**
 ** \file sqlite3_parser.hpp
 ** Define the  sqlb::parser ::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_SQLITE3_PARSER_HPP_INCLUDED
# define YY_YY_SQLITE3_PARSER_HPP_INCLUDED
// "%code requires" blocks.
#line 12 "sqlite3_parser.yy"

	#include <string>
	#include <tuple>
	#include "../sqlitetypes.h"
	#include "../ObjectIdentifier.h"
	namespace sqlb { namespace parser { class ParserDriver; } }
	typedef void* yyscan_t;

	// Colum definitions are a tuple of two elements: the Field object and a set of table constraints
	using ColumndefData = std::tuple<sqlb::Field, sqlb::ConstraintSet>;

#line 61 "sqlite3_parser.hpp"

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
#ifndef YY_ASSERT
# include <cassert>
# define YY_ASSERT assert
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
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
#line 196 "sqlite3_parser.hpp"




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
      YY_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    semantic_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~semantic_type () YY_NOEXCEPT
    {
      YY_ASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
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
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
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
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == *that.yytypeid_);
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
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    semantic_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

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
      // columndef
      char dummy1[sizeof (ColumndefData)];

      // optional_if_not_exists
      // optional_unique
      // optional_temporary
      // optional_withoutrowid
      // optional_always_generated
      char dummy2[sizeof (bool)];

      // columnconstraint
      // tableconstraint
      char dummy3[sizeof (sqlb::ConstraintPtr)];

      // columnconstraint_list
      // tableconstraint_list
      // optional_tableconstraint_list
      char dummy4[sizeof (sqlb::ConstraintSet)];

      // createindex_stmt
      char dummy5[sizeof (sqlb::IndexPtr)];

      // indexed_column
      char dummy6[sizeof (sqlb::IndexedColumn)];

      // indexed_column_list
      char dummy7[sizeof (sqlb::IndexedColumnVector)];

      // columnid_list
      // optional_columnid_with_paren_list
      char dummy8[sizeof (sqlb::StringVector)];

      // createvirtualtable_stmt
      // createtable_stmt
      char dummy9[sizeof (sqlb::TablePtr)];

      // "ABORT"
      // "ACTION"
      // "ALWAYS"
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
      // "GENERATED"
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
      // "STORED"
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
      // columnid
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
      // optional_conflictclause
      // optional_typename
      // optional_storage_identifier
      // optional_constraintname
      // fk_clause_part
      // fk_clause_part_list
      // optional_fk_clause
      char dummy10[sizeof (std::string)];

      // columndef_list
      char dummy11[sizeof (std::vector<ColumndefData>)];
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

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        TOK_YYEMPTY = -2,
    TOK_EOF = 0,                   // "end of file"
    TOK_YYerror = 256,             // error
    TOK_YYUNDEF = 257,             // "invalid token"
    TOK_LPAREN = 258,              // "("
    TOK_RPAREN = 259,              // ")"
    TOK_DOT = 260,                 // "."
    TOK_COMMA = 261,               // ","
    TOK_SEMI = 262,                // ";"
    TOK_PLUS = 263,                // "+"
    TOK_MINUS = 264,               // "-"
    TOK_STAR = 265,                // "*"
    TOK_SLASH = 266,               // "/"
    TOK_TILDE = 267,               // "~"
    TOK_AMPERSAND = 268,           // "&"
    TOK_PERCENT = 269,             // "%"
    TOK_BITOR = 270,               // "|"
    TOK_OROP = 271,                // "||"
    TOK_EQUAL = 272,               // "="
    TOK_EQUAL2 = 273,              // "=="
    TOK_GREATER = 274,             // ">"
    TOK_GREATEREQUAL = 275,        // ">="
    TOK_LOWER = 276,               // "<"
    TOK_LOWEREQUAL = 277,          // "<="
    TOK_UNEQUAL = 278,             // "!="
    TOK_UNEQUAL2 = 279,            // "<>"
    TOK_BITWISELEFT = 280,         // "<<"
    TOK_BITWISERIGHT = 281,        // ">>"
    TOK_ABORT = 282,               // "ABORT"
    TOK_ACTION = 283,              // "ACTION"
    TOK_ALWAYS = 284,              // "ALWAYS"
    TOK_AND = 285,                 // "AND"
    TOK_AND_BETWEEN = 286,         // "AND BETWEEN"
    TOK_AS = 287,                  // "AS"
    TOK_ASC = 288,                 // "ASC"
    TOK_AUTOINCREMENT = 289,       // "AUTOINCREMENT"
    TOK_BETWEEN = 290,             // "BETWEEN"
    TOK_CASCADE = 291,             // "CASCADE"
    TOK_CASE = 292,                // "CASE"
    TOK_CAST = 293,                // "CAST"
    TOK_CHECK = 294,               // "CHECK"
    TOK_COLLATE = 295,             // "COLLATE"
    TOK_CONFLICT = 296,            // "CONFLICT"
    TOK_CONSTRAINT = 297,          // "CONSTRAINT"
    TOK_CREATE = 298,              // "CREATE"
    TOK_CURRENT_DATE = 299,        // "CURRENT_DATE"
    TOK_CURRENT_TIME = 300,        // "CURRENT_TIME"
    TOK_CURRENT_TIMESTAMP = 301,   // "CURRENT_TIMESTAMP"
    TOK_DEFAULT = 302,             // "DEFAULT"
    TOK_DEFERRABLE = 303,          // "DEFERRABLE"
    TOK_DEFERRED = 304,            // "DEFERRED"
    TOK_DELETE = 305,              // "DELETE"
    TOK_DESC = 306,                // "DESC"
    TOK_DISTINCT = 307,            // "DISTINCT"
    TOK_ELSE = 308,                // "ELSE"
    TOK_END = 309,                 // "END"
    TOK_ESCAPE = 310,              // "ESCAPE"
    TOK_EXISTS = 311,              // "EXISTS"
    TOK_FAIL = 312,                // "FAIL"
    TOK_FALSE = 313,               // "FALSE"
    TOK_FILTER = 314,              // "FILTER"
    TOK_FOLLOWING = 315,           // "FOLLOWING"
    TOK_FOREIGN = 316,             // "FOREIGN"
    TOK_GENERATED = 317,           // "GENERATED"
    TOK_GLOB = 318,                // "GLOB"
    TOK_IF = 319,                  // "IF"
    TOK_IGNORE = 320,              // "IGNORE"
    TOK_IMMEDIATE = 321,           // "IMMEDIATE"
    TOK_IN = 322,                  // "IN"
    TOK_INDEX = 323,               // "INDEX"
    TOK_INITIALLY = 324,           // "INITIALLY"
    TOK_INSERT = 325,              // "INSERT"
    TOK_IS = 326,                  // "IS"
    TOK_ISNULL = 327,              // "ISNULL"
    TOK_KEY = 328,                 // "KEY"
    TOK_LIKE = 329,                // "LIKE"
    TOK_MATCH = 330,               // "MATCH"
    TOK_NO = 331,                  // "NO"
    TOK_NOT = 332,                 // "NOT"
    TOK_NOTNULL = 333,             // "NOTNULL"
    TOK_NULL = 334,                // "NULL"
    TOK_ON = 335,                  // "ON"
    TOK_OR = 336,                  // "OR"
    TOK_OVER = 337,                // "OVER"
    TOK_PARTITION = 338,           // "PARTITION"
    TOK_PRECEDING = 339,           // "PRECEDING"
    TOK_PRIMARY = 340,             // "PRIMARY"
    TOK_RAISE = 341,               // "RAISE"
    TOK_RANGE = 342,               // "RANGE"
    TOK_REFERENCES = 343,          // "REFERENCES"
    TOK_REGEXP = 344,              // "REGEXP"
    TOK_REPLACE = 345,             // "REPLACE"
    TOK_RESTRICT = 346,            // "RESTRICT"
    TOK_ROLLBACK = 347,            // "ROLLBACK"
    TOK_ROWID = 348,               // "ROWID"
    TOK_ROWS = 349,                // "ROWS"
    TOK_SELECT = 350,              // "SELECT"
    TOK_SET = 351,                 // "SET"
    TOK_STORED = 352,              // "STORED"
    TOK_TABLE = 353,               // "TABLE"
    TOK_TEMP = 354,                // "TEMP"
    TOK_TEMPORARY = 355,           // "TEMPORARY"
    TOK_THEN = 356,                // "THEN"
    TOK_TRUE = 357,                // "TRUE"
    TOK_UNBOUNDED = 358,           // "UNBOUNDED"
    TOK_UNIQUE = 359,              // "UNIQUE"
    TOK_UPDATE = 360,              // "UPDATE"
    TOK_USING = 361,               // "USING"
    TOK_VIRTUAL = 362,             // "VIRTUAL"
    TOK_WHEN = 363,                // "WHEN"
    TOK_WHERE = 364,               // "WHERE"
    TOK_WITHOUT = 365,             // "WITHOUT"
    TOK_IDENTIFIER = 366,          // "identifier"
    TOK_NUMERIC = 367,             // "numeric"
    TOK_STRINGLITERAL = 368,       // "string literal"
    TOK_QUOTEDLITERAL = 369,       // "quoted literal"
    TOK_BLOBLITERAL = 370,         // "blob literal"
    TOK_BINDPARAMETER = 371        // "bind parameter"
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::yytokentype token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 117, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_LPAREN = 3,                            // "("
        S_RPAREN = 4,                            // ")"
        S_DOT = 5,                               // "."
        S_COMMA = 6,                             // ","
        S_SEMI = 7,                              // ";"
        S_PLUS = 8,                              // "+"
        S_MINUS = 9,                             // "-"
        S_STAR = 10,                             // "*"
        S_SLASH = 11,                            // "/"
        S_TILDE = 12,                            // "~"
        S_AMPERSAND = 13,                        // "&"
        S_PERCENT = 14,                          // "%"
        S_BITOR = 15,                            // "|"
        S_OROP = 16,                             // "||"
        S_EQUAL = 17,                            // "="
        S_EQUAL2 = 18,                           // "=="
        S_GREATER = 19,                          // ">"
        S_GREATEREQUAL = 20,                     // ">="
        S_LOWER = 21,                            // "<"
        S_LOWEREQUAL = 22,                       // "<="
        S_UNEQUAL = 23,                          // "!="
        S_UNEQUAL2 = 24,                         // "<>"
        S_BITWISELEFT = 25,                      // "<<"
        S_BITWISERIGHT = 26,                     // ">>"
        S_ABORT = 27,                            // "ABORT"
        S_ACTION = 28,                           // "ACTION"
        S_ALWAYS = 29,                           // "ALWAYS"
        S_AND = 30,                              // "AND"
        S_AND_BETWEEN = 31,                      // "AND BETWEEN"
        S_AS = 32,                               // "AS"
        S_ASC = 33,                              // "ASC"
        S_AUTOINCREMENT = 34,                    // "AUTOINCREMENT"
        S_BETWEEN = 35,                          // "BETWEEN"
        S_CASCADE = 36,                          // "CASCADE"
        S_CASE = 37,                             // "CASE"
        S_CAST = 38,                             // "CAST"
        S_CHECK = 39,                            // "CHECK"
        S_COLLATE = 40,                          // "COLLATE"
        S_CONFLICT = 41,                         // "CONFLICT"
        S_CONSTRAINT = 42,                       // "CONSTRAINT"
        S_CREATE = 43,                           // "CREATE"
        S_CURRENT_DATE = 44,                     // "CURRENT_DATE"
        S_CURRENT_TIME = 45,                     // "CURRENT_TIME"
        S_CURRENT_TIMESTAMP = 46,                // "CURRENT_TIMESTAMP"
        S_DEFAULT = 47,                          // "DEFAULT"
        S_DEFERRABLE = 48,                       // "DEFERRABLE"
        S_DEFERRED = 49,                         // "DEFERRED"
        S_DELETE = 50,                           // "DELETE"
        S_DESC = 51,                             // "DESC"
        S_DISTINCT = 52,                         // "DISTINCT"
        S_ELSE = 53,                             // "ELSE"
        S_END = 54,                              // "END"
        S_ESCAPE = 55,                           // "ESCAPE"
        S_EXISTS = 56,                           // "EXISTS"
        S_FAIL = 57,                             // "FAIL"
        S_FALSE = 58,                            // "FALSE"
        S_FILTER = 59,                           // "FILTER"
        S_FOLLOWING = 60,                        // "FOLLOWING"
        S_FOREIGN = 61,                          // "FOREIGN"
        S_GENERATED = 62,                        // "GENERATED"
        S_GLOB = 63,                             // "GLOB"
        S_IF = 64,                               // "IF"
        S_IGNORE = 65,                           // "IGNORE"
        S_IMMEDIATE = 66,                        // "IMMEDIATE"
        S_IN = 67,                               // "IN"
        S_INDEX = 68,                            // "INDEX"
        S_INITIALLY = 69,                        // "INITIALLY"
        S_INSERT = 70,                           // "INSERT"
        S_IS = 71,                               // "IS"
        S_ISNULL = 72,                           // "ISNULL"
        S_KEY = 73,                              // "KEY"
        S_LIKE = 74,                             // "LIKE"
        S_MATCH = 75,                            // "MATCH"
        S_NO = 76,                               // "NO"
        S_NOT = 77,                              // "NOT"
        S_NOTNULL = 78,                          // "NOTNULL"
        S_NULL = 79,                             // "NULL"
        S_ON = 80,                               // "ON"
        S_OR = 81,                               // "OR"
        S_OVER = 82,                             // "OVER"
        S_PARTITION = 83,                        // "PARTITION"
        S_PRECEDING = 84,                        // "PRECEDING"
        S_PRIMARY = 85,                          // "PRIMARY"
        S_RAISE = 86,                            // "RAISE"
        S_RANGE = 87,                            // "RANGE"
        S_REFERENCES = 88,                       // "REFERENCES"
        S_REGEXP = 89,                           // "REGEXP"
        S_REPLACE = 90,                          // "REPLACE"
        S_RESTRICT = 91,                         // "RESTRICT"
        S_ROLLBACK = 92,                         // "ROLLBACK"
        S_ROWID = 93,                            // "ROWID"
        S_ROWS = 94,                             // "ROWS"
        S_SELECT = 95,                           // "SELECT"
        S_SET = 96,                              // "SET"
        S_STORED = 97,                           // "STORED"
        S_TABLE = 98,                            // "TABLE"
        S_TEMP = 99,                             // "TEMP"
        S_TEMPORARY = 100,                       // "TEMPORARY"
        S_THEN = 101,                            // "THEN"
        S_TRUE = 102,                            // "TRUE"
        S_UNBOUNDED = 103,                       // "UNBOUNDED"
        S_UNIQUE = 104,                          // "UNIQUE"
        S_UPDATE = 105,                          // "UPDATE"
        S_USING = 106,                           // "USING"
        S_VIRTUAL = 107,                         // "VIRTUAL"
        S_WHEN = 108,                            // "WHEN"
        S_WHERE = 109,                           // "WHERE"
        S_WITHOUT = 110,                         // "WITHOUT"
        S_IDENTIFIER = 111,                      // "identifier"
        S_NUMERIC = 112,                         // "numeric"
        S_STRINGLITERAL = 113,                   // "string literal"
        S_QUOTEDLITERAL = 114,                   // "quoted literal"
        S_BLOBLITERAL = 115,                     // "blob literal"
        S_BINDPARAMETER = 116,                   // "bind parameter"
        S_YYACCEPT = 117,                        // $accept
        S_sql = 118,                             // sql
        S_statement = 119,                       // statement
        S_literalvalue = 120,                    // literalvalue
        S_id = 121,                              // id
        S_allowed_keywords_as_identifier = 122,  // allowed_keywords_as_identifier
        S_tableid = 123,                         // tableid
        S_columnid = 124,                        // columnid
        S_signednumber = 125,                    // signednumber
        S_signednumber_or_numeric = 126,         // signednumber_or_numeric
        S_typename_namelist = 127,               // typename_namelist
        S_type_name = 128,                       // type_name
        S_unary_expr = 129,                      // unary_expr
        S_binary_expr = 130,                     // binary_expr
        S_like_expr = 131,                       // like_expr
        S_exprlist_expr = 132,                   // exprlist_expr
        S_function_expr = 133,                   // function_expr
        S_isnull_expr = 134,                     // isnull_expr
        S_between_expr = 135,                    // between_expr
        S_in_expr = 136,                         // in_expr
        S_whenthenlist_expr = 137,               // whenthenlist_expr
        S_case_expr = 138,                       // case_expr
        S_raise_expr = 139,                      // raise_expr
        S_expr = 140,                            // expr
        S_select_stmt = 141,                     // select_stmt
        S_optional_if_not_exists = 142,          // optional_if_not_exists
        S_optional_sort_order = 143,             // optional_sort_order
        S_optional_unique = 144,                 // optional_unique
        S_optional_where = 145,                  // optional_where
        S_tableid_with_uninteresting_schema = 146, // tableid_with_uninteresting_schema
        S_indexed_column = 147,                  // indexed_column
        S_indexed_column_list = 148,             // indexed_column_list
        S_createindex_stmt = 149,                // createindex_stmt
        S_optional_exprlist_with_paren = 150,    // optional_exprlist_with_paren
        S_createvirtualtable_stmt = 151,         // createvirtualtable_stmt
        S_optional_temporary = 152,              // optional_temporary
        S_optional_withoutrowid = 153,           // optional_withoutrowid
        S_optional_conflictclause = 154,         // optional_conflictclause
        S_optional_typename = 155,               // optional_typename
        S_optional_storage_identifier = 156,     // optional_storage_identifier
        S_optional_always_generated = 157,       // optional_always_generated
        S_columnconstraint = 158,                // columnconstraint
        S_columnconstraint_list = 159,           // columnconstraint_list
        S_columndef = 160,                       // columndef
        S_columndef_list = 161,                  // columndef_list
        S_optional_constraintname = 162,         // optional_constraintname
        S_columnid_list = 163,                   // columnid_list
        S_optional_columnid_with_paren_list = 164, // optional_columnid_with_paren_list
        S_fk_clause_part = 165,                  // fk_clause_part
        S_fk_clause_part_list = 166,             // fk_clause_part_list
        S_optional_fk_clause = 167,              // optional_fk_clause
        S_tableconstraint = 168,                 // tableconstraint
        S_tableconstraint_list = 169,            // tableconstraint_list
        S_optional_tableconstraint_list = 170,   // optional_tableconstraint_list
        S_createtable_stmt = 171                 // createtable_stmt
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
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
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
        , location (std::move (that.location))
      {
        switch (this->kind ())
    {
      case symbol_kind::S_columndef: // columndef
        value.move< ColumndefData > (std::move (that.value));
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_withoutrowid: // optional_withoutrowid
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.move< bool > (std::move (that.value));
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
      case symbol_kind::S_tableconstraint: // tableconstraint
        value.move< sqlb::ConstraintPtr > (std::move (that.value));
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.move< sqlb::ConstraintSet > (std::move (that.value));
        break;

      case symbol_kind::S_createindex_stmt: // createindex_stmt
        value.move< sqlb::IndexPtr > (std::move (that.value));
        break;

      case symbol_kind::S_indexed_column: // indexed_column
        value.move< sqlb::IndexedColumn > (std::move (that.value));
        break;

      case symbol_kind::S_indexed_column_list: // indexed_column_list
        value.move< sqlb::IndexedColumnVector > (std::move (that.value));
        break;

      case symbol_kind::S_columnid_list: // columnid_list
      case symbol_kind::S_optional_columnid_with_paren_list: // optional_columnid_with_paren_list
        value.move< sqlb::StringVector > (std::move (that.value));
        break;

      case symbol_kind::S_createvirtualtable_stmt: // createvirtualtable_stmt
      case symbol_kind::S_createtable_stmt: // createtable_stmt
        value.move< sqlb::TablePtr > (std::move (that.value));
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
        value.move< std::string > (std::move (that.value));
        break;

      case symbol_kind::S_columndef_list: // columndef_list
        value.move< std::vector<ColumndefData> > (std::move (that.value));
        break;

      default:
        break;
    }

      }
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
      basic_symbol (typename Base::kind_type t, ColumndefData&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ColumndefData& v, const location_type& l)
        : Base (t)
        , value (v)
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
      basic_symbol (typename Base::kind_type t, sqlb::ConstraintPtr&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sqlb::ConstraintPtr& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sqlb::ConstraintSet&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sqlb::ConstraintSet& v, const location_type& l)
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
      basic_symbol (typename Base::kind_type t, sqlb::StringVector&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sqlb::StringVector& v, const location_type& l)
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
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<ColumndefData>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<ColumndefData>& v, const location_type& l)
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
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_columndef: // columndef
        value.template destroy< ColumndefData > ();
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_withoutrowid: // optional_withoutrowid
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.template destroy< bool > ();
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
      case symbol_kind::S_tableconstraint: // tableconstraint
        value.template destroy< sqlb::ConstraintPtr > ();
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.template destroy< sqlb::ConstraintSet > ();
        break;

      case symbol_kind::S_createindex_stmt: // createindex_stmt
        value.template destroy< sqlb::IndexPtr > ();
        break;

      case symbol_kind::S_indexed_column: // indexed_column
        value.template destroy< sqlb::IndexedColumn > ();
        break;

      case symbol_kind::S_indexed_column_list: // indexed_column_list
        value.template destroy< sqlb::IndexedColumnVector > ();
        break;

      case symbol_kind::S_columnid_list: // columnid_list
      case symbol_kind::S_optional_columnid_with_paren_list: // optional_columnid_with_paren_list
        value.template destroy< sqlb::StringVector > ();
        break;

      case symbol_kind::S_createvirtualtable_stmt: // createvirtualtable_stmt
      case symbol_kind::S_createtable_stmt: // createtable_stmt
        value.template destroy< sqlb::TablePtr > ();
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
        value.template destroy< std::string > ();
        break;

      case symbol_kind::S_columndef_list: // columndef_list
        value.template destroy< std::vector<ColumndefData> > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// The user-facing name of this symbol.
      std::string name () const YY_NOEXCEPT
      {
        return parser::symbol_name (this->kind ());
      }

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

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
    struct by_kind
    {
      /// Default constructor.
      by_kind ();

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that);
#endif

      /// Copy constructor.
      by_kind (const by_kind& that);

      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type(token_type (tok), std::move (l))
      {
        YY_ASSERT (tok == token::TOK_EOF || tok == token::TOK_YYerror || tok == token::TOK_YYUNDEF || tok == token::TOK_LPAREN || tok == token::TOK_RPAREN || tok == token::TOK_DOT || tok == token::TOK_COMMA || tok == token::TOK_SEMI || tok == token::TOK_PLUS || tok == token::TOK_MINUS || tok == token::TOK_STAR || tok == token::TOK_SLASH || tok == token::TOK_TILDE || tok == token::TOK_AMPERSAND || tok == token::TOK_PERCENT || tok == token::TOK_BITOR || tok == token::TOK_OROP || tok == token::TOK_EQUAL || tok == token::TOK_EQUAL2 || tok == token::TOK_GREATER || tok == token::TOK_GREATEREQUAL || tok == token::TOK_LOWER || tok == token::TOK_LOWEREQUAL || tok == token::TOK_UNEQUAL || tok == token::TOK_UNEQUAL2 || tok == token::TOK_BITWISELEFT || tok == token::TOK_BITWISERIGHT);
      }
#else
      symbol_type (int tok, const location_type& l)
        : super_type(token_type (tok), l)
      {
        YY_ASSERT (tok == token::TOK_EOF || tok == token::TOK_YYerror || tok == token::TOK_YYUNDEF || tok == token::TOK_LPAREN || tok == token::TOK_RPAREN || tok == token::TOK_DOT || tok == token::TOK_COMMA || tok == token::TOK_SEMI || tok == token::TOK_PLUS || tok == token::TOK_MINUS || tok == token::TOK_STAR || tok == token::TOK_SLASH || tok == token::TOK_TILDE || tok == token::TOK_AMPERSAND || tok == token::TOK_PERCENT || tok == token::TOK_BITOR || tok == token::TOK_OROP || tok == token::TOK_EQUAL || tok == token::TOK_EQUAL2 || tok == token::TOK_GREATER || tok == token::TOK_GREATEREQUAL || tok == token::TOK_LOWER || tok == token::TOK_LOWEREQUAL || tok == token::TOK_UNEQUAL || tok == token::TOK_UNEQUAL2 || tok == token::TOK_BITWISELEFT || tok == token::TOK_BITWISERIGHT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YY_ASSERT (tok == token::TOK_ABORT || tok == token::TOK_ACTION || tok == token::TOK_ALWAYS || tok == token::TOK_AND || tok == token::TOK_AND_BETWEEN || tok == token::TOK_AS || tok == token::TOK_ASC || tok == token::TOK_AUTOINCREMENT || tok == token::TOK_BETWEEN || tok == token::TOK_CASCADE || tok == token::TOK_CASE || tok == token::TOK_CAST || tok == token::TOK_CHECK || tok == token::TOK_COLLATE || tok == token::TOK_CONFLICT || tok == token::TOK_CONSTRAINT || tok == token::TOK_CREATE || tok == token::TOK_CURRENT_DATE || tok == token::TOK_CURRENT_TIME || tok == token::TOK_CURRENT_TIMESTAMP || tok == token::TOK_DEFAULT || tok == token::TOK_DEFERRABLE || tok == token::TOK_DEFERRED || tok == token::TOK_DELETE || tok == token::TOK_DESC || tok == token::TOK_DISTINCT || tok == token::TOK_ELSE || tok == token::TOK_END || tok == token::TOK_ESCAPE || tok == token::TOK_EXISTS || tok == token::TOK_FAIL || tok == token::TOK_FALSE || tok == token::TOK_FILTER || tok == token::TOK_FOLLOWING || tok == token::TOK_FOREIGN || tok == token::TOK_GENERATED || tok == token::TOK_GLOB || tok == token::TOK_IF || tok == token::TOK_IGNORE || tok == token::TOK_IMMEDIATE || tok == token::TOK_IN || tok == token::TOK_INDEX || tok == token::TOK_INITIALLY || tok == token::TOK_INSERT || tok == token::TOK_IS || tok == token::TOK_ISNULL || tok == token::TOK_KEY || tok == token::TOK_LIKE || tok == token::TOK_MATCH || tok == token::TOK_NO || tok == token::TOK_NOT || tok == token::TOK_NOTNULL || tok == token::TOK_NULL || tok == token::TOK_ON || tok == token::TOK_OR || tok == token::TOK_OVER || tok == token::TOK_PARTITION || tok == token::TOK_PRECEDING || tok == token::TOK_PRIMARY || tok == token::TOK_RAISE || tok == token::TOK_RANGE || tok == token::TOK_REFERENCES || tok == token::TOK_REGEXP || tok == token::TOK_REPLACE || tok == token::TOK_RESTRICT || tok == token::TOK_ROLLBACK || tok == token::TOK_ROWID || tok == token::TOK_ROWS || tok == token::TOK_SELECT || tok == token::TOK_SET || tok == token::TOK_STORED || tok == token::TOK_TABLE || tok == token::TOK_TEMP || tok == token::TOK_TEMPORARY || tok == token::TOK_THEN || tok == token::TOK_TRUE || tok == token::TOK_UNBOUNDED || tok == token::TOK_UNIQUE || tok == token::TOK_UPDATE || tok == token::TOK_USING || tok == token::TOK_VIRTUAL || tok == token::TOK_WHEN || tok == token::TOK_WHERE || tok == token::TOK_WITHOUT || tok == token::TOK_IDENTIFIER || tok == token::TOK_NUMERIC || tok == token::TOK_STRINGLITERAL || tok == token::TOK_QUOTEDLITERAL || tok == token::TOK_BLOBLITERAL || tok == token::TOK_BINDPARAMETER);
      }
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YY_ASSERT (tok == token::TOK_ABORT || tok == token::TOK_ACTION || tok == token::TOK_ALWAYS || tok == token::TOK_AND || tok == token::TOK_AND_BETWEEN || tok == token::TOK_AS || tok == token::TOK_ASC || tok == token::TOK_AUTOINCREMENT || tok == token::TOK_BETWEEN || tok == token::TOK_CASCADE || tok == token::TOK_CASE || tok == token::TOK_CAST || tok == token::TOK_CHECK || tok == token::TOK_COLLATE || tok == token::TOK_CONFLICT || tok == token::TOK_CONSTRAINT || tok == token::TOK_CREATE || tok == token::TOK_CURRENT_DATE || tok == token::TOK_CURRENT_TIME || tok == token::TOK_CURRENT_TIMESTAMP || tok == token::TOK_DEFAULT || tok == token::TOK_DEFERRABLE || tok == token::TOK_DEFERRED || tok == token::TOK_DELETE || tok == token::TOK_DESC || tok == token::TOK_DISTINCT || tok == token::TOK_ELSE || tok == token::TOK_END || tok == token::TOK_ESCAPE || tok == token::TOK_EXISTS || tok == token::TOK_FAIL || tok == token::TOK_FALSE || tok == token::TOK_FILTER || tok == token::TOK_FOLLOWING || tok == token::TOK_FOREIGN || tok == token::TOK_GENERATED || tok == token::TOK_GLOB || tok == token::TOK_IF || tok == token::TOK_IGNORE || tok == token::TOK_IMMEDIATE || tok == token::TOK_IN || tok == token::TOK_INDEX || tok == token::TOK_INITIALLY || tok == token::TOK_INSERT || tok == token::TOK_IS || tok == token::TOK_ISNULL || tok == token::TOK_KEY || tok == token::TOK_LIKE || tok == token::TOK_MATCH || tok == token::TOK_NO || tok == token::TOK_NOT || tok == token::TOK_NOTNULL || tok == token::TOK_NULL || tok == token::TOK_ON || tok == token::TOK_OR || tok == token::TOK_OVER || tok == token::TOK_PARTITION || tok == token::TOK_PRECEDING || tok == token::TOK_PRIMARY || tok == token::TOK_RAISE || tok == token::TOK_RANGE || tok == token::TOK_REFERENCES || tok == token::TOK_REGEXP || tok == token::TOK_REPLACE || tok == token::TOK_RESTRICT || tok == token::TOK_ROLLBACK || tok == token::TOK_ROWID || tok == token::TOK_ROWS || tok == token::TOK_SELECT || tok == token::TOK_SET || tok == token::TOK_STORED || tok == token::TOK_TABLE || tok == token::TOK_TEMP || tok == token::TOK_TEMPORARY || tok == token::TOK_THEN || tok == token::TOK_TRUE || tok == token::TOK_UNBOUNDED || tok == token::TOK_UNIQUE || tok == token::TOK_UPDATE || tok == token::TOK_USING || tok == token::TOK_VIRTUAL || tok == token::TOK_WHEN || tok == token::TOK_WHERE || tok == token::TOK_WITHOUT || tok == token::TOK_IDENTIFIER || tok == token::TOK_NUMERIC || tok == token::TOK_STRINGLITERAL || tok == token::TOK_QUOTEDLITERAL || tok == token::TOK_BLOBLITERAL || tok == token::TOK_BINDPARAMETER);
      }
#endif
    };

    /// Build a parser object.
    parser (yyscan_t yyscanner_yyarg, ParserDriver& drv_yyarg);
    virtual ~parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    parser (const parser&) = delete;
    /// Non copyable.
    parser& operator= (const parser&) = delete;
#endif

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

    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static std::string symbol_name (symbol_kind_type yysymbol);

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
      make_YYerror (location_type l)
      {
        return symbol_type (token::TOK_YYerror, std::move (l));
      }
#else
      static
      symbol_type
      make_YYerror (const location_type& l)
      {
        return symbol_type (token::TOK_YYerror, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYUNDEF (location_type l)
      {
        return symbol_type (token::TOK_YYUNDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYUNDEF (const location_type& l)
      {
        return symbol_type (token::TOK_YYUNDEF, l);
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
      make_ALWAYS (std::string v, location_type l)
      {
        return symbol_type (token::TOK_ALWAYS, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ALWAYS (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_ALWAYS, v, l);
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
      make_GENERATED (std::string v, location_type l)
      {
        return symbol_type (token::TOK_GENERATED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_GENERATED (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_GENERATED, v, l);
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
      make_STORED (std::string v, location_type l)
      {
        return symbol_type (token::TOK_STORED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_STORED (const std::string& v, const location_type& l)
      {
        return symbol_type (token::TOK_STORED, v, l);
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


    class context
    {
    public:
      context (const parser& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead () const { return yyla_; }
      symbol_kind_type token () const { return yyla_.kind (); }
      const location_type& location () const { return yyla_.location; }

      /// Put in YYARG at most YYARGN of the expected tokens, and return the
      /// number of tokens stored in YYARG.  If YYARG is null, return the
      /// number of expected tokens (guaranteed to be less than YYNTOKENS).
      int expected_tokens (symbol_kind_type yyarg[], int yyargn) const;

    private:
      const parser& yyparser_;
      const symbol_type& yyla_;
    };

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    parser (const parser&);
    /// Non copyable.
    parser& operator= (const parser&);
#endif


    /// Stored state numbers (used for stacks).
    typedef short state_type;

    /// The arguments of the error message.
    int yy_syntax_error_arguments_ (const context& yyctx,
                                    symbol_kind_type yyarg[], int yyargn) const;

    /// Generate an error message.
    /// \param yyctx     the context in which the error occurred.
    virtual std::string yysyntax_error_ (const context& yyctx) const;
    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const short yypact_ninf_;
    static const short yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_type enum.
    static symbol_kind_type yytranslate_ (int t);

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *yystr);

    /// For a symbol, its name in clear.
    static const char* const yytname_[];


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const short yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const short yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const short yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
    // symbol of state STATE-NUM.
    static const unsigned char yystos_[];

    // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
    static const unsigned char yyr1_[];

    // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
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

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

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

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200)
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
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
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
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
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range)
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
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
      yylast_ = 3320,     ///< Last index in yytable_.
      yynnts_ = 55,  ///< Number of nonterminal symbols.
      yyfinal_ = 13 ///< Termination state number.
    };


    // User arguments.
    yyscan_t yyscanner;
    ParserDriver& drv;

  };

  inline
  parser::symbol_kind_type
  parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
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
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116
    };
    // Last valid token kind.
    const int code_max = 371;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return YY_CAST (symbol_kind_type, translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_columndef: // columndef
        value.copy< ColumndefData > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_withoutrowid: // optional_withoutrowid
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.copy< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
      case symbol_kind::S_tableconstraint: // tableconstraint
        value.copy< sqlb::ConstraintPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.copy< sqlb::ConstraintSet > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_createindex_stmt: // createindex_stmt
        value.copy< sqlb::IndexPtr > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_indexed_column: // indexed_column
        value.copy< sqlb::IndexedColumn > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_indexed_column_list: // indexed_column_list
        value.copy< sqlb::IndexedColumnVector > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columnid_list: // columnid_list
      case symbol_kind::S_optional_columnid_with_paren_list: // optional_columnid_with_paren_list
        value.copy< sqlb::StringVector > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_createvirtualtable_stmt: // createvirtualtable_stmt
      case symbol_kind::S_createtable_stmt: // createtable_stmt
        value.copy< sqlb::TablePtr > (YY_MOVE (that.value));
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
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_columndef_list: // columndef_list
        value.copy< std::vector<ColumndefData> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }

  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_columndef: // columndef
        value.move< ColumndefData > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_optional_if_not_exists: // optional_if_not_exists
      case symbol_kind::S_optional_unique: // optional_unique
      case symbol_kind::S_optional_temporary: // optional_temporary
      case symbol_kind::S_optional_withoutrowid: // optional_withoutrowid
      case symbol_kind::S_optional_always_generated: // optional_always_generated
        value.move< bool > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_columnconstraint: // columnconstraint
      case symbol_kind::S_tableconstraint: // tableconstraint
        value.move< sqlb::ConstraintPtr > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_columnconstraint_list: // columnconstraint_list
      case symbol_kind::S_tableconstraint_list: // tableconstraint_list
      case symbol_kind::S_optional_tableconstraint_list: // optional_tableconstraint_list
        value.move< sqlb::ConstraintSet > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_createindex_stmt: // createindex_stmt
        value.move< sqlb::IndexPtr > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_indexed_column: // indexed_column
        value.move< sqlb::IndexedColumn > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_indexed_column_list: // indexed_column_list
        value.move< sqlb::IndexedColumnVector > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_columnid_list: // columnid_list
      case symbol_kind::S_optional_columnid_with_paren_list: // optional_columnid_with_paren_list
        value.move< sqlb::StringVector > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_createvirtualtable_stmt: // createvirtualtable_stmt
      case symbol_kind::S_createtable_stmt: // createtable_stmt
        value.move< sqlb::TablePtr > (YY_MOVE (s.value));
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
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_columndef_list: // columndef_list
        value.move< std::vector<ColumndefData> > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  inline
  parser::by_kind::by_kind ()
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  parser::by_kind::by_kind (by_kind&& that)
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  inline
  parser::by_kind::by_kind (const by_kind& that)
    : kind_ (that.kind_)
  {}

  inline
  parser::by_kind::by_kind (token_kind_type t)
    : kind_ (yytranslate_ (t))
  {}

  inline
  void
  parser::by_kind::clear ()
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  inline
  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  inline
  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }

  inline
  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }

#line 10 "sqlite3_parser.yy"
} } //  sqlb::parser 
#line 4270 "sqlite3_parser.hpp"




#endif // !YY_YY_SQLITE3_PARSER_HPP_INCLUDED
