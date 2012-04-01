#ifndef PASTEL_TREE_ITERATOR_HPP
#define PASTEL_TREE_ITERATOR_HPP

#include "pastel/sys/tree_iterator.h"

namespace Pastel
{

	template <typename Data>
	class Tree_ConstIterator
	{
	public:
		typedef Tree_ConstCursor<Data, 2> ConstCursor;

		explicit Tree_ConstIterator(
			const ConstCursor& cursor)
			: cursor_(cursor)
		{
		}

		Tree_ConstIterator& operator++()
		{
			// If the cursor is in the sentinel node,
			// we should remain in that node.
			if (!cursor_.sentinel())
			{
				cursor_ = next(1);
			}

			return *this;
		}

		Tree_ConstIterator& operator--()
		{
			// If the cursor is in the sentinel node,
			// we should back out from it to the
			// 'maximum' node (the parent of the
			// sentinel node). Thus no special 
			// handling for the sentinel node here.

			cursor_ = next(0);

			return *this;
		}

		const Data& operator*() const
		{
			return *cursor_;
		}

		const Data* operator->() const
		{
			return &(*cursor_);
		}

	private:
		ConstCursor next(integer forward)
		{
			ASSERT_OP(forward, >=, 0);
			ASSERT_OP(forward, <=, 1);

			const integer backward = !forward;

			ConstCursor result;
			ConstCursor child = cursor_.child(forward);
		
			if (child.sentinel())
			{
				result = cursor_;

				ConstCursor previous;
				do
				{
					previous = result;
					result = previous.parent();
				}
				while(!result.sentinel() && result.child(backward) != previous);
			}
			else
			{
				result = child;

				while(!result.child(backward).sentinel())
				{
					result = result.child(backward);
				}
			}

			return result;
		}

		ConstCursor cursor_;
	};

	template <typename Data>
	class Tree_Iterator
		: public Tree_ConstIterator<Data>
	{
	public:
		typedef Tree_ConstIterator<Data> Base;
		typedef Tree_Cursor<Data, 2> Cursor;

		explicit Tree_Iterator(
			const Cursor& cursor)
			: Base(cursor)
		{
		}

		Tree_ConstIterator& operator++()
		{
			Base::operator++();

			return *this;
		}

		Tree_ConstIterator& operator--()
		{
			Base::operator--();

			return *this;
		}

		Data& operator*()
		{
			return (Data&)Base::operator*();
		}

		Data* operator->()
		{
			return (Data*)Base::operator->();
		}
	};

	template <typename Data>
	Tree_ConstIterator<Data> asIterator(
		const Tree_ConstCursor<Data, 2>& cursor)
	{
		return Tree_ConstIterator<Data>(cursor);
	}

	template <typename Data>
	Tree_Iterator<Data> asIterator(
		const Tree_Cursor<Data, 2>& cursor)
	{
		return Tree_Iterator<Data>(cursor);
	}

}

#endif
