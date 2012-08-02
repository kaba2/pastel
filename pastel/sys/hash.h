// Description: Hash functions
// Documentation: hash.txt

#ifndef PASTEL_HASH_H
#define PASTEL_HASH_H

#include "pastel/sys/iterator_range.h"
#include "pastel/sys/keyvalue.h"
#include "pastel/sys/mytypes.h"

#include <boost/type_traits/is_integral.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>

#include <functional>
#include <string>

namespace Pastel
{

	//! Computes a hash for 'that'.
	template <typename Type>
	hash_integer computeHash(
		const Type& that);

	//! Combines an existing 32-bit hash with the hash of 'that'.
	template <typename Integer, typename Type> 
	PASTEL_ENABLE_IF((boost::mpl::and_<
		boost::is_integral<Integer>, 
		boost::mpl::bool_<sizeof(Integer) == sizeof(uint32)>>), Integer) 
		combineHash(Integer hash, const Type& that);

	//! Combines an existing 64-bit hash with the hash of 'that'.
	template <typename Integer, typename Type> 
	PASTEL_ENABLE_IF((boost::mpl::and_<
		boost::is_integral<Integer>, 
		boost::mpl::bool_<sizeof(Integer) == sizeof(uint64)>>), Integer) 
		combineHash(Integer hash, const Type& that);

	//! Sequentially combines the hashes of the input values.
	template <typename ConstIterator>
	hash_integer computeHashMany(
		ForwardIterator_Range<ConstIterator> input);

}

namespace Pastel
{

	class IteratorAddress_Hash
	{ 
	public:
		template <typename Iterator>
		hash_integer operator()(const Iterator& iter) const
		{
			return computeHash(&*iter);
		}
	};

}

#include "pastel/sys/hash.hpp"

#endif
