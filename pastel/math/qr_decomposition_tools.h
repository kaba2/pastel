// Description: Algorithms for QrDecomposition class

#ifndef PASTELMATH_QR_DECOMPOSITION_TOOLS_H
#define PASTELMATH_QR_DECOMPOSITION_TOOLS_H

#include "pastel/math/qr_decomposition.h"

namespace Pastel
{

	//! Computes the absolute determinant from a qr decomposition.
	/*!
	The determinant of the orthogonal matrix is either -1 or 1.
	It would be costly to find out which. However, the
	absolute value of the determinant can be found quickly
	from the upper triangular matrix.
	*/

	template <typename Real, int N>
	Real absDeterminant(
		const QrDecomposition<Real, N>& qr);

	//! Solves the linear system QRx = b <=> x^T R^T Q^T = b^T.

	template <typename Real, int N, typename Expression>
	Vector<Real, N> solveLinear(
		const QrDecomposition<Real, N>& qr,
		const VectorExpression<Real, N, Expression>& b);

}

#include "pastel/math/qr_decomposition_tools.hpp"

#endif
