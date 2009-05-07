#ifndef PASTELSYS_BLOCKARRAY_HPP
#define PASTELSYS_BLOCKARRAY_HPP

#include "pastel/sys/blockarray.h"
#include "pastel/sys/destruct.h"
#include "pastel/sys/ensure.h"

#include <memory>

namespace Pastel
{

	template <typename Type, int Log2BlockSize>
	BlockArray<Type, Log2BlockSize>::BlockArray()
		: block_()
		, size_(0)
		, lastBlockSize_(0)
	{
	}

	template <typename Type, int Log2BlockSize>
	BlockArray<Type, Log2BlockSize>::BlockArray(integer elements, const Type& data)
		: block_()
		, size_(0)
		, lastBlockSize_(0)
	{
		resize(elements, data);
	}

	template <typename Type, int Log2BlockSize>
	BlockArray<Type, Log2BlockSize>::BlockArray(const BlockArray& that)
		: block_()
		, size_(0)
		, lastBlockSize_(0)
	{
		try
		{
			const integer blocks = that.block_.size();
			if (blocks > 1)
			{
				for (integer i = 0;i < blocks - 1;++i)
				{
					constructElements(BlockSize, that.block_[i]);
				}

				constructElements(that.lastBlockSize_, that.block_[blocks - 1]);
			}
		}
		catch(...)
		{
			clear();
			throw;
		}
	}

	template <typename Type, int Log2BlockSize>
	BlockArray<Type, Log2BlockSize>::~BlockArray()
	{
		clear();
	}

	template <typename Type, int Log2BlockSize>
	BlockArray<Type, Log2BlockSize> BlockArray<Type, Log2BlockSize>::operator=(const BlockArray& that)
	{
		BlockArray copy(that);
		swap(copy);
		return *this;
	}

	template <typename Type, int Log2BlockSize>
	void BlockArray<Type, Log2BlockSize>::swap(BlockArray& that)
	{
		block_.swap(that.block_);
		std::swap(size_, that.size_);
		std::swap(lastBlockSize_, that.lastBlockSize_);
	}

	template <typename Type, int Log2BlockSize>
	void BlockArray<Type, Log2BlockSize>::clear()
	{
		destructElements(size_);

		ASSERT1(size_ == 0, size_);
		ASSERT1(lastBlockSize_ == 0, lastBlockSize_);
	}

	template <typename Type, int Log2BlockSize>
	const Type& BlockArray<Type, Log2BlockSize>::operator[](integer index) const
	{
		PENSURE1(index >= 0, index);
		PENSURE2(index < size_, index, size_);

		const integer blockIndex = index >> BlockShift;
		const integer inBlockIndex = index & BlockMask;

		return block_[blockIndex][inBlockIndex];
	}

	template <typename Type, int Log2BlockSize>
	Type& BlockArray<Type, Log2BlockSize>::operator[](integer index)
	{
		return (Type&)((const BlockArray&)*this)[index];
	}

	template <typename Type, int Log2BlockSize>
	void BlockArray<Type, Log2BlockSize>::resize(
		integer size,
		const Type& data)
	{
		ENSURE1(size >= 0, size);

		if (size == 0)
		{
			clear();
			return;
		}

		if (size > size_)
		{
			constructElements(size - size_, data);
		}
		else if (size < size_)
		{
			destructElements(size_ - size);
		}

		ASSERT2(size_ == size, size_, size);
	}

	template <typename Type, int Log2BlockSize>
	void BlockArray<Type, Log2BlockSize>::push_back(const Type& data)
	{
		constructElements(1, data);
	}

	template <typename Type, int Log2BlockSize>
	void BlockArray<Type, Log2BlockSize>::pop_back()
	{
		PENSURE1(size_ >= 1, size_);

		destructElements(1);
	}

	template <typename Type, int Log2BlockSize>
	Type& BlockArray<Type, Log2BlockSize>::front()
	{
		return (*this)[0];
	}

	template <typename Type, int Log2BlockSize>
	const Type& BlockArray<Type, Log2BlockSize>::front() const
	{
		return (*this)[0];
	}

	template <typename Type, int Log2BlockSize>
	Type& BlockArray<Type, Log2BlockSize>::back()
	{
		return (*this)[size_ - 1];
	}

	template <typename Type, int Log2BlockSize>
	const Type& BlockArray<Type, Log2BlockSize>::back() const
	{
		return (*this)[size_ - 1];
	}

	template <typename Type, int Log2BlockSize>
	integer BlockArray<Type, Log2BlockSize>::size() const
	{
		return size_;
	}

	template <typename Type, int Log2BlockSize>
	integer BlockArray<Type, Log2BlockSize>::capacity() const
	{
		return block_.size() * BlockSize;
	}

	// Private

	template <typename Type, int Log2BlockSize>
	void BlockArray<Type, Log2BlockSize>::addBlock()
	{
		ASSERT2((block_.empty() && lastBlockSize_ == 0) ||
			lastBlockSize_ == BlockSize, lastBlockSize_, BlockSize);

		void* newBlock = allocateRaw(sizeof(Type) * BlockSize);
		Type* typedNewBlock = (Type*)newBlock;

		try
		{
			block_.push_back(typedNewBlock);
		}
		catch(...)
		{
			deallocateRaw(newBlock);
			throw;
		}

		lastBlockSize_ = 0;
	}

	template <typename Type, int Log2BlockSize>
	void BlockArray<Type, Log2BlockSize>::removeBlock()
	{
		ASSERT1(lastBlockSize_ == 0, lastBlockSize_);

		Type* block = block_.back();

		deallocateRaw((void*)block);

		block_.pop_back();

		if (block_.empty())
		{
			lastBlockSize_ = 0;
		}
		else
		{
			lastBlockSize_ = BlockSize;
		}
	}

	template <typename Type, int Log2BlockSize>
	void BlockArray<Type, Log2BlockSize>::constructElements(integer elements, const Type& data)
	{
		ASSERT1(elements >= 0, elements);

		integer elementsToCreate = elements;

		try
		{
			while(elementsToCreate > 0)
			{
				if (block_.empty() ||
					lastBlockSize_ == BlockSize)
				{
					addBlock();
				}

				Type* block = block_.back();

				const integer maxCreation =
					std::min(BlockSize - lastBlockSize_, elementsToCreate);

				std::uninitialized_fill(block + lastBlockSize_,
					block + lastBlockSize_ + maxCreation, data);

				lastBlockSize_ += maxCreation;
				size_ += maxCreation;
				elementsToCreate -= maxCreation;
			}
		}
		catch(...)
		{
			destructElements(elements - elementsToCreate);
			throw;
		}
	}

	template <typename Type, int Log2BlockSize>
	void BlockArray<Type, Log2BlockSize>::constructElements(integer elements, const Type* sourceBlock)
	{
		ASSERT2(elements >= 0 && elements <= BlockSize, elements, BlockSize);

		integer elementsToCreate = elements;

		try
		{
			while(elementsToCreate > 0)
			{
				if (block_.empty() ||
					lastBlockSize_ == BlockSize)
				{
					addBlock();
				}

				Type* block = block_.back();

				const integer maxCreation =
					std::min(BlockSize - lastBlockSize_, elementsToCreate);

				std::uninitialized_copy(
					sourceBlock, sourceBlock + maxCreation, block);

				lastBlockSize_ += maxCreation;
				size_ += maxCreation;
				elementsToCreate -= maxCreation;
			}
		}
		catch(...)
		{
			destructElements(elements - elementsToCreate);
			throw;
		}
	}

	template <typename Type, int Log2BlockSize>
	void BlockArray<Type, Log2BlockSize>::destructElements(integer elements)
	{
		ASSERT2(elements >= 0 && elements <= size_, elements, size_);

		integer elementsToRemove = elements;

		while(elementsToRemove > 0)
		{
			Type* block = block_.back();

			const integer maxRemoval =
				std::min(lastBlockSize_, elementsToRemove);

			const integer startIndex = lastBlockSize_ - maxRemoval;

			destruct(block + startIndex, block + lastBlockSize_);

			lastBlockSize_ -= maxRemoval;

			if (lastBlockSize_ == 0)
			{
				removeBlock();
			}

			elementsToRemove -= maxRemoval;
		}

		size_ -= elements;
	}

}

#endif
