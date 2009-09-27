#ifndef PASTEL_DISTANCE_LINE_LINE_HPP
#define PASTEL_DISTANCE_LINE_LINE_HPP

#include "pastel/geometry/distance_line_line.h"
#include "pastel/geometry/closest_line_line.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Real distance2(const Line<Real, N>& aLine,
		const Line<Real, N>& bLine)
	{
		const Tuple<Real, 2> uv(closest(aLine, bLine));

		const Vector<Real, N> aPoint(
			aLine.position() + aLine.direction() * uv[0]);
		const Vector<Real, N> bPoint(
			bLine.position() + bLine.direction() * uv[1]);

		const Vector<Real, N> delta(bPoint - aPoint);

		return dot(delta, delta);
	}

}

#endif
