// Description: Overlap tests between an aligned box and a triangle

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_TRIANGLE_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_TRIANGLE_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/triangle.h"

namespace Pastel
{

	//! Tests if an aligned box and a triangle overlap.
	/*!
	Preconditions:
	box.n() == triangle.n()

	Returns:
	Whether the box and the triangle intersect.

	Note: Only dimensions 2 and 3 are supported.
	Note: Currently the box is assumed to be closed.
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedBox<Real, N>& box,
		const PASTEL_TRIANGLE(Real, N)& triangle);

}

#include "pastel/geometry/overlap/overlaps_alignedbox_triangle.hpp"

#endif
