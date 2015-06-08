// Description: Native types as an ordered monoid concept
// DocumentationOf: ordered_monoid_concept.h

#ifndef PASTELSYS_NATIVE_ORDERED_MONOID_H
#define PASTELSYS_NATIVE_ORDERED_MONOID_H

#include "pastel/sys/sfinae.h"

#include <cstdlib>

#define PASTEL_DEFINE_ABS(T) inline const T abs(T that) {return that;}

namespace Pastel
{

	// Ordered additive monoid

	// The abs-functions for unsigned integers
	// need to be defined as normal functions;
	// a single function template constrained
	// to unsigned integers will not do. The
	// reason is that otherwise abs((uchar)1)
	// is ambiguous between std::abs(int) and
	// the template.
	PASTEL_DEFINE_ABS(uchar)
	PASTEL_DEFINE_ABS(uint)
	PASTEL_DEFINE_ABS(ulong)
	PASTEL_DEFINE_ABS(ulonglong)

}

#undef PASTEL_DEFINE_ABS

namespace Pastel
{

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
