// Description: Overlap tests between two spheres

#ifndef PASTEL_OVERLAPS_SPHERE_SPHERE_H
#define PASTEL_OVERLAPS_SPHERE_SPHERE_H

#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Tests if two solid spheres intersect.
	/*!
	Preconditions:
	aSphere.dimension() == bSphere.dimension()

	Time complexity:
	O(n), where n is the dimension.

	Returns:
	Whether the spheres overlap.
	*/
	template <typename Real, int N>
	bool overlaps(
		const Sphere<Real, N>& aSphere,
		const Sphere<Real, N>& bSphere);

}

#include "pastel/geometry/overlaps_sphere_sphere.hpp"

#endif
