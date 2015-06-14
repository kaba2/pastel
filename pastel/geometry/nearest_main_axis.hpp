#ifndef PASTELGEOMETRY_NEAREST_MAIN_AXIS_HPP
#define PASTELGEOMETRY_NEAREST_MAIN_AXIS_HPP

#include "pastel/geometry/nearest_main_axis.h"

namespace Pastel
{

	template <typename Real, integer N>
	integer nearestMainAxis(
		const Vector<Real, N>& that)
	{
		// projectedLength = |<e_i, that> / |e_i|| = |that[i]|
		return maxIndex(abs(that));
	}

}

#endif
