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
			node.data() = combineHash(
				(hash_integer)Hash()(node.key()),
				(hash_integer)combineHash(
				node.left().data(),
				node.right().data()));
		}

	private:
		Hash_RedBlackTree_Customization(const Hash_RedBlackTree_Customization&) PASTEL_DELETE;
		Hash_RedBlackTree_Customization(Hash_RedBlackTree_Customization&&) PASTEL_DELETE;
		Hash_RedBlackTree_Customization& operator=(Hash_RedBlackTree_Customization) PASTEL_DELETE;
	};

	template <typename Key, typename Compare, typename Hash>
	bool operator==(
		const HashedTree<Key, Compare, Hash>& left,
		const HashedTree<Key, Compare, Hash>& right)
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

	template <typename Key, typename Compare, typename Hash>
	bool operator!=(
		const HashedTree<Key, Compare, Hash>& left,
		const HashedTree<Key, Compare, Hash>& right)
	{
		return !(left == right);
	}

}

namespace std
{

	template <typename Key, typename Compare, typename Hash>
	struct hash<Pastel::HashedTree<Key, Compare, Hash>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::HashedTree<Key, Compare, Hash>& that) const
		{
			// The hashed tree stores the hash of all elements
			// in the root element data.
			return that.croot().data();
		}
	};

}

#endif
