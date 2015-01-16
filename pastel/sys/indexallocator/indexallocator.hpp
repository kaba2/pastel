#ifndef PASTELSYS_INDEXALLOCATOR_HPP
#define PASTELSYS_INDEXALLOCATOR_HPP

#include "pastel/sys/indexallocator.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/math_functions.h"

#include <vector>

namespace Pastel
{

	inline IndexAllocator::IndexAllocator()
		: data_()
		, allocated_(0)
		, heapHeight_(0)
	{
		/*
		The implementation
		uses a heap binary tree stored in an array.
		Every node of this tree describes the state of its subtree:
		if all subnodes of the node are all allocated, then the node
		is also allocated, otherwise the node is free.
		You can visualize the heap as follows:
		take all the leaf nodes and arrange them in a row. This
		is the actual identifier allocation information. Every subsequent
		pair of this row have a common parent. Replace these pairs with
		their parents and again arrange them in a row. This row is exactly
		half the width of the original. For a given pair, if both nodes
		are allocated, then the parent is also marked allocated, otherwise the
		parent is marked free. If you repeat in such a way all the way to the
		root, you end up producing a shortening sequence of "blurred"
		allocation information. In the root all the information has been
		blurred in a single value. This data structure can be
		easily used for logarithmic searches of free nodes as
		well as modifying a node and updating the heap in logarithmic time.
		*/
	}

	inline void IndexAllocator::swap(IndexAllocator& that)
	{
		data_.swap(that.data_);
		std::swap(allocated_, that.allocated_);
		std::swap(heapHeight_, that.heapHeight_);
	}

	inline void IndexAllocator::clear()
	{
		data_.clear();
		allocated_ = 0;
		heapHeight_ = 0;
	}

	inline void IndexAllocator::reserve(integer count)
	{
		/*!
		If the queried index count is less than or equal to
		current capacity, no action is action. Otherwise
		the heap is grown in whole levels to satisfy the
		query.

		count: 
		Amount of indices to allocate space for.
		*/

		ENSURE_OP(count, >=, 0);

		// Only if a reallocation is needed...
		if (count > capacity())
		{
			integer neededHeight = integerLog2(count + 1) + 2;
			integer toGrow = neededHeight - heapHeight_;
			// toGrow must be positive
			ASSERT1(toGrow > 0, toGrow);

			grow(toGrow);
		}
	}

	inline integer IndexAllocator::size() const
	{
		/*!
		returns: 
		Amount of allocated indices.
		*/
		return allocated_;
	}

	inline integer IndexAllocator::capacity() const
	{
		/*!
		returns:
		Amount of capacity for indices.
		*/
		return (data_.size() + 1) / 2;
	}

	inline bool IndexAllocator::isAllocated(integer index) const
	{
		/*!
		returns: 
		true if the index has already been allocated, false otherwise.
		*/
		PENSURE2(index >= 0 && index < size(),
			index, size());

		return data_[toInternalIndex(index)];
	}

	inline integer IndexAllocator::allocate()
	{
		/*!
		If all indices have been allocated, the heap
		is grown by one level, thus multiplying the
		capacity by 2. Otherwise the current heap is
		used. The heap is searched hierarchically to find
		a free leaf node. If a node contains the constant
		true, all leaf nodes reachable from that node
		are already allocated. A free node can be found
		by traversing the heap tree by always choosing a
		branch that is not true. Because of the
		possible growing, a free leaf node must exist.

		returns: 
		A newly allocated index.
		*/

		// If needed, grow the heap.
		if (allocated_ == capacity())
		{
			grow(1);
		}

		integer index = findFree();
		// A free leaf node must exist.
		ASSERT1(0 <= index && index < data_.size(), index);

		data_[index] = true;
		++allocated_;

		// The heap search structure must be updated in
		// case some branches just became all full.
		update(index);

		return toExternalIndex(index);
	}

	inline void IndexAllocator::deallocate(integer index)
	{
		/*!
		index: 
		Index to deallocate.
		*/

		PENSURE2(index >= 0 && index < capacity(),
			index, capacity());

		integer actualIndex = toInternalIndex(index);

		data_[actualIndex] = false;
		--allocated_;

		// Update the heap in case parent
		// branches were marked allocated.
		update(actualIndex);
	}

	// Private

	inline bool IndexAllocator::isLeaf(integer index) const
	{
		ASSERT2(index >= 0 && index < data_.size(),
			index, data_.size());

		return (index >= (data_.size() - 1) / 2);
	}

	inline integer IndexAllocator::leftChild(integer index) const
	{
		/*
		Given an index pointing to some node in the heap,
		calculate the index of the node's left child.

		index: 
		The heap node index to do the query for.
		
		returns:
		The index of the left child of the given heap node index.
		*/
		ASSERT1(!isLeaf(index), index);

		return (index * 2) + 1;
	}

	inline integer IndexAllocator::rightChild(integer index) const
	{
		/*
		Given an index pointing to some node in the heap,
		calculate the index of the node's right child.

		index: 
		The heap node index to do the query for.
		
		returns:
		The index of the right child of the given heap node index.
		*/
		ASSERT1(!isLeaf(index), index);

		return (index * 2) + 2;
	}

	inline integer IndexAllocator::parent(integer index) const
	{
		/*
		Given an index pointing to some node in the heap,
		calculate the index of the node's parent.

		returns: 
		Index of the parent node of the given heap node index.
		*/
		ASSERT(index != 0);

		return (index - 1) / 2;
	}

	inline integer IndexAllocator::toInternalIndex(integer index) const
	{
		/*
		The real allocation information is in the leaves.
		Leaves of the heap are stored sequentially after the
		half-way of the array. Externally we number the
		leaves from 0 to some N. Internally, we need
		indices to point directly to the physical array.

		index: 
		An external index.
		
		returns:
		An internal index that has been converted from an external index.
		*/

		ASSERT2(index >= 0 && index < capacity(),
			index, capacity());

		return (index + (data_.size() - 1) / 2);
	}

	inline integer IndexAllocator::toExternalIndex(integer index) const
	{
		/*
		Read the documentation for toInternalIndex.

		index: 
		An internal index.
		
		returns: 
		An external index that has been converted from an internal index.
		*/
		ASSERT1(isLeaf(index), index);

		integer result = index - (data_.size() - 1) / 2;

		return result;
	}

	inline void IndexAllocator::grow(integer toGrow)
	{
		/*
		If you imagine the heap as a triangular data structure
		with the root at the top, the growing of the heap
		works as follows: the current heap will become a subtree
		located in the lower left corner of the new, bigger
		heap. This leads to the following algorithm: first
		fill a triangle with free nodes with such a height
		that its leftmost leaf would be the parent of the old heap.
		After this weave together the current heap with the
		new free heap row by row. The algorithm sets the minimum
		size of the heap to 5 levels, that is, 16 identifiers.
		*/
		ASSERT(toGrow > 0);

		integer minHeapHeight = 5;
		integer newHeapHeight = std::max(
			heapHeight_ + toGrow,
			minHeapHeight);
		integer newHeapSize = (1 << newHeapHeight) - 1;

		toGrow = newHeapHeight - heapHeight_;

		integer growHeapSize = (1 << toGrow) - 1;

		std::vector<bool> copy;
		copy.reserve(newHeapSize);

		// Fill the top of the heap with free nodes.
		copy.insert(copy.begin(), growHeapSize, false);

		// Weave free-heap and the current heap
		// together.

		integer padCount = growHeapSize;
		integer count = 1;
		integer index = 0;

		for (integer i = 0;i < heapHeight_;++i)
		{
			// First copy a row of the current heap.
			copy.insert(copy.end(), data_.begin() + index,
				data_.begin() + index + count);

			// Then pad with the free nodes.
			copy.insert(copy.end(), padCount, false);

			index += count;

			count *= 2;
			padCount *= 2;
		}

		ASSERT(index == data_.size());

		// Commit
		data_.swap(copy);
		heapHeight_ = newHeapHeight;
	}

	inline void IndexAllocator::update(integer index)
	{
		/*
		Whenever an identifier is allocated or deallocated,
		the upper levels of the binary heap tree must
		possibly be updated to reflect the new information.
		This is done as follows: move to the parent of the
		current node. If both children of this node are
		allocated, then make this node also allocated,
		otherwise make it free. If the allocation status of
		this node did not change, we can stop, otherwise
		this procedure is repeated recursively up to the root.
		*/
		ASSERT2(0 <= index && index < data_.size(),
			index, data_.size());

		while (index > 0)
		{
			index = parent(index);
			bool previousValue = data_[index];
			bool newValue = false;

			if (data_[leftChild(index)] &&
				data_[rightChild(index)])
			{
				newValue = true;
			}

			if (previousValue == newValue)
			{
				break;
			}

			data_[index] = newValue;
		}
	}

	inline integer IndexAllocator::findFree() const
	{
		/*
		The search begins from the root of the heap tree.
		If the root is allocated, the heap is full and
		does not contain a free node. Otherwise child
		nodes are investigated and the free one chosen
		for a recursive subtree search. In case both children
		are free, the left one is chosen.
		*/
		ASSERT(data_.size() > 0);

		integer index = 0;
		if (data_[index])
		{
			// Out of identifiers.
			return -1;
		}

		for (integer i = 0;i < heapHeight_ - 1;++i)
		{
			if (!data_[leftChild(index)])
			{
				index = leftChild(index);
			}
			else
			{
				index = rightChild(index);
				ASSERT(!data_[index]);
			}
		}

		return index;
	}

	inline void swap(IndexAllocator& left,
		IndexAllocator& right)
	{
		left.swap(right);
	}

}

#endif
