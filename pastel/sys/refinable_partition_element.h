#ifndef PASTEL_REFINABLE_PARTITION_ELEMENT_H
#define PASTEL_REFINABLE_PARTITION_ELEMENT_H

#include "pastel/sys/refinable_partition.h"

namespace Pastel
{

	class RefinablePartition_Fwd::Element
	{
	public:
		Element& operator=(Element that)
		{
			swap(that);
			return *this;
		}

		void swap(Element& that)
		{
			using std::swap;
			swap(block_, that.block_);
			swap(position_, that.position_);
		}

		Block_ConstIterator block() const
		{
			return block_;
		}

		Partition_ConstIterator position() const
		{
			return position_;
		}

	private:
		friend class RefinablePartition;

		// Deleted.
		Element();

		Element(
			Block_Iterator block,
			Partition_Iterator position)
			: block_(block)
			, position_(position)
		{
		}

		Block_Iterator block_;
		Partition_Iterator position_;
	};

}

#endif
