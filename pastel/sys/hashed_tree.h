// Description: Hashed tree

#ifndef PASTELSYS_HASHED_TREE_H
#define PASTELSYS_HASHED_TREE_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/hashing.h"

#include <utility>

namespace Pastel
{

	template <typename Settings>
	class Hash_RedBlackTree_Customization;
	
	template <typename Propagation>
	class Hash_Propagation
	{
	public:
		Hash_Propagation()
		: data_()
		, hash_(0)
		{
		}

		template <typename That>
		Hash_Propagation(That that)
		: data_(std::move(that))
		, hash_(0)
		{
		}

		operator const Propagation&() const
		{
			return data_;
		}

		operator Propagation&()
		{
			return data_;
		}

		hash_integer hash() const
		{
			return hash_;
		}

	private:
		template <typename Settings>
		friend class Hash_RedBlackTree_Customization;

		Propagation data_;
		hash_integer hash_;
	};

	template <
		typename Settings_,
		typename Hash_>
	class HashedTree_Settings
		: public Settings_
	{
	public:
		using Propagation = Hash_Propagation<typename Settings_::Propagation>;
		using Hash = Hash_;	
	};

	template <typename Settings, typename Hash>
	using HashedTree = RedBlackTree<
		HashedTree_Settings<Settings, Hash>, 
		Hash_RedBlackTree_Customization>; 

}

namespace Pastel
{

	template <
		typename Key,
		typename Less = LessThan,
		typename Propagation = Empty,
		typename SentinelData = Empty,
		typename Hash = std::hash<Key>>
	using Hashed_Set = HashedTree<
		RedBlackTree_Set_Settings<Key, Empty, Less, Propagation, SentinelData, false>, 
		Hash>;

	template <
		typename Key,
		typename Less = LessThan,
		typename Propagation = Empty,
		typename SentinelData = Empty,
		typename Hash = std::hash<Key>>
	using Hashed_MultiSet = HashedTree<
		RedBlackTree_Set_Settings<Key, Empty, Less, Propagation, SentinelData, true>, 
		Hash>;

}

namespace Pastel
{

	template <
		typename Key,
		typename Data,
		typename Less = LessThan,
		typename Propagation = Empty,
		typename SentinelData = Empty,
		typename Hash = std::hash<Key>>
	using Hashed_Map = HashedTree<
		RedBlackTree_Set_Settings<Key, Data, Less, Propagation, SentinelData, false>,
		Hash>;

	template <
		typename Key,
		typename Data,
		typename Less = LessThan,
		typename Propagation = Empty,
		typename SentinelData = Empty,
		typename Hash = std::hash<Key>>
	using Hashed_MultiMap = HashedTree<
		RedBlackTree_Set_Settings<Key, Data, Less, Propagation, SentinelData, true>,
		Hash>;

}

#include "pastel/sys/hashed_tree.hpp"

#endif
