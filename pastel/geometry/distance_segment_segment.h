#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_SEGMENT_H
#define PASTELGEOMETRY_DISTANCE_SEGMENT_SEGMENT_H

#include "pastel/geometry/segment.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(
		const Segment<N, Real>& aSegment,
		const Segment<N, Real>& bSegment);
}

#include "pastel/geometry/distance_segment_segment.hpp"

#endif
