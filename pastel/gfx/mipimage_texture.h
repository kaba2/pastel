// Description: MipImage_Texture class
// Detail: Image-based texture with linear reconstruction and mip-filtering.

#ifndef PASTELGFX_MIPIMAGE_TEXTURE_H
#define PASTELGFX_MIPIMAGE_TEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/dsp/mipmap.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/array.h"

#include "pastel/sys/arrayextender.h"

#include <vector>

namespace Pastel
{

	template <typename Type, int N = 2>
	class MipImage_Texture
		: public Texture<Type, N>
	{
	public:
		MipImage_Texture();

		explicit MipImage_Texture(
			const MipMap<Type, N>& mipMap,
			const ArrayExtender<N, Type>& extender = ArrayExtender<N, Type>());

		Type operator()(
			const Vector<real, N>& p,
			const Matrix<real, N, N>& m) const;

		void setMipMap(const MipMap<Type, N>& mipMap)
		{
			mipMap_ = &mipMap;
		}

		void setExtender(const ArrayExtender<N, Type>& extender)
		{
			extender_ = extender;
		}

		virtual std::string name() const
		{
			return std::string("MipImage");
		}

	private:
		const MipMap<Type, N>* mipMap_;
		ArrayExtender<N, Type> extender_;
	};

	template <typename Type, int N>
	MipImage_Texture<Type, N> mipImageTexture(
		const MipMap<Type, N>& mipMap)
	{
		return MipImage_Texture<Type, N>(mipMap);
	}

}

#include "pastel/gfx/mipimage_texture.hpp"

#endif
