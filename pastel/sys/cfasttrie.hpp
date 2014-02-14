#ifndef PASTELSYS_CFASTTRIE_HPP
#define PASTELSYS_CFASTTRIE_HPP

#include "pastel/sys/cfasttrie.h"

namespace Pastel
{

	template <
		typename Settings,
		typename Customization>
	bool checkInvariants(
		const CFastTrie<Settings, Customization>& trie)
	{
		if (trie.empty())
		{
			return true;
		}

		auto iter = trie.cbegin();

		// If the c-fast trie is non-empty, then  the
		// first chain must be the zero-chain.
		if (!zero(iter->chain()->first))
		{
			return false;
		}

		auto next = std::next(iter);
		while(next != trie.cend())
		{
			// The elements must be in increasing order.
			if (iter->key() >= next->key())
			{
				return false;
			}
			
			// The chain-keys must also be in increasing order.
			if (iter->chain()->first >= next->chain()->first)
			{
				return false;
			}

			// The chain and the element must correspond to 
			// each other.
			if (iter->chain()->second.element() != iter)
			{
				return false;
			}

			++iter;
			++next;
		}

		return true;
	}

}

#endif
