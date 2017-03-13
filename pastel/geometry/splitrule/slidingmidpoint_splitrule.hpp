#ifndef PASTELGEOMETRY_SLIDINGMIDPOINT_SPLITRULE_HPP
#define PASTELGEOMETRY_SLIDINGMIDPOINT_SPLITRULE_HPP

#include "pastel/geometry/splitrule/slidingmidpoint_splitrule.h"

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/locator/locator_concept.h"
#include "pastel/geometry/shape/alignedbox.h"

namespace Pastel
{

	class SlidingMidpoint_SplitRule
	{
	public:
		template <
			typename PointSet,
			typename Real = PointSet_Real<PointSet>,
			integer N = PointSet_Dimension<PointSet>::value,
			Requires<
				Models<PointSet, PointSet_Concept>
			> = 0
		>
		std::pair<Real, integer> operator()(
			const PointSet& pointSet,
			const AlignedBox<Real, N>& bound) const
		{
			// Split along the longest dimension.
			integer splitAxis = maxIndex(bound.extent());

			// First try the mid-point of the bounding box.
			Real splitPosition = linear(
				bound.min()[splitAxis], 
				bound.max()[splitAxis], 0.5);

			if (emptySet(pointSet))
			{
				return std::make_pair(splitPosition, splitAxis);
			}
				
			// Sliding mid-point

			Real leftMax = -Infinity();
			integer leftCount = 0;

			Real rightMin = Infinity();
			integer rightCount = 0;

			forEach(pointSet, [&](auto&& point)
			{
				Real position = pointAxis(point, splitAxis);
				if (position < splitPosition)
				{
					if (position > leftMax)
					{
						leftMax = position;
					}
					++leftCount;
				}
				else
				{
					if (position < rightMin)
					{
						rightMin = position;
					}
					++rightCount;
				}

				return true;
			});

			if (leftCount == 0)
			{
				splitPosition = rightMin;
			}
			else if (rightCount == 0)
			{
				splitPosition = leftMax;
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
