// Description: Distance between a plane and a sphere

#ifndef PASTELGEOMETRY_DISTANCE_PLANE_SPHERE_H
#define PASTELGEOMETRY_DISTANCE_PLANE_SPHERE_H

#include "pastel/geometry/shape/plane.h"
#include "pastel/geometry/shape/sphere.h"
#include "pastel/geometry/distance/distance_plane_point.h"

namespace Pastel
{

	//! Euclidean distance between a plane and a sphere.
	/*!
	Preconditions:
	plane.n() == sphere.n()

	Time complexity:
	O(n), where n is the dimension.
	*/
	template <typename Real, int N>
	auto distance2(
		const Plane<Real, N>& plane,
		const Sphere<Real, N>& sphere)
	{
		PENSURE_OP(plane.n(), ==, sphere.n());

		// Let
		// s = distance from sphere's center to the plane.
		// r = sphere's radius
		// Then the distance d is given by
		// d = s - r
		// Negative distance means overlapping,
		// so then zero distance is returned.

		auto norm = Euclidean_Norm<Real>();
		auto centerDistance = distance2(plane, sphere.position());
		if (centerDistance <= sphere.radius()) return norm();
		return norm((Real)centerDistance - sphere.radius());
	}

}

#endif
