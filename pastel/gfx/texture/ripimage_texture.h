// Description: RipImage_Texture class
// Detail: Image-based texture with linear reconstruction and rip-filtering.

#ifndef PASTELGFX_RIPIMAGE_TEXTURE_H
#define PASTELGFX_RIPIMAGE_TEXTURE_H

#include "pastel/gfx/texture/texture.h"

#include "pastel/gfx/ripmap/ripmap.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/array.h"

#include "pastel/sys/extender/arrayextender.h"

#include <vector>

namespace Pastel
{

	template <typename Type, int N = 2>
	class RipImage_Texture
		: public Texture<Type, N>
	{
	public:
		using ArrayExtender_ = ArrayExtender<N, Type>;
		
		RipImage_Texture();

		virtual ~RipImage_Texture()
		{
		}

		explicit RipImage_Texture(
			const RipMap<Type, N>& ripMap,
			const ArrayExtender_& extender = ArrayExtender_());

		virtual Type operator()(
			const Vector<dreal, N>& uv,
			const Matrix<dreal, N, N>& m) const;

		void setRipMap(const RipMap<Type, N>& ripMap)
		{
			ripMap_ = &ripMap;
		}

		void setExtender(const ArrayExtender_& extender)
		{
			extender_ = extender;
		}

		virtual std::string name() const
		{
			return std::string("RipImage");
		}

	private:
		const RipMap<Type, N>* ripMap_;
		ArrayExtender_ extender_;
	};

}

#include "pastel/gfx/texture/ripimage_texture.hpp"

#endif
