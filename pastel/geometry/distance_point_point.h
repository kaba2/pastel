#ifndef PASTELGEOMETRY_DISTANCE_POINT_POINT_H
#define PASTELGEOMETRY_DISTANCE_POINT_POINT_H

#include "pastel/sys/point.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distanceManhattan(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint);

	template <int N, typename Real>
	Real distance2(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint);

	template <int N, typename Real>
	Real distancePower(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint,
		const PASTEL_NO_DEDUCTION(Real)& metric);

	template <int N, typename Real>
	Real distanceInfinity(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint);

}

#include "pastel/geometry/distance_point_point.hpp"

#endif
