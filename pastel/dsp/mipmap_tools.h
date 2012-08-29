// Description: Algorithms for mip-maps

#ifndef PASTELDSP_MIPMAP_TOOLS_H
#define PASTELDSP_MIPMAP_TOOLS_H

#include "pastel/dsp/mipmap.h"

namespace Pastel
{

	template <typename Type, int N, typename TransformFunctor>
	void transform(
		MipMap<Type, N>& mipMap,
		const TransformFunctor& transform);

}

#include "pastel/dsp/mipmap_tools.hpp"

#endif
