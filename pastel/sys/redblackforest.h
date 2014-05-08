// Description: Red-black forest

#ifndef PASTELSYS_REDBLACKFOREST_H
#define PASTELSYS_REDBLACKFOREST_H

#include "pastel/sys/redblackforest_concepts.h"
#include "pastel/sys/redblackforest_fwd.h"
#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings>
	using Empty_RedBlackForest_Customization =
		RedBlackForest_Concepts::Customization<Settings>;

	template <
		typename Settings_, 
		template <typename> class Customization_ = Empty_RedBlackForest_Customization>
	class RedBlackForest
	{
	public:
		using Settings = Settings_;
		using Fwd = RedBlackForest_Fwd<Settings>;

		PASTEL_FWD(Tree);
		PASTEL_FWD(Element_Iterator);
		PASTEL_FWD(Element_ConstIterator);

		PASTEL_FWD(TreeSet);
		PASTEL_FWD(Tree_Iterator);
		PASTEL_FWD(Tree_ConstIterator);

		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Range);
		PASTEL_FWD(ConstRange);

		using Customization = Customization_<Settings>;

		//! Constructs an empty forest.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		RedBlackForest()
		: treeSet_()
		{
			// Create the first tree.
			insert(treeSet_.cend());
			// Create the last tree.
			insert(treeSet_.cend());
		}

		//! Copy-constructs from another forest.
		/*!
		Time complexity: O(that.trees() + that.size())
		Exception safety: strong
		*/
		RedBlackForest(const RedBlackForest& that)
		: RedBlackForest()
		{
			for (auto&& tree : that.ctreeRange())
			{
				treeSet_.emplace(ctreeEnd(), tree);
			}
		}

		//! Move-constructs from another forest.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		RedBlackForest(RedBlackForest&& that)
		: RedBlackForest()
		{
			swap(that);
		}
		
		//! Assigns from another forest.
		/*!
		Time complexity: O(that.trees() + that.size())
		Exception safety: strong
		*/
		RedBlackForest& operator=(RedBlackForest that)
		{
			swap(that);
			return *this;
		}

		//! Destructs a forest.
		/*!
		Time complexity: O(that.tree() + that.size())
		Exception safety: nothrow
		*/
		~RedBlackForest() = default;

		//! Removes all elements and trees.
		/*!
		Time complexity: O(trees() + size())
		Exception safety: nothrow
		*/
		void clear()
		{
			Tree_Iterator tree = ctreeBegin();
			Tree_Iterator end = ctreeEnd();
			while (tree != end)
			{
				tree = eraseSet(tree);
			}
		}

		//! Swaps two forests.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(RedBlackForest& that)
		{
			treeSet_.swap(that.treeSet_);
		}

		//! Inserts an empty tree at the end of the forest.
		/*!
		This is a convenience function which calls
		insertSet(ctreeEnd()).
		*/
		Tree_Iterator insert()
		{
			return insert(ctreeEnd());
		}

		//! Inserts an empty tree into the forest.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		Tree_Iterator insert(
			const Tree_ConstIterator& before)
		{
			Tree_Iterator tree = treeSet_.emplace(before);
			tree->end().sentinelData().tree = tree;
			return tree;
		}

		//! Removes a tree from the forest.
		/*!
		Time complexity: O(tree->size())
		Exception safety: nothrow
		*/
		Tree_Iterator erase(const Tree_ConstIterator& tree)
		{
			return treeSet_.erase(tree);
		}

		//! Finds the tree that contains a given element.
		/*!
		Time complexity: 
		O(log(n + 2))
		where
		n is the number of elements in the tree.

		Exception safety: nothrow
		*/
		Tree_ConstIterator find(
			const ConstIterator& element) const
		{
			while(!element.isSentinel())
			{
				element = element.parent();
			}

			return element.sentinelData().tree();
		}

		//! Finds the tree that contains a given element.
		/*!
		This is a convenience function which returns
		cast(addConst(*this).findSet(element));
		*/
		Tree_Iterator find(
			const ConstIterator& element)
		{
			return cast(addConst(*this).find(element));
		}

		//! Returns the number of trees in the forest.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer trees() const
		{
			return treeSet_.size() - 2;
		}

		//! Removes constness from an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return treeBegin()->cast(that);
		}

		//! Removes constness from a tree-iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Tree_Iterator cast(const Tree_ConstIterator& that)
		{
			return treeSet_.erase(that, that);
		}

		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Tree_, treeBegin, std::next(treeSet_.begin()));
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Tree_, treeEnd, std::prev(treeSet_.end()));
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Tree_, treeLast, std::prev(treeEnd()));

		PASTEL_ITERATOR_FUNCTIONS(begin, treeBegin()->begin());
		PASTEL_ITERATOR_FUNCTIONS(end, treeEnd()->end());
		PASTEL_ITERATOR_FUNCTIONS(last, treeEnd()->last());
		PASTEL_RANGE_FUNCTIONS(range, begin, end);

	private:
		TreeSet treeSet_;
	};

}

namespace Pastel
{

	template <typename RedBlackTree_>
	class RedBlackForest_Settings
	{
	public:
		using Tree = RedBlackTree_;
	};

}

namespace Pastel
{

	template <typename RedBlackTree_,
		template <typename> class Customization = Empty_RedBlackForest_Customization>
	using RedBlackForest_Set = 
		RedBlackForest<RedBlackForest_Settings<RedBlackTree_>, 
		Customization>;

}

#include "pastel/sys/redblackforest_invariants.hpp"

#endif
