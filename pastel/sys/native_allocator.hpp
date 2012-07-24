#ifndef PASTEL_NATIVE_ALLOCATOR_HPP
#define PASTEL_NATIVE_ALLOCATOR_HPP

#include "pastel/sys/native_allocator.h"

namespace Pastel
{

	inline void swap(NativeAllocator& left,
		NativeAllocator& right)
	{
		left.swap(right);
	}

}

#endif
