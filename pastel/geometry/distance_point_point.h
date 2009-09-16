#ifndef PASTEL_DISTANCE_POINT_POINT_H
#define PASTEL_DISTANCE_POINT_POINT_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distanceManhattan(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	template <int N, typename Real>
	Real distance2(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	template <int N, typename Real>
	Real distancePower(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint,
		const PASTEL_NO_DEDUCTION(Real)& metric);

	template <int N, typename Real>
	Real distanceInfinity(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	template <int N, typename Real, typename NormBijection>
	Real distance2(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint,
		const NormBijection& normBijection);

	template <int N, typename Real, typename NormBijection>
	Real distance2(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint,
		const NormBijection& normBijection,
		const PASTEL_NO_DEDUCTION(Real)& cullDistance);

}

#include "pastel/geometry/distance_point_point.hpp"

#endif
