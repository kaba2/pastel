// Description: Distance between two aligned boxes

#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Computes the distance between two aligned boxes.
	template <typename Real, int N, typename NormBijection>
	Real distance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection);

	//! Computes the distance between two aligned boxes.
	/*!
	This is a convenience function that calls:

	distance2(aBox, bBox, 
		Euclidean_NormBijection<Real>());
	*/
	template <typename Real, int N>
	Real distance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox);

	//! Computes the farthest distance between two aligned boxes.
	template <typename Real, int N, typename NormBijection>
	Real farthestDistance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection);

	//! Computes the farthest distance between two aligned boxes.
	/*!
	This is a convenience function that calls:

	farthestDistance2(aBox, bBox, 
		Euclidean_NormBijection<Real>());
	*/
	template <typename Real, int N>
	Real farthestDistance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox);

}

#include "pastel/geometry/distance_alignedbox_alignedbox.hpp"

#endif
