// Description: ConstantColorTexture
// Detail: A texture of constant color

#ifndef PASTEL_CONSTANTCOLORTEXTURE_H
#define PASTEL_CONSTANTCOLORTEXTURE_H

#include "pastel/gfx/texture.h"

namespace Pastel
{

	template <typename Type>
	class ConstantColorTexture
		: public Texture<Type>
	{
	public:
		ConstantColorTexture()
			: color_()
		{
		}

		explicit ConstantColorTexture(
			const Type& color)
			: color_(color)
		{
		}

		virtual ~ConstantColorTexture()
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
	ConstantColorTexture<Type> constantColorTexture(
		const PASTEL_NO_DEDUCTION(Type)& color)
	{
		return ConstantColorTexture<Type>(color);
	}

}

#endif
