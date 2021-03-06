// Description: Modified sliding midpoint splitting rule

#ifndef PASTELGEOMETRY_SLIDINGMIDPOINT2_SPLITRULE_H
#define PASTELGEOMETRY_SLIDINGMIDPOINT2_SPLITRULE_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/locator/locator_concept.h"

#include "pastel/geometry/splitrule/splitrule_concept.h"
#include "pastel/geometry/shape/alignedbox.h"

namespace Pastel
{

	//! Modified sliding midpoint splitting rule.
	class SlidingMidpoint2_SplitRule
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

			// Modified sliding midpoint

			if (isEmptyRange(pointSet))
			{
				return std::make_pair(splitPosition, splitAxis);
			}

			Real leftMax = -Infinity();
			Real rightMin = Infinity();
			integer leftCount = 0;
			integer rightCount = 0;

			// Count the number of points on left
			// and right. Also, find maximum
			// point on the left side and the minimum
			// on the right side.

			for (auto&& point : pointSet)
			{
				Real position = 
					pointAxis(point, splitAxis);

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
			}

			if (leftCount > 0)
			{
				if (rightCount > 0)
				{
					if (leftCount < rightCount)
					{
						splitPosition = rightMin;
					}
					else
					{
						splitPosition = leftMax;
					}
					/*
					splitPosition = (splitPosition - leftMax) < 
						(rightMin - splitPosition) ?
						//leftMax : rightMin;
						rightMin : leftMax;
					*/
				}
				else
				{
					splitPosition = leftMax;
				}
			}
			else
			{
				ASSERT(rightCount > 0);
				splitPosition = rightMin;
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
