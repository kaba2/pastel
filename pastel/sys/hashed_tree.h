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
	
	template <typename Propagation>
	class Hash_Propagation
	: public Class<Propagation>
	{
	public:
		typedef Class<Propagation>
			Propagation_Class;

		Hash_Propagation()
		: Propagation_Class()
		, hash_(0)
		{
		}

		template <typename That>
		Hash_Propagation(That that)
		: Propagation_Class(std::move(that))
		, hash_(0)
		{
		}

		operator const Propagation_Class&() const
		{
			return *this;
		}

		operator Propagation_Class&()
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
		typename Propagation = void,
		typename Hash = std::hash<Key>>
	using Hashed_Set = HashedTree<
		RedBlack_Settings<Key, void, Less, Propagation, false>, 
		Hash>;

	template <
		typename Key,
		typename Less = LessThan,
		typename Propagation = void,
		typename Hash = std::hash<Key>>
	using Hashed_MultiSet = HashedTree<
		RedBlack_Settings<Key, void, Less, Propagation, true>, 
		Hash>;

}

namespace Pastel
{

	template <
		typename Key,
		typename Data,
		typename Less = LessThan,
		typename Propagation = void,
		typename Hash = std::hash<Key>>
	using Hashed_Map = HashedTree<
		RedBlack_Settings<Key, Data, Less, Propagation, false>,
		Hash>;

	template <
		typename Key,
		typename Data,
		typename Less = LessThan,
		typename Propagation = void,
		typename Hash = std::hash<Key>>
	using Hashed_MultiMap = HashedTree<
		RedBlack_Settings<Key, Data, Less, Propagation, true>,
		Hash>;

}

#include "pastel/sys/hashed_tree.hpp"

#endif
