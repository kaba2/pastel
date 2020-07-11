// Description: Temporal kd-tree invariants
// Documentation: tdtree.txt

#ifndef PASTELGEOMETRY_TDTREE_INVARIANTS_H
#define PASTELGEOMETRY_TDTREE_INVARIANTS_H

#include "pastel/geometry/tdtree/tdtree.h"

namespace Pastel
{

	namespace TdTree_
	{

		template <
			typename Settings,
			template <typename> class Customization>	
		bool testInvariants(
			const TdTree<Settings, Customization>& tree,
			const typename TdTree<Settings, Customization>::Cursor& parent,
			bool right,
			const typename TdTree<Settings, Customization>::Cursor& node,
			integer depth)
		{
			using Tree = TdTree<Settings, Customization>;
			
			if (node == tree.endNode())
			{
				return true;
			}

			if (node.splitAxis() < 0 ||
				node.splitAxis() >= tree.dimension())
			{
				// The split-axis must be in the
				// range [0, d[, where d is the
				// dimension of the tree.
				return false;
			}

			if (node.min() > node.max() ||
				node.prevMin() > node.prevMax())
			{
				// Node-bound minimum must not
				// exceed node-bound maximum.
				return false;
			}

			if (node.prevMin() > node.min() ||
				node.prevMax() < node.max())
			{
				// The previous node-bounds must enclose the
				// current node-bounds.
				return false;
			}

			if (parent != tree.endNode() &&
				node.points() > parent.points())
			{
				// The number of points in a child-node
				// must not exceed that in the parent-node.
				return false;
			}

			// Recurse to children.

			if (!testInvariants(tree, node, false, node.left(), depth + 1))
			{
				return false;
			}

			if (!testInvariants(tree, node, true, node.right(), depth + 1))
			{
				return false;
			}

			return true;
		}

	}

	//! Returns whether the invariants hold for the temporal kd-tree.
	/*!
	Time complexity: O(n log(n))
	
	This function is useful only for testing; a correct implementation
	always returns true.
	*/
	template <
		typename Settings,
		template <typename> class Customization>	
	bool testInvariants(
		const TdTree<Settings, Customization>& tree)
	{
		if (tree.empty() != (tree.size() == 0))
		{
			// The tree is empty if and only if
			// the size is zero.
			return false;
		}

		if (tree.size() != tree.root().points())
		{
			// The number of points in the tree
			// must equal the number of points
			// in the root node.
			return false;
		}

		return TdTree_::testInvariants(tree, tree.endNode(), false, tree.root(), 0);
	}

}

#endif
