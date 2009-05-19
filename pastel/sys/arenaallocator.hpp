#ifndef PASTEL_ARENAALLOCATOR_HPP
#define PASTEL_ARENAALLOCATOR_HPP

#include "pastel/sys/arenaallocator.h"

namespace Pastel
{

	inline void swap(ArenaAllocator& left,
		ArenaAllocator& right)
	{
		left.swap(right);
	}

}

#endif
