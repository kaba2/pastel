#ifndef PASTEL_MARCHINGTRIANGLES_H
#define PASTEL_MARCHINGTRIANGLES_H

#include "pastel/sys/array.h"

#include "pastel/sys/math_functions.h"
#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

#include "pastel/gfx/gfxrenderer.h"

namespace Pastel
{

	template <typename Real, typename Type>
	void renderMarchingTriangle(
		const Point3& aPosition, const Real& aValue,
		const Point2& bPosition, const Real& bValue,
		const Point2& cPosition, const Real& cValue,
		GfxRenderer<Type>& renderer)
	{
		uint32 mask = 0;

		mask += aValue >= 0 ? 1 : 0;
		mask += bValue >= 0 ? 2 : 0;
		mask += cValue >= 0 ? 4 : 0;

		switch(mask)
		{
		case 0:
			{
				// Do nothing
			}
			break;
		case 1:
			{
				const Point2 abPosition =
					linear(aPosition, bPosition, aValue / (bValue - aValue);
				const Point2 acPosition =
					linear(aPosition, cPosition, aValue / (cValue - aValue);
				renderer.drawTriangle(
					Triangle2(
					aPosition,
					abPosition,
					acPosition));
			}
			break;
		case 2:
			{
				const Point2 bcPosition =
					linear(bPosition, cPosition, bValue / (cValue - bValue);
				const Point2 acPosition =
					linear(aPosition, cPosition, aValue / (cValue - aValue);
				renderer.drawTriangle(
					Triangle2(
					acPosition,
					bcPosition,
					cPosition));
			}
			break;
		case 3:
			{
				const Point2 abPosition =
					linear(aPosition, bPosition, aValue / (bValue - aValue);
				const Point2 acPosition =
					linear(aPosition, cPosition, aValue / (cValue - aValue);

				renderer.drawTriangle(
					Triangle2(
					aPosition,
					bcPosition,
					cPosition));
				renderer.drawTriangle(
					Triangle2(
					abPosition,
					bcPosition,
					cPosition));
			}
			break;
		case 4:
			{
				const Point2 abPosition =
					linear(aPosition, bPosition, aValue / (bValue - aValue);
				const Point2 bcPosition =
					linear(bPosition, cPosition, bValue / (cValue - bValue);
				renderer.drawTriangle(
					Triangle2(
					abPosition,
					bPosition,
					bcPosition));
			}
			break;
		case 5:
			{
				const Point2 bcPosition =
					linear(bPosition, cPosition, bValue / (cValue - bValue);
				const Point2 acPosition =
					linear(aPosition, cPosition, aValue / (cValue - aValue);
				renderer.drawTriangle(
					Triangle2(
					aPosition,
					bcPosition,
					acPosition));
				renderer.drawTriangle(
					Triangle2(
					aPosition,
					bPosition,
					bcPosition));
			}
			break;
		case 6:
			{
				const Point2 abPosition =
					linear(aPosition, bPosition, aValue / (bValue - aValue);
				const Point2 acPosition =
					linear(aPosition, cPosition, aValue / (cValue - aValue);
				renderer.drawTriangle(
					Triangle2(
					abPosition,
					bPosition,
					acPosition));
				renderer.drawTriangle(
					Triangle2(
					bPosition,
					cPosition,
					acPosition));
			}
			break;
		case 7:
			{
				renderer.drawTriangle(
					Triangle2(
					aPosition,
					bPosition,
					cPosition));
			}
			break;
		};
	}

	template <typename Real, typename Type>
	void marchingTriangles(
		const Array<2, Real>& value,
		const AlignedBox2& window,
		GfxRenderer<Type>& renderer)
	{
		const integer width = value.width();
		const integer height = value.height();

		const Vector2 delta = window.max() - window.min();

		const Vector2 positionDelta = delta / Vector2(width - 1, height - 1);
		const Vector2 rightOffset(positionDelta.x(), 0);
		const Vector2 upOffset(0, positionDelta.y());
		const Vector2& rightUpOffset = positionDelta;

		Point2 position = window.min();

		for (integer y = 0; y < height - 1;++y)
		{
			position.x() = window.min().x();

			for (integer x = 0;x < width - 1;++x)
			{
				renderMarchingTriangle(
					position, value(x, y),
					position + rightUpOffset, value(x + 1, y + 1),
					position + upOffset, value(x, y + 1));

				renderMarchingTriangle(
					position, value(x, y),
					position + rightOffset, value(x + 1, y),
					position + rightUpOffset, value(x + 1, y + 1));

				position.x() += positionDelta.x();
			}

			position.y() += positionDelta.y();
		}
	}

}

#endif
