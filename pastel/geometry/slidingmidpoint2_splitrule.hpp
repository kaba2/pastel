#ifndef PASTELGEOMETRY_SLIDINGMIDPOINT2_SPLITRULE_HPP
#define PASTELGEOMETRY_SLIDINGMIDPOINT2_SPLITRULE_HPP

#include "pastel/geometry/slidingmidpoint2_splitrule.h"
#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	class SlidingMidpoint2_SplitRule
	{
	public:
		template <
			typename Real, int N,
			typename PointPolicy>
			std::pair<Real, integer> operator()(
			const PointKdTree<Real, N, PointPolicy>& tree,
			const typename PointKdTree<Real, N, PointPolicy>::Cursor& cursor,
			const Vector<Real, N>& minBound,
			const Vector<Real, N>& maxBound,
			integer depth) const
		{
			typedef typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator 
				Point_ConstIterator;

			const PointPolicy& pointPolicy = tree.pointPolicy();

			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			// Modified sliding midpoint

			if (!cursor.empty())
			{
				Real leftMax = -infinity<Real>();
				Real rightMin = infinity<Real>();
				integer leftCount = 0;
				integer rightCount = 0;

				// Count the number of points on left
				// and right. Also, find maximum
				// point on the left side and the minimum
				// on the right side.

				Point_ConstIterator iter = cursor.begin();
				const Point_ConstIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					const Real position = 
						pointPolicy.axis(iter->point(), splitAxis);

						//pointPolicy(iter->point())[splitAxis];
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

					++iter;
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
