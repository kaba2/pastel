#ifndef PASTEL_DISTANCE_ALIGNEDBOX_SPHERE_HPP
#define PASTEL_DISTANCE_ALIGNEDBOX_SPHERE_HPP

#include "distance_alignedbox_sphere.h"

#include "distance_alignedbox_point.h"

#include <algorithm>

namespace Pastel
{

	template <typename Real, int N>
	Real distance(
		const AlignedBox<Real, N>& box,
		const Sphere<Real, N>& sphere)
	{
		PENSURE_OP(box.dimension(), ==, sphere.dimension());

		// Let
		// s = distance from sphere's center to 'box'.
		// r = sphere's radius
		// Then the distance d is given by
		// d = s - r
		// Negative distance means overlapping,
		// so then zero distance is returned.
		// d < 0 <=> s - r < 0 <=> s < r <=> s^2 < r^2

		const Real centerDistance2 =
			distance2(box, sphere.position());

		if (centerDistance2 <= square(sphere.radius()))
		{
			return 0;
		}

		return std::sqrt(centerDistance2) - sphere.radius();
	}

	template <typename Real, int N>
	Real distance2(
		const AlignedBox<Real, N>& box,
		const Sphere<Real, N>& sphere)
	{
		PENSURE_OP(box.dimension(), ==, sphere.dimension());

		return square(Pastel::distance(box, sphere));
	}

}

#endif
