// Description: The intersection between two triangles

#ifndef PASTEL_INTERSECT_TRIANGLE_TRIANGLE_H
#define PASTEL_INTERSECT_TRIANGLE_TRIANGLE_H

#include "pastel/geometry/triangle.h"
#include "pastel/geometry/segment.h"

namespace Pastel
{

	//! Test if and where two 3d triangles intersect.

	/*!
	If the planes of the triangles coincide,
	the possible intersection is a convex polygon.
	This case is not considered intersecting.
	*/

	template <typename Real>
	bool intersect(
		const Triangle<3, Real>& aTriangle,
		const Triangle<3, Real>& bTriangle,
		Line<3, Real>& intersectionLine,
		AlignedBox<1, Real>& intersectionRange);

}

#include "pastel/geometry/intersect_triangle_triangle.hpp"

#endif
