// Description: Algorithms for rip-maps

#ifndef PASTEL_RIPMAP_TOOLS_H
#define PASTEL_RIPMAP_TOOLS_H

#include "pastel/dsp/ripmap.h"

namespace Pastel
{

	template <typename Type, int N, typename TransformFunctor>
	void transform(
		RipMap<Type, N>& ripMap,
		const TransformFunctor& transform);

}

#include "pastel/dsp/ripmap_tools.hpp"

#endif
