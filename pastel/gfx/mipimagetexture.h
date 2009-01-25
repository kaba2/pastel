#ifndef PASTELGFX_MIPIMAGETEXTURE_H
#define PASTELGFX_MIPIMAGETEXTURE_H

#include "pastel/gfx/texture.h"
#include "pastel/gfx/mipmap.h"

#include "pastel/sys/point.h"
#include "pastel/sys/lineararray.h"

#include "pastel/sys/arrayextender.h"

#include <vector>

namespace Pastel
{

	template <typename Type>
	class MipImageTexture
		: public Texture<Type>
	{
	public:
		MipImageTexture();

		explicit MipImageTexture(
			const MipMap<2, Type>& mipMap,
			const ArrayExtender<2, Type>& extender = ArrayExtender<2, Type>());

		Type operator()(
			const Point2& uv,
			const Vector2& dUvDx,
			const Vector2& dUvDy) const;

		void setMipMap(const MipMap<2, Type>& mipMap)
		{
			mipMap_ = &mipMap;
		}

		void setExtender(const ArrayExtender<2, Type>& extender)
		{
			extender_ = extender;
		}

		virtual std::string name() const
		{
			return std::string("MipImage");
		}

	private:
		const MipMap<2, Type>* mipMap_;
		ArrayExtender<2, Type> extender_;
	};

	template <typename Type>
	MipImageTexture<Type> mipImageTexture(
		const MipMap<2, Type>& mipMap)
	{
		return MipImageTexture<Type>(mipMap);
	}

}

#include "pastel/gfx/mipimagetexture.hpp"

#endif
