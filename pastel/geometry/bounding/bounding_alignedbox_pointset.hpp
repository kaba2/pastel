#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_HPP
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_HPP

#include "pastel/geometry/bounding/bounding_alignedbox_pointset.h"
#include "pastel/geometry/bounding/bounding_alignedbox_alignedbox.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <
		typename PointSet,
		Requires<Models<PointSet, PointSet_Concept>>
	>
	auto boundingAlignedBox(PointSet pointSet)
	-> AlignedBox<PointSet_Real<PointSet>, PointSet_N<PointSet>::value>
	{
		using Real = PointSet_Real<PointSet>;
		static constexpr integer N = PointSet_N<PointSet>::value;
		integer d = pointSetDimension(pointSet);
		
		AlignedBox<Real, N> bound(d);
		pointSet.forEach([&](auto&& point)
		{
			for (integer i = 0;i < d;++i)
			{
				auto x = pointAxis(point, i);

				if (x < bound.min()[i])
				{
					bound.min()[i] = x;
				}

				if (x > bound.max()[i])
				{
					bound.max()[i] = x;
				}
			}

			return true;
		});

		return bound;
	}

}

#endif
