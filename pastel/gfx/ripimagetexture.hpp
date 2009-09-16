#ifndef PASTEL_RIPIMAGETEXTURE_HPP
#define PASTEL_RIPIMAGETEXTURE_HPP

#include "pastel/gfx/ripimagetexture.h"
#include "pastel/gfx/bilinearimagetexture.h"
#include "pastel/dsp/ripmap.h"

#include "pastel/sys/syscommon.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Type>
	RipImageTexture<Type>::RipImageTexture()
		: ripMap_(0)
		, extender_()
	{
	}

	template <typename Type>
	RipImageTexture<Type>::RipImageTexture(
		const RipMap<2, Type>& ripMap,
		const ArrayExtender<2, Type>& extender)
		: ripMap_(&ripMap)
		, extender_(extender)
	{
	}

	template <typename Type>
	Type RipImageTexture<Type>::operator()(
		const Vector2& uv,
		const Vector2& dUvDx,
		const Vector2& dUvDy) const
	{
		if (!ripMap_ || (*ripMap_).empty())
		{
			return Type();
		}

		const Array<Type, 2>& mostDetailedImage = (*ripMap_).mostDetailed();

		const Vector2 dx = dUvDx * Vector2(mostDetailedImage.extent());
		const Vector2 dy = dUvDy * Vector2(mostDetailedImage.extent());

		const Vector2 radius = max(evaluate(mabs(dx)), evaluate(mabs(dy)));

		if (allLessEqual(radius, 1))
		{
			// Magnification: just do bilinear interpolation.

			return sampleBilinear(
				uv * Vector2(mostDetailedImage.extent()),
				mostDetailedImage, extender_);
		}

		const real invLn2 = inverse(constantLn2<real>());
		const Vector2 level(max(evaluate(log(radius) * invLn2), 0));

		if (allLessEqual(level, 0))
		{
			return sampleBilinear(
				uv * Vector2(mostDetailedImage.extent()),
				mostDetailedImage, extender_);
		}

		if (anyGreaterEqual(level, Vector2((*ripMap_).levels() - 1)))
		{
			// Return the coarsest ripmap pixel.

			return  (*ripMap_).coarsest()(0, 0);
		}

		// Quadrilinear interpolation.

		const Vector<integer, 2> level00(floor(level));
		const Vector<integer, 2> level10 = level00 + Vector<integer, 2>(1, 0);
		const Vector<integer, 2> level11 = level00 + Vector<integer, 2>(1, 1);
		const Vector<integer, 2> level01 = level00 + Vector<integer, 2>(0, 1);

		const Vector2 tDetail = level - Vector2(level00);

		const Array<Type, 2>& image00 = (*ripMap_)(level00);
		const Array<Type, 2>& image10 = (*ripMap_)(level10);
		const Array<Type, 2>& image11 = (*ripMap_)(level11);
		const Array<Type, 2>& image01 = (*ripMap_)(level01);

		const Type sample00 =
			sampleBilinear(
			uv * Vector2(image00.extent()),
			image00, extender_);

		const Type sample10 =
			sampleBilinear(
			uv * Vector2(image10.extent()),
			image10, extender_);

		const Type sample11 =
			sampleBilinear(
			uv * Vector2(image11.extent()),
			image11, extender_);

		const Type sample01 =
			sampleBilinear(
			uv * Vector2(image01.extent()),
			image01, extender_);

		return linear(
			linear(sample00, sample10, tDetail.x()),
			linear(sample01, sample11, tDetail.x()),
			tDetail.y());
	}

}

#endif
