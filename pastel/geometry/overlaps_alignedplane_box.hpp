#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_BOX_HPP
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_BOX_HPP

#include "pastel/geometry/overlaps_alignedplane_box.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

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

			radius += mabs(box.rotation()[i][plane.axis()]);
		}

		Real centerDistance =
			mabs(box.position()[plane.axis()] - plane.position());

		return centerDistance <= radius;
	}

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
