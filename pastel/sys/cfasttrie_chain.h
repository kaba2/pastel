// Description: C-fast trie node

#ifndef PASTELSYS_CFASTTRIE_NODE_H
#define PASTELSYS_CFASTTRIE_NODE_H

#include "pastel/sys/cfasttrie.h"
#include "pastel/sys/logarithm.h"
#include "pastel/sys/leading_zero_bits.h"

#include <unordered_map>
#include <array>
#include <memory>

namespace Pastel
{

	namespace CFastTrie_
	{

		template <typename Integer>
		integer chainHeight(const Integer& that)
		{
			return even(that) ? 
				leadingZeroBits() :
				leadingOneBits();
		}

		template <
			typename Key,
			typename Iterator>
		class Chain
		{
		public:
			explicit Chain(Iterator element)
			: height_(chainHeight(element->key()))
			, split_(0)
			, element_(element)
			{
			}

			integer height() const
			{
				return height_;
			}

			const Key& split() const
			{
				return split_;
			}

			Iterator& element()
			{
				return element_;
			}

			const Iterator& element() const
			{
				return element_;
			}

		private:
			//! The height of the chain.
			/*!
			The height h of a chain is the number of
			nodes in it, and takes O(log(h + 2)) time to 
			compute. This value must be computed when 
			inserting the chain, since the split-bit of 
			the chain just above this chain needs to be 
			set. By storing the height we save some time
			when searching for the lowest ancestors.
			*/
			integer height_;

			//! The split-node markers.
			/*!
			The i:th bit is 1 if and only if
			the node at level i of the chain is a 
			split node.
			*/
			Key split_;

			//! The element stored in this chain.
			Iterator element_;
		};

	}

}

#endif
