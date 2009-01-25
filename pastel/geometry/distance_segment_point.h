#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_POINT_H
#define PASTELGEOMETRY_DISTANCE_SEGMENT_POINT_H

#include "pastel/geometry/segment.h"
#include "pastel/geometry/point.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(const Segment<N, Real>& segment,
		const Point<N, Real>& point);

}

#include "pastel/geometry/distance_segment_point.hpp"

#endif
