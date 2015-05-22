#ifndef PASTELSYS_NATIVE_ORDERED_ADDITIVE_MONOID_H
#define PASTELSYS_NATIVE_ORDERED_ADDITIVE_MONOID_H

#include "pastel/sys/sfinae.h"

#include <cstdlib>

namespace Pastel
{

	// Ordered additive monoid

	template <
		typename Type, 
		Requires<std::is_unsigned<Type>> = 0>
	const Type& abs(const Type& that)
	{
		// std::abs does not support unsigned types.
		// An unsigned integer is already non-negative.
		return that;
	}

	// Support abs() for floating-point and signed integers.
	using std::abs;

	template <
		typename Type, 
		RequiresSome<
			std::is_floating_point<Type>,
			std::is_signed<Type>
		> = 0
	>
	bool negative(const Type& that)
	{
		return that < 0;
	}

	template <
		typename Type, 
		Requires<std::is_unsigned<Type>> = 0
	>
	bool negative(const Type& that)
	{
		// An unsigned integer is never negative.
		return false;
	}

	template <
		typename Type, 
		Requires<std::is_arithmetic<Type>> = 0>
	bool positive(const Type& that)
	{
		return that > 0;
	}

}

#endif
