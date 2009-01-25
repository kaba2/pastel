/*!
\file
\brief A function for computing the distance between a line and a point.
*/

#ifndef PASTELGEOMETRY_DISTANCE_LINE_POINT_H
#define PASTELGEOMETRY_DISTANCE_LINE_POINT_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/line.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	//! Computes the squared distance between a point to a line.

	template <int N, typename Real>
		Real distance2(
			const Line<N, Real>& line,
			const Point<N, Real>& point);

}

#include "pastel/geometry/distance_line_point.hpp"

#endif
