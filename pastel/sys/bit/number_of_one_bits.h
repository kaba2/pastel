// Description: Number of one bits
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_NUMBER_OF_ONE_BITS_H
#define PASTELSYS_NUMBER_OF_ONE_BITS_H

#include "pastel/sys/mytypes.h"
#include <limits.h>

namespace Pastel
{

	namespace NumberOfOneBits_
	{

		template <typename Type, int N>
		struct IsUnsigned
		{
		public:
			static constexpr bool value =
				std::is_unsigned<Type>::value &&
				(sizeof(Type) * CHAR_BIT) == N;
		};

	}

	//! Returns the number of 1-bits in a 64-bit unsigned integer.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Type, Requires<NumberOfOneBits_::IsUnsigned<Type, 64>> = 0>
	integer numberOfOneBits(Type that);

	//! Returns the number of 1-bits in a 32-bit unsigned integer.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Type, Requires<NumberOfOneBits_::IsUnsigned<Type, 32>> = 0>
	integer numberOfOneBits(Type that);

	//! Returns the number of 1-bits in a 16-bit unsigned integer.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Type, Requires<NumberOfOneBits_::IsUnsigned<Type, 16>> = 0>
	integer numberOfOneBits(Type that);

	//! Returns the number of 1-bits in an 8-bit unsigned integer.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Type, Requires<NumberOfOneBits_::IsUnsigned<Type, 8>> = 0>
	integer numberOfOneBits(Type that);

}

#include "pastel/sys/bit/number_of_one_bits.hpp"

#endif
