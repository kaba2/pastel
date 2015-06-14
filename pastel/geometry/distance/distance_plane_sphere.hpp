#ifndef PASTELGEOMETRY_DISTANCE_PLANE_SPHERE_HPP
#define PASTELGEOMETRY_DISTANCE_PLANE_SPHERE_HPP

#include "pastel/geometry/distance/distance_plane_sphere.h"
#include "pastel/geometry/distance/distance_plane_point.h"

namespace Pastel
{

	template <typename Real, integer N>
	Real distance(
		const Plane<Real, N>& plane,
		const Sphere<Real, N>& sphere)
	{
		return std::sqrt(distance2(plane, sphere));
	}

	template <typename Real, integer N>
	Real distance2(
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
		// d < 0 <=> s - r < 0 <=> s < r <=> s^2 < r^2

		Real centerDistance2 =
			distance2(plane, sphere.position());
		if (centerDistance2 <= square(sphere.radius()))
		{
			return 0;
		}

		return square(std::sqrt(centerDistance2) - sphere.radius());
	}

}

#endif
