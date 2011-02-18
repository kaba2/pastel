#ifndef PASTEL_DISTANCE_PLANE_SPHERE_HPP
#define PASTEL_DISTANCE_PLANE_SPHERE_HPP

#include "pastel/geometry/distance_plane_sphere.h"
#include "pastel/geometry/distance_plane_point.h"

namespace Pastel
{

	template <typename Real, int N>
	Real distance2(
		const Plane<Real, N>& plane,
		const Sphere<Real, N>& sphere)
	{
		PENSURE_OP(plane.dimension(), ==, sphere.dimension());

		// Let
		// s = distance from sphere's center to the plane.
		// r = sphere's radius
		// Then the distance d is given by
		// d = s - r
		// Negative distance means overlapping,
		// so then zero distance is returned.
		// d < 0 <=> s - r < 0 <=> s < r <=> s^2 < r^2

		const Real centerDistance2 =
			distance2(plane, sphere.position());
		if (centerDistance2 <= square(sphere.radius()))
		{
			return 0;
		}

		return square(std::sqrt(centerDistance2) - sphere.radius());
	}

}

#endif
