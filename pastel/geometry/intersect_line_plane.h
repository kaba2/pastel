// Description: Intersection between a line and a plane

#ifndef PASTEL_INTERSECT_LINE_PLANE_H
#define PASTEL_INTERSECT_LINE_PLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/line.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	//! Finds the intersection between a line and a plane.
	/*!
	This algorithm works also with non-unit line direction and
	plane normal.
	*/

	template <int N, typename Real>
		bool intersect(
			const Line<Real, N>& line,
			const Plane<Real, N>& plane,
			Real& t);

}

#include "pastel/geometry/intersect_line_plane.hpp"

#endif
