#ifndef PASTELGEOMETRY_FAIR_SPLITRULE_HPP
#define PASTELGEOMETRY_FAIR_SPLITRULE_HPP

#include "pastel/geometry/splitrule/fair_splitrule.h"

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/locator/locator_concept.h"
#include "pastel/geometry/shape/alignedbox.h"

namespace Pastel
{

	class Fair_SplitRule
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

			// Find out the minimum bounding interval for
			// the contained points on the splitting axis.

			Real minPosition = infinity<Real>();
			Real maxPosition = -infinity<Real>();

			forEach(pointSet, [&](auto&& point)
			{
				Real position = 
					pointAxis(point, splitAxis);
					
				if (position < minPosition)
				{
					minPosition = position;
				}
				if (position > maxPosition)
				{
					maxPosition = position;
				}

				return true;
			});

			// Split at the midpoint of the minimum
			// bounding interval.

			splitPosition = 
				linear(minPosition, maxPosition, 0.5);

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
