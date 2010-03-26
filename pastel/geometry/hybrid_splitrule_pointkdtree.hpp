#ifndef PASTEL_HYBRID_SPLITRULE_POINTKDTREE_HPP
#define PASTEL_HYBRID_SPLITRULE_POINTKDTREE_HPP

#include "pastel/geometry/hybrid_splitrule_pointkdtree.h"
#include "pastel/geometry/midpoint_splitrule_pointkdtree.h"
#include "pastel/geometry/longestmedian_splitrule_pointkdtree.h"
#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	class Hybrid_SplitRule_PointKdTree
	{
	public:
		template <
			typename Real, int N,
			typename ObjectPolicy>
			std::pair<Real, integer> operator()(
			const PointKdTree<Real, N, ObjectPolicy>& tree,
			const typename PointKdTree<Real, N, ObjectPolicy>::Cursor& cursor,
			const Vector<Real, N>& minBound,
			const Vector<Real, N>& maxBound,
			integer depth) const
		{
			const Midpoint_SplitRule_PointKdTree midpoint;
			const LongestMedian_SplitRule_PointKdTree median;

			if ((depth & 1) == 1)
			{
				return midpoint(tree, cursor, minBound, maxBound, depth);
			}

			return median(tree, cursor, minBound, maxBound, depth);
		}
	};

}

#endif
