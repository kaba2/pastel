#ifndef PASTELGFX_MIPMAP_TOOLS_HPP
#define PASTELGFX_MIPMAP_TOOLS_HPP

#include "pastel/gfx/mipmap/mipmap_tools.h"

#include "pastel/sys/view/view_tools.h"

namespace Pastel
{

	template <typename Type, integer N, typename TransformFunctor>
	void transform(
		MipMap<Type, N>& mipMap,
		const TransformFunctor& transform)
	{
		integer images = mipMap.levels();
		for (integer i = 0;i < images;++i)
		{
			Pastel::transform(mipMap.view(i), transform);
		}
	}

}

#endif
