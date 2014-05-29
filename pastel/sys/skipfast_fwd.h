// Description: Types for the skip-fast trie

#ifndef PASTELSYS_SKIPFAST_FWD_H
#define PASTELSYS_SKIPFAST_FWD_H

#include "pastel/sys/class.h"
#include "pastel/sys/integer.h"
#include "pastel/sys/redblacktree.h"
#include "pastel/sys/redblackforest.h"

#include <unordered_map>

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	class SkipFast;

	namespace SkipFast_
	{

		template <typename>
		class Chain;

		template <typename, typename>
		class Iterator;

		template <typename Settings>
		class Chain_Customization
		: public Empty_RedBlackTree_Customization<Settings>
		{
		protected:
			using Fwd = RedBlackTree_Fwd<Settings>;

			PASTEL_FWD(Iterator);
			PASTEL_FWD(ConstIterator);
			PASTEL_FWD(Propagation_Class);

			Chain_Customization() {}

			void updatePropagation(
				const Iterator& element,
				Propagation_Class& propagation) 
			{
				propagation.nonEmpty = 
					!element->elementSet_.empty() ||
					element.left().propagation().nonEmpty ||
					element.right().propagation().nonEmpty;
			}

		private:
			Chain_Customization(const Chain_Customization& that) = delete;
			Chain_Customization(Chain_Customization&& that) = delete;
			Chain_Customization& operator=(Chain_Customization) = delete;
		};

	}

	//! Types for the skip-fast trie
	template <typename Settings_>
	class SkipFast_Fwd
	{
	public:
		// See skipfast_concepts.h for the definitions
		// of the types in the Settings.
		using Settings = Settings_;

		static PASTEL_CONSTEXPR integer Bits = Settings::Bits;
		PASTEL_STATIC_ASSERT(Bits > 0);

		using Key = Unsigned_Integer<Bits>;
		using Value = typename Settings::Value;
		using Value_Class = As_Class<Value>;

		class Chain_Propagation
		{
		public:
			Chain_Propagation()
			: nonEmpty(false)
			{				
			}

			bool nonEmpty;
		};

		using Chain = SkipFast_::Chain<Settings>;
		using Group = RedBlackTree_Set<Key, Chain, LessThan, 
			Chain_Propagation, void, false,
			SkipFast_::Chain_Customization>;

		using GroupSet = RedBlackForest_Set<Group>;
		using Group_ConstIterator = typename GroupSet::Tree_ConstIterator;
		using Group_Iterator = typename GroupSet::Tree_Iterator;
		using Group_ConstRange = boost::iterator_range<Group_ConstIterator>;
		using Group_Range = boost::iterator_range<Group_Iterator>;

		using Chain_ConstIterator = typename GroupSet::ConstIterator;
		using Chain_Iterator = typename GroupSet::Iterator;
		using Chain_ConstRange = boost::iterator_range<Chain_ConstIterator>;
		using Chain_Range = boost::iterator_range<Chain_Iterator>;
		using Chain_Iterator_Local = typename GroupSet::Element_Iterator;
		using Chain_ConstIterator_Local = typename GroupSet::Element_ConstIterator;

		class Element_SentinelData
		{
		public:
			Chain_Iterator_Local chain;
		};

		using ElementSet = RedBlackTree_Set<Key, Value, LessThan, void, Element_SentinelData>;
		using Element_ConstIterator = typename ElementSet::ConstIterator;
		using Element_Iterator = typename ElementSet::Iterator;

		using TrieSet = std::unordered_map<Key, Chain_Iterator>;
		using Trie_ConstIterator = typename TrieSet::const_iterator;
		using Trie_Iterator = typename TrieSet::iterator;

		using ConstIterator = SkipFast_::Iterator<Settings, Element_ConstIterator>;
		using Iterator = SkipFast_::Iterator<Settings, Element_Iterator>;
		using ConstRange = boost::iterator_range<ConstIterator>;
		using Range = boost::iterator_range<Iterator>;

		// These are for compatibility with Boost ranges.
		using const_iterator = ConstIterator;
		using iterator = Iterator;
	};

}

#endif
