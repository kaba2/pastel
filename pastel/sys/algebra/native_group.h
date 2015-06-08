// Description: Native types as a group concept
// DocumentationOf: group_concept.h

#ifndef PASTELSYS_NATIVE_GROUP_H
#define PASTELSYS_NATIVE_GROUP_H

#include "pastel/sys/sfinae.h"

namespace Pastel
{

	// Multiplicative group

	template <
		typename Type, 
		Requires<std::is_floating_point<Type>> = 0>
	Type inverse(Type that)
	{
		return 1 / that;
	}
	
}

#endif
