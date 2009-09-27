// Description: Distance between an aligned box and a point

#ifndef PASTEL_DISTANCE_ALIGNEDBOX_POINT_H
#define PASTEL_DISTANCE_ALIGNEDBOX_POINT_H

#include "pastel/geometry/alignedbox.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Calculates the squared distance between an aligned box and a point.

	template <typename Real, int N>
	Real distance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point);

	//! Calculates a bijective distance between an aligned box and a point.

	template <typename Real, int N, typename NormBijection>
	Real distance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point,
		const NormBijection& normBijection);

	//! Calculates farthest squared distance between an aligned box and a point.

	template <typename Real, int N>
	Real farthestDistance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point);

	//! Calculates farthest bijective distance between an aligned box and a point.

	template <typename Real, int N, typename NormBijection>
	Real farthestDistance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point,
		const NormBijection& normBijection);

}

#include "pastel/geometry/distance_alignedbox_point.hpp"

#endif
