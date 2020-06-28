#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_HPP
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_POINTSET_HPP

#include "pastel/geometry/bounding/bounding_alignedbox_pointset.h"
#include "pastel/geometry/bounding/bounding_alignedbox_alignedbox.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <PointSet_Concept_ PointSet>
	auto boundingAlignedBox(PointSet pointSet)
	-> AlignedBox<PointSet_Real<PointSet>, PointSet_Dimension<PointSet>::value>
	{
		using Real = PointSet_Real<PointSet>;
		static constexpr int N = PointSet_Dimension<PointSet>::value;
		integer d = pointSetDimension(pointSet);
		
		AlignedBox<Real, N> bound(d);
		RANGES_FOR(auto&& point, pointSet)
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
		}

		return bound;
	}

}

#endif
