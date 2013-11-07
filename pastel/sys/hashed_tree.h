// Description: Hashed tree

#ifndef PASTELSYS_HASHED_TREE_H
#define PASTELSYS_HASHED_TREE_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/lessthan.h"
#include "pastel/sys/hashing.h"

#include <utility>

namespace Pastel
{

	template <typename Settings, typename Hash>
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
		template <typename Settings, typename Hash>
		friend class Hash_RedBlackTree_Customization;

		hash_integer hash_;
	};

	template <
		typename Key,
		typename Data = void,
		typename Compare = LessThan,
		typename Key_Hash = std::hash<Key>>
	using HashedTree = Map<Key, HashedTree_Data<Data>, Compare, RedBlackTree_Dereference_Key,
		Hash_RedBlackTree_Customization<
		Map_Settings<Key, HashedTree_Data<Data>, Compare>, Key_Hash>>;

}

#include "pastel/sys/hashed_tree.hpp"

#endif
