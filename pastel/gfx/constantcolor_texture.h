// Description: ConstantColor_Texture class
// Detail: A texture of constant color.
// Documentation: texture.txt

#ifndef PASTEL_CONSTANTCOLOR_TEXTURE_H
#define PASTEL_CONSTANTCOLOR_TEXTURE_H

#include "pastel/gfx/texture.h"

namespace Pastel
{

	template <typename Type>
	class ConstantColor_Texture
		: public Texture<Type>
	{
	public:
		ConstantColor_Texture()
			: color_()
		{
		}

		explicit ConstantColor_Texture(
			const Type& color)
			: color_(color)
		{
		}

		virtual ~ConstantColor_Texture()
		{
		}

		virtual Type operator()(
			const Vector2& p,
			const Vector2& dpdx,
			const Vector2& dpdy) const
		{
			return color_;
		}

		virtual std::string name() const
		{
			return std::string("ConstantColor");
		}

	private:
		Type color_;
	};

	template <typename Type>
	ConstantColor_Texture<Type> constantColorTexture(
		const PASTEL_NO_DEDUCTION(Type)& color)
	{
		return ConstantColor_Texture<Type>(color);
	}

}

#endif
