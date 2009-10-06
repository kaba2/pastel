// Description: RipImage_Texture class
// Detail: Image-based texture with linear reconstruction and rip-filtering.

#ifndef PASTEL_RIPIMAGE_TEXTURE_H
#define PASTEL_RIPIMAGE_TEXTURE_H

#include "pastel/gfx/texture.h"
#include "pastel/dsp/ripmap.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/array.h"

#include "pastel/sys/arrayextender.h"

#include <vector>

namespace Pastel
{

	template <typename Type, int N = 2>
	class RipImage_Texture
		: public Texture<Type, N>
	{
	public:
		RipImage_Texture();

		virtual ~RipImage_Texture()
		{
		}

		explicit RipImage_Texture(
			const RipMap<Type, N>& ripMap,
			const ArrayExtender<N, Type>& extender = ArrayExtender<N, Type>());

		virtual Type operator()(
			const Vector<real, N>& uv,
			const Matrix<real, N, N>& m) const;

		void setRipMap(const RipMap<Type, N>& ripMap)
		{
			ripMap_ = &ripMap;
		}

		void setExtender(const ArrayExtender<N, Type>& extender)
		{
			extender_ = extender;
		}

		virtual std::string name() const
		{
			return std::string("RipImage");
		}

	private:
		const RipMap<Type, N>* ripMap_;
		ArrayExtender<N, Type> extender_;
	};

}

#include "pastel/gfx/ripimage_texture.hpp"

#endif
