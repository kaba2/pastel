// Description: Distance between a line segment and a point

#ifndef PASTELGEOMETRY_DISTANCE_SEGMENT_POINT_H
#define PASTELGEOMETRY_DISTANCE_SEGMENT_POINT_H

#include "pastel/geometry/segment.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes the squared distance between a line segment and a point.
	/*!
	Preconditions:
	segment.dimension() == point.dimension()

	Time complexity:
	O(n), where n is the dimension.
	*/
	template <typename Real, int N>
	Real distance2(
		const Segment<Real, N>& segment,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/distance_segment_point.hpp"

#endif
