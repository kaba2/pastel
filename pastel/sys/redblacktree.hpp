#ifndef PASTEL_REDBLACKTREE_HPP
#define PASTEL_REDBLACKTREE_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{
	template <typename Key, typename Compare, typename RbtPolicy>
	RedBlackTree<Key, Compare, RbtPolicy>::RedBlackTree(
		const RbtPolicy& policy,
		const Key& sentinelKey,
		const ValueType& sentinelValue)
		: root_(0)
		, sentinel_(0)
		, minimum_(0)
		, size_(0)
		, allocator_(sizeof(Node))
		, compare_()
		, policy_(policy)
	{
		allocateSentinel(sentinelKey, &sentinelValue);

		// This doesn't throw.
		initialize();
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	RedBlackTree<Key, Compare, RbtPolicy>::RedBlackTree(
		const RedBlackTree& that,
		const RbtPolicy& policy)
		: root_(0)
		, sentinel_(0)
		, minimum_(0)
		, size_(0)
		, allocator_(sizeof(Node))
		, compare_(that.compare_)
		, policy_(policy)
	{
		// We don't want to rollback this in case of exception,
		// because it gives a default-constructed state.
		allocateSentinel(that.sentinel_->key(), that.sentinel_->valuePtr());

		// This doesn't throw.
		initialize();

		try
		{
			// We could simply insert the values here
			// in order. However, because 'that' is already
			// a working tree, we can make the copy more efficient
			// by also copying its structure.

			root_ = copyConstruct(sentinel_, that.root_);
			ASSERT(root_->parent() == sentinel_);
		}
		catch(...)
		{
			// In case of an exception, 
			// the deallocations are done all at once.
			allocator_.clear();
			throw;
		}

		// Find the smallest node.
		{
			Node* node = root_;
			while(node->left() != sentinel_)
			{
				node = node->left();
			}

			setMinimum(node);
		}

		// Find the greatest node.
		{
			Node* node = root_;
			while(node->right() != sentinel_)
			{
				node = node->right();
			}

			setMaximum(node);
		}

		size_ = that.size_;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	RedBlackTree<Key, Compare, RbtPolicy>&
	RedBlackTree<Key, Compare, RbtPolicy>::operator=(
		const RedBlackTree& that)
	{
		if (this != &that)
		{
			RedBlackTree copy(that);
			swap(copy);
		}

		return *this;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	RedBlackTree<Key, Compare, RbtPolicy>::~RedBlackTree()
	{
		clear();
		deallocateSentinel();
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::swap(
		RedBlackTree& that)
	{
		using std::swap;
		swap(root_, that.root_);
		swap(sentinel_, that.sentinel_);
		swap(minimum_, that.minimum_);
		swap(size_, that.size_);
		allocator_.swap(that.allocator_);
		swap(compare_, that.compare_);
		policy_.swap(that.policy_);
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::clear()
	{
		clear(root_);

		// The memory of the nodes is deallocated
		// all at once. This is more efficient.
		allocator_.clear();

		initialize();
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	integer RedBlackTree<Key, Compare, RbtPolicy>::size() const
	{
		return size_;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	bool RedBlackTree<Key, Compare, RbtPolicy>::empty() const
	{
		return root_ == sentinel_;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Iterator 
	RedBlackTree<Key, Compare, RbtPolicy>::insert(
	const Key& key, const ValueType& value)
	{
		Node* newNode = sentinel_;
		root_ = insert(key, &value, root_, sentinel_, true, newNode);

		// It is an invariant of the red-black tree that 
		// the root-node is black. Simply changing the color from
		// red to black in the root can not break any invariant:
		//
		// * The number of black nodes increments by one on _every_ path.
		// * It has no effect on the left-leaning property.
		// * It removes the possible last invariant violation of two 
		//   subsequent red nodes next to the root.

		root_->setBlack();

		return Iterator(newNode);
	}
	
	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Iterator 
		RedBlackTree<Key, Compare, RbtPolicy>::erase(
		const ConstIterator& that)
	{
		return Iterator(erase(that.iter_.node_));
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Iterator 
		RedBlackTree<Key, Compare, RbtPolicy>::erase(
		const Key& key)
	{
		return erase(find(key));
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Iterator 
		RedBlackTree<Key, Compare, RbtPolicy>::find(const Key& key)
	{
		// Use the const-version to do the find.
		const ConstIterator iter = 
			((const RedBlackTree&)*this).find(key);

		// Cast constness away. This is ok, since
		// we own the data.
		Node* node = (Node*)iter.iter_.node_;

		// Return the iterator.
		return Iterator(node);
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::ConstIterator 
	RedBlackTree<Key, Compare, RbtPolicy>::find(const Key& key) const
	{
		Node* node = root_;
		while(node != sentinel_)
		{
			if (compare_(key, node->key()))
			{
				// Elements less than the value
				// in this node are on the left.
				node = node->left();
			}
			else if (compare_(node->key(), key))
			{
				// Elements greater than the value
				// in this node are on the right.
				node = node->right();
			}
			else
			{
				// The value in this node is equivalent
				// to the finded element. 
				// Return an iterator to it.
				return ConstIterator(node);
			}
		}

		return end();
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Iterator 
	RedBlackTree<Key, Compare, RbtPolicy>::begin()
	{
		return Iterator(minimum());
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::ConstIterator 
	RedBlackTree<Key, Compare, RbtPolicy>::begin() const
	{
		return ConstIterator(minimum());
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Iterator 
	RedBlackTree<Key, Compare, RbtPolicy>::end()
	{
		return Iterator(sentinel_);
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::ConstIterator 
	RedBlackTree<Key, Compare, RbtPolicy>::end() const
	{
		return ConstIterator(sentinel_);
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Iterator 
	RedBlackTree<Key, Compare, RbtPolicy>::last()
	{
		return Iterator(maximum());
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::ConstIterator 
	RedBlackTree<Key, Compare, RbtPolicy>::last() const
	{
		return ConstIterator(maximum());
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Iterator 
	RedBlackTree<Key, Compare, RbtPolicy>::root()
	{
		return Iterator(root_);
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::ConstIterator 
	RedBlackTree<Key, Compare, RbtPolicy>::root() const
	{
		return ConstIterator(root_);
	}

}

#endif
