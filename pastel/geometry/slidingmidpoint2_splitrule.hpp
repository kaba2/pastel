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
			typename Settings, template <typename> class Customization,
			typename Real = typename Settings::Real,
			integer N = Settings::N,
			typename Cursor = typename PointKdTree<Settings, Customization>::Cursor>
		std::pair<typename Settings::Real, integer> operator()(
			const PointKdTree<Settings, Customization>& tree,
			const PASTEL_NO_DEDUCTION(Cursor)& cursor,
			const PASTEL_NO_DEDUCTION((Vector<Real, N>))& minBound,
			const PASTEL_NO_DEDUCTION((Vector<Real, N>))& maxBound,
			integer depth) const
		{
			using Tree = PointKdTree<Settings, Customization>;
			using Fwd = Tree;
			PASTEL_FWD(Point_ConstIterator);
			PASTEL_FWD(PointPolicy);

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
