#ifndef PASTELGFX_MIPIMAGETEXTURE_HPP
#define PASTELGFX_MIPIMAGETEXTURE_HPP

#include "pastel/gfx/mipimagetexture.h"
#include "pastel/gfx/bilinearimagetexture.h"
#include "pastel/gfx/mipmap.h"

#include "pastel/sys/syscommon.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/repeatedconstview.h"
#include "pastel/sys/view_tools.h"

namespace Pastel
{

	template <typename Type>
	MipImageTexture<Type>::MipImageTexture()
		: mipMap_(0)
		, extender_()
	{
	}

	template <typename Type>
	MipImageTexture<Type>::MipImageTexture(
		const MipMap<2, Type>& mipMap,
		const ArrayExtender<2, Type>& extender)
		: mipMap_(&mipMap)
		, extender_(extender)
	{
	}

	template <typename Type>
	Type MipImageTexture<Type>::operator()(
		const Point2& uv,
		const Vector2& dUvDx,
		const Vector2& dUvDy) const
	{
		if (!mipMap_ || (*mipMap_).empty())
		{
			return Type();
		}

		const Array<2, Type>& mostDetailedImage = (*mipMap_).mostDetailed();

		const Vector2 dx = dUvDx * Vector2(mostDetailedImage.extent());
		const Vector2 dy = dUvDy * Vector2(mostDetailedImage.extent());

		const real d = std::max(dot(dx), dot(dy));
		const real invLn2 = inverse(constantLn2<real>());
		const real level = 0.5 * std::log(d) * invLn2;

		if (level <= 0)
		{
			// Magnification: just do bilinear interpolation.

			return sampleBilinear(
				asPoint(asVector(uv) * Vector2(mostDetailedImage.extent())),
				mostDetailedImage, extender_);
		}

		if (level >= (*mipMap_).levels() - 1)
		{
			// Return the coarsest mipmap pixel.

			return  (*mipMap_).coarsest()(0, 0);
		}

		// The normal case: trilinear interpolation.

		const integer detailLevel = std::floor(level);
		const integer coarseLevel = detailLevel + 1;

		const real tDetail = level - detailLevel;

		const Array<2, Type>& detailImage = (*mipMap_)(detailLevel);
		const Array<2, Type>& coarseImage = (*mipMap_)(coarseLevel);

		const Type detailSample =
			sampleBilinear(
			asPoint(asVector(uv) * Vector2(detailImage.extent())),
			detailImage, extender_);

		const Type coarseSample =
			sampleBilinear(
			asPoint(asVector(uv) * Vector2(coarseImage.extent())),
			coarseImage, extender_);

		return linear(detailSample, coarseSample, tDetail);
	}

}

#endif
