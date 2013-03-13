/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id:$
 */

/*
 * DLL stub for MSVC++. Based upon versions of Stephen Naughton and Michael
 * T. Richter
 */

// RK: Uncommented by instruction of Alexander Lenski
//#if _MSC_VER > 1000
//# pragma once
//#endif // _MSC_VER > 1000

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
# error "DLL Build not supported on old MSVC's"
// Ok it seems to be possible with STLPort in stead of the vanilla MSVC STL
// implementation. This needs some work though. (and don't try it if you're
// not that familiar with compilers/building C++ DLL's in windows)
#endif

#include <vector>
#include "antlr/config.hpp"
#include "antlr/Token.hpp"
#include "antlr/CircularQueue.hpp"

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

// Take care of necessary implicit instantiations of templates from STL

// This should take care of MSVC 7.0
#if defined( _MSC_VER ) && ( _MSC_VER == 1300 )

// these come from AST.hpp
template class ANTLR_API ASTRefCount< AST >;
template class ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< RefAST >;
template class ANTLR_API ANTLR_USE_NAMESPACE(std)vector< RefAST >;
//template ANTLR_API int operator<( ASTRefCount< AST >, ASTRefCount< AST > );

// ASTFactory.hpp
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< factory_descriptor_* >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const char*, factory_type_ > >;
template struct ANTLR_API ANTLR_USE_NAMESPACE(std)pair< const char*, factory_type_ >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)_Vector_val< factory_descriptor_*, ANTLR_USE_NAMESPACE(std)allocator< factory_descriptor_* > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)vector< factory_descriptor_* >;

// BitSet.hpp
template class ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< bool >;
template class ANTLR_API ANTLR_USE_NAMESPACE(std)_Vector_val< bool, ANTLR_USE_NAMESPACE(std)allocator< bool > >;
template class ANTLR_API ANTLR_USE_NAMESPACE(std)vector< bool >;

// CharScanner.hpp
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< ANTLR_USE_NAMESPACE(std)string, int > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, int > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)_Tree_nod< ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, int, CharScannerLiteralsLess, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, int > >, false > >::_Node >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)_Tree_ptr< ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, int, CharScannerLiteralsLess, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, int > >, false > >::_Nodeptr >;
template struct ANTLR_API ANTLR_USE_NAMESPACE(std)pair< ANTLR_USE_NAMESPACE(std)string, int >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, int, CharScannerLiteralsLess, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, int > >,false >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)_Tree_nod< ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, int, CharScannerLiteralsLess, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, int > >,false > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)_Tree_ptr< ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, int, CharScannerLiteralsLess, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, int > >,false > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)_Tree_val< ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, int, CharScannerLiteralsLess, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, int > >,false > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)_Tree< ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, int, CharScannerLiteralsLess, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, int > >,false > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)map< ANTLR_USE_NAMESPACE(std)string, int, CharScannerLiteralsLess >;

// CircularQueue.hpp
// RK: it might well be that a load of these ints need to be unsigned ints
//  (made some more stuff unsigned)
template class ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< int >;
template class ANTLR_API ANTLR_USE_NAMESPACE(std)_Vector_val< int, ANTLR_USE_NAMESPACE(std)allocator< int > >;
template class ANTLR_API ANTLR_USE_NAMESPACE(std)vector< int >;
template class ANTLR_API ANTLR_USE_NAMESPACE(std)vector< int, ANTLR_USE_NAMESPACE(std)allocator< int > >;
//  template ANTLR_API inline int CircularQueue< int >::entries() const;

template class ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< RefToken >;
template class ANTLR_API ANTLR_USE_NAMESPACE(std)_Vector_val< RefToken, ANTLR_USE_NAMESPACE(std)allocator< RefToken > >;
template class ANTLR_API ANTLR_USE_NAMESPACE(std)vector< RefToken >;
template class ANTLR_API ANTLR_USE_NAMESPACE(std)vector< RefToken, ANTLR_USE_NAMESPACE(std)allocator< RefToken > >;
//  template ANTLR_API inline int CircularQueue< RefToken >::entries() const;

// CommonAST.hpp
template class ANTLR_API ASTRefCount< CommonAST >;

// CommonASTWithHiddenTokenTypes.hpp
template class ANTLR_API ASTRefCount< CommonASTWithHiddenTokens >;

// LexerSharedInputState.hpp
template class ANTLR_API RefCount< LexerInputState >;

// ParserSharedInputState.hpp
template class ANTLR_API RefCount< ParserInputState >;

// TokenStreamSelector.hpp
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< ANTLR_USE_NAMESPACE(std)string, TokenStream* > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, TokenStream* > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)_Tree_nod< ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, TokenStream*, ANTLR_USE_NAMESPACE(std)less< ANTLR_USE_NAMESPACE(std)string >, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, TokenStream* > >, false > >::_Node >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)_Tree_ptr< ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, TokenStream*, ANTLR_USE_NAMESPACE(std)less< ANTLR_USE_NAMESPACE(std)string >, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, TokenStream* > >, false > >::_Nodeptr >;
template struct ANTLR_API ANTLR_USE_NAMESPACE(std)pair< ANTLR_USE_NAMESPACE(std)string, TokenStream* >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, TokenStream*, ANTLR_USE_NAMESPACE(std)less< ANTLR_USE_NAMESPACE(std)string >, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, TokenStream* > >,false >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)_Tree_nod< ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, TokenStream*, ANTLR_USE_NAMESPACE(std)less< ANTLR_USE_NAMESPACE(std)string >, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, TokenStream* > >,false > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)_Tree_ptr< ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, TokenStream*, ANTLR_USE_NAMESPACE(std)less< ANTLR_USE_NAMESPACE(std)string >, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, TokenStream* > >,false > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)_Tree_val< ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, TokenStream*, ANTLR_USE_NAMESPACE(std)less< ANTLR_USE_NAMESPACE(std)string >, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, TokenStream* > >,false > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)_Tree< ANTLR_USE_NAMESPACE(std)_Tmap_traits< ANTLR_USE_NAMESPACE(std)string, TokenStream*, ANTLR_USE_NAMESPACE(std)less< ANTLR_USE_NAMESPACE(std)string >, ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)pair< const ANTLR_USE_NAMESPACE(std)string, TokenStream* > >,false > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)map< ANTLR_USE_NAMESPACE(std)string, TokenStream* >;

template class  ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< TokenStream* >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)allocator< ANTLR_USE_NAMESPACE(std)_Deque_map< TokenStream* , ANTLR_USE_NAMESPACE(std)allocator< TokenStream* > >::_Tptr >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)_Deque_map< TokenStream*, ANTLR_USE_NAMESPACE(std)allocator< TokenStream* > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)_Deque_val< TokenStream*, ANTLR_USE_NAMESPACE(std)allocator< TokenStream* > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)deque< TokenStream*, ANTLR_USE_NAMESPACE(std)allocator< TokenStream* > >;
template class  ANTLR_API ANTLR_USE_NAMESPACE(std)stack< TokenStream*, ANTLR_USE_NAMESPACE(std)deque<TokenStream*> >;

#elif  defined( _MSC_VER ) && ( _MSC_VER == 1310 )
// Instantiations for MSVC 7.1
template class ANTLR_API CircularQueue< int >;
template class ANTLR_API CircularQueue< RefToken >;

// #else future msvc's

#endif

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	return TRUE;
}
