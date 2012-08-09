#ifndef PASTEL_HASHED_TREE_HPP
#define PASTEL_HASHED_TREE_HPP

#include "pastel/sys/hashed_tree.h"

namespace Pastel
{

	template <typename Element, typename Compare>
	class Hash_RedBlackTree_Customization
		: public RedBlackTree_Concepts::Customization<
		Element, Compare, hash_integer>
	{
	protected:
		Hash_RedBlackTree_Customization() {}

		typedef RedBlackTree_Fwd<Element, Compare, hash_integer> Fwd;

		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);

		void updateHierarchical(const Iterator& node)
		{
			node.data() = combineHash(
				node.left().data(),
				node.right().data());
		}

	private:
		Hash_RedBlackTree_Customization(const Hash_RedBlackTree_Customization&) PASTEL_DELETE;
		Hash_RedBlackTree_Customization(Hash_RedBlackTree_Customization&&) PASTEL_DELETE;
		Hash_RedBlackTree_Customization& operator=(Hash_RedBlackTree_Customization) PASTEL_DELETE;
	};

	template <typename Element, typename Compare>
	bool operator==(
		const RedBlackTree<Element, Compare, hash_integer,
			Hash_RedBlackTree_Customization<Element, Compare>>& left,
		const RedBlackTree<Element, Compare, hash_integer,
			Hash_RedBlackTree_Customization<Element, Compare>>& right)
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
			// the same way, as is the case with ItemSet.
			result = std::equal(
				left.cbegin(), left.cend(),
				right.cbegin());
		}

		return result;
	}

}

namespace std
{

	template <typename Element, typename Compare>
	class hash<Pastel::RedBlackTree<Element, Compare, Pastel::hash_integer,
		Pastel::Hash_RedBlackTree_Customization<Element, Compare>>>
	{
	public:
		Pastel::hash_integer operator()(
			const typename Pastel::AsHashedTree<Element, Compare>::type& that) const
		{
			// The hashed tree stores the hash of all elements
			// in the root element data.
			return that.croot().data();
		}
	};

}

#endif
