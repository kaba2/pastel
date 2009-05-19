#ifndef PASTEL_BILINEARIMAGETEXTURE_HPP
#define PASTEL_BILINEARIMAGETEXTURE_HPP

#include "pastel/gfx/bilinearimagetexture.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Type>
	Type sampleBilinear(
		const Point2& uv,
		const Array<2, Type>& image,
		const ArrayExtender<2, Type>& extender)
	{
		if (image.empty())
		{
			return Type();
		}

		const Point<2, integer> x(floor(asVector(uv) - 0.5));

		const real u = (uv[0] - 0.5) - x.x();
		const real v = (uv[1] - 0.5) - x.y();

		return linear(
			linear(extender(image, x), extender(image, x + IVector2(1, 0)), u),
			linear(extender(image, x + IVector2(0, 1)), extender(image, x + IVector2(1)), u),
			v);
	}

}

#endif
