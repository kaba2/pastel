// Description: Overlap tests between an aligned box and a sphere

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_SPHERE_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/sphere.h"
#include "pastel/geometry/distance/distance_alignedbox_sphere.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	//! Tests if an aligned box and a sphere overlap.
	/*!
	Preconditions:
	alignedBox.n() == sphere.n()

	Time complexity:
	O(n), where n is the dimension.

	Returns:
	Whether the box and the sphere overlap.

	Note: Currently the box is assumed closed.
	*/
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

		Real d2 =
			distance2(alignedBox, sphere.position());
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
