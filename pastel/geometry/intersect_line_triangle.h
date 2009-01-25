/*!
\file
\brief A function for finding the intersection between a line and a triangle.
*/

#ifndef PASTELGEOMETRY_INTERSECT_LINE_TRIANGLE_H
#define PASTELGEOMETRY_INTERSECT_LINE_TRIANGLE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/line.h"
#include "pastel/geometry/triangle.h"

namespace Pastel
{

	//! Finds the intersection between a line and a triangle.

	template <typename Real>
	bool intersect(const Line<2, Real>& line,
		const Triangle<2, Real>& triangle,
		Real& tIntersection);

	//! Finds the intersection between a line and a triangle.
	/*!
	'tIntersection' is the line's parametric distance to
	the intersection point.
	'uIntersection' and 'vIntersection' form the barycentric
	coordinates of the intersection point on the triangle.
	*/

	template <typename Real>
	bool intersect(
		const Line<3, Real>& line,
		const Triangle<3, Real>& triangle,
		Real& tIntersection,
		Real& uIntersection,
		Real& vIntersection);

}

#include "pastel/geometry/intersect_line_triangle.hpp"

#endif
