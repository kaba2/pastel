#ifndef PASTELGEOMETRY_FAIR_SPLITRULE_HPP
#define PASTELGEOMETRY_FAIR_SPLITRULE_HPP

#include "pastel/geometry/fair_splitrule.h"
#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	class Fair_SplitRule
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
				// Find out the minimum bounding interval for
				// the contained points on the splitting axis.

				Real minPosition = infinity<Real>();
				Real maxPosition = -infinity<Real>();

				Point_ConstIterator iter = cursor.begin();
				const Point_ConstIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					const Real position = 
						pointPolicy.axis(iter->point(), splitAxis);
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
