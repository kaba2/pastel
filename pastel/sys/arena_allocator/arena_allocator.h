// Description: Arena allocator
// Detail: A deallocate-once, arbitrary size memory allocator

#ifndef PASTELSYS_ARENA_ALLOCATOR_H
#define PASTELSYS_ARENA_ALLOCATOR_H

#include "pastel/sys/mytypes.h"

#include <list>

namespace Pastel
{

	//! A deallocate-once, arbitrary size memory allocator.

	class ArenaAllocator
	{
	public:
		//! Constructs with the given parameters.
		/*!
		Time complexity: constant
		Exception safety: strong
		*/
		explicit ArenaAllocator(
			integer unitSize = 1,
			integer blockSize = 256);

		//! Destructs the allocators.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		~ArenaAllocator();

		//! Swaps two allocators.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void swap(ArenaAllocator& that);

		//! Deallocates all memory.
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

		//! Returns the amount of units allocated this far.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer allocated() const;

		//! Returns the amount of units allocated internally.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer capacity() const;

		//! Allocates given number of units of memory.
		/*!
		Preconditions:
		units >= 0

		Time complexity: amortized constant
		Exception safety: nothrow

		If 'units' is zero a null pointer is
		returned.
		*/
		void* allocate(integer units = 1);

		//! Deallocates given number of units of memory
		/*
		Time complexity: constant
		Exception safety: nothrow
		*/
		void deallocate(const void* memoryIgnored, integer units = 1);

	private:
		ArenaAllocator(const ArenaAllocator& that) = delete;
		ArenaAllocator& operator=(const ArenaAllocator& that) = delete;

		struct Block
		{
			uint8* data_;
			integer unitsAllocated_;
			integer unitsReserved_;
		};

		using Container = std::list<Block>;
		using Iterator = Container::iterator;
		using ConstIterator = Container::const_iterator;

		void allocateBlock(integer minUnits);
		void deallocateBlock(Block* block);

		Container blocks_;
		integer unitsAllocated_;
		integer unitsReserved_;
		integer unitSize_;
		integer blockSize_;
	};

	void swap(ArenaAllocator& left, ArenaAllocator& right);

}

#include "pastel/sys/arena_allocator/arena_allocator.hpp"

#endif
