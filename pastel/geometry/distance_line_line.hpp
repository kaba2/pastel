#ifndef PASTELGEOMETRY_DISTANCE_LINE_LINE_HPP
#define PASTELGEOMETRY_DISTANCE_LINE_LINE_HPP

#include "pastel/geometry/distance_line_line.h"
#include "pastel/geometry/closest_line_line.h"

#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(const Line<N, Real>& aLine,
		const Line<N, Real>& bLine)
	{
		const Tuple<2, Real> uv(closest(aLine, bLine));

		const Point<N, Real> aPoint(
			aLine.position() + aLine.direction() * uv[0]);
		const Point<N, Real> bPoint(
			bLine.position() + bLine.direction() * uv[1]);

		const Vector<N, Real> delta(bPoint - aPoint);

		return dot(delta, delta);
	}

}

#endif
