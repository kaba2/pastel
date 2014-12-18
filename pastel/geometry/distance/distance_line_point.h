// Description: Distance between a line and a point

#ifndef PASTELGEOMETRY_DISTANCE_LINE_POINT_H
#define PASTELGEOMETRY_DISTANCE_LINE_POINT_H

#include "pastel/geometry/shapes/line.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Euclidean distance between a point and a line.
	/*!
	This is a convenience function which returns
	std::sqrt(distance2(line, point)).
	*/
	template <typename Real, int N>
	Real distance(
		const Line<Real, N>& line,
		const Vector<Real, N>& point);

	//! Squared Euclidean distance between a point and a line.
	template <typename Real, int N>
	Real distance2(
		const Line<Real, N>& line,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/distance/distance_line_point.hpp"

#endif
