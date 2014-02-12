#ifndef PASTELSYS_CFASTTRIE_HPP
#define PASTELSYS_CFASTTRIE_HPP

#include "pastel/sys/cfasttrie.h"

namespace Pastel
{

	template <typename CFastTrie_Settings>
	bool checkInvariants(const CFastTrie<CFastTrie_Settings>& trie)
	{
		if (trie.empty())
		{
			return true;
		}

		auto iter = trie.cbegin();
		auto next = std::next(iter);
		while(next != trie.cend())
		{
			if (iter->key() >= next->key())
			{
				return false;
			}
			
			if (iter->chain()->first >= next->chain()->first)
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
