#ifndef PASTELGFX_RIPIMAGETEXTURE_H
#define PASTELGFX_RIPIMAGETEXTURE_H

#include "pastel/gfx/texture.h"
#include "pastel/gfx/ripmap.h"

#include "pastel/sys/point.h"
#include "pastel/sys/lineararray.h"
#include "pastel/sys/arrayextender.h"

#include <vector>

namespace Pastel
{

	template <typename Type>
	class RipImageTexture
		: public Texture<Type>
	{
	public:
		RipImageTexture();

		virtual ~RipImageTexture()
		{
		}

		explicit RipImageTexture(
			const RipMap<2, Type>& ripMap,
			const ArrayExtender<2, Type>& extender = ArrayExtender<2, Type>());

		virtual Type operator()(
			const Point2& uv,
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

#include "pastel/gfx/ripimagetexture.hpp"

#endif
