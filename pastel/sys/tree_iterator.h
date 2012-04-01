// Description: Preorder tree iterator

#ifndef PASTEL_TREE_ITERATOR_H
#define PASTEL_TREE_ITERATOR_H

#include "pastel/sys/tree_cursor.h"

namespace Pastel
{

	//! A preorder tree iterator.
	template <typename Data>
	class Tree_Iterator;

	//! A preorder tree iterator.
	template <typename Data>
	class Tree_ConstIterator;

	//! Converts a tree cursor to a preorder tree iterator.
	template <typename Data>
	Tree_ConstIterator<Data> asIterator(
		const Tree_ConstCursor<Data, 2>& cursor);

	//! Converts a tree cursor to a preorder tree iterator.
	template <typename Data>
	Tree_Iterator<Data> asIterator(
		const Tree_Cursor<Data, 2>& cursor);

}

#include "pastel/sys/tree_iterator.hpp"

#endif
