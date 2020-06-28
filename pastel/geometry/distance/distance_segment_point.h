// Description: Distance between a line segment and a point

#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_POINT_H
#define PASTELGEOMETRY_DISTANCE_SEGMENT_POINT_H

#include "pastel/geometry/shape/segment.h"
#include "pastel/sys/vector.h"
#include "pastel/geometry/closest/closest_segment_point.h"
#include "pastel/math/norm/euclidean_norm.h"

namespace Pastel
{

	//! Euclidean distance between a line segment and a point.
	/*!
	Preconditions:
	segment.n() == point.n()

	Time complexity: O(segment.n())
	*/
	template <typename Real, int N>
	auto distance2(
		const Segment<Real, N>& segment,
		const Vector<Real, N>& point)
	{
		PENSURE_OP(segment.n(), ==, point.n());
		auto norm = Euclidean_Norm<Real>();
		const Real t = closest(segment, point);
		return norm[dot(segment.at(t) - point)];
	}

}

#endif
