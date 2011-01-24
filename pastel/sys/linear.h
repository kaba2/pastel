// Description: Linear interpolation
// Documentation: basic_math.txt

#ifndef PASTEL_LINEAR_H
#define PASTEL_LINEAR_H

#include "pastel/sys/vector.h"
#include "pastel/sys/tuple.h"
#include "pastel/sys/iteratorrange.h"

#include <iterator>

namespace Pastel
{

	//! Interpolates linearly between points.
	/*!
	linear(startPoint, endPoint, time) =
	startPoint + time * (endPoint - startPoint)
	*/
	template <
		typename PointType,
		typename Real>
	PointType linear(
			const PointType& startPoint,
			const PointType& endPoint,
			const Real& time);

	//! Interpolates linearly between vectors.
	template <typename Real, int N>
	Vector<Real, N> linear(
		const Vector<Real, N>& startPoint,
		const Vector<Real, N>& endPoint,
		const PASTEL_NO_DEDUCTION(Real)& time);

	//! Interpolates linearly in a [0, 1]^n cube.
	/*!
	Preconditions:
	data.size() >= (1 << n)

	Time complexity:
	O(2^n), where n is the dimensionality.

	data:
	The values of the cube vertices 
	in lexicographical order in coordinates.
	*/
	template <int N, typename Iterator>
	typename std::iterator_traits<Iterator>::value_type linear(
		const Vector<real, N>& position,
		const RandomAccessIterator_Range<Iterator>& data);

	//! Return midpoint of two points.
	/*!
	This is a convenience function that calls:

	linear(startPoint, endPoint, (Real)0.5);
	*/
	template <typename Real, int N>
	Vector<Real, N> midpoint(
		const Vector<Real, N>& startPoint,
		const Vector<Real, N>& endPoint);

}

#include "pastel/sys/linear.hpp"

#endif
