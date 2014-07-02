#ifndef PASTELGEOMETRY_RANGETREE_INVARIANTS_HPP
#define PASTELGEOMETRY_RANGETREE_INVARIANTS_HPP

#include "pastel/geometry/rangetree_invariants.h"

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
				if (parent)
				{
					// Check the fractional cascading information.

					if (parent->entryRange()[parent->entries()].cascade(right) !=
						node->entries())
					{
						// The last fractional cascading information must
						// to the last entry of the child.
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
							if (multiLess(*childEntry.point(), *entry.point(), depth + 1))
							{
								// The fractional cascading link in the parent node 
								// points to an entry >= in the child node.
								return false;
							}
						}

						if (cascade > 0)
						{
							const Entry& prevChildEntry = node->entryRange()[cascade - 1];
							if (!multiLess(*prevChildEntry.point(), *entry.point(), depth + 1))
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
