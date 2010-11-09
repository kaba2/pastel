// Description: Distance between two points

#ifndef PASTEL_DISTANCE_POINT_POINT_H
#define PASTEL_DISTANCE_POINT_POINT_H

#include "pastel/sys/vector.h"

#include "pastel/math/normbijection_concept.h"

namespace Pastel
{

	//! Computes the Euclidean distance between points.
	template <typename Real, int N>
	Real distance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	//! Computes the Manhattan distance between points.
	template <typename Real, int N>
	Real manhattanDistance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	//! Computes the Euclidean distance between points.
	template <typename Real, int N>
	Real euclideanDistance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	//! Computes a Minkowski distance between points.
	/*!
	Preconditions:
	metric >= 1
	*/
	template <typename Real, int N>
	Real minkowskiDistance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint,
		const PASTEL_NO_DEDUCTION(Real)& metric);

	//! Computes the maximum distance between points.
	template <typename Real, int N>
	Real maximumDistance(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	//! Computes the squared Euclidean distance between points.
	template <typename Real, int N>
	Real distance2(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	//! Computes the squared Euclidean distance between points.
	/*!
	Preconditions:
	dimension >= 0
	*/
	template <
		typename Real_ConstIterator_A,
		typename Real_ConstIterator_B, 
		typename NormBijection>
	typename NormBijection::Real distance2(
		Real_ConstIterator_A aPoint,
		Real_ConstIterator_B bPoint,
		integer dimension,
		const NormBijection& normBijection);

	//! Computes the squared Euclidean distance between points.
	/*!
	Preconditions:
	dimension >= 0

	The computation is terminated if at some point
	the distance gets greater than the 'cullDistance'.
	This can lead to faster performance when the dimension
	is higher.
	*/
	template <
		typename Real_ConstIterator_A,
		typename Real_ConstIterator_B, 
		typename NormBijection>
	typename NormBijection::Real distance2(
		Real_ConstIterator_A aPoint,
		Real_ConstIterator_B bPoint,
		integer dimension,
		const NormBijection& normBijection,
		const typename NormBijection::Real& cullDistance);

	//! Computes the squared Euclidean distance between points.
	template <typename Real, int N, typename NormBijection>
	Real distance2(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint,
		const NormBijection& normBijection);

	//! Computes the squared Euclidean distance between points.
	/*!
	The computation is terminated if at some point
	the distance gets greater than the 'cullDistance'.
	This can lead to faster performance when the dimension
	is higher.
	*/
	template <typename Real, int N, typename NormBijection>
	Real distance2(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint,
		const NormBijection& normBijection,
		const PASTEL_NO_DEDUCTION(Real)& cullDistance);

}

#include "pastel/geometry/distance_point_point.hpp"

#endif
