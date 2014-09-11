#ifndef PASTELGEOMETRY_LONGESTMEDIAN_SPLITRULE_HPP
#define PASTELGEOMETRY_LONGESTMEDIAN_SPLITRULE_HPP

#include "pastel/geometry/longestmedian_splitrule.h"

#include "pastel/sys/vector_tools.h"

#include <algorithm>

namespace Pastel
{

	class LongestMedian_SplitRule
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
			Real splitPosition = linear(
				bound.min()[splitAxis], 
				bound.max()[splitAxis], 0.5);

			if (!pointSet.empty())
			{
				// Get the positions of the points along the splitting axis.

				std::vector<Real> positionSet;
				positionSet.reserve(pointSet.nHint());

				while(!pointSet.empty())
				{
					positionSet.emplace_back(
						locator(pointSet.get(), splitAxis));
					pointSet.pop();
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
