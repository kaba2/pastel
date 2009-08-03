// Description: Some geometric predicates
// Documentation: common.txt

#ifndef PASTEL_PREDICATES_H
#define PASTEL_PREDICATES_H

#include "pastel/sys/point.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	template <int N, typename Real>
	Real side(
		const Point<Real, N>& point,
		const Plane<Real, N>& plane);

}

#include "pastel/geometry/predicates.hpp"

#endif
