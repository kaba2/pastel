#ifndef PASTEL_INTERSECT_PLANE_SPHERE_HPP
#define PASTEL_INTERSECT_PLANE_SPHERE_HPP

#include "pastel/geometry/intersect_plane_sphere.h"

#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"
#include "pastel/geometry/line.h"

#include "pastel/geometry/intersect_line_plane.h"
#include "pastel/geometry/overlaps_plane_sphere.h"

namespace Pastel
{

	template <int N, typename Real>
		bool intersect(
			const Plane<N, Real>& plane,
			const Sphere<N, Real>& sphere,
			Sphere<N, Real>& result)
	{
		const Line<N, Real> line(
			sphere.position(),
			plane.normal());

		Real t(0);

		bool intersected = Pastel::intersect(line, plane, t));
		// There is no way to miss the plane.
		ASSERT(intersected);

		const Real radius2 = sphere.radius() * sphere.radius();

		// Let
		// P the center of the sphere
		// N be the (unit) normal of the plane

		// ||(P + t * N) - P||^2 == ||t * N||^2 = t^2
		const Real nearestDistance2 = t * t;

		if (radius2 < nearestDistance2)
		{
			// The sphere does not intersect the plane.
			return false;
		}

		result.setPosition(line.at(t));
		result.setRadius(
			std::sqrt(radius2 - nearestDistance2));

		return true;
	}

}

#endif
