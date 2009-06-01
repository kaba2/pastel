#ifndef PASTEL_DIAGONAL_AXIS_H
#define PASTEL_DIAGONAL_AXIS_H

namespace Pastel
{
	typedef uint32 DiagonalAxis;

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
