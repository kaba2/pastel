#ifndef PASTELGEOMETRY_ALIGNEDBOXTOOLS_HPP
#define PASTELGEOMETRY_ALIGNEDBOXTOOLS_HPP

#include "pastel/geometry/alignedbox_tools.h"

namespace Pastel
{

	template <typename Real>
	AlignedBox<3, Real> operator*(
		const AlignedBox<3, Real>& left,
		const AffineTransformation<3, Real>& right)
	{
		Point<3, Real> aPoint(left.min());
		Point<3, Real> bPoint(left.max());
		Point<3, Real> cPoint(aPoint[0], aPoint[1], bPoint[2]);
		Point<3, Real> dPoint(aPoint[0], bPoint[1], aPoint[2]);
		Point<3, Real> ePoint(aPoint[0], bPoint[1], bPoint[2]);
		Point<3, Real> fPoint(bPoint[0], aPoint[1], aPoint[2]);
		Point<3, Real> gPoint(bPoint[0], aPoint[1], bPoint[2]);
		Point<3, Real> hPoint(bPoint[0], bPoint[1], aPoint[2]);

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
	Point<N, Real> discreteToContinuous(
		const AlignedBox<N, Real>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Point<N, integer>& discretePoint)
	{
		return continuousRange.min() +
			(Vector<N, Real>(discretePoint - discreteRange.min()) /
			Vector<N, Real>(discreteRange.extent())) *
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
	Point<N, integer> continuousToDiscrete(
		const AlignedBox<N, Real>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Point<N, Real>& continuousPoint)
	{
		return
			discreteRange.min() +
			Vector<N, integer>(floor(
			((continuousPoint - continuousRange.min()) / continuousRange.extent()) *
			Vector<N, Real>(discreteRange.extent())));
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
