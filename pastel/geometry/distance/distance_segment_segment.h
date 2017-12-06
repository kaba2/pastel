// Description: Distance between two line segments

#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_SEGMENT_H
#define PASTELGEOMETRY_DISTANCE_SEGMENT_SEGMENT_H

#include "pastel/geometry/shape/segment.h"
#include "pastel/geometry/closest/closest_segment_segment.h"
#include "pastel/math/norm/euclidean_norm.h"

namespace Pastel
{

	//! Euclidean distance between line segments.
	/*!
	Preconditions:
	aSegment.n() == bSegment.n()

	Time complexity: O(aSegment.n())
	*/
	template <typename Real, integer N>
	auto distance2(
		const Segment<Real, N>& aSegment,
		const Segment<Real, N>& bSegment)
	{
		PENSURE_OP(aSegment.n(), ==, bSegment.n());
		auto norm = Euclidean_Norm<Real>();
		const Tuple<Real, 2> t = closest(aSegment, bSegment);
		return norm[dot(bSegment.at(t[1]) - aSegment.at(t[0]))];
	}

}

#endif
