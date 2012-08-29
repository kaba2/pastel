// Description: Distance between an aligned box and a point

#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_H
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_H

#include "pastel/geometry/alignedbox.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes a bijective distance between an aligned box and a point.
	template <typename Real, int N, typename NormBijection>
	Real distance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point,
		const NormBijection& normBijection);

	//! Computes the squared distance between an aligned box and a point.
	/*!
	This is a convenience function that calls:

	distance2(alignedBox, point,
		Euclidean_NormBijection<Real>())
	*/
	template <typename Real, int N>
	Real distance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point);

	//! Computes farthest bijective distance between an aligned box and a point.
	template <typename Real, int N, typename NormBijection>
	Real farthestDistance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point,
		const NormBijection& normBijection);

	//! Computes farthest squared distance between an aligned box and a point.
	/*!
	This is a convenience function that calls:
	
	fartherDistance2(
		alignedBox, point,
		Euclidean_NormBijection<Real>())
	*/
	template <typename Real, int N>
	Real farthestDistance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/distance_alignedbox_point.hpp"

#endif
