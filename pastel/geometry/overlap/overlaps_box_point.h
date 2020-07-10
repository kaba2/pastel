// Description: Overlap tests between a box and a point

#ifndef PASTELGEOMETRY_OVERLAPS_BOX_POINT_H
#define PASTELGEOMETRY_OVERLAPS_BOX_POINT_H

#include "pastel/geometry/shape/box.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Tests if an box and a point overlap.
	/*!
	Preconditions:
	box.n() == point.n()

	Returns:
	Whether the box and the point overlap.
	*/
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

			Real projectedDelta =
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
