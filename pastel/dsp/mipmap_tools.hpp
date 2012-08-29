#ifndef PASTELDSP_MIPMAP_TOOLS_HPP
#define PASTELDSP_MIPMAP_TOOLS_HPP

#include "pastel/dsp/mipmap_tools.h"

#include "pastel/sys/view_tools.h"

namespace Pastel
{

	template <typename Type, int N, typename TransformFunctor>
	void transform(
		MipMap<Type, N>& mipMap,
		const TransformFunctor& transform)
	{
		const integer images = mipMap.levels();
		for (integer i = 0;i < images;++i)
		{
			Pastel::transform(mipMap.view(i), transform);
		}
	}

}

#endif
