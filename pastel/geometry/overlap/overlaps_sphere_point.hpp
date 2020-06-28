#ifndef PASTELGEOMETRY_OVERLAPS_SPHERE_POINT_HPP
#define PASTELGEOMETRY_OVERLAPS_SPHERE_POINT_HPP

#include "pastel/geometry/overlap/overlaps_sphere_point.h"
#include "pastel/geometry/shape/sphere.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, int N>
	bool overlaps(
		const Sphere<Real, N>& sphere,
		const Vector<Real, N>& point)
	{
		PENSURE_OP(sphere.n(), ==, point.n());

		// A point intersects a sphere if
		// the distance of the point from
		// the sphere's center is less or 
		// equal than the sphere's radius.

		Real d2 = 
			dot(point - sphere.position());
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

}

#endif
