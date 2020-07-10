#ifndef PASTELSYS_NATIVE_FINITE_INTEGER_H
#define PASTELSYS_NATIVE_FINITE_INTEGER_H

#include "pastel/sys/sfinae.h"

namespace Pastel
{

	// Finite integer

	template <typename Type>
	requires std::is_integral_v<Type>
	integer bits(const Type& that)
	{
		return SizeInBits<Type>::value;
	}

}

#endif
