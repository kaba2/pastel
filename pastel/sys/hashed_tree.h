// Description: Hashed tree

#ifndef PASTELSYS_HASHED_TREE_H
#define PASTELSYS_HASHED_TREE_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/lessthan.h"
#include "pastel/sys/hashing.h"

namespace Pastel
{

	template <typename Settings, typename Hash>
	class Hash_RedBlackTree_Customization;

	template <
		typename Element, 
		typename Compare = LessThan,
		typename Hash = std::hash<Element>>
	using HashedTree = Map<Element, hash_integer, Compare,
		Hash_RedBlackTree_Customization<
		Map_Settings<Element, hash_integer, Compare>, Hash>>;

}

#include "pastel/sys/hashed_tree.hpp"

#endif
