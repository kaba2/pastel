#ifndef PASTELSYS_ARENA_ALLOCATOR_HPP
#define PASTELSYS_ARENA_ALLOCATOR_HPP

#include "pastel/sys/arena_allocator.h"

namespace Pastel
{

	inline void swap(ArenaAllocator& left,
		ArenaAllocator& right)
	{
		left.swap(right);
	}

}

#endif
