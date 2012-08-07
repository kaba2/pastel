#ifndef PASTEL_HASHED_TREE_H
#define PASTEL_HASHED_TREE_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/lessthan.h"
#include "pastel/sys/hash.h"

namespace Pastel
{

	template <typename Element>
	class Hash_Customization;

	// This simulates an alias template until it becomes
	// available in Visual Studio.
	template <typename Element>
	class AsHashedTree
	{
	public:
		typedef RedBlackTree<Element, LessThan, hash_integer,
			Hash_Customization<Element>> type;
	};

}

#include "pastel/sys/hashed_tree.hpp"

#endif
