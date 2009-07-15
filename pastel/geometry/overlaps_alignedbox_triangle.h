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
	The aligned box and the triangle is considered closed
	and solid.
	*/

	template <typename Real>
	bool overlaps(
		const AlignedBox<2, Real>& alignedBox,
		const Triangle<3, Real>& triangle);

	//! Tests if an aligned box and a triangle overlap.

	/*!
	The aligned box and the triangle is considered closed
	and solid.
	*/

	template <typename Real>
	bool overlaps(
		const AlignedBox<2, Real>& alignedBox,
		const Triangle<3, Real>& triangle);

}

#include "pastel/geometry/overlaps_alignedbox_triangle.hpp"

#endif
