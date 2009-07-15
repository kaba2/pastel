// Description: Overlap tests between two triangles

#ifndef PASTEL_OVERLAPS_TRIANGLE_TRIANGLE_H
#define PASTEL_OVERLAPS_TRIANGLE_TRIANGLE_H

#include "pastel/geometry/triangle.h"
#include "pastel/geometry/line.h"

namespace Pastel
{

	//! Tests if two triangles overlap.

	/*!
	The triangles are considered to be closed
	and solid.
	*/

	template <typename Real>
	bool overlaps(
		const Triangle<1, Real>& aTriangle,
		const Triangle<1, Real>& bTriangle);

	//! Tests if two triangles overlap.

	/*!
	The triangles are considered to be closed
	and solid.
	*/

	template <typename Real>
	bool overlaps(
		const Triangle<2, Real>& aTriangle,
		const Triangle<2, Real>& bTriangle);

	//! Tests if two triangles overlap.

	/*!
	The triangles are considered to be closed
	and solid.
	*/

	template <typename Real>
	bool overlaps(
		const Triangle<3, Real>& aTriangle,
		const Triangle<3, Real>& bTriangle);

}

#include "pastel/geometry/overlaps_triangle_triangle.hpp"

#endif
