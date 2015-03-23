#ifndef PASTELSYS_MYTYPES_HPP
#define PASTELSYS_MYTYPES_HPP

#include "pastel/sys/mytypes.h"

#include <functional>
#include <climits>

namespace Pastel
{

	template <typename Type>
	integer sizeInBits()
	{
		return SizeInBits<Type>::value;
	}

	inline void* allocateRaw(integer size)
	{
		//PENSURE_OP(size, >, 0);

		if (size <= 0)
		{
			return 0;
		}

		return ::operator new(size);		
	}

	inline void deallocateRaw(const void* data)
	{
		::operator delete((void*)data);
	}

}

#endif
