#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_HPP
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_HPP

#include "pastel/geometry/bounding/bounding_alignedbox_pointset.h"
#include "pastel/geometry/bounding/bounding_alignedbox_alignedbox.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <
		typename PointSet,
		typename
	>
	auto boundingAlignedBox(PointSet pointSet)
	-> AlignedBox<PointSet_Real<PointSet>, PointSet_N<PointSet>::value>
	{
		using Real = PointSet_Real<PointSet>;
		static PASTEL_CONSTEXPR integer N = PointSet_N<PointSet>::value;
		integer d = pointSetDimension(pointSet);
		
		AlignedBox<Real, N> bound(d);
		while (!pointSetEmpty(pointSet))
		{
			auto&& point = pointSetGet(pointSet);
			for (integer i = 0;i < d;++i)
			{
				auto x = axis(point, i);

				if (x < bound.min()[i])
				{
					bound.min()[i] = x;
				}

				if (x > bound.max()[i])
				{
					bound.max()[i] = x;
				}
			}

			pointSetPop(pointSet);
		}

		return bound;
	}

}

#endif
