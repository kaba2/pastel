#ifndef PASTELMATH_CHOLESKY_DECOMPOSITION_HPP
#define PASTELMATH_CHOLESKY_DECOMPOSITION_HPP

#include "pastel/math/matrix/cholesky_decomposition.h"

#include <cmath>

namespace Pastel
{

	template <typename Real, int M, int N>
	CholeskyDecomposition<Real, M, N>::CholeskyDecomposition(
		integer n)
		: cholesky_(n, n)
		, succeeded_(true)
	{
	}

	template <typename Real, int M, int N>
	CholeskyDecomposition<Real, M, N>::CholeskyDecomposition(
		Matrix<Real, M, N> that)
		: cholesky_(std::move(that))
		, succeeded_(false)
	{
		ENSURE_OP(that.cols(), ==, that.rows());

		decompose();
	}

	template <typename Real, int M, int N>
	CholeskyDecomposition<Real, M, N>::CholeskyDecomposition(
		const CholeskyDecomposition& that)
		: cholesky_(that.cholesky_)
		, succeeded_(that.succeeded_)
	{
	}

	template <typename Real, int M, int N>
	CholeskyDecomposition<Real, M, N>::CholeskyDecomposition(
		CholeskyDecomposition&& that)
		: cholesky_(that.n(), that.n())
		, succeeded_(true)
	{
		swap(that);
	}

	template <typename Real, int M, int N>
	CholeskyDecomposition<Real, M, N>& CholeskyDecomposition<Real, M, N>::operator=(
		CholeskyDecomposition that)
	{
		swap(that);
		return *this;
	}

	template <typename Real, int M, int N>
	void CholeskyDecomposition<Real, M, N>::swap(
		CholeskyDecomposition& that)
	{
		cholesky_.swap(that.cholesky_);
		std::swap(succeeded_, that.succeeded_);
	}

	template <typename Real, int M, int N>
	const Matrix<Real, M, N>& CholeskyDecomposition<Real, M, N>::lower() const
	{
		return cholesky_;
	}

	template <typename Real, int M, int N>
	bool CholeskyDecomposition<Real, M, N>::succeeded() const
	{
		return succeeded_;
	}

	template <typename Real, int M, int N>
	bool CholeskyDecomposition<Real, M, N>::decompose(
		const Matrix<Real, M, N>& that)
	{
		ENSURE_OP(that.rows(), ==, that.n());

		cholesky_ = that;
		return decompose();
	}

	// Private

	template <typename Real, int M, int N>
	bool CholeskyDecomposition<Real, M, N>::decompose()
	{
		// See "Numerical Recipes: The art of scientific
		// computing", 3rd ed, section 2.9: Cholesky Decomposition.

		succeeded_ = false;

		integer n = cholesky_.cols();
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

namespace Pastel
{

	template <typename Real, int M, int N>
	Real determinant(const CholeskyDecomposition<Real, M, N>& that)
	{
		const Matrix<Real, M, N>& lower = that.lower();
		integer n = lower.cols();

		Real result = 1;
		for (integer i = 0;i < n;++i)
		{

			result *= square(lower(i, i));
		}

		return result;
	}

}

#endif
