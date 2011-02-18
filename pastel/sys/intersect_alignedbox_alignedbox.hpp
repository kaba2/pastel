#ifndef PASTEL_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTEL_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/sys/intersect_alignedbox_alignedbox.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	bool intersect(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		AlignedBox<Real, N>& result)
	{
		PENSURE_OP(aBox.dimension(), ==, bBox.dimension());

		const integer n = aBox.dimension();
		for (integer i = 0;i < n;++i)
		{
			if (aBox.max()[i] <= bBox.min()[i])
			{
				if (aBox.max()[i] < bBox.min()[i] ||
					aBox.maxTopology()[i] == Topology::Open ||
					bBox.minTopology()[i] == Topology::Open)
				{
					return false;
				}
			}
			if (bBox.max()[i] <= aBox.min()[i])
			{
				if (bBox.max()[i] < aBox.min()[i] ||
					bBox.maxTopology()[i] == Topology::Open ||
					aBox.minTopology()[i] == Topology::Open)
				{
					return false;
				}
			}
		}

		result.set(
			max(aBox.min(), bBox.min()),
			min(aBox.max(), bBox.max()));

		return true;
	}

}

#endif
