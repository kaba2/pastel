#ifndef PASTEL_MYTYPES_HPP
#define PASTEL_MYTYPES_HPP

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Type>
	void unused(const Type&)
	{
		// Do nothing.
	}

	template <typename Type>
	inline PASTEL_ENABLE_IF(boost::is_arithmetic<Type>, Type) 
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
		// Can't use PENSURE here, since
		// that would include ensure.h to
		// mytypes.h.

		//PENSURE_OP(size, >, 0);

		return ::operator new(size);		
	}

	inline void deallocateRaw(const void* data)
	{
		::operator delete((void*)data);
	}

	template <typename Type, typename Compare>
	bool equivalent(
		const Type& left, const Type& right,
		const Compare& compare)
	{
		return !(compare(left, right) || compare(right, left));
	}

	template <typename Type>
	bool equivalent(
		const Type& left, const Type& right)
	{
		return Pastel::equivalent(
			left, right, std::less<Type>());
	}

}

#endif
