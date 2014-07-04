#ifndef PASTELGEOMETRY_LONGESTMEDIAN_SPLITRULE_HPP
#define PASTELGEOMETRY_LONGESTMEDIAN_SPLITRULE_HPP

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
