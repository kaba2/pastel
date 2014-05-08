// Description: Concepts for the red-black forest

#ifndef PASTELSYS_REDBLACKFOREST_CONCEPTS_H
#define PASTELSYS_REDBLACKFOREST_CONCEPTS_H

#include "pastel/sys/redblackforest_fwd.h"

namespace Pastel
{

	namespace RedBlackForest_Concepts
	{

		//! RedBlackForest_Settings concept
		class Settings
		{
		public:
			//! The red-black tree to use for the trees.
			/*!
			This type will be further customized by the red-black
			forest; the sentinel-data of the red-black trees stores
			the tree-iterator corresponding to the tree.
			*/
			using Tree = UserDefinedType;
		};

		//! RedBlackForest_Customization concept
		template <typename Settings>
		class Customization
		{
		public:
			// Since the RedBlackForest inherits from its customization,
			// you can extend the public interface of the RedBlackForest
			// by specifying public functions.

		protected:
			// The customization functions should be protected
			// so that they can only be called by the RedBlackForest
			// implementation.

			using Fwd = RedBlackForest_Fwd<Settings>;

			PASTEL_FWD(Tree_Iterator);
			PASTEL_FWD(Tree_ConstIterator);

			//! Constructs an empty customization.
			/*!
			Exception safety: basic
			*/
			Customization() {}

			//! Swaps two customizations.
			/*!
			Time complexity: O(1)
			Exception safefy: nothrow
			*/
			void swap(Customization& that) {}

			//! Called at the end of a constructor.
			/*!
			Exception safefy: basic
			*/
			void onConstruction() {};

			//! Called at the end of insertSet().
			/*!
			Exception safety: basic

			tree:
			The tree which was inserted.
			*/
			void onInsert(const Tree_Iterator& element) {}

			//! Called at the start of eraseSet().
			/*!
			Exception safety: nothrow

			tree:
			The tree which is going to be removed.
			*/
			void onErase(const Tree_Iterator& element) {}

			//! Called at the start of that.splice().
			/*!
			Exception safety: nothrow

			from:
			The tree which is going to be spliced 
			away from this tree.
			*/
			void onSpliceFrom(const Tree_Iterator& from) {}

			//! Called at the start of that.splice().
			/*!
			Exception safety: nothrow
			*/
			void onSpliceFrom() {}

			//! Called at the start of splice().
			/*!
			Exception safety: nothrow

			tree:
			The tree which is going to be spliced
			into this tree.
			*/
			void onSplice(const Tree_Iterator& tree) {}

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
