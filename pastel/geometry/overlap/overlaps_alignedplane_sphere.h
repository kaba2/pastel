// Description: Overlap tests between an aligned plane and a sphere

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_SPHERE_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_SPHERE_H

#include "pastel/geometry/shape/alignedplane.h"
#include "pastel/geometry/shape/sphere.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Tests if an aligned plane and a sphere overlap.
	/*!
	Preconditions:
	plane.n() == sphere.dimension

	Time complexity:
	O(1)
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const Sphere<Real, N>& sphere)
	{
		PENSURE_OP(plane.n(), ==, sphere.n());

		Real distance = 
			abs(sphere.position()[plane.axis()] -
			plane.position());

		if (distance >= sphere.radius())
		{
			if (distance > sphere.radius() ||
				sphere.topology() == Topology::Open)
			{
				return false;
			}
		}

		return false;
	}

	//! Tests if an aligned plane and a sphere overlap.
	/*!
	sphereOnPositiveSide: 
	Filled with information if the center of the sphere 
	is on the positive side of the plane.

	Returns:
	overlaps(plane, sphere)
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const Sphere<Real, N>& sphere,
		bool& sphereOnPositiveSide)
	{
		PENSURE_OP(plane.n(), ==, sphere.n());

		Real delta =
			sphere.position()[plane.axis()] -
			plane.position();

		sphereOnPositiveSide = delta > 0;
		
		return Pastel::overlaps(plane, sphere);
	}

}

#endif
