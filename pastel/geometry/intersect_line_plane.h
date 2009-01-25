/*!
\file
\brief A function for finding the intersection between a line and a plane.
*/

#ifndef PASTELGEOMETRY_INTERSECT_LINE_PLANE_H
#define PASTELGEOMETRY_INTERSECT_LINE_PLANE_H

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
			const Line<N, Real>& line,
			const Plane<N, Real>& plane,
			Real& t);

}

#include "pastel/geometry/intersect_line_plane.hpp"

#endif
