#ifndef PASTELSYS_REDBLACKTREE_HPP
#define PASTELSYS_REDBLACKTREE_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{
	template <typename Settings, typename Customization>
	RedBlackTree<Settings, Customization>::RedBlackTree(
		Key sentinelKey,
		Data_Class sentinelData)
		: root_(0)
		, sentinel_(0)
		, minimum_(0)
		, size_(0)
		, compare_()
	{
		allocateSentinel(std::move(sentinelKey), std::move(sentinelData));
		initialize();
	}

	template <typename Settings, typename Customization>
	RedBlackTree<Settings, Customization>::RedBlackTree(
		const RedBlackTree& that)
		: root_(0)
		, sentinel_(0)
		, minimum_(0)
		, size_(0)
		, compare_()
	{
		allocateSentinel(that.sentinel_->key(), *that.sentinel_);
		initialize();

		for (auto iter = that.cbegin();
			iter != that.cend();
			++iter)
		{
			insert(iter.key(), iter.data());
		}
	}

	template <typename Settings, typename Customization>
	RedBlackTree<Settings, Customization>::RedBlackTree(
		RedBlackTree&& that)
		: root_(0)
		, sentinel_(0)
		, minimum_(0)
		, size_(0)
		, compare_()
	{
		allocateSentinel(that.sentinel_->key(), *that.sentinel_);
		initialize();

		swap(that);
	}

	template <typename Settings, typename Customization>
	RedBlackTree<Settings, Customization>&
	RedBlackTree<Settings, Customization>::operator=(
		RedBlackTree that)
	{
		swap(that);
		return *this;
	}

	template <typename Settings, typename Customization>
	RedBlackTree<Settings, Customization>::~RedBlackTree()
	{
		clear();
		deallocateSentinel();
	}

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::swap(
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

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::clear()
	{
		this->onClear();

		clear(root_);

		initialize();
	}

	template <typename Settings, typename Customization>
	integer RedBlackTree<Settings, Customization>::size() const
	{
		return size_;
	}

	template <typename Settings, typename Customization>
	bool RedBlackTree<Settings, Customization>::empty() const
	{
		return root_ == sentinel_;
	}

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::Iterator 
	RedBlackTree<Settings, Customization>::insert(
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

		this->onInsert(element);
		
		return element;
	}
	
	template <typename Settings, typename Customization>
	template <typename Key_ConstIterator>
	void RedBlackTree<Settings, Customization>::insertMany(
		Key_ConstIterator begin,
		Key_ConstIterator end)
	{
		while(begin != end)
		{
			insert(*begin);
			++begin;
		}
	}

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::Iterator 
		RedBlackTree<Settings, Customization>::erase(
		const ConstIterator& that)
	{
		this->onErase(that);

		return Iterator(erase(that.iter_.node_));
	}

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::Iterator 
		RedBlackTree<Settings, Customization>::erase(
		const Key& key)
	{
		return erase(find(key));
	}

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::Iterator 
		RedBlackTree<Settings, Customization>::find(const Key& key)
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

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::ConstIterator 
	RedBlackTree<Settings, Customization>::find(const Key& key) const
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

		return cend();
	}

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::Iterator 
	RedBlackTree<Settings, Customization>::begin()
	{
		return Iterator(minimum());
	}

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::ConstIterator 
	RedBlackTree<Settings, Customization>::cbegin() const
	{
		return ConstIterator(minimum());
	}

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::Iterator 
	RedBlackTree<Settings, Customization>::end()
	{
		return Iterator(sentinel_);
	}

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::ConstIterator 
	RedBlackTree<Settings, Customization>::cend() const
	{
		return ConstIterator(sentinel_);
	}

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::Iterator 
	RedBlackTree<Settings, Customization>::last()
	{
		return Iterator(maximum());
	}

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::ConstIterator 
	RedBlackTree<Settings, Customization>::clast() const
	{
		return ConstIterator(maximum());
	}

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::Iterator 
	RedBlackTree<Settings, Customization>::root()
	{
		return Iterator(root_);
	}

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::ConstIterator 
	RedBlackTree<Settings, Customization>::croot() const
	{
		return ConstIterator(root_);
	}

}

#endif
