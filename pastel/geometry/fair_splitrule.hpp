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
