#ifndef PASTEL_ADDRESSOF_HPP
#define PASTEL_ADDRESSOF_HPP

#include "pastel/sys/addressof.h"

namespace Pastel
{

	template <typename Type>
	void* addressOf(Type& that)
	{
		return (void*)(&((int&)that));
	}

	template <typename Type>
	const void* addressOf(const Type& that)
	{
		return (const void*)(&((const int&)that));
	}

}

#endif
