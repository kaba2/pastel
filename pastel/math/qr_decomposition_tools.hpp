#ifndef PASTEL_QR_DECOMPOSITION_TOOLS_HPP
#define PASTEL_QR_DECOMPOSITION_TOOLS_HPP

#include "pastel/math/qr_decomposition_tools.h"
#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Real absDeterminant(
		const QrDecomposition<N, Real>& qr)
	{
		return mabs(diagonalProduct(qr.r()));
	}

	template <int N, typename Real, typename Expression>
	Vector<N, Real> solveLinear(
		const QrDecomposition<N, Real>& qr,
		const VectorExpression<N, Real, Expression>& b)
	{
		ENSURE2(qr.width() == b.size(),
			qr.width(), b.size());

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
