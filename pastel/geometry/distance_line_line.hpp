#ifndef PASTEL_DISTANCE_LINE_LINE_HPP
#define PASTEL_DISTANCE_LINE_LINE_HPP

#include "pastel/geometry/distance_line_line.h"
#include "pastel/geometry/closest_line_line.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Real distance2(
		const Line<Real, N>& aLine,
		const Line<Real, N>& bLine)
	{
		const Tuple<Real, 2> uv = closest(aLine, bLine);

		return dot(aLine.at(uv[0]) - bLine.at(uv[1]));
	}

}

#endif
