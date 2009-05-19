#ifndef PASTEL_OVERLAPS_SPHERE_POINT_HPP
#define PASTEL_OVERLAPS_SPHERE_POINT_HPP

#include "pastel/geometry/overlaps_sphere_point.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/sphere.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	template <int N, typename Real>
		bool overlaps(const Sphere<N, Real>& sphere,
			const Point<N, Real>& point)
	{
		// A point intersects a sphere if
		// the distance of the point from
		// the sphere center is less or equal than
		// the sphere's radius.

		const Vector<N, Real> delta(point - sphere.position());

		return dot(delta) <=
			sphere.radius() * sphere.radius();
	}

}

#endif
