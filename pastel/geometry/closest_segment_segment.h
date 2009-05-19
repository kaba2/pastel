#ifndef PASTEL_CLOSEST_SEGMENT_SEGMENT_H
#define PASTEL_CLOSEST_SEGMENT_SEGMENT_H

#include "pastel/geometry/segment.h"

namespace Pastel
{

	template <int N, typename Real>
	Tuple<2, Real> closest(
		const Segment<N, Real>& aSegment,
		const Segment<N, Real>& bSegment);

}

#include "pastel/geometry/closest_segment_segment.hpp"

#endif
