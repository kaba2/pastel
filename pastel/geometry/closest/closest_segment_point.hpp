#ifndef PASTELGEOMETRY_CLOSEST_SEGMENT_POINT_HPP
#define PASTELGEOMETRY_CLOSEST_SEGMENT_POINT_HPP

#include "pastel/geometry/closest/closest_segment_point.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

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
