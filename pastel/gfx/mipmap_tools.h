#ifndef PASTELGFX_MIPMAP_TOOLS_H
#define PASTELGFX_MIPMAP_TOOLS_H

#include "pastel/gfx/mipmap.h"

namespace Pastel
{

	template <int N, typename Type, typename TransformFunctor>
	void transform(
		MipMap<N, Type>& mipMap,
		const TransformFunctor& transform);

}

#include "pastel/gfx/mipmap_tools.hpp"

#endif
