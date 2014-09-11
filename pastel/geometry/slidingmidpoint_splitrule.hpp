#ifndef PASTELGEOMETRY_SLIDINGMIDPOINT_SPLITRULE_HPP
#define PASTELGEOMETRY_SLIDINGMIDPOINT_SPLITRULE_HPP

#include "pastel/geometry/slidingmidpoint_splitrule.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	class SlidingMidpoint_SplitRule
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

			// First try the mid-point of the bounding box.
			Real splitPosition = linear(
				bound.min()[splitAxis], 
				bound.max()[splitAxis], 0.5);

			if (!pointSet.empty())
			{
				// Sliding mid-point

				Real leftMax = -infinity<Real>();
				integer leftCount = 0;

				Real rightMin = infinity<Real>();
				integer rightCount = 0;

				while(!pointSet.empty())
				{
					Real position = locator(pointSet.get(), splitAxis);
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

					pointSet.pop();
				}

				if (leftCount == 0)
				{
					splitPosition = rightMin;
				}
				else if (rightCount == 0)
				{
					splitPosition = leftMax;
				}
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
