// Description: Hash for std::pair
// Documentation: hashes.txt

#ifndef PASTEL_PAIR_HASH_H
#define PASTEL_PAIR_HASH_H

#include "pastel/sys/hashing.h"

namespace Pastel
{

	//! Computes hash for std::pair.
	class Pair_Hash
	{
	public:
		template <typename First, typename Second>
		hash_integer operator()(
			const std::pair<First, Second>& that) const
		{
			return combineHash(
				computeHash(that.first),
				computeHash(that.second));
		}
	};


}

#endif
