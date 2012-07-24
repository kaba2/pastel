#ifndef PASTEL_REFINABLE_PARTITION_BLOCK_H
#define PASTEL_REFINABLE_PARTITION_BLOCK_H

#include "pastel/sys/refinable_partition.h"

namespace Pastel
{

	class RefinablePartition_Fwd::Block
	{
	public:
		Block(
			Partition_Iterator begin,
			Partition_Iterator end,
			Split_Iterator split)
			: begin_(begin)
			, end_(end)
			, unmarkedBegin_(begin)
			, split_(split)
		{
		}

		integer elements() const
		{
			return end_ - begin_;
		}

		integer marked() const
		{
			return unmarkedBegin_ - begin_;
		}

		integer unmarked() const
		{
			return end_ - unmarkedBegin_;
		}

		Partition_ConstIterator begin() const
		{
			return begin_;
		}

		Partition_ConstIterator end() const
		{
			return end_;
		}

		Partition_ConstIterator unmarkedBegin() const
		{
			return unmarkedBegin_;
		}

		Partition_ConstIterator unmarkedEnd() const
		{
			return end_;
		}

		Partition_ConstIterator markedBegin() const
		{
			return begin_;
		}

		Partition_ConstIterator markedEnd() const
		{
			return unmarkedBegin_;
		}

	private:
		friend class RefinablePartition;

		// Deleted.
		Block();
		// Deleted.
		Block& operator=(const Block& that);

		Partition_Iterator begin_;
		Partition_Iterator end_;
		Partition_Iterator unmarkedBegin_;
		Split_Iterator split_;
	};

}

#endif
