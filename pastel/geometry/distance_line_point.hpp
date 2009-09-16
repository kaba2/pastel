#ifndef PASTEL_DISTANCE_LINE_POINT_HPP
#define PASTEL_DISTANCE_LINE_POINT_HPP

#include "pastel/geometry/distance_line_point.h"
#include "pastel/geometry/closest_line_point.h"
#include "pastel/geometry/line.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N, typename Real>
		Real distance2(
			const Line<Real, N>& line,
			const Vector<Real, N>& point)
	{
		const Real t(closest(line, point));

		const Vector<Real, N> delta(line.at(t) - point);

		return dot(delta, delta);
	}

}

#endif
