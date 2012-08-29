#ifndef PASTELMATH_CHOLESKY_DECOMPOSITION_HPP
#define PASTELMATH_CHOLESKY_DECOMPOSITION_HPP

#include "pastel/math/cholesky_decomposition.hpp"

#include <cmath>

namespace Pastel
{

	template <typename Real, int N>
	CholeskyDecomposition<Real, N>::CholeskyDecomposition()
		: cholesky_()
		, succeeded_(false)
	{
	}

	template <typename Real, int N>
	CholeskyDecomposition<Real, N>::CholeskyDecomposition(
		const Matrix<Real, N, N>& that)
		: cholesky_(that)
		, succeeded_(false)
	{
		ENSURE2(that.width() == that.height(), that.width(), that.height());

		decompose();
	}

	template <typename Real, int N>
	void CholeskyDecomposition<Real, N>::swap(
		CholeskyDecomposition& that)
	{
		cholesky_.swap(that.cholesky_);
		std::swap(succeeded_, that.succeeded_);
	}

	template <typename Real, int N>
	const Matrix<Real, N, N>& CholeskyDecomposition<Real, N>::lower() const
	{
		return cholesky_;
	}

	template <typename Real, int N>
	bool CholeskyDecomposition<Real, N>::succeeded() const
	{
		return succeeded_;
	}

	template <typename Real, int N>
	bool CholeskyDecomposition<Real, N>::decompose(
			const Matrix<Real, N, N>& that)
	{
		// See "Numerical Recipes: The art of scientific
		// computing", 3rd ed, section 2.9: Cholesky Decomposition.

		ENSURE2(that.width() == that.height(), that.width(), that.height());

		cholesky_ = that;
		return decompose();
	}

	// Private

	template <typename Real, int N>
	bool CholeskyDecomposition<Real, N>::decompose()
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
