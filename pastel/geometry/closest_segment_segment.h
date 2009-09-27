// Description: Closest points between line segments

#ifndef PASTEL_CLOSEST_SEGMENT_SEGMENT_H
#define PASTEL_CLOSEST_SEGMENT_SEGMENT_H

#include "pastel/geometry/segment.h"

namespace Pastel
{

	//! Computes closest points between line segments.
	/*!
	Returns:
	Parameters (s, t) such that 'aSegment.at(s)' and
	'bSegment.at(t)' are closest points.
	*/
	template <typename Real, int N>
	Tuple<Real, 2> closest(
		const Segment<Real, N>& aSegment,
		const Segment<Real, N>& bSegment);

}

#include "pastel/geometry/closest_segment_segment.hpp"

#endif
