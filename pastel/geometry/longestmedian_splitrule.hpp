#ifndef PASTEL_LONGESTMEDIAN_SPLITRULE_HPP
#define PASTEL_LONGESTMEDIAN_SPLITRULE_HPP

#include "pastel/geometry/longestmedian_splitrule.h"
#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/vector_tools.h"

#include <algorithm>

namespace Pastel
{

	class LongestMedian_SplitRule
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

			if (!cursor.empty())
			{
				// Get the positions of the points along the splitting axis.

				std::vector<Real> positionSet;
				positionSet.reserve(cursor.points());

				Point_ConstIterator iter = cursor.begin();
				const Point_ConstIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					positionSet.push_back(
						pointPolicy(iter->point())[splitAxis]);
					++iter;
				}

				// Get the median of the points on the splitting axis.

				std::sort(positionSet.begin(), positionSet.end());

				splitPosition = positionSet[positionSet.size() / 2];
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
