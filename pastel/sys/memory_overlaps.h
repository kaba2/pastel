#ifndef PASTEL_MEMORY_OVERLAPS_H
#define PASTEL_MEMORY_OVERLAPS_H

namespace Pastel
{

	inline bool memoryOverlaps(
		const void* aMemoryBegin, const void* aMemoryEnd,
		const void* bMemoryBegin, const void* bMemoryEnd)
	{
		PENSURE_OP(aMemoryBegin, <=, aMemoryEnd);
		PENSURE_OP(bMemoryBegin, <=, bMemoryEnd);

		return aMemoryEnd > bMemoryBegin &&
			aMemoryBegin < bMemoryEnd;
			
	}

}

#endif
