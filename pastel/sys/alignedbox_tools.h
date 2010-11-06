// Description: Algorithms for aligned boxes

#ifndef PASTEL_ALIGNEDBOX_TOOLS_H
#define PASTEL_ALIGNEDBOX_TOOLS_H

#include "pastel/sys/alignedbox.h"

#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	template <int N, typename PositionVisitor>
	integer forEach(
		const AlignedBox<integer, N>& region,
		PositionVisitor positionVisitor);

	template <typename Real>
	AlignedBox<Real, 3> operator*(
		const AlignedBox<Real, 3>& left,
		const AffineTransformation<Real, 3>& right);

	template <typename Real, int N>
	Vector<Real, N> discreteToContinuous(
		const AlignedBox<Real, N>& continuousRange,
		const AlignedBox<integer, N>& discreteRange,
		const Vector<integer, N>& discretePoint);

	template <typename Real, int N>
	AlignedBox<Real, N> discreteToContinuous(
		const AlignedBox<Real, N>& continuousRange,
		const AlignedBox<integer, N>& discreteRange,
		const AlignedBox<integer, N>& discreteBox);

	template <typename Real, int N>
	Vector<integer, N> continuousToDiscrete(
		const AlignedBox<Real, N>& continuousRange,
		const AlignedBox<integer, N>& discreteRange,
		const Vector<Real, N>& continuousPoint);

	template <typename Real, int N>
	AlignedBox<integer, N> continuousToDiscrete(
		const AlignedBox<Real, N>& continuousRange,
		const AlignedBox<integer, N>& discreteRange,
		const AlignedBox<Real, N>& continuousBox);

}

#include "pastel/sys/alignedbox_tools.hpp"

#endif
