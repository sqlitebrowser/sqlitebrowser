/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/src/ASTFactory.cpp#2 $
 */

#include "antlr/CommonAST.hpp"
#include "antlr/ANTLRException.hpp"
#include "antlr/IOException.hpp"
#include "antlr/ASTFactory.hpp"
#include "antlr/ANTLRUtil.hpp"

#include <iostream>
#include <istream>

using namespace std;

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** AST Support code shared by TreeParser and Parser.
 * We use delegation to share code (and have only one
 * bit of code to maintain) rather than subclassing
 * or superclassing (forces AST support code to be
 * loaded even when you don't want to do AST stuff).
 *
 * This class collects all factories of AST types used inside the code.
 * New AST node types are registered with the registerFactory method.
 * On creation of an ASTFactory object a default AST node factory may be
 * specified.
 *
 * When registering types gaps between different types are filled with entries
 * for the default factory.
 */

/// Initialize factory
ASTFactory::ASTFactory()
: default_factory_descriptor(ANTLR_USE_NAMESPACE(std)make_pair(CommonAST::TYPE_NAME,&CommonAST::factory))
{
	nodeFactories.resize( Token::MIN_USER_TYPE, &default_factory_descriptor );
}

/** Initialize factory with a non default node type.
 * factory_node_name should be the name of the AST node type the factory
 * generates. (should exist during the existance of this ASTFactory instance)
 */
ASTFactory::ASTFactory( const char* factory_node_name, factory_type fact )
: default_factory_descriptor(ANTLR_USE_NAMESPACE(std)make_pair(factory_node_name, fact))
{
	nodeFactories.resize( Token::MIN_USER_TYPE, &default_factory_descriptor );
}

/// Delete ASTFactory
ASTFactory::~ASTFactory()
{
	factory_descriptor_list::iterator i = nodeFactories.begin();

	while( i != nodeFactories.end() )
	{
		if( *i != &default_factory_descriptor )
			delete *i;
		i++;
	}
}

/// Register a factory for a given AST type
void ASTFactory::registerFactory( int type, const char* ast_name, factory_type factory )
{
	// check validity of arguments...
	if( type < Token::MIN_USER_TYPE )
		throw ANTLRException("Internal parser error invalid type passed to RegisterFactory");
	if( factory == 0 )
		throw ANTLRException("Internal parser error 0 factory passed to RegisterFactory");

	// resize up to and including 'type' and initalize any gaps to default
	// factory.
	if( nodeFactories.size() < (static_cast<unsigned int>(type)+1) )
		nodeFactories.resize( type+1, &default_factory_descriptor );

	// And add new thing..
	nodeFactories[type] = new ANTLR_USE_NAMESPACE(std)pair<const char*, factory_type>( ast_name, factory );
}

void ASTFactory::setMaxNodeType( int type )
{
	if( nodeFactories.size() < (static_cast<unsigned int>(type)+1) )
		nodeFactories.resize( type+1, &default_factory_descriptor );
}

/** Create a new empty AST node; if the user did not specify
 *  an AST node type, then create a default one: CommonAST.
 */
RefAST ASTFactory::create()
{
	RefAST node = nodeFactories[0]->second();
	node->setType(Token::INVALID_TYPE);
	return node;
}

RefAST ASTFactory::create(int type)
{
	RefAST t = nodeFactories[type]->second();
	t->initialize(type,"");
	return t;
}

RefAST ASTFactory::create(int type, const ANTLR_USE_NAMESPACE(std)string& txt)
{
	RefAST t = nodeFactories[type]->second();
	t->initialize(type,txt);
	return t;
}

#ifdef ANTLR_SUPPORT_XML
RefAST ASTFactory::create(const ANTLR_USE_NAMESPACE(std)string& type_name, ANTLR_USE_NAMESPACE(std)istream& infile )
{
	factory_descriptor_list::iterator fact = nodeFactories.begin();

	while( fact != nodeFactories.end() )
	{
		if( type_name == (*fact)->first )
		{
			RefAST t = (*fact)->second();
			t->initialize(infile);
			return t;
		}
		fact++;
	}

	string error = "ASTFactory::create: Unknown AST type '" + type_name + "'";
	throw ANTLRException(error);
}
#endif

/** Create a new empty AST node; if the user did not specify
 *  an AST node type, then create a default one: CommonAST.
 */
RefAST ASTFactory::create(RefAST tr)
{
	if (!tr)
		return nullAST;

//	cout << "create(tr)" << endl;

	RefAST t = nodeFactories[tr->getType()]->second();
	t->initialize(tr);
	return t;
}

RefAST ASTFactory::create(RefToken tok)
{
//	cout << "create( tok="<< tok->getType() << ", " << tok->getText() << ")" << nodeFactories.size() << endl;
	RefAST t = nodeFactories[tok->getType()]->second();
	t->initialize(tok);
	return t;
}

/** Add a child to the current AST */
void ASTFactory::addASTChild(ASTPair& currentAST, RefAST child)
{
	if (child)
	{
		if (!currentAST.root)
		{
			// Make new child the current root
			currentAST.root = child;
		}
		else
		{
			if (!currentAST.child)
			{
				// Add new child to current root
				currentAST.root->setFirstChild(child);
			}
			else
			{
				currentAST.child->setNextSibling(child);
			}
		}
		// Make new child the current child
		currentAST.child = child;
		currentAST.advanceChildToEnd();
	}
}

/** Deep copy a single node. This function the new clone() methods in the AST
 * interface. Returns nullAST if t is null.
 */
RefAST ASTFactory::dup(RefAST t)
{
	if( t )
		return t->clone();
	else
		return RefAST(nullASTptr);
}

/** Duplicate tree including siblings of root. */
RefAST ASTFactory::dupList(RefAST t)
{
	RefAST result = dupTree(t);         // if t == null, then result==null
	RefAST nt = result;

	while( t )
	{												// for each sibling of the root
		t = t->getNextSibling();
		nt->setNextSibling(dupTree(t));	// dup each subtree, building new tree
		nt = nt->getNextSibling();
	}
	return result;
}

/** Duplicate a tree, assuming this is a root node of a tree
 * duplicate that node and what's below; ignore siblings of root node.
 */
RefAST ASTFactory::dupTree(RefAST t)
{
	RefAST result = dup(t);		// make copy of root
	// copy all children of root.
	if( t )
		result->setFirstChild( dupList(t->getFirstChild()) );
	return result;
}

/** Make a tree from a list of nodes.  The first element in the
 * array is the root.  If the root is null, then the tree is
 * a simple list not a tree.  Handles null children nodes correctly.
 * For example, make(a, b, null, c) yields tree (a b c).  make(null,a,b)
 * yields tree (nil a b).
 */
RefAST ASTFactory::make(ANTLR_USE_NAMESPACE(std)vector<RefAST>& nodes)
{
	if ( nodes.size() == 0 )
		return RefAST(nullASTptr);

	RefAST root = nodes[0];
	RefAST tail = RefAST(nullASTptr);

	if( root )
		root->setFirstChild(RefAST(nullASTptr));	// don't leave any old pointers set

	// link in children;
	for( unsigned int i = 1; i < nodes.size(); i++ )
	{
		if ( nodes[i] == 0 )		// ignore null nodes
			continue;

		if ( root == 0 )			// Set the root and set it up for a flat list
			root = tail = nodes[i];
		else if ( tail == 0 )
		{
			root->setFirstChild(nodes[i]);
			tail = root->getFirstChild();
		}
		else
		{
			tail->setNextSibling(nodes[i]);
			tail = tail->getNextSibling();
		}

		if( tail )	// RK: I cannot fathom why this missing check didn't bite anyone else...
		{
			// Chase tail to last sibling
			while (tail->getNextSibling())
				tail = tail->getNextSibling();
		}
	}

	return root;
}

/** Make a tree from a list of nodes, where the nodes are contained
 * in an ASTArray object
 */
RefAST ASTFactory::make(ASTArray* nodes)
{
	RefAST ret = make(nodes->array);
	delete nodes;
	return ret;
}

/// Make an AST the root of current AST
void ASTFactory::makeASTRoot( ASTPair& currentAST, RefAST root )
{
	if (root)
	{
		// Add the current root as a child of new root
		root->addChild(currentAST.root);
		// The new current child is the last sibling of the old root
		currentAST.child = currentAST.root;
		currentAST.advanceChildToEnd();
		// Set the new root
		currentAST.root = root;
	}
}

void ASTFactory::setASTNodeFactory( const char* factory_node_name,
											   factory_type factory )
{
	default_factory_descriptor.first = factory_node_name;
	default_factory_descriptor.second = factory;
}

#ifdef ANTLR_SUPPORT_XML
bool ASTFactory::checkCloseTag( ANTLR_USE_NAMESPACE(std)istream& in )
{
	char ch;

	if( in.get(ch) )
	{
		if( ch == '<' )
		{
			char ch2;
			if( in.get(ch2) )
			{
				if( ch2 == '/' )
				{
					in.putback(ch2);
					in.putback(ch);
					return true;
				}
				in.putback(ch2);
				in.putback(ch);
				return false;
			}
		}
		in.putback(ch);
		return false;
	}
	return false;
}

void ASTFactory::loadChildren( ANTLR_USE_NAMESPACE(std)istream& infile,
										 RefAST current )
{
	char ch;

	for(;;)			// for all children of this node....
	{
		eatwhite(infile);

		infile.get(ch);	// '<'
		if( ch != '<' )
		{
			string error = "Invalid XML file... no '<' found (";
			error += ch + ")";
			throw IOException(error);
		}

		infile.get(ch);		// / or text....

		if( ch == '/' )		// check for close tag...
		{
			string temp;

			// read until '>' and see if it matches the open tag... if not trouble
			temp = read_identifier( infile );

			if( strcmp(temp.c_str(), current->typeName() ) != 0 )
			{
				string error = "Invalid XML file... close tag does not match start tag: ";
				error += current->typeName();
				error += " closed by " + temp;
				throw IOException(error);
			}

			infile.get(ch);	// must be a '>'

			if( ch != '>' )
			{
				string error = "Invalid XML file... no '>' found (";
				error += ch + ")";
				throw IOException(error);
			}
			// close tag => exit loop
			break;
		}

		// put our 'look ahead' back where it came from
		infile.putback(ch);
		infile.putback('<');

		// and recurse into the tree...
		RefAST child = LoadAST(infile);

		current->addChild( child );
	}
}

void ASTFactory::loadSiblings(ANTLR_USE_NAMESPACE(std)istream& infile,
										RefAST current )
{
	for(;;)
	{
		eatwhite(infile);

		if( infile.eof() )
			break;

		if( checkCloseTag(infile) )
			break;

		RefAST sibling = LoadAST(infile);
		current->setNextSibling(sibling);
	}
}

RefAST ASTFactory::LoadAST( ANTLR_USE_NAMESPACE(std)istream& infile )
{
	RefAST current = nullAST;
	char ch;

	eatwhite(infile);

	if( !infile.get(ch) )
		return nullAST;

	if( ch != '<' )
	{
		string error = "Invalid XML file... no '<' found (";
		error += ch + ")";
		throw IOException(error);
	}

	string ast_type = read_identifier(infile);

	// create the ast of type 'ast_type'
	current = create( ast_type, infile );
	if( current == nullAST )
	{
		string error = "Unsuported AST type: " + ast_type;
		throw IOException(error);
	}

	eatwhite(infile);

	infile.get(ch);

	// now if we have a '/' here it's a single node. If it's a '>' we get
	// a tree with children

	if( ch == '/' )
	{
		infile.get(ch);		// get the closing '>'
		if( ch != '>' )
		{
			string error = "Invalid XML file... no '>' found after '/' (";
			error += ch + ")";
			throw IOException(error);
		}

		// get the rest on this level
		loadSiblings( infile, current );

		return current;
	}

	// and finaly see if we got the close tag...
	if( ch != '>' )
	{
		string error = "Invalid XML file... no '>' found (";
		error += ch + ")";
		throw IOException(error);
	}

	// handle the ones below this level..
	loadChildren( infile, current );

	// load the rest on this level...
	loadSiblings( infile, current );

	return current;
}
#endif // ANTLR_SUPPORT_XML

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

/* Heterogeneous AST/XML-I/O ramblings...
 *
 * So there is some heterogeneous AST support....
 * basically in the code generators a new custom ast is generated without
 * going throug the factory. It also expects the RefXAST to be defined.
 *
 * Is it maybe better to register all AST types with the ASTFactory class
 * together with the respective factory methods.
 *
 * More and more I get the impression that hetero ast was a kindoff hack
 * on top of ANTLR's normal AST system.
 *
 * The heteroast stuff will generate trouble for all astFactory.create( ... )
 * invocations. Most of this is handled via getASTCreateString methods in the
 * codegenerator. At the moment getASTCreateString(GrammarAtom, String) has
 * slightly to little info to do it's job (ok the hack that is in now
 * works, but it's an ugly hack)
 *
 * An extra caveat is the 'nice' action.g thing. Which also judiciously calls
 * getASTCreateString methods because it handles the #( ... ) syntax.
 * And converts that to ASTFactory calls.
 *
 *
 */
