#ifndef PASTEL_DISTANCE_SEGMENT_POINT_HPP
#define PASTEL_DISTANCE_SEGMENT_POINT_HPP

#include "pastel/geometry/distance_segment_point.h"
#include "pastel/geometry/closest_segment_point.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(const Segment<N, Real>& segment,
		const Point<N, Real>& point)
	{
		const Real t = closest(segment, point);
		const Vector<N, Real> delta = segment.at(t) - point;

		return dot(delta, delta);
	}

}

#endif
