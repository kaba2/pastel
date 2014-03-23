#ifndef PASTELSYS_HASHED_TREE_HPP
#define PASTELSYS_HASHED_TREE_HPP

#include "pastel/sys/hashed_tree.h"

namespace Pastel
{

	template <typename Settings>
	class Hash_RedBlackTree_Customization
		: public Empty_RedBlackTree_Customization<Settings>
	{
	protected:
		using Fwd = RedBlackTree_Fwd<Settings>;
		using Hash = typename Settings::Hash;
		
		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Propagation);

		Hash_RedBlackTree_Customization() {}

		void updatePropagation(
			const Iterator& element,
			Propagation& propagation)
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

			propagation.hash_ = 
				Hash()(element.key()) + 
				element.left().propagation().hash() +
				element.right().propagation().hash();
		}

	private:
		typedef RedBlackTree<Settings, Pastel::Hash_RedBlackTree_Customization>
			Derived;

		Hash_RedBlackTree_Customization(const Hash_RedBlackTree_Customization&) = delete;
		Hash_RedBlackTree_Customization(Hash_RedBlackTree_Customization&&) = delete;
		Hash_RedBlackTree_Customization& operator=(Hash_RedBlackTree_Customization) = delete;

	public:
		//! Returns the combined hash of all elements in the tree.
		hash_integer hash() const
		{
			const Derived& tree = (const Derived&)*this;

			if (tree.empty())
			{
				return 0;
			}

			return tree.croot().propagation().hash();
		}
	};

	template <typename Settings, typename Hash>
	bool operator==(
		const HashedTree<Settings, Hash>& left,
		const HashedTree<Settings, Hash>& right)
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

	template <typename Settings, typename Hash>
	bool operator!=(
		const HashedTree<Settings, Hash>& left,
		const HashedTree<Settings, Hash>& right)
	{
		return !(left == right);
	}

}

namespace std
{

	template <typename Settings, typename Hash>
	struct hash<Pastel::HashedTree<Settings, Hash>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::HashedTree<Settings, Hash>& that) const
		{
			// The hashed tree stores the hash of all elements
			// in the root element data.
			return that.hash();
		}
	};

}

#endif
