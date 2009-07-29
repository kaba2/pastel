#ifndef PASTEL_MYTYPES_HPP
#define PASTEL_MYTYPES_HPP

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Type>
	void unused(const Type&)
	{
	}

	inline float mabs(float that)
	{
		if (that < 0)
		{
			return -that;
		}

		return that;
	}

	inline double mabs(double that)
	{
		if (that < 0)
		{
			return -that;
		}

		return that;
	}

	inline void* allocateRaw(integer size)
	{
		return ::operator new(size);		
	}

	inline void deallocateRaw(const void* data)
	{
		::operator delete((void*)data);
	}

}

#endif
