// Description: Pool allocator
// Detail: A uniform-sized memory allocator

#ifndef PASTELSYS_POOL_ALLOCATOR_H
#define PASTELSYS_POOL_ALLOCATOR_H

#include "pastel/sys/mytypes.h"

#include <boost/operators.hpp>

#include <set>

namespace Pastel
{

	//! A uniform sized memory allocator.
	/*!
	This allocator can be used to allocate constant
	sized memory blocks in a fast and cache friendly manner.
	This allocator avoids memory fragmentation by
	guaranteeing that if there is space available then
	it can also be allocated.
	*/
	class PoolAllocator
		: boost::equality_comparable<
		PoolAllocator
		, boost::less_than_comparable<
		PoolAllocator
		> >
	{
	public:
		//! Constructs the allocator with the given unit size.
		/*!
		Preconditions:
		'unitSize' > 0

		Time complexity: constant
		Exception safety: strong
		*/
		explicit PoolAllocator(integer unitSize);

		//! Destructs the allocator.
		/*!
		Time complexity: linear
		Exception safety: nothrow
		*/
		~PoolAllocator();

		//! Compares two allocators.
		/*!
		The allocators are compared by their
		memory address.

		Time complexity: constant
		Exception safety: nothrow
		*/
		bool operator==(const PoolAllocator& that) const;

		//! Compares two allocators.
		/*!
		The allocators are compared by their
		memory address.

		Time complexity: constant
		Exception safety: nothrow
		*/
		bool operator<(const PoolAllocator& that) const;

		//! Swaps two allocators.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void swap(PoolAllocator& that);

		//! Frees all memory.
		/*!
		Time complexity: linear
		Exception safety: nothrow
		*/
		void clear();

		//! Returns the unit size.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer unitSize() const;

		//! Returns the amount of requested memory.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer allocated() const;

		//! Returns the actual amount of allocated memory.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer capacity() const;

		//! Allocates unitSize-sized memory area.
		/*!
		Time complexity: amortized constant
		Exception safety: strong
		*/
		void* allocate();

		//! Deallocates a previously allocated area.
		/*!
		Preconditions:
		'memAddress' != 0
		'memAddress' has been allocated from this allocator.

		Time complexity: logarithmic
		Exception safety: nothrow
		*/
		void deallocate(const void* memAddress);

	private:
		PoolAllocator(const PoolAllocator& that) = delete;
		PoolAllocator& operator=(const PoolAllocator& that) = delete;

		struct Block
		{
			bool full() const
			{
				return unitsAllocated_ == unitsCapacity_;
			}

			uint8 firstFreeUnit_;
			uint8 unitsAllocated_;
			uint8 unitsCapacity_;
			uint8 padding_;
			Block* nextFreeBlock_;
			Block* previousFreeBlock_;
		};

		using Container = std::set<Block*>;
		using Iterator = Container::iterator;
		using ConstIterator = Container::const_iterator;

		void allocateBlock();
		Iterator deallocateBlock(const Iterator& that);
		Iterator searchBlock(uint8* memAddress);
		bool blockContains(const Block* block,
			const uint8* memAddress) const;
		void pushBackFreeBlock(Block* block);
		void removeFreeBlock(Block* block);

		integer unitSize_;
		integer unitsAllocated_;
		integer unitsCapacity_;
		Block* firstFreeBlock_;
		Block* lastFreeBlock_;
		integer freeBlocks_;
		Iterator deallocationBlock_;
		Container blockList_;
	};

	void swap(PoolAllocator& left, PoolAllocator& right);

}

#include "pastel/sys/pool_allocator.hpp"

#endif
