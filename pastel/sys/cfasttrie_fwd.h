// Description: Types for the c-fast trie

#ifndef PASTELSYS_CFASTTRIE_FWD_H
#define PASTELSYS_CFASTTRIE_FWD_H

#include "pastel/sys/integer.h"
#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/cfasttrie_iterator.h"
#include "pastel/sys/skiplist.h"

#include <list>
#include <map>
#include <memory>
#include <unordered_map>

namespace Pastel
{

	template <
		typename Settings,
		typename Customization>
	class CFastTrie;

	namespace CFastTrie_
	{

		template <typename CFastTrie_Settings>
		class Element;

		template <typename CFastTrie_Settings>
		class Fork;

		template <typename CFastTrie_Settings>
		class Chain;

		template <typename CFastTrie_Settings>
		class Bundle;

	}

	//! Types for the c-fast trie
	template <typename CFastTrie_Settings>
	class CFastTrie_Fwd
	{
	public:
		using Settings = CFastTrie_Settings;

		//! The number of bits in an element.
		PASTEL_CONSTEXPR int Bits = Settings::Bits;
		PASTEL_STATIC_ASSERT(Bits > 0);

		//! The type of a key.
		using Key = Unsigned_Integer<Bits>;

		//! The user-data to associate with the key.
		/*!
		Settings this to void avoids any memory use for
		the user-data.
		*/
		using Value = typename Settings::Value;
		using Value_Class = Class<Value>;

		//! Elements
		/*!
		The elements are stored in a doubly-linked list so
		that an element can be inserted to or removed from
		the list in constant time. In addition, this allows
		the elements to be accessed as a sequential range.
		The elements are stored in increasing order, which
		is essential for the c-fast trie to work. By a 
		property of the c-fast trie the associated chains
		are also in increasing order.
		*/
		using Element = CFastTrie_::Element<CFastTrie_Settings>;
		using DataSet = SkipList_Map<Key, Element>;
		using ConstIterator = typename DataSet::const_iterator;
		using Iterator = typename DataSet::iterator;
		using Range = boost::iterator_range<Iterator>;
		using ConstRange = boost::iterator_range<ConstIterator>;

		// These are for compatibility with Boost ranges.
		using const_iterator = ConstIterator;
		using iterator = Iterator;

		//! Access to keys instead of elements.
		using Key_ConstIterator = 
			CFastTrie_::Key_ConstIterator<ConstIterator, Key>;
		using Key_ConstRange = 
			boost::iterator_range<Key_ConstIterator>;

		//! Bundles
		using Bundle = CFastTrie_::Bundle<CFastTrie_Settings>;
		using BundlePtr = std::shared_ptr<Bundle>;
		using Bundle_WeakPtr = std::weak_ptr<Bundle>;

		//! Chains
		/*!
		The chains are stored in a doubly-linked list, because
		when a bundle is partioned, every chain in the bundle
		needs to be redirected to its new bundle.
		*/
		using Chain = CFastTrie_::Chain<CFastTrie_Settings>;
		using ChainSet = std::list<Chain>;
		using Chain_ConstIterator = typename ChainSet::const_iterator;
		using Chain_Iterator = typename ChainSet::iterator;
		using Chain_ConstRange = boost::iterator_range<Chain_ConstIterator>;

		//! Forks
		/*!
		When removing an element, it must be determined
		which fork has the element as its lowest ancestor.
		Storing the fork-chains by their chain-keys in 
		a map allows to solve this fast.
		*/
		using Fork = CFastTrie_::Fork<CFastTrie_Settings>;
		using ForkSet = std::map<Key, Fork>;
		using Fork_ConstIterator = 
			typename ForkSet::const_iterator;
		using Fork_Iterator = 
			typename ForkSet::iterator;

		//! The storage for the trie.
		/*!
		Each chain-key is paired to its chain. Using replicates,
		this encodes the whole compact x-fast trie formed by
		the chain-keys.
		*/
		using TrieSet = std::unordered_map<Key, Chain_Iterator>;
		using Trie_ConstIterator = typename TrieSet::const_iterator;
		using Trie_Iterator = typename TrieSet::iterator;
	};

}

#endif
