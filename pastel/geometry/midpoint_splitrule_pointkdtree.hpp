#ifndef MIDPOINT_SPLITRULE_POINTKDTREE_HPP
#define MIDPOINT_SPLITRULE_POINTKDTREE_HPP

#include "pastel/geometry/midpoint_splitrule_pointkdtree.h"
#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/point_tools.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	class Midpoint_SplitRule_PointKdTree
	{
	public:
		template <
			int N, typename Real,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const PointKdTree<Real, N, ObjectPolicy>& tree,
			const typename PointKdTree<Real, N, ObjectPolicy>::Cursor& cursor,
			const Point<Real, N>& minBound,
			const Point<Real, N>& maxBound) const
		{
			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			const Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
