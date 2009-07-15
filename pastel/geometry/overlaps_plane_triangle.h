// Description: Overlap tests between a plane and a triangle

#ifndef PASTEL_OVERLAPS_PLANE_TRIANGLE_H
#define PASTEL_OVERLAPS_PLANE_TRIANGLE_H

#include "pastel/geometry/plane.h"
#include "pastel/geometry/triangle.h"

namespace Pastel
{

	//! Tests if a plane and a triangle overlap.

	/*!
	The triangle is considered to be closed and solid.
	*/

	template <int N, typename Real>
		bool overlaps(
			const Plane<N, Real>& plane,
			const Triangle<N, Real>& triangle);

}

#include "pastel/geometry/overlaps_plane_triangle.hpp"

#endif
