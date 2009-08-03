// Description: Overlap tests between an aligned plane an a triangle

#ifndef PASTEL_OVERLAPS_ALIGNEDPLANE_TRIANGLE_H
#define PASTEL_OVERLAPS_ALIGNEDPLANE_TRIANGLE_H

#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/triangle.h"

namespace Pastel
{

	//! Tests if an aligned plane and a triangle overlap.

	/*!
	The triangle is considered to be closed and solid.
	*/

	template <int N, typename Real>
		bool overlaps(const AlignedPlane<Real, N>& alignedPlane,
			const Triangle<Real, N>& triangle);

	//! Tests if an aligned plane and a triangle overlap.

	/*!
	The triangle is considered to be closed and solid.
	'triangleOnPositiveSide' is filled with the
	information if the first point of the triangle
	is on the positive side of the plane.
	*/

	template <int N, typename Real>
		bool overlaps(const AlignedPlane<Real, N>& alignedPlane,
			const Triangle<Real, N>& triangle,
			bool& triangleOnPositiveSide);

}

#include "pastel/geometry/overlaps_alignedplane_triangle.hpp"

#endif
