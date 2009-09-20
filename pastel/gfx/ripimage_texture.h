// Description: RipImage_Texture class
// Detail: Image-based texture with bilinear reconstruction and rip-filtering.
// Documentation: texture.txt

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

	template <typename Type>
	class RipImage_Texture
		: public Texture<Type>
	{
	public:
		RipImage_Texture();

		virtual ~RipImage_Texture()
		{
		}

		explicit RipImage_Texture(
			const RipMap<2, Type>& ripMap,
			const ArrayExtender<2, Type>& extender = ArrayExtender<2, Type>());

		virtual Type operator()(
			const Vector2& uv,
			const Vector2& dUvDx,
			const Vector2& dUvDy) const;

		void setRipMap(const RipMap<2, Type>& ripMap)
		{
			ripMap_ = &ripMap;
		}

		void setExtender(const ArrayExtender<2, Type>& extender)
		{
			extender_ = extender;
		}

		virtual std::string name() const
		{
			return std::string("RipImage");
		}

	private:
		const RipMap<2, Type>* ripMap_;
		ArrayExtender<2, Type> extender_;
	};

}

#include "pastel/gfx/ripimage_texture.hpp"

#endif
