#ifndef PASTEL_SLIDINGMIDPOINT_SPLITRULE_POINTKDTREE_HPP
#define PASTEL_SLIDINGMIDPOINT_SPLITRULE_POINTKDTREE_HPP

#include "pastel/geometry/slidingmidpoint_splitrule_pointkdtree.h"
#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	class SlidingMidpoint_SplitRule_PointKdTree
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
			typedef typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			const integer dimension = tree.dimension();
			const PointPolicy& pointPolicy = tree.pointPolicy();

			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			// Sliding midpoint

			if (!cursor.empty())
			{
				Real leftMax = -infinity<Real>();
				Real rightMin = infinity<Real>();
				integer leftCount = 0;
				integer rightCount = 0;

				ConstObjectIterator iter = cursor.begin();
				const ConstObjectIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					const Real position = 
						pointPolicy(iter->object())[splitAxis];
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
