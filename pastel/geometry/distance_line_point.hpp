#ifndef PASTELGEOMETRY_DISTANCE_LINE_POINT_HPP
#define PASTELGEOMETRY_DISTANCE_LINE_POINT_HPP

#include "pastel/geometry/distance_line_point.h"
#include "pastel/geometry/closest_line_point.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Real distance2(
		const Line<Real, N>& line,
		const Vector<Real, N>& point)
	{
		const Real t = closest(line, point);

		return dot(line.at(t) - point);
	}

}

#endif
