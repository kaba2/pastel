#ifndef PASTEL_ALIGNEDBOXTOOLS_HPP
#define PASTEL_ALIGNEDBOXTOOLS_HPP

#include "pastel/geometry/alignedbox_tools.h"

namespace Pastel
{

	template <typename Real>
	AlignedBox<3, Real> operator*(
		const AlignedBox<3, Real>& left,
		const AffineTransformation<3, Real>& right)
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

		AlignedBox<3, Real> result(
			min(min(min(min(min(min(min(aPoint,
			bPoint), cPoint), dPoint), ePoint),
			fPoint), gPoint), hPoint),
			max(max(max(max(max(max(max(aPoint,
			bPoint), cPoint), dPoint), ePoint),
			fPoint), gPoint), hPoint));

		return result;
	}

	template <int N, typename Real>
	Point<Real, N> discreteToContinuous(
		const AlignedBox<N, Real>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Point<integer, N>& discretePoint)
	{
		return continuousRange.min() +
			(Vector<Real, N>(discretePoint - discreteRange.min()) /
			Vector<Real, N>(discreteRange.extent())) *
			continuousRange.extent();
	}

	template <int N, typename Real>
	AlignedBox<N, Real> discreteToContinuous(
		const AlignedBox<N, Real>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Rectangle<N>& discreteBox)
	{
		return AlignedBox<N, Real>(
			Pastel::discreteToContinuous(continuousRange, discreteRange, clippedBox.min()),
			Pastel::discreteToContinuous(continuousRange, discreteRange, clippedBox.max()));
	}

	template <int N, typename Real>
	Point<integer, N> continuousToDiscrete(
		const AlignedBox<N, Real>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Point<Real, N>& continuousPoint)
	{
		return
			discreteRange.min() +
			Vector<integer, N>(floor(
			((continuousPoint - continuousRange.min()) / continuousRange.extent()) *
			Vector<Real, N>(discreteRange.extent())));
	}

	template <int N, typename Real>
	Rectangle<N> continuousToDiscrete(
		const AlignedBox<N, Real>& continuousRange,
		const Rectangle<N>& discreteRange,
		const AlignedBox<N, Real>& continuousBox)
	{
		return Rectangle<N>(
			Pastel::continuousToDiscrete(continuousRange, discreteRange, continuousBox.min()),
			Pastel::continuousToDiscrete(continuousRange, discreteRange, continuousBox.max()) + 1);
	}

}

#endif
