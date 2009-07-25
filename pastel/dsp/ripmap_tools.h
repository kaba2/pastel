// Description: Tools for working with ripmaps

#ifndef PASTEL_RIPMAP_TOOLS_H
#define PASTEL_RIPMAP_TOOLS_H

#include "pastel/dsp/ripmap.h"

namespace Pastel
{

	template <int N, typename Type, typename TransformFunctor>
	void transform(
		RipMap<N, Type>& ripMap,
		const TransformFunctor& transform);

}

#include "pastel/dsp/ripmap_tools.hpp"

#endif
