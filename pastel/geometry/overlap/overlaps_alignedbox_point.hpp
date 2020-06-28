#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_POINT_HPP
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_POINT_HPP

#include "pastel/geometry/overlap/overlaps_alignedbox_point.h"
#include "pastel/geometry/overlap/overlaps_alignedbox_sphere.h"
#include "pastel/geometry/shape/alignedbox.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <typename Real, int N>
	bool overlaps(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point)
	{
		PENSURE_OP(alignedBox.n(), ==, point.n());

		integer n = alignedBox.n();
		for (integer i = 0;i < n;++i)
		{
			// Test if the i:th coordinate of the point
			// lies beyond the i:th coordinate range
			// of the aligned box.

			if (point[i] <= alignedBox.min()[i])
			{
				if (point[i] < alignedBox.min()[i] ||
					alignedBox.minTopology()[i] == Topology::Open)
				{
					return false;
				}
			}
			
			if (point[i] >= alignedBox.max()[i])
			{
				if (point[i] > alignedBox.max()[i] ||
					alignedBox.maxTopology()[i] == Topology::Open)
				{
					return false;
				}
			}
		}

		return true;
	}

}

#endif
