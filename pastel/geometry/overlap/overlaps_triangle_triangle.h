// Description: Overlap tests between two triangles

#ifndef PASTELGEOMETRY_OVERLAPS_TRIANGLE_TRIANGLE_H
#define PASTELGEOMETRY_OVERLAPS_TRIANGLE_TRIANGLE_H

#include "pastel/geometry/shape/triangle.h"
#include "pastel/geometry/shape/line.h"

namespace Pastel
{

	//! Tests if two triangles overlap.
	template <typename Real>
	bool overlaps(
		const PASTEL_TRIANGLE(Real, 1)& aTriangle,
		const PASTEL_TRIANGLE(Real, 1)& bTriangle);

	//! Tests if two triangles overlap.
	template <typename Real>
	bool overlaps(
		const PASTEL_TRIANGLE(Real, 2)& aTriangle,
		const PASTEL_TRIANGLE(Real, 2)& bTriangle);

	//! Tests if two triangles overlap.
	template <typename Real>
	bool overlaps(
		const PASTEL_TRIANGLE(Real, 3)& aTriangle,
		const PASTEL_TRIANGLE(Real, 3)& bTriangle);

}

#include "pastel/geometry/overlap/overlaps_triangle_triangle.hpp"

#endif
