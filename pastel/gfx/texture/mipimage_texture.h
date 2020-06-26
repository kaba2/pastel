// Description: MipImage_Texture class
// Detail: Image-based texture with linear reconstruction and mip-filtering.

#ifndef PASTELGFX_MIPIMAGE_TEXTURE_H
#define PASTELGFX_MIPIMAGE_TEXTURE_H

#include "pastel/gfx/texture/texture.h"

#include "pastel/gfx/mipmap/mipmap.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/array.h"

#include "pastel/sys/extender/arrayextender.h"

#include <vector>

namespace Pastel
{

	template <typename Type, integer N = 2>
	class MipImage_Texture
		: public Texture<Type, N>
	{
	public:
		using ArrayExtender_ = ArrayExtender<N, Type>;
		
		MipImage_Texture();

		explicit MipImage_Texture(
			const MipMap<Type, N>& mipMap,
			const ArrayExtender_& extender = ArrayExtender_());

		Type operator()(
			const Vector<dreal, N>& p,
			const Matrix<dreal>& m) const;

		void setMipMap(const MipMap<Type, N>& mipMap)
		{
			mipMap_ = &mipMap;
		}

		void setExtender(const ArrayExtender_& extender)
		{
			extender_ = extender;
		}

		virtual std::string name() const
		{
			return std::string("MipImage");
		}

	private:
		const MipMap<Type, N>* mipMap_;
		ArrayExtender_ extender_;
	};

	template <typename Type, integer N>
	MipImage_Texture<Type, N> mipImageTexture(
		const MipMap<Type, N>& mipMap)
	{
		return MipImage_Texture<Type, N>(mipMap);
	}

}

#include "pastel/gfx/texture/mipimage_texture.hpp"

#endif
