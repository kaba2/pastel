// Description: Distance between two line segments

#ifndef PASTEL_DISTANCE_SEGMENT_SEGMENT_H
#define PASTEL_DISTANCE_SEGMENT_SEGMENT_H

#include "pastel/geometry/segment.h"

namespace Pastel
{

	template <typename Real, int N>
	Real distance2(
		const Segment<Real, N>& aSegment,
		const Segment<Real, N>& bSegment);
}

#include "pastel/geometry/distance_segment_segment.hpp"

#endif
