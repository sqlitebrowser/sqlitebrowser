#ifndef INC_config_hpp__
#define INC_config_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/config.hpp#2 $
 */

/*
 * Just a simple configuration file to differentiate between the
 * various compilers used and reconfigure stuff for any oddities of the
 * compiler in question.
 *
 * These are the defaults. Per compiler these are amended.
 */
#define ANTLR_USE_NAMESPACE(_x_) _x_::
#define ANTLR_USING_NAMESPACE(_x_) using namespace _x_;
#define ANTLR_CXX_SUPPORTS_NAMESPACE 1
#define ANTLR_C_USING(_x_)
#define ANTLR_API
#ifndef CUSTOM_API
# define CUSTOM_API
#endif
#define ANTLR_IOS_BASE ios_base
/** define if cctype functions/macros need a std:: prefix. A lot of compilers
 * define these as macros, in which case something barfs.
 */
#define ANTLR_CCTYPE_NEEDS_STD

/// Define if C++ compiler supports std::uncaught_exception
#define ANTLR_CXX_SUPPORTS_UNCAUGHT_EXCEPTION

#define ANTLR_ATOI_IN_STD

/******************************************************************************/
/*{{{ Microsoft Visual C++ */
// NOTE: If you provide patches for a specific MSVC version guard them for
// the specific version!!!!
// _MSC_VER == 1100 for Microsoft Visual C++ 5.0
// _MSC_VER == 1200 for Microsoft Visual C++ 6.0
// _MSC_VER == 1300 for Microsoft Visual C++ 7.0
#if defined(_MSC_VER)

# if _MSC_VER < 1300
#	define NOMINMAX
#	pragma warning(disable : 4786)
#	define min _cpp_min
# endif

// This warning really gets on my nerves.
// It's the one about symbol longer than 256 chars, and it happens
// all the time with STL.
# pragma warning( disable : 4786 4231 )
// this shuts up some DLL interface warnings for STL
# pragma warning( disable : 4251 )

# ifdef ANTLR_CXX_USE_STLPORT
#	undef ANTLR_CXX_SUPPORTS_UNCAUGHT_EXCEPTION
# endif

# if ( _MSC_VER < 1300 ) && ( defined(ANTLR_EXPORTS) || defined(ANTLR_IMPORTS) )
#	error "DLL Build not supported on these MSVC versions."
// see comment in lib/cpp/src/dll.cpp
# endif

// For the DLL support originally contributed by Stephen Naughton
// If you are building statically leave ANTLR_EXPORTS/ANTLR_IMPORTS undefined
// If you are building the DLL define ANTLR_EXPORTS
// If you are compiling code to be used with the DLL define ANTLR_IMPORTS
# ifdef ANTLR_EXPORTS
#	undef ANTLR_API
#	define ANTLR_API __declspec(dllexport)
# endif

# ifdef ANTLR_IMPORTS
#	undef ANTLR_API
#	define ANTLR_API __declspec(dllimport)
# endif

# if ( _MSC_VER < 1200 )
// supposedly only for MSVC5 and before...
// Using vector<XXX> requires operator<(X,X) to be defined
#	define NEEDS_OPERATOR_LESS_THAN
# endif

// VC6
# if ( _MSC_VER == 1200 )
#	undef ANTLR_ATOI_IN_STD
# endif

# if ( _MSC_VER < 1310 )
// Supposedly only for MSVC7 and before...
// Not allowed to put 'static const int XXX=20;' in a class definition
#	define NO_STATIC_CONSTS
#	define NO_TEMPLATE_PARTS
# endif

// No strcasecmp in the C library (so use stricmp instead)
// - Anyone know which is in which standard?
# define NO_STRCASECMP
# undef ANTLR_CCTYPE_NEEDS_STD
#	define NO_STATIC_CONSTS
#endif	// End of Microsoft Visual C++

/*}}}*/
/******************************************************************************/
/*{{{ SunPro Compiler (Using OBJECTSPACE STL)
 *****************************************************************************/
#ifdef __SUNPRO_CC

# if (__SUNPRO_CC >= 0x500)

#	define NEEDS_OPERATOR_LESS_THAN
#	define NO_TEMPLATE_PARTS

# else

#	undef namespace
#	define namespace

#	if (__SUNPRO_CC == 0x420)

/* This code is specif to SunWspro Compiler 4.2, and will compile with
 the objectspace 2.1 toolkit for Solaris2.6 */
#	 define HAS_NOT_CASSERT_H
#	 define HAS_NOT_CSTRING_H
#	 define HAS_NOT_CCTYPE_H
#	 define HAS_NOT_CSTDIO_H
#	 define HAS_OSTREAM_H

/* #define OS_SOLARIS_2_6
 #define OS_NO_WSTRING
 #define OS_NO_ALLOCATORS
 #define OS_MULTI_THREADED
 #define OS_SOLARIS_NATIVE
 #define OS_REALTIME
 #define __OSVERSION__=5
 #define SVR4
 */

// ObjectSpace + some specific templates constructions with stl.
/* #define OS_NO_ALLOCATOR */

// This great compiler does not have the namespace feature.
#	 undef  ANTLR_USE_NAMESPACE
#	 define ANTLR_USE_NAMESPACE(_x_)
#	 undef ANTLR_USING_NAMESPACE
#	 define ANTLR_USING_NAMESPACE(_x_)
#	 undef ANTLR_CXX_SUPPORTS_NAMESPACE
#	endif		// End __SUNPRO_CC == 0x420

#	undef explicit
#	define explicit

#	define exception os_exception
#	define bad_exception os_bad_exception

// Not allowed to put 'static const int XXX=20;' in a class definition
#	define NO_STATIC_CONSTS
// Using vector<XXX> requires operator<(X,X) to be defined
#	define NEEDS_OPERATOR_LESS_THAN

# endif

# undef ANTLR_CCTYPE_NEEDS_STD

#endif	// end __SUNPRO_CC
/*}}}*/
/*****************************************************************************/
/*{{{ Inprise C++ Builder 3.0
 *****************************************************************************/
#ifdef __BCPLUSPLUS__
# define NO_TEMPLATE_PARTS
# define NO_STRCASECMP
# undef ANTLR_CCTYPE_NEEDS_STD
#endif	// End of C++ Builder 3.0
/*}}}*/
/*****************************************************************************/
/*{{{ IBM VisualAge C++ ( which includes the Dinkumware C++ Library )
 *****************************************************************************/
#ifdef __IBMCPP__

// No strcasecmp in the C library (so use stricmp instead)
// - Anyone know which is in which standard?
#if (defined(_AIX) && (__IBMCPP__ >= 600))
# define NO_STATIC_CONSTS
#else
# define NO_STRCASECMP
# undef ANTLR_CCTYPE_NEEDS_STD
#endif

#endif	// end IBM VisualAge C++
/*}}}*/
/*****************************************************************************/
/*{{{ Metrowerks Codewarrior
 *****************************************************************************/
#ifdef __MWERKS__
# if (__MWERKS__ <= 0x2201)
#	define NO_TEMPLATE_PARTS
# endif

// CW 6.0 and 7.0 still do not have it.
# define ANTLR_REALLY_NO_STRCASECMP

# undef ANTLR_C_USING
# define ANTLR_C_USING(_x_)   using std:: ## _x_;

# define ANTLR_CCTYPE_NEEDS_STD
# undef ANTLR_CXX_SUPPORTS_UNCAUGHT_EXCEPTION

#endif	// End of Metrowerks Codewarrior
/*}}}*/
/*****************************************************************************/
/*{{{ SGI Irix 6.5.10 MIPSPro compiler
 *****************************************************************************/
// (contributed by Anna Winkler)
// Note: you can't compile ANTLR with the MIPSPro compiler on
// anything < 6.5.10 because SGI just fixed a big bug dealing with
// namespaces in that release.
#ifdef __sgi
# define HAS_NOT_CCTYPE_H
# define HAS_NOT_CSTRING_H
# define HAS_NOT_CSTDIO_H
# undef ANTLR_CCTYPE_NEEDS_STD
#endif // End IRIX MIPSPro
/*}}}*/
/*****************************************************************************/
/*{{{ G++ in various incarnations
 *****************************************************************************/
// With the gcc-2.95 and 3.0 being in the near future we should start handling
// incompatabilities between the various libstdc++'s.
#if defined(__GNUC__) || defined(__GNUG__)
// gcc 2 branch..
# if (__GNUC__ == 2 )
#	if (__GNUC_MINOR__ <= 8 )
#	 undef ANTLR_USE_NAMESPACE
#	 define ANTLR_USE_NAMESPACE(_x_)
#	 undef ANTLR_USING_NAMESPACE
#	 define ANTLR_USING_NAMESPACE(_x_)
#	 undef ANTLR_CXX_SUPPORTS_NAMESPACE
#	endif
#	if (__GNUC_MINOR__ > 8 && __GNUC_MINOR__ <= 95 )
#	  undef ANTLR_IOS_BASE
#	  define ANTLR_IOS_BASE ios
#	  undef ANTLR_CCTYPE_NEEDS_STD
// compiling with -ansi ?
#	  ifdef __STRICT_ANSI__
#		undef ANTLR_REALLY_NO_STRCASECMP
#		define ANTLR_REALLY_NO_STRCASECMP
#	  endif
#	else
// experimental .96 .97 branches..
#	 undef ANTLR_CCTYPE_NEEDS_STD
#	endif
# endif
#endif // ! __GNUC__
/*}}}*/
/*****************************************************************************/
/*{{{ Digital CXX (Tru64)
 *****************************************************************************/
#ifdef __DECCXX
#define __USE_STD_IOSTREAM
#endif
/*}}}*/
/*****************************************************************************/
#ifdef __BORLANDC__
# if  __BORLANDC__ >= 560
#	include <ctype>
#	include <stdlib>
#	define ANTLR_CCTYPE_NEEDS_STD
# else
#	error "sorry, compiler is too old - consider an update."
# endif
#endif

// Redefine these for backwards compatability..
#undef ANTLR_BEGIN_NAMESPACE
#undef ANTLR_END_NAMESPACE

#if ANTLR_CXX_SUPPORTS_NAMESPACE == 1
# define ANTLR_BEGIN_NAMESPACE(_x_) namespace _x_ {
# define ANTLR_END_NAMESPACE }
#else
# define ANTLR_BEGIN_NAMESPACE(_x_)
# define ANTLR_END_NAMESPACE
#endif

#endif //INC_config_hpp__
