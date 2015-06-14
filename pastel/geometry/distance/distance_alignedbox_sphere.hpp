#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_SPHERE_HPP
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_SPHERE_HPP

#include "pastel/geometry/distance/distance_alignedbox_sphere.h"
#include "pastel/geometry/distance/distance_alignedbox_point.h"

namespace Pastel
{

	template <typename Real, integer N>
	Real distance(
		const AlignedBox<Real, N>& box,
		const Sphere<Real, N>& sphere)
	{
		PENSURE_OP(box.n(), ==, sphere.n());

		// Let
		// s = distance from sphere's center to 'box'.
		// r = sphere's radius
		// Then the distance d is given by
		// d = s - r
		// Negative distance means overlapping,
		// so then zero distance is returned.
		// d < 0 <=> s - r < 0 <=> s < r <=> s^2 < r^2

		Real centerDistance2 =
			distance2(box, sphere.position());

		if (centerDistance2 <= square(sphere.radius()))
		{
			return 0;
		}

		return std::sqrt(centerDistance2) - sphere.radius();
	}

	template <typename Real, integer N>
	Real distance2(
		const AlignedBox<Real, N>& box,
		const Sphere<Real, N>& sphere)
	{
		PENSURE_OP(box.n(), ==, sphere.n());

		return square(Pastel::distance(box, sphere));
	}

}

#endif
