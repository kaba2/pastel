// Description: Skip list

#ifndef PASTELSYS_SKIPLIST_H
#define PASTELSYS_SKIPLIST_H

#include "pastel/sys/skiplist_concepts.h"
#include "pastel/sys/skiplist_node.h"
#include "pastel/sys/skiplist_iterator.h"
#include "pastel/sys/random_geometric.h"
#include "pastel/sys/lessthan.h"
#include "pastel/sys/directed_predicate.h"

namespace Pastel
{

	class Default_SkipList_Settings
	{
	public:
		typedef LessThan Compare;
	};

	//! Skip list
	/*!
	Space complexity:
	O(size()) expected

	Key:
	The type of the key by which elements are ordered.

	Value:
	The type of the data associated with a given key. 
	Setting this to void avoids allocating any memory 
	for the associated data. In addition, while normally
	skip list iterators dereference to the value, 
	after setting Value to void iterators dereference 
	to the key.

	Settings:
	A type implementing the SkipList_Settings concept.
	*/
	template <
		typename Key_, 
		typename Value_ = void,
		typename Settings_ = Default_SkipList_Settings>
	class SkipList
	{
	public:
		typedef Key_ Key;
		typedef Value_ Value;
		typedef Settings_ Settings;
		typedef typename Settings::Compare Compare;
		typedef typename AsClass<Value>::type Value_Class;

	private:
		typedef SkipList_::Node Node;
		typedef SkipList_::Data_Node<Key, Value_Class> Data_Node;

	public:
		typedef SkipList_::Iterator<Node*, Key, Value_Class> 
			Iterator;
		typedef SkipList_::Iterator<const Node*, Key, Value_Class> 
			ConstIterator;

		// These are aliases for compatibility between boost ranges.
		typedef Iterator iterator;
		typedef ConstIterator const_iterator;

		//! Constructs a skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		strong
		*/
		SkipList()
		: end_(0)
		, levelRatio_(0.5)
		, size_(0)
		{
			initialize();
		}

		//! Copy-constructs a skip list.
		/*!
		Time complexity:
		O(that.size())

		Exception safety:
		strong
		*/
		SkipList(const SkipList& that)
		: end_(0)
		, levelRatio_(0.5)
		, size_(0)
		{
			initialize();

			try
			{
				for (auto iter = that.cbegin();iter != that.cend();++iter)
				{
					insert(iter.key(), iter.value());
				}
			}
			catch(...)
			{
				// The destructor will not be run,
				// so remember to clean up in case
				// of an exception.
				deinitialize();
			}
		}

		//! Move-constructs a skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		strong
		*/
		SkipList(SkipList&& that)
		: end_(0)
		, levelRatio_(0.5)
		, size_(0)
		{
			initialize();

			// We want to preserve the sentinel
			// node in 'key'. This is why we
			// don't use the usual swap() here.
			*this = std::move(that);
		}

		//! Destructs a skip list.
		/*!
		Time complexity:
		O(size())

		Exception safety:
		nothrow
		*/
		~SkipList()
		{
			deinitialize();
		}

		//! Copy-assigns from another skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		strong

		The end() iterator will be preserved.
		*/
		SkipList& operator=(const SkipList& that)
		{
			if (this == &that)
			{
				return *this;
			}

			// We want to preserve the end() iterator,
			// so we do not use swap().
			*this = std::move(SkipList(that));

			return *this;
		}

		//! Move-assigns from another skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow

		The end() iterator will be preserved.
		*/
		SkipList& operator=(SkipList&& that)
		{
			if (this == &that)
			{
				return *this;
			}

			// Delete the current contents.
			clear();

			// We want to preserve the sentinel node.
			// This is why we splice rather than swap().
			integer n = end_->size();
			for (integer i = 0;i < n;++i)
			{
				if (that.end_->link_[i].next[1] != that.end_)
				{
					// Splice the non-empty lists into this
					// skip-list.
					Node* thatNext = that.end_->link_[i].next[1];
					Node* thatPrev = that.end_->link_[i].next[0];;

					link(end_, thatNext, i);
					link(thatPrev, end_, i);
					link(that.end_, that.end_, i);
				}
			}

			size_ = that.size_;
			that.size_ = 0;

			return *this;
		}

		//! Swaps two skip lists.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		void swap(SkipList& that)
		{
			using std::swap;
			swap(end_, that.end_);
			swap(levelRatio_, that.levelRatio_);
			swap(size_, that.size_);
		}

		//! Removes all elements from the skip list.
		/*!
		Time complexity:
		O(size())
		*/
		void clear()
		{
			// Since every element will be removed,
			// there is no need to take care of the links.

			// Delete every node, except the sentinel node.
			Node* node = end_->link<true>(0);
			Node* end = end_;
			while(node != end)
			{
				Node* next = node->link<true>(0);
				delete node;
				node = next;
			}

			// Update the links in the sentinel node.
			integer n = end_->size();
			for (integer i = 0;i < n;++i)
			{
				link(end, end, i);
			}

			// Update the size.
			size_ = 0;
		}

		//! Inserts an element into the skip list.
		/*!
		Time complexity:
		O(log(size())) expected

		Exception safety:
		strong
		*/
		Iterator insert(Key key, Value_Class value = Value_Class())
		{
			// Choose the number of levels in a node
			// as a geometrically-distributed random
			// number.
			integer levels = std::min(
					randomGeometric<real>(levelRatio_) + 1,
					maxLevels());

			// Find the predecessor node from the
			// linked list at each level.
			Node* end = end_;
			Node* node = end;
			std::vector<Node*> beforeSet(levels);
			for (integer i = maxLevels() - 1;i >= 0;--i)
			{
				Node* next = node->link<true>(i);
				while (next != end && 
					Compare()(((Data_Node*)next)->key(), key))
				{
					node = next;
					next = node->link<true>(i);
				}

				if (i < levels)
				{
					beforeSet[i] = node;
				}
			}

			// Create a new node with the given number
			// of levels and the given data.
			Data_Node* newNode =
				new Data_Node(levels, std::move(key), std::move(value));

			for (integer i = 0;i < levels;++i)
			{
				// Link the new node into the linked list
				// at the i:th level.
				Node* next = beforeSet[i]->link<true>(i);
				Node* prev = beforeSet[i];

				link(newNode, next, i);
				link(prev, newNode, i);
			}

			// Update the size.
			++size_;

			// Return an iterator to the new node.
			return Iterator(newNode);
		}

		//! Removes all elements equivalent to 'key'.
		/*!
		Time complexity:
		O(k)
		where
		k is the number of elements equivalent to 'key'.

		An element x is equivalent to element y if
		!(x < y) && !(y < x), where < is the comparison
		predicate.

		returns:
		The iterator following the last removed element.
		*/
		Iterator erase(Key key)
		{
			// Note that we take 'key' by value;
			// a reference could point to one of
			// the nodes being removed.

			ConstIterator iter = lower_bound(key);

			// Remove all elements equivalent to 'key'.
			while(iter != cend() && 
				!Compare()(iter.key(), key) &&
				!Compare()(key, iter.key()))
			{
				iter = erase(iter);
			}

			return cast(iter);
		}

		//! Removes an element from the skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow

		returns:
		The iterator following 'key'.
		*/
		Iterator erase(const ConstIterator& that)
		{
			PENSURE(that != cend());

			Node* node = (Node*)that.base();

			Iterator result(node);
			++result;

			// Link the node off from the list.
			integer n = node->size();
			for (integer i = 0;i < n;++i)
			{
				Node* prev = node->link<false>(i);
				Node* next = node->link<true>(i);
				
				link(prev, next, i);
			}

			// Delete the node.
			delete node;
			--size_;

			// Return the next iterator.
			return result;
		}

		//! Casts a const-iterator to an iterator.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return Iterator((Node*)that.base());
		}

		//! Returns the first element == 'key'.
		/*!
		Time complexity:
		O(log(size())) expected

		Exception safety:
		nothrow

		returns:
		An iterator to the element, if found,
		end(), otherwise.
		*/
		Iterator find(const Key& key)
		{
			Iterator result = lower_bound(key);
			if (result == end() ||
				Compare()(key, result.key()))
			{
				return end();
			}

			return result;
		}

		//! Returns the first element equivalent to 'key'.
		/*!
		Time complexity:
		O(log(size())) expected

		Exception safety:
		nothrow

		returns:
		An iterator to the element, if found,
		cend(), otherwise.
		*/
		ConstIterator find(const Key& key) const
		{
			ConstIterator result = lower_bound(key);
			if (result == cend() ||
				Compare()(key, result->key()))
			{
				return cend();
			}

			return result;
		}

		//! Returns the first element >= 'key'.
		/*!
		Time complexity:
		O(log(size())) expected

		Exception safety:
		nothrow
		*/
		Iterator lower_bound(const Key& key)
		{
			return Iterator(nodeBound<true>(key));
		}

		//! Returns the first element >= 'key'.
		/*!
		Time complexity:
		O(log(size())) expected

		Exception safety:
		nothrow
		*/
		ConstIterator lower_bound(const Key& key) const
		{
			return ConstIterator(nodeBound<true>(key));
		}

		//! Returns the first element > 'key'.
		/*!
		Time complexity:
		O(log(size())) expected

		Exception safety:
		nothrow
		*/
		Iterator upper_bound(const Key& key)
		{
			return Iterator(nodeBound<false>(key));
		}

		//! Returns the first element > 'key'.
		/*!
		Time complexity:
		O(log(size())) expected

		Exception safety:
		nothrow
		*/
		ConstIterator upper_bound(const Key& key) const
		{
			return ConstIterator(nodeBound<false>(key));
		}

		//! Returns the number of elements in the skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		integer size() const
		{
			return size_;
		}

		//! Returns whether the skip list is empty.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		bool empty() const
		{
			return size_ == 0;
		}

		//! Returns the maximum number of levels in the skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow

		Note that this is not the maximum number of levels over
		the elements in the skip list. This is the number that
		limits the number of possible levels in a given element 
		from above.
		*/
		integer maxLevels() const
		{
			return end_->size();
		}

		//! Sets the level ratio.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow

		The level-ratio is the success-probability of the
		geometric distribution from which the random number 
		of levels is drawn for each element.
		*/
		void setLevelRatio(real levelRatio)
		{
			ENSURE_OP(levelRatio, >, 0);
			ENSURE_OP(levelRatio, <, 0);

			levelRatio_ = levelRatio;
		}

		//! Returns the level-ratio.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		real levelRatio() const
		{
			return levelRatio_;
		}

		//! Returns the first element in the skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		Iterator begin()
		{
			return Iterator(end_->link<true>(0));
		}

		ConstIterator begin() const
		{
			return cbegin();
		}

		ConstIterator cbegin() const
		{
			return ConstIterator(end_->link<true>(0));
		}

		//! Returns the end-iterator of the skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		Iterator end()
		{
			return Iterator(end_);
		}

		ConstIterator end() const
		{
			return cend();
		}

		ConstIterator cend() const
		{
			return ConstIterator(end_);
		}

		friend void print(const SkipList& list)
		{
			for (integer i = 0;i < list.maxLevels();++i)
			{
				Node* node = list.end_;
				node = node->link_[i].next[1];
				while(node != list.end_)
				{
					std::cout << ((Data_Node*)node)->key() << ", ";
					node = node->link<true>(i);
				}
				std::cout << std::endl;
			}
		}


	private:
		void initialize()
		{
			ASSERT(!end_);
			const integer maxLevels = 64;

			// Create the sentinel node.
			end_ = new Node(maxLevels);

			// Link the sentinel node to itself.
			Node* node = end_;
			integer n = node->size();
			for (integer i = 0;i < n;++i)
			{
				link(node, node, i);
			}
		}

		void deinitialize()
		{
			clear();
			delete end_;
		}

		const Key& nodeKey(Node* node) const
		{
			ASSERT(node != end_);
			return ((Data_Node*)node)->key();
		}

		template <bool Direction>
		Node* nodeBound(const Key& key) const
		{
			typedef Directed_Predicate<Compare, Direction>
				Directed_Compare;

			Node* end = end_;
			Node* node = end;
			integer n = node->size();
			for (integer i = n - 1;i >= 0;--i)
			{
				Node* next = node->link<Direction>(i);

				while (next != end && 
					Directed_Compare()(nodeKey(next), key))
				{
					node = next;
					next = node->link<Direction>(i);
				}
			}
			
			if (Direction)
			{
				node = node->link<Direction>(0);
			}

			return node;
		}

		void link(Node* left, Node* right, integer i)
		{
			left->link<true>(i) = right;
			right->link<false>(i) = left;
		}

		Node* end_;
		real levelRatio_;
		integer size_;
	};

}

#endif
