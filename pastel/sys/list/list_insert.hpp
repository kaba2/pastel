#ifndef PASTELSYS_LIST_INSERT_HPP
#define PASTELSYS_LIST_INSERT_HPP

#include "pastel/sys/list.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	template <typename... Type>
	auto List<Settings, Customization>::insert(
		const ConstIterator& to, Type&&... data)
		-> Iterator
	{
		ENSURE(!to.empty());

		Node* toNode = cast(to).base();
		Node* node = nodeAllocate(
			std::forward<Type>(data)...);
		Node* previous = toNode->prev();

		linkNodes(previous, node);
		linkNodes(node, toNode);

		++size_;

		return Iterator(node);
	}

}

#endif
