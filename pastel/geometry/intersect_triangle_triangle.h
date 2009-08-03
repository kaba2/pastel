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
		const Triangle<Real, 3>& aTriangle,
		const Triangle<Real, 3>& bTriangle,
		Line<Real, 3>& intersectionLine,
		AlignedBox<Real, 1>& intersectionRange);

}

#include "pastel/geometry/intersect_triangle_triangle.hpp"

#endif
