// Description: Overlapping of memory regions

#ifndef PASTELSYS_MEMORY_OVERLAPS_H
#define PASTELSYS_MEMORY_OVERLAPS_H

#include "pastel/sys/ensure.h"

namespace Pastel
{

	inline bool memoryOverlaps(
		const void* aMemoryBegin, const void* aMemoryEnd,
		const void* bMemoryBegin, const void* bMemoryEnd)
	{
		PENSURE(aMemoryBegin <= aMemoryEnd);
		PENSURE(bMemoryBegin <= bMemoryEnd);

		return aMemoryEnd > bMemoryBegin &&
			aMemoryBegin < bMemoryEnd;
			
	}

}

#endif
