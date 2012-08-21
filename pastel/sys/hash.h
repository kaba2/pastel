// Description: Hash computation
// Documentation: hash.txt

#ifndef PASTEL_HASH_H
#define PASTEL_HASH_H

#include "pastel/sys/range.h"
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
	uint32 combineHash(uint32 left, uint32 right);

	//! Combines an existing 64-bit hash with the hash of 'that'.
	uint64 combineHash(uint64 left, uint64 right);

	//! Sequentially combines the hashes of the input values.
	template <typename ConstRange>
	hash_integer computeHashMany(
		const ConstRange& input);

}

#include "pastel/sys/hash.hpp"

#endif
