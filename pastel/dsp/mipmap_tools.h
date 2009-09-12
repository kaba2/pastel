// Description: Algorithms for MipMap's

#ifndef PASTEL_MIPMAP_TOOLS_H
#define PASTEL_MIPMAP_TOOLS_H

#include "pastel/dsp/mipmap.h"

namespace Pastel
{

	template <int N, typename Type, typename TransformFunctor>
	void transform(
		MipMap<N, Type>& mipMap,
		const TransformFunctor& transform);

}

#include "pastel/dsp/mipmap_tools.hpp"

#endif
