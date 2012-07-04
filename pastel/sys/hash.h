// Description: Hash functions
// Documentation: hash_tables.txt

#ifndef PASTEL_HASH_H
#define PASTEL_HASH_H

#include "pastel/sys/iterator_range.h"
#include "pastel/sys/keyvalue.h"

#include <functional>
#include <string>

namespace Pastel
{

	//! Computes a hash for 'that'.
	template <typename Type>
	hash_integer computeHash(
		const Type& that);

	//! Combines an existing hash with the hash of 'that'.
	template <class Type> 
	hash_integer combineHash(
		hash_integer hash, 
		const Type& that);

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
