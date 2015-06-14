// Description: Closest points between a line and a point

#ifndef PASTELGEOMETRY_CLOSEST_LINE_POINT_H
#define PASTELGEOMETRY_CLOSEST_LINE_POINT_H

#include "pastel/geometry/shape/line.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes closest points between a line and a point.
	/*!
	Returns:
	A parameter t such that 'line.at(t)' and 'point' are closest points.
	*/
	template <typename Real, integer N>
	Real closest(
		const Line<Real, N>& line,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/closest/closest_line_point.hpp"

#endif
