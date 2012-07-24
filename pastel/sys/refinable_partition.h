// Description: Refinable partition

#ifndef PASTEL_REFINABLE_PARTITION_H
#define PASTEL_REFINABLE_PARTITION_H

#include "pastel/sys/mytypes.h"

#include "pastel/sys/refinable_partition_fwd.h"
#include "pastel/sys/refinable_partition_element.h"
#include "pastel/sys/refinable_partition_block.h"

namespace Pastel
{

	//! Refinable partition
	/*!
	Preconditions:
	Type is move/copy-constructible.
	*/
	template <typename Type>
	class RefinablePartition
		: public RefinablePartition_Fwd<Type>
	{
	public:
		//! Constructs an empty refinable partition.
		/*!
		Time complexity:
		constant

		Exception safety:
		strong
		*/
		RefinablePartition()
			: elementSet_()
			, blockSet_()
			, partitionSet_()
			, splitSet_()
		{
		}

		//! Constructs a refinable partition.
		/*!
		Time complexity:
		O(std::distance(begin, end)) * copy/move-construct(Type)

		Exception safety:
		strong

		Note: if begin == end, there will be no sets in the 
		partition. Otherwise, there will be exactly one set 
		in the partition, containing all the elements.
		*/
		template <typename Type_ConstIterator>
		RefinablePartition(
			const Type_ConstIterator& begin,
			const Type_ConstIterator& end)
			: elementSet_()
			, blockSet_()
			, partitionSet_()
			, splitSet_()
		{
			if (begin == end)
			{
				// Nothing to do.
				return;
			}

			// Create the block. At this point
			// the partitionSet is still empty,
			// so we need to set the block elements
			// later.
			blockSet_.emplace_back(
				Block(
				partitionSet_.begin(),
				partitionSet_.end(),
				splitSet_.end()));

			Block_Iterator block =
				blockSet_.end();
			--block;

			const integer n = std::distance(begin, end);

			// Create the elements and the initial partition.
			elementSet_.reserve(n);
			partitionSet_.reserve(n);
			integer i = 0;
			for (auto iter = begin; iter != end; ++iter)
			{
				elementSet_.emplace_back(
					Element(block, 
					partitionSet_.begin() + i,
					*iter));

				partitionSet_.emplace_back(
					elementSet_.begin() + i);

				++i;
			}

			// Set the block elements properly.
			block->begin_ = partitionSet_.begin();
			block->end_ = partitionSet_.end();
			block->unmarkedBegin_ = block->begin_;
		}

		//! Move-constructs from another refinable partition.
		/*!
		Time complexity:
		constant

		Exception safety:
		strong
		*/

		RefinablePartition(
			RefinablePartition&& that)
			: elementSet_()
			, blockSet_()
			, partitionSet_()
			, splitSet_()
		{
			swap(that);
		}

		//! Assigns from another refinable partition.
		/*!
		Time complexity:
		O(that.elements()), if that was copy-constructed,
		constant, if that was move-constructed.

		Exception safety:
		strong
		*/
		RefinablePartition& operator=(
			RefinablePartition that)
		{
			swap(that);
			return *this;
		}

		//! Removes all elements and blocks.
		/*!
		Time complexity:
		O(elements())

		Exception safety:
		nothrow
		*/
		void clear()
		{
			elementSet_.clear();
			blockSet_.clear();
			partitionSet_.clear();
			splitSet_.clear();
		}

		//! Swaps two refinable partitions.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		void swap(RefinablePartition& that)
		{
			elementSet_.swap(that.elementSet_);
			blockSet_.swap(that.blockSet_);
			partitionSet_.swap(that.partitionSet_);
			splitSet_.swap(that.splitSet_);
		}

		//! Marks an element of a block.
		/*!
		This is a convenience function that calls
		mark(element, true).
		*/
		void mark(const Element_ConstIterator& element)
		{
			mark(element, true);
		}

		//! Unmarks an element of a block.
		/*!
		This is a convenience function that calls
		mark(element, false).
		*/
		void unmark(const Element_ConstIterator& element)
		{
			mark(element, false);
		}

		//! Marks or unmarks an element of a block.
		/*!
		Time complexity:
		constant

		Exception safety:
		nothrow
		*/
		void mark(const Element_ConstIterator& element, bool type)
		{
			PENSURE(element != elementSet_.cend());

			Block_Iterator block = element->block_;

			// An element is marked if and only if it is in the 
			// marked region of the block, i.e. 
			// block->begin <= element->position < block->unmarkedBegin.
			// Since we already know that the element is in its
			// block, the latter test suffices.
			const bool currentType = 
				(element->position_ < block->unmarkedBegin_);

			if (currentType == type)
			{
				// Nothing to do.
				return;
			}

			Element_Iterator& p = *element->position_;
			Element_Iterator& q = *block->unmarkedBegin_;

			if (type)
			{
				// The element is unmarked and we
				// want to mark it.

				if (block->marked() == 0)
				{
					// This is the first marked element
					// of the block. Add the block to be
					// splitted later.
					splitSet_.emplace_back(block);
					
					Split_Iterator split = splitSet_.end();
					--split;
					block->split_ = split;
				}

				// Swap the first unmarked element with
				// the element to be marked.
				std::swap(p, q);
				std::swap(p->position_, q->position_);

				// Then extend the marked region to
				// cover the given element.
				++block->unmarkedBegin_;
			}
			else
			{
				// The element is marked and we
				// want to unmark it.

				// Swap the last marked element with
				// the element to be unmarked.
				std::swap(p, q - 1);
				std::swap(p->position_, (q - 1)->position_);

				// Then shrink the marked region to
				// exclude the given element.
				--block->unmarkedBegin_;

				if (block->marked() == 0)
				{
					// The block no more contains a marked
					// element. Remove the block from the
					// split set.
					splitSet_.erase(block->split_);
					block->split_ = splitSet_.end();
				}
			}
		}

		//! Splits blocks with both marked and unmarked elements.
		/*!
		Time complexity:
		amortized constant

		Exception safety:
		nothrow
		*/
		void split()
		{
			while(!splitSet_.empty())
			{
				Block& block = *splitSet_.back();
				splitSet_.pop_back();
				
				// As an invariant, a block is only pushed in the
				// splitSet if it has at least one marked element.
				// Enforce that.
				ASSERT_OP(block.marked(), >, 0);

				if (block.marked() == block.elements())
				{
					// If all elements have been marked,
					// there is nothing to split.

					// Clear the set of marked elements in the current block.
					block.unmarkedBegin_ = block.begin_;

					continue;
				}

				// This will contain the newly-created
				// block. The other part will reside in
				// the current block.
				Block_Iterator newBlock;

				// Split off the smaller part, and the marked
				// part if of equal size.
				if (block.marked() <= block.unmarked())
				{
					// The marked part is the smaller part.
					// Make it the new block.
					newBlock = blockSet_.emplace(
						blockSet_.cend(),
						Block(block.begin_, block.unmarkedBegin_, 
						splitSet_.end()));
					
					// Use the current block for the larger part.
					block.begin_ = block.unmarkedBegin_;
				}
				else
				{
					// The unmarked part is the smaller part.
					// Make it the new block.
					newBlock = blockSet_.emplace(
						blockSet_.cend(),
						Block(block.unmarkedBegin_, block.end_,
						splitSet_.end()));

					// Use the current block for the larger part.
					block.end_ = block.unmarkedBegin_;
				}

				// Update the block-reference for each element
				// of the block.
				for (auto iter = newBlock->begin();
					iter != newBlock->end();
					++iter)
				{
					(*iter)->block_ = newBlock;
				}
				
				// Clear the set of marked elements in the current block.
				block.unmarkedBegin_ = block.begin_;
			}
		}

		// Blocks

		Block_ConstIterator blockBegin() const
		{
			return blockSet_.cbegin();
		}

		Block_ConstIterator blockEnd() const
		{
			return blockSet_.cend();
		}

		integer blocks() const
		{
			return blockSet_.size();
		}

		// Elements

		Element_ConstIterator elementBegin() const
		{
			return elementSet_.cbegin();
		}

		Element_ConstIterator elementEnd() const
		{
			return elementSet_.cend();
		}

		integer elements() const
		{
			return elementSet_.size();
		}

		// Splits

		Split_ConstIterator splitBegin() const
		{
			return splitSet_.cbegin();
		}

		Split_ConstIterator splitEnd() const
		{
			return splitSet_.cend();
		}

		integer splits() const
		{
			return splitSet_.size();
		}

	private:
		// TODO: Implement
		RefinablePartition(const RefinablePartition& that);

		//! The set of elements.
		ElementSet elementSet_;

		//! The set of blocks.
		BlockSet blockSet_;

		//! The partition of elements.
		PartitionSet partitionSet_;

		//! Blocks with marked elements.
		SplitSet splitSet_;
	};

}

#endif
