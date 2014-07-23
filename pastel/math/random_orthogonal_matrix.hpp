#ifndef PASTELMATH_RANDOM_ORTHOGONAL_MATRIX_HPP
#define PASTELMATH_RANDOM_ORTHOGONAL_MATRIX_HPP

#include "pastel/math/random_orthogonal_matrix.h"
#include "pastel/math/qr_decomposition.h"

#include "pastel/sys/random_gaussian.h"
#include "pastel/sys/sign.h"

namespace Pastel
{

	template <typename Real>
	void setRandomOrthogonal(
		Matrix<Real>& q, integer orientation)
	{
		// See "How to Generate Random Matrices
		// from the Classical Compact Groups",
		// Francesco Mezzadri,
		// Notices of the AMS,
		// Volume 54, Number 5, 2007.

		ENSURE_OP(q.m(), ==, q.n());

		integer m = q.m();
		const integer n = q.n();
		
		typedef typename Matrix<Real>::Iterator 
			Iterator;

		Iterator iter = q.begin();
		const Iterator iterEnd = q.end();
		while(iter != iterEnd)
		{

			*iter = randomGaussian<Real>();
			++iter;
		}

		QrDecomposition<Real> qr(q);

		q = qr.qTransposed();

		for (integer i = 0;i < n;++i)
		{
			if (qr.r()(i, i)  < 0)
			{
				q.column(i) = -q.column(i);
			}
		}

		if (orientation != 0)
		{
			if (sign(determinant(q)) != sign(orientation))
			{
				// While the following gives the correct
				// determinant, and preserves orthogonality,
				// I don't have a proof that this will preserve
				// the uniform distribution property. This may
				// or may not be correct.
				for (integer i = 0;i < m;++i)
				{
					q(i, 0) = -q(i, 0);
				}
			}
		}
	}

	template <typename Real>
	Matrix<Real> randomOrthogonalMatrix(
		integer n, integer orientation)
	{
		ENSURE_OP(n, >=, 0);

		Matrix<Real> q(n, n);
		setRandomOrthogonal(q, orientation);
		return q;
	}

}

#endif
