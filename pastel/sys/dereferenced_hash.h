// Description: Hash from dereferenced value
// Documentation: hash.txt

#ifndef PASTEL_DEREFERENCED_HASH_H
#define PASTEL_DEREFERENCED_HASH_H

#include "pastel/sys/hash.h"

namespace Pastel
{

	//! Computes hash from dereferenced value.
	class Dereferenced_Hash
	{
	public:
		template <typename Iterator>
		hash_integer operator()(const Iterator& iter) const
		{
			return computeHash(*iter);
		}
	};

}

#endif
