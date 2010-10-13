#ifndef PASTEL_REDBLACKTREE_TOOLS_H
#define PASTEL_REDBLACKTREE_TOOLS_H

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Key, typename Compare, typename RbtPolicy>
	bool check(const RedBlackTree<Key, Compare, RbtPolicy>& tree);

}

#include "pastel/sys/redblacktree_tools.hpp"

#endif
