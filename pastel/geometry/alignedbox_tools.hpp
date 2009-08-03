#ifndef PASTEL_ALIGNEDBOXTOOLS_HPP
#define PASTEL_ALIGNEDBOXTOOLS_HPP

#include "pastel/geometry/alignedbox_tools.h"

namespace Pastel
{

	template <typename Real>
	AlignedBox<Real, 3> operator*(
		const AlignedBox<Real, 3>& left,
		const AffineTransformation<Real, 3>& right)
	{
		Point<Real, 3> aPoint(left.min());
		Point<Real, 3> bPoint(left.max());
		Point<Real, 3> cPoint(aPoint[0], aPoint[1], bPoint[2]);
		Point<Real, 3> dPoint(aPoint[0], bPoint[1], aPoint[2]);
		Point<Real, 3> ePoint(aPoint[0], bPoint[1], bPoint[2]);
		Point<Real, 3> fPoint(bPoint[0], aPoint[1], aPoint[2]);
		Point<Real, 3> gPoint(bPoint[0], aPoint[1], bPoint[2]);
		Point<Real, 3> hPoint(bPoint[0], bPoint[1], aPoint[2]);

		aPoint = aPoint * right;
		bPoint = bPoint * right;
		cPoint = cPoint * right;
		dPoint = dPoint * right;
		ePoint = ePoint * right;
		fPoint = fPoint * right;
		gPoint = gPoint * right;
		hPoint = hPoint * right;

		AlignedBox<Real, 3> result(
			min(min(min(min(min(min(min(aPoint,
			bPoint), cPoint), dPoint), ePoint),
			fPoint), gPoint), hPoint),
			max(max(max(max(max(max(max(aPoint,
			bPoint), cPoint), dPoint), ePoint),
			fPoint), gPoint), hPoint));

		return result;
	}

	template <typename Real, int N>
	Point<Real, N> discreteToContinuous(
		const AlignedBox<Real, N>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Point<integer, N>& discretePoint)
	{
		return continuousRange.min() +
			(Vector<Real, N>(discretePoint - discreteRange.min()) /
			Vector<Real, N>(discreteRange.extent())) *
			continuousRange.extent();
	}

	template <typename Real, int N>
	AlignedBox<Real, N> discreteToContinuous(
		const AlignedBox<Real, N>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Rectangle<N>& discreteBox)
	{
		return AlignedBox<Real, N>(
			Pastel::discreteToContinuous(continuousRange, discreteRange, clippedBox.min()),
			Pastel::discreteToContinuous(continuousRange, discreteRange, clippedBox.max()));
	}

	template <typename Real, int N>
	Point<integer, N> continuousToDiscrete(
		const AlignedBox<Real, N>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Point<Real, N>& continuousPoint)
	{
		return
			discreteRange.min() +
			Vector<integer, N>(floor(
			((continuousPoint - continuousRange.min()) / continuousRange.extent()) *
			Vector<Real, N>(discreteRange.extent())));
	}

	template <typename Real, int N>
	Rectangle<N> continuousToDiscrete(
		const AlignedBox<Real, N>& continuousRange,
		const Rectangle<N>& discreteRange,
		const AlignedBox<Real, N>& continuousBox)
	{
		return Rectangle<N>(
			Pastel::continuousToDiscrete(continuousRange, discreteRange, continuousBox.min()),
			Pastel::continuousToDiscrete(continuousRange, discreteRange, continuousBox.max()) + 1);
	}

}

#endif
