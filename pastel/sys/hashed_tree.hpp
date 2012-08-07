#ifndef PASTEL_HASHED_TREE_HPP
#define PASTEL_HASHED_TREE_HPP

#include "pastel/sys/hashed_tree.h"

namespace Pastel
{

	template <typename Element>
	class Hash_Customization
		: public RedBlackTree_Concepts::Customization<
		Element, LessThan, hash_integer>
	{
	protected:
		Hash_Customization() {}

		typedef RedBlackTree_Fwd<Element, LessThan, hash_integer> Fwd;

		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);

		void updateHierarchical(const Iterator& node)
		{
			*node = combineHash(
				*node.left(),
				*node.right());
		}

	private:
		Hash_Customization(const Hash_Customization&) PASTEL_DELETE;
		Hash_Customization(Hash_Customization&&) PASTEL_DELETE;
		Hash_Customization& operator=(Hash_Customization) PASTEL_DELETE;
	};

}

#endif
