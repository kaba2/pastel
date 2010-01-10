// Description: Distance between two points

#ifndef PASTEL_DISTANCE_POINT_POINT_H
#define PASTEL_DISTANCE_POINT_POINT_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real, int N>
	Real distance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	template <typename Real, int N>
	Real manhattanDistance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	template <typename Real, int N>
	Real euclideanDistance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	template <typename Real, int N>
	Real minkowskiDistance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint,
		const PASTEL_NO_DEDUCTION(Real)& metric);

	template <typename Real, int N>
	Real maximumDistance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	template <typename Real, int N>
	Real distance2(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	template <typename Real, typename NormBijection>
	Real distance2(
		const Real* aPoint,
		const Real* bPoint,
		integer dimension,
		const NormBijection& normBijection);

	template <typename Real, int N, typename NormBijection>
	Real distance2(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint,
		const NormBijection& normBijection);

	template <typename Real, int N, typename NormBijection>
	Real distance2(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint,
		const NormBijection& normBijection,
		const PASTEL_NO_DEDUCTION(Real)& cullDistance);

}

#include "pastel/geometry/distance_point_point.hpp"

#endif
