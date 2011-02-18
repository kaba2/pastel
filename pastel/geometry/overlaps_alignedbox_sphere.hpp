#ifndef PASTEL_OVERLAPS_ALIGNEDBOX_SPHERE_HPP
#define PASTEL_OVERLAPS_ALIGNEDBOX_SPHERE_HPP

#include "pastel/geometry/overlaps_alignedbox_sphere.h"
#include "pastel/geometry/distance_alignedbox_sphere.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, int N>
	bool overlaps(
		const AlignedBox<Real, N>& alignedBox,
		const Sphere<Real, N>& sphere)
	{
		// An aligned box and a sphere intersect if
		// the distance of the sphere's center
		// point from the aligned box is smaller
		// than the sphere's radius.

		// FIX: The box's topology is not handled correctly.
		// I do not see a way to handle it.
		// The box is assumed closed.

		const Real d2 =
			distance2(alignedBox, sphere.position());
		const Real radius2 =
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
