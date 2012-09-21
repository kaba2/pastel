#ifndef PASTELSYS_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTELSYS_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_HPP

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
		PENSURE_OP(aBox.n(), ==, bBox.n());

		bool empty = false;
		const integer n = aBox.n();
		for (integer i = 0;i < n;++i)
		{
			if (aBox.min()[i] > bBox.min()[i] ||
				(aBox.min()[i] == bBox.min()[i] &&
				aBox.minTopology()[i] == Topology::Open))
			{
				result.min()[i] = aBox.min()[i];
				result.minTopology()[i] = aBox.minTopology()[i];
			}
			else
			{
				result.min()[i] = bBox.min()[i];
				result.minTopology()[i] = bBox.minTopology()[i];
			}

			if (aBox.max()[i] < bBox.max()[i] ||
				(aBox.max()[i] == bBox.max()[i] &&
				aBox.maxTopology()[i] == Topology::Open))
			{
				result.max()[i] = aBox.max()[i];
				result.maxTopology()[i] = aBox.maxTopology()[i];
			}
			else
			{
				result.max()[i] = bBox.max()[i];
				result.maxTopology()[i] = bBox.maxTopology()[i];
			}

			empty |= result.empty(i);
		}
		
		return !empty;
	}

}

#endif
