#ifndef PASTEL_POINTKDTREE_SPLITPREDICATE_H
#define PASTEL_POINTKDTREE_SPLITPREDICATE_H

#include "pastel/geometry/pointkdtree.hpp"

namespace Pastel
{

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::SplitPredicate_KdTree
	{
	public:
		SplitPredicate_KdTree(
			const Real& splitPosition,
			integer splitAxis,
			const Vector<N, Real>* splitDirection,
			const ObjectPolicy& objectPolicy)
			: splitPosition_(splitPosition)
			, splitAxis_(splitAxis)
			, objectPolicy_(objectPolicy)
		{
			// splitDirection is not used.
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

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::SplitPredicate_BspTree
	{
	public:
		SplitPredicate_BspTree(
			const Real& splitPosition,
			integer splitAxis,
			const Vector<N, Real>* splitDirection,
			const ObjectPolicy& objectPolicy)
			: splitPosition_(splitPosition)
			, splitDirection_(*splitDirection)
			, objectPolicy_(objectPolicy)
		{
			ASSERT(splitDirection);
			// splitAxis is not used.
		}
		
		TriState::Enum operator()(const ObjectInfo& object) const
		{
			return triLess(
				dot(asVector(objectPolicy_.point(*object)), splitDirection_), 
				splitPosition_);
		}

	private:
		Real splitPosition_;
		const Vector<N, Real>& splitDirection_;
		const ObjectPolicy& objectPolicy_;
	};

}

#endif
