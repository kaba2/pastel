// Description: Hashing of dereferenced address
// Documentation: hashes.txt

#ifndef PASTEL_ITERATORADDRESS_HASH_H
#define PASTEL_ITERATORADDRESS_HASH_H

#include "pastel/sys/hashing.h"

namespace Pastel
{

	//! Computes the hash from dereferenced address.
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

#endif
