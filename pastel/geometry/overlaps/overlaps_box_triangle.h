// Description: Overlap tests between a box and a triangle

#ifndef PASTELGEOMETRY_OVERLAPS_BOX_TRIANGLE_H
#define PASTELGEOMETRY_OVERLAPS_BOX_TRIANGLE_H

#include "pastel/geometry/shapes/box.h"
#include "pastel/geometry/shapes/triangle.h"

namespace Pastel
{

	//! Tests if an box and a triangle overlap.
	template <typename Real, int N>
	bool overlaps(
		const Box<Real, N>& box,
		const PASTEL_TRIANGLE(Real, N)& triangle);

}

#include "pastel/geometry/overlaps/overlaps_box_triangle.hpp"

#endif
