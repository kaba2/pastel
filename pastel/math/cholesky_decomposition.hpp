#ifndef PASTEL_CHOLESKY_DECOMPOSITION_HPP
#define PASTEL_CHOLESKY_DECOMPOSITION_HPP

#include "pastel/math/cholesky_decomposition.hpp"

#include <cmath>

namespace Pastel
{

	template <int N, typename Real>
	CholeskyDecomposition<N, Real>::CholeskyDecomposition()
		: cholesky_()
		, succeeded_(false)
	{
	}

	template <int N, typename Real>
	CholeskyDecomposition<N, Real>::CholeskyDecomposition(
		const Matrix<N, N, Real>& that)
		: cholesky_(that)
		, succeeded_(false)
	{
		ENSURE2(that.width() == that.height(), that.width(), that.height());

		decompose();
	}

	template <int N, typename Real>
	void CholeskyDecomposition<N, Real>::swap(
		CholeskyDecomposition& that)
	{
		cholesky_.swap(that.cholesky_);
		std::swap(succeeded_, that.succeeded_);
	}

	template <int N, typename Real>
	const Matrix<N, N, Real>& CholeskyDecomposition<N, Real>::lower() const
	{
		return cholesky_;
	}

	template <int N, typename Real>
	bool CholeskyDecomposition<N, Real>::succeeded() const
	{
		return succeeded_;
	}

	template <int N, typename Real>
	bool CholeskyDecomposition<N, Real>::decompose(
			const Matrix<N, N, Real>& that)
	{
		// See "Numerical Recipes: The art of scientific
		// computing", 3rd ed, section 2.9: Cholesky Decomposition.

		ENSURE2(that.width() == that.height(), that.width(), that.height());

		cholesky_ = that;
		return decompose();
	}

	// Private

	template <int N, typename Real>
	bool CholeskyDecomposition<N, Real>::decompose()
	{
		const integer n = cholesky_.width();

		for (integer i = 0;i < n;++i)
		{
			for (integer j = 0;j < n;++j)
			{
				Real sum = cholesky_(i, j);
				for (integer k = i - 1;k >= 0;--k)
				{
					sum -= cholesky_(i, k) * cholesky_(j, k);
				}
				if (i == j)
				{
					if (sum <= 0)
					{
						// The matrix is not positive definite.
						succeeded_ = false;
						return succeeded_;
					}
					cholesky_(i, i) = std::sqrt(sum);
				}
				else
				{
					cholesky_(j, i) = sum / cholesky_(i, i);
				}
			}
		}

		for (integer i = 0;i < n;++i)
		{
			for (integer j = 0;j < i;++j)
			{
				cholesky_(j, i) = 0;
			}
		}

		succeeded_ = true;
		
		return succeeded_;
	}

}

#endif
