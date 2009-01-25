#ifndef PASTELSYS_NATIVEALLOCATOR_HPP
#define PASTELSYS_NATIVEALLOCATOR_HPP

#include "pastel/sys/nativeallocator.h"

namespace Pastel
{

	inline void swap(NativeAllocator& left,
		NativeAllocator& right)
	{
		left.swap(right);
	}

}

#endif
