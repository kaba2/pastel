#ifndef PASTELSYS_NATIVE_FIELD_H
#define PASTELSYS_NATIVE_FIELD_H

#include "pastel/sys/sfinae_macros.h"

namespace Pastel
{

	// Field

	template <
		typename Type, 
		Requires<std::is_floating_point<Type>> = 0>
	Type inverse(Type that)
	{
		return 1 / that;
	}

}

#endif
