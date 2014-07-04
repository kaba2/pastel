// Description: SplitRule concept

#ifndef PASTELGEOMETRY_SPLITRULE_CONCEPT_H
#define PASTELGEOMETRY_SPLITRULE_CONCEPT_H

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
			1) 0 <= splitAxis <= tree.n()
			2) minBound[splitAxis] <= splitPosition <= maxBound[splitAxis]
			3) The splitting plane has to split the node in such
			a way that both sides get points. This guarantees an O(n)
			upper bound for the number of nodes when refining the 
			PointKdTree.
			*/
			template <typename Settings, template <typename> class Customization>
			std::pair<typename Settings::Real, integer> operator()(
				const PointKdTree<Settings, Customization>& tree,
				const typename PointKdTree<Settings, Customization>::Cursor& cursor,
				const Vector<typename Settings:: Real, Settings::N>& minBound,
				const Vector<typename Settings::Real, Settings::N>& maxBound,
				integer depth) const;
		};

	}

}

#endif
