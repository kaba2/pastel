// Description: Distance between a plane and a sphere

#ifndef PASTELGEOMETRY_DISTANCE_PLANE_SPHERE_H
#define PASTELGEOMETRY_DISTANCE_PLANE_SPHERE_H

#include "pastel/geometry/shapes/plane.h"
#include "pastel/geometry/shapes/sphere.h"

namespace Pastel
{

	//! Computes the squared distance between a plane and a sphere.
	/*!
	This is a convenience function which returns
	std::sqrt(distance2(plane, sphere)).
	*/
	template <typename Real, int N>
	Real distance(
		const Plane<Real, N>& plane,
		const Sphere<Real, N>& sphere);

	//! Squared Euclidean distance between a plane and a sphere.
	/*!
	Preconditions:
	plane.n() == sphere.n()

	Time complexity:
	O(n), where n is the dimension.
	*/
	template <typename Real, int N>
	Real distance2(
		const Plane<Real, N>& plane,
		const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/distance/distance_plane_sphere.hpp"

#endif
