// Description: C-fast trie node

#ifndef PASTELSYS_CFASTTRIE_NODE_H
#define PASTELSYS_CFASTTRIE_NODE_H

#include "pastel/sys/cfasttrie.h"
#include "pastel/sys/logarithm.h"
#include "pastel/sys/leading_zero_bits.h"
#include "pastel/sys/leading_one_bits.h"

#include <unordered_map>
#include <array>
#include <memory>

namespace Pastel
{

	template <typename CFastTrie_Settings>
	class CFastTrie;

	namespace CFastTrie_
	{

		template <
			typename Key,
			typename Iterator,
			typename Const_Iterator>
		class Chain
		{
		public:
			Chain(
				Iterator element,
				integer height)
			: height_(height)
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

			Const_Iterator element() const
			{
				return element_;
			}

		private:
			template <typename CFastTrie_Settings>
			friend class CFastTrie;

			//! The height of the chain.
			/*!
			The height h of a chain is the number of
			nodes in it, and takes O(log(h + 2)) time to 
			compute. This value must be computed when 
			inserting the chain, since the split-bit of 
			the chain just above this chain needs to be 
			set. By storing the height we save some time
			when searching for the lowest ancestors.
			
			The number zero denotes an infinite height,
			which is only possible for the 0-chain.
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
