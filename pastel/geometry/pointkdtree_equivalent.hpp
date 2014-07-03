#ifndef PASTELGEOMETRY_POINTKDTREE_EQUIVALENT_HPP
#define PASTELGEOMETRY_POINTKDTREE_EQUIVALENT_HPP

#include "pastel/geometry/pointkdtree_equivalent.h"

namespace Pastel
{

	namespace Equivalent_PointKdTree_
	{

		template <typename A_Cursor, 
			typename B_Cursor>
			bool equivalent(
			const A_Cursor& aTree,
			const B_Cursor& bTree)
		{
			if (aTree.leaf() != bTree.leaf())
			{
				return false;
			}

			if (aTree.leaf())
			{
				if (aTree.points() != bTree.points())
				{
					return false;
				}
			}
			else
			{
				if (aTree.min() != bTree.min() ||
					aTree.max() != bTree.max() ||
					aTree.splitAxis() != bTree.splitAxis() ||
					aTree.splitPosition() != bTree.splitPosition())
				{
					return false;
				}

				if (!equivalent(aTree.left(), bTree.left()))
				{
					return false;
				}
				if (!equivalent(aTree.right(), bTree.right()))
				{
					return false;
				}
			}			

			return true;
		}

	}
	template <int N_A, typename Real, typename PointPolicy_A, 
		int N_B, typename PointPolicy_B>
	bool equivalent(
		const PointKdTree<Real, N_A, PointPolicy_A>& aTree,
		const PointKdTree<Real, N_B, PointPolicy_B>& bTree)
	{
		if (aTree.nodes() != bTree.nodes() ||
			aTree.points() != bTree.points() ||
			aTree.leaves() != bTree.leaves() ||
			aTree.n() != bTree.n())
		{
			return false;
		}

		return Equivalent_PointKdTree_::equivalent(
			aTree.root(), bTree.root());
	}
}

#endif
