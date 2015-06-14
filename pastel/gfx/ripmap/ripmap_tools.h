// Description: Algorithms for rip-maps

#ifndef PASTELGFX_RIPMAP_TOOLS_H
#define PASTELGFX_RIPMAP_TOOLS_H

#include "pastel/gfx/ripmap/ripmap.h"

namespace Pastel
{

	template <typename Type, integer N, typename TransformFunctor>
	void transform(
		RipMap<Type, N>& ripMap,
		const TransformFunctor& transform);

}

#include "pastel/gfx/ripmap/ripmap_tools.hpp"

#endif
