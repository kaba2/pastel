// Description: SplitRule concept

#ifndef PASTEL_SPLITRULE_CONCEPT_H
#define PASTEL_SPLITRULE_CONCEPT_H

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	namespace SplitRule_Concept
	{

		class SplitRule
		{
		public:
			//! Returns the splitting plane for a PointKdTree node.
			/*!
			tree:
			The kd-tree in which the splitting takes place.

			cursor:
			The node of the 'tree' to determine a splitting 
			plane for.

			minBound, maxBound:
			The minimum and maximum vertices of the node
			pointed to by the 'cursor'.

			Returns:
			The first element of the returned pair contains
			the index of the standard basis axis which is to be
			the normal of the splitting plane.
			The second element of the returned pair contains
			the position of the splitting plane on the splitting axis.

			Postconditions:
			1) 0 <= splitAxis <= tree.dimension()
			2) minBound[splitAxis] <= splitPosition <= maxBound[splitAxis]
			3) The splitting plane has to split the node in such
			a way that both sides get points. This guarantees an O(n)
			upper bound for the number of nodes when refining the 
			PointKdTree.
			*/
			template <typename Real, int N,
				typename PointPolicy>
				std::pair<Real, integer> operator()(
				const PointKdTree<Real, N, PointPolicy>& tree,
				const typename PointKdTree<Real, N, PointPolicy>::Cursor& cursor,
				const Vector<Real, N>& minBound,
				const Vector<Real, N>& maxBound,
				integer depth) const;
		};

	}

}

#endif
