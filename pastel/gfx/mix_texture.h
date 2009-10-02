// Description: Mix_Texture class
// Detail: A point-wise interpolation of two textures with a third.
// Documentation: texture.txt

#ifndef PASTEL_MIX_TEXTURE_H
#define PASTEL_MIX_TEXTURE_H

#include "pastel/gfx/texture.h"
#include "pastel/gfx/colormixer.h"

namespace Pastel
{

	template <typename Type, typename Inter_Type = Type, int N = 2>
	class Mix_Texture
		: public Texture<Type, N>
	{
	public:
		Mix_Texture(
			const Texture<Type, N>& aTexture,
			const Texture<Type, N>& bTexture,
			const Texture<Inter_Type, N>& tTexture)
			: aTexture_(aTexture)
			, bTexture_(bTexture)
			, tTexture_(tTexture)
		{
		}

		virtual ~Mix_Texture()
		{
		}

		virtual Type operator()(
			const Vector<real, N>& p,
			const Matrix<real, N, N>& m) const
		{
			const Inter_Type t =
				tTexture_(p, m);

			return
				aTexture_(p, m) * (1 - t) +
				bTexture_(p, m) * t;
		}

		virtual std::string name() const
		{
			return std::string("Mix");
		}

	private:
		const Texture<Type, N>& aTexture_;
		const Texture<Type, N>& bTexture_;
		const Texture<Inter_Type, N>& tTexture_;
	};

	template <typename Type, typename Inter_Type, int N>
	Mix_Texture<Type, Inter_Type, N> mixTexture(
		const Texture<Type, N>& aTexture,
		const Texture<Type, N>& bTexture,
		const Texture<Inter_Type, N>& tTexture)
	{
		return Mix_Texture<Type, Inter_Type, N>(aTexture, bTexture, tTexture);
	}

}

#endif
