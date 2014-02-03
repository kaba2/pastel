#ifndef PASTELSYS_MYTYPES_HPP
#define PASTELSYS_MYTYPES_HPP

#include "pastel/sys/mytypes.h"

#include <functional>
#include <climits>

namespace Pastel
{

	template <typename Type>
	void unused(const Type&)
	{
		// Do nothing.
	}

	template <typename Type>
	const Type& addConst(Type& that)
	{
		return (const Type&)that;
	}

	template <typename Type>
	Type& removeConst(const Type& that)
	{
		return (Type&)that;
	}

	template <typename Type>
	integer sizeInBits()
	{
		// Note that 
		// std::numeric_limits<Type>::digits is 
		// CHAR_BIT for unsigned integers, and
		// CHAR_BIT - 1 for signed integers.
		// So CHAR_BIT is really what we want 
		// to use here.
		return sizeof(Type) * CHAR_BIT;
	}

	template <typename Type>
	inline PASTEL_ENABLE_IF(std::is_arithmetic<Type>, Type) 
		mabs(Type that)
	{
		if (that < 0)
		{
			return -that;
		}

		return that;
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
