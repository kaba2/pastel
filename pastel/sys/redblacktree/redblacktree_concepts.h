// Description: Red-black tree concepts

#ifndef PASTELSYS_REDBLACKTREE_CONCEPTS_H
#define PASTELSYS_REDBLACKTREE_CONCEPTS_H

#include "pastel/sys/redblacktree/redblacktree_fwd.h"
#include "pastel/sys/algebra/less_concept.h"

namespace Pastel
{

	namespace RedBlackTree_Concepts
	{

		//! Red-black tree settings
		class Settings
		{
		public:
			//! The type of the keys.
			/*!
			Precondition:
			Key != void

			The key is attached to every non-sentinel node,
			and can be read, but not modified, through an iterator.
			The key is used to order the data by the relation
			determined by Less.
			*/
			using Key = UserDefinedType;

			//! The type of the user data.
			/*!
			The user data is attached to every non-sentinel node,
			and can be read and modified through an iterator.

			Set to void to avoid allocating any	memory 
			for the user data.
			*/
			using Data = UserDefinedType;

			//! The type of the propagation data.
			/*!
			The propagation data is attached to every 
			node, including sentinel nodes.
			The propagation data can be read, but not modified,
			through an iterator. The propagation data
			is modified through the customization's
			updatePropagation() function, which computes
			the propagation data based on the subtree
			rooted at the given node.

			Set to void to avoid allocating any	memory 
			for the propagation data.
			*/
			using Propagation = UserDefinedType;

			//! The type of the sentinel user data.
			/*!
			The sentinel user data is attached to every sentinel node,
			and can be read and modified through a sentinel iterator.

			Set to void to avoid allocating any memory
			for the sentinel user data.
			*/
			using SentinelData = UserDefinedType;

			//! The type of the predicate to use for comparing keys.
			/*!
			The comparison is done as Less()(left, right).
			*/
			using Less = UserDefinedType;

			//! Whether to allow multiple equal keys.
			static constexpr bool MultipleKeys = UserDefinedBoolean;

			//! Whether to store user-data also in the sentinel nodes.
			static constexpr bool UserDataInSentinelNodes = UserDefinedBoolean;
		};

		//! Red-black tree customization
		template <typename Settings>
		class Customization
		{
		public:
			// Since the RedBlackTree inherits from its customization,
			// you can extend the public interface of the RedBlackTree 
			// by specifying public functions.

		protected:
			// The customization functions should be protected
			// so that they can only be called by the RedBlackTree
			// implementation.

			using Fwd = RedBlackTree_Fwd<Settings>;

			PASTEL_FWD(Iterator);
			PASTEL_FWD(ConstIterator);
			PASTEL_FWD(Propagation);

			//! Constructs an empty customization.
			/*!
			Exception safety: basic
			*/
			Customization() {}

			//! Called at the end of a constructor.
			/*!
			Exception safefy: basic
			*/
			void onConstruction() {};

			//! Swaps two customizations.
			/*!
			Time complexity: O(1)
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
			Exception safety: basic

			element:
			The element which was inserted.
			*/
			void onInsert(const Iterator& element) {}

			//! Called at the start of erase().
			/*!
			Exception safety: nothrow

			element:
			The element which is going to be removed.
			*/
			void onErase(const Iterator& element) {}

			//! Called at the start of that.splice().
			/*!
			Exception safety: nothrow

			element:
			The element which is going to be spliced 
			away from this tree.
			*/
			void onSpliceFrom(const Iterator& element) {}

			//! Called at the start of splice().
			/*!
			Exception safety: nothrow

			element:
			The element which is going to be spliced
			into this tree.
			*/
			void onSplice(const Iterator& element) {}

			//! Updates the propagation data in a node.
			/*!
			Exception safety: nothrow

			This function updates the propagation data for the
			subtree pointed to by 'node' under the assumption that 
			the propagation data in the subtree rooted at 'node'
			is up to date. It is called by the insert() and erase() 
			functions of the RedBlackTree whenever the structure of 
			the subtree has changed. The time complexities of 
			the insert() and erase() functions are multiplied by the 
			time complexity of this function. Thus, for example, to 
			retain O(log n) complexity for those functions, this 
			function must perform in O(1) (which is usually the case).

			element:
			The element which is to be updated.

			propagation:
			The propagation data of the 'element'. This is the 
			only way to get a mutable reference to the propagation 
			data. If you know that Propagation != void, you should 
			use Propagation for the reference for better readability.
			*/
			void updatePropagation(
				const Iterator& element,
				Propagation& propagation) {}

		private:
			// These functions will not be used, and so should
			// be deleted to avoid accidental splicing.
			Customization(const Customization& that) = delete;
			Customization(Customization&& that) = delete;
			Customization& operator=(Customization) = delete;

			// You can introduce customization state here.
		};

	}

}

#endif
