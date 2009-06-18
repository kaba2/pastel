#ifndef PASTEL_QR_DECOMPOSITION_TOOLS_H
#define PASTEL_QR_DECOMPOSITION_TOOLS_H

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

	template <int N, typename Real>
	Real absDeterminant(
		const QrDecomposition<N, Real>& qr);

	//! Solves the linear system QRx = b <=> x^T R^T Q^T = b^T.

	template <int N, typename Real, typename Expression>
	Vector<N, Real> solveLinear(
		const QrDecomposition<N, Real>& qr,
		const VectorExpression<N, Real, Expression>& b);

}

#include "pastel/math/qr_decomposition_tools.hpp"

#endif
