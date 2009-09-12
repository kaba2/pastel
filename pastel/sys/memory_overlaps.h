// Description: Overlapping of memory regions

#ifndef PASTEL_MEMORY_OVERLAPS_H
#define PASTEL_MEMORY_OVERLAPS_H

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
