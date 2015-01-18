#ifndef PASTELGEOMETRY_FAIR_SPLITRULE_HPP
#define PASTELGEOMETRY_FAIR_SPLITRULE_HPP

#include "pastel/geometry/splitrule/fair_splitrule.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	class Fair_SplitRule
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
				// Find out the minimum bounding interval for
				// the contained points on the splitting axis.

				Real minPosition = infinity<Real>();
				Real maxPosition = -infinity<Real>();

				while(!pointSet.empty())
				{
					Real position = 
						locator(pointSet(), splitAxis);
					if (position < minPosition)
					{
						minPosition = position;
					}
					if (position > maxPosition)
					{
						maxPosition = position;
					}
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
