// Description: Closest points between a line segment and a point

#ifndef PASTEL_CLOSEST_SEGMENT_POINT_H
#define PASTEL_CLOSEST_SEGMENT_POINT_H

#include "pastel/geometry/segment.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	//! Computes closest points between a line segment and a point.
	/*!
	Returns:
	A parameter t such that 'segment.at(t)' and 'point' are closest points;
	*/

	template <int N, typename Real>
	Real closest(
		const Segment<N, Real>& segment,
		const Point<Real, N>& point);

}

#include "pastel/geometry/closest_segment_point.hpp"

#endif
