#ifndef PASTELGFX_BILINEARCOLORTEXTURE_H
#define PASTELGFX_BILINEARCOLORTEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/math/linear.h"

namespace Pastel
{

	template <typename Type>
	class BilinearColorTexture
		: public Texture<Type>
	{
	public:
		BilinearColorTexture()
			: color00_()
			, color10_()
			, color11_()
			, color01_()
		{
		}

		explicit BilinearColorTexture(
			const Type& color00,
			const Type& color10,
			const Type& color11,
			const Type& color01)
			: color00_(color00)
			, color10_(color10)
			, color11_(color11)
			, color01_(color01)
		{
		}

		virtual ~BilinearColorTexture()
		{
		}

		virtual Type operator()(
			const Point2& p,
			const Vector2& dpdx,
			const Vector2& dpdy) const
		{
			return linear(
				linear(color00_, color10_, p.x()),
				linear(color01_, color11_, p.x()),
				p.y());
		}

		virtual std::string name() const
		{
			return std::string("BilinearColor");
		}

	private:
		Type color00_;
		Type color10_;
		Type color11_;
		Type color01_;
	};

	template <typename Type>
	BilinearColorTexture<Type> bilinearColorTexture(
		const PASTEL_NO_DEDUCTION(Type)& color00,
		const PASTEL_NO_DEDUCTION(Type)& color10,
		const PASTEL_NO_DEDUCTION(Type)& color11,
		const PASTEL_NO_DEDUCTION(Type)& color01)
	{
		return BilinearColorTexture<Type>(color00, color10, color11, color01);
	}

}

#endif
