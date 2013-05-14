#ifndef PASTELSYS_REDBLACKTREE_HPP
#define PASTELSYS_REDBLACKTREE_HPP

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/directed_predicate.h"

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
	{
		allocateSentinel(that.sentinel_->key(), (Data_Class&)*that.sentinel_);
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
	{
		allocateSentinel(that.sentinel_->key(), (Data_Class&)*that.sentinel_);
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
	auto RedBlackTree<Settings, Customization>::insert(
	Key key, Data_Class data)
	-> Iterator
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
	template <typename Key_ConstIterator_>
	void RedBlackTree<Settings, Customization>::insertMany(
		Key_ConstIterator_ begin, Key_ConstIterator_ end)
	{
		while(begin != end)
		{
			insert(*begin);
			++begin;
		}
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::erase(const ConstIterator& that)
	-> Iterator
	{
		this->onErase(that);

		return Iterator(erase((Node*)that.base()));
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::erase(const Key& key)
	-> Iterator
	{
		return erase(find(key));
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::cast(
		const ConstIterator& that)
	-> Iterator
	{
		return Iterator((Node*)that.base());
	}

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
		ConstIterator result = lower_bound(key);
		if (result != cend() &&
			Compare()(key, result.key()))
		{
			result = cend();
		}

		return result;
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::lower_bound(const Key& key)
	-> Iterator
	{
		return cast(((const RedBlackTree&)*this).lower_bound(key));
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::lower_bound(const Key& key) const
	-> ConstIterator
	{
		return bound<Right>(key);
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::upper_bound(const Key& key)
	-> Iterator
	{
		return cast(((const RedBlackTree&)*this).upper_bound(key));
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::upper_bound(const Key& key) const
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
			Node* nextNode = 0;
			if (Directed_Compare()(key, node->key()))
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
			else if (Directed_Compare()(node->key(), key))
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

}

#endif
