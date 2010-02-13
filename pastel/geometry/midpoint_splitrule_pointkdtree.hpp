#ifndef PASTEL_MIDPOINT_SPLITRULE_POINTKDTREE_HPP
#define PASTEL_MIDPOINT_SPLITRULE_POINTKDTREE_HPP

#include "pastel/geometry/midpoint_splitrule_pointkdtree.h"
#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	class Midpoint_SplitRule_PointKdTree
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
			// Split along the longest dimension.

			const integer splitAxis = maxIndex(maxBound - minBound);
			const Real splitPosition = linear(minBound[splitAxis], 
				maxBound[splitAxis], 0.5);

			return std::make_pair(splitPosition, splitAxis);
		}
	};

}

#endif
