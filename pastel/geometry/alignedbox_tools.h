// Description: Tools for working with aligned boxes
// Documentation: shapes.txt

#ifndef PASTEL_ALIGNEDBOXTOOLS_H
#define PASTEL_ALIGNEDBOXTOOLS_H

#include "pastel/geometry/alignedbox.h"

#include "pastel/math/affinetransformation.h"

#include "pastel/sys/rectangle.h"

namespace Pastel
{

	template <typename Real>
	AlignedBox<3, Real> operator*(
		const AlignedBox<3, Real>& left,
		const AffineTransformation<3, Real>& right);

	template <int N, typename Real>
	Point<Real, N> discreteToContinuous(
		const AlignedBox<N, Real>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Point<integer, N>& discretePoint);

	template <int N, typename Real>
	AlignedBox<N, Real> discreteToContinuous(
		const AlignedBox<N, Real>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Rectangle<N>& discreteBox);

	template <int N, typename Real>
	Point<integer, N> continuousToDiscrete(
		const AlignedBox<N, Real>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Point<Real, N>& continuousPoint);

	template <int N, typename Real>
	Rectangle<N> continuousToDiscrete(
		const AlignedBox<N, Real>& continuousRange,
		const Rectangle<N>& discreteRange,
		const AlignedBox<N, Real>& continuousBox);

}

#include "pastel/geometry/alignedbox_tools.hpp"

#endif
