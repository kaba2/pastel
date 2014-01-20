// Description: Compact x-fast trie

#ifndef PASTELSYS_XFASTTRIE_H
#define PASTELSYS_XFASTTRIE_H

#include "pastel/sys/xfasttrie_concepts.h"
#include "pastel/sys/xfasttrie_chain.h"
#include "pastel/sys/integer.h"
#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/skiplist.h"
#include "pastel/sys/exponential_binary_search.h"
#include "pastel/sys/bitmask.h"

#include <unordered_map>
#include <vector>
#include <memory>

namespace Pastel
{

	//! Compact x-fast trie
	template <typename XFastTrie_Settings>
	class XFastTrie
	{
	public:
		using Settings = XFastTrie_Settings;

		enum
		{
			Bits = Settings::Bits
		};
		PASTEL_STATIC_ASSERT(Bits > 0);

		using Key = Integer<Bits>;
		using Value = typename Settings::Value;
		using Value_Class = Class<Value>;

		class Element
		: public Value_Class
		{
		public:
			Element(Key key, Value_Class&& value)
			: Value_Class(std::move(value))
			, key_(key)
			{
			}

			const Key& key() const
			{
				return key_;
			}

			Value_Class& value() const
			{
				return (Value_Class&)*this;
			}

		private:
			Key key_;
		};

		using Key_Hash = Integer_Hash<Bits>;

		using DataSet = std::list<Element>;
		using ConstIterator = typename DataSet::const_iterator;
		using Iterator = typename DataSet::iterator;

		// These are for compatibility with Boost ranges.
		using const_iterator = ConstIterator;
		using iterator = Iterator;

		using Chain = XFastTrie_::Chain<Key, Iterator>;

		using ChainSet = std::unordered_map<Key, Chain, Key_Hash>
		using Chain_ConstIterator = typename ChainSet::const_iterator;
		using Chain_Iterator = typename ChainSet::iterator;

		//! Constructs an empty trie.
		/*!
		Time complexity: O(endBit - beginBit)
		Exception safety: strong
		*/
		explicit XFastTrie(
			integer beginBit = 0, 
			integer endBit = Bits)
		: beginBit_(beginBit)
		, endBit_(endBit)
		, chainSet_()
		, dataSet_()
		{
			ENSURE_OP(0, <=, beginBit);
			ENSURE_OP(beginBit, <, endBit);
			ENSURE_OP(endBit, <=, Bits);

			// Bounding the height of the skip-list
			// allows the skip-list operations to be
			// performed in log(bits()) time, provided
			// that we can provide the operations  
			// start-positions which are correspondingly 
			// close. This is essential for the
			// y-fast trie to achieve its performance
			// guarantees.
			dataSet_.setMaxHeight(bits());

			/*
			Consider the following x-fast trie on
			2-bit integers.

			          -
			       /     \
			      0       1
	    		 / \     / \
	    	   00   01 10   11

	    	Then bits() = 2, and the height of the
	    	node tree is bits() + 1. This is
	    	also the general formula. 
			*/

			// Create the hash tables with the desired hash functions.
			// By using localized hash functions we get both better
			// performance and cleaner code, since we do not need to
			// extract the suffixes to search in a hash table.
			nodeSetSet_.reserve(bits() + 1);
			for (integer i = 0;i < bits() + 1;++i)
			{
				nodeSetSet_.emplace_back(0, 
					Key_Hash(beginBit_ + i, endBit_));
			}

			// Create the root node.
			ChainSet& nodeSet = nodeSetSet_[bits()];
			nodeSet.insert(Key(), Chain());

			// Create the empty bucket stored at the root.
			endBucket_ = BucketPtr(new Bucket(
				dataSet_.end(), dataSet_.end()));

			// Assign the empty bucket at the root node.
			nodeSet.begin()->second.setLeaf(std::move(endBucket_));
		}

		//! Destructs the trie.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow
		*/
		~XFastTrie()
		{
			clear();
		}

		//! Swaps two tries.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(XFastTrie& that)
		{
			using std::swap;

			swap(beginBit_, that.beginBit_);
			swap(endBit_, that.endBit_);
			nodeSetSet_.swap(nodeSetSet_);
			dataSet_.swap(that.dataSet_);
			endBucket_.swap(that.endBucket_);
		}

		//! Removes all elements.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow
		*/
		void clear()
		{
			// Clear all elements.
			dataSet_.clear();

			// Clear all nodes, except the root.
			for (integer i = 0;i < bits();++i)
			{
				nodeSetSet_[i].clear();
			}

			// Reset the root node.
			Iterator root = nodeSetSet_.front().begin()->second;
			root->second.setLeaf(std::move(endBucket_));
		}

		//! Inserts an element.
		/*!
		Time complexity: O(bits())
		Exception safety: strong

		returns:
		An iterator to the inserted element, if it did not
		exist before, or an iterator to the existing element,
		otherwise.
		*/
		Iterator insert(
			Key key, 
			Value_Class value = Value_Class())
		{
			Key actualKey(key, beginBit_, endBit_);

			// See if the key already exists.
			{
				Chain_Iterator node = nodeSetSet_.front().find(actualKey);
				if (node != nodeSetSet_.front().end())
				{
					return node->second.element();
				}
			}

			Iterator keyUpperbound = upperBound(key);

			std::vector<Chain_Iterator> nodeSet;
			nodeSet.reserve(bits());

			integer newChains = 0;
			Iterator element;

			integer rollback = 0;
			try
			{
				// Create the nodes to the key.
				for (integer i = beginBit_;i < endBit_;++i)
				{
					auto result = nodeSetSet_[i - beginBit_].insert(
						std::make_pair(Key(key, i, endBit_), Chain()));
					
					Chain_Iterator iter = result.first;
					bool alreadyThere = !result.second;

					Chain* node = &iter->second;

					if (!nodeSet.empty() && node->shortcut() == 0)
					{
						Chain* leaf = &nodeSet.front()->second;
						node->shortcut() = leaf;
					}
				
					nodeSet.push_back(iter);

					if (!alreadyThere)
					{
						++newChains;
					}
				}
				++rollback;

				// Insert the element.
				element = dataSet_.emplace(keyUpperBound, 
					actualKey, std::move(value));
			}
			catch(...)
			{
				for (integer i = 0;i < newChains;++i)
				{
					nodeSetSet_[i].erase(nodeSet[i]);
				}

				throw;
			};

			nodeSet.front()->element() = element;

			return element;
		}

		//! Returns an element with a given key.
		/*!
		Time complexity: 
		O(log(log(Delta))),
		where
		Delta is the distance to the nearest stored
		neighbor of 'key'.

		Exception safety:
		nothrow

		returns:
		An iterator to the element, if the key exists,
		cend(), otherwise.
		*/
		ConstIterator find(const Key& key) const
		{
			ConstIterator iter = lowerBound(key);
			if (key < iter->key())
			{
				return cend();
			}

			return iter;
		}

		Iterator find(const Key& key)
		{
			return cast(removeConst(*this).find(key));
		}

		Chain_ConstIterator findChain(const Key& key, integer level) const
		{
			return chainSet_.find(replicate(key, level));
		}

		bool keyExists(const Key& key, integer level) const
		{
			return chainSet_.count(replicate(key, level)) != 0;
		}

		//! Returns an iterator to the smallest element > 'key'.
		/*!
		Time complexity: 
		O(log(log(Delta + 4))),
		where
		Delta is the distance between the searched key
		and the key of the returned element.

		Exception safety:
		nothrow

		returns:
		An iterator to the smallest element 'u' such that 
		'u > key'. The 'cend()' is considered to have a key 
		larger than any stored key.
		*/
		ConstIterator upperBound(const Key& key) const
		{
			ConstIterator lower = lowerBound(key);
			if (lower == cend())
			{
				// If the lower-bound does not exist,
				// then neither does the upper-bound.
				return lower;
			}

			if (*lower > key)
			{
				// If the lower-bound is greater-than the
				// key, then it is also the upper-bound.
				return lower;
			}

			// Otherwise the upper-bound is given by 
			// the next element.
			return std::next(lower);
		}

		Iterator upperBound(const Key& key)
		{
			return cast(removeConst(*this).upperBound(key));
		}

		ConstIterator upper_bound(const Key& key) const
		{
			return upperBound(key);
		}

		Iterator upper_bound(const Key& key)
		{
			return upperBound(key);
		}

		//! Returns an iterator to the smallest element >= 'key'.
		/*!
		Time complexity: 
		O(log(log(Delta + 4))),
		where
		Delta is the distance between the searched key
		and the key of the returned element.

		Exception safety:
		nothrow

		returns:
		An iterator to the smallest element 'u' such that 
		'u >= key'. The 'cend()' is considered to have a key 
		larger than any stored key.
		*/
		ConstIterator lowerBound(const Key& key) const
		{
			Chain_ConstIterator keyChain = findChain(key, 0);
			if (keyChain != chainSet_.cend())
			{
				const Key& chainKey = keyChain->first;
				const Chain& chain = keyChain->second;

				if (key <= chain->element()->key())
				{
					return chain->element();
				}

				return std::next(chain->element());
			}

			ConstIterator leftGap = leftGapBound(key);
			if (key <= leftGap->key())
			{
				return leftGap;
			}

			ConstIterator rightGap = std::next(leftGap);
			if (key <= rightGap->key())
			{
				return rightGap;
			}

			return std::next(rightGap);
		}

		Iterator lowerBound(const Key& key)
		{
			return cast(removeConst(*this).lowerBound(key));
		}

		ConstIterator lower_bound(const Key& key) const
		{
			return lowerBound(key);
		}

		Iterator lower_bound(const Key& key)
		{
			return lowerBound(key);
		}

		//! Returns whether the element exists.
		/*!
		Time complexity: O(1) expected
		Exception safety: nothrow
		*/
		bool exists(const Key& key) const
		{
			return find(key) != cend();
		}

		//! Converts a const-iterator to an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return dataSet_.erase(that, that);
		}

		//! Returns a const-iterator to the smallest element.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ConstIterator cbegin() const
		{
			return dataSet_.cbegin();
		}

		Iterator begin()
		{
			return dataSet_.begin();
		}

		ConstIterator begin() const
		{
			return dataSet_.begin();
		}

		//! Returns the one-past-last const-iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ConstIterator cend() const
		{
			return dataSet_.cend();
		}

		Iterator end()
		{
			return dataSet_.end();
		}

		ConstIterator end() const
		{
			return dataSet_.end();
		}

		//! Returns the number of used bits.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer bits() const
		{
			return endBit_ - beginBit_;
		}

		//! Returns the number of elements.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer size() const
		{
			return dataSet_.size();
		}

	private:
		Key replicate(const Key& key, integer level)
		{
			ASSERT_OP(level, >=, 0);
			ASSERT_OP(level, <, bits());

			Key result(key, beginBit_, endBit_);
			if (result[level])
			{
				// The bit at level 'level' is 1.
				// Make all lower bits 1.
				result |= bitMask<Key>(level);
			}
			else
			{
				// The bit at level 'level' is 0.
				// Make all lower bits 0.
				result &= bitMask<Key>(level, bits());
			}

			return result;
		}

		bool prefixExists(const Key& key, integer level) const
		{
			return chainSet_.count(replicate(key, level)) > 0;
		}

		ConstIterator leftGapBoundFromUpperGap(
			const Key& key, integer level) const
		{
			Chain_ConstIterator keyChain = findChain(key, nearbyLevel);
			ASSERT(keyChain != chainSet_.cend());

			const Key& key = keyChain->first;
			const Chain& chain = keyChain->second;
			const Key& split = chain.key();

			auto turnAtOrAbove = [&](integer offset)
			{
				return (split & bitMask<Key>(level - offset, level + 1)) != 0;
			};

			// Find the first turn in the gap-path below the
			// level 'level'. If there is no turn, then this
			// will return 'level', which is also correct for
			// what follows.
			integer turnOffset = 
				exponentialBinarySearch(0, level, turnAtOrAbove);

			Key gapKey = key | bitmask<Key>(0, level);
			if (odd(key))
			{
				// Follow the upper 1-gap-path downwards 
				// to the first split- or leaf-node, and
				// then follow the lower 0-gap-path.
				gapKey ^= bitMask<Key>(level - turnOffset);
			}
			else
			{
				// Follow the upper 0-gap-path downwards 
				// to the first split- or leaf-node, and
				// then follow the lower 1-gap-path.
				gapKey ^= bitMask<Key>(level - turnOffset, level);
			}

			// Find the element corresponding to the gap-bound.
			ConstIterator gapBound = dataSet_.find(gapKey);
			// By the properties of gap-paths, this element exists.
			ASSERT(gapBound != dataSet_.end());

			if (odd(key))
			{
				// Since we followed the right gap-path,
				// we are now at the right gap-bound.
				// The left gap-bound is its predecessor.
				ASSERT(gapBound != cbegin());
				--gapBound;
			}

			return gapBound;
		}

		Chain_ConstIterator findChain(const Key& key, integer level) const
		{
			return chainSet_.find(replicate(key, nearbyLevel));
		}

		bool odd(const Key& key) const
		{
			return key[0];
		}

		ConstIterator leftGapBound(const Key& key) const
		{
			if (empty())
			{
				// There are no elements, so there isn't a 
				// left gap-bound either.
				return cend();
			}

			auto nearby = [&](integer level)
			{
				return 
					prefixExists(key, level) ||
					prefixExists(key - powerOfTwo<Key>(level), level) ||
					prefixExists(key + powerOfTwo<Key>(level), level);
			};

			integer nearbyLevel =
				exponentialBinarySearch(0, bits(), nearby);

			if (prefixExists(key, nearbyLevel))
			{
				return leftGapBoundFromUpperGap(key, nearbyLevel);
			}

			bool oddKey = odd(key);
			bool nextExists = prefixExists(key + powerOfTwo<Key>(level), nearbyLevel);
			if (oddKey == nextExists)
			{
				Key nearbyKey = key;
				if (nextExists)
				{
					nearbyKey += powerOfTwo<Key>(level);
				}
				else
				{
					nearbyKey -= powerOfTwo<Key>(level);
				}
				return leftGapBoundFromUpperGap(nearbyKey, nearbyLevel);
			}

			Chain_ConstIterator gapBoundChain = findChain(key, nearbyLevel);
			ASSERT(gapBoundChain != chainSet_.cend());

			ConstIterator gapBound = gapBoundChain->element();
			if (oddKey)
			{
				--gapBound;
			}
			return gapBound;
		}

		//! Returns the lowest ancestor node of the given key.
		/*!
		Time complexity: 
		O(log(h(a) - minLevel + 2))
		where
		h(a) is the level of the lowest ancestor node.
		
		Exception safety: 
		nothrow

		returns:
		A pair (chain, level), where 'chain' is an iterator
		to the chain of the lowest-ancestor node, and the 
		'level' is the level at which the lowest-ancestor 
		node resides in the chain.
		*/
		auto lowestAncestor(const Key& key) -> integer
		{
			integer minLevel = 0;
			while(true)
			{
				auto nearby = [&](integer level)
				{
					return 
						prefixExists(key, level) ||
						prefixExists(key - powerOfTwo<Key>(level), level) ||
						prefixExists(key + powerOfTwo<Key>(level), level);
				};

				integer nearbyLevel =
					exponentialBinarySearch(minLevel, bits(), nearby);

				if (prefixExists(key, nearbyLevel))
				{
					// If a prefix of the key exists in the 'nearbyLevel'
					// of the trie, then the corresponding node is the
					// lowest ancestor of the key.
					return nearbyLevel;
				}

				bool nextExists = prefixExists(
					key + powerOfTwo<Key>(nearbyLevel), 
					nearbyLevel);

				Key nearbyKey = key;
				if (nextExists)
				{
					nearbyKey += powerOfTwo<Key>(nearbyLevel);
				}
				else
				{
					nearbyKey -= powerOfTwo<Key>(nearbyLevel);
				}
				nearbyKey = replicate(nearbyKey, nearbyLevel);

				bool oddNearbyKey = nearbyKey[nearbyLevel];
				if (nextExists == oddNearbyKey)
				{
					// If the right neighbor is on the right 1-gap-path,
					// then the level above must contain the lowest-ancestor
					// of the key. Similarly for the left neighbor on 
					// the left 0-gap-path.
					return nearbyLevel + 1;
				}

				// Find the level above the chain of 'nearbyKey'.
				auto differentChain = [&](integer level)
				{
					return replicate(nearbyKey, level) != nearbyChainKey;
				};

				minLevel = 
					exponentialBinarySearch(nearbyLevel + 1, bits(), differentChain);
			}

			return 0;
		}

		//! The bits to use in a key.
		/*!
		Preconditions:
		0 <= beginBit_ < endBit_ <= Bits

		The indices of the bits to use are in the range 
		[beginBit_, endBit_).
		*/
		integer beginBit_;
		integer endBit_;

		//! The set of chains.
		/*!
		Each physical key 'k' is associated bijectively
		with a chain. A chain is a maximal integer interval 
		C = [0, H), such that the h-replication of 'k' is 
		constant for all h in C. Thus each chain is able to 
		pack H nodes of an x-fast trie together. By choosing 
		the physical keys so that they make use of the 
		chain-packing, the space complexity can be made 
		linear.
		*/
		ChainSet chainSet_;

		//! The elements stored in increasing order.
		DataSet dataSet_;
	};
	
}

namespace Pastel
{

	template <integer Bits_, typename Value_>
	class XFastTrie_Map_Settings
	{
	public:
		static const integer Bits = Bits_;
		using Value = Value_;
	};

	template <integer Bits, typename Value>
	using XFastTrie_Map = XFastTrie<XFastTrie_Map_Settings<Bits, Value>>;

	template <integer Bits> 
	using XFastTrie_Set_Settings = XFastTrie_Map_Settings<Bits, void>;

	template <integer Bits>
	using XFastTrie_Set = XFastTrie<XFastTrie_Set_Settings<Bits>>;

}

#endif
