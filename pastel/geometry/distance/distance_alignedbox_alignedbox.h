// Description: Distance between two aligned boxes

#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_H
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_H

#include "pastel/geometry/shape/alignedbox.h"

#include "pastel/math/euclidean_normbijection.h"

namespace Pastel
{

	//! Computes the distance between two aligned boxes.
	/*!
	This is a convenience function which returns
	normBijection.toNorm(
		distance2(aBox, bBox, normBijection)).
	*/
	template <
		typename Real, int N, 
		typename NormBijection = Euclidean_NormBijection<Real>>
	Real distance(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection = NormBijection());

	//! Computes the norm-bijection distance between two aligned boxes.
	/*!
	Preconditions:
	aBox.n() == bBox.n()

	Time complexity: O(aBox.n())

	The distance is computed assuming that the boxes are solid. 
	*/
	template <
		typename Real, int N, 
		typename NormBijection = Euclidean_NormBijection<Real>>
	Real distance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection = NormBijection());

	//! Computes the farthest distance between two aligned boxes.
	/*!
	This is a convenience function which returns
	normBijection.toNorm(
		farthestDistance2(aBox, bBox, normBijection)).
	*/
	template <
		typename Real, int N, 
		typename NormBijection = Euclidean_NormBijection<Real>>
	Real farthestDistance(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection = NormBijection());

	//! Computes the farthest norm-bijection distance between two aligned boxes.
	/*!
	Preconditions:
	aBox.n() == bBox.n()

	Time complexity: O(aBox.n())

	The distance is computed assuming that the boxes are solid. 
	*/
	template <
		typename Real, int N, 
		typename NormBijection = Euclidean_NormBijection<Real>>
	Real farthestDistance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection = NormBijection());

}

#include "pastel/geometry/distance/distance_alignedbox_alignedbox.hpp"

#endif
