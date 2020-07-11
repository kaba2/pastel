// Description: Range tree invariants
// Documentation: rangetree.txt

#ifndef PASTELGEOMETRY_RANGETREE_INVARIANTS_H
#define PASTELGEOMETRY_RANGETREE_INVARIANTS_H

#include "pastel/geometry/rangetree/rangetree.h"

namespace Pastel
{

	namespace RangeTree_
	{

		template <
			typename Settings,
			template <typename> class Customization>	
		bool testInvariants(
			const RangeTree<Settings, Customization>& tree,
			const typename RangeTree<Settings, Customization>::Node_ConstIterator& parent,
			bool right,
			const typename RangeTree<Settings, Customization>::Node_ConstIterator& node,
			integer depth)
		{
			using Tree = RangeTree<Settings, Customization>;
			
			using Fwd = Tree;
			PASTEL_FWD(Node_ConstIterator);
			PASTEL_FWD(MultiLess);
			PASTEL_FWD(Entry);
			PASTEL_FWD(Point);

			if (!node)
			{
				// No node reference is null.
				return false;
			}

			if (node->isEnd())
			{
				return true;
			}

			MultiLess multiLess;

			if (node->isBottom())
			{
				auto xLess = [&](const Point& left, const Point& right)
				{
					return multiLess(left, right, tree.orders() - 1);
				};

				for (integer i = 0;i < node->entries() - 1;++i)
				{
					if (xLess(*node->entryRange()[i + 1].point(), *node->entryRange()[i].point()))
					{
						// The points must always be sorted with respect
						// to the last order.
						return false;
					}
				}

				if (parent)
				{
					// Check the fractional cascading information.

					if (parent->entryRange()[parent->entries()].cascade(right) !=
						node->entries())
					{
						// The last fractional cascading information must
						// point to the last entry of the child.
						return false;
					}

					for (integer i = 0;i < parent->entries();++i)
					{
						const Entry& entry = parent->entryRange()[i];
						integer cascade = entry.cascade(right);
						if (cascade < 0 ||
							cascade > node->entries())
						{
							// The parent node's cascading links must
							// be in the range of the child's entry-set.
							return false;
						}

						if (cascade < node->entries())
						{
							const Entry& childEntry = node->entryRange()[cascade];
							if (xLess(*childEntry.point(), *entry.point()))
							{
								// The fractional cascading link in the parent node 
								// points to an entry >= in the child node.
								return false;
							}
						}

						if (cascade > 0)
						{
							const Entry& prevChildEntry = node->entryRange()[cascade - 1];
							if (!xLess(*prevChildEntry.point(), *entry.point()))
							{
								// The fractional cascading link in the parent node 
								// points to the _smallest_ entry >= in the child node.
								return false;
							}
						}
					}
				}
			}
			else
			{
				// Recurse down.

				if (!testInvariants(tree, nullptr, false, node->down(), depth + 1))
				{
					return false;
				}
			}

			// Recurse to children.

			if (!testInvariants(tree, node, false, node->child(false), depth))
			{
				return false;
			}

			if (!testInvariants(tree, node, true, node->child(true), depth))
			{
				return false;
			}

			return true;
		}
	
	}

	//! Returns whether the invariants hold for the range tree.
	/*!
	Time complexity: O(n log(n))
	
	This function is useful only for testing; a correct implementation
	always returns true.
	*/
	template <
		typename Settings,
		template <typename> class Customization>	
	bool testInvariants(
		const RangeTree<Settings, Customization>& tree)
	{
		if (tree.empty() != (tree.size() == 0))
		{
			// The tree is empty if and only if
			// the size is zero.
			return false;
		}

		return RangeTree_::testInvariants(tree, nullptr, false, tree.root(), 0);
	}

}

#endif
