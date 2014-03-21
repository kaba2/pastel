// Description: Concepts for the c-fast trie

#ifndef PASTELSYS_CFASTTRIE_CONCEPTS_H
#define PASTELSYS_CFASTTRIE_CONCEPTS_H

#include "pastel/sys/cfasttrie_fwd.h"

namespace Pastel
{

	namespace CFastTrie_Concepts
	{

		class Settings
		{
		public:
			//! The number of bits in the stored integers.
			/*!
			Preconditions:
			Bits > 0
			*/
			PASTEL_CONSTEXPR int Bits = UserDefinedInteger;

			//! The associated user-data.
			/*!
			Settings this to void avoids any memory
			use for the user-data.
			*/
			using Value = UserDefinedType;
		};

		template <typename Settings>
		class Customization
		{
		public:
			using Fwd = CFastTrie_Fwd<Settings>;
			
			PASTEL_FWD(Iterator);
			PASTEL_FWD(ConstIterator);

		/*
		The customization functions should be protected
		so that they can only be called by the CFastTrie
		implementation.
		*/
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
			void onErase(const Iterator& element) {}

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
