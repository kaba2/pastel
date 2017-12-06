// Description: Distance between two lines

#ifndef PASTELGEOMETRY_DISTANCE_LINE_LINE_H
#define PASTELGEOMETRY_DISTANCE_LINE_LINE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/geometry/shape/line.h"
#include "pastel/geometry/closest/closest_line_line.h"
#include "pastel/math/norm/euclidean_norm.h"

namespace Pastel
{

	//! Euclidean distance between two lines.
	template <typename Real, integer N>
	auto distance2(
		const Line<Real, N>& aLine,
		const Line<Real, N>& bLine)
	{
		auto norm = Euclidean_Norm<Real>();
		const Tuple<Real, 2> uv = closest(aLine, bLine);
		return norm[dot(aLine.at(uv[0]) - bLine.at(uv[1]))];
	}

}

#endif
