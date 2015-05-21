#ifndef PASTELSYS_NATIVE_ORDERED_ADDITIVE_MONOID_H
#define PASTELSYS_NATIVE_ORDERED_ADDITIVE_MONOID_H

#include "pastel/sys/sfinae_macros.h"

#include <cstdlib>

namespace Pastel
{

	// Ordered additive monoid

	template <
		typename Type, 
		EnableIf<std::is_unsigned<Type>> = 0>
	const Type& abs(const Type& that)
	{
		// std::abs does not support unsigned types.
		return that;
	}

	using std::abs;

	template <
		typename Type, 
		EnableIf<std::is_arithmetic<Type>> = 0>
	bool negative(const Type& that)
	{
		return that < 0;
	}

	template <
		typename Type, 
		EnableIf<std::is_arithmetic<Type>> = 0>
	bool positive(const Type& that)
	{
		return that > 0;
	}

}

#endif
