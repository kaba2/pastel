#ifndef PASTELGFX_COMBINETEXTURE_H
#define PASTELGFX_COMBINETEXTURE_H

#include "pastel/gfx/texture.h"
#include "pastel/gfx/colormixer.h"

namespace Pastel
{

	template <typename Type>
	class CombineTexture
		: public Texture<Type>
	{
	public:
		CombineTexture(
			const Texture<Type>& aSampler,
			const Texture<Type>& bSampler,
			const ColorMixer<Type>& colorMixer)
			: aSampler_(aSampler)
			, bSampler_(bSampler)
			, colorMixer_(colorMixer)
		{
		}

		virtual ~CombineTexture() 
		{
		}

		virtual Type operator()(
			const Point2& p, 
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
	CombineTexture<Type> combineTexture(
		const Texture<Type>& aSampler,
		const Texture<Type>& bSampler,
		const ColorMixer<Type>& colorMixer)
	{
		return CombineTexture<Type>(aSampler, bSampler, colorMixer);
	}

}

#endif
