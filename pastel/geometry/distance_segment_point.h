// Description: Distance between a line segment and a point

#ifndef PASTEL_DISTANCE_SEGMENT_POINT_H
#define PASTEL_DISTANCE_SEGMENT_POINT_H

#include "pastel/geometry/segment.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(const Segment<Real, N>& segment,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/distance_segment_point.hpp"

#endif
