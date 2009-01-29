#ifndef PASTELGFX_RIPMAP_TOOLS_H
#define PASTELGFX_RIPMAP_TOOLS_H

#include "pastel/gfx/ripmap.h"

namespace Pastel
{

	template <int N, typename Type, typename TransformFunctor>
	void transform(
		RipMap<N, Type>& ripMap,
		const TransformFunctor& transform);

}

#include "pastel/gfx/ripmap_tools.hpp"

#endif
