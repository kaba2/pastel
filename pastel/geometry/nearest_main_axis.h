#ifndef PASTEL_NEAREST_MAIN_AXIS_H
#define PASTEL_NEAREST_MAIN_AXIS_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N, typename Real>
	integer nearestMainAxis(
		const Vector<N, Real>& that);

}

#include "pastel/geometry/nearest_main_axis.hpp"

#endif
