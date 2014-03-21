// Description: Hashed tree

#ifndef PASTELSYS_HASHED_TREE_H
#define PASTELSYS_HASHED_TREE_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/lessthan.h"
#include "pastel/sys/hashing.h"

#include <utility>

namespace Pastel
{

	template <typename Settings>
	class Hash_RedBlackTree_Customization;
	
	template <typename Data>
	class HashedTree_Data
	: public Class<Data>
	{
	public:
		typedef Class<Data>
			Data_Class;

		HashedTree_Data()
		: Data_Class()
		, hash_(0)
		{
		}

		template <typename That>
		HashedTree_Data(That that)
		: Data_Class(std::move(that))
		, hash_(0)
		{
		}

		operator const Data_Class&() const
		{
			return *this;
		}

		operator Data_Class&()
		{
			return *this;
		}

		hash_integer hash() const
		{
			return hash_;
		}

	private:
		template <typename Settings>
		friend class Hash_RedBlackTree_Customization;

		hash_integer hash_;
	};

	template <
		typename Key,
		typename Data = void,
		typename Compare = LessThan,
		typename Hash_ = std::hash<Key>>
	class HashedTree_Settings
		: public RedBlack_Settings<Key, HashedTree_Data<Data>, 
		Compare, RedBlackTree_Dereference_Key>
	{
	public:
		using Hash = Hash_;	
	};

	template <
		typename Key,
		typename Data = void,
		typename Compare = LessThan,
		typename Hash = std::hash<Key>>
	using HashedTree = RedBlackTree<HashedTree_Settings<Key, HashedTree_Data<Data>, Compare, Hash>, 
		Hash_RedBlackTree_Customization>;

}

#include "pastel/sys/hashed_tree.hpp"

#endif
