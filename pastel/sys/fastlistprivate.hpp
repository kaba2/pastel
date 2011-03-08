#ifndef PASTEL_FASTLISTPRIVATE_HPP
#define PASTEL_FASTLISTPRIVATE_HPP

#include "pastel/sys/fastlist.hpp"

#include "pastel/sys/addressof.h"

namespace Pastel
{

	// Private

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::initialize()
	{
		// Why is the head_ allocated with new and not
		// with allocator_ ?
		// This is because we do not want temporary lists
		// to stress our allocators. Some allocators, such
		// as the ArenaAllocator have no way to deallocate
		// memory, and sometimes this can be a remarkable
		// source of wasted memory. A case in this
		// is given by the kdtree implementation which
		// uses FastList's to splice data to the leaf nodes.
		// If there are 7 million nodes, then there
		// are 7 million temporary allocations for
		// the head_ nodes of the lists, although new
		// data itself is not created (the data is
		// just spliced around). ArenaAllocator can't
		// free that memory and accumulates it
		// with disastrous memory consumption.

		head_ = new Node;

		linkNodes(head_, head_);
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::Node*
		FastList<Type, UniformAllocator>::nodeBegin() const
	{
		ASSERT(head_);
		return head_->next_;
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::Node*
		FastList<Type, UniformAllocator>::nodeEnd() const
	{
		ASSERT(head_);
		return head_;
	}

	template <typename Type, typename UniformAllocator>
	template <typename InputIterator>
	typename FastList<Type, UniformAllocator>::iterator
		FastList<Type, UniformAllocator>::insert(
		const const_iterator& there,
		const InputIterator& from,
		const InputIterator& to,
		RangeTag)
	{
		if (from == to)
		{
			return iterator((Node*)there.node_);
		}

		const_iterator first(insert(there, *from));

		try
		{
			InputIterator current(from);
			// The first element was already
			// inserted.
			++current;
			while (current != to)
			{
				insert(there, *current);
				++current;
			}
		}
		catch(...)
		{
			erase(first, there);
			throw;
		}

		return iterator((Node*)first.node_);
	}

	template <typename Type, typename UniformAllocator>
	typename FastList<Type, UniformAllocator>::iterator
		FastList<Type, UniformAllocator>::insert(
		const const_iterator& there,
		size_type count,
		const value_type& that,
		ValueTag)
	{
		ENSURE_OP(count, >=, 0);

		if (count == 0)
		{
			return iterator((Node*)there.node_);
		}

		const_iterator first(insert(there, that));

		try
		{
			for (size_type i = 0;i < count - 1;++i)
			{
				insert(there, that);
			}
		}
		catch(...)
		{
			erase(first, there);
			throw;
		}

		return iterator((Node*)first.node_);
	}

		template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::Node*
		FastList<Type, UniformAllocator>::insert(
		Node* node, const value_type& data)
	{
		ASSERT(node);

		Node* newNode = (Node*)nodeAllocate(data);
		Node* previous = node->previous_;

		linkNodes(previous, newNode);
		linkNodes(newNode, node);

		addSize(1);

		return newNode;
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::Node*
		FastList<Type, UniformAllocator>::erase(Node* node)
	{
		ASSERT(node);
		ASSERT(node != head_);

		Node* next = node->next_;
		Node* previous = node->previous_;

		ASSERT(next);
		ASSERT(previous);

		linkNodes(previous, next);

		nodeDeallocate((DataNode*)node);

		subtractSize(1);

		return next;
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::splice(
		Node* position, FastList& that,
		Node* thatFrom, Node* thatTo)
	{
		ENSURE(allocator_ == that.allocator_);

		ASSERT(position);
		ASSERT(thatFrom);
		ASSERT(thatTo);

		if (thatFrom == thatTo ||
			position == thatFrom)
		{
			// Nothing to be done.
			return;
		}

		size_type count = UnknownSize;

		if (thatFrom->next_ == thatTo)
		{
			count = 1;
		}

		// Remove range [from, to[ from that.

		Node* thatFromPrevious = thatFrom->previous_;
		ASSERT(thatFromPrevious);
		Node* thatToPrevious = thatTo->previous_;
		ASSERT(thatToPrevious);

		linkNodes(thatFromPrevious, thatTo);

		// Insert range into this

		Node* thisPrevious = position->previous_;
		ASSERT(thisPrevious);

		linkNodes(thisPrevious, thatFrom);
		linkNodes(thatToPrevious, position);

		// Update sizes

		// If the splice is inside
		// a single container,
		// no size update needs to
		// be done.
		if (this != addressOf(that))
		{
			addSize(count);
			that.subtractSize(count);
		}
	}

	template <
		typename Type,
		typename UniformAllocator>
		typename FastList<Type, UniformAllocator>::DataNode*
		FastList<Type, UniformAllocator>::nodeAllocate(
		const value_type& data)
	{
		DataNode* newMemory = (DataNode*)allocator_->allocate();
		try
		{
			new(newMemory) DataNode(data);
		}
		catch(...)
		{
			allocator_->deallocate(newMemory);
			throw;
		}

		return newMemory;
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::nodeDeallocate(
		const DataNode* node)
	{
		ASSERT(node != head_);

		// If 'Type' has a trivial destructor,
		// then so does the whole DataNode,
		// and the destruction can be avoided.
		if (!boost::has_trivial_destructor<Type>())
		{
			destruct(node);
		}

		allocator_->deallocate(node);
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::linkNodes(
		Node* that, Node* nextThat)
	{
		ASSERT(that);
		ASSERT(nextThat);

		that->next_ = (Node*)nextThat;
		nextThat->previous_ = (Node*)that;
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::addSize(
		size_type count)
	{
		ASSERT1(count == UnknownSize || count > 0, count);

		if (count == UnknownSize)
		{
			size_ = UnknownSize;
		}
		else
		{
			if (size_ != UnknownSize)
			{
				size_ += count;
			}
		}
	}

	template <
		typename Type,
		typename UniformAllocator>
		void FastList<Type, UniformAllocator>::subtractSize(
		size_type count)
	{
		ASSERT1(count == UnknownSize || count > 0, count);

		if (count == UnknownSize)
		{
			size_ = UnknownSize;
		}
		else
		{
			if (size_ != UnknownSize)
			{
				size_ -= count;
			}
		}
	}

}

#endif
