#ifndef PASTELMATH_CHOLESKY_DECOMPOSITION_HPP
#define PASTELMATH_CHOLESKY_DECOMPOSITION_HPP

#include "pastel/math/matrix/cholesky_decomposition.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	CholeskyDecomposition<Real>::CholeskyDecomposition(
		integer n)
		: cholesky_(n, n)
		, succeeded_(true)
	{
	}

	template <typename Real>
	CholeskyDecomposition<Real>::CholeskyDecomposition(
		Matrix<Real> that)
		: cholesky_(std::move(that))
		, succeeded_(false)
	{
		ENSURE2(that.width() == that.height(), that.width(), that.height());

		decompose();
	}

	template <typename Real>
	CholeskyDecomposition<Real>::CholeskyDecomposition(
		const CholeskyDecomposition& that)
		: cholesky_(that.cholesky_)
		, succeeded_(that.succeeded_)
	{
	}

	template <typename Real>
	CholeskyDecomposition<Real>::CholeskyDecomposition(
		CholeskyDecomposition&& that)
		: cholesky_(that.n(), that.n())
		, succeeded_(true)
	{
		swap(that);
	}

	template <typename Real>
	CholeskyDecomposition<Real>& CholeskyDecomposition<Real>::operator=(
		CholeskyDecomposition that)
	{
		swap(that);
		return *this;
	}

	template <typename Real>
	void CholeskyDecomposition<Real>::swap(
		CholeskyDecomposition& that)
	{
		cholesky_.swap(that.cholesky_);
		std::swap(succeeded_, that.succeeded_);
	}

	template <typename Real>
	const Matrix<Real>& CholeskyDecomposition<Real>::lower() const
	{
		return cholesky_;
	}

	template <typename Real>
	bool CholeskyDecomposition<Real>::succeeded() const
	{
		return succeeded_;
	}

	template <typename Real>
	bool CholeskyDecomposition<Real>::decompose(
		Matrix<Real> that)
	{
		ENSURE_OP(that.m(), ==, that.n());

		cholesky_ = std::move(that);
		return decompose();
	}

	// Private

	template <typename Real>
	bool CholeskyDecomposition<Real>::decompose()
	{
		// See "Numerical Recipes: The art of scientific
		// computing", 3rd ed, section 2.9: Cholesky Decomposition.

		succeeded_ = false;

		integer n = cholesky_.n();
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

	template <typename Real>
	Real determinant(const CholeskyDecomposition<Real>& that)
	{
		const Matrix<Real>& lower = that.lower();
		integer n = lower.n();

		Real result = 1;
		for (integer i = 0;i < n;++i)
		{

			result *= square(lower(i, i));
		}

		return result;
	}

}

#endif
