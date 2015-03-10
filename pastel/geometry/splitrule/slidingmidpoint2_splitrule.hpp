#ifndef PASTELGEOMETRY_SLIDINGMIDPOINT2_SPLITRULE_HPP
#define PASTELGEOMETRY_SLIDINGMIDPOINT2_SPLITRULE_HPP

#include "pastel/geometry/splitrule/slidingmidpoint2_splitrule.h"

#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	class SlidingMidpoint2_SplitRule
	{
	public:
		template <
			typename Point_Input,
			typename Locator,
			typename Real = typename Locator::Real,
			integer N = Locator::N>
		std::pair<Real, integer> operator()(
			Point_Input pointSet,
			const Locator& locator,
			const AlignedBox<Real, N>& bound) const
		{
			// Split along the longest dimension.

			integer splitAxis = maxIndex(bound.extent());
			Real splitPosition = linear(
				bound.min()[splitAxis], 
				bound.max()[splitAxis], 0.5);

			// Modified sliding midpoint

			if (!pointSet.empty())
			{
				Real leftMax = -infinity<Real>();
				Real rightMin = infinity<Real>();
				integer leftCount = 0;
				integer rightCount = 0;

				// Count the number of points on left
				// and right. Also, find maximum
				// point on the left side and the minimum
				// on the right side.

				while(!pointSet.empty())
				{
					Real position = 
						locator(pointSet.get(), splitAxis);
					pointSet.pop();

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
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
