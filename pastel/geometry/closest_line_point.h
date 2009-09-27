// Description: Closest points between a line and a point

#ifndef PASTEL_CLOSEST_LINE_POINT_H
#define PASTEL_CLOSEST_LINE_POINT_H

#include "pastel/geometry/line.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes closest points between a line and a point.
	/*!
	Returns:
	A parameter t such that 'line.at(t)' and 'point' are closest points.
	*/
	template <typename Real, int N>
	Real closest(
		const Line<Real, N>& line,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/closest_line_point.hpp"

#endif
