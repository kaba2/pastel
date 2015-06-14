#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_POINT_HPP
#define PASTELGEOMETRY_DISTANCE_SEGMENT_POINT_HPP

#include "pastel/geometry/distance/distance_segment_point.h"
#include "pastel/geometry/closest/closest_segment_point.h"

namespace Pastel
{

	template <typename Real, integer N>
	Real distance(
		const Segment<Real, N>& segment,
		const Vector<Real, N>& point)
	{
		return std::sqrt(distance2(segment, point));
	}

	template <typename Real, integer N>
	Real distance2(
		const Segment<Real, N>& segment,
		const Vector<Real, N>& point)
	{
		PENSURE_OP(segment.n(), ==, point.n());

		Real t = closest(segment, point);
		Vector<Real, N> delta = segment.at(t) - point;

		return dot(delta, delta);
	}

}

#endif
