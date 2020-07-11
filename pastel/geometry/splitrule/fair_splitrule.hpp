#ifndef PASTELGEOMETRY_FAIR_SPLITRULE_HPP
#define PASTELGEOMETRY_FAIR_SPLITRULE_HPP

#include "pastel/geometry/splitrule/fair_splitrule.h"

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/locator/locator_concept.h"
#include "pastel/geometry/shape/alignedbox.h"

namespace Pastel
{

	class Fair_SplitRule
	{
	public:
		template <
			PointSet_Concept PointSet,
			typename Real = PointSet_Real<PointSet>,
			int N = PointSet_Dimension<PointSet>::value
		>
		std::pair<Real, integer> operator()(
			const PointSet& pointSet,
			const AlignedBox<Real, N>& bound) const
		{
			// Split along the longest dimension.

			integer splitAxis = maxIndex(bound.extent());
			Real splitPosition = linear(
				bound.min()[splitAxis], 
				bound.max()[splitAxis], 0.5);

			if (pointSet.empty())
			{
				return std::make_pair(splitPosition, splitAxis);
			}

			// Find out the minimum bounding interval for
			// the contained points on the splitting axis.

			Real minPosition = Infinity();
			Real maxPosition = -Infinity();

			RANGES_FOR(auto&& point, pointSet)
			{
				Real position = 
					pointAxis(point, splitAxis);
					
				if (position < minPosition)
				{
					minPosition = position;
				}
				if (position > maxPosition)
				{
					maxPosition = position;
				}
			}

			// Split at the midpoint of the minimum
			// bounding interval.

			splitPosition = 
				linear(minPosition, maxPosition, 0.5);

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
