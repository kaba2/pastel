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
