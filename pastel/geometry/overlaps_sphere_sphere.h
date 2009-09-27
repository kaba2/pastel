// Description: Overlap tests between two spheres

#ifndef PASTEL_OVERLAPS_SPHERE_SPHERE_H
#define PASTEL_OVERLAPS_SPHERE_SPHERE_H

#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Tests if two solid spheres intersect.
	/*!
	The spheres are considered to be closed and solid.
	*/
	template <typename Real, int N>
	bool overlaps(
		const Sphere<Real, N>& aSphere,
		const Sphere<Real, N>& bSphere);

}

#include "pastel/geometry/overlaps_sphere_sphere.hpp"

#endif
