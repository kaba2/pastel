#ifndef PASTEL_REFINABLE_PARTITION_BLOCK_H
#define PASTEL_REFINABLE_PARTITION_BLOCK_H

#include "pastel/sys/refinable_partition.h"

namespace Pastel
{

	template <typename Type>
	class RefinablePartition_Fwd<Type>::Block
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

		Block(const Block& that)
			: begin_(that.begin_)
			, end_(that.end_)
			, unmarkedBegin_(that.unmarkedBegin_)
			, split_(that.split_)
		{
		}

		Block(Block&& that)
			: begin_()
			, end_()
			, unmarkedBegin_()
			, split_()
		{
			swap(that);
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
		template <typename Type>
		friend class RefinablePartition;

		Block()
			: begin_()
			, end_()
			, unmarkedBegin_()
			, split_()
		{
		}

		Block& operator=(Block that)
		{
			swap(that);
			return *this;
		}

		void swap(Block& that)
		{
			using std::swap;
			swap(begin_, that.begin_);
			swap(end_, that.end_);
			swap(unmarkedBegin_, that.unmarkedBegin_);
			swap(split_, that.split_);
		}

		Partition_Iterator begin_;
		Partition_Iterator end_;
		Partition_Iterator unmarkedBegin_;
		Split_Iterator split_;
	};

}

#endif
