// Description: Overlap tests between an aligned box and a sphere

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_SPHERE_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/sphere.h"

namespace Pastel
{

	//! Tests if an aligned box and a sphere overlap.
	/*!
	Preconditions:
	alignedBox.n() == sphere.n()

	Time complexity:
	O(n), where n is the dimension.

	Returns:
	Whether the box and the sphere overlap.

	Note: Currently the box is assumed closed.
	*/
	template <typename Real, integer N>
	bool overlaps(
		const AlignedBox<Real, N>& alignedBox,
		const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/overlap/overlaps_alignedbox_sphere.hpp"

#endif
