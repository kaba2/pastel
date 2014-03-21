#ifndef PASTELGEOMETRY_POINTKDTREE_SPLITPREDICATE_H
#define PASTELGEOMETRY_POINTKDTREE_SPLITPREDICATE_H

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	template <typename Real, int N, typename PointPolicy>
	class PointKdTree<Real, N, PointPolicy>::SplitPredicate
	{
	public:
		SplitPredicate(
			const Real& splitPosition,
			integer splitAxis,
			const PointPolicy& pointPolicy)
			: splitPosition_(splitPosition)
			, splitAxis_(splitAxis)
			, pointPolicy_(pointPolicy)
		{
		}
		
		TriState operator()(const PointInfo& point) const
		{
			return triLess(
				pointPolicy_.axis(*point, splitAxis_),
				splitPosition_);
		}

	private:
		Real splitPosition_;
		integer splitAxis_;
		const PointPolicy& pointPolicy_;
	};

}

#endif
