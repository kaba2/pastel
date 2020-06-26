// Description: Constant color texture
// Documentation: synthetic_textures.txt

#ifndef PASTELGFX_COLOR_TEXTURE_H
#define PASTELGFX_COLOR_TEXTURE_H

#include "pastel/gfx/texture/texture.h"

namespace Pastel
{

	template <typename Type, integer N = 2>
	class Color_Texture
		: public Texture<Type, N>
	{
	public:
		Color_Texture()
			: color_()
		{
		}

		explicit Color_Texture(
			const Type& color)
			: color_(color)
		{
		}

		virtual ~Color_Texture()
		{
		}

		virtual Type operator()(
			const Vector<dreal, N>& p,
			const Matrix<dreal>& m) const
		{
			return color_;
		}

		virtual std::string name() const
		{
			return std::string("Color");
		}

	private:
		Type color_;
	};

	template <typename Type, integer N>
	Color_Texture<Type, N> colorTexture(
		const NoDeduction<Type>& color)
	{
		return Color_Texture<Type, N>(color);
	}

}

#endif
