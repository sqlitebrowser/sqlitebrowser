#include <antlr/config.hpp>

#include <string>
#include <list>
#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include <iterator>
#include <sstream>
#include <cassert>

#include <antlr/TokenStream.hpp>
#include <antlr/TokenWithIndex.hpp>
#include <antlr/BitSet.hpp>
#include <antlr/TokenStreamRewriteEngine.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

#ifndef NO_STATIC_CONSTS
const size_t TokenStreamRewriteEngine::MIN_TOKEN_INDEX = 0;
const int TokenStreamRewriteEngine::PROGRAM_INIT_SIZE = 100;
#endif

const char* TokenStreamRewriteEngine::DEFAULT_PROGRAM_NAME = "default";

namespace {

	struct compareOperationIndex {
		typedef TokenStreamRewriteEngine::RewriteOperation RewriteOperation;
		bool operator() ( const RewriteOperation* a, const RewriteOperation* b ) const
		{
			return a->getIndex() < b->getIndex();
		}
	};
	struct dumpTokenWithIndex {
		dumpTokenWithIndex( ANTLR_USE_NAMESPACE(std)ostream& o ) : out(o) {}
		void operator() ( const RefTokenWithIndex& t ) {
			out << "[txt='" << t->getText() << "' tp=" << t->getType() << " idx=" << t->getIndex() << "]\n";
		}
		ANTLR_USE_NAMESPACE(std)ostream& out;
	};
}

TokenStreamRewriteEngine::TokenStreamRewriteEngine(TokenStream& upstream)
: stream(upstream)
, index(MIN_TOKEN_INDEX)
, tokens()
, programs()
, discardMask()
{
}

TokenStreamRewriteEngine::TokenStreamRewriteEngine(TokenStream& upstream, size_t initialSize )
: stream(upstream)
, index(MIN_TOKEN_INDEX)
, tokens(initialSize)
, programs()
, discardMask()
{
}

RefToken TokenStreamRewriteEngine::nextToken( void )
{
	RefTokenWithIndex t;
	// suck tokens until end of stream or we find a non-discarded token
	do {
		t = RefTokenWithIndex(stream.nextToken());
		if ( t )
		{
			t->setIndex(index);  // what is t's index in list?
			if ( t->getType() != Token::EOF_TYPE ) {
				tokens.push_back(t);  // track all tokens except EOF
			}
			index++;			// move to next position
		}
	} while ( t && discardMask.member(t->getType()) );
	return RefToken(t);
}

void TokenStreamRewriteEngine::rollback( const std::string& programName,
													  size_t instructionIndex )
{
	program_map::iterator rewrite = programs.find(programName);
	if( rewrite != programs.end() )
	{
		operation_list& prog = rewrite->second;
		operation_list::iterator
			j = prog.begin(),
			end = prog.end();

		std::advance(j,instructionIndex);
		if( j != end )
			prog.erase(j, end);
	}
}

void TokenStreamRewriteEngine::originalToStream( std::ostream& out,
																 size_t start,
																 size_t end ) const
{
	token_list::const_iterator s = tokens.begin();
	std::advance( s, start );
	token_list::const_iterator e = s;
	std::advance( e, end-start );
	std::for_each( s, e, tokenToStream(out) );
}

void TokenStreamRewriteEngine::toStream( std::ostream& out,
													  const std::string& programName,
													  size_t firstToken,
													  size_t lastToken ) const
{
	if( tokens.size() == 0 )
		return;

	program_map::const_iterator rewriter = programs.find(programName);

	if ( rewriter == programs.end() )
		return;

	// get the prog and some iterators in it...
	const operation_list& prog = rewriter->second;
	operation_list::const_iterator
		rewriteOpIndex = prog.begin(),
		rewriteOpEnd = prog.end();

	size_t tokenCursor = firstToken;
	// make sure we don't run out of the tokens we have...
	if( lastToken > (tokens.size() - 1) )
		lastToken = tokens.size() - 1;

    while ( tokenCursor <= lastToken )
    {
        // std::cout << "tokenCursor = " << tokenCursor << " first prog index = " << (*rewriteOpIndex)->getIndex() << std::endl;

        if( rewriteOpIndex != rewriteOpEnd )
        {
            size_t up_to_here = std::min(lastToken,(*rewriteOpIndex)->getIndex());
            while( tokenCursor < up_to_here )
                out << tokens[tokenCursor++]->getText();
        }
        while ( rewriteOpIndex != rewriteOpEnd &&
                tokenCursor == (*rewriteOpIndex)->getIndex() &&
                tokenCursor <= lastToken )
        {
            tokenCursor = (*rewriteOpIndex)->execute(out);
            ++rewriteOpIndex;
        }
        if( tokenCursor <= lastToken )
            out << tokens[tokenCursor++]->getText();
    }
	// std::cout << "Handling tail operations # left = " << std::distance(rewriteOpIndex,rewriteOpEnd) << std::endl;
	// now see if there are operations (append) beyond last token index
	std::for_each( rewriteOpIndex, rewriteOpEnd, executeOperation(out) );
	rewriteOpIndex = rewriteOpEnd;
}

void TokenStreamRewriteEngine::toDebugStream( std::ostream& out,
															 size_t start,
															 size_t end ) const
{
	token_list::const_iterator s = tokens.begin();
	std::advance( s, start );
	token_list::const_iterator e = s;
	std::advance( e, end-start );
	std::for_each( s, e, dumpTokenWithIndex(out) );
}

void TokenStreamRewriteEngine::addToSortedRewriteList( const std::string& programName,
																		 RewriteOperation* op )
{
	program_map::iterator rewrites = programs.find(programName);
	// check if we got the program already..
	if ( rewrites == programs.end() )
	{
		// no prog make a new one...
		operation_list ops;
		ops.push_back(op);
		programs.insert(std::make_pair(programName,ops));
		return;
	}
	operation_list& prog = rewrites->second;

	if( prog.empty() )
	{
		prog.push_back(op);
		return;
	}

	operation_list::iterator i, end = prog.end();
	i = end;
	--i;
	// if at or beyond last op's index, just append
	if ( op->getIndex() >= (*i)->getIndex() ) {
		prog.push_back(op); // append to list of operations
		return;
	}
	i = prog.begin();

	if( i != end )
	{
		operation_list::iterator pos = std::upper_bound( i, end, op, compareOperationIndex() );
		prog.insert(pos,op);
	}
	else
		prog.push_back(op);
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif
