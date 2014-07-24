#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_SPHERE_HPP
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_SPHERE_HPP

#include "pastel/geometry/overlaps_alignedplane_sphere.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const Sphere<Real, N>& sphere)
	{
		PENSURE_OP(plane.n(), ==, sphere.n());

		Real distance = 
			mabs(sphere.position()[plane.axis()] -
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
