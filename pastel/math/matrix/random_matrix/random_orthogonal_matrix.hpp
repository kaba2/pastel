#ifndef PASTELMATH_RANDOM_ORTHOGONAL_MATRIX_HPP
#define PASTELMATH_RANDOM_ORTHOGONAL_MATRIX_HPP

#include "pastel/math/matrix/random_matrix/random_orthogonal_matrix.h"
#include "pastel/math/matrix/qr_decomposition.h"

#include "pastel/sys/random/random_gaussian.h"
#include "pastel/sys/math/sign.h"

namespace Pastel
{

	template <typename Real>
	Matrix<Real> randomOrthogonal(
		integer n, integer orientation)
	{
		ENSURE_OP(n, >=, 0);

		// "How to Generate Random Matrices
		// from the Classical Compact Groups",
		// Francesco Mezzadri,
		// Notices of the AMS,
		// Volume 54, Number 5, 2007.

		Matrix<Real> q(n, n);

		for (auto& value : q)
		{
			value = randomGaussian<Real>();
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
				for (integer i = 0;i < n;++i)
				{
					q(i, 0) = -q(i, 0);
				}
			}
		}

		return q;
	}

}

#endif