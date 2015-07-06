// Description: Next greater and next smaller floating point values
// Documentation: math_functions.txt

#ifndef PASTELSYS_EPS_H
#define PASTELSYS_EPS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/real/real_concept.h"

namespace Pastel
{

	//! Returns the next greater 64-bit floating point value.
	/*!
	returns:
	The smallest floating point value > that, unless
	'that' is -infinity, +infinity, or NaN, in which
	itself.
	*/
	real64_ieee nextGreater(real64_ieee that);

	//! Returns the next smaller 64-bit floating point value.
	/*!
	returns:
	The greatest floating point value < that, unless
	'that' is -infinity, +infinity, or NaN, in which
	itself.
	*/
	real64_ieee nextSmaller(real64_ieee that);

	//! Returns the next greater 32-bit floating point value.
	/*!
	See the documentation for the 64-bit version.
	*/
	real32_ieee nextGreater(real32_ieee that);

	//! Returns the next smaller 32-bit floating point value.
	/*!
	See the documentation for the 64-bit version.
	*/
	real32_ieee nextSmaller(real32_ieee that);

}

#include "pastel/sys/math/eps.hpp"

#endif
