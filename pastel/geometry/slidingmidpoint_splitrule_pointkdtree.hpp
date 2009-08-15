#ifndef PASTEL_SLIDINGMIDPOINT_SPLITRULE_POINTKDTREE_HPP
#define PASTEL_SLIDINGMIDPOINT_SPLITRULE_POINTKDTREE_HPP

#include "pastel/geometry/slidingmidpoint_splitrule_pointkdtree.h"
#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/point_tools.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	class SlidingMidpoint_SplitRule_PointKdTree
	{
	public:
		template <
			int N, typename Real,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const PointKdTree<Real, N, ObjectPolicy>& tree,
			const typename PointKdTree<Real, N, ObjectPolicy>::Cursor& cursor,
			const Point<Real, N>& minBound,
			const Point<Real, N>& maxBound) const
		{
			typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			const integer dimension = tree.dimension();
			const ObjectPolicy& objectPolicy = tree.objectPolicy();

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
					const Real position = objectPolicy.point(iter->object(), splitAxis);
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

	class SlidingMidpoint2_SplitRule_PointKdTree
	{
	public:
		template <
			int N, typename Real,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const PointKdTree<Real, N, ObjectPolicy>& tree,
			const typename PointKdTree<Real, N, ObjectPolicy>::Cursor& cursor,
			const Point<Real, N>& minBound,
			const Point<Real, N>& maxBound) const
		{
			typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			const integer dimension = tree.dimension();
			const ObjectPolicy& objectPolicy = tree.objectPolicy();

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
					const Real position = objectPolicy.point(iter->object(), splitAxis);
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
