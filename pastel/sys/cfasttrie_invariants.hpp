#ifndef PASTELSYS_CFASTTRIE_INVARIANTS_HPP
#define PASTELSYS_CFASTTRIE_INVARIANTS_HPP

#include "pastel/sys/cfasttrie.h"
#include "pastel/sys/leading_one_bits.h"
#include "pastel/sys/leading_zero_bits.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	bool testInvariants(const CFastTrie<Settings, Customization>& that)
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

		// Check the chains.
		{
			auto iter = that.cchainBegin();

			// The first chain is the zero-chain.
			if (!zero(iter->key()))
			{
				return false;
			}

			while (iter != that.cchainEnd())
			{
				auto next = std::next(iter);

				// The chains are in increasing order.
				if (next != that.cchainEnd() &&
					iter->key() >= next->key())
				{
					return false;
				}

				// The chain is of height zero if and only if
				// the chain-key is zero.
				if ((iter->height() == 0) != (iter->key() == 0))
				{
					return false;
				}

				// Each chain has a parent chain, except the zero chain.
				auto above = (iter->key() == 0) ?
					that.cchainEnd() : that.findChain(iter->key(), iter->height());
				if (iter->key() != 0 && above == that.cchainEnd())
				{
					return false;
				}

				// The chain-height equals the number of 
				// zero (one) bits in the chain-key if the 
				// chain-key is even (odd).
				if (iter->height() > 0 &&
					(even(iter->key()) && leadingZeroBits(iter->key()) != iter->height()) ||
					(odd(iter->key()) && leadingOneBits(iter->key()) != iter->height()))
				{
					return false;
				}

				// This chain causes a split mark in its
				// parent chain.
				if (above != that.cchainEnd() &&
					!above->splitExists(iter->height()))
				{
					return false;
				}
			
				++iter;
			}
		}

		// Check the bundles
		{
			// There are at least 2 bundles, the first 
			// and the last bundle.
			if (that.bundles() < 2)
			{
				return false;
			}

			// The first and the last bundle are 
			// both empty.
			if (!that.cbundleBegin()->empty() ||
				!that.cbundleLast()->empty())
			{
				return false;
			}

			auto bundle = that.cbundleBegin();
			while (bundle != that.cbundleEnd())
			{
				ASSERT(bundle->testInvariants());

				// The element-set of each bundle stores its bundle 
				// in the sentinel data of the end-node.
				if (bundle->end().sentinelData().bundle != bundle)
				{
					return false;
				}

				if (bundle != that.cbundleBegin() &&
					bundle != that.cbundleLast())
				{
					// The intermediate bundles are never empty.
					if (bundle->empty())
					{
						return false;
					}

					// The largest element of the previous bundle must
					// be smaller than the smallest element of the
					// current bundle.
					auto prevBundle = std::prev(bundle);
					if (!prevBundle->empty())
					{
						ASSERT(!prevBundle->last().isSentinel());
						ASSERT(!bundle->begin().isSentinel());
						const auto& lastOfPrev = prevBundle->last().key();
						const auto& firstOfThis = bundle->begin().key();
						if (lastOfPrev >= firstOfThis)
						{
							return false;
						}
					}
				}

				++bundle;
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

				// Each bundle has no more than log(w) elements.
				/*
				if (iter->bundle()->forks() > that.maxBits())
				{
					return false;
				}
				*/

				// The elements are in increasing order.
				if (iter.key() >= next.key())
				{
					return false;
				}

				// The elements equalToChain() is set if and only if
				// there exists a chain which is equal to the element.
				if (iter.equalToChain() != that.chainExists(iter.key(), 0))
				{
					return false;
				}

				iter = next;
				++next;
			}
		}

		return true;
	}

}

#endif
