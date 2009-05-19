#ifndef PASTEL_INDEXALLOCATOR_HPP
#define PASTEL_INDEXALLOCATOR_HPP

#include "pastel/sys/indexallocator.h"

namespace Pastel
{

	inline void swap(IndexAllocator& left,
		IndexAllocator& right)
	{
		left.swap(right);
	}

}

#endif
