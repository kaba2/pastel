// Description: Overlap tests between a box and a sphere

#ifndef PASTELGEOMETRY_OVERLAPBOXSPHERE_H
#define PASTELGEOMETRY_OVERLAPBOXSPHERE_H

#include "pastel/geometry/shape/box.h"
#include "pastel/geometry/shape/sphere.h"

namespace Pastel
{

	//! Tests if an box and a sphere overlap.
	template <typename Real, integer N>
	bool overlaps(
		const Box<Real, N>& box,
		const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/overlap/overlaps_box_sphere.hpp"

#endif
