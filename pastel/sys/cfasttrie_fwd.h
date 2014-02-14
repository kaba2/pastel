// Description: Types for the c-fast trie

#ifndef PASTELSYS_CFASTTRIE_FWD_H
#define PASTELSYS_CFASTTRIE_FWD_H

#include "pastel/sys/integer.h"
#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/cfasttrie_chain.h"
#include "pastel/sys/cfasttrie_iterator.h"

#include <unordered_map>
#include <list>

namespace Pastel
{

	template <
		typename Settings,
		typename Customization>
	class CFastTrie;

	//! Types for the c-fast trie
	template <typename CFastTrie_Settings>
	class CFastTrie_Fwd
	{
	public:
		using Settings = CFastTrie_Settings;

		//! The number of bits in an element.
		enum {Bits = Settings::Bits};
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

		//! The hash function for the keys.
		/*!
		This is needed to store the chains in a hash table
		by their chain-keys.
		*/
		using Key_Hash = Integer_Hash<typename Key::Settings>;

		// Elements

		class Element;

		//! The storage for the elements.
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
		using DataSet = std::list<Element>;
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

		// Chains

		using Chain = CFastTrie_::Chain<Key, Iterator, ConstIterator>;

		//! The storage for chains.
		/*!
		The chains are stored in a hash-table, accessed by their
		chain-keys. Each chain corresponds to an element and vice
		versa.
		*/
		using ChainSet = std::unordered_map<Key, Chain, Key_Hash>;
		using Chain_ConstIterator = typename ChainSet::const_iterator;
		using Chain_Iterator = typename ChainSet::iterator;

		//! An element
		/*!
		An element stores the key, the potential user-data, and
		the corresponding chain. We make use of the empty base-class 
		optimization to avoid memory-use if Value = void.
		*/
		class Element
		: public Value_Class
		{
		public:
			Element(
				const Chain_Iterator& chain,
				Key key,
				Value_Class&& value)
			: Value_Class(std::move(value))
			, chain_(chain)
			, key_(key)
			{
			}

			//! Returns the chain corresponding to the element.
			const Chain_ConstIterator& chain() const
			{
				return chain_;
			}

			//! Returns the actual element.
			const Key& key() const
			{
				return key_;
			}

		private:
			template <
				typename Settings,
				typename Customization>
			friend class CFastTrie;

			Chain_Iterator chain_;
			Key key_;
		};

	};

}

#endif
