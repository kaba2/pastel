#ifndef PASTELGEOMETRY_DISTANCE_POINT_POINT_HPP
#define PASTELGEOMETRY_DISTANCE_POINT_POINT_HPP

#include "pastel/geometry/distance_point_point.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distanceManhattan(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint)
	{
		const Vector<N, Real> delta(bPoint - aPoint);
		return normManhattan(delta);
	}

	template <int N, typename Real>
	Real distance2(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint)
	{
		const Vector<N, Real> delta(bPoint - aPoint);
		return dot(delta, delta);
	}

	template <int N, typename Real>
	Real distancePower(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint,
		const PASTEL_NO_DEDUCTION(Real)& metric)
	{
		const Vector<N, Real> delta(bPoint - aPoint);
		return normPower(delta, metric);
	}

	template <int N, typename Real>
	Real distanceInfinity(
		const Point<N, Real>& aPoint,
		const Point<N, Real>& bPoint)
	{
		const Vector<N, Real> delta(bPoint - aPoint);
		return normInfinity(delta);
	}

}

#endif
