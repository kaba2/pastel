// Description: Overlap tests between a plane and a sphere

#ifndef PASTELGEOMETRY_OVERLAPS_PLANE_SPHERE_H
#define PASTELGEOMETRY_OVERLAPS_PLANE_SPHERE_H

#include "pastel/geometry/plane.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Tests if a plane and a sphere overlap.
	/*!
	Preconditions:
	plane.n() == sphere.n()

	Time complexity:
	O(n), where n is the dimension.

	Returns:
	Whether the plane and the sphere overlap.
	*/
	template <typename Real, int N>
	bool overlaps(
		const Plane<Real, N>& plane,
		const Sphere<Real, N>& sphere);

	//! Tests if a plane and a sphere overlap.
	/*!
	sphereOnPositiveSide: 
	Set true, if the center point of the box
	is on the positive side of the plane.
	Otherwise set to false.

	Returns:
	overlaps(plane, sphere)
	*/
	template <typename Real, int N>
	bool overlaps(
		const Plane<Real, N>& plane,
		const Sphere<Real, N>& sphere,
		bool& sphereOnPositiveSide);

}

#include "pastel/geometry/overlaps/overlaps_plane_sphere.hpp"

#endif
