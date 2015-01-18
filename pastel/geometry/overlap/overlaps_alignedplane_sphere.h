// Description: Overlap tests between an aligned plane and a sphere

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_SPHERE_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_SPHERE_H

#include "pastel/geometry/shape/alignedplane.h"
#include "pastel/geometry/shape/sphere.h"

namespace Pastel
{

	//! Tests if an aligned plane and a sphere overlap.
	/*!
	Preconditions:
	plane.n() == sphere.dimension

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

#include "pastel/geometry/overlap/overlaps_alignedplane_sphere.hpp"

#endif
