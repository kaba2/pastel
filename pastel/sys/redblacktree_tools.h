#ifndef PASTELSYS_REDBLACKTREE_TOOLS_H
#define PASTELSYS_REDBLACKTREE_TOOLS_H

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Key, typename Compare, typename Data, typename Customization>
	bool check(const RedBlackTree<Key, Compare, Data, Customization>& tree);

}

#include "pastel/sys/redblacktree_tools.hpp"

#endif
