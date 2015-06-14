// Description: Distance between two line segments

#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_SEGMENT_H
#define PASTELGEOMETRY_DISTANCE_SEGMENT_SEGMENT_H

#include "pastel/geometry/shape/segment.h"

namespace Pastel
{

	//! Euclidean distance between line segments.
	/*!
	This is a convenience function which returns
	std::sqrt(distance2(aSegment, bSegment)).
	*/
	template <typename Real, integer N>
	Real distance(
		const Segment<Real, N>& aSegment,
		const Segment<Real, N>& bSegment);

	//! Computes the squared distance between line segments.
	/*!
	Preconditions:
	aSegment.n() == bSegment.n()

	Time complexity: O(aSegment.n())
	*/
	template <typename Real, integer N>
	Real distance2(
		const Segment<Real, N>& aSegment,
		const Segment<Real, N>& bSegment);
}

#include "pastel/geometry/distance/distance_segment_segment.hpp"

#endif
