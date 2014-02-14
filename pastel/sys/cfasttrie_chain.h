// Description: C-fast trie chain

#ifndef PASTELSYS_CFASTTRIE_CHAIN_H
#define PASTELSYS_CFASTTRIE_CHAIN_H

#include "pastel/sys/logarithm.h"
#include "pastel/sys/leading_zero_bits.h"
#include "pastel/sys/leading_one_bits.h"
#include "pastel/sys/bitmask.h"

#include <unordered_map>
#include <array>
#include <memory>

namespace Pastel
{

	template <
		typename Settings,
		typename Customization>
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

			//! Returns the height of the chain.
			/*!
			The zero chain has an infinite height;
			this special case is denoted by zero.
			*/
			integer height() const
			{
				return height_;
			}

			//! Returns whether the node at level 'level' is a split-node.
			bool splitExists(integer level) const
			{
				PENSURE_OP(level, >=, 0);
				PENSURE_OP(level, <, checkHeight());

				if (level == 0)
				{
					return false;
				}

				return split_.bit(level - 1);
			}

			//! Returns whether there are split-nodes in range [begin, end).
			bool splitExists(integer begin, integer end) const
			{
				PENSURE_OP(begin, >=, 0);
				PENSURE_OP(begin, <=, end);
				PENSURE_OP(end, <=, checkHeight());

				if (begin == 0)
				{
					++begin;
				}

				if (end == 0)
				{
					++end;
				}

				return !zero(split_ & bitMask<Key>(begin - 1, end - 1));
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
			template <
				typename Settings,
				typename Customization>
			friend class CFastTrie;

			void setSplit(integer level, bool value = true)
			{
				split_.setBit(level - 1, value);
			}

			integer checkHeight() const
			{
				return height_ > 0 ? height_ : split_.bits() + 1;
			}

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
			The i:th bit is 1 if and only if the node 
			at level (i + 1) of the chain is a split node.
			Note that a split-node can never be at level 0,
			and that such an encoding is necessary to be
			able to make use of all bits in the key.
			*/
			Key split_;

			//! The element stored in this chain.
			Iterator element_;
		};

	}

}

#endif
