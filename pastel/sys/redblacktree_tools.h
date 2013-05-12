#ifndef PASTELSYS_REDBLACKTREE_TOOLS_H
#define PASTELSYS_REDBLACKTREE_TOOLS_H

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, typename Customization>
	bool check(const RedBlackTree<Settings, Customization>& tree);

}

#include "pastel/sys/redblacktree_tools.hpp"

#endif
