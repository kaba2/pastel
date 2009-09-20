// Description: MipImage_Texture class
// Detail: Image-based texture with bilinear reconstruction and mip-filtering.
// Documentation: texture.txt

#ifndef PASTEL_MIPIMAGE_TEXTURE_H
#define PASTEL_MIPIMAGE_TEXTURE_H

#include "pastel/gfx/texture.h"
#include "pastel/dsp/mipmap.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/array.h"

#include "pastel/sys/arrayextender.h"

#include <vector>

namespace Pastel
{

	template <typename Type>
	class MipImage_Texture
		: public Texture<Type>
	{
	public:
		MipImage_Texture();

		explicit MipImage_Texture(
			const MipMap<2, Type>& mipMap,
			const ArrayExtender<2, Type>& extender = ArrayExtender<2, Type>());

		Type operator()(
			const Vector2& uv,
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
	MipImage_Texture<Type> mipImageTexture(
		const MipMap<2, Type>& mipMap)
	{
		return MipImage_Texture<Type>(mipMap);
	}

}

#include "pastel/gfx/mipimage_texture.hpp"

#endif
