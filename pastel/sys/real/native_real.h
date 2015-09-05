// Description: Floating point number as a real number
// Documentation: native_reals.txt

#ifndef PASTELSYS_NATIVE_REAL_H
#define PASTELSYS_NATIVE_REAL_H

// See concept.txt on why real_concept.h must not
// be included here.
//#include "pastel/sys/real/real_concept.h"

#include "pastel/sys/real/scientific_notation_fwd.h"

#include <type_traits>
#include <limits>
#include <cmath>

namespace Pastel
{

	// Real

	template <
		typename Type, 
		Requires<std::is_floating_point<Type>> = 0>
	bool isInfinity(const Type& that)
	{
		return that == std::numeric_limits<Type>::infinity();
	}

	template <
		typename Type, 
		Requires<std::is_floating_point<Type>> = 0>
	bool isMinusInfinity(const Type& that)
	{
		return that == -std::numeric_limits<Type>::infinity();
	}

	template <
		typename Type, 
		Requires<std::is_floating_point<Type>> = 0>
	Type nan()
	{
		return std::numeric_limits<Type>::quiet_NaN();
	}

	template <
		typename Type, 
		Requires<std::is_floating_point<Type>> = 0>
	bool isNan(const Type& that)
	{
		return that != that;
	}

	using std::floor;
	using std::ceil;

}

#include "pastel/sys/real/ieee_float.h"

namespace Pastel
{

	template <
		typename Type, 
		Requires<std::is_floating_point<Type>> = 0>
	ScientificNotation asScientific(const Type& that)
	{
		return ieeeFloatAsScientific(that);
	}

}

#endif
