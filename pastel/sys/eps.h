// Description: Next greater and next smaller floating point values
// Documentation: math.txt

#ifndef PASTEL_EPS_H
#define PASTEL_EPS_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	real64_ieee nextGreater(real64_ieee that);

	real64_ieee nextSmaller(real64_ieee that);

	real32_ieee nextGreater(real32_ieee that);

	real32_ieee nextSmaller(real32_ieee that);

}

#include "pastel/sys/eps.hpp"

#endif
