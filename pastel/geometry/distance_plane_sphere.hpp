#ifndef PASTELGEOMETRY_DISTANCE_PLANE_SPHERE_HPP
#define PASTELGEOMETRY_DISTANCE_PLANE_SPHERE_HPP

#include "pastel/geometry/distance_plane_sphere.h"

#include "pastel/geometry/distance_plane_point.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(
		const Plane<N, Real>& plane,
		const Sphere<N, Real>& sphere)
	{
		// Let
		// s = distance from sphere's center to the plane.
		// r = sphere's radius
		// Then the distance d is given by
		// d = s - r
		// Negative distance means overlapping,
		// so then zero distance is returned.
		// d < 0 <=> s - r < 0 <=> s < r <=> s^2 < r^2

		// Otherwise
		// d^2 = (s - r)^2 = s^2 + r^2 - 2 * s * r
		// = s^2 + r^2 - 2 * sqrt(s^2) * r

		const Real radius(sphere.radius());
		const Real radius2(radius * radius);
		const Real centerDistance2(
			distance2(plane, sphere.position()));

		if (centerDistance2 < radius2)
		{
			return 0;
		}

		return (centerDistance2 + radius2) -
			2 * std::sqrt(centerDistance2) * radius;
	}

}

#endif
