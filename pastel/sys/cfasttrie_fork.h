// Description: C-fast trie fork

#ifndef PASTELSYS_CFASTTRIE_FORK_H
#define PASTELSYS_CFASTTRIE_FORK_H

#include "pastel/sys/cfasttrie_fwd.h"

#include <map>

namespace Pastel
{

	namespace CFastTrie_
	{

		//! C-fast trie fork
		template <typename CFastTrie_Settings>
		class Fork
		{
		public:
			using Fwd = CFastTrie_Fwd<CFastTrie_Settings>;

			PASTEL_FWD(Iterator);
			PASTEL_FWD(Chain_Iterator);

			Fork(const Iterator& element_,
				const Chain_Iterator& chain_)
			: element(element_)
			, chain(chain_)
			{
			}

			//! The representative element.
			/*!
			It is an invariant of the c-fast trie that each
			fork-chain contains at least one element. The
			fork references any such element, to localize
			a skip-list search.
			*/
			Iterator element;

			//! The chain.
			/*!
			The fork stores the chain itself, so that
			it can be removed when removing an ancestor 
			element of the fork-chain.
			*/
			Chain_Iterator chain;
		};

	}

}

#endif
