#ifndef PASTEL_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTEL_CONTAINS_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/geometry/contains_alignedbox_alignedbox.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	bool contains(
		const AlignedBox<Real, N>& outer,
		const AlignedBox<Real, N>& inner)
	{
		PENSURE_OP(outer.dimension(), ==, inner.dimension());

		if (outer.empty())
		{
			if (inner.empty())
			{
				// The empty box is contained in
				// an empty box.
				return true;
			}

			// Since the inner box is not empty
			// but the outer box is,
			// there can't be containment.
			return false;
		}

		const integer n = outer.dimension();
		for (integer i = 0;i < n;++i)
		{
			if (inner.empty(i))
			{
				// The empty box is contained in every box.
				return true;
			}

			if (inner.min()[i] <= outer.min()[i])
			{
				// The inner box possibly extends 
				// outside the minimum of the outer box.
				if (inner.min()[i] < outer.min()[i] ||
					(inner.minTopology()[i] == Topology::Closed &&
					outer.minTopology()[i] == Topology::Open))
				{
					// It does, thus no containment.
					return false;
				}
			}

			if (inner.max()[i] >= outer.max()[i])
			{
				// The inner box possibly extends 
				// outside the maximum of the outer box.
				if (inner.max()[i] > outer.max()[i] ||
					(inner.maxTopology()[i] == Topology::Closed &&
					outer.maxTopology()[i] == Topology::Open))
				{
					// It does, thus no containment.
					return false;
				}
			}
		}

		// The inner box is contained in the outer box.
		return true;
	}

}

#endif
