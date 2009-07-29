// Description: ArenaAllocator class
// Detail: A deallocate-once, arbitrary size memory allocator

#ifndef PASTEL_ARENAALLOCATOR_H
#define PASTEL_ARENAALLOCATOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"

#include <list>

namespace Pastel
{

	//! A deallocate-once, arbitrary size memory allocator.

	/*!
	In situations where dynamic allocation and deallocation
	of small objects is frequent and large scale, performance
	suffers an unacceptable penalty. One commonly encountered
	property of these situations is that while allocations
	can happen at any time, deallocation of the objects is
	done all at once. This memory allocator is optimized for
	arbitrary allocation, in both size and time, but
	only simultaneous deallocation.
	This allows for minimal book-keeping and very fast allocation and
	deallocation. The allocator reserves memory in big blocks
	which it then shares in required pieces.
	ArenaAllocator allocates raw memory, construction
	and destruction of objects is left to the user of the memory.
	*/

	class PASTELSYS ArenaAllocator
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
		Time complexity: logarithmic
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
		// Prohibited
		ArenaAllocator(const ArenaAllocator& that);
		// Prohibited
		ArenaAllocator& operator=(const ArenaAllocator& that);

		struct Block
		{
			uint8* data_;
			integer unitsAllocated_;
			integer unitsReserved_;
		};

		typedef std::list<Block> Container;
		typedef Container::iterator Iterator;
		typedef Container::const_iterator ConstIterator;

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

#include "pastel/sys/arenaallocator.hpp"

#endif
