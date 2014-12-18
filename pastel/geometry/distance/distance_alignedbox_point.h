// Description: Distance between an aligned box and a point

#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_H
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_H

#include "pastel/geometry/shapes/alignedbox.h"

#include "pastel/math/euclidean_normbijection.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Distance between an aligned box and a point.
	/*!
	This is a convenience function which returns
	normBijection.toNorm(
		distance2(alignedBox, point, normBijection)).
	*/
	template <typename Real, int N, 
		typename NormBijection = Euclidean_NormBijection<Real>>
	Real distance(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point,
		const NormBijection& normBijection = NormBijection());

	//! Bijective distance between an aligned box and a point.
	/*!
	Preconditions:
	alignedBox.n() == point.n()

	Time complexity: O(alignedBox.n())

	It is assumed that the aligned box is solid.
	*/
	template <typename Real, int N, 
		typename NormBijection = Euclidean_NormBijection<Real>>
	Real distance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point,
		const NormBijection& normBijection = NormBijection());

	//! Farthest distance between an aligned box and a point.
	/*!
	This is a convenience function which returns
	normBijection.toNorm(
		farthestDistance2(alignedBox, point, normBijection)).
	*/
	template <typename Real, int N, 
		typename NormBijection = Euclidean_NormBijection<Real>>
	Real farthestDistance(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point,
		const NormBijection& normBijection = NormBijection());

	//! Farthest bijective distance between an aligned box and a point.
	/*!
	Preconditions:
	alignedBox.n() == point.n()

	Time complexity: O(alignedBox.n())

	It is assumed that the aligned box is solid.
	*/
	template <typename Real, int N, 
		typename NormBijection = Euclidean_NormBijection<Real>>
	Real farthestDistance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point,
		const NormBijection& normBijection = NormBijection());

}

#include "pastel/geometry/distance/distance_alignedbox_point.hpp"

#endif
