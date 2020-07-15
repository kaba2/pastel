// Description: Median-of-longest-side splitting rule

#ifndef PASTELGEOMETRY_LONGESTMEDIAN_SPLITRULE_H
#define PASTELGEOMETRY_LONGESTMEDIAN_SPLITRULE_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/locator/locator_concept.h"

#include "pastel/geometry/splitrule/splitrule_concept.h"
#include "pastel/geometry/shape/alignedbox.h"

#include <algorithm>
#include <vector>

namespace Pastel
{

	//! Median-of-longest-side splitting rule.
	class LongestMedian_SplitRule
	{
	public:
		template <
			PointSet_Concept PointSet,
			typename Real = PointSet_Real<PointSet>,
			int N = PointSet_Dimension<PointSet>::value
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

			if (isEmptyRange(pointSet))
			{
				return std::make_pair(splitPosition, splitAxis);
			}

			// Get the positions of the points along the splitting axis.

			std::vector<Real> positionSet;
			positionSet.reserve(rangeSize(pointSet));

			for (auto&& point : pointSet)
			{
				positionSet.emplace_back(
					pointAxis(point, splitAxis));
			}

			// Get the median of the points on the splitting axis.
			std::sort(positionSet.begin(), positionSet.end());
			splitPosition = positionSet[positionSet.size() / 2];

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
