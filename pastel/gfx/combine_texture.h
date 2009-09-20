// Description: Combine_Texture class
// Detail: A point-wise combination of two textures.
// Documentation: texture.txt

#ifndef PASTEL_COMBINE_TEXTURE_H
#define PASTEL_COMBINE_TEXTURE_H

#include "pastel/gfx/texture.h"
#include "pastel/gfx/colormixer.h"

namespace Pastel
{

	template <typename Type>
	class Combine_Texture
		: public Texture<Type>
	{
	public:
		Combine_Texture(
			const Texture<Type>& aSampler,
			const Texture<Type>& bSampler,
			const ColorMixer<Type>& colorMixer)
			: aSampler_(aSampler)
			, bSampler_(bSampler)
			, colorMixer_(colorMixer)
		{
		}

		virtual ~Combine_Texture()
		{
		}

		virtual Type operator()(
			const Vector2& p,
			const Vector2& dpDx,
			const Vector2& dpDy) const
		{
			return colorMixer_(
				aSampler_(p, dpDx, dpDy),
				bSampler_(p, dpDx, dpDy));
		}

		virtual std::string name() const
		{
			return std::string("Combine");
		}

	private:
		const Texture<Type>& aSampler_;
		const Texture<Type>& bSampler_;
		const ColorMixer<Type>& colorMixer_;
	};

	template <typename Type>
	Combine_Texture<Type> combineTexture(
		const Texture<Type>& aSampler,
		const Texture<Type>& bSampler,
		const ColorMixer<Type>& colorMixer)
	{
		return Combine_Texture<Type>(aSampler, bSampler, colorMixer);
	}

}

#endif
