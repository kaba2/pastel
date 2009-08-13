#ifndef PASTEL_POINTKDTREE_SPLITPREDICATE_H
#define PASTEL_POINTKDTREE_SPLITPREDICATE_H

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	template <typename Real, int N, typename ObjectPolicy>
	class PointKdTree<Real, N, ObjectPolicy>::SplitPredicate
	{
	public:
		SplitPredicate(
			const Real& splitPosition,
			integer splitAxis,
			const ObjectPolicy& objectPolicy)
			: splitPosition_(splitPosition)
			, splitAxis_(splitAxis)
			, objectPolicy_(objectPolicy)
		{
		}
		
		TriState::Enum operator()(const ObjectInfo& object) const
		{
			return triLess(objectPolicy_.point(*object, splitAxis_),
				splitPosition_);
		}

	private:
		Real splitPosition_;
		integer splitAxis_;
		const ObjectPolicy& objectPolicy_;
	};

}

#endif
