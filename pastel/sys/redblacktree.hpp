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
		Iterator element = find(key);
		if (element != cend())
		{
			// The key already exists in the tree.
			return element;
		}

		// Create the new node.
		Node* newNode = 
			allocateNode(std::move(key), std::move(data), true);
		element = Iterator(newNode);

		// Attach the new node into the tree.
		root_ = insert(newNode, root_, sentinel_, true);

		// It is an invariant of the red-black tree that 
		// the root-node is black. Simply changing the color from
		// red to black in the root can not break any invariant:
		//
		// * The number of black nodes increments by one on _every_ path.
		// * It has no effect on the left-leaning property.
		// * It removes the possible last invariant violation of two 
		//   subsequent red nodes next to the root.
		root_->setBlack();

		// Notify the customization of this tree of the
		// insertion.
		this->onInsert(element);

		// Return the new element.
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
	auto RedBlackTree<Settings, Customization>::erase(
		const ConstIterator& that)
	-> Iterator
	{
		// Notify the customization of the tree.
		this->onErase(cast(that));
	
		// Detach the element from the tree.
		auto erasedAndSuccessor = erase((Node*)that.base());

		// Deallocate the element.
		delete erasedAndSuccessor.first;

		// Return the successor of the erased element.
		return Iterator(erasedAndSuccessor.second);
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::erase(const Key& key)
	-> Iterator
	{
		return erase(find(key));
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::splice(
		RedBlackTree& that,
		const ConstIterator& thatFrom)
	-> Iterator
	{
		ENSURE(thatFrom != that.cend());

		if (this == &that)
		{
			// Splicing inside the same tree does not
			// affect anything.
			return cast(thatFrom);
		}

		// Notify the customization of 'that' tree.
		that.onSpliceFrom(that.cast(thatFrom));

		// Detach the node from 'that' tree.
		Node* detached = that.erase((Node*)thatFrom.base()).first;
		Iterator element(detached);

		// Attach the new node into this tree.
		root_ = insert(detached, root_, sentinel_, true);

		// See the insert() function for this.
		root_->setBlack();

		// Notify the customization of this tree.
		this->onSplice(element);

		// Return an iterator to the new element.
		return element;
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
