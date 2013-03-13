#ifndef INC_TokenStreamRewriteEngine_hpp__
#define INC_TokenStreamRewriteEngine_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 */

#include <string>
#include <list>
#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include <iterator>
#include <cassert>
#include <algorithm>

#include <antlr/config.hpp>

#include <antlr/TokenStream.hpp>
#include <antlr/TokenWithIndex.hpp>
#include <antlr/BitSet.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** This token stream tracks the *entire* token stream coming from
 *	 a lexer, but does not pass on the whitespace (or whatever else
 *	 you want to discard) to the parser.
 *
 *	 This class can then be asked for the ith token in the input stream.
 *	 Useful for dumping out the input stream exactly after doing some
 *	 augmentation or other manipulations.	Tokens are index from 0..n-1
 *
 *	 You can insert stuff, replace, and delete chunks.	 Note that the
 *	 operations are done lazily--only if you convert the buffer to a
 *	 String.	 This is very efficient because you are not moving data around
 *	 all the time.	 As the buffer of tokens is converted to strings, the
 *	 toString() method(s) check to see if there is an operation at the
 *	 current index.  If so, the operation is done and then normal String
 *	 rendering continues on the buffer.	 This is like having multiple Turing
 *	 machine instruction streams (programs) operating on a single input tape. :)
 *
 *	 Since the operations are done lazily at toString-time, operations do not
 *	 screw up the token index values.  That is, an insert operation at token
 *	 index i does not change the index values for tokens i+1..n-1.
 *
 *	 Because operations never actually alter the buffer, you may always get
 *	 the original token stream back without undoing anything.  Since
 *	 the instructions are queued up, you can easily simulate transactions and
 *	 roll back any changes if there is an error just by removing instructions.
 *	 For example,
 *
 *			TokenStreamRewriteEngine rewriteEngine =
 *				new TokenStreamRewriteEngine(lexer);
 *		  JavaRecognizer parser = new JavaRecognizer(rewriteEngine);
 *		  ...
 *		  rewriteEngine.insertAfter("pass1", t, "foobar");}
 *			rewriteEngine.insertAfter("pass2", u, "start");}
 *			System.out.println(rewriteEngine.toString("pass1"));
 *			System.out.println(rewriteEngine.toString("pass2"));
 *
 *	 You can also have multiple "instruction streams" and get multiple
 *	 rewrites from a single pass over the input.	 Just name the instruction
 *	 streams and use that name again when printing the buffer.	This could be
 *	 useful for generating a C file and also its header file--all from the
 *	 same buffer.
 *
 *	 If you don't use named rewrite streams, a "default" stream is used.
 *
 *	 Terence Parr, parrt@cs.usfca.edu
 *	 University of San Francisco
 *	 February 2004
 */
class TokenStreamRewriteEngine : public TokenStream
{
public:
	typedef ANTLR_USE_NAMESPACE(std)vector<antlr::RefTokenWithIndex> token_list;
	static const char* DEFAULT_PROGRAM_NAME;
#ifndef NO_STATIC_CONSTS
	static const size_t MIN_TOKEN_INDEX;
	static const int PROGRAM_INIT_SIZE;
#else
	enum {
		MIN_TOKEN_INDEX = 0,
		PROGRAM_INIT_SIZE = 100
	};
#endif

	struct tokenToStream {
		tokenToStream( ANTLR_USE_NAMESPACE(std)ostream& o ) : out(o) {}
		template <typename T> void operator() ( const T& t ) {
			out << t->getText();
		}
		ANTLR_USE_NAMESPACE(std)ostream& out;
	};

	class RewriteOperation {
	protected:
		RewriteOperation( size_t idx, const ANTLR_USE_NAMESPACE(std)string& txt )
		: index(idx), text(txt)
		{
		}
	public:
		virtual ~RewriteOperation()
		{
		}
		/** Execute the rewrite operation by possibly adding to the buffer.
		 *	 Return the index of the next token to operate on.
		 */
		virtual size_t execute( ANTLR_USE_NAMESPACE(std)ostream& /* out */ ) {
			return index;
		}
		virtual size_t getIndex() const {
			return index;
		}
		virtual const char* type() const {
			return "RewriteOperation";
		}
	protected:
		size_t index;
		ANTLR_USE_NAMESPACE(std)string text;
	};

	struct executeOperation {
		ANTLR_USE_NAMESPACE(std)ostream& out;
		executeOperation( ANTLR_USE_NAMESPACE(std)ostream& s ) : out(s) {}
		void operator () ( RewriteOperation* t ) {
			t->execute(out);
		}
	};

	/// list of rewrite operations
	typedef ANTLR_USE_NAMESPACE(std)list<RewriteOperation*> operation_list;
	/// map program name to <program counter,program> tuple
	typedef ANTLR_USE_NAMESPACE(std)map<ANTLR_USE_NAMESPACE(std)string,operation_list> program_map;

	class InsertBeforeOp : public RewriteOperation
	{
	public:
		InsertBeforeOp( size_t index, const ANTLR_USE_NAMESPACE(std)string& text )
		: RewriteOperation(index, text)
		{
		}
		virtual ~InsertBeforeOp() {}
		virtual size_t execute( ANTLR_USE_NAMESPACE(std)ostream& out )
		{
			out << text;
			return index;
		}
		virtual const char* type() const {
			return "InsertBeforeOp";
		}
	};

	class ReplaceOp : public RewriteOperation
	{
	public:
		ReplaceOp(size_t from, size_t to, ANTLR_USE_NAMESPACE(std)string text)
		: RewriteOperation(from,text)
		, lastIndex(to)
		{
		}
		virtual ~ReplaceOp() {}
		virtual size_t execute( ANTLR_USE_NAMESPACE(std)ostream& out ) {
			out << text;
			return lastIndex+1;
		}
		virtual const char* type() const {
			return "ReplaceOp";
		}
	protected:
		size_t lastIndex;
	};

	class DeleteOp : public ReplaceOp {
	public:
		DeleteOp(size_t from, size_t to)
		: ReplaceOp(from,to,"")
		{
		}
		virtual const char* type() const {
			return "DeleteOp";
		}
	};

	TokenStreamRewriteEngine(TokenStream& upstream);

	TokenStreamRewriteEngine(TokenStream& upstream, size_t initialSize);

	RefToken nextToken( void );

	void rollback(size_t instructionIndex) {
		rollback(DEFAULT_PROGRAM_NAME, instructionIndex);
	}

	/** Rollback the instruction stream for a program so that
	 *	 the indicated instruction (via instructionIndex) is no
	 *	 longer in the stream.	UNTESTED!
	 */
	void rollback(const ANTLR_USE_NAMESPACE(std)string& programName,
					  size_t instructionIndex );

	void deleteProgram() {
		deleteProgram(DEFAULT_PROGRAM_NAME);
	}

	/** Reset the program so that no instructions exist */
	void deleteProgram(const ANTLR_USE_NAMESPACE(std)string& programName) {
		rollback(programName, MIN_TOKEN_INDEX);
	}

	void insertAfter( RefTokenWithIndex t,
							const ANTLR_USE_NAMESPACE(std)string& text )
	{
		insertAfter(DEFAULT_PROGRAM_NAME, t, text);
	}

	void insertAfter(size_t index, const ANTLR_USE_NAMESPACE(std)string& text) {
		insertAfter(DEFAULT_PROGRAM_NAME, index, text);
	}

	void insertAfter( const ANTLR_USE_NAMESPACE(std)string& programName,
							RefTokenWithIndex t,
							const ANTLR_USE_NAMESPACE(std)string& text )
	{
		insertAfter(programName, t->getIndex(), text);
	}

	void insertAfter( const ANTLR_USE_NAMESPACE(std)string& programName,
							size_t index,
							const ANTLR_USE_NAMESPACE(std)string& text )
	{
		// to insert after, just insert before next index (even if past end)
		insertBefore(programName,index+1, text);
	}

	void insertBefore( RefTokenWithIndex t,
							 const ANTLR_USE_NAMESPACE(std)string& text )
	{
		// std::cout << "insertBefore index " << t->getIndex() << " " << text << std::endl;
		insertBefore(DEFAULT_PROGRAM_NAME, t, text);
	}

	void insertBefore(size_t index, const ANTLR_USE_NAMESPACE(std)string& text) {
		insertBefore(DEFAULT_PROGRAM_NAME, index, text);
	}

	void insertBefore( const ANTLR_USE_NAMESPACE(std)string& programName,
							 RefTokenWithIndex t,
							 const ANTLR_USE_NAMESPACE(std)string& text )
	{
		insertBefore(programName, t->getIndex(), text);
	}

	void insertBefore( const ANTLR_USE_NAMESPACE(std)string& programName,
							 size_t index,
							 const ANTLR_USE_NAMESPACE(std)string& text )
	{
		addToSortedRewriteList(programName, new InsertBeforeOp(index,text));
	}

	void replace(size_t index, const ANTLR_USE_NAMESPACE(std)string& text)
	{
		replace(DEFAULT_PROGRAM_NAME, index, index, text);
	}

	void replace( size_t from, size_t to,
					  const ANTLR_USE_NAMESPACE(std)string& text)
	{
		replace(DEFAULT_PROGRAM_NAME, from, to, text);
	}

	void replace( RefTokenWithIndex indexT,
					  const ANTLR_USE_NAMESPACE(std)string& text )
	{
		replace(DEFAULT_PROGRAM_NAME, indexT->getIndex(), indexT->getIndex(), text);
	}

	void replace( RefTokenWithIndex from,
					  RefTokenWithIndex to,
					  const ANTLR_USE_NAMESPACE(std)string& text )
	{
		replace(DEFAULT_PROGRAM_NAME, from, to, text);
	}

	void replace(const ANTLR_USE_NAMESPACE(std)string& programName,
					 size_t from, size_t to,
					 const ANTLR_USE_NAMESPACE(std)string& text )
	{
		addToSortedRewriteList(programName,new ReplaceOp(from, to, text));
	}

	void replace( const ANTLR_USE_NAMESPACE(std)string& programName,
					  RefTokenWithIndex from,
					  RefTokenWithIndex to,
					  const ANTLR_USE_NAMESPACE(std)string& text )
	{
		replace(programName,
				  from->getIndex(),
				  to->getIndex(),
				  text);
	}

	void remove(size_t index) {
		remove(DEFAULT_PROGRAM_NAME, index, index);
	}

	void remove(size_t from, size_t to) {
		remove(DEFAULT_PROGRAM_NAME, from, to);
	}

	void remove(RefTokenWithIndex indexT) {
		remove(DEFAULT_PROGRAM_NAME, indexT, indexT);
	}

	void remove(RefTokenWithIndex from, RefTokenWithIndex to) {
		remove(DEFAULT_PROGRAM_NAME, from, to);
	}

	void remove( const ANTLR_USE_NAMESPACE(std)string& programName,
					 size_t from, size_t to)
	{
		replace(programName,from,to,"");
	}

	void remove( const ANTLR_USE_NAMESPACE(std)string& programName,
					 RefTokenWithIndex from, RefTokenWithIndex to )
	{
		replace(programName,from,to,"");
	}

	void discard(int ttype) {
		discardMask.add(ttype);
	}

	RefToken getToken( size_t i )
	{
		return RefToken(tokens.at(i));
	}

	size_t getTokenStreamSize() const {
		return tokens.size();
	}

	void originalToStream( ANTLR_USE_NAMESPACE(std)ostream& out ) const {
		ANTLR_USE_NAMESPACE(std)for_each( tokens.begin(), tokens.end(), tokenToStream(out) );
	}

	void originalToStream( ANTLR_USE_NAMESPACE(std)ostream& out,
								  size_t start, size_t end ) const;

	void toStream( ANTLR_USE_NAMESPACE(std)ostream& out ) const {
		toStream( out, MIN_TOKEN_INDEX, getTokenStreamSize());
	}

	void toStream( ANTLR_USE_NAMESPACE(std)ostream& out,
						const ANTLR_USE_NAMESPACE(std)string& programName ) const
	{
		toStream( out, programName, MIN_TOKEN_INDEX, getTokenStreamSize());
	}

	void toStream( ANTLR_USE_NAMESPACE(std)ostream& out,
						size_t start, size_t end ) const
	{
		toStream(out, DEFAULT_PROGRAM_NAME, start, end);
	}

	void toStream( ANTLR_USE_NAMESPACE(std)ostream& out,
						const ANTLR_USE_NAMESPACE(std)string& programName,
						size_t firstToken, size_t lastToken ) const;

	void toDebugStream( ANTLR_USE_NAMESPACE(std)ostream& out ) const {
		toDebugStream( out, MIN_TOKEN_INDEX, getTokenStreamSize());
	}

	void toDebugStream( ANTLR_USE_NAMESPACE(std)ostream& out,
							  size_t start, size_t end ) const;

	size_t getLastRewriteTokenIndex() const {
		return getLastRewriteTokenIndex(DEFAULT_PROGRAM_NAME);
	}

	/** Return the last index for the program named programName
	 * return 0 if the program does not exist or the program is empty.
	 * (Note this is different from the java implementation that returns -1)
	 */
	size_t getLastRewriteTokenIndex(const ANTLR_USE_NAMESPACE(std)string& programName) const {
		program_map::const_iterator rewrites = programs.find(programName);

		if( rewrites == programs.end() )
			return 0;

		const operation_list& prog = rewrites->second;
		if( !prog.empty() )
		{
			operation_list::const_iterator last = prog.end();
			--last;
			return (*last)->getIndex();
		}
		return 0;
	}

protected:
	/** If op.index > lastRewriteTokenIndexes, just add to the end.
	 *	 Otherwise, do linear */
	void addToSortedRewriteList(RewriteOperation* op) {
		addToSortedRewriteList(DEFAULT_PROGRAM_NAME, op);
	}

	void addToSortedRewriteList( const ANTLR_USE_NAMESPACE(std)string& programName,
										  RewriteOperation* op );

protected:
	/** Who do we suck tokens from? */
	TokenStream& stream;
	/** track index of tokens */
	size_t index;

	/** Track the incoming list of tokens */
	token_list tokens;

	/** You may have multiple, named streams of rewrite operations.
	 *  I'm calling these things "programs."
	 *  Maps String (name) -> rewrite (List)
	 */
	program_map programs;

	/** Which (whitespace) token(s) to throw out */
	BitSet discardMask;
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif
