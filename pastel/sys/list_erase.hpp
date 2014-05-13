#ifndef PASTELSYS_LIST_ERASE_HPP
#define PASTELSYS_LIST_ERASE_HPP

#include "pastel/sys/list.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	auto List<Settings, Customization>::erase(
		const ConstIterator& that)
		-> Iterator
	{
		ASSERT(that.isNormal());

		Node* node = (Node*)that.base();

		Node* next = node->next();
		ASSERT(next);

		Node* previous = node->prev();
		ASSERT(previous);

		linkNodes(previous, next);

		nodeDeallocate(node);

		--size_;

		return Iterator(next);
	}

}

#endif
