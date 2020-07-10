// Description: Overlap tests between a plane and a sphere

#ifndef PASTELGEOMETRY_OVERLAPS_PLANE_SPHERE_H
#define PASTELGEOMETRY_OVERLAPS_PLANE_SPHERE_H

#include "pastel/geometry/shape/plane.h"
#include "pastel/geometry/shape/sphere.h"
#include "pastel/geometry/distance/distance_plane_point.h"

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
		bool& sphereOnPositiveSide)
	{
		sphereOnPositiveSide = 
			dot(plane.normal(),	
			sphere.position() - plane.position()) > 0;

		return Pastel::overlaps(plane, sphere);
	}

}

#endif
