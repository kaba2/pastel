#ifndef PASTELSYS_SKIPFAST_INVARIANTS_HPP
#define PASTELSYS_SKIPFAST_INVARIANTS_HPP

#include "pastel/sys/skipfast_invariants.h"
#include "pastel/sys/leading_one_bits.h"
#include "pastel/sys/leading_zero_bits.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	bool testInvariants(const SkipFast<Settings, Customization>& that)
	{
		// The trie is empty if and only if its size is zero.
		if (that.empty() != (that.size() == 0))
		{
			return false;
		}

		if (that.empty())
		{
			return true;
		}

		// Check the groups.
		{
			auto group = that.cgroupBegin();
			while (group != that.cgroupEnd())
			{
				auto chain = group->begin();
				while (chain != group->end())
				{
					bool nonEmpty = 
						!chain->elementSet_.empty() ||
						chain.left().propagation().nonEmpty ||
						chain.right().propagation().nonEmpty;

					if (nonEmpty != chain.propagation().nonEmpty)
					{
						// The propagation data must be up-to-date.
						return false;
					}
					++chain;
				}

				++group;
			}
		}

		// Check the chains.
		{
			auto iter = that.cchainBegin();

			// The first chain is the zero-chain.
			if (!zero(iter.key()))
			{
				return false;
			}

			while (iter != that.cchainEnd())
			{
				auto next = std::next(iter);

				// The chains are in increasing order.
				if (next != that.cchainEnd() &&
					iter.key() >= next.key())
				{
					return false;
				}

				/*
				// The chain is of height zero if and only if
				// the chain-key is zero.
				if ((iter->height() == 0) != (iter.key() == 0))
				{
					return false;
				}
				*/

				// Each chain has a parent chain, except the zero chain.
				auto above = (iter.key() == 0) ?
					that.cchainEnd() : that.findChain(iter.key(), iter->height());
				if (iter.key() != 0 && above == that.cchainEnd())
				{
					return false;
				}

				// The chain-height equals the number of 
				// zero (one) bits in the chain-key if the 
				// chain-key is even (odd).
				if ((even(iter.key()) && leadingZeroBits(iter.key()) != iter->height()) ||
					(odd(iter.key()) && leadingOneBits(iter.key()) != iter->height()))
				{
					return false;
				}

				++iter;
			}
		}

		// Check the elements.
		{
			auto iter = that.cbegin();
			auto next = std::next(iter);
			while (!next.isSentinel())
			{
				ASSERT(!iter.isSentinel());
				ASSERT(!next.isSentinel());

				// The elements are in increasing order.
				if (iter.key() >= next.key())
				{
					return false;
				}

				iter = next;
				++next;
			}
		}

		if (that.size() != std::distance(that.begin(), that.end()))
		{
			// The size of the trie must equal the number of
			// elements accessible through iterators.
			return false;
		}

		return true;
	}

}

#endif
