// Description: Overlap tests between an aligned plane an a triangle

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_TRIANGLE_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_TRIANGLE_H

#include "pastel/geometry/shape/alignedplane.h"
#include "pastel/geometry/shape/triangle.h"

namespace Pastel
{

	//! Tests if an aligned plane and a triangle overlap.
	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& alignedPlane,
		const PASTEL_TRIANGLE(Real, N)& triangle);

	//! Tests if an aligned plane and a triangle overlap.
	/*!
	'triangleOnPositiveSide' is filled with the
	information if the first point of the triangle
	is on the positive side of the plane.
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& alignedPlane,
		const PASTEL_TRIANGLE(Real, N)& triangle,
		bool& triangleOnPositiveSide);

}

#include "pastel/geometry/overlap/overlaps_alignedplane_triangle.hpp"

#endif
