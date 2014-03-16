#ifndef PASTELSYS_REDBLACKTREE_SEARCH_HPP
#define PASTELSYS_REDBLACKTREE_SEARCH_HPP

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/directed_predicate.h"

namespace Pastel
{

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::find(const Key& key)
	-> Iterator
	{
		return cast(((const RedBlackTree&)*this).find(key));
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::find(const Key& key) const
	-> ConstIterator
	{
		ConstIterator result = lowerBound(key);
		if (result != cend() &&
			Compare()(key, result.key()))
		{
			result = cend();
		}

		return result;
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::lowerBound(const Key& key)
	-> Iterator
	{
		return cast(((const RedBlackTree&)*this).lowerBound(key));
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::lowerBound(const Key& key) const
	-> ConstIterator
	{
		return bound<Right>(key);
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::upperBound(const Key& key)
	-> Iterator
	{
		return cast(((const RedBlackTree&)*this).upperBound(key));
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::upperBound(const Key& key) const
	-> ConstIterator
	{
		return bound<Left>(key);
	}

	template <typename Settings, typename Customization>
	template <bool Direction>
	auto RedBlackTree<Settings, Customization>::bound(const Key& key) const
	-> ConstIterator
	{
		using Directed_Compare = 
			Directed_Predicate<Compare, Direction>;

		ConstIterator result = cend();

		Node* node = root_;
		while(node != sentinel_)
		{
			const Key& nodeKey = ((Data_Node*)node)->key();

			Node* nextNode = 0;
			if (Directed_Compare()(key, nodeKey))
			{
				// Elements less than the key
				// in this node are on the left.
				nextNode = node->child(!Direction);
				if (nextNode == sentinel_)
				{
					// The searched element does not
					// exist in the tree; the current 
					// node is the successor of 'key'.
					result = ConstIterator(node);
					break;
				}
			}
			else if (Directed_Compare()(nodeKey, key))
			{
				// Elements greater than the key
				// in this node are on the right.
				nextNode = node->child(Direction);
				if (nextNode == sentinel_)
				{
					// The searched element does not
					// exist in the tree; the next node
					// in the in-order traversal is the
					// successor of 'key'.
					result = ConstIterator(node);
					if (Direction)
					{
						++result;
					}
					else
					{
						--result;
					}
				}
			}
			else
			{
				// The key in this node is equivalent
				// to the searched element. 
				// Return an iterator to it.
				result = ConstIterator(node);
				break;
			}

			node = nextNode;
		}

		if (!Direction)
		{
			if (result == cend())
			{
				result = cbegin();
			}
			else if (!Compare()(key, result.key()))
			{
				++result;
			}
		}

		return result;
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::findInsertParent(
		const Key& key) const
	-> std::pair<Node*, bool>
	{
		// Find the place to insert the new element.
		Node* parent = sentinel_;
		Node* child = root_;

		// If the tree is empty, then any element should be
		// inserted as the left child of the sentinel node.
		bool rightChild = false;
		while(!child->isSentinel())
		{
			parent = child;
			rightChild = !Compare()(key, ((Data_Node*)parent)->key());
			// If the node key is equivalent
			// to the searched key, then we will go right,
			// so as to place the new element at the
			// end of the equivalent range of elements.
			child = parent->child(rightChild);
		}

		return std::make_pair(parent, rightChild);
	}

}

#endif
