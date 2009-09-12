// Description: Sweeping a given shape along a line segment
// Documentation: pastelgeometry.txt

#ifndef PASTEL_SWEEP_H
#define PASTEL_SWEEP_H

#include "pastel/geometry/segment.h"
#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	//! Sweeps a given shape along a line segment.
	/*!
	Time complexity: O(n)
	Exception safety: strong

	The pivot point of the shape is assumed
	to be the origin.
	The x-axis of the pen is aligned with
	the segment.
	*/

	template <typename Real>
	void orientedSweep(
		const std::vector<Point<Real, 2> >& pen,
		const Segment<Real, 2>& segment,
		std::vector<Point<Real, 2> >& result);

	//! Sweeps a given shape along a line segment.
	/*!
	Time complexity: O(n)
	Exception safety: strong

	The pivot point of the shape is assumed
	to be the origin.
	*/

	template <typename Real>
	void sweep(
		const std::vector<Point<Real, 2> >& pen,
		const Segment<Real, 2>& segment,
		std::vector<Point<Real, 2> >& result);

}

#include "pastel/geometry/sweep.hpp"

#endif
