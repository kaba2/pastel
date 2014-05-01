// Description: Floating point number as a real number
// Documentation: native_reals.txt

#ifndef PASTELSYS_NATIVE_REAL_H
#define PASTELSYS_NATIVE_REAL_H

#include "pastel/sys/real_concept.h"

#include <limits>
#include <cmath>

namespace Pastel
{

	// Real

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_floating_point<Type>, Type) 
		infinity()
	{
		return std::numeric_limits<Type>::infinity();
	}

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_floating_point<Type>, Type) 
		nan()
	{
		return std::numeric_limits<Type>::quiet_NaN();
	}

	using std::floor;
	using std::ceil;

	// Field

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_floating_point<Type>, Type) 
		inverse(Type that)
	{
		return 1 / that;
	}

	// Ring

	// Operators *= and * are built-in.

	// Ordered additive monoid.

	using std::abs;

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_floating_point<Type>, bool) 
		negative(Type that)
	{
		return that < 0;
	}

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_floating_point<Type>, bool) 
		positive(Type that)
	{
		return that > 0;
	}

	// Additive monoid

	template <typename Type>
	PASTEL_ENABLE_IF(std::is_floating_point<Type>, bool) 
		zero(Type that)
	{
		return that == 0;
	}

}

#endif