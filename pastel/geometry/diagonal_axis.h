#ifndef PASTEL_DIAGONAL_AXIS_H
#define PASTEL_DIAGONAL_AXIS_H

namespace Pastel
{
	typedef uint32 DiagonalAxis;

	//! Returns the variance along each axis.

	template <int N, typename Real>
	TemporaryPoint<N, Real> mean(
		const std::vector<Point<N, Real> >& pointSet);

	template <int N, typename Real>
	TemporaryVector<N, Real> axisAlignedVariance(
		const std::vector<Point<N, Real> >& pointSet,
		const Point<N, Real>& mean);

	template <int N, typename Real>
	DiagonalAxis maximalDiagonalVariance(
		const std::vector<Point<N, Real> >& pointSet);

	template <int N, typename Real>
	DiagonalAxis nearestDiagonalAxis(
		const Vector<N, Real>& that);

	template <int N, typename Real>
	TemporaryVector<N, Real> diagonalAxis(
		integer dimension,
		DiagonalAxis diagonal);

}

#include "pastel/geometry/diagonal_axis.hpp"

#endif
