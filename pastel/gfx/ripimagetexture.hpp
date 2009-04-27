#ifndef PASTELGFX_RIPIMAGETEXTURE_HPP
#define PASTELGFX_RIPIMAGETEXTURE_HPP

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
		const Point2& uv,
		const Vector2& dUvDx,
		const Vector2& dUvDy) const
	{
		if (!ripMap_ || (*ripMap_).empty())
		{
			return Type();
		}

		const Array<2, Type>& mostDetailedImage = (*ripMap_).mostDetailed();

		const Vector2 dx = dUvDx * Vector2(mostDetailedImage.extent());
		const Vector2 dy = dUvDy * Vector2(mostDetailedImage.extent());

		const Vector2 radius = max(evaluate(mabs(dx)), evaluate(mabs(dy)));

		if (allLessEqual(radius, 1))
		{
			// Magnification: just do bilinear interpolation.

			return sampleBilinear(
				asPoint(asVector(uv) * Vector2(mostDetailedImage.extent())),
				mostDetailedImage, extender_);
		}

		const real invLn2 = inverse(constantLn2<real>());
		const Point2 level(max(evaluate(log(radius) * invLn2), 0));

		if (allLessEqual(level, 0))
		{
			return sampleBilinear(
				asPoint(asVector(uv) * Vector2(mostDetailedImage.extent())),
				mostDetailedImage, extender_);
		}

		if (anyGreaterEqual(asVector(level), Vector2((*ripMap_).levels() - 1)))
		{
			// Return the coarsest ripmap pixel.

			return  (*ripMap_).coarsest()(0, 0);
		}

		// Quadrilinear interpolation.

		const Point<2, integer> level00(floor(asVector(level)));
		const Point<2, integer> level10 = level00 + Vector<2, integer>(1, 0);
		const Point<2, integer> level11 = level00 + Vector<2, integer>(1, 1);
		const Point<2, integer> level01 = level00 + Vector<2, integer>(0, 1);

		const Vector2 tDetail = level - Point2(level00);

		const Array<2, Type>& image00 = (*ripMap_)(level00);
		const Array<2, Type>& image10 = (*ripMap_)(level10);
		const Array<2, Type>& image11 = (*ripMap_)(level11);
		const Array<2, Type>& image01 = (*ripMap_)(level01);

		const Type sample00 =
			sampleBilinear(
			asPoint(asVector(uv) * Vector2(image00.extent())),
			image00, extender_);

		const Type sample10 =
			sampleBilinear(
			asPoint(asVector(uv) * Vector2(image10.extent())),
			image10, extender_);

		const Type sample11 =
			sampleBilinear(
			asPoint(asVector(uv) * Vector2(image11.extent())),
			image11, extender_);

		const Type sample01 =
			sampleBilinear(
			asPoint(asVector(uv) * Vector2(image01.extent())),
			image01, extender_);

		return linear(
			linear(sample00, sample10, tDetail.x()),
			linear(sample01, sample11, tDetail.x()),
			tDetail.y());
	}

}

#endif
