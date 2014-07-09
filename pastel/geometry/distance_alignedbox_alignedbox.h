// Description: Distance between two aligned boxes

#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Computes the distance between two aligned boxes.
	/*!
	The distance is computed assuming that the boxes
	are solid. 
	*/
	template <typename Real, int N, 
		typename NormBijection = Euclidean_NormBijection<Real>>
	Real distance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection = NormBijection());

	//! Computes the farthest distance between two aligned boxes.
	template <typename Real, int N, 
		typename NormBijection = Euclidean_NormBijection<Real>>
	Real farthestDistance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection = NormBijection());

}

#include "pastel/geometry/distance_alignedbox_alignedbox.hpp"

#endif
