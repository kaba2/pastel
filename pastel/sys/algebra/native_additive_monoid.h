#ifndef PASTELSYS_NATIVE_ADDITIVE_MONOID_H
#define PASTELSYS_NATIVE_ADDITIVE_MONOID_H

#include "pastel/sys/sfinae_macros.h"

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

}

#endif
