// Description: Tools for working with diagonal vectors
// Detail: A diagonal vector is of the form {-1, 0, 1}^n
// Documentation: pastelgeometry.txt

#ifndef PASTEL_DIAGONAL_AXIS_H
#define PASTEL_DIAGONAL_AXIS_H

namespace Pastel
{
	typedef uint32 DiagonalAxis;

	//! Finds the diagonal vector of maximal variance.
	template <int N, typename Real>
	DiagonalAxis maximalDiagonalVariance(
		const std::vector<Point<N, Real> >& pointSet);

	//! Finds the nearest diagonal vector to a given vector.
	template <int N, typename Real>
	DiagonalAxis nearestDiagonalAxis(
		const Vector<N, Real>& that);

	//! Converts a diagonal vector to a vector.
	template <int N, typename Real>
	TemporaryVector<N, Real> diagonalAxis(
		integer dimension,
		DiagonalAxis diagonal);

}

#include "pastel/geometry/diagonal_axis.hpp"

#endif
