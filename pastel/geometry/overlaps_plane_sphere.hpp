#ifndef PASTELGEOMETRY_OVERLAPS_PLANE_SPHERE_HPP
#define PASTELGEOMETRY_OVERLAPS_PLANE_SPHERE_HPP

#include "pastel/geometry/overlaps_plane_sphere.h"
#include "pastel/geometry/distance_plane_point.h"

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

		Real d2 = 
			distance2(plane, sphere.position());
		Real radius2 =
			square(sphere.radius());

		if (d2 >= radius2)
		{
			if (d2 > radius2 ||
				sphere.topology() == Topology::Open)
			{
				return false;
			}
		}
	
		return true;
	}

	template <typename Real, int N>
	bool overlaps(
		const Plane<Real, N>& plane,
		const Sphere<Real, N>& sphere,
		bool& sphereOnPositiveSide)
	{
		sphereOnPositiveSide = 
			dot(plane.normal(),	
			sphere.position() - plane.position()) > 0;

		return Pastel::overlaps(plane, sphere);
	}

}

#endif
