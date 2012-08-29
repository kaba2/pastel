#ifndef PASTELMATH_QR_DECOMPOSITION_TOOLS_HPP
#define PASTELMATH_QR_DECOMPOSITION_TOOLS_HPP

#include "pastel/math/qr_decomposition_tools.h"
#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Real absDeterminant(
		const QrDecomposition<Real, N>& qr)
	{
		return mabs(diagonalProduct(qr.r()));
	}

	template <typename Real, int N, typename Expression>
	Vector<Real, N> solveLinear(
		const QrDecomposition<Real, N>& qr,
		const VectorExpression<Real, N, Expression>& b)
	{
		ENSURE_OP(qr.width(), ==, b.size());

		// x^T QR = b^T
		//
		// Let
		// y^T = x^T Q
		//
		// First solve for y:
		// y^T R = b^T
		//
		// Then solve for x:
		// x^T = y^T Q^T
		
		return solveUpperTriangular(qr.r(), b) * qr.qTransposed();
	}

}

#endif
