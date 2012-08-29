#ifndef PASTELSYS_POOL_ALLOCATOR_HPP
#define PASTELSYS_POOL_ALLOCATOR_HPP

#include "pastel/sys/pool_allocator.h"

namespace Pastel
{

	inline void swap(PoolAllocator& left,
		PoolAllocator& right)
	{
		left.swap(right);
	}

}

#endif
