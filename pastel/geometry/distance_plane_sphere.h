// Description: Distance between a plane and a sphere

#ifndef PASTEL_DISTANCE_PLANE_SPHERE_H
#define PASTEL_DISTANCE_PLANE_SPHERE_H

#include "pastel/geometry/plane.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Computes the squared distance between a plane and a sphere.
	/*!
	Preconditions:
	plane.dimension() == sphere.dimension()

	Time complexity:
	O(n), where n is the dimension.

	Returns:
	The squared distance between the plane and the sphere.
	*/
	template <typename Real, int N>
	Real distance2(
		const Plane<Real, N>& plane,
		const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/distance_plane_sphere.hpp"

#endif
