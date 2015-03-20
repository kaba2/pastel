// Description: Floating point number as a real number
// Documentation: native_reals.txt

#ifndef PASTELSYS_NATIVE_REAL_H
#define PASTELSYS_NATIVE_REAL_H

#include "pastel/sys/real/real_concept.h"

#include <limits>
#include <cmath>

namespace Pastel
{

	namespace NativeReal_
	{

		struct Tag{};

	}

	// Real

	template <typename Type, EnableIf<std::is_floating_point<Type>> = 0>
	Type infinity(NativeReal_::Tag = NativeReal_::Tag())
	{
		return std::numeric_limits<Type>::infinity();
	}

	template <typename Type, EnableIf<std::is_floating_point<Type>> = 0>
	bool isInfinity(const Type& that)
	{
		return that == std::numeric_limits<Type>::infinity();
	}

	template <typename Type, EnableIf<std::is_floating_point<Type>> = 0>
	bool isMinusInfinity(const Type& that)
	{
		return that == -std::numeric_limits<Type>::infinity();
	}

	template <typename Type, EnableIf<std::is_floating_point<Type>> = 0>
	Type nan()
	{
		return std::numeric_limits<Type>::quiet_NaN();
	}

	template <typename Type, EnableIf<std::is_floating_point<Type>> = 0>
	bool isNan(const Type& that)
	{
		return that != that;
	}

	using std::floor;
	using std::ceil;

	// Field

	template <typename Type, EnableIf<std::is_floating_point<Type>> = 0>
	Type inverse(Type that)
	{
		return 1 / that;
	}

	// Ring

	// Operators *= and * are built-in.

	// Ordered additive monoid.

	using std::abs;

	template <typename Type, EnableIf<std::is_floating_point<Type>> = 0>
	bool negative(Type that)
	{
		return that < 0;
	}

	template <typename Type, EnableIf<std::is_floating_point<Type>> = 0>
	bool positive(Type that)
	{
		return that > 0;
	}

	// Additive monoid

	template <typename Type, EnableIf<std::is_floating_point<Type>> = 0>
	bool zero(Type that)
	{
		return that == 0;
	}

}

#endif
