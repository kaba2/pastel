#ifndef PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_INTERSECT_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/geometry/intersect_alignedbox_alignedbox.h"

#include "pastel/sys/point_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	bool intersect(
		const AlignedBox<N, Real>& aBox,
		const AlignedBox<N, Real>& bBox,
		AlignedBox<N, Real>& result)
	{
		PENSURE(aBox.dimension() == bBox.dimension());

		const integer dimension = aBox.dimension();

		for (integer i = 0;i < dimension;++i)
		{
			if (aBox.max()[i] < bBox.min()[i] ||
				bBox.max()[i] < aBox.min()[i])
			{
				return false;
			}
		}

		result.set(
			max(aBox.min(), bBox.min()),
			min(aBox.max(), bBox.max()));

		return true;
	}

}

#endif
