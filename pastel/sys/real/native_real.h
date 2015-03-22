// Description: Floating point number as a real number
// Documentation: native_reals.txt

#ifndef PASTELSYS_NATIVE_REAL_H
#define PASTELSYS_NATIVE_REAL_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/real/ieee_float.h"

#include <limits>
#include <cmath>

namespace Pastel
{

	// Additive monoid

	template <
		typename Type, 
		EnableIf<std::is_floating_point<Type>> = 0>
	bool zero(Type that)
	{
		return that == 0;
	}

	// Ordered additive monoid.

	using std::abs;

	template <
		typename Type, 
		EnableIf<std::is_floating_point<Type>> = 0>
	bool negative(Type that)
	{
		return that < 0;
	}

	template <
		typename Type, 
		EnableIf<std::is_floating_point<Type>> = 0>
	bool positive(Type that)
	{
		return that > 0;
	}

	// Ring

	// Operators *= and * are built-in.

	// Field

	template <
		typename Type, 
		EnableIf<std::is_floating_point<Type>> = 0>
	Type inverse(Type that)
	{
		return 1 / that;
	}

	// Real

	template <
		typename Type, 
		EnableIf<std::is_floating_point<Type>> = 0>
	Type infinity()
	{
		return std::numeric_limits<Type>::infinity();
	}

	template <
		typename Type, 
		EnableIf<std::is_floating_point<Type>> = 0>
	bool isInfinity(const Type& that)
	{
		return that == std::numeric_limits<Type>::infinity();
	}

	template <
		typename Type, 
		EnableIf<std::is_floating_point<Type>> = 0>
	bool isMinusInfinity(const Type& that)
	{
		return that == -std::numeric_limits<Type>::infinity();
	}

	template <
		typename Type, 
		EnableIf<std::is_floating_point<Type>> = 0>
	Type nan()
	{
		return std::numeric_limits<Type>::quiet_NaN();
	}

	template <
		typename Type, 
		EnableIf<std::is_floating_point<Type>> = 0>
	bool isNan(const Type& that)
	{
		return that != that;
	}

	using std::floor;
	using std::ceil;

	template <
		typename Type, 
		EnableIf<std::is_floating_point<Type>> = 0>
	ScientificNotation asScientific(const Type& that)
	{
		return ieeeFloatAsScientific(that);
	}

}

#endif
