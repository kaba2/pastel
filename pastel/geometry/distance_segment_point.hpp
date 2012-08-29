#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_POINT_HPP
#define PASTELGEOMETRY_DISTANCE_SEGMENT_POINT_HPP

#include "pastel/geometry/distance_segment_point.h"
#include "pastel/geometry/closest_segment_point.h"

namespace Pastel
{

	template <typename Real, int N>
	Real distance2(
		const Segment<Real, N>& segment,
		const Vector<Real, N>& point)
	{
		PENSURE_OP(segment.dimension(), ==, point.dimension());

		const Real t = closest(segment, point);
		const Vector<Real, N> delta = segment.at(t) - point;

		return dot(delta, delta);
	}

}

#endif
