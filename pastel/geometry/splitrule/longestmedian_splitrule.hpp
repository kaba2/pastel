#ifndef PASTELGEOMETRY_LONGESTMEDIAN_SPLITRULE_HPP
#define PASTELGEOMETRY_LONGESTMEDIAN_SPLITRULE_HPP

#include "pastel/geometry/splitrule/longestmedian_splitrule.h"

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/locator/locator_concept.h"
#include "pastel/geometry/shape/alignedbox.h"

#include <algorithm>
#include <vector>

namespace Pastel
{

	class LongestMedian_SplitRule
	{
	public:
		template <
			typename PointSet,
			typename Real = PointSet_Real<PointSet>,
			integer N = PointSet_Dimension<PointSet>::value,
			Requires<
				Models<PointSet, PointSet_Concept>
			> = 0
		>
		std::pair<Real, integer> operator()(
			const PointSet& pointSet,
			const AlignedBox<Real, N>& bound) const
		{
			// Split along the longest dimension.

			integer splitAxis = maxIndex(bound.extent());
			Real splitPosition = linear(
				bound.min()[splitAxis], 
				bound.max()[splitAxis], 0.5);

			if (pointSet.empty())
			{
				return std::make_pair(splitPosition, splitAxis);
			}

			// Get the positions of the points along the splitting axis.

			std::vector<Real> positionSet;
			positionSet.reserve(pointSet.n());

			forEach(pointSet, [&](auto&& point)
			{
				positionSet.emplace_back(
					pointAxis(point, splitAxis));
				return true;
			});

			// Get the median of the points on the splitting axis.
			std::sort(positionSet.begin(), positionSet.end());
			splitPosition = positionSet[positionSet.size() / 2];

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
