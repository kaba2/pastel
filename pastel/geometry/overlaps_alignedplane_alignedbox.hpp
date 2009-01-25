#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_ALIGNEDBOX_HPP

#include "pastel/geometry/overlaps_alignedplane_alignedbox.h"

#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
		bool overlaps(AlignedPlane<N, Real> const &alignedPlane,
			AlignedBox<N, Real> const &alignedBox)
	{
		if (alignedPlane.position() > alignedBox.max()[alignedPlane.axis()])
		{
			return false;
		}

		if (alignedPlane.position() < alignedBox.min()[alignedPlane.axis()])
		{
			return false;
		}

		return true;
	}

	template <int N, typename Real>
		bool overlaps(AlignedPlane<N, Real> const &alignedPlane,
			AlignedBox<N, Real> const &alignedBox,
			bool &alignedBoxOnPositiveSide)
	{
		alignedBoxOnPositiveSide =
			alignedBox.min()[alignedPlane.axis()] > alignedPlane.position();

		return overlaps(alignedPlane, alignedBox);
	}

}

#endif
