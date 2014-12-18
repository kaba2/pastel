// Description: Overlap tests between a plane and a triangle

#ifndef PASTELGEOMETRY_OVERLAPS_PLANE_TRIANGLE_H
#define PASTELGEOMETRY_OVERLAPS_PLANE_TRIANGLE_H

#include "pastel/geometry/shapes/plane.h"
#include "pastel/geometry/shapes/triangle.h"

namespace Pastel
{

	//! Tests if a plane and a triangle overlap.

	/*!
	The triangle is considered to be closed and solid.
	*/

	template <typename Real, int N>
		bool overlaps(
			const Plane<Real, N>& plane,
			const PASTEL_TRIANGLE(Real, N)& triangle);

}

#include "pastel/geometry/overlaps/overlaps_plane_triangle.hpp"

#endif
