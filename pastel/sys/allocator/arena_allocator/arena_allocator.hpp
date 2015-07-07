#ifndef PASTELSYS_ARENA_ALLOCATOR_HPP
#define PASTELSYS_ARENA_ALLOCATOR_HPP

#include "pastel/sys/allocator/arena_allocator.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	inline ArenaAllocator::ArenaAllocator(
		integer unitSize, integer blockSize)
		: blocks_()
		, unitsAllocated_(0)
		, unitsReserved_(0)
		, unitSize_(unitSize)
		, blockSize_(blockSize)
	{
		ENSURE_OP(unitSize, >=, 1);
		ENSURE_OP(blockSize, >=, 1);
	}

	inline ArenaAllocator::~ArenaAllocator()
	{
		REPORT1(unitsAllocated_ != 0, unitsAllocated_);

		clear();
	}

	inline void ArenaAllocator::swap(ArenaAllocator& that)
	{
		blocks_.swap(that.blocks_);
		std::swap(unitsAllocated_, that.unitsAllocated_);
		std::swap(unitsReserved_, that.unitsReserved_);
		std::swap(unitSize_, that.unitSize_);
		std::swap(blockSize_, that.blockSize_);
	}

	inline void ArenaAllocator::clear()
	{
		// For every block..

		Iterator iter(blocks_.begin());
		Iterator iterEnd(blocks_.end());

		while (iter != iterEnd)
		{
			// ..Deallocate the block..

			deallocateBlock(&*iter);
			++iter;
		}

		// And clear the list of blocks.

		blocks_.clear();

		ASSERT1(unitsReserved_ == 0, unitsReserved_);

		unitsAllocated_ = 0;
		unitsReserved_ = 0;
	}

	inline integer ArenaAllocator::unitSize() const
	{
		return unitSize_;
	}

	inline integer ArenaAllocator::allocated() const
	{
		return unitsAllocated_;
	}

	inline integer ArenaAllocator::capacity() const
	{
		return unitsReserved_;
	}

	inline void* ArenaAllocator::allocate(integer units)
	{
		ENSURE_OP(units, >, 0);

		Block* block = 0;
		if (!blocks_.empty())
		{
			block = &blocks_.back();
		}

		if (!block ||
			block->unitsReserved_ - block->unitsAllocated_ < units)
		{
			// Not enough space on this block,
			// allocate another one.
			allocateBlock(units);
			block = &blocks_.back();
		}

		// Calculate the memory address of the first
		// free unit inside the block.
		integer firstFreeIndex = block->unitsAllocated_;

		uint8* const memAddress = block->data_ + firstFreeIndex * unitSize_;

		// Keep up book-keeping.
		block->unitsAllocated_ += units;
		unitsAllocated_ += units;

		// Return the allocated memory address.
		return (void*)memAddress;
	}

	inline void ArenaAllocator::deallocate(const void* memoryIgnored, integer units)
	{
		unused(memoryIgnored);

		unitsAllocated_ -= units;
		REPORT1(unitsAllocated_ < 0, unitsAllocated_);
	}

	// Private

	inline void ArenaAllocator::allocateBlock(integer minUnits)
	{
		ASSERT1(minUnits >= 0, minUnits);

		// The block must be at least of this size.
		integer blockSize = minUnits;

		// Requests larger than or equal to blockSize_ 
		// are satisfied exactly.
		if (blockSize < blockSize_)
		{
			// Otherwise, we seek to possibly allocate 
			// a larger block.

			// The block size must be at least 16.

			integer MinBlockSize = 16;
			blockSize = std::max(blockSize, MinBlockSize);

			// Choose the block size by an 1.5-exponential rule,
			// if it makes the block larger. This gives blocks
			// smoothly increasing sizes at the beginning,
			// rather than allocating 'blockSize_' blocks right
			// from the start. The intent here is to save
			// memory when only a small number of elements
			// is needed.

			blockSize = std::max(unitsAllocated_ >> 1, blockSize);

			// But limit that exponential rule so that it
			// does not exceed blockSize_. 

			blockSize = std::min(blockSize, blockSize_);
		}

		// Allocate and initialize a new
		// block.
		Block block;

		block.data_= (uint8*)allocateRaw(blockSize * unitSize_);
		block.unitsAllocated_ = 0;
		block.unitsReserved_ = blockSize;

		try
		{
			blocks_.push_back(block);
		}
		catch(...)
		{
			deallocateRaw((void*)block.data_);
			throw;
		}

		// Keep up the book-keeping.
		unitsReserved_ += blockSize;
	}

	inline void ArenaAllocator::deallocateBlock(
		Block* block)
	{
		ASSERT(block);

		// Keep up book-keeping.
		unitsReserved_ -= block->unitsReserved_;

		// Delete block's raw memory.
		deallocateRaw((void*)block->data_);
	}

	inline void swap(
		ArenaAllocator& left,
		ArenaAllocator& right)
	{
		left.swap(right);
	}

}

#endif
