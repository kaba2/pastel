#ifndef PASTELGEOMETRY_POINTKDTREE_SPLITPREDICATE_H
#define PASTELGEOMETRY_POINTKDTREE_SPLITPREDICATE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/pointkdtree_fwd.h"

namespace Pastel
{

	template <typename Settings>
	class PointKdTree_Fwd<Settings>::SplitPredicate
	{
	public:
		SplitPredicate(
			const Real& splitPosition,
			integer splitAxis,
			const Locator& locator)
			: splitPosition_(splitPosition)
			, splitAxis_(splitAxis)
			, locator_(locator)
		{
		}
		
		TriState operator()(const PointInfo& point) const
		{
			return triLess(
				locator_(*point, splitAxis_),
				splitPosition_);
		}

	private:
		Real splitPosition_;
		integer splitAxis_;
		const Locator& locator_;
	};

}

#endif
