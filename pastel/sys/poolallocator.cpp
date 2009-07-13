#include "pastel/sys/poolallocator.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/log.h"
#include "pastel/sys/addressof.h"

namespace Pastel
{

	PoolAllocator::PoolAllocator(
		integer unitSize)
		: unitSize_(unitSize)
		, unitsAllocated_(0)
		, unitsCapacity_(0)
		, firstFreeBlock_(0)
		, lastFreeBlock_(0)
		, freeBlocks_(0)
		, deallocationBlock_()
		, blockList_()
	{
		ENSURE_OP(unitSize, >, 0);

		deallocationBlock_ = blockList_.end();
	}

	PoolAllocator::~PoolAllocator()
	{
		REPORT1(unitsAllocated_ > 0, unitsAllocated_);

		clear();
	}

	bool PoolAllocator::operator==(
		const PoolAllocator& that) const
	{
		return this == &that;
	}

	bool PoolAllocator::operator<(
		const PoolAllocator& that) const
	{
		return this < &that;
	}

	void PoolAllocator::swap(PoolAllocator& that)
	{
		std::swap(unitSize_, that.unitSize_);
		std::swap(unitsAllocated_, that.unitsAllocated_);
		std::swap(unitsCapacity_, that.unitsCapacity_);
		std::swap(firstFreeBlock_, that.firstFreeBlock_);
		std::swap(lastFreeBlock_, that.lastFreeBlock_);
		std::swap(freeBlocks_, that.freeBlocks_);
		std::swap(deallocationBlock_, that.deallocationBlock_);
		blockList_.swap(that.blockList_);
	}

	void PoolAllocator::clear()
	{
		Iterator iter(blockList_.begin());
		Iterator iterEnd(blockList_.end());
		while (iter != iterEnd)
		{
			Block* block = *iter;
			deallocateRaw((void*)block);
			++iter;
		}

		unitsAllocated_ = 0;
		unitsCapacity_ = 0;
		firstFreeBlock_ = 0;
		lastFreeBlock_ = 0;
		freeBlocks_ = 0;
		blockList_.clear();
		deallocationBlock_ = blockList_.end();
	}

	integer PoolAllocator::unitSize() const
	{
		return unitSize_;
	}

	integer PoolAllocator::allocated() const
	{
		return unitsAllocated_;
	}

	integer PoolAllocator::capacity() const
	{
		return unitsCapacity_;
	}

	void* PoolAllocator::allocate()
	{
		Block* block = firstFreeBlock_;

		if (!block)
		{
			allocateBlock();
			block = firstFreeBlock_;
		}

		// As an invariant, the inspected block
		// should not be full.

		ASSERT(block);
		ASSERT(!block->full());

		// Calculate the memory address of the first
		// free unit inside the block.

		const uint8 firstFreeUnit = block->firstFreeUnit_;
		const integer firstFreeIndex = unitSize_ * firstFreeUnit;
		uint8* memAddress = (uint8*)block +
			sizeof(Block) + firstFreeIndex;

		// Pop the first unit off the internal
		// linked list.

		const uint8 nextFreeIndex = *memAddress;
		block->firstFreeUnit_ = nextFreeIndex;
		++(block->unitsAllocated_);
		++unitsAllocated_;

		if (block->full())
		{
			removeFreeBlock(block);
		}

		// Return the allocated memory address.

		return (void*)memAddress;
	}

	void PoolAllocator::deallocate(void* memAddress)
	{
		// Clearly a null pointer can't
		// be allocated from this allocator.

		PENSURE(memAddress);

		// For convenience, we want to deal with bytes.

		uint8* byteAddress = (uint8*)memAddress;

		// Search for the block that contains
		// the given memory address.

		// First try the cache.
		Iterator iter(deallocationBlock_);
		if (iter == blockList_.end() ||
			!blockContains(*iter, byteAddress))
		{
			// Not found in the cache,
			// search for the right block.
			iter = searchBlock(byteAddress);

			// The memory should be found from some
			// block. Otherwise the deallocation is
			// a bug from the callers side.
			const bool invalidMemoryAddress =
				(iter == blockList_.end());
			PENSURE(!invalidMemoryAddress);
		}

		// Inspect the found block.

		Block* block = *iter;
		ASSERT(block);

		// If the block contains no allocated
		// units, abort. This clearly reflects
		// a bug in the caller's side.

		PENSURE1(block->unitsAllocated_ != 0,
			block->unitsAllocated_);

		uint8* blockBegin = (uint8*)block + sizeof(Block);

		// Calculate the distance in bytes from
		// the block's starting address.

		const integer indexInBytes = byteAddress - blockBegin;

		// If the given memory address is not aligned
		// on unit intervals, abort. This clearly reflects
		// a bug in the caller's side.

		PENSURE3(
			indexInBytes % unitSize_ == 0,
			indexInBytes, unitSize_,
			indexInBytes % unitSize_);

		// Append the internal linked list by
		// the deallocated unit.

		blockBegin[indexInBytes] = block->firstFreeUnit_;

		const integer index = indexInBytes / unitSize_;

		block->firstFreeUnit_ = index;

		// If the block was full, it now
		// becomes free.
		if (block->full())
		{
			pushBackFreeBlock(block);
		}

		--(block->unitsAllocated_);
		--unitsAllocated_;

		deallocationBlock_ = iter;

		// If the block now becomes totally
		// free, check if we can deallocate
		// it. We want to protect ourself
		// against the pattern of someone
		// allocating and deallocating
		// a single unit repeatedly.
		// We do this by making sure
		// there is always at least
		// one free block.

		if (block->unitsAllocated_ == 0 &&
			freeBlocks_ > 1)
		{
			deallocateBlock(iter);
		}
	}

	// Private

	void PoolAllocator::allocateBlock()
	{
		// Allocate memory for the block.

		integer blockSize = unitsAllocated_ >> 1;

		const integer MinBlockSize = 16;
		const integer MaxBlockSize = 255;
		blockSize = std::max(blockSize, MinBlockSize);
		blockSize = std::min(blockSize, MaxBlockSize);

		Block* block = (Block*)allocateRaw(
			sizeof(Block) + unitSize_ * blockSize);

		block->firstFreeUnit_ = 0;
		block->unitsAllocated_ = 0;
		block->unitsCapacity_ = blockSize;
		block->padding_ = 0;
		block->nextFreeBlock_ = 0;
		block->previousFreeBlock_ = 0;

		uint8* data = (uint8*)block + sizeof(Block);

		// Insert the new block in the beginning
		// of the block list.

		try
		{
			blockList_.insert(block);
		}
		catch(...)
		{
			deallocateRaw((void*)block);
			throw;
		}

		// For every unit, its first bytes contains
		// an index into the next unit in the
		// internal linked list.
		// The index is converted into an actual
		// memory address by:
		// addr = blockBegin + index * unitSize_

		for (integer i = 0;i < blockSize;++i)
		{
			data[i * unitSize_] = i + 1;
		}

		unitsCapacity_ += blockSize;

		pushBackFreeBlock(block);
	}

	PoolAllocator::Iterator PoolAllocator::deallocateBlock(
		const Iterator& that)
	{
		ASSERT(that != blockList_.end());

		Block* block = *that;

		unitsCapacity_ -= block->unitsCapacity_;

		if (that == deallocationBlock_)
		{
			deallocationBlock_ = blockList_.end();
		}

		const bool isFreeBlock =
			block == firstFreeBlock_ ||
			block->previousFreeBlock_ != 0 ||
			block->nextFreeBlock_ != 0;

		if (isFreeBlock)
		{
			removeFreeBlock(block);
		}

		deallocateRaw((void*)block);

		Iterator result = that;
		++result;

		blockList_.erase(that);

		return result;
	}

	PoolAllocator::Iterator PoolAllocator::searchBlock(
		uint8* memAddress)
	{
		ASSERT(memAddress);

		// If there are no allocated blocks,
		// return.

		if (blockList_.empty())
		{
			return blockList_.end();
		}

		// Note that using lower_bound here would
		// give wrong results.

		Iterator iter(
			blockList_.upper_bound((Block*)memAddress));

		// If the iterator points to the
		// first block, then it is because
		// the memAddress is out of range.

		if (iter == blockList_.begin())
		{
			return blockList_.end();
		}

		// The previous iterator always exists:
		// the container is not empty and the
		// iterator does not point to the first block.

		--iter;

		if (blockContains(*iter, memAddress))
		{
			return iter;
		}

		return blockList_.end();
	}

	bool PoolAllocator::blockContains(
		const Block* block,
		const uint8* memAddress) const
	{
		ASSERT(block);

		const integer blockSizeInBytes =
			unitSize_ * block->unitsCapacity_;
		const uint8* blockBegin =
			(uint8*)block + sizeof(Block);

		if (blockBegin <= memAddress &&
			memAddress < blockBegin + blockSizeInBytes)
		{
			return true;
		}

		return false;
	}

	void PoolAllocator::pushBackFreeBlock(Block* block)
	{
		ASSERT(block);

		// Append this block
		// to the list of empty blocks.

		ASSERT(block->previousFreeBlock_ == 0);
		ASSERT(block->nextFreeBlock_ == 0);

		if (!lastFreeBlock_)
		{
			ASSERT(!firstFreeBlock_);
			ASSERT1(freeBlocks_ == 0, freeBlocks_);
			firstFreeBlock_ = block;
			lastFreeBlock_ = block;
		}
		else
		{
			ASSERT(!lastFreeBlock_->nextFreeBlock_);
			lastFreeBlock_->nextFreeBlock_ = block;
			block->previousFreeBlock_ = lastFreeBlock_;
			lastFreeBlock_ = block;
		}

		++freeBlocks_;
	}

	void PoolAllocator::removeFreeBlock(Block* block)
	{
		ASSERT(block);

		if (block != firstFreeBlock_ &&
			block != lastFreeBlock_)
		{
			Block* previous = block->previousFreeBlock_;
			Block* next = block->nextFreeBlock_;

			previous->nextFreeBlock_ = next;
			next->previousFreeBlock_ = previous;
		}
		else
		{
			if (block == firstFreeBlock_)
			{
				firstFreeBlock_ = block->nextFreeBlock_;
				if (firstFreeBlock_)
				{
					firstFreeBlock_->previousFreeBlock_ = 0;
				}
			}

			if (block == lastFreeBlock_)
			{
				lastFreeBlock_ = block->previousFreeBlock_;
				if (lastFreeBlock_)
				{
					lastFreeBlock_->nextFreeBlock_ = 0;
				}
			}
		}

		block->previousFreeBlock_ = 0;
		block->nextFreeBlock_ = 0;

		--freeBlocks_;
	}

}
