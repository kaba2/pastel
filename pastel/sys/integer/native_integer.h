// Description: Native integer type as a finite integer.

#ifndef PASTELSYS_NATIVE_INTEGER_H
#define PASTELSYS_NATIVE_INTEGER_H

#include "pastel/sys/integer/integer_concept.h"
#include "pastel/sys/real/real_concept.h"

#include <limits>
#include <climits>
#include <cmath>
#include <type_traits>

namespace Pastel
{

	// Additive semi-group

	// Operators += and + are inbuilt.

	// Additive monoid

	template <
		typename Type, 
		EnableIf<std::is_integral<Type>> = 0>
	bool zero(const Type& that)
	{
		return that == 0;
	}

	// Ordered additive monoid

	using std::abs;

	template <
		typename Type, 
		EnableIf<std::is_unsigned<Type>> = 0>
	const Type& abs(const Type& that)
	{
		// std::abs does not support unsigned types.
		return that;
	}

	template <
		typename Type, 
		EnableIf<std::is_integral<Type>> = 0>
	bool negative(const Type& that)
	{
		return that < 0;
	}

	template <
		typename Type, 
		EnableIf<std::is_integral<Type>> = 0>
	bool positive(const Type& that)
	{
		return that > 0;
	}

	// Integer

	template <
		typename Type, 
		EnableIf<std::is_unsigned<Type>> = 0>
	bool even(const Type& that)
	{
		return (that & 1) == 0;
	}

	template <
		typename Type, 
		EnableIf<std::is_signed<Type>> = 0>
	bool even(const Type& that)
	{
		// This should read the following, but I can not 
		// include twos_complement.h, because that would 
		// create a circular dependency.
		
		// return even(signedToTwosComplement(that));

		// This is the equivalent code.
		using Unsigned = typename std::make_unsigned<Type>::type;
		return even((Unsigned)that);
	}

	template <
		typename Type, 
		EnableIf<std::is_integral<Type>> = 0>
	bool odd(const Type& that)
	{
		return !even(that);
	}

	template <
		typename Type, 
		EnableIf<std::is_integral<Type>> = 0>
	ScientificNotation asScientific(const Type& that)
	{
		return ScientificNotation {negative(that), 0, abs(that)};
	}

	// Finite integer

	template <
		typename Type, 
		EnableIf<std::is_integral<Type>> = 0>
	integer bits(const Type& that)
	{
		return SizeInBits<Type>::value;
	}

}

#endif
