// Description: Algorithms for mip-maps

#ifndef PASTELGFX_MIPMAP_TOOLS_H
#define PASTELGFX_MIPMAP_TOOLS_H

#include "pastel/gfx/mipmap/mipmap.h"

namespace Pastel
{

	template <typename Type, integer N, typename TransformFunctor>
	void transform(
		MipMap<Type, N>& mipMap,
		const TransformFunctor& transform);

}

#include "pastel/gfx/mipmap/mipmap_tools.hpp"

#endif
