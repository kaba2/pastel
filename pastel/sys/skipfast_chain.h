// Description: Skip-fast trie chain

#ifndef PASTELSYS_SKIPFAST_CHAIN_H
#define PASTELSYS_SKIPFAST_CHAIN_H

#include "pastel/sys/skipfast_fwd.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	class SkipFast;

	namespace SkipFast_
	{

		//! Skip-fast trie chain
		template <typename Settings>
		class Chain
		{
		public:
			using Fwd = SkipFast_Fwd<Settings>;
			PASTEL_FWD(ElementSet);
			PASTEL_FWD(Key);

			Chain()
				: height_(0)
				, levelBegin_(0)
				, elementSet_()
			{
			}

			Chain(integer levelBegin, integer height)
			: height_(height)
			, levelBegin_(levelBegin)
			, elementSet_()
			{
				ASSERT_OP(height, >=, 0);
				ASSERT_OP(levelBegin, >=, 0);
				ASSERT_OP(levelBegin, <=, height);
			}

			Chain(const Chain&) = default;

			//! Returns the height of the chain.
			/*!
			The zero chain has an infinite height;
			this special case is denoted by zero.
			*/
			integer height() const
			{
				return height_;
			}

			integer levelBegin() const
			{
				return levelBegin_;
			}

		private:
			template <typename, template <typename> class>
			friend class Pastel::SkipFast;

			Chain(Chain&&) = delete;
			Chain& operator=(Chain) = delete;

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

			//! The level at which the chain begins.
			integer levelBegin_;

		public:
			//! The set of elements under the lowest node.
			ElementSet elementSet_;
		};

	}

}

#endif
