// Description: Skip list

#ifndef PASTELSYS_SKIPLIST_H
#define PASTELSYS_SKIPLIST_H

#include "pastel/sys/skiplist_concepts.h"
#include "pastel/sys/skiplist_node.h"
#include "pastel/sys/skiplist_iterator.h"
#include "pastel/sys/random_geometric.h"
#include "pastel/sys/lessthan.h"

#include <memory>

namespace Pastel
{

	template <typename Type_>
	class Default_SkipList_Settings
	{
	public:
		typedef Type_ Type;
		typedef LessThan Compare;
	};

	template <
		typename Type_, 
		typename Settings_ = Default_SkipList_Settings<Type_>>
	class SkipList
	{
	public:
		typedef Type_ Type;
		typedef Settings_ Settings;
		typedef typename Settings::Compare Compare;
		typedef typename AsClass<Type>::type Type_Class;

	private:
		typedef SkipList_::Node Node;
		typedef SkipList_::Data_Node<Type_Class> Data_Node;

	public:
		typedef SkipList_::Iterator<Node*, Type_Class> 
			Iterator;
		typedef SkipList_::Iterator<const Node*, Type_Class> 
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
		: sentinel_()
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
		: sentinel_()
		, levelRatio_(0.5)
		, size_(0)
		{
			initialize();

			try
			{
				for (const Type_Class& element : that)
				{
					insert(element);
				}
			}
			catch(...)
			{
				// The destructor will not be run,
				// so remember to clean up in case
				// of an exception.
				clear();
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
		: sentinel_()
		, levelRatio_(0.5)
		, size_(0)
		{
			initialize();

			swap(that);
		}

		//! Copy-assigns from another skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow

		The end() iterator will be preserved.
		*/
		SkipList& operator=(const SkipList& that)
		{
			if (this == &that)
			{
				return *this;
			}

			SkipList copy(that);
			swap(copy);

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
			integer n = sentinel_->size();
			for (integer i = 0;i < n;++i)
			{
				if (sentinel_->link<true>(i) != sentinel_.get())
				{
					// Splice the non-empty lists into this
					// skip-list.
					link(sentinel_.get(), that.sentinel_->link<true>(i), i);
					link(that.sentinel_->link<false>(i), sentinel_.get(), i);
					link(that.sentinel_.get(), that.sentinel_.get(), i);
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
			swap(sentinel_, that.sentinel_);
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
			Node* node = sentinel_->link<true>(0);
			Node* end = sentinel_.get();
			while(node != end)
			{
				Node* next = node->link<true>(0);
				delete node;
				node = next;
			}

			// Update the links in the sentinel node.
			integer n = sentinel_->size();
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
		Iterator insert(Type_Class that)
		{
			// Choose the number of levels in a node
			// as a geometrically-distributed random
			// number.
			integer levels = std::min(
					randomGeometric<real>(levelRatio_) + 1,
					maxLevels());

			// Find the predecessor node from the
			// linked list at each level.
			Node* end = sentinel_.get();
			Node* node = end;
			std::vector<Node*> beforeSet(levels);
			for (integer i = maxLevels() - 1;i >= 0;--i)
			{
				Node* next = node->link<true>(i);
				while (next != end && 
					Compare()(*(Data_Node*)next, that))
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
				new Data_Node(levels, std::move(that));

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

		//! Removes all elements equivalent to 'that'.
		/*!
		Time complexity:
		O(k)
		where
		k is the number of elements equivalent to 'that'.

		An element x is equivalent to element y if
		!(x < y) && !(y < x), where < is the comparison
		predicate.

		returns:
		The iterator following the last removed element.
		*/
		Iterator erase(Type_Class that)
		{
			// Note that we take 'that' by value;
			// a reference could point to one of
			// the nodes being removed.

			ConstIterator iter = lower_bound(that);

			// Remove all elements equivalent to 'that'.
			while(iter != cend() && 
				!Compare()(*iter, that) &&
				!Compare()(that, *iter))
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
		The iterator following 'that'.
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

		//! Returns the first element == 'that'.
		/*!
		Time complexity:
		O(log(size())) expected

		Exception safety:
		nothrow

		returns:
		An iterator to the element, if found,
		end(), otherwise.
		*/
		Iterator find(const Type_Class& that)
		{
			Iterator result = lower_bound(that);
			if (result == end() ||
				Compare()(that, *result))
			{
				return end();
			}

			return result;
		}

		//! Returns the first element equivalent to 'that'.
		/*!
		Time complexity:
		O(log(size())) expected

		Exception safety:
		nothrow

		returns:
		An iterator to the element, if found,
		cend(), otherwise.
		*/
		ConstIterator find(const Type_Class& that) const
		{
			ConstIterator result = lower_bound(that);
			if (result == cend() ||
				Compare()(that, *result))
			{
				return cend();
			}

			return result;
		}

		//! Returns the first element >= 'that'.
		/*!
		Time complexity:
		O(log(size())) expected

		Exception safety:
		nothrow
		*/
		Iterator lower_bound(const Type_Class& that)
		{
			return Iterator(nodeBound<true>(that));
		}

		//! Returns the first element >= 'that'.
		/*!
		Time complexity:
		O(log(size())) expected

		Exception safety:
		nothrow
		*/
		ConstIterator lower_bound(const Type_Class& that) const
		{
			return ConstIterator(nodeBound<true>(that));
		}

		//! Returns the first element > 'that'.
		/*!
		Time complexity:
		O(log(size())) expected

		Exception safety:
		nothrow
		*/
		Iterator upper_bound(const Type_Class& that)
		{
			return Iterator(nodeBound<false>(that));
		}

		//! Returns the first element > 'that'.
		/*!
		Time complexity:
		O(log(size())) expected

		Exception safety:
		nothrow
		*/
		ConstIterator upper_bound(const Type_Class& that) const
		{
			return ConstIterator(nodeBound<false>(that));
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
			return sentinel_->size();
		}

		//! Returns the level-ratio.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow

		The level-ratio is the success-probability of the
		geometric distribution from which the random number 
		of levels is drawn for each element.
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
			return Iterator(sentinel_->link<true>(0));
		}

		ConstIterator begin() const
		{
			return cbegin();
		}

		ConstIterator cbegin() const
		{
			return ConstIterator(sentinel_->link<true>(0));
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
			return Iterator(sentinel_.get());
		}

		ConstIterator end() const
		{
			return cend();
		}

		ConstIterator cend() const
		{
			return ConstIterator(sentinel_.get());
		}

		friend void print(const SkipList& list)
		{
			for (integer i = 0;i < list.maxLevels();++i)
			{
				Node* node = list.sentinel_.get();
				node = node->link_[i].next[1];
				while(node != list.sentinel_.get())
				{
					std::cout << *(Data_Node*)node << ", ";
					node = node->link<true>(i);
				}
				std::cout << std::endl;
			}
		}


	private:
		void initialize()
		{
			ASSERT(!sentinel_.get());
			const integer maxLevels = 4;

			sentinel_.reset(new Node(maxLevels));

			// Link the sentinel node to itself.
			Node* node = sentinel_.get();
			integer n = node->size();
			for (integer i = 0;i < n;++i)
			{
				link(node, node, i);
			}
		}

		const Type_Class& nodeData(Node* node) const
		{
			ASSERT(node != sentinel_.get());
			return (const Type_Class&)*(Data_Node*)node;
		}

		template <bool Direction>
		Node* nodeBound(
			const Type_Class& that) const
		{
			typedef SkipList_::Directed_Compare<Compare, Direction>
				Directed_Compare;

			Node* end = sentinel_.get();
			Node* node = end;
			integer n = node->size();
			for (integer i = n - 1;i >= 0;--i)
			{
				Node* next = node->link<Direction>(i);

				while (next != end && 
					Directed_Compare()(nodeData(next), that))
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

		std::unique_ptr<Node> sentinel_;
		real levelRatio_;
		integer size_;
	};

}

#endif
