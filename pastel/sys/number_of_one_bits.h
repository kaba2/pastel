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
			PASTEL_CONSTEXPR bool value =
				std::is_unsigned<Type>::value &&
				(sizeof(Type) * CHAR_BIT) == N;
		};

	}

	//! Returns the number of 1-bits in a 64-bit unsigned integer.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Type>
	PASTEL_ENABLE_IF((NumberOfOneBits_::IsUnsigned<Type, 64>), integer)
		numberOfOneBits(Type that);

	//! Returns the number of 1-bits in a 32-bit unsigned integer.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Type>
	PASTEL_ENABLE_IF((NumberOfOneBits_::IsUnsigned<Type, 32>), integer)
		numberOfOneBits(Type that);

	//! Returns the number of 1-bits in a 16-bit unsigned integer.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Type>
	PASTEL_ENABLE_IF((NumberOfOneBits_::IsUnsigned<Type, 16>), integer)
		numberOfOneBits(Type that);

	//! Returns the number of 1-bits in an 8-bit unsigned integer.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Type>
	PASTEL_ENABLE_IF((NumberOfOneBits_::IsUnsigned<Type, 8>), integer)
		numberOfOneBits(Type that);

}

#include "pastel/sys/number_of_one_bits.hpp"

#endif
