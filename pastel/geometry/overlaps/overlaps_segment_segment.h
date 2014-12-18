// Description: Overlap tests between two line segments

#ifndef PASTELGEOMETRY_OVERLAPS_SEGMENT_SEGMENT_H
#define PASTELGEOMETRY_OVERLAPS_SEGMENT_SEGMENT_H

#include "pastel/geometry/segment.h"

namespace Pastel
{

	//! Tests if two segments overlap.

	/*!
	The segments are considered to be open. Only those
	intersections that result in a point are reported.
	This test is only reasonable in dimensions 1 and 2.
	In larger dimensions the probability of random segments
	intersecting is zero. Use distance
	calculations instead to tell the
	proximity of two segments in larger dimensions.
	*/

	template <typename Real>
	bool overlaps(
		const Segment<Real, 1>& aSegment,
		const Segment<Real, 1>& bSegment);

	//! Tests if two segments overlap.

	/*!
	The segments are considered to be open. Only those
	intersections that result in a point are reported.
	This test is only reasonable in dimensions 1 and 2.
	In larger dimensions the probability of random segments
	intersecting is zero. Use distance
	calculations instead to tell the
	proximity of two segments in larger dimensions.
	*/

	template <typename Real>
	bool overlaps(
		const Segment<Real, 2>& aSegment,
		const Segment<Real, 2>& bSegment);

}

#include "pastel/geometry/overlaps/overlaps_segment_segment.hpp"

#endif
