#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_SEGMENT_HPP
#define PASTELGEOMETRY_DISTANCE_SEGMENT_SEGMENT_HPP

#include "pastel/geometry/distance_segment_segment.h"
#include "pastel/geometry/closest_segment_segment.h"

namespace Pastel
{

	template <typename Real, int N>
	Real distance2(
		const Segment<Real, N>& aSegment,
		const Segment<Real, N>& bSegment)
	{
		PENSURE_OP(aSegment.n(), ==, bSegment.n());

		const Tuple<Real, 2> t = closest(aSegment, bSegment);
		return dot(bSegment.at(t[1]) - aSegment.at(t[0]));
	}

}

#endif
