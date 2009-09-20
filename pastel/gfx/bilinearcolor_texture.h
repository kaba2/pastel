// Description: BilinearColor_Texture class
// Detail: A bilinear color patch texture.
// Documentation: texture.txt

#ifndef PASTEL_BILINEARCOLOR_TEXTURE_H
#define PASTEL_BILINEARCOLOR_TEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Type>
	class BilinearColor_Texture
		: public Texture<Type>
	{
	public:
		BilinearColor_Texture()
			: color00_()
			, color10_()
			, color11_()
			, color01_()
		{
		}

		explicit BilinearColor_Texture(
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

		virtual ~BilinearColor_Texture()
		{
		}

		virtual Type operator()(
			const Vector2& p,
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
	BilinearColor_Texture<Type> bilinearColorTexture(
		const PASTEL_NO_DEDUCTION(Type)& color00,
		const PASTEL_NO_DEDUCTION(Type)& color10,
		const PASTEL_NO_DEDUCTION(Type)& color11,
		const PASTEL_NO_DEDUCTION(Type)& color01)
	{
		return BilinearColor_Texture<Type>(color00, color10, color11, color01);
	}

}

#endif
