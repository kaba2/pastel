// Description: Algorithms for aligned boxes

#ifndef PASTEL_ALIGNEDBOXTOOLS_H
#define PASTEL_ALIGNEDBOXTOOLS_H

#include "pastel/geometry/alignedbox.h"

#include "pastel/math/affinetransformation.h"

#include "pastel/sys/rectangle.h"

namespace Pastel
{

	template <typename Real>
	AlignedBox<Real, 3> operator*(
		const AlignedBox<Real, 3>& left,
		const AffineTransformation<Real, 3>& right);

	template <typename Real, int N>
	Point<Real, N> discreteToContinuous(
		const AlignedBox<Real, N>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Point<integer, N>& discretePoint);

	template <typename Real, int N>
	AlignedBox<Real, N> discreteToContinuous(
		const AlignedBox<Real, N>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Rectangle<N>& discreteBox);

	template <typename Real, int N>
	Point<integer, N> continuousToDiscrete(
		const AlignedBox<Real, N>& continuousRange,
		const Rectangle<N>& discreteRange,
		const Point<Real, N>& continuousPoint);

	template <typename Real, int N>
	Rectangle<N> continuousToDiscrete(
		const AlignedBox<Real, N>& continuousRange,
		const Rectangle<N>& discreteRange,
		const AlignedBox<Real, N>& continuousBox);

}

#include "pastel/geometry/alignedbox_tools.hpp"

#endif
