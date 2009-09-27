#ifndef PASTEL_DISTANCE_SEGMENT_POINT_HPP
#define PASTEL_DISTANCE_SEGMENT_POINT_HPP

#include "pastel/geometry/distance_segment_point.h"
#include "pastel/geometry/closest_segment_point.h"

namespace Pastel
{

	template <typename Real, int N>
	Real distance2(const Segment<Real, N>& segment,
		const Vector<Real, N>& point)
	{
		const Real t = closest(segment, point);
		const Vector<Real, N> delta = segment.at(t) - point;

		return dot(delta, delta);
	}

}

#endif
