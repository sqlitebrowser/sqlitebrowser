#ifndef INC_ASTFactory_hpp__
#define INC_ASTFactory_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/ASTFactory.hpp#2 $
 */

#include <antlr/config.hpp>
#include <antlr/AST.hpp>
#include <antlr/ASTArray.hpp>
#include <antlr/ASTPair.hpp>

#include <istream>
#include <utility>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

// Using these extra types to appease MSVC
typedef RefAST (*factory_type_)();
typedef ANTLR_USE_NAMESPACE(std)pair< const char*, factory_type_ >  factory_descriptor_;
typedef ANTLR_USE_NAMESPACE(std)vector< factory_descriptor_* >      factory_descriptor_list_;

/** AST Super Factory shared by TreeParser and Parser.
 * This super factory maintains a map of all AST node types to their respective
 * AST factories. One instance should be shared among a parser/treeparser
 * chain.
 *
 * @todo check all this code for possible use of references in
 * stead of RefAST's.
 */
class ANTLR_API ASTFactory {
public:
	typedef factory_type_             factory_type;
	typedef factory_descriptor_       factory_descriptor;
	typedef factory_descriptor_list_  factory_descriptor_list;
protected:
	/* The mapping of AST node type to factory..
	 */
	factory_descriptor default_factory_descriptor;
	factory_descriptor_list nodeFactories;
public:
	/// Make new factory. Per default (Ref)CommonAST instances are generated.
	ASTFactory();
	/** Initialize factory with a non default node type.
	 * factory_node_name should be the name of the AST node type the factory
	 * generates. (should exist during the existance of this ASTFactory
	 * instance)
	 */
	ASTFactory( const char* factory_node_name, factory_type factory );
	/// Destroy factory
	virtual ~ASTFactory();

	/// Register a node factory for the node type type with name ast_name
	void registerFactory( int type, const char* ast_name, factory_type factory );
	/// Set the maximum node (AST) type this factory may encounter
	void setMaxNodeType( int type );

	/// Add a child to the current AST
	void addASTChild(ASTPair& currentAST, RefAST child);
	/// Create new empty AST node. The right default type shou
	virtual RefAST create();
	/// Create AST node of the right type for 'type'
	RefAST create(int type);
	/// Create AST node of the right type for 'type' and initialize with txt
	RefAST create(int type, const ANTLR_USE_NAMESPACE(std)string& txt);
	/// Create duplicate of tr
	RefAST create(RefAST tr);
	/// Create new AST node and initialize contents from a token.
	RefAST create(RefToken tok);
	/// Create new AST node and initialize contents from a stream.
	RefAST create(const ANTLR_USE_NAMESPACE(std)string& txt, ANTLR_USE_NAMESPACE(std)istream& infile );
	/** Deep copy a single node. This function the new clone() methods in the
	 * AST interface. Returns a new RefAST(nullASTptr) if t is null.
	 */
	RefAST dup(RefAST t);
	/// Duplicate tree including siblings of root.
	RefAST dupList(RefAST t);
	/** Duplicate a tree, assuming this is a root node of a tree--
	 * duplicate that node and what's below; ignore siblings of root node.
	 */
	RefAST dupTree(RefAST t);
	/** Make a tree from a list of nodes. The first element in the
	 * array is the root. If the root is null, then the tree is
	 * a simple list not a tree. Handles null children nodes correctly.
	 * For example, make(a, b, null, c) yields tree (a b c).  make(null,a,b)
	 * yields tree (nil a b).
	 */
	RefAST make(ANTLR_USE_NAMESPACE(std)vector<RefAST>& nodes);
	/** Make a tree from a list of nodes, where the nodes are contained
	 * in an ASTArray object. The ASTArray is deleted after use.
	 * @todo FIXME! I have a feeling we can get rid of this ugly ASTArray thing
	 */
	RefAST make(ASTArray* nodes);
	/// Make an AST the root of current AST
	void makeASTRoot(ASTPair& currentAST, RefAST root);

	/** Set a new default AST type.
	 * factory_node_name should be the name of the AST node type the factory
	 * generates. (should exist during the existance of this ASTFactory
	 * instance).
	 * Only change factory between parser runs. You might get unexpected results
	 * otherwise.
	 */
	void setASTNodeFactory( const char* factory_node_name, factory_type factory );

#ifdef ANTLR_SUPPORT_XML
	/** Load a XML AST from stream. Make sure you have all the factories
	 * registered before use.
	 * @note this 'XML' stuff is quite rough still. YMMV.
	 */
	RefAST LoadAST( ANTLR_USE_NAMESPACE(std)istream& infile );
#endif
protected:
	void loadChildren( ANTLR_USE_NAMESPACE(std)istream& infile, RefAST current );
	void loadSiblings( ANTLR_USE_NAMESPACE(std)istream& infile, RefAST current );
	bool checkCloseTag( ANTLR_USE_NAMESPACE(std)istream& infile );

#ifdef ANTLR_VECTOR_HAS_AT
	/// construct a node of 'type'
	inline RefAST getNodeOfType( unsigned int type )
	{
		return RefAST(nodeFactories.at(type)->second());
	}
	/// get the name of the node 'type'
	const char* getASTNodeType( unsigned int type )
	{
		return nodeFactories.at(type)->first;
	}
	/// get the factory used for node 'type'
	factory_type getASTNodeFactory( unsigned int type )
	{
		return nodeFactories.at(type)->second;
	}
#else
	inline RefAST getNodeOfType( unsigned int type )
	{
		return RefAST(nodeFactories[type]->second());
	}
	/// get the name of the node 'type'
	const char* getASTNodeType( unsigned int type )
	{
		return nodeFactories[type]->first;
	}
	factory_type getASTNodeFactory( unsigned int type )
	{
		return nodeFactories[type]->second;
	}
#endif

private:
	// no copying and such..
	ASTFactory( const ASTFactory& );
	ASTFactory& operator=( const ASTFactory& );
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_ASTFactory_hpp__
