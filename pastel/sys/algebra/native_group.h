// Description: Native types as a group concept
// DocumentationOf: group_concept.h

#ifndef PASTELSYS_NATIVE_GROUP_H
#define PASTELSYS_NATIVE_GROUP_H

#include "pastel/sys/sfinae.h"

namespace Pastel
{

	// Multiplicative group

	template <typename Type>
	requires std::is_floating_point_v<Type>
	Type inverse(Type that)
	{
		return 1 / that;
	}
	
}

#endif
