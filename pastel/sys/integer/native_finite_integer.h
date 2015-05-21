#ifndef PASTELSYS_NATIVE_FINITE_INTEGER_H
#define PASTELSYS_NATIVE_FINITE_INTEGER_H

#include "pastel/sys/sfinae_macros.h"

namespace Pastel
{

	// Finite integer

	template <
		typename Type, 
		EnableIf<std::is_integral<Type>> = 0>
	integer bits(const Type& that)
	{
		return SizeInBits<Type>::value;
	}

}

#endif
