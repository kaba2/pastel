#ifndef PASTEL_CLOSEST_LINE_POINT_H
#define PASTEL_CLOSEST_LINE_POINT_H

#include "pastel/geometry/line.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	//! Returns the parameter of the closest point on the line w.r.t a point.

	template <int N, typename Real>
	Real closest(
		const Line<N, Real>& line,
		const Point<N, Real>& point);

}

#include "pastel/geometry/closest_line_point.hpp"

#endif
