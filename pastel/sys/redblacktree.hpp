#ifndef PASTEL_REDBLACKTREE_HPP
#define PASTEL_REDBLACKTREE_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{
	template <typename Key, typename Compare, typename Data, typename Customization>
	RedBlackTree<Key, Compare, Data, Customization>::RedBlackTree(
		Key sentinelKey,
		Data_Class sentinelData)
		: root_(0)
		, sentinel_(0)
		, minimum_(0)
		, size_(0)
		, compare_()
	{
		allocateSentinel(std::move(sentinelKey), std::move(sentinelData));

		// This doesn't throw.
		initialize();
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	RedBlackTree<Key, Compare, Data, Customization>::RedBlackTree(
		RedBlackTree&& that)
		: root_(0)
		, sentinel_(0)
		, minimum_(0)
		, size_(0)
		, compare_()
	{
		swap(that);
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	RedBlackTree<Key, Compare, Data, Customization>&
	RedBlackTree<Key, Compare, Data, Customization>::operator=(
		RedBlackTree that)
	{
		swap(that);
		return *this;
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	RedBlackTree<Key, Compare, Data, Customization>::~RedBlackTree()
	{
		clear();
		deallocateSentinel();
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	void RedBlackTree<Key, Compare, Data, Customization>::swap(
		RedBlackTree& that)
	{
		using std::swap;
		Customization::swap(that);
		swap(root_, that.root_);
		swap(sentinel_, that.sentinel_);
		swap(minimum_, that.minimum_);
		swap(size_, that.size_);
		swap(compare_, that.compare_);
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	void RedBlackTree<Key, Compare, Data, Customization>::clear()
	{
		onClear();

		clear(root_);

		initialize();
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	integer RedBlackTree<Key, Compare, Data, Customization>::size() const
	{
		return size_;
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	bool RedBlackTree<Key, Compare, Data, Customization>::empty() const
	{
		return root_ == sentinel_;
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::Iterator 
	RedBlackTree<Key, Compare, Data, Customization>::insert(
	Key key, Data_Class data)
	{
		Node* newNode = sentinel_;
		root_ = insert(std::move(key), std::move(data), 
			root_, sentinel_, true, newNode);

		// It is an invariant of the red-black tree that 
		// the root-node is black. Simply changing the color from
		// red to black in the root can not break any invariant:
		//
		// * The number of black nodes increments by one on _every_ path.
		// * It has no effect on the left-leaning property.
		// * It removes the possible last invariant violation of two 
		//   subsequent red nodes next to the root.

		root_->setBlack();

		Iterator element(newNode);

		onInsert(element);
		
		return element;
	}
	
	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::Iterator 
		RedBlackTree<Key, Compare, Data, Customization>::erase(
		const ConstIterator& that)
	{
		onErase(that);

		return Iterator(erase(that.iter_.node_));
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::Iterator 
		RedBlackTree<Key, Compare, Data, Customization>::erase(
		const Key& key)
	{
		return erase(find(key));
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::Iterator 
		RedBlackTree<Key, Compare, Data, Customization>::find(const Key& key)
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

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::ConstIterator 
	RedBlackTree<Key, Compare, Data, Customization>::find(const Key& key) const
	{
		Node* node = root_;
		while(node != sentinel_)
		{
			if (compare_(key, node->key()))
			{
				// Elements less than the key
				// in this node are on the left.
				node = node->left();
			}
			else if (compare_(node->key(), key))
			{
				// Elements greater than the key
				// in this node are on the right.
				node = node->right();
			}
			else
			{
				// The key in this node is equivalent
				// to the finded element. 
				// Return an iterator to it.
				return ConstIterator(node);
			}
		}

		return end();
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::Iterator 
	RedBlackTree<Key, Compare, Data, Customization>::begin()
	{
		return Iterator(minimum());
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::ConstIterator 
	RedBlackTree<Key, Compare, Data, Customization>::begin() const
	{
		return ConstIterator(minimum());
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::Iterator 
	RedBlackTree<Key, Compare, Data, Customization>::end()
	{
		return Iterator(sentinel_);
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::ConstIterator 
	RedBlackTree<Key, Compare, Data, Customization>::end() const
	{
		return ConstIterator(sentinel_);
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::Iterator 
	RedBlackTree<Key, Compare, Data, Customization>::last()
	{
		return Iterator(maximum());
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::ConstIterator 
	RedBlackTree<Key, Compare, Data, Customization>::last() const
	{
		return ConstIterator(maximum());
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::Iterator 
	RedBlackTree<Key, Compare, Data, Customization>::root()
	{
		return Iterator(root_);
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::ConstIterator 
	RedBlackTree<Key, Compare, Data, Customization>::root() const
	{
		return ConstIterator(root_);
	}

}

#endif
