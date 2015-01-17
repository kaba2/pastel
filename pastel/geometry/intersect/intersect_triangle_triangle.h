// Description: Intersection between two triangles

#ifndef PASTELGEOMETRY_INTERSECT_TRIANGLE_TRIANGLE_H
#define PASTELGEOMETRY_INTERSECT_TRIANGLE_TRIANGLE_H

#include "pastel/geometry/shapes/triangle.h"
#include "pastel/geometry/shapes/segment.h"

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
		const PASTEL_TRIANGLE(Real, 3)& aTriangle,
		const PASTEL_TRIANGLE(Real, 3)& bTriangle,
		Line<Real, 3>& intersectionLine,
		AlignedBox<Real, 1>& intersectionRange);

}

#include "pastel/geometry/intersect/intersect_triangle_triangle.hpp"

#endif