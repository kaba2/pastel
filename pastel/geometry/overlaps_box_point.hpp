#ifndef PASTELGEOMETRY_OVERLAPS_BOX_POINT_HPP
#define PASTELGEOMETRY_OVERLAPS_BOX_POINT_HPP

#include "pastel/geometry/overlaps_box_point.h"

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Real, int N>
	bool overlaps(
		const Box<Real, N>& box,
		const Vector<Real, N>& point)
	{
		PENSURE_OP(alignedBox.n(), ==, point.n());

		integer n = alignedBox.n();
		for (integer i = 0;i < n;++i)
		{
			// When transformed to the box's coordinate
			// system, the problem becomes the testing
			// of overlap between an aligned box and a 
			// point.

			// Test if the i:th coordinate of the point
			// lies beyond the i:th coordinate range
			// of the aligned box.

			const Real projectedDelta =
				dot(point - box.position(), box.rotation()[i]);

			if (projectedDelta <= -box.width()[i])
			{
				if (projectedDelta < -box.width()[i] ||
					box.minTopology()[i] == Topology::Open)
				{
					return false;
				}
			}

			if (projectedDelta >= box.width()[i])
			{
				if (projectedDelta > box.width()[i] ||
					box.maxTopology()[i] == Topology::Open)
				{
					return false;
				}
			}
		}

		return true;
	}

}

#endif
