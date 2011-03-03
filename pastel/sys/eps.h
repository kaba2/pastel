// Description: Next greater and next smaller floating point values
// Documentation: math_functions.txt

#ifndef PASTEL_EPS_H
#define PASTEL_EPS_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns the next greater 64-bit floating point value.
	real64_ieee nextGreater(real64_ieee that);

	//! Returns the next smaller 64-bit floating point value.
	real64_ieee nextSmaller(real64_ieee that);

	//! Returns the next greater 32-bit floating point value.
	real32_ieee nextGreater(real32_ieee that);

	//! Returns the next smaller 32-bit floating point value.
	real32_ieee nextSmaller(real32_ieee that);

}

#include "pastel/sys/eps.hpp"

#endif
