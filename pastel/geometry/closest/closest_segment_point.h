// Description: Closest points between a line segment and a point

#ifndef PASTELGEOMETRY_CLOSEST_SEGMENT_POINT_H
#define PASTELGEOMETRY_CLOSEST_SEGMENT_POINT_H

#include "pastel/geometry/shapes/segment.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes closest points between a line segment and a point.
	/*!
	Returns:
	A parameter t such that 'segment.at(t)' and 'point' are 
	closest points.
	*/
	template <typename Real, int N>
	Real closest(
		const Segment<Real, N>& segment,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/closest/closest_segment_point.hpp"

#endif
