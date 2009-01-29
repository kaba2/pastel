#ifndef PASTELGFX_SATIMAGETEXTURE_HPP
#define PASTELGFX_SATIMAGETEXTURE_HPP

#include "pastel/gfx/satimagetexture.h"
#include "pastel/gfx/summedareatable.h"

#include "pastel/sys/syscommon.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/clampedconstview.h"

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/box.h"
#include "pastel/geometry/bounding_alignedbox.h"

namespace Pastel
{

	template <typename Type>
	SatImageTexture<Type>::SatImageTexture()
		: Texture<Type>()
		, sumImage_()
	{
	}

	template <typename Type>
	template <typename Image_ConstView>
	SatImageTexture<Type>::SatImageTexture(
		const ConstView<2, Type, Image_ConstView>& image)
		: Texture<Type>()
		, sumImage_(image.extent())
	{
		computeSummedAreaTable(image, arrayView(sumImage_));
	}

	template <typename Type>
	Type SatImageTexture<Type>::operator()(
		const Point2& uv,
		const Vector2& dUvDx,
		const Vector2& dUvDy) const
	{
		const Vector2 dx = dUvDx * sumImage_.width();
		const Vector2 dy = dUvDy * sumImage_.height();

		AlignedBox2 bound(
			std::min(std::min(std::min(dx.x(), -dx.x()), dy.x()), -dy.x()),
			std::min(std::min(std::min(dx.y(), -dx.y()), dy.y()), -dy.y()),
			std::max(std::max(std::max(dx.x(), -dx.x()), dy.x()), -dy.x()),
			std::max(std::max(std::max(dx.y(), -dx.y()), dy.y()), -dy.y()));

		bound +=
			asVector(uv) * Vector2(sumImage_.width(), sumImage_.height());

		const Type sum = summedAreaTable(
			clampedConstView(constArrayView(sumImage_)), bound);

		return sum / product(bound.extent());
	}

}

#endif
