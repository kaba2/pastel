// Description: Closest points between line segments

#ifndef PASTELGEOMETRY_CLOSEST_SEGMENT_SEGMENT_H
#define PASTELGEOMETRY_CLOSEST_SEGMENT_SEGMENT_H

#include "pastel/geometry/shape/segment.h"

namespace Pastel
{

	//! Computes closest points between line segments.
	/*!
	Preconditions:
	aSegment.n() == bSegment.n()

	Time complexity:
	O(n), where n is the dimension.

	Returns:
	Parameters (s, t) such that 'aSegment.at(s)' and
	'bSegment.at(t)' are closest points.
	*/
	template <typename Real, integer N>
	Tuple<Real, 2> closest(
		const Segment<Real, N>& aSegment,
		const Segment<Real, N>& bSegment);

}

#include "pastel/geometry/closest/closest_segment_segment.hpp"

#endif
