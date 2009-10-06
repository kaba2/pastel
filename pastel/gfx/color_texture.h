// Description: Color_Texture class
// Detail: A texture of constant color.
// Documentation: texture.txt

#ifndef PASTEL_COLOR_TEXTURE_H
#define PASTEL_COLOR_TEXTURE_H

#include "pastel/gfx/texture.h"

namespace Pastel
{

	template <typename Type, int N = 2>
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
			const Vector<real, N>& p,
			const Matrix<real, N, N>& m) const
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

	template <typename Type, int N>
	Color_Texture<Type, N> colorTexture(
		const PASTEL_NO_DEDUCTION(Type)& color)
	{
		return Color_Texture<Type, N>(color);
	}

}

#endif
