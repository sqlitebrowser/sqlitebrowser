/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/src/BitSet.cpp#2 $
 */
#include "antlr/BitSet.hpp"
#include <string>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

BitSet::BitSet(unsigned int nbits)
: storage(nbits)
{
	for (unsigned int i = 0; i < nbits ; i++ )
		storage[i] = false;
}

BitSet::BitSet( const unsigned long* bits_, unsigned int nlongs )
: storage(nlongs*32)
{
	for ( unsigned int i = 0 ; i < (nlongs * 32); i++)
		storage[i] = (bits_[i>>5] & (1UL << (i&31))) ? true : false;
}

BitSet::~BitSet()
{
}

void BitSet::add(unsigned int el)
{
	if( el >= storage.size() )
		storage.resize( el+1, false );

	storage[el] = true;
}

bool BitSet::member(unsigned int el) const
{
	if ( el >= storage.size())
		return false;

	return storage[el];
}

ANTLR_USE_NAMESPACE(std)vector<unsigned int> BitSet::toArray() const
{
	ANTLR_USE_NAMESPACE(std)vector<unsigned int> elems;
	for (unsigned int i = 0; i < storage.size(); i++)
	{
		if (storage[i])
			elems.push_back(i);
	}

	return elems;
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif
