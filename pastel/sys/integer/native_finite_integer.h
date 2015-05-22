#ifndef PASTELSYS_NATIVE_FINITE_INTEGER_H
#define PASTELSYS_NATIVE_FINITE_INTEGER_H

#include "pastel/sys/sfinae.h"

namespace Pastel
{

	// Finite integer

	template <
		typename Type, 
		Requires<std::is_integral<Type>> = 0
	>
	integer bits(const Type& that)
	{
		return SizeInBits<Type>::value;
	}

}

#endif
