#ifndef PASTELSYS_NUMBER_OF_ONE_BITS_HPP
#define PASTELSYS_NUMBER_OF_ONE_BITS_HPP

#include "pastel/sys/bit/number_of_one_bits.h"

namespace Pastel
{

	template <typename Type, EnableIf<NumberOfOneBits_::IsUnsigned<Type, 64>>>
	integer numberOfOneBits(Type that)
	{
		that = 
			( that        & 0x5555555555555555ull) +
			((that >> 1)  & 0x5555555555555555ull);

		that = 
			( that        & 0x3333333333333333ull) +
			((that >> 2)  & 0x3333333333333333ull);

		that = 
			( that        & 0x0F0F0F0F0F0F0F0Full) +
			((that >> 4)  & 0x0F0F0F0F0F0F0F0Full);

		that = 
			( that        & 0x00FF00FF00FF00FFull) +
			((that >> 8)  & 0x00FF00FF00FF00FFull);

		that = 
			( that        & 0x0000FFFF0000FFFFull) +
			((that >> 16) & 0x0000FFFF0000FFFFull);

		that = 
			( that        & 0x00000000FFFFFFFFull) +
			((that >> 32) & 0x00000000FFFFFFFFull);

		return (integer)that;
	}

	template <typename Type, EnableIf<NumberOfOneBits_::IsUnsigned<Type, 32>>>
	integer numberOfOneBits(Type that)
	{
		that = 
			( that        & 0x55555555ul) +
			((that >> 1)  & 0x55555555ul);

		that = 
			( that        & 0x33333333ul) +
			((that >> 2)  & 0x33333333ul);

		that = 
			( that        & 0x0F0F0F0Ful) +
			((that >> 4)  & 0x0F0F0F0Ful);

		that = 
			( that        & 0x00FF00FFul) +
			((that >> 8)  & 0x00FF00FFul);

		that = 
			( that        & 0x0000FFFFul) +
			((that >> 16) & 0x0000FFFFul);

		return (integer)that;
	}

	template <typename Type, EnableIf<NumberOfOneBits_::IsUnsigned<Type, 16>>>
	integer numberOfOneBits(Type that)
	{
		that = 
			( that        & 0x5555u) +
			((that >> 1)  & 0x5555u);

		that = 
			( that        & 0x3333u) +
			((that >> 2)  & 0x3333u);

		that = 
			( that        & 0x0F0Fu) +
			((that >> 4)  & 0x0F0Fu);

		that = 
			( that        & 0x00FFu) +
			((that >> 8)  & 0x00FFu);

		return (integer)that;
	}

	template <typename Type, EnableIf<NumberOfOneBits_::IsUnsigned<Type, 8>>>
	integer numberOfOneBits(Type that)
	{
		that = 
			( that        & 0x55u) +
			((that >> 1)  & 0x55u);

		that = 
			( that        & 0x33u) +
			((that >> 2)  & 0x33u);

		that = 
			( that        & 0x0Fu) +
			((that >> 4)  & 0x0Fu);

		return (integer)that;
	}

}

#endif
