// Description: Distance between a line and a point

#ifndef PASTELGEOMETRY_DISTANCE_LINE_POINT_H
#define PASTELGEOMETRY_DISTANCE_LINE_POINT_H

#include "pastel/geometry/shape/line.h"
#include "pastel/sys/vector.h"
#include "pastel/geometry/closest/closest_line_point.h"
#include "pastel/math/norm/euclidean_norm.h"

namespace Pastel
{

	//! Euclidean distance between a point and a line.
	template <typename Real, int N>
	auto distance2(
		const Line<Real, N>& line,
		const Vector<Real, N>& point)
	{
		auto norm = Euclidean_Norm<Real>();
		const Real t = closest(line, point);
		return norm[dot(line.at(t) - point)];
	}

}

#endif
