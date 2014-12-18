// Description: Overlap tests between an aligned box and a sphere

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_SPHERE_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"

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
	template <typename Real, int N>
	bool overlaps(
		const AlignedBox<Real, N>& alignedBox,
		const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/overlaps/overlaps_alignedbox_sphere.hpp"

#endif
