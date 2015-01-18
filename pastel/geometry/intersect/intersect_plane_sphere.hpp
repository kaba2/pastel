#ifndef PASTELGEOMETRY_INTERSECT_PLANE_SPHERE_HPP
#define PASTELGEOMETRY_INTERSECT_PLANE_SPHERE_HPP

#include "pastel/geometry/intersect/intersect_plane_sphere.h"
#include "pastel/geometry/intersect/intersect_line_plane.h"
#include "pastel/geometry/overlap/overlaps_plane_sphere.h"
#include "pastel/geometry/shape/line.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real, int N>
		bool intersect(
			const Plane<Real, N>& plane,
			const Sphere<Real, N>& sphere,
			Sphere<Real, N>& result)
	{
		Line<Real, N> line(
			sphere.position(),
			plane.normal());

		Real t(0);

		bool intersected = Pastel::intersect(line, plane, t);
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
