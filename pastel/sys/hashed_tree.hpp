#ifndef PASTELSYS_HASHED_TREE_HPP
#define PASTELSYS_HASHED_TREE_HPP

#include "pastel/sys/hashed_tree.h"

namespace Pastel
{

	template <typename Settings, typename Hash>
	class Hash_RedBlackTree_Customization
	: public Empty_RedBlackTree_Customization<Settings>
	{
	protected:
		using Fwd = RedBlackTree_Fwd<Settings>;
		
		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);

		Hash_RedBlackTree_Customization() {}

		void updateHierarchical(const Iterator& node)
		{
			// Note: the hash combination function
			// must be associative. This is because
			// the red-black tree might perform tree
			// rotations.

			// Currently, the hash combination function
			// below is just the addition. This is
			// probably a very bad hash combination
			// function; the hash at the root is just the
			// sum of all elements.
			// One could also use Tillich-Zemor
			// hashing (then the hash combination function
			// is the matrix-multiplication), but I think 
			// that would be too slow.
			node.data().hash_ = 
				(hash_integer)Hash()(node.key()) +
				node.left().data().hash() +
				node.right().data().hash();
		}

	private:
		typedef RedBlackTree<Settings, Hash_RedBlackTree_Customization>
			Derived;

		Hash_RedBlackTree_Customization(const Hash_RedBlackTree_Customization&) PASTEL_DELETE;
		Hash_RedBlackTree_Customization(Hash_RedBlackTree_Customization&&) PASTEL_DELETE;
		Hash_RedBlackTree_Customization& operator=(Hash_RedBlackTree_Customization) PASTEL_DELETE;

	public:
		//! Returns the combined hash of all elements in the tree.
		hash_integer hash() const
		{
			const Derived& tree = (const Derived&)*this;

			if (tree.empty())
			{
				return 0;
			}

			return tree.croot().data().hash();
		}
	};

	template <typename Key, typename Data, typename Compare, typename Hash>
	bool operator==(
		const HashedTree<Key, Data, Compare, Hash>& left,
		const HashedTree<Key, Data, Compare, Hash>& right)
	{
		bool result = false;

		// The item-sets can only be equal if they
		// have the same hash, and are of the same size.
		// This will make most of the comparisons trivial.
		if (computeHash(left) == computeHash(right) &&
			left.size() == right.size())
		{
			// Note that this testing for equality of
			// item-sets relies on the items being ordered
			// the same way, as is the case with RedBlackTree.
			result = std::equal(
				left.cbegin(), left.cend(),
				right.cbegin());
		}

		return result;
	}

	template <typename Key, typename Data, typename Compare, typename Hash>
	bool operator!=(
		const HashedTree<Key, Data, Compare, Hash>& left,
		const HashedTree<Key, Data, Compare, Hash>& right)
	{
		return !(left == right);
	}

}

namespace std
{

	template <typename Key, typename Data, typename Compare, typename Hash>
	struct hash<Pastel::HashedTree<Key, Data, Compare, Hash>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::HashedTree<Key, Data, Compare, Hash>& that) const
		{
			// The hashed tree stores the hash of all elements
			// in the root element data.
			return that.croot().data().hash();
		}
	};

}

#endif
