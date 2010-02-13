#ifndef PASTEL_LONGESTMEDIAN_SPLITRULE_POINTKDTREE_HPP
#define PASTEL_LONGESTMEDIAN_SPLITRULE_POINTKDTREE_HPP

#include "pastel/geometry/longestmedian_splitrule_pointkdtree.h"
#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/vector_tools.h"

#include <algorithm>

namespace Pastel
{

	class LongestMedian_SplitRule_PointKdTree
	{
	public:
		template <
			typename Real, int N,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const PointKdTree<Real, N, ObjectPolicy>& tree,
			const typename PointKdTree<Real, N, ObjectPolicy>::Cursor& cursor,
			const Vector<Real, N>& minBound,
			const Vector<Real, N>& maxBound) const
		{
			typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator 
				ConstObjectIterator;

			const ObjectPolicy& objectPolicy = tree.objectPolicy();

			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			if (!cursor.empty())
			{
				// Get the positions of the points along the splitting axis.

				std::vector<Real> positionSet;
				positionSet.reserve(cursor.objects());

				ConstObjectIterator iter = cursor.begin();
				const ConstObjectIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					positionSet.push_back(objectPolicy.point(iter->object(), splitAxis));
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
