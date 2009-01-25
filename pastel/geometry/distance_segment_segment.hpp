#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_SEGMENT_HPP
#define PASTELGEOMETRY_DISTANCE_SEGMENT_SEGMENT_HPP

#include "pastel/geometry/distance_segment_segment.h"
#include "pastel/geometry/closest_segment_segment.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(
		const Segment<N, Real>& aSegment,
		const Segment<N, Real>& bSegment)
	{
		const Tuple<2, Real> t = closest(aSegment, bSegment);
		return dot(bSegment.at(t[1]) - aSegment.at(t[0]));
	}

}

#endif
