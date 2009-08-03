// Description: Distance computation between an aligned box and a point

#ifndef PASTEL_DISTANCE_ALIGNEDBOX_POINT_H
#define PASTEL_DISTANCE_ALIGNEDBOX_POINT_H

#include "pastel/geometry/alignedbox.h"

#include "pastel/sys/point.h"

namespace Pastel
{

	//! Calculates the squared distance between an aligned box and a point.

	template <int N, typename Real>
	Real distance2(
		const AlignedBox<N, Real>& alignedBox,
		const Point<Real, N>& point);

	//! Calculates a bijective distance between an aligned box and a point.

	template <int N, typename Real, typename NormBijection>
	Real distance2(
		const AlignedBox<N, Real>& alignedBox,
		const Point<Real, N>& point,
		const NormBijection& normBijection);

	//! Calculates farthest squared distance between an aligned box and a point.

	template <int N, typename Real>
	Real farthestDistance2(
		const AlignedBox<N, Real>& alignedBox,
		const Point<Real, N>& point);

	//! Calculates farthest bijective distance between an aligned box and a point.

	template <int N, typename Real, typename NormBijection>
	Real farthestDistance2(
		const AlignedBox<N, Real>& alignedBox,
		const Point<Real, N>& point,
		const NormBijection& normBijection);

}

#include "pastel/geometry/distance_alignedbox_point.hpp"

#endif
