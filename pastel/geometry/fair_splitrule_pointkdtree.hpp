#ifndef PASTEL_FAIR_SPLITRULE_POINTKDTREE_HPP
#define PASTEL_FAIR_SPLITRULE_POINTKDTREE_HPP

#include "pastel/geometry/fair_splitrule_pointkdtree.h"
#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	class Fair_SplitRule_PointKdTree
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
				// Find out the minimum bounding interval for
				// the contained points on the splitting axis.

				Real minPosition = infinity<Real>();
				Real maxPosition = -infinity<Real>();

				ConstObjectIterator iter = cursor.begin();
				const ConstObjectIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					const Real position = 
						objectPolicy.point(iter->object(), splitAxis);
					if (position < minPosition)
					{
						minPosition = position;
					}
					if (position > maxPosition)
					{
						maxPosition = position;
					}
					++iter;
				}

				// Split at the midpoint of the minimum
				// bounding interval.

				splitPosition = 
					linear(minPosition, maxPosition, 0.5);
			}

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
