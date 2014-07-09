// Description: Distance between a line segment and a point

#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_POINT_H
#define PASTELGEOMETRY_DISTANCE_SEGMENT_POINT_H

#include "pastel/geometry/segment.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Euclidean distance between a line segment and a point.
	/*!
	This is a convenience function which returns
	std::sqrt(distance2(segment, point)).
	*/
	template <typename Real, int N>
	Real distance(
		const Segment<Real, N>& segment,
		const Vector<Real, N>& point);

	//! Squared Euclidean distance between a line segment and a point.
	/*!
	Preconditions:
	segment.n() == point.n()

	Time complexity: O(segment.n())
	*/
	template <typename Real, int N>
	Real distance2(
		const Segment<Real, N>& segment,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/distance_segment_point.hpp"

#endif
