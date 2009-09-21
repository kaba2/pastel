// Description: Linear interpolation
// Documentation: math.txt

#ifndef PASTEL_LINEAR_H
#define PASTEL_LINEAR_H

#include "pastel/sys/vector.h"
#include "pastel/sys/tuple.h"

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
	template <int N, typename Real>
	Vector<Real, N> linear(
		const Vector<Real, N>& startPoint,
		const Vector<Real, N>& endPoint,
		const PASTEL_NO_DEDUCTION(Real)& time);

	//! Interpolates linearly in a cube.
	/*!
	Complexity:
	O(2^n), where n is the dimensionality.

	data:
	The values of the cube vertices 
	in lexicographical order in coordinates.
	*/
	template <typename Type, int N>
	Type linear(
		const Vector<real, N>& position,
		const Tuple<Type, ModifyN<N, 1 << N>::Result>& data);

}

#include "pastel/sys/linear.hpp"

#endif
