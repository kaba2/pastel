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

		Node* node = (Node*)to.base();
		Node* newNode = (Node*)nodeAllocate(
			std::forward<Type>(data)...);
		Node* previous = node->prev();

		linkNodes(previous, newNode);
		linkNodes(newNode, node);

		++size_;

		return Iterator(newNode);
	}

}

#endif
