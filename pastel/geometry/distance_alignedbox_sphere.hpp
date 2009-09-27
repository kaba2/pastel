#ifndef PASTEL_DISTANCE_ALIGNEDBOX_SPHERE_HPP
#define PASTEL_DISTANCE_ALIGNEDBOX_SPHERE_HPP

#include "distance_alignedbox_sphere.h"

#include "distance_alignedbox_point.h"

#include <algorithm>

namespace Pastel
{

	template <typename Real, int N>
	Real distance2(
		const AlignedBox<Real, N>& alignedBox,
		const Sphere<Real, N>& sphere)
	{
		// Let
		// s = distance from sphere's center to 'alignedBox'.
		// r = sphere's radius
		// Then the distance d is given by
		// d = s - r
		// Negative distance means overlapping,
		// so then zero distance is returned.
		// d < 0 <=> s - r < 0 <=> s < r <=> s^2 < r^2

		// Otherwise
		// d^2 = (s - r)^2 = s^2 + r^2 - 2 * s * r
		// = s^2 + r^2 - 2 * sqrt(s^2) * r

		const Real radius(sphere.radius());
		const Real radius2(radius * radius);
		const Real centerDistance2(distance2(alignedBox, sphere.position()));

		if (centerDistance2 < radius2)
		{
			return 0;
		}

		return (centerDistance2 + radius2) -
			2 * std::sqrt(centerDistance2) * radius;
	}

}

#endif
