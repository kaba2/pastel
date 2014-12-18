// Description: Intersection between a line and a plane

#ifndef PASTELGEOMETRY_INTERSECT_LINE_PLANE_H
#define PASTELGEOMETRY_INTERSECT_LINE_PLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/shapes/line.h"
#include "pastel/geometry/shapes/plane.h"

namespace Pastel
{

	//! Finds the intersection between a line and a plane.
	/*!
	This algorithm works also with non-unit line direction and
	plane normal.
	*/

	template <typename Real, int N>
		bool intersect(
			const Line<Real, N>& line,
			const Plane<Real, N>& plane,
			Real& t);

}

#include "pastel/geometry/intersect/intersect_line_plane.hpp"

#endif
