#ifndef INC_ASTPair_hpp__
#define INC_ASTPair_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/ASTPair.hpp#2 $
 */

#include <antlr/config.hpp>
#include <antlr/AST.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** ASTPair:  utility class used for manipulating a pair of ASTs
  * representing the current AST root and current AST sibling.
  * This exists to compensate for the lack of pointers or 'var'
  * arguments in Java.
  *
  * OK, so we can do those things in C++, but it seems easier
  * to stick with the Java way for now.
  */
class ANTLR_API ASTPair {
public:
	RefAST root;		// current root of tree
	RefAST child;		// current child to which siblings are added

	/** Make sure that child is the last sibling */
	void advanceChildToEnd() {
		if (child) {
			while (child->getNextSibling()) {
				child = child->getNextSibling();
			}
		}
	}
//	/** Copy an ASTPair.  Don't call it clone() because we want type-safety */
//	ASTPair copy() {
//		ASTPair tmp = new ASTPair();
//		tmp.root = root;
//		tmp.child = child;
//		return tmp;
//	}
	ANTLR_USE_NAMESPACE(std)string toString() const {
		ANTLR_USE_NAMESPACE(std)string r = !root ? ANTLR_USE_NAMESPACE(std)string("null") : root->getText();
		ANTLR_USE_NAMESPACE(std)string c = !child ? ANTLR_USE_NAMESPACE(std)string("null") : child->getText();
		return "["+r+","+c+"]";
	}
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_ASTPair_hpp__
