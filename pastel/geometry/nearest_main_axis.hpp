#ifndef PASTEL_NEAREST_MAIN_AXIS_HPP
#define PASTEL_NEAREST_MAIN_AXIS_HPP

#include "pastel/geometry/nearest_main_axis.h"

namespace Pastel
{

	template <int N, typename Real>
	integer nearestMainAxis(
		const Vector<N, Real>& that)
	{
		// projectedLength = |<e_i, that> / |e_i|| = |that[i]|
		return maxIndex(mabs(that));
	}

}

#endif
