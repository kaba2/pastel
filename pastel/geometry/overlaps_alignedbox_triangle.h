/*!
\file
\brief A function for testing overlap between an alignedBox and a triangle.
*/

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_TRIANGLE_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_TRIANGLE_H

#include "pastel/geometry/geometrylibrary.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/triangle.h"

namespace Pastel
{

	//! Tests if an alignedBox and a triangle overlap.

	/*!
	The alignedBox and the triangle is considered closed
	and solid.
	*/

	template <typename Real>
	bool overlaps(
		const AlignedBox<2, Real>& alignedBox,
		const Triangle<3, Real>& triangle);

	//! Tests if an alignedBox and a triangle overlap.

	/*!
	The alignedBox and the triangle is considered closed
	and solid.
	*/

    template <typename Real>
	bool overlaps(
		const AlignedBox<2, Real>& alignedBox,
		const Triangle<3, Real>& triangle);

}

#include "pastel/geometry/overlaps_alignedbox_triangle.hpp"

#endif
