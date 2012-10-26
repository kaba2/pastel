#ifndef PASTELMATH_RANDOM_ORTHOGONAL_MATRIX_HPP
#define PASTELMATH_RANDOM_ORTHOGONAL_MATRIX_HPP

#include "pastel/math/random_orthogonal_matrix.h"
#include "pastel/math/qr_decomposition.h"

#include "pastel/sys/random_gaussian.h"

namespace Pastel
{

	template <typename Real>
	void setRandomOrthogonal(
		Matrix<Real>& result)
	{
		// See "How to Generate Random Matrices
		// from the Classical Compact Groups",
		// Francesco Mezzadri,
		// Notices of the AMS,
		// Volume 54, Number 5, 2007.

		ENSURE_OP(result.width(), ==, result.height());

		const integer n = result.width();
		
		typedef typename Matrix<Real>::Iterator 
			Iterator;

		Iterator iter = result.begin();
		const Iterator iterEnd = result.end();
		while(iter != iterEnd)
		{
			*iter = randomGaussian<Real>();
			++iter;
		}

		QrDecomposition<Real> qr(result);

		result = qr.qTransposed();

		for (integer i = 0;i < n;++i)
		{
			if (qr.r()(i, i)  < 0)
			{
				result.column(i) = -result.column(i);
			}
		}
	}

	template <typename Real>
	Matrix<Real> randomOrthogonalMatrix(
		integer n)
	{
		ENSURE_OP(n, >=, 0);

		Matrix<Real> q(n, n);
		setRandomOrthogonal(q);
		return q;
	}

}

#endif
