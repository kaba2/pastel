// Description: Point-wise combination of two textures
// Documentation: modifier_textures.txt

#ifndef PASTELGFX_COMBINE_TEXTURE_H
#define PASTELGFX_COMBINE_TEXTURE_H

#include "pastel/gfx/texture.h"
#include "pastel/gfx/colormixer.h"

namespace Pastel
{

	template <typename Type, int N = 2>
	class Combine_Texture
		: public Texture<Type, N>
	{
	public:
		Combine_Texture(
			const Texture<Type, N>& aTexture,
			const Texture<Type, N>& bTexture,
			const ColorMixer<Type>& colorMixer)
			: aTexture_(aTexture)
			, bTexture_(bTexture)
			, colorMixer_(colorMixer)
		{
		}

		virtual ~Combine_Texture()
		{
		}

		virtual Type operator()(
			const Vector<real, N>& p,
			const Matrix<real, N, N>& m) const
		{
			return colorMixer_(
				aTexture_(p, m),
				bTexture_(p, m));
		}

		virtual std::string name() const
		{
			return std::string("Combine(" + aTexture_.fullName() + 
				", " + bTexture_.fullName() + ")");
		}

	private:
		const Texture<Type, N>& aTexture_;
		const Texture<Type, N>& bTexture_;
		const ColorMixer<Type>& colorMixer_;
	};

	template <typename Type, int N>
	Combine_Texture<Type, N> combineTexture(
		const Texture<Type, N>& aTexture,
		const Texture<Type, N>& bTexture,
		const ColorMixer<Type>& colorMixer)
	{
		return Combine_Texture<Type, N>(aTexture, bTexture, colorMixer);
	}

}

#endif
