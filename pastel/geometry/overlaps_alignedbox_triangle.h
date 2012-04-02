// Description: Overlap tests between an aligned box and a triangle

#ifndef PASTEL_OVERLAPS_ALIGNEDBOX_TRIANGLE_H
#define PASTEL_OVERLAPS_ALIGNEDBOX_TRIANGLE_H

#include "pastel/geometry/geometrylibrary.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/triangle.h"

namespace Pastel
{

	//! Tests if an aligned box and a triangle overlap.
	/*!
	Preconditions:
	box.dimension() == triangle.dimension()

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

#include "pastel/geometry/overlaps_alignedbox_triangle.hpp"

#endif
