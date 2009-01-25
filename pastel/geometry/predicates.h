#ifndef PASTELGEOMETRY_PREDICATES_H
#define PASTELGEOMETRY_PREDICATES_H

#include "pastel/sys/point.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	template <int N, typename Real>
	Real side(
		const Point<N, Real>& point,
		const Plane<N, Real>& plane);

}

#include "pastel/geometry/predicates.hpp"

#endif
