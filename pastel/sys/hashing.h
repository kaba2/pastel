// Description: Hash computation
// Documentation: hashing.txt

#ifndef PASTELSYS_HASHING_H
#define PASTELSYS_HASHING_H

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
	template <typename Integer>
	PASTEL_ENABLE_IF_C(sizeof(Integer) == sizeof(uint32), Integer) 
		combineHash(Integer left, Integer right);

	//! Combines an existing 64-bit hash with the hash of 'that'.
	template <typename Integer>
	PASTEL_ENABLE_IF_C(sizeof(Integer) == sizeof(uint64), Integer) 
		combineHash(Integer left, Integer right);

	//! Sequentially combines the hashes of the input values.
	template <typename ConstRange>
	hash_integer computeHashMany(
		const ConstRange& input);

}

#include "pastel/sys/hashing.hpp"

#endif
