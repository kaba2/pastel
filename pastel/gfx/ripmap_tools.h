// Description: Algorithms for rip-maps

#ifndef PASTELGFX_RIPMAP_TOOLS_H
#define PASTELGFX_RIPMAP_TOOLS_H

#include "pastel/gfx/ripmap.h"

namespace Pastel
{

	template <typename Type, int N, typename TransformFunctor>
	void transform(
		RipMap<Type, N>& ripMap,
		const TransformFunctor& transform);

}

#include "pastel/gfx/ripmap_tools.hpp"

#endif
