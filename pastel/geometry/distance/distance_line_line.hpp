#ifndef PASTELGEOMETRY_DISTANCE_LINE_LINE_HPP
#define PASTELGEOMETRY_DISTANCE_LINE_LINE_HPP

#include "pastel/geometry/distance/distance_line_line.h"
#include "pastel/geometry/closest/closest_line_line.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real, int N>
	Real distance(
		const Line<Real, N>& aLine,
		const Line<Real, N>& bLine)
	{
		return std::sqrt(distance2(aLine, bLine));
	}

	template <typename Real, int N>
	Real distance2(
		const Line<Real, N>& aLine,
		const Line<Real, N>& bLine)
	{
		Tuple<Real, 2> uv = closest(aLine, bLine);

		return dot(aLine.at(uv[0]) - bLine.at(uv[1]));
	}

}

#endif
