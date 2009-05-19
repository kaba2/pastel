#ifndef PASTEL_MIXTEXTURE_H
#define PASTEL_MIXTEXTURE_H

#include "pastel/gfx/texture.h"
#include "pastel/gfx/colormixer.h"

namespace Pastel
{

	template <typename Type, typename Inter_Type>
	class MixTexture
		: public Texture<Type>
	{
	public:
		MixTexture(
			const Texture<Type>& aTexture,
			const Texture<Type>& bTexture,
			const Texture<Inter_Type>& tTexture)
			: aTexture_(aTexture)
			, bTexture_(bTexture)
			, tTexture_(tTexture)
		{
		}

		virtual ~MixTexture()
		{
		}

		virtual Type operator()(
			const Point2& p,
			const Vector2& dpDx,
			const Vector2& dpDy) const
		{
			const Inter_Type t =
				tTexture_(p, dpDx, dpDy);

			return
				aTexture_(p, dpDx, dpDy) * (1 - t) +
				bTexture_(p, dpDx, dpDy) * t;
		}

		virtual std::string name() const
		{
			return std::string("Mix");
		}

	private:
		const Texture<Type>& aTexture_;
		const Texture<Type>& bTexture_;
		const Texture<Inter_Type>& tTexture_;
	};

	template <typename Type, typename Inter_Type>
	MixTexture<Type, Inter_Type> mixTexture(
		const Texture<Type>& aTexture,
		const Texture<Type>& bTexture,
		const Texture<Inter_Type>& tTexture)
	{
		return MixTexture<Type, Inter_Type>(aTexture, bTexture, tTexture);
	}

}

#endif
