// Description: Concepts for the red-black tree

#ifndef PASTELSYS_REDBLACKTREE_CONCEPTS_H
#define PASTELSYS_REDBLACKTREE_CONCEPTS_H

#include "pastel/sys/redblacktree_fwd.h"

namespace Pastel
{

	namespace RedBlackTree_Concepts
	{

		//! RedBlackTree_Settings concept
		class Settings
		{
		public:
			//! The type of the keys.
			using Key = UserDefinedType;

			//! The type of the data.
			/*!
			Settings this to void avoids allocating any
			memory for the data.
			*/
			using Data = UserDefinedType;

			//! The type of the predicate to use for comparing keys.
			/*!
			The comparison is done as Compare()(left, right).
			*/
			using Compare = UserDefinedType;

			//! To which type iterators should dereference to.
			/*!
			RedBlackTree_Dereference_Default: 
			When Data != void, dereference to the Data. Otherwise
			dereference to the Key.

			RedBlackTree_Dereference_Key:
			Always dereference to the Key.

			RedBlackTree_Dereference_Data:
			Always dereference to the Data.
			*/
			enum
			{
				DereferenceType = RedBlackTree_Dereference_Default
			};

			//! Whether to allow multiple equal keys.
			enum {MultipleKeys = UserDefinedInteger};
		};

		//! RedBlackTree_Customization concept
		template <typename Settings>
		class Customization
		{
		private:
			using Fwd = RedBlackTree_Fwd<Settings>;
			
		/*
		The customization functions should be protected
		so that they can only be called by the RedBlackTree
		implementation.
		*/
		protected:
			PASTEL_FWD(Iterator);
			PASTEL_FWD(ConstIterator);

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
			void onErase(const Iterator& element) {}

			//! Called at the start of that.splice().
			/*!
			Exception safety: nothrow

			The element at the given position is going to
			be spliced to 'that' tree.
			*/
			void onSpliceFrom(const Iterator& element) {}

			//! Called at the start of splice().
			/*!
			Exception safety: nothrow

			The element at the give position was spliced 
			to this tree.
			*/
			void onSplice(const Iterator& element) {}

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
			void updateHierarchical(const Iterator& node) {}

		private:
			// These functions will not be used, and so should
			// be deleted to avoid accidental splicing.
			Customization(const Customization& that) = delete;
			Customization(Customization&& that) = delete;
			Customization& operator=(Customization) = delete;
		};

	}

}

#endif
