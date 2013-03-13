#ifndef INC_BitSet_hpp__
#define INC_BitSet_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.antlr/release/antlr-2.7.7/lib/cpp/antlr/BitSet.hpp#2 $
 */

#include <antlr/config.hpp>
#include <vector>
#include <stdexcept>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** A BitSet to replace java.util.BitSet.
 * Primary differences are that most set operators return new sets
 * as opposed to oring and anding "in place".  Further, a number of
 * operations were added.  I cannot contain a BitSet because there
 * is no way to access the internal bits (which I need for speed)
 * and, because it is final, I cannot subclass to add functionality.
 * Consider defining set degree.  Without access to the bits, I must
 * call a method n times to test the ith bit...ack!
 *
 * Also seems like or() from util is wrong when size of incoming set is bigger
 * than this.length.
 *
 * This is a C++ version of the Java class described above, with only
 * a handful of the methods implemented, because we don't need the
 * others at runtime. It's really just a wrapper around vector<bool>,
 * which should probably be changed to a wrapper around bitset, once
 * bitset is more widely available.
 *
 * @author Terence Parr, MageLang Institute
 * @author <br><a href="mailto:pete@yamuna.demon.co.uk">Pete Wells</a>
 */
class ANTLR_API BitSet {
private:
	ANTLR_USE_NAMESPACE(std)vector<bool> storage;

public:
	BitSet( unsigned int nbits=64 );
	BitSet( const unsigned long* bits_, unsigned int nlongs);
	~BitSet();

	void add( unsigned int el );

	bool member( unsigned int el ) const;

	ANTLR_USE_NAMESPACE(std)vector<unsigned int> toArray() const;
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_BitSet_hpp__
