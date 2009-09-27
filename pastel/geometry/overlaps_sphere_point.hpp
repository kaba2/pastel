#ifndef PASTEL_OVERLAPS_SPHERE_POINT_HPP
#define PASTEL_OVERLAPS_SPHERE_POINT_HPP

#include "pastel/geometry/overlaps_sphere_point.h"
#include "pastel/geometry/sphere.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real, int N>
		bool overlaps(const Sphere<Real, N>& sphere,
			const Vector<Real, N>& point)
	{
		// A point intersects a sphere if
		// the distance of the point from
		// the sphere center is less or equal than
		// the sphere's radius.

		const Vector<Real, N> delta(point - sphere.position());

		return dot(delta) <=
			sphere.radius() * sphere.radius();
	}

}

#endif
