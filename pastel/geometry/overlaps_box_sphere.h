// Description: Overlap tests between a box and a sphere

#ifndef PASTEL_OVERLAPBOXSPHERE_H
#define PASTEL_OVERLAPBOXSPHERE_H

#include "pastel/geometry/box.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Tests if an box and a sphere overlap.

	/*!
	The box and the sphere are considered
	to be closed and solid.
	*/

	template <int N, typename Real>
	bool overlaps(
		const Box<Real, N>& box,
		const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/overlaps_box_sphere.hpp"

#endif
