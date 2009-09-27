#ifndef PASTEL_OVERLAPS_PLANE_SPHERE_HPP
#define PASTEL_OVERLAPS_PLANE_SPHERE_HPP

#include "pastel/geometry/overlaps_plane_sphere.h"

#include "pastel/geometry/distance_plane_sphere.h"
#include "pastel/geometry/intersect_line_plane.h"

namespace Pastel
{

	template <typename Real, int N>
		bool overlaps(
			const Plane<Real, N>& plane,
			const Sphere<Real, N>& sphere)
	{
		// A plane intersects a sphere if
		// the distance of the sphere's center
		// point from the plane is smaller
		// than the sphere's radius.

		return distance2(plane, sphere.position()) <
			sphere.radius() * sphere.radius();
	}

	template <typename Real, int N>
		bool overlaps(
			const Plane<Real, N>& plane,
			const Sphere<Real, N>& sphere,
			bool& sphereOnPositiveSide)
	{
		// A plane intersects a sphere if
		// the distance of the sphere's center
		// point from the plane is smaller
		// than the sphere's radius.

		Real signedSquaredDistance =
			dot(plane.normal(),
			sphere.position() - plane.position()) /
			dot(plane.normal(), plane.normal());

		if (signedSquaredDistance <= 0)
		{
			sphereOnPositiveSide = false;
			signedSquaredDistance = -signedSquaredDistance;
		}
		else
		{
			sphereOnPositiveSide = true;
		}

		return (signedSquaredDistance <
			sphere.radius() * sphere.radius());
	}

}

#endif
