// Description: Algorithms for aligned boxes

#ifndef PASTELSYS_ALIGNEDBOX_TOOLS_H
#define PASTELSYS_ALIGNEDBOX_TOOLS_H

#include "pastel/sys/alignedbox.h"

namespace Pastel
{

	//! Converts a point to a linear index in an integer grid.
	/*!
	point:
	The point to convert.	

	stride:
	The 'stride[i]' tells how much the index increases
	when moving one unit along the i:th axis.

	returns:
	The linear index, which is computed from 'dot(point, stride)'.
	*/
	template <int N>
	integer linearIndex(
		const Vector<integer, N>& point,
		const Vector<integer, N>& stride);

	//! Converts a linear index into a point in an integer grid.
	/*!
	This is the inverse function of 'linearIndex()'. 
	See the documentation for that function.
	*/
	template <int N>
	Vector<integer, N> position(
		integer linearIndex,
		const Vector<integer, N>& stride,
		const Vector<integer, N>& order);

	//! Converts a linear index into a point in an integer grid.
	/*!
	This function assumes that order = [0, 1, ..., n - 1],
	i.e. row-major.
	*/
	template <int N>
	Vector<integer, N> position(
		integer linearIndex,
		const Vector<integer, N>& stride);

	//! Calls 'positionVisitor' at each point in the region.
	/*!
	region:
	The set of points to visit.

	positionVisitor:
	The functor to call at each point.
	See 'positionvisitor.txt'.
	
	Returns:
	The number of visited points.

	The number of visited points is not necessarily the number
	of points in the 'region', because the 'positionVisitor'
	can ask for an early exit.
	*/
	template <int N, typename PositionVisitor>
	integer forEach(
		const AlignedBox<integer, N>& region,
		PositionVisitor positionVisitor);

	template <typename Real, int N>
	Vector<Real, N> discreteToContinuous(
		const AlignedBox<Real, N>& continuousRange,
		const AlignedBox<integer, N>& discreteRange,
		const Vector<integer, N>& discretePoint);

	template <typename Real, int N>
	AlignedBox<Real, N> discreteToContinuous(
		const AlignedBox<Real, N>& continuousRange,
		const AlignedBox<integer, N>& discreteRange,
		const AlignedBox<integer, N>& discreteBox);

	template <typename Real, int N>
	Vector<integer, N> continuousToDiscrete(
		const AlignedBox<Real, N>& continuousRange,
		const AlignedBox<integer, N>& discreteRange,
		const Vector<Real, N>& continuousPoint);

	template <typename Real, int N>
	AlignedBox<integer, N> continuousToDiscrete(
		const AlignedBox<Real, N>& continuousRange,
		const AlignedBox<integer, N>& discreteRange,
		const AlignedBox<Real, N>& continuousBox);

}

#include "pastel/sys/alignedbox/alignedbox_tools.hpp"

#endif
