#ifndef PASTEL_POOLALLOCATOR_HPP
#define PASTEL_POOLALLOCATOR_HPP

#include "pastel/sys/poolallocator.h"

namespace Pastel
{

	inline void swap(PoolAllocator& left,
		PoolAllocator& right)
	{
		left.swap(right);
	}

}

#endif
