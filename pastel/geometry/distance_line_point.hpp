#ifndef PASTELGEOMETRY_DISTANCE_LINE_POINT_HPP
#define PASTELGEOMETRY_DISTANCE_LINE_POINT_HPP

#include "pastel/geometry/distance_line_point.h"
#include "pastel/geometry/closest_line_point.h"

#include "pastel/geometry/line.h"
#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N, typename Real>
		Real distance2(
			const Line<N, Real>& line,
			const Point<N, Real>& point)
	{
		const Real t(closest(line, point));

		const Vector<N, Real> delta(line.at(t) - point);

		return dot(delta, delta);
	}

}

#endif
