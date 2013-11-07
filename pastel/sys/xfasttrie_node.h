// Description: X-fast trie node

#ifndef PASTELSYS_XFASTTRIE_NODE_H
#define PASTELSYS_XFASTTRIE_NODE_H

#include "pastel/sys/xfasttrie.h"

#include <unordered_map>
#include <array>

namespace Pastel
{

	namespace XFastTrie_
	{

		template <typename Iterator>
		class Node
		{
		public:
			Node()
			: element_()
			{
			}

			Iterator element_;
		};

	}

}

#endif
