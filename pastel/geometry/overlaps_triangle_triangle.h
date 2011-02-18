// Description: Overlap tests between two triangles

#ifndef PASTEL_OVERLAPS_TRIANGLE_TRIANGLE_H
#define PASTEL_OVERLAPS_TRIANGLE_TRIANGLE_H

#include "pastel/geometry/triangle.h"
#include "pastel/geometry/line.h"

namespace Pastel
{

	//! Tests if two triangles overlap.
	template <typename Real>
	bool overlaps(
		const Triangle<Real, 1>& aTriangle,
		const Triangle<Real, 1>& bTriangle);

	//! Tests if two triangles overlap.
	template <typename Real>
	bool overlaps(
		const Triangle<Real, 2>& aTriangle,
		const Triangle<Real, 2>& bTriangle);

	//! Tests if two triangles overlap.
	template <typename Real>
	bool overlaps(
		const Triangle<Real, 3>& aTriangle,
		const Triangle<Real, 3>& bTriangle);

}

#include "pastel/geometry/overlaps_triangle_triangle.hpp"

#endif
