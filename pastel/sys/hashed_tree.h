#ifndef PASTEL_HASHED_TREE_H
#define PASTEL_HASHED_TREE_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/lessthan.h"
#include "pastel/sys/hash.h"

namespace Pastel
{

	template <typename Element, typename Compare>
	class Hash_RedBlackTree_Customization;

	// This simulates an alias template until it becomes
	// available in Visual Studio.
	template <typename Element, typename Compare = LessThan>
	class AsHashedTree
	{
	public:
		typedef RedBlackTree<Element, Compare, hash_integer,
			Hash_RedBlackTree_Customization<Element, Compare>> type;
	};

}

#include "pastel/sys/hashed_tree.hpp"

#endif
