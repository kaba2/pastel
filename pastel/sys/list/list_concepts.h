// Description: List concepts

#ifndef PASTELSYS_LIST_CONCEPTS_H
#define PASTELSYS_LIST_CONCEPTS_H

#include "pastel/sys/list/list_fwd.h"

namespace Pastel
{

	namespace List_Concepts
	{

		//! List settings
		class Settings
		{
		public:
			//! The user-data stored in a node.
			/*!
			Set to void to avoid allocating any memory
			for the user-data. This data can be accessed
			by dereferencing a list iterator. The user-data 
			is not stored in the end-node, unless 
			UserDataInEndNode is set to true. 
			*/
			using Data = UserDefinedType;

			//! Whether to store user-data also in the end-node.
			static constexpr bool UserDataInEndNode = UserDefinedBoolean;

			//! The data stored in the end node.
			/*!
			Set to void to avoid allocating any memory
			for this data. This data can be accessed through
			the endData() function of a list end-iterator.
			*/
			using EndData = UserDefinedType;
		};

		//! List customization
		template <typename Settings>
		class Customization
		{
		public:
			// Since the List inherits from its customization,
			// you can extend the public interface of the List
			// by specifying public functions.

		protected:
			// The customization functions should be protected
			// so that they can only be called by the List
			// implementation.

			using Fwd = List_Fwd<Settings>;

			PASTEL_FWD(Iterator);
			PASTEL_FWD(ConstIterator);

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
