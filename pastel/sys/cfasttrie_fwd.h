// Description: Types for the c-fast trie

#ifndef PASTELSYS_CFASTTRIE_FWD_H
#define PASTELSYS_CFASTTRIE_FWD_H

#include "pastel/sys/integer.h"
#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/cfasttrie_iterator.h"
#include "pastel/sys/cfasttrie_customization.h"
#include "pastel/sys/redblacktree.h"

#include <list>
#include <map>
#include <memory>
#include <unordered_map>

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	class CFastTrie;

	namespace CFastTrie_
	{

		template <typename>
		class Element;

		template <typename>
		class Fork;

		template <typename>
		class Chain;

		template <typename>
		class Bundle;

	}

	//! Types for the c-fast trie
	template <typename Settings_>
	class CFastTrie_Fwd
	{
	public:
		// See cfasttrie_concepts.h for the definitions
		// of the types in the Settings.
		using Settings = Settings_;

		PASTEL_CONSTEXPR int Bits = Settings::Bits;
		PASTEL_STATIC_ASSERT(Bits > 0);

		using Key = Unsigned_Integer<Bits>;
		using Value = typename Settings::Value;
		using Value_Class = Class<Value>;
		
		class Propagation
		{
		public:
			Propagation()
			: minComplexity(infinity<integer>())
			{
			}

			integer minComplexity;
		};

		class SentinelData;

		//! Elements
		using Element = CFastTrie_::Element<Settings>;

		//! A bucket of elements.
		/*
		A bucket is a red-black tree, because then an element
		can be searched, inserted, or removed efficiently.
		In addition, a bucket can be split into two buckets,
		or two buckets merged into a single bucket efficiently.

		The progagation data is a boolean which tells whether
		the key is equal to some chain in the c-fast trie.
		This allows to efficiently find the next (previous) 
		element which does not have that property.

		The sentinel data stores links to neighboring
		red-black tree buckets. 

		The customization adds functions for linking the
		buckets together, as well providing a function for
		updating the propagation data.
		*/
		using DataSet = RedBlack_Map<Key, Element, 
			Pastel::LessThan, Propagation, SentinelData,
			CFastTrie_::Link_RedBlackTree_Customization>;

		using Element_ConstIterator = typename DataSet::const_iterator;
		using Element_Iterator = typename DataSet::iterator;

		using ConstIterator = CFastTrie_::Continuous_Iterator<Element_ConstIterator, Key>;
		using Iterator = CFastTrie_::Continuous_Iterator<Element_Iterator, Key>;
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
		/*!
		A bundle consists of an interval of chains, and
		of the elements that fall between the first chain
		and the last chain in this interval. The bundles
		are kept sufficiently small by splitting them.
		*/
		using Bundle = CFastTrie_::Bundle<Settings>;
		using BundleSet = std::list<Bundle>;
		using Bundle_ConstIterator = typename BundleSet::const_iterator;
		using Bundle_Iterator = typename BundleSet::iterator;
		using Bundle_ConstRange = boost::iterator_range<Bundle_ConstIterator>;
		using Bundle_Range = boost::iterator_range<Bundle_Iterator>;

		//! An ordered list of chains.
		/*!
		The chains are stored in a doubly-linked list. The
		list is maintained in increasing order in the associated 
		keys of the chains. The chain-list is needed because when a 
		bundle is partioned, every chain in the bundle needs to 
		be redirected to its new bundle. While this redirection takes
		linear time in the number of chains in the bundle, we
		amortize the cost by delaying the splitting of the bundles.
		*/
		using Chain = CFastTrie_::Chain<Settings>;
		using ChainSet = std::list<Chain>;
		using Chain_ConstIterator = typename ChainSet::const_iterator;
		using Chain_Iterator = typename ChainSet::iterator;
		using Chain_ConstRange = boost::iterator_range<Chain_ConstIterator>;
		using Chain_Range = boost::iterator_range<Chain_Iterator>;

		//! Forks
		/*!
		A fork is a pair of subsequent chains (l, r),
		where l is even and r is odd. When removing an element, 
		it must be determined which fork has the element as its 
		lowest ancestor. Storing the fork-chains by their 
		chain-keys in  a map allows to solve this fast.
		*/
		using Fork = CFastTrie_::Fork<Settings>;
		using ForkSet = std::map<Key, Fork>;
		using Fork_ConstIterator = 
			typename ForkSet::const_iterator;
		using Fork_Iterator = 
			typename ForkSet::iterator;

		//! The storage for the compact x-fast trie.
		/*!
		The compact x-fast trie is stored as a set of chains, where 
		each chain is a maximal set of nodes with equal replications. 
		Each chain is identified with the unique leaf node which is 
		part of the chain. Rather than mapping a leaf node key
		to a chain directly, we map it to a chain-iterator into a
		chain-list. See the documentation for the chain-list.
		*/
		using TrieSet = std::unordered_map<Key, Chain_Iterator>;
		using Trie_ConstIterator = typename TrieSet::const_iterator;
		using Trie_Iterator = typename TrieSet::iterator;

		class SentinelData
		{
		public:
			Bundle_Iterator bundle;
		};
	};

}

#endif
