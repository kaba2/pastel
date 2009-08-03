#ifndef PASTEL_CLOSEST_SEGMENT_POINT_HPP
#define PASTEL_CLOSEST_SEGMENT_POINT_HPP

#include "pastel/geometry/closest_segment_point.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Real closest(
		const Segment<N, Real>& segment,
		const Point<Real, N>& point)
	{
		const Vector<Real, N> delta(segment.end() - segment.start());

		Real t = dot(point - segment.start(), delta) /
			dot(delta, delta);

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
