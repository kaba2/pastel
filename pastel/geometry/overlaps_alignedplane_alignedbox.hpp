#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_ALIGNEDBOX_HPP

#include "pastel/geometry/overlaps_alignedplane_alignedbox.h"

#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const AlignedBox<Real, N>& box)
	{
		PENSURE_OP(plane.dimension(), ==, box.dimension());

		if (plane.position() >= box.max()[plane.axis()])
		{
			if (plane.position > box.max()[plane.axis()] ||
				box.maxTopology()[plane.axis()] == Topology::Open)
			{
				return false;
			}
		}

		if (plane.position() <= box.min()[plane.axis()])
		{
			if (plane.position() < box.min()[plane.axis()] ||
				box.minTopology()[plane.axis()] == Topology::Open)
			{
				return false;
			}
		}

		return true;
	}

	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const AlignedBox<Real, N>& box,
		bool &boxOnPositiveSide)
	{
		boxOnPositiveSide =
			box.min()[plane.axis()] > plane.position();

		return overlaps(plane, box);
	}

}

#endif
