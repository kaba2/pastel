// Description: Raw memory allocation
// DocumentationOf: environment.h

#ifndef PASTELSYS_ALLOCATION_H
#define PASTELSYS_ALLOCATION_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Allocates a raw memory block.
	/*!
	size:
	The size of the memory block in bytes.
	*/
	inline void* allocateRaw(integer size)
	{
		return size > 0 
			? ::operator new(size)
			: nullptr;
	}

	//! Deallocates a raw memory block.
	inline void deallocateRaw(const void* data)
	{
		::operator delete((void*)data);
	}

}

#endif
