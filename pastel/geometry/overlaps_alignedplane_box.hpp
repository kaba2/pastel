#ifndef PASTEL_OVERLAPS_ALIGNEDPLANE_BOX_HPP
#define PASTEL_OVERLAPS_ALIGNEDPLANE_BOX_HPP

#include "pastel/geometry/overlaps_alignedplane_box.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	template <int N, typename Real>
		bool overlaps(AlignedPlane<Real, N> const &alignedPlane,
			Box<Real, N> const &box)
	{
		PENSURE_OP(alignedPlane.dimension(), ==, box.dimension());

		// Compute the radius of the projection
		// of box into the axis determined by the alignedplane

		const integer dimension = alignedPlane.dimension();

		Real radius(0);

		for (integer i = 0;i < dimension;++i)
		{
			// Move on the edges of the box
			// so that the positive axis direction
			// is always chosen.
			// When finished, we will be in the point
			// where the boxs surface is farthest from
			// its origin. Because the box is symmetric
			// w.r.t its axes, the minimal point
			// is also found in -radius.

			radius += mabs(box.rotation()[i][alignedPlane.axis()]);
		}

		if (mabs(box.position()[alignedPlane.axis()] -
			alignedPlane.position()) > radius)
		{
			// alignedPlane and box do not intersect

			return false;
		}

		// alignedPlane and box intersect

		return true;
	}

	template <int N, typename Real>
		bool overlaps(AlignedPlane<Real, N> const &alignedPlane,
			Box<Real, N> const &box,
			bool &boxOnPositiveSide)
	{
		boxOnPositiveSide =
			box.position()[alignedPlane.axis()] > alignedPlane.position();

		return overlaps(alignedPlane, box);
	}

}

#endif
