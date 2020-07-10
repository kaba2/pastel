// Description: Overlap tests between an aligned plane an a box

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_BOX_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_BOX_H

#include "pastel/geometry/shape/alignedplane.h"
#include "pastel/geometry/shape/box.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Tests if an aligned plane and an box overlap.
	/*!
	Preconditions:
	plane.n() == box.n()

	Time complexity:
	O(n), where n is the dimension.

	Note: Currently the box is assumed to be closed.
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const Box<Real, N>& box)
	{
		PENSURE_OP(plane.n(), ==, box.n());

		// FIX: The topology of the box is not
		// handled correctly. The box is assumed
		// to be closed.

		// Compute the radius of the projection
		// of the box onto the plane normal.

		Real radius = 0;

		integer n = plane.n();
		for (integer i = 0;i < n;++i)
		{
			// Move on the edges of the box
			// so that the positive axis direction
			// is always chosen.
			// When finished, we will be in the point
			// where the box's surface is farthest from
			// its origin. Because the box is symmetric
			// w.r.t its axes, the minimal point
			// is also found in -radius.

			radius += abs(box.rotation()[i][plane.axis()]);
		}

		Real centerDistance =
			abs(box.position()[plane.axis()] - plane.position());

		return centerDistance <= radius;
	}

	//! Tests if an aligned plane and an box overlap.
	/*!
	boxOnPositiveSide:
	Filled with the information
	if the center point of the box is on the positive
	side of the plane.

	Returns:
	overlaps(plane, box)
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const Box<Real, N>& box,
		bool& boxOnPositiveSide)
	{
		boxOnPositiveSide =
			box.position()[plane.axis()] > plane.position();

		return overlaps(plane, box);
	}

}

#endif
