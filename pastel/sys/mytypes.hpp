#ifndef PASTEL_MYTYPES_HPP
#define PASTEL_MYTYPES_HPP

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Type>
	void unused(const Type&)
	{
	}

	template <typename Type>
	inline typename boost::enable_if<boost::is_arithmetic<Type>, Type>::type mabs(Type that)
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
