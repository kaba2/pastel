#ifndef PASTELSYS_NATIVE_MONOID_H
#define PASTELSYS_NATIVE_MONOID_H

#include "pastel/sys/sfinae.h"

namespace Pastel
{

	// Additive monoid

	template <
		typename Type, 
		Requires<std::is_arithmetic<Type>> = 0>
	bool zero(Type that)
	{
		return that == 0;
	}

	// Multiplicative monoid

	template <
		typename Type, 
		Requires<std::is_arithmetic<Type>> = 0>
	bool one(Type that)
	{
		return that == 1;
	}

}

#endif
