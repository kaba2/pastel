#ifndef PASTEL_REDBLACKTREE_CONCEPTS_H
#define PASTEL_REDBLACKTREE_CONCEPTS_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/redblacktree_fwd.h"

namespace Pastel
{

	namespace RedBlackTree_Concepts
	{

		template <
			typename Key,
			typename Compare,
			typename Data>
		class Customization
		{
		private:
			typedef RedBlackTree_Fwd<Key, Compare, Data> 
				Fwd;

			PASTEL_FWD(Iterator);
			PASTEL_FWD(ConstIterator);

		protected:
			//! Constructs an empty customization.
			/*!
			Exception safety: strong
			*/
			Customization() {}

			//! Swaps two customizations.
			/*!
			Time complexity: O(1).
			Exception safefy: nothrow
			*/
			void swap(Customization& that) {}

			//! Called at the start of clear().
			/*!
			Exception safety: nothrow
			*/
			void onClear() {}

			//! Called at the end of insert().
			/*!
			Exception safety: basic or stronger
			*/
			void onInsert(const Iterator& element) {}

			//! Called at the start of erase().
			/*!
			Exception safety: nothrow
			*/
			void onErase(const ConstIterator& element) {}

			//! Updates the hierarchical data in a node.
			/*!
			This function updates the hierarchical data for the
			subtree pointed to by 'iter' under the assumption that 
			the child subtrees are up to date. It is called by the
			insert() and erase() functions of the RedBlackTree
			whenever the structure of the subtree has changed.
			Note that the complexities of the insert() and erase()
			functions are multiplied by the complexity of this 
			function. Thus, for example, to retain O(log n) 
			complexity for those functions, this function must 
			perform in O(1) (which is usually the case when the
			hierarchical data is recursively defined).
			*/
			void updateHierarchical(const ConstIterator& node) {}

		private:
			// These functions will not be used, and so should
			// be deleted to avoid accidental splicing.
			Customization(const Customization& that) PASTEL_DELETE;
			Customization(Customization&& that) PASTEL_DELETE;
			Customization& operator=(Customization) PASTEL_DELETE;
		};

	}

}

#endif
