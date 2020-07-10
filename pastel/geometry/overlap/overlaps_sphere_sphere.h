// Description: Overlap tests between two spheres

#ifndef PASTELGEOMETRY_OVERLAPS_SPHERE_SPHERE_H
#define PASTELGEOMETRY_OVERLAPS_SPHERE_SPHERE_H

#include "pastel/geometry/shape/sphere.h"

namespace Pastel
{

	//! Tests if two solid spheres intersect.
	/*!
	Preconditions:
	aSphere.n() == bSphere.n()

	Time complexity:
	O(n), where n is the dimension.

	Returns:
	Whether the spheres overlap.
	*/
	template <typename Real, int N>
	bool overlaps(
		const Sphere<Real, N>& aSphere,
		const Sphere<Real, N>& bSphere)
	{
		PENSURE_OP(aSphere.n(), ==, bSphere.n());

		// Two spheres intersect if the distance
		// between their center points is
		// less than or equal to the sum of their radii.

		Real centerDistance2 =
			dot(bSphere.position() - aSphere.position());
		Real radiusSum2 =
			square(aSphere.radius() + bSphere.radius());

		if (centerDistance2 >= radiusSum2)
		{
			if (centerDistance2 > radiusSum2 ||
				aSphere.topology() == Topology::Open ||
				bSphere.topology() == Topology::Open)
			{
				return false;
			}
		}

		return true;
	}

}

#endif
