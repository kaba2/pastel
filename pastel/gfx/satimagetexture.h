#ifndef PASTELGFX_SATIMAGETEXTURE_H
#define PASTELGFX_SATIMAGETEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/sys/point.h"
#include "pastel/sys/lineararray.h"

#include <vector>

namespace Pastel
{

	template <typename Type>
	class SatImageTexture
		: public Texture<Type>
	{
	public:
		SatImageTexture();
		
		template <typename Image_ConstView>
		explicit SatImageTexture(
			const ConstView<2, Type, Image_ConstView>& image);

		Type operator()(
			const Point2& uv,
			const Vector2& dUvDx,
			const Vector2& dUvDy) const;

	private:
		LinearArray<2, Type> sumImage_;
	};

}

#include "pastel/gfx/satimagetexture.hpp"

#endif
