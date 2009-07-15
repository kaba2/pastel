// Description: Closest points between a line and a point

#ifndef PASTEL_CLOSEST_LINE_POINT_H
#define PASTEL_CLOSEST_LINE_POINT_H

#include "pastel/geometry/line.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	//! Computes closest points between a line and a point.
	/*!
	Returns:
	A parameter t such that 'line.at(t)' and 'point' are closest points.
	*/
	template <int N, typename Real>
	Real closest(
		const Line<N, Real>& line,
		const Point<N, Real>& point);

}

#include "pastel/geometry/closest_line_point.hpp"

#endif
