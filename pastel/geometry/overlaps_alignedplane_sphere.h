// Description: Overlap tests between an aligned plane and a sphere

#ifndef PASTEL_OVERLAPS_ALIGNEDPLANE_SPHERE_H
#define PASTEL_OVERLAPS_ALIGNEDPLANE_SPHERE_H

#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Tests if an aligned plane and a sphere overlap.
	/*!
	Preconditions:
	plane.dimension() == sphere.dimension

	Time complexity:
	O(1)
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const Sphere<Real, N>& sphere);

	//! Tests if an aligned plane and a sphere overlap.
	/*!
	sphereOnPositiveSide: 
	Filled with information if the center of the sphere 
	is on the positive side of the plane.

	Returns:
	overlaps(plane, sphere)
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const Sphere<Real, N>& sphere,
		bool &sphereOnPositiveSide);

}

#include "pastel/geometry/overlaps_alignedplane_sphere.hpp"

#endif
