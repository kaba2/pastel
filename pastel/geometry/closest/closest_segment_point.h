// Description: Closest points between a line segment and a point

#ifndef PASTELGEOMETRY_CLOSEST_SEGMENT_POINT_H
#define PASTELGEOMETRY_CLOSEST_SEGMENT_POINT_H

#include "pastel/geometry/shape/segment.h"

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
		const Vector<Real, N>& point)
	{
		Real t = dot(point - segment.start(), 
			segment.end() - segment.start()) /
			dot(segment.end() - segment.start());

		if (t < 0)
		{
			t = 0;
		}
		else if (t > 1)
		{
			t = 1;
		}

		return t;
	}

}

#endif
